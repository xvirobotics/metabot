import * as fs from 'node:fs';
import * as os from 'node:os';
import * as path from 'node:path';
import { afterEach, describe, expect, it, vi } from 'vitest';
import { MessageBridge } from '../src/bridge/message-bridge.js';
import { ClaudeExecutor, type ExecutionHandle, type ExecutorOptions, type SDKMessage } from '../src/engines/claude/executor.js';
import { CodexExecutor } from '../src/engines/codex/executor.js';
import type { BotConfigBase } from '../src/config.js';
vi.mock('../src/engines/claude/resolve-claude.js', () => ({ resolveClaudePath: () => 'claude-test' }));

const rejection: SDKMessage = { type: 'result', subtype: 'error_during_execution', is_error: true, errors: ['API Error: 400 unknown provider for model retired'], total_cost_usd: 0.1 };
const success: SDKMessage = { type: 'result', subtype: 'success', result: 'recovered', total_cost_usd: 0.2, session_id: 'recovered-session' };
const question: SDKMessage = { type: 'assistant', message: { content: [{ type: 'tool_use', name: 'AskUserQuestion', id: 'question-1', input: { questions: [{ question: 'Proceed?', header: 'Next', options: [], multiSelect: false }] } }] } };
const logger = { debug: vi.fn(), info: vi.fn(), warn: vi.fn(), error: vi.fn(), child: () => logger };
const cleanups: Array<() => void> = [];
afterEach(() => { cleanups.splice(0).reverse().forEach(cleanup => cleanup()); vi.restoreAllMocks(); vi.unstubAllEnvs(); });

function handle(messages: SDKMessage[] = [], before?: () => void): ExecutionHandle {
  return {
    stream: (async function* () { before?.(); yield* messages; })(),
    finish: vi.fn(), sendAnswer: vi.fn(), resolveQuestion: vi.fn(),
  };
}

function fixture() {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), 'pr419-'));
  vi.stubEnv('SESSION_STORE_DIR', dir);
  const config: BotConfigBase = {
    name: 'fallback-test', engine: 'claude', persistentExecutor: { enabled: false },
    claude: { defaultWorkingDirectory: dir, outputsBaseDir: path.join(dir, 'outputs'), downloadsDir: dir, model: 'bot-retired', maxTurns: undefined, maxBudgetUsd: undefined, apiKey: undefined, backend: 'sdk' },
  };
  const sender = {
    sendCard: vi.fn(async () => 'main-card'), updateCard: vi.fn(async () => true),
    sendQuestionCard: vi.fn(async () => 'question-card'), updateQuestionCard: vi.fn(async () => true),
    sendTextNotice: vi.fn(async () => {}), sendText: vi.fn(async () => {}),
    sendImageFile: vi.fn(async () => true), sendLocalFile: vi.fn(async () => true),
    downloadImage: vi.fn(async () => false), downloadFile: vi.fn(async () => false),
  };
  const bridge = new MessageBridge(config, logger, sender);
  const start = vi.spyOn(ClaudeExecutor.prototype, 'startExecution');
  const internal = bridge as any;
  const output = vi.spyOn(internal.outputHandler, 'sendOutputFiles').mockResolvedValue(undefined);
  const record = vi.spyOn(internal, 'recordSession');
  const audit = vi.spyOn(internal.audit, 'log');
  const cost = vi.spyOn(bridge.costTracker, 'record');
  const goal = vi.spyOn(internal.codexCommands, 'maybeScheduleGoalContinuation').mockImplementation(() => {});
  const observe = vi.spyOn(internal.shadowMode, 'observeFinal').mockResolvedValue(undefined);
  const activity = vi.fn(); bridge.onActivityEvent = activity;
  const manager = bridge.getSessionManager();
  manager.setSessionModel('chat', 'retired', 'claude');
  cleanups.push(() => { bridge.destroy(); fs.rmSync(dir, { recursive: true, force: true }); });
  const chat = () => internal.startQuery({ chatId: 'chat', chatType: 'private', userId: 'user', text: 'do work', messageId: 'input' });
  return { bridge, internal, config, sender, start, output, record, audit, cost, goal, observe, activity, manager, chat };
}

function rejectFirst(ctx: ReturnType<typeof fixture>, mode: 'acquisition' | 'throw' | 'result', fallback: ExecutionHandle) {
  if (mode === 'acquisition') ctx.start.mockImplementationOnce(() => { throw new Error('API Error: 400 unknown provider for model retired'); });
  else ctx.start.mockReturnValueOnce(mode === 'result' ? handle([rejection]) : handle([], () => { throw new Error('API Error: 400 unknown provider for model retired'); }));
  ctx.start.mockReturnValueOnce(fallback);
}

function expectFinalized(ctx: ReturnType<typeof fixture>, api: boolean, ok = true) {
  expect(ctx.output).toHaveBeenCalledTimes(1);
  expect(ctx.record).toHaveBeenCalledTimes(1);
  expect(ctx.cost).toHaveBeenCalledTimes(1);
  expect(ctx.cost).toHaveBeenCalledWith(expect.objectContaining({ success: ok }));
  expect(ctx.activity.mock.calls.filter(([event]) => event.type !== 'task_started')).toHaveLength(1);
  expect(ctx.audit.mock.calls.filter(([event]) => ['task_complete', 'task_error', 'api_task_complete'].includes((event as any).event))).toHaveLength(1);
  expect(ctx.bridge.isBusy('chat')).toBe(false);
  if (!api) { expect(ctx.goal).toHaveBeenCalledTimes(1); expect(ctx.observe).toHaveBeenCalledTimes(ok ? 1 : 0); }
}

describe('unknown-model canonical lifecycle', () => {
  it.each(['acquisition', 'throw', 'result'] as const)('API %s fallback preserves limits, questions, callbacks and exactly one finalization', async mode => {
    const ctx = fixture(); ctx.config.persistentExecutor = { enabled: true };
    const fallback = handle([question, success]); rejectFirst(ctx, mode, fallback);
    const onQuestion = vi.fn(async () => JSON.stringify({ answers: { 'Proceed?': 'yes' } }));
    const onUpdate = vi.fn();
    const result = await ctx.bridge.executeApiTask({ chatId: 'chat', prompt: 'do work', maxTurns: 1, allowedTools: [], sendCards: true, onQuestion, onUpdate, groupId: 'group', groupMembers: ['worker'] });
    expect(result.success).toBe(true);
    expect(onQuestion).toHaveBeenCalledTimes(1);
    expect(fallback.resolveQuestion).toHaveBeenCalledWith('question-1', { 'Proceed?': 'yes' });
    expect(ctx.start).toHaveBeenCalledTimes(2);
    const [first, second] = ctx.start.mock.calls.map(([options]) => options);
    expect(second).toMatchObject({ prompt: first.prompt, cwd: first.cwd, abortController: first.abortController, outputsDir: first.outputsDir, apiContext: first.apiContext, maxTurns: 1, allowedTools: [], modelPolicy: { kind: 'endpoint-default' } });
    expect(onUpdate.mock.calls.filter(([, , final]) => final)).toHaveLength(1);
    expect(onUpdate.mock.calls.at(-1)?.[0]).toMatchObject({ status: 'complete', responseText: 'recovered' });
    expect(ctx.manager.getSession('chat').sessionId).toBe('recovered-session');
    expectFinalized(ctx, true);
  });

  it.each(['acquisition', 'throw', 'result'] as const)('chat %s fallback uses the normal output, accounting and goal tail', async mode => {
    const ctx = fixture(); const fallback = handle([question, success]); rejectFirst(ctx, mode, fallback);
    await ctx.chat();
    expect(ctx.sender.sendQuestionCard).toHaveBeenCalledTimes(1);
    expect(ctx.sender.updateCard.mock.calls.filter(([, state]) => ['complete', 'error'].includes(state.status))).toHaveLength(1);
    expectFinalized(ctx, false);
  });

  it.each([
    { surface: 'API', reasoningEffort: undefined, serviceTier: undefined },
    { surface: 'chat', reasoningEffort: undefined, serviceTier: undefined },
    { surface: 'API', reasoningEffort: 'low', serviceTier: 'flex' },
    { surface: 'chat', reasoningEffort: 'low', serviceTier: 'flex' },
  ] satisfies Array<{ surface: 'API' | 'chat' } & Pick<ExecutorOptions, 'reasoningEffort' | 'serviceTier'>>)(
    '$surface keeps captured effort=$reasoningEffort and tier=$serviceTier through fallback until the next turn',
    async ({ surface, reasoningEffort, serviceTier }) => {
      const ctx = fixture();
      ctx.config.engine = 'codex';
      ctx.manager.setSessionEngine('chat', 'codex');
      ctx.manager.setSessionModel('chat', 'retired', 'codex');
      ctx.manager.setReasoningEffort('chat', reasoningEffort);
      ctx.manager.setServiceTier('chat', serviceTier);
      const start = vi.spyOn(CodexExecutor.prototype, 'startExecution')
        .mockReturnValueOnce(handle([rejection], () => {
          ctx.manager.setReasoningEffort('chat', 'high');
          ctx.manager.setServiceTier('chat', 'fast');
        }))
        .mockReturnValueOnce(handle([success]))
        .mockReturnValueOnce(handle([success]));
      const run = async () => {
        if (surface === 'chat') await ctx.chat();
        else expect((await ctx.bridge.executeApiTask({ chatId: 'chat', prompt: 'work' })).success).toBe(true);
      };

      await run();
      expect(start).toHaveBeenCalledTimes(2);
      expect(start.mock.calls.map(([options]) => ({
        reasoningEffort: options.reasoningEffort, serviceTier: options.serviceTier,
      }))).toStrictEqual([
        { reasoningEffort, serviceTier },
        { reasoningEffort, serviceTier },
      ]);
      expect(start.mock.calls[1][0].modelPolicy).toEqual({ kind: 'endpoint-default' });
      expect(ctx.manager.getSession('chat')).toMatchObject({ reasoningEffort: 'high', serviceTier: 'fast' });

      await run();
      expect(start).toHaveBeenCalledTimes(3);
      expect(start.mock.calls[2][0]).toMatchObject({ reasoningEffort: 'high', serviceTier: 'fast' });
    },
  );

  it('keeps an unrelated stored preference when an API override fails', async () => {
    const ctx = fixture(); rejectFirst(ctx, 'result', handle([success]));
    await ctx.bridge.executeApiTask({ chatId: 'chat', prompt: 'work', model: 'api-retired' });
    expect(ctx.manager.getSession('chat').model).toBe('retired');
  });

  it.each(['different', 'retired'])('does not clear a concurrent %s preference write', async selected => {
    const ctx = fixture(); ctx.start.mockReturnValueOnce(handle([rejection], () => ctx.manager.setSessionModel('chat', selected, 'claude'))).mockReturnValueOnce(handle([success]));
    await ctx.bridge.executeApiTask({ chatId: 'chat', prompt: 'work' });
    expect(ctx.manager.getSession('chat').model).toBe(selected);
  });

  it('clears the still-current selected preference and aggregates attempt costs once', async () => {
    const ctx = fixture(); rejectFirst(ctx, 'result', handle([success]));
    const result = await ctx.bridge.executeApiTask({ chatId: 'chat', prompt: 'work', sendCards: true });
    expect(ctx.manager.getSession('chat').model).toBeUndefined();
    expect(result.costUsd).toBeCloseTo(0.3);
    expect(ctx.cost).toHaveBeenCalledWith(expect.objectContaining({ costUsd: result.costUsd }));
    expect(ctx.manager.getSession('chat').cumulativeCostUsd).toBeCloseTo(0.3);
  });

  it.each([
    { type: 'assistant', message: { content: [{ type: 'text', text: 'started' }] } },
    { type: 'assistant', message: { content: [{ type: 'tool_use', name: 'Bash', id: 'tool', input: { command: 'write something' } }] } },
    question,
    { type: 'system', subtype: 'task_started' },
    { type: 'stream_event', event: { type: 'content_block_delta', delta: { type: 'text_delta', text: 'started' } } },
  ] satisfies SDKMessage[])('does not replay after $type side effects', async message => {
    const ctx = fixture(); ctx.start.mockReturnValueOnce(handle([message, rejection])).mockReturnValueOnce(handle([success]));
    const result = await ctx.bridge.executeApiTask({ chatId: 'chat', prompt: 'work', onQuestion: async () => '{}' });
    expect(result.success).toBe(false);
    expect(ctx.start).toHaveBeenCalledTimes(1);
    expect(ctx.manager.getSession('chat').model).toBe('retired');
  });

  it.each(['maximum context length exceeded', 'No conversation found with session ID old'])(
    'does not replay a resumed turn after a tool and %s', async error => {
      const ctx = fixture();
      ctx.manager.setSessionId('chat', 'old', 'claude');
      ctx.start.mockReturnValueOnce(handle([
        { type: 'assistant', message: { content: [{ type: 'tool_use', name: 'Bash', id: 'write', input: { command: 'write output' } }] } },
        { ...rejection, errors: [error] },
      ])).mockReturnValueOnce(handle([success]));
      const result = await ctx.bridge.executeApiTask({ chatId: 'chat', prompt: 'work' });
      expect(result.success).toBe(false);
      expect(ctx.start).toHaveBeenCalledTimes(1);
    },
  );

  it('recovers an explicit zero-turn unknown-model error with an unprefixed result', async () => {
    const ctx = fixture();
    ctx.start.mockReturnValueOnce(handle([
      { ...rejection, num_turns: 0, errors: ['invalid model name'], result: 'invalid model name' },
    ])).mockReturnValueOnce(handle([success]));
    const result = await ctx.bridge.executeApiTask({ chatId: 'chat', prompt: 'work' });
    expect(result.success).toBe(true);
    expect(ctx.start).toHaveBeenCalledTimes(2);
    expect(ctx.manager.getSession('chat').model).toBeUndefined();
  });

  it('does not classify assistant prose as a provider rejection', async () => {
    const ctx = fixture(); ctx.start.mockReturnValueOnce(handle([{ ...success, result: 'The requested model does not exist in this documentation.' }]));
    const result = await ctx.bridge.executeApiTask({ chatId: 'chat', prompt: 'work' });
    expect(result.success).toBe(true); expect(ctx.start).toHaveBeenCalledTimes(1);
  });

  it.each(['empty', 'aborted', 'rejected', 'stale', 'throw'] as const)('normalizes %s fallback and never submits a third attempt', async mode => {
    const ctx = fixture();
    const fallback = mode === 'empty' ? handle() : mode === 'rejected' ? handle([rejection]) : mode === 'stale' ? handle([{ ...rejection, errors: ['No conversation found with session ID old'] }]) : handle([], () => {
      if (mode === 'aborted') ctx.start.mock.calls[1][0].abortController.abort();
      else throw new Error('fallback failed');
    });
    ctx.manager.setSessionId('chat', 'old', 'claude'); rejectFirst(ctx, 'result', fallback);
    const onUpdate = vi.fn();
    const result = await ctx.bridge.executeApiTask({ chatId: 'chat', prompt: 'work', onUpdate });
    expect(result.success).toBe(false);
    expect(ctx.start).toHaveBeenCalledTimes(2);
    expect(onUpdate.mock.calls.filter(([, , final]) => final)).toHaveLength(1);
    expect(onUpdate.mock.calls.at(-1)?.[0].status).toBe('error');
    expectFinalized(ctx, true, false);
    expect(fallback.finish).toHaveBeenCalledTimes(1);
  });
});
