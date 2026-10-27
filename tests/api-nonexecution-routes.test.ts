import { EventEmitter } from 'node:events';
import { mkdtempSync, rmSync } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { afterEach, describe, expect, it, vi } from 'vitest';
import { BotRegistry } from '../src/api/bot-registry.js';
import { CircuitBreaker } from '../src/api/circuit-breaker.js';
import { handleTaskRoutes } from '../src/api/routes/task-routes.js';
import { __resetCoreChatRunsForTests, handleCoreChatRoutes } from '../src/api/routes/core-chat-routes.js';
import type { RouteContext } from '../src/api/routes/types.js';
import { MessageBridge } from '../src/bridge/message-bridge.js';
import type { IMessageSender } from '../src/bridge/message-sender.interface.js';
import type { BotConfigBase } from '../src/config.js';
import type { ExecutionHandle, SDKMessage } from '../src/engines/index.js';
import { createLogger } from '../src/utils/logger.js';

const backend = vi.hoisted(() => ({ start: vi.fn<() => ExecutionHandle>() }));
vi.mock('../src/engines/index.js', async (importOriginal) => ({
  ...await importOriginal<typeof import('../src/engines/index.js')>(),
  createEngine: () => ({ name: 'codex', createExecutor: () => ({ startExecution: backend.start }) }),
}));

function deferred<T>() {
  let resolve!: (value: T) => void;
  const promise = new Promise<T>((done) => { resolve = done; });
  return { promise, resolve };
}

function resultHandle(success = true, error = 'provider failed'): ExecutionHandle {
  return {
    stream: (async function* (): AsyncGenerator<SDKMessage> {
      yield { type: 'result', subtype: success ? 'success' : 'error', result: success ? 'done' : '', errors: success ? [] : [error] };
    })(),
    finish: vi.fn(), sendAnswer: vi.fn(), resolveQuestion: vi.fn(),
  };
}

async function flush() {
  await new Promise<void>((resolve) => setImmediate(resolve));
}

type Route = 'talk' | 'core';
const cleanups: Array<() => Promise<void>> = [];

afterEach(async () => {
  for (const cleanup of cleanups.splice(0)) await cleanup();
  __resetCoreChatRunsForTests();
  vi.restoreAllMocks();
  vi.unstubAllGlobals();
  vi.unstubAllEnvs();
});

function fixture(route: Route) {
  const root = mkdtempSync(join(tmpdir(), 'metabot-api-admission-'));
  vi.stubEnv('SESSION_STORE_DIR', root);
  vi.stubGlobal('fetch', vi.fn(async () => ({ ok: true, status: 200 })));
  backend.start.mockReset().mockImplementation(() => resultHandle());
  const logger = createLogger('silent');
  const sender = {
    sendCard: async () => 'card', updateCard: async () => true,
    sendTextNotice: async () => {}, sendText: async () => {},
    sendImageFile: async () => true, sendLocalFile: async () => true,
    downloadImage: async () => false, downloadFile: async () => false,
    getThreadContext: vi.fn<NonNullable<IMessageSender['getThreadContext']>>(async () => undefined),
  } satisfies IMessageSender;
  const config: BotConfigBase = {
    name: 'bot', engine: 'codex', persistentExecutor: { enabled: false },
    claude: {
      defaultWorkingDirectory: root, outputsBaseDir: join(root, 'outputs'), downloadsDir: join(root, 'downloads'), backend: 'sdk',
      maxTurns: undefined, maxBudgetUsd: undefined, model: undefined, apiKey: undefined,
    },
  };
  const bridge = new MessageBridge(config, logger, sender);
  const registry = new BotRegistry();
  registry.register({ name: 'bot', platform: 'web', bridge, config, sender });
  const circuitBreaker = new CircuitBreaker(logger, { failureThreshold: 1, resetTimeoutMs: 1000, halfOpenMaxAttempts: 2 });
  const ctx = {
    registry, logger, circuitBreaker, ws: {},
    budgetManager: { canAcceptTask: () => ({ allowed: true }), recordCost: vi.fn() },
  } as RouteContext;
  const chatId = route === 'talk' ? 'chat' : 'core-conv-bot';
  let sequence = 0;
  const pending: Array<Promise<unknown>> = [];
  const releases: Array<() => void> = [];
  cleanups.push(async () => {
    bridge.stopChatTask(chatId);
    releases.forEach((release) => release());
    await Promise.all(pending);
    await flush();
    await bridge.destroyAsync();
    rmSync(root, { recursive: true, force: true });
  });

  async function request() {
    const req = new EventEmitter();
    Object.assign(req, { headers: {} });
    const res = {
      statusCode: 0, body: '',
      writeHead(status: number) { this.statusCode = status; },
      end(body: string) { this.body = body; },
    };
    const runId = `run-${++sequence}`;
    const body = route === 'talk'
      ? { botName: 'bot', chatId, prompt: 'hello', sendCards: false }
      : { runId, conversationId: 'conv', triggerMessageId: 'msg', targetBot: 'bot', prompt: 'hello', eventCallbackUrl: 'https://core.example/events' };
    process.nextTick(() => {
      req.emit('data', Buffer.from(JSON.stringify(body)));
      req.emit('end');
    });
    const handler = route === 'talk' ? handleTaskRoutes : handleCoreChatRoutes;
    await handler(ctx, req as Parameters<typeof handler>[1], res as Parameters<typeof handler>[2], 'POST', route === 'talk' ? '/api/talk' : '/api/core-chat/runs');
    return { res, runId };
  }

  async function completeRequest() {
    const { res, runId } = await request();
    if (route === 'core') {
      expect(res.statusCode).toBe(202);
      await vi.waitFor(() => expect(vi.mocked(fetch).mock.calls.some((call) => {
        const event = JSON.parse(String(call[1]?.body));
        return event.runId === runId && ['complete', 'error'].includes(event.type);
      })).toBe(true));
      await flush();
    }
    return res;
  }

  async function parkStart() {
    bridge.getSessionManager().getSession(chatId).threadContextInitialized = false;
    const entered = deferred<void>();
    const gate = deferred<undefined>();
    sender.getThreadContext.mockImplementationOnce(async () => {
      entered.resolve();
      return gate.promise;
    });
    releases.push(() => gate.resolve(undefined));
    return { entered: entered.promise, release: () => gate.resolve(undefined) };
  }

  async function halfOpen() {
    const now = vi.spyOn(Date, 'now').mockReturnValue(10_000);
    const admission = circuitBreaker.tryAcquire('bot');
    expect(admission).toBeDefined();
    admission?.settle('failure');
    expect(circuitBreaker.getStatus().bot).toEqual({ state: 'open', failures: 1 });
    now.mockReturnValue(11_000);
  }

  return { bridge, circuitBreaker, chatId, completeRequest, request, parkStart, halfOpen, pending };
}

describe.each<Route>(['talk', 'core'])('%s real bridge non-execution admission', (route) => {
  it.each([
    ['busy', 'closed'], ['restart', 'closed'],
    ['busy', 'half-open'], ['restart', 'half-open'],
  ] as const)('abandons %s refusal with a %s circuit', async (refusal, state) => {
    const f = fixture(route);
    if (state === 'half-open') await f.halfOpen();
    const callsBefore = backend.start.mock.calls.length;
    let release: () => void;
    if (refusal === 'busy') {
      const gate = await f.parkStart();
      f.pending.push(f.bridge.executeApiTask({ chatId: f.chatId, prompt: 'unrelated owner' }));
      await gate.entered;
      release = () => { f.bridge.stopChatTask(f.chatId); gate.release(); };
    } else {
      f.bridge.beginRestartQuiesce('restart-request');
      release = () => f.bridge.cancelRestartQuiesce('restart-request');
    }
    const failures = state === 'closed' ? 0 : 1;
    try {
      const res = await f.completeRequest();
      if (route === 'talk') expect(res.statusCode).toBe(500);
      expect(backend.start).toHaveBeenCalledTimes(callsBefore);
      expect(f.circuitBreaker.getStatus().bot).toEqual({ state, failures });
      expect(f.circuitBreaker.isAvailable('bot')).toBe(true);
    } finally {
      release();
      await Promise.all(f.pending);
    }
    await f.completeRequest();
    if (state === 'half-open') {
      expect(f.circuitBreaker.getStatus().bot).toEqual({ state: 'half-open', failures: 1 });
      await f.completeRequest();
    }
    expect(f.circuitBreaker.getStatus().bot).toEqual({ state: 'closed', failures: 0 });
  });

  it('abandons a real pre-start cancellation result without counting recovery', async () => {
    const f = fixture(route);
    await f.halfOpen();
    const callsBefore = backend.start.mock.calls.length;
    const gate = await f.parkStart();
    const request = f.completeRequest();
    f.pending.push(request);
    await gate.entered;
    expect(f.bridge.stopChatTask(f.chatId)).toBe(true);
    gate.release();
    await request;
    expect(backend.start).toHaveBeenCalledTimes(callsBefore);
    expect(f.circuitBreaker.getStatus().bot).toEqual({ state: 'half-open', failures: 1 });
    expect(f.circuitBreaker.isAvailable('bot')).toBe(true);
    await f.completeRequest();
    expect(f.circuitBreaker.getStatus().bot.state).toBe('half-open');
    await f.completeRequest();
    expect(f.circuitBreaker.getStatus().bot).toEqual({ state: 'closed', failures: 0 });
  });

  it('still counts an unmarked backend failure even when its text matches a busy refusal', async () => {
    const f = fixture(route);
    backend.start.mockImplementationOnce(() => resultHandle(false, 'Chat is busy with another task'));
    await f.completeRequest();
    expect(backend.start).toHaveBeenCalledTimes(1);
    expect(f.circuitBreaker.getStatus().bot).toEqual({ state: 'open', failures: 1 });
  });
});
