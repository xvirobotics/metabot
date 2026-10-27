import { modelName, resolveModelPolicy } from '../model-policy.js';
import type { BotConfigBase } from '../../config.js';
import type { Logger } from '../../utils/logger.js';
import type { ApiContext, ExecutionHandle, ExecutorOptions, SDKMessage } from '../claude/executor.js';
import { buildMetaBotApiPromptContext } from '../prompt-context.js';
import {
  KimiDaemonClient,
  type KimiPendingQuestion,
  type KimiPermissionMode,
  type KimiSessionSnapshot,
  type KimiSessionStatus,
  type KimiSubagentTask,
  type KimiWireMessage,
} from './daemon-client.js';

const SNAPSHOT_POLL_MS = 350;

export interface KimiClientLike {
  resolveModel(configured?: string): Promise<{ id: string; displayName: string }>;
  openSession(cwd: string, sessionId?: string, model?: string): Promise<{ id: string }>;
  getSnapshot(sessionId: string): Promise<KimiSessionSnapshot>;
  getStatus(sessionId: string): Promise<KimiSessionStatus>;
  getGoal(sessionId: string): Promise<Record<string, unknown> | null>;
  submitPrompt(
    sessionId: string,
    text: string,
    options?: { model?: string; thinking?: string; goalObjective?: string; permissionMode?: KimiPermissionMode },
  ): Promise<{ prompt_id: string; status: 'running' | 'queued' | 'blocked' }>;
  steer(sessionId: string, text: string, options?: { model?: string; thinking?: string }): Promise<void>;
  abortSession(sessionId: string): Promise<void>;
  setGoal(sessionId: string, objective: string): Promise<void>;
  controlGoal(sessionId: string, action: 'pause' | 'resume' | 'cancel'): Promise<void>;
  approve(sessionId: string, approvalId: string): Promise<void>;
  respondQuestion(sessionId: string, question: KimiPendingQuestion, answers: Record<string, string>): Promise<void>;
}

interface ActiveKimiTurn {
  sessionId?: string;
  model?: string;
  thinking?: string;
  pendingSteers: number;
  ready: Promise<void>;
  resolveReady: () => void;
  rejectReady: (error: unknown) => void;
}

interface KimiTurnState {
  sessionId: string;
  model: string;
  contextWindow: number;
  startTime: number;
  baselineMessageIds: Set<string>;
  emittedText: string;
  toolInputs: Map<string, string>;
  completedTools: Set<string>;
  pendingQuestions: Map<string, KimiPendingQuestion>;
  emittedQuestions: Set<string>;
  approvedRequests: Set<string>;
  subagentStates: Map<string, string>;
  lastSnapshot?: KimiSessionSnapshot;
}

/**
 * Kimi Code 0.27 executor backed by the official local Server API.
 *
 * This is the same frontend contract used by Kimi's own web UI. It gives the
 * Feishu bridge durable Sessions, atomic live snapshots, native prompt queue
 * steering, questions, cancellation, usage, and subagent activity without a
 * bespoke runner or a long-lived per-chat process.
 */
export class KimiExecutor {
  private readonly client: KimiClientLike;
  private readonly activeTurns = new Map<string, ActiveKimiTurn>();

  constructor(
    private readonly config: BotConfigBase,
    private readonly logger: Logger,
    client?: KimiClientLike,
  ) {
    this.client =
      client ??
      new KimiDaemonClient({
        executable: config.kimi?.executable,
        serverUrl: config.kimi?.serverUrl,
        apiKey: config.kimi?.apiKey,
      });
  }

  startExecution(options: ExecutorOptions): ExecutionHandle {
    const { prompt, cwd, sessionId, abortController, outputsDir, apiContext } = options;
    const turnKey = apiContext?.chatId ?? sessionId ?? `kimi:${Date.now()}`;
    let resolveReady!: () => void;
    let rejectReady!: (error: unknown) => void;
    const ready = new Promise<void>((resolve, reject) => {
      resolveReady = resolve;
      rejectReady = reject;
    });
    // A failed launch may have no steering waiter. Mark the rejection handled
    // while preserving it for callers that do await active.ready.
    void ready.catch(() => undefined);
    const active: ActiveKimiTurn = {
      pendingSteers: 0,
      ready,
      resolveReady,
      rejectReady,
    };
    this.activeTurns.set(turnKey, active);

    const abort = () => {
      const sid = active.sessionId;
      if (sid) {
        void this.client
          .abortSession(sid)
          .catch((error) =>
            this.logger.warn({ error, sessionId: sid, engine: 'kimi' }, 'Failed to abort Kimi session'),
          );
      }
    };
    if (abortController.signal.aborted) abort();
    else abortController.signal.addEventListener('abort', abort, { once: true });

    const client = this.client;
    const config = this.config;
    const logger = this.logger;
    const activeTurns = this.activeTurns;
    const buildPromptWithContext = this.buildPromptWithContext.bind(this);
    const resolveQuestion = this.resolveQuestion.bind(this);
    let turnState: KimiTurnState | undefined;
    async function* stream(): AsyncGenerator<SDKMessage> {
      try {
        const model = await client.resolveModel(modelName(resolveModelPolicy(options.modelPolicy, options.model, config.kimi?.model)));
        const session = await client.openSession(cwd, sessionId, model.id);
        active.sessionId = session.id;
        active.model = model.id;
        active.thinking = config.kimi?.thinking === undefined ? undefined : config.kimi.thinking ? 'high' : 'off';

        const initial = await client.getSnapshot(session.id);
        turnState = {
          sessionId: session.id,
          model: model.displayName,
          contextWindow: config.kimi?.contextWindow ?? 1_048_576,
          startTime: Date.now(),
          baselineMessageIds: new Set(initial.messages.items.map((message) => message.id)),
          emittedText: '',
          toolInputs: new Map(),
          completedTools: new Set(),
          pendingQuestions: new Map(),
          emittedQuestions: new Set(),
          approvedRequests: new Set(),
          subagentStates: new Map(),
        };
        const state = turnState;

        yield { type: 'system', subtype: 'init', session_id: session.id };

        if (abortController.signal.aborted) {
          await client.abortSession(session.id).catch(() => undefined);
          throw abortError();
        }

        const goal = parseGoalCommand(prompt);
        if (goal.kind === 'control') {
          await client.controlGoal(session.id, goal.action);
          const result = goal.action === 'cancel' ? 'Goal cancelled.' : `Goal ${goal.action}d.`;
          active.resolveReady();
          yield localResult(session.id, state, result);
          return;
        }
        if (goal.kind === 'status') {
          const current = await client.getGoal(session.id);
          const text = current ? formatGoalStatus(current) : 'No active Kimi goal.';
          active.resolveReady();
          yield localResult(session.id, state, text);
          return;
        }
        if (goal.kind === 'start') await client.setGoal(session.id, goal.objective);

        const fullPrompt = buildPromptWithContext(
          goal.kind === 'start' ? goal.objective : prompt,
          outputsDir,
          apiContext,
        );
        const submitted = await client.submitPrompt(session.id, fullPrompt, {
          model: model.id,
          thinking: active.thinking,
          permissionMode: config.kimi?.permissionMode ?? 'auto',
        });
        if (submitted.status === 'blocked') throw new Error('Kimi Code blocked the prompt before execution');
        active.resolveReady();

        for (;;) {
          const snapshot = await client.getSnapshot(session.id);
          state.lastSnapshot = snapshot;

          if (snapshot.pending_approvals.length > 0) {
            if (config.kimi?.permissionMode === 'yolo') {
              for (const approval of snapshot.pending_approvals) {
                if (state.approvedRequests.has(approval.approval_id)) continue;
                state.approvedRequests.add(approval.approval_id);
                await client.approve(session.id, approval.approval_id);
              }
            } else {
              await client.abortSession(session.id).catch(() => undefined);
              throw new Error(
                'Kimi Code requested a tool approval that MetaBot did not auto-approve. ' +
                'Run the action directly in Kimi Code, or explicitly set kimi.permissionMode to "yolo" only for a trusted workspace.',
              );
            }
          }

          for (const message of translateSnapshot(snapshot, state)) yield message;

          if (
            !snapshot.session.busy &&
            snapshot.in_flight_turn === null &&
            snapshot.pending_questions.length === 0 &&
            active.pendingSteers === 0
          ) {
            break;
          }
          if (abortController.signal.aborted && !snapshot.session.busy) break;
          await sleep(SNAPSHOT_POLL_MS);
        }

        const status = await client.getStatus(session.id).catch(() => undefined);
        if (status) {
          state.contextWindow = status.max_context_tokens || state.contextWindow;
          if (status.model) state.model = status.model;
        }
        yield buildResult(state, status, abortController.signal.aborted);
      } catch (error) {
        active.rejectReady(error);
        if (abortController.signal.aborted || (error instanceof Error && error.name === 'AbortError')) {
          yield {
            type: 'result',
            subtype: 'error_cancelled',
            session_id: turnState?.sessionId ?? sessionId,
            duration_ms: turnState ? Date.now() - turnState.startTime : 0,
            is_error: true,
            errors: ['Aborted by user'],
          };
        } else {
          logger.error({ error, engine: 'kimi', cwd }, 'Kimi Code execution failed');
          yield {
            type: 'result',
            subtype: 'error_during_execution',
            session_id: turnState?.sessionId ?? sessionId,
            duration_ms: turnState ? Date.now() - turnState.startTime : 0,
            is_error: true,
            errors: [error instanceof Error ? error.message : String(error)],
          };
        }
      } finally {
        abortController.signal.removeEventListener('abort', abort);
        if (activeTurns.get(turnKey) === active) activeTurns.delete(turnKey);
      }
    }

    return {
      stream: stream(),
      sendAnswer: (toolUseId: string, _sid: string, answerText: string) => {
        const question = turnState?.pendingQuestions.get(toolUseId);
        if (!question || !active.sessionId) return;
        const answers = Object.fromEntries(question.questions.map((item) => [item.question, answerText]));
        void resolveQuestion(active.sessionId, question, answers);
      },
      resolveQuestion: (toolUseId: string, answers: Record<string, string>) => {
        const question = turnState?.pendingQuestions.get(toolUseId);
        if (!question || !active.sessionId) return;
        void resolveQuestion(active.sessionId, question, answers);
      },
      finish: () => undefined,
    };
  }

  async *execute(options: ExecutorOptions): AsyncGenerator<SDKMessage> {
    const handle = this.startExecution(options);
    try {
      for await (const message of handle.stream) yield message;
    } finally {
      handle.finish();
    }
  }

  canSteer(chatId: string): boolean {
    return this.activeTurns.has(chatId);
  }

  async steer(chatId: string, prompt: string): Promise<'steered' | 'no-active-turn'> {
    const active = this.activeTurns.get(chatId);
    if (!active) return 'no-active-turn';
    active.pendingSteers += 1;
    try {
      await active.ready;
      if (!active.sessionId) return 'no-active-turn';
      await this.client.steer(active.sessionId, prompt, { model: active.model, thinking: active.thinking });
      return 'steered';
    } finally {
      active.pendingSteers -= 1;
    }
  }

  private async resolveQuestion(
    sessionId: string,
    question: KimiPendingQuestion,
    answers: Record<string, string>,
  ): Promise<void> {
    try {
      await this.client.respondQuestion(sessionId, question, answers);
      this.logger.info({ engine: 'kimi', questionId: question.question_id }, 'Resolved Kimi question from Feishu');
    } catch (error) {
      this.logger.warn({ error, engine: 'kimi', questionId: question.question_id }, 'Failed to resolve Kimi question');
    }
  }

  private buildPromptWithContext(
    prompt: string,
    outputsDir: string | undefined,
    apiContext: ApiContext | undefined,
  ): string {
    const sections: string[] = [];
    if (outputsDir) {
      sections.push(
        `## Output Files\nWhen producing output files for the user, copy them to: ${outputsDir}\nMetaBot will automatically send files placed there.`,
      );
    }
    if (apiContext) {
      sections.push(buildMetaBotApiPromptContext(apiContext));
      if (apiContext.teamContext) sections.push(apiContext.teamContext);
      if (apiContext.groupMembers?.length) {
        const peers = apiContext.groupMembers.filter((name) => name !== apiContext.botName);
        if (peers.length) sections.push(`## Agent Organization\nOther Agents in this group: ${peers.join(', ')}.`);
      }
    }
    return sections.length > 0 ? `${prompt}\n\n---\n\n${sections.join('\n\n')}` : prompt;
  }
}

function translateSnapshot(snapshot: KimiSessionSnapshot, state: KimiTurnState): SDKMessage[] {
  const messages: SDKMessage[] = [];
  const currentMessages = snapshot.messages.items.filter((message) => !state.baselineMessageIds.has(message.id));
  const fullText = collectAssistantText(currentMessages, snapshot.in_flight_turn?.assistant_text);
  if (fullText !== state.emittedText) {
    if (fullText.startsWith(state.emittedText)) {
      const delta = fullText.slice(state.emittedText.length);
      if (delta) {
        messages.push({
          type: 'stream_event',
          session_id: state.sessionId,
          event: { type: 'content_block_delta', delta: { type: 'text_delta', text: delta } },
        });
      }
    } else {
      messages.push({
        type: 'assistant',
        session_id: state.sessionId,
        message: { content: [{ type: 'text', text: fullText }] },
      });
    }
    state.emittedText = fullText;
  }

  const tools = collectTools(currentMessages, snapshot);
  for (const [id, tool] of tools) {
    const signature = stableValue(tool.input);
    if (state.toolInputs.get(id) !== signature) {
      state.toolInputs.set(id, signature);
      messages.push({
        type: 'assistant',
        session_id: state.sessionId,
        message: {
          content: [{ type: 'tool_use', id, name: normalizeKimiToolName(tool.name), input: tool.input ?? {} }],
        },
      });
    }
    if (tool.completed && !state.completedTools.has(id)) {
      state.completedTools.add(id);
      messages.push({
        type: 'user',
        session_id: state.sessionId,
        message: { content: [{ type: 'tool_result', id, text: formatToolOutput(tool.output) }] },
      });
    }
  }

  for (const question of snapshot.pending_questions) {
    const toolUseId = question.tool_call_id ?? question.question_id;
    state.pendingQuestions.set(toolUseId, question);
    if (state.emittedQuestions.has(question.question_id)) continue;
    state.emittedQuestions.add(question.question_id);
    messages.push({
      type: 'assistant',
      session_id: state.sessionId,
      message: {
        content: [
          {
            type: 'tool_use',
            id: toolUseId,
            name: 'AskUserQuestion',
            input: {
              questions: question.questions.map((item) => ({
                question: item.question,
                header: item.header ?? '',
                options: item.options.map((option) => ({
                  label: option.label,
                  description: option.description ?? '',
                })),
                multiSelect: item.multi_select === true,
              })),
            },
          },
        ],
      },
    });
  }

  for (const task of snapshot.subagents ?? []) messages.push(...translateSubagent(task, state));
  return messages;
}

function collectAssistantText(messages: KimiWireMessage[], inFlightText?: string): string {
  const durable = messages
    .filter((message) => message.role === 'assistant')
    .flatMap((message) => message.content)
    .flatMap((content) => (content.type === 'text' && typeof content.text === 'string' ? [content.text] : []))
    .join('');
  return durable + (inFlightText ?? '');
}

function collectTools(
  messages: KimiWireMessage[],
  snapshot: KimiSessionSnapshot,
): Map<string, { name: string; input?: unknown; output?: unknown; completed: boolean }> {
  const tools = new Map<string, { name: string; input?: unknown; output?: unknown; completed: boolean }>();
  for (const message of messages) {
    for (const content of message.content) {
      if (content.type === 'tool_use' && typeof content.tool_call_id === 'string') {
        tools.set(content.tool_call_id, {
          name: typeof content.tool_name === 'string' ? content.tool_name : 'Tool',
          input: content.input,
          completed: false,
        });
      } else if (content.type === 'tool_result' && typeof content.tool_call_id === 'string') {
        const previous = tools.get(content.tool_call_id);
        tools.set(content.tool_call_id, {
          name: previous?.name ?? 'Tool',
          input: previous?.input,
          output: content.output,
          completed: true,
        });
      }
    }
  }
  for (const tool of snapshot.in_flight_turn?.running_tools ?? []) {
    const previous = tools.get(tool.tool_call_id);
    tools.set(tool.tool_call_id, {
      name: tool.name,
      input: tool.args ?? previous?.input,
      output: previous?.output,
      completed: previous?.completed ?? false,
    });
  }
  return tools;
}

function translateSubagent(task: KimiSubagentTask, state: KimiTurnState): SDKMessage[] {
  const signature = `${task.status}:${task.subagent_phase ?? ''}:${task.output_preview ?? ''}`;
  if (state.subagentStates.get(task.id) === signature) return [];
  const previous = state.subagentStates.get(task.id);
  state.subagentStates.set(task.id, signature);
  const terminal = task.status === 'completed' || task.status === 'failed' || task.status === 'cancelled';
  return [
    {
      type: 'system',
      subtype: terminal ? 'task_notification' : previous ? 'task_progress' : 'task_started',
      session_id: state.sessionId,
      task_id: task.id,
      description: task.description || task.subagent_type || 'Kimi subagent',
      status: terminal ? (task.status === 'completed' ? 'completed' : 'failed') : 'running',
      summary: task.output_preview,
    } as SDKMessage,
  ];
}

function buildResult(state: KimiTurnState, status: KimiSessionStatus | undefined, aborted: boolean): SDKMessage {
  const snapshot = state.lastSnapshot;
  const failed = snapshot?.session.last_turn_reason === 'failed';
  const cancelled = aborted || snapshot?.session.last_turn_reason === 'cancelled';
  // Kimi's durable session usage can lag the just-completed turn by one
  // snapshot. The live status endpoint is authoritative for current context.
  const usage = snapshot?.session.usage;
  const outputTokens = usage?.output_tokens ?? 0;
  const inputTokens =
    usage?.input_tokens ?? Math.max(0, (status?.context_tokens || usage?.context_tokens || 0) - outputTokens);
  return {
    type: 'result',
    subtype: cancelled ? 'error_cancelled' : failed ? 'error_during_execution' : 'success',
    session_id: state.sessionId,
    duration_ms: Date.now() - state.startTime,
    result: state.emittedText,
    is_error: failed || cancelled,
    num_turns: snapshot?.session.usage?.turn_count,
    modelUsage: {
      [state.model]: {
        contextWindow: status?.max_context_tokens || state.contextWindow,
        inputTokens,
        outputTokens,
        costUSD: snapshot?.session.usage?.total_cost_usd ?? 0,
      },
    },
    ...(failed ? { errors: ['Kimi Code turn failed'] } : {}),
    ...(cancelled ? { errors: ['Aborted by user'] } : {}),
  };
}

function localResult(sessionId: string, state: KimiTurnState, text: string): SDKMessage {
  state.emittedText = text;
  return {
    type: 'result',
    subtype: 'success',
    session_id: sessionId,
    duration_ms: Date.now() - state.startTime,
    result: text,
    is_error: false,
    modelUsage: {
      [state.model]: { contextWindow: state.contextWindow, inputTokens: 0, outputTokens: 0, costUSD: 0 },
    },
  };
}

function parseGoalCommand(
  prompt: string,
):
  | { kind: 'none' }
  | { kind: 'start'; objective: string }
  | { kind: 'control'; action: 'pause' | 'resume' | 'cancel' }
  | { kind: 'status' } {
  const match = prompt.trim().match(/^\/goal(?:\s+(.*))?$/i);
  if (!match) return { kind: 'none' };
  const value = match[1]?.trim() ?? '';
  if (!value || value.toLowerCase() === 'status') return { kind: 'status' };
  const normalized = value.toLowerCase();
  if (normalized === 'pause') return { kind: 'control', action: 'pause' };
  if (normalized === 'resume') return { kind: 'control', action: 'resume' };
  if (['clear', 'stop', 'off', 'reset', 'none', 'cancel'].includes(normalized)) {
    return { kind: 'control', action: 'cancel' };
  }
  return { kind: 'start', objective: value };
}

function formatGoalStatus(goal: Record<string, unknown>): string {
  const objective = typeof goal.objective === 'string' ? goal.objective : 'Kimi goal';
  const status = typeof goal.status === 'string' ? goal.status : 'active';
  const turns = typeof goal.turnsUsed === 'number' ? ` · ${goal.turnsUsed} turns` : '';
  return `Goal ${status}: ${objective}${turns}`;
}

function normalizeKimiToolName(name: string): string {
  const clean = name.replace(/_\d+$/, '');
  const names: Record<string, string> = {
    ReadFile: 'Read',
    WriteFile: 'Write',
    StrReplaceFile: 'Edit',
    ReplaceFile: 'Edit',
    Shell: 'Bash',
    RunShellCommand: 'Bash',
    FindFiles: 'Glob',
    SearchFiles: 'Grep',
    SearchWeb: 'WebSearch',
    FetchURL: 'WebFetch',
  };
  return names[clean] ?? clean;
}

function stableValue(value: unknown): string {
  try {
    return JSON.stringify(value ?? {});
  } catch {
    return String(value);
  }
}

function formatToolOutput(value: unknown): string {
  if (typeof value === 'string') return value;
  try {
    return JSON.stringify(value);
  } catch {
    return String(value ?? '');
  }
}

function abortError(): Error {
  const error = new Error('Aborted by user');
  error.name = 'AbortError';
  return error;
}

function sleep(ms: number): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms));
}
