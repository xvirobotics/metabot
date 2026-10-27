import type * as http from 'node:http';
import { jsonResponse, parseJsonBody } from './helpers.js';
import type { RouteContext } from './types.js';
import type { ApiTaskResult } from '../../bridge/message-bridge.js';
import type { CircuitAdmission } from '../circuit-breaker.js';
import type { OutputFile } from '../../bridge/outputs-manager.js';
import type { EngineName } from '../../engines/index.js';
import type { CardState, PendingQuestion } from '../../types.js';
import { resolveSTTProvider, resolveTTSProvider, resolveTTSVoice } from '../voice-handler.js';

const CORE_CHAT_RUN_PREFIX = '/api/core-chat/runs';
const CORE_CHAT_CAPABILITIES_PATH = '/api/core-chat/capabilities';
const CALLBACK_RETRY_ATTEMPTS = 3;
const CALLBACK_RETRY_DELAY_MS = 250;
const DEFAULT_QUESTION_TIMEOUT_MS = 15 * 60 * 1000;

type CoreChatRunStatus = 'running' | 'completed' | 'failed' | 'canceled';
type CoreChatEventType = 'state' | 'question' | 'file' | 'log' | 'complete' | 'error';

interface PendingCoreChatQuestion {
  toolUseId: string;
  resolve: (answerJson: string) => void;
  timeout: ReturnType<typeof setTimeout>;
}

interface CoreChatRunRecord {
  runId: string;
  targetBot: string;
  executionChatId: string;
  eventCallbackUrl: string;
  status: CoreChatRunStatus;
  admission: CircuitAdmission;
  execution: 'pending' | 'executing' | 'finished';
  dispatcher?: CoreChatEventDispatcher;
  pendingQuestion?: PendingCoreChatQuestion;
}

export interface CoreChatRunRequest {
  runId: string;
  conversationId: string;
  triggerMessageId: string;
  targetBot: string;
  prompt: string;
  eventCallbackUrl: string;
  executionChatId?: string;
  userId?: string;
  engine?: EngineName;
  model?: string;
  notification?: 'result' | 'error';
  presentation?: {
    mode: 'child-direct' | 'origin-proxy';
    chatId: string;
    targetAgentRef: string;
    requestedBy: string;
    originBotName?: string;
    wakeLead?: boolean;
  };
  maxTurns?: number;
  allowedTools?: string[];
  voice?: {
    announceCapabilities?: boolean;
    tts?: boolean;
    ttsProvider?: string;
    ttsVoice?: string;
  };
  metadata?: {
    groupId?: string;
    groupMembers?: string[];
  };
}

interface CoreChatEvent {
  runId: string;
  seq: number;
  type: CoreChatEventType;
  createdAt: string;
  bridge: {
    botName: string;
    executionChatId: string;
  };
  payload: Record<string, unknown>;
}

const activeCoreChatRuns = new Map<string, CoreChatRunRecord>();

class CoreChatTerminalStateError extends Error {
  readonly state: CardState;
  readonly bridgeMessageId: string | undefined;

  constructor(message: string, state: CardState, bridgeMessageId: string | undefined) {
    super(message);
    this.name = 'CoreChatTerminalStateError';
    this.state = state;
    this.bridgeMessageId = bridgeMessageId;
  }
}

function asString(value: unknown): string | undefined {
  return typeof value === 'string' && value.trim() ? value : undefined;
}

function asStringArray(value: unknown): string[] | undefined {
  if (!Array.isArray(value)) return undefined;
  const strings = value.filter((item): item is string => typeof item === 'string' && item.trim().length > 0);
  return strings.length === value.length ? strings : undefined;
}

function asEngineName(value: unknown): EngineName | undefined {
  return value === 'claude' || value === 'kimi' || value === 'codex' ? value : undefined;
}

function sanitizeChatIdPart(value: string): string {
  return value.replace(/[^a-zA-Z0-9._:-]/g, '_').slice(0, 160);
}

function defaultExecutionChatId(conversationId: string, targetBot: string): string {
  return `core-${sanitizeChatIdPart(conversationId)}-${sanitizeChatIdPart(targetBot)}`;
}

function questionAnswerJson(answer: string): string {
  return JSON.stringify({ answers: { _web: answer } });
}

function questionTimeoutMs(): number {
  const parsed = Number.parseInt(process.env.METABOT_CORE_CHAT_QUESTION_TIMEOUT_MS || '', 10);
  return Number.isFinite(parsed) && parsed > 0 ? parsed : DEFAULT_QUESTION_TIMEOUT_MS;
}

function validateCallbackUrl(raw: string): boolean {
  try {
    const parsed = new URL(raw);
    return parsed.protocol === 'http:' || parsed.protocol === 'https:';
  } catch {
    return false;
  }
}

export function parseCoreChatRunRequest(body: Record<string, unknown>): { request?: CoreChatRunRequest; error?: string } {
  const runId = asString(body.runId);
  const conversationId = asString(body.conversationId);
  const triggerMessageId = asString(body.triggerMessageId);
  const targetBot = asString(body.targetBot);
  const prompt = asString(body.prompt);
  const eventCallbackUrl = asString(body.eventCallbackUrl);

  if (!runId || !conversationId || !triggerMessageId || !targetBot || !prompt || !eventCallbackUrl) {
    return { error: 'Missing required fields: runId, conversationId, triggerMessageId, targetBot, prompt, eventCallbackUrl' };
  }
  if (!validateCallbackUrl(eventCallbackUrl)) {
    return { error: 'Invalid eventCallbackUrl' };
  }

  const metadata = typeof body.metadata === 'object' && body.metadata !== null
    ? body.metadata as Record<string, unknown>
    : undefined;
  const maxTurns = typeof body.maxTurns === 'number' && Number.isFinite(body.maxTurns)
    ? body.maxTurns
    : undefined;
  const voice = typeof body.voice === 'object' && body.voice !== null
    ? body.voice as Record<string, unknown>
    : undefined;
  const rawPresentation = typeof body.presentation === 'object' && body.presentation !== null
    ? body.presentation as Record<string, unknown>
    : undefined;
  const presentationMode: 'child-direct' | 'origin-proxy' | undefined =
    rawPresentation?.mode === 'child-direct' || rawPresentation?.mode === 'origin-proxy'
      ? rawPresentation.mode
      : undefined;
  const presentationChatId = asString(rawPresentation?.chatId);
  const presentationTarget = asString(rawPresentation?.targetAgentRef);
  const presentationRequestedBy = asString(rawPresentation?.requestedBy);
  const presentation =
    (presentationMode === 'child-direct' || presentationMode === 'origin-proxy') &&
    presentationChatId && presentationTarget && presentationRequestedBy
      ? {
          mode: presentationMode,
          chatId: presentationChatId,
          targetAgentRef: presentationTarget,
          requestedBy: presentationRequestedBy,
          originBotName: asString(rawPresentation?.originBotName),
          wakeLead: rawPresentation?.wakeLead !== false,
        }
      : undefined;

  return {
    request: {
      runId,
      conversationId,
      triggerMessageId,
      targetBot,
      prompt,
      eventCallbackUrl,
      executionChatId: asString(body.executionChatId),
      userId: asString(body.userId),
      engine: asEngineName(body.engine),
      model: asString(body.model),
      notification: body.notification === 'result' || body.notification === 'error' ? body.notification : undefined,
      presentation,
      maxTurns,
      allowedTools: asStringArray(body.allowedTools),
      voice: voice ? {
        announceCapabilities: voice.announceCapabilities === true,
        tts: voice.tts === true,
        ttsProvider: asString(voice.ttsProvider),
        ttsVoice: asString(voice.ttsVoice),
      } : undefined,
      metadata: metadata ? {
        groupId: asString(metadata.groupId),
        groupMembers: asStringArray(metadata.groupMembers),
      } : undefined,
    },
  };
}

function callbackAuthHeaders(): Record<string, string> {
  const token = process.env.METABOT_CORE_TOKEN?.trim();
  return token ? { Authorization: `Bearer ${token}` } : {};
}

async function sleep(ms: number): Promise<void> {
  await new Promise((resolve) => setTimeout(resolve, ms));
}

export async function postCoreChatRunEvent(
  callbackUrl: string,
  event: CoreChatEvent,
  options?: { attempts?: number; retryDelayMs?: number; fetchImpl?: typeof fetch },
): Promise<void> {
  const attempts = options?.attempts ?? CALLBACK_RETRY_ATTEMPTS;
  const retryDelayMs = options?.retryDelayMs ?? CALLBACK_RETRY_DELAY_MS;
  const fetchImpl = options?.fetchImpl ?? fetch;
  let lastError: unknown;

  for (let attempt = 1; attempt <= attempts; attempt++) {
    try {
      const response = await fetchImpl(callbackUrl, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          ...callbackAuthHeaders(),
        },
        body: JSON.stringify(event),
      });
      if (response.ok) return;
      const text = await response.text().catch(() => '');
      lastError = new Error(`core callback failed with HTTP ${response.status}${text ? `: ${text.slice(0, 200)}` : ''}`);
    } catch (err) {
      lastError = err;
    }

    if (attempt < attempts) {
      await sleep(retryDelayMs);
    }
  }

  throw lastError instanceof Error ? lastError : new Error('core callback failed');
}

class CoreChatEventDispatcher {
  private nextSeq = 1;
  private chain: Promise<void> = Promise.resolve();

  constructor(
    private readonly callbackUrl: string,
    private readonly runId: string,
    private readonly botName: string,
    private readonly executionChatId: string,
    private readonly logger: RouteContext['logger'],
  ) {}

  enqueue(type: CoreChatEventType, payload: Record<string, unknown>): void {
    const event = this.buildEvent(type, payload);
    this.chain = this.chain
      .then(() => postCoreChatRunEvent(this.callbackUrl, event))
      .catch((err) => {
        this.logger.warn({ err, runId: this.runId, seq: event.seq, type }, 'Core chat callback failed');
      });
  }

  async send(type: CoreChatEventType, payload: Record<string, unknown>): Promise<void> {
    const event = this.buildEvent(type, payload);
    this.chain = this.chain.then(() => postCoreChatRunEvent(this.callbackUrl, event));
    await this.chain;
  }

  async drain(): Promise<void> {
    await this.chain;
  }

  private buildEvent(type: CoreChatEventType, payload: Record<string, unknown>): CoreChatEvent {
    return {
      runId: this.runId,
      seq: this.nextSeq++,
      type,
      createdAt: new Date().toISOString(),
      bridge: {
        botName: this.botName,
        executionChatId: this.executionChatId,
      },
      payload,
    };
  }
}

function waitForCoreChatAnswer(record: CoreChatRunRecord, toolUseId: string): Promise<string> {
  return new Promise((resolve) => {
    if (record.status !== 'running') {
      resolve(questionAnswerJson('The run is no longer active. Stop safely.'));
      return;
    }
    if (record.pendingQuestion) {
      clearTimeout(record.pendingQuestion.timeout);
      record.pendingQuestion.resolve(questionAnswerJson('A newer question replaced this one. Continue safely.'));
    }
    const timeout = setTimeout(() => {
      if (record.pendingQuestion?.toolUseId !== toolUseId) return;
      record.pendingQuestion = undefined;
      resolve(questionAnswerJson('No browser answer arrived before timeout. Use your safest reasonable default and continue.'));
    }, questionTimeoutMs());
    timeout.unref?.();
    record.pendingQuestion = { toolUseId, resolve, timeout };
  });
}

export function answerCoreChatRun(
  runId: string,
  toolUseId: string,
  answer: string,
): { status: number; body: Record<string, unknown> } {
  const record = activeCoreChatRuns.get(runId);
  if (!record || record.status !== 'running') {
    return { status: 200, body: { runId, status: 'not_running', answered: false } };
  }
  const pending = record.pendingQuestion;
  if (!pending || pending.toolUseId !== toolUseId) {
    return { status: 409, body: { runId, status: 'question_not_pending', answered: false } };
  }
  clearTimeout(pending.timeout);
  record.pendingQuestion = undefined;
  pending.resolve(questionAnswerJson(answer));
  return { status: 200, body: { runId, status: 'answered', answered: true } };
}

export async function cancelCoreChatRun(
  ctx: RouteContext,
  runId: string,
): Promise<{ status: number; body: Record<string, unknown> }> {
  const record = activeCoreChatRuns.get(runId);
  if (!record || record.status !== 'running') {
    return { status: 200, body: { runId, status: 'not_running', stopped: false } };
  }

  record.status = 'canceled';
  record.admission.settle('abandoned');
  const pending = record.pendingQuestion;
  record.pendingQuestion = undefined;
  if (pending) {
    clearTimeout(pending.timeout);
    pending.resolve(questionAnswerJson('The user canceled this run. Stop now.'));
  }
  let stopped = false;
  if (record.execution === 'executing') {
    try {
      const bot = ctx.registry.get(record.targetBot);
      stopped = bot?.bridge.stopChatTask(record.executionChatId) ?? false;
    } catch (err) {
      ctx.logger.warn({ err, runId }, 'Core chat cancellation stop failed');
    }
  }
  if (record.dispatcher) {
    try {
      await record.dispatcher.send('error', {
        final: true,
        status: 'canceled',
        error: 'Canceled by user',
      });
    } catch (err) {
      ctx.logger.warn({ err, runId }, 'Core chat cancellation callback failed');
    }
  }
  return {
    status: 200,
    body: {
      runId,
      status: 'canceled',
      stopped,
      executionChatId: record.executionChatId,
      targetBot: record.targetBot,
    },
  };
}

function outputFilePayload(files: OutputFile[]): Record<string, unknown> {
  return {
    files: files.map((file) => ({
      name: file.fileName,
      sizeBytes: file.sizeBytes,
      extension: file.extension,
      isImage: file.isImage,
      mimeType: mimeTypeForExtension(file.extension),
      transfer: {
        mode: 'bridge-private',
        note: 'bridge-local file path intentionally not exposed; core storage ingestion is required before download',
      },
    })),
  };
}

function updateRunTerminalStatus(record: CoreChatRunRecord, status: Exclude<CoreChatRunStatus, 'running'>): void {
  if (record.status === 'canceled') return;
  record.status = status;
}

function terminalErrorFromState(state: CardState): string | undefined {
  const status = typeof state.status === 'string' ? state.status.toLowerCase() : '';
  const message = [state.errorMessage, state.responseText]
    .filter((value): value is string => typeof value === 'string' && value.trim().length > 0)
    .join('\n')
    .trim();
  if (status === 'error' || status === 'failed') {
    return message || 'Agent run failed';
  }
  if (!message) return undefined;

  if (
    /\bapi error\b/i.test(message) ||
    /request rejected/i.test(message) ||
    /rate limit/i.test(message) ||
    /\b429\b/.test(message)
  ) {
    return message;
  }
  return undefined;
}

function mimeTypeForExtension(extension: string): string {
  switch (extension.toLowerCase()) {
    case '.png': return 'image/png';
    case '.jpg':
    case '.jpeg': return 'image/jpeg';
    case '.gif': return 'image/gif';
    case '.webp': return 'image/webp';
    case '.svg': return 'image/svg+xml';
    case '.mp4': return 'video/mp4';
    case '.webm': return 'video/webm';
    case '.mp3': return 'audio/mpeg';
    case '.wav': return 'audio/wav';
    case '.pdf': return 'application/pdf';
    case '.json': return 'application/json';
    case '.csv': return 'text/csv';
    case '.md': return 'text/markdown';
    case '.txt': return 'text/plain';
    case '.html': return 'text/html';
    default: return 'application/octet-stream';
  }
}

export function buildCoreChatCapabilities(ctx: RouteContext): Record<string, unknown> {
  const sttProvider = resolveSTTProvider('');
  const ttsProvider = resolveTTSProvider('');
  const doubaoSpeechConfigured = Boolean(process.env.VOLCENGINE_TTS_APPID && process.env.VOLCENGINE_TTS_ACCESS_KEY);
  const openAiConfigured = Boolean(process.env.OPENAI_API_KEY);
  const elevenLabsConfigured = Boolean(process.env.ELEVENLABS_API_KEY);
  const rtcConfigured = ctx.rtcService?.isConfigured() ?? false;

  return {
    auth: {
      mode: 'bridge-api-secret',
      header: 'Authorization: Bearer <API_SECRET>',
      queryTokenAccepted: true,
    },
    voice: {
      browserSpeechRecognition: {
        mode: 'client-side',
        note: 'Web Chat can use browser SpeechRecognition and send transcript as normal chat text.',
      },
      stt: {
        endpoint: '/api/voice?sttOnly=true',
        defaultProvider: sttProvider,
        providers: {
          doubao: { configured: doubaoSpeechConfigured },
          whisper: { configured: openAiConfigured },
        },
      },
      tts: {
        endpoint: '/api/tts',
        defaultProvider: ttsProvider,
        defaultVoice: resolveTTSVoice('', ttsProvider),
        providers: {
          doubao: { configured: doubaoSpeechConfigured },
          edge: { configured: true },
          openai: { configured: openAiConfigured },
          elevenlabs: { configured: elevenLabsConfigured },
        },
      },
      rtc: {
        configured: rtcConfigured,
        endpoints: {
          config: '/api/rtc/config',
          start: '/api/rtc/start',
          voice: '/api/rtc/voice',
          stop: '/api/rtc/stop',
          transcript: '/api/rtc/transcript',
        },
      },
    },
    agents: ctx.registry.list().map((bot) => {
      const model = bot.model || '';
      const doubaoModel = /doubao|seed/i.test(model);
      return {
        name: bot.name,
        platform: bot.platform,
        engine: bot.engine,
        ...(bot.model ? { model: bot.model } : {}),
        ...(bot.description ? { description: bot.description } : {}),
        ...(bot.specialties?.length ? { specialties: bot.specialties } : {}),
        ...(bot.ttsVoice ? { ttsVoice: bot.ttsVoice } : {}),
        capabilities: {
          coreChat: true,
          voiceInput: true,
          tts: true,
          rtc: rtcConfigured,
          doubaoVoice: doubaoSpeechConfigured || Boolean(bot.ttsVoice),
          doubaoModel,
        },
      };
    }),
    routes: {
      coreChatRun: CORE_CHAT_RUN_PREFIX,
      coreChatAnswer: `${CORE_CHAT_RUN_PREFIX}/{runId}/answer`,
      coreChatCancel: `${CORE_CHAT_RUN_PREFIX}/{runId}/cancel`,
      capabilities: CORE_CHAT_CAPABILITIES_PATH,
    },
  };
}

function voiceRunPayload(ctx: RouteContext, request: CoreChatRunRequest, responseText?: string): Record<string, unknown> {
  const capabilities = buildCoreChatCapabilities(ctx);
  const provider = resolveTTSProvider(request.voice?.ttsProvider || '');
  const voice = resolveTTSVoice(request.voice?.ttsVoice || '', provider, responseText);
  return {
    kind: responseText ? 'voice_tts_ready' : 'voice_capabilities',
    capabilities,
    tts: {
      requested: request.voice?.tts === true,
      endpoint: '/api/tts',
      provider,
      voice,
      textLength: responseText?.length ?? 0,
    },
  };
}

async function runCoreChatTask(ctx: RouteContext, request: CoreChatRunRequest, record: CoreChatRunRecord): Promise<void> {
  try {
    const { registry, logger, budgetManager } = ctx;
    const { executionChatId, admission } = record;
    const bot = registry.get(request.targetBot);
    if (!bot) {
      updateRunTerminalStatus(record, 'failed');
      return;
    }

    const dispatcher = new CoreChatEventDispatcher(
      request.eventCallbackUrl,
      request.runId,
      request.targetBot,
      executionChatId,
      logger,
    );
    record.dispatcher = dispatcher;

    let finalState: CardState | undefined;
    let finalMessageId: string | undefined;

    try {
      dispatcher.enqueue('state', {
        final: false,
        state: {
          status: 'running',
          userPrompt: request.prompt,
          responseText: '',
          toolCalls: [],
        },
      });

      if (request.voice?.announceCapabilities) {
        dispatcher.enqueue('log', voiceRunPayload(ctx, request));
      }

      let steered = false;
      if (request.notification) {
        const deadline = Date.now() + 5 * 60_000;
        while (bot.bridge.isBusy(executionChatId)) {
          if (record.status !== 'running') return;
          steered = request.notification === 'result'
            && await bot.bridge.steerAgentNotification(executionChatId, request.prompt);
          if (steered) break;
          if (Date.now() >= deadline) throw new Error('Notification origin remained busy; result is available in session history');
          await new Promise(resolve => setTimeout(resolve, 1000));
        }
      }
      if (record.status !== 'running') return;
      let result: ApiTaskResult;
      if (steered) {
        admission.settle('abandoned');
        result = { success: true, responseText: 'Agent Bus notification delivered to the active turn.' };
      } else {
        record.execution = 'executing';
        try {
          result = await bot.bridge.executeApiTask({
            prompt: request.prompt,
            chatId: executionChatId,
            userId: request.userId ?? 'core-chat',
            sendCards: false,
            engine: request.engine,
            model: request.model,
            maxTurns: request.maxTurns,
            allowedTools: request.allowedTools,
            groupId: request.metadata?.groupId,
            groupMembers: request.metadata?.groupMembers,
            onUpdate: (state, bridgeMessageId, final) => {
              if (final) {
                finalState = state;
                finalMessageId = bridgeMessageId;
                return;
              }
              const terminalError = terminalErrorFromState(state);
              if (terminalError) {
                finalState = state;
                finalMessageId = bridgeMessageId;
                dispatcher.enqueue('error', {
                  messageId: bridgeMessageId,
                  final: true,
                  state,
                  error: terminalError,
                });
                throw new CoreChatTerminalStateError(terminalError, state, bridgeMessageId);
              }
              dispatcher.enqueue('state', { messageId: bridgeMessageId, final: false, state });
            },
            onQuestion: async (question: PendingQuestion) => {
              try {
                await dispatcher.send('question', {
                  toolUseId: question.toolUseId,
                  question,
                  autoAnswer: false,
                });
              } catch (err) {
                admission.settle('abandoned');
                throw err;
              }
              return waitForCoreChatAnswer(record, question.toolUseId);
            },
            onOutputFiles: (files) => {
              if (files.length > 0) {
                dispatcher.enqueue('file', outputFilePayload(files));
              }
            },
          });
        } catch (err) {
          admission.settle('failure');
          throw err;
        } finally {
          record.execution = 'finished';
        }
        admission.settle(result.execution === 'not-started' ? 'abandoned' : result.success ? 'success' : 'failure');
      }

      if (record.status !== 'running') return;
      if (result.costUsd) {
        budgetManager.recordCost(request.targetBot, result.costUsd);
      }

      await dispatcher.drain();
      if (request.voice?.tts && result.responseText) {
        await dispatcher.send('log', voiceRunPayload(ctx, request, result.responseText));
      }
      await dispatcher.send(result.success ? 'complete' : 'error', {
        messageId: finalMessageId,
        final: true,
        state: finalState,
        result: apiTaskResultPayload(result),
        ...(result.error ? { error: result.error } : {}),
      });

      updateRunTerminalStatus(record, result.success ? 'completed' : 'failed');
    } catch (err: any) {
      admission.settle('abandoned');
      if (record.status !== 'running') return;
      await dispatcher.drain();
      if (err instanceof CoreChatTerminalStateError) {
        updateRunTerminalStatus(record, 'failed');
        return;
      }
      await dispatcher.send('error', {
        messageId: finalMessageId,
        final: true,
        state: finalState,
        error: err?.message || 'Task execution failed',
      });
      updateRunTerminalStatus(record, 'failed');
    }
  } finally {
    record.admission.settle('abandoned');
  }
}

export function acceptCoreChatRun(ctx: RouteContext, request: CoreChatRunRequest): {
  status: number;
  body: Record<string, unknown>;
} {
  const { registry, logger, circuitBreaker, budgetManager } = ctx;
  const bot = registry.get(request.targetBot);
  if (!bot) {
    return { status: 404, body: { error: `Bot not found: ${request.targetBot}` } };
  }
  if (!circuitBreaker.isAvailable(request.targetBot)) {
    return { status: 503, body: { error: `Bot "${request.targetBot}" is temporarily unavailable (circuit open)` } };
  }
  const budgetCheck = budgetManager.canAcceptTask(request.targetBot);
  if (!budgetCheck.allowed) {
    return { status: 429, body: { error: budgetCheck.reason } };
  }

  const executionChatId =
    request.presentation?.mode === 'child-direct'
      ? request.presentation.chatId
      : request.executionChatId || defaultExecutionChatId(request.conversationId, request.targetBot);
  if (activeCoreChatRuns.get(request.runId)?.status === 'running') {
    return { status: 409, body: { error: `Run already active: ${request.runId}` } };
  }
  const admission = circuitBreaker.tryAcquire(request.targetBot);
  if (!admission) {
    return { status: 503, body: { error: `Bot "${request.targetBot}" is temporarily unavailable (circuit open)` } };
  }
  try {
    const record: CoreChatRunRecord = {
      runId: request.runId,
      targetBot: request.targetBot,
      executionChatId,
      eventCallbackUrl: request.eventCallbackUrl,
      status: 'running',
      admission,
      execution: 'pending',
    };
    activeCoreChatRuns.set(request.runId, record);

    logger.info({ runId: request.runId, targetBot: request.targetBot, executionChatId }, 'Core chat run accepted');
    void runCoreChatTask(ctx, request, record).catch((err) => {
      logger.error({ err, runId: request.runId, targetBot: request.targetBot }, 'Core chat run failed outside task handler');
      updateRunTerminalStatus(record, 'failed');
    });
  } catch (err) {
    admission.settle('abandoned');
    throw err;
  }

  return {
    status: 202,
    body: {
      runId: request.runId,
      status: 'accepted',
      targetBot: request.targetBot,
      executionChatId,
    },
  };
}

function apiTaskResultPayload(result: ApiTaskResult): Record<string, unknown> {
  return {
    success: result.success,
    responseText: result.responseText,
    sessionId: result.sessionId,
    costUsd: result.costUsd,
    durationMs: result.durationMs,
    error: result.error,
  };
}

export async function handleCoreChatRoutes(
  ctx: RouteContext,
  req: http.IncomingMessage,
  res: http.ServerResponse,
  method: string,
  url: string,
): Promise<boolean> {
  if (method === 'GET' && (url === CORE_CHAT_CAPABILITIES_PATH || url.startsWith(`${CORE_CHAT_CAPABILITIES_PATH}?`))) {
    jsonResponse(res, 200, buildCoreChatCapabilities(ctx));
    return true;
  }

  if (method === 'POST' && (url === CORE_CHAT_RUN_PREFIX || url.startsWith(`${CORE_CHAT_RUN_PREFIX}?`))) {
    const parsed = parseCoreChatRunRequest(await parseJsonBody(req));
    if (!parsed.request) {
      jsonResponse(res, 400, { error: parsed.error });
      return true;
    }
    const accepted = acceptCoreChatRun(ctx, parsed.request);
    jsonResponse(res, accepted.status, accepted.body);
    return true;
  }

  const answerMatch = url.match(/^\/api\/core-chat\/runs\/([^/?]+)\/answer(?:\?.*)?$/);
  if (method === 'POST' && answerMatch) {
    const runId = decodeURIComponent(answerMatch[1] || '');
    const body = await parseJsonBody(req);
    const toolUseId = asString(body.toolUseId);
    const answer = asString(body.answer);
    if (!toolUseId || !answer) {
      jsonResponse(res, 400, { error: 'toolUseId and answer are required' });
      return true;
    }
    const result = answerCoreChatRun(runId, toolUseId, answer);
    jsonResponse(res, result.status, result.body);
    return true;
  }

  const cancelMatch = url.match(/^\/api\/core-chat\/runs\/([^/?]+)\/cancel(?:\?.*)?$/);
  if (method === 'POST' && cancelMatch) {
    const runId = decodeURIComponent(cancelMatch[1] || '');
    const result = await cancelCoreChatRun(ctx, runId);
    jsonResponse(res, result.status, result.body);
    return true;
  }

  return false;
}

export function __resetCoreChatRunsForTests(): void {
  for (const record of activeCoreChatRuns.values()) {
    if (record.pendingQuestion) clearTimeout(record.pendingQuestion.timeout);
  }
  activeCoreChatRuns.clear();
}
