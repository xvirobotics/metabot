import { EventEmitter } from 'node:events';
import { afterEach, describe, expect, it, vi } from 'vitest';
import { CircuitBreaker } from '../src/api/circuit-breaker.js';
import { handleTaskRoutes } from '../src/api/routes/task-routes.js';
import type { RouteContext } from '../src/api/routes/types.js';

const logger = {
  debug: vi.fn(), info: vi.fn(), warn: vi.fn(), error: vi.fn(), child: () => logger,
} as any;

function makeCtx() {
  const executeApiTask = vi.fn(async (_options: any) => ({ success: true, responseText: 'done' }));
  const callback = vi.fn(async () => ({ success: true, responseText: 'delivered' }));
  const create = vi.fn(() => ({ id: 'task-1' }));
  const update = vi.fn((_id: string, _patch: any) => {});
  const canAcceptTask = vi.fn(() => ({ allowed: true, reason: '' }));
  const ctx: RouteContext = {
    registry: {
      get: (name: string) => name === 'bot' ? { bridge: { executeApiTask } }
        : name === 'callback' ? { bridge: { executeApiTask: callback } } : undefined,
    } as any,
    logger,
    asyncTaskStore: { create, update } as any,
    circuitBreaker: new CircuitBreaker(logger, {
      failureThreshold: 1, resetTimeoutMs: 1000, halfOpenMaxAttempts: 2,
    }),
    budgetManager: { canAcceptTask, recordCost: vi.fn() } as any,
    ws: {},
  } as RouteContext;
  return { ctx, executeApiTask, callback, create, update, canAcceptTask };
}

async function talk(ctx: RouteContext, body: Record<string, unknown> = {}) {
  const req = new EventEmitter() as any;
  req.headers = {};
  const res = {
    statusCode: 0,
    body: '',
    writeHead(status: number) { this.statusCode = status; },
    end(body: string) { this.body = body; },
  } as any;
  process.nextTick(() => {
    req.emit('data', Buffer.from(JSON.stringify({ botName: 'bot', chatId: 'chat', prompt: 'hello', ...body })));
    req.emit('end');
  });
  await handleTaskRoutes(ctx, req, res, 'POST', '/api/talk');
  return res;
}

async function flush() {
  await new Promise<void>((resolve) => setImmediate(resolve));
}

async function readyProbe(fixture: ReturnType<typeof makeCtx>) {
  const now = vi.spyOn(Date, 'now').mockReturnValue(0);
  fixture.executeApiTask.mockResolvedValueOnce({ success: false, responseText: 'provider failed' });
  expect((await talk(fixture.ctx)).statusCode).toBe(500);
  expect(fixture.ctx.circuitBreaker.getStatus().bot.state).toBe('open');
  now.mockReturnValue(1000);
  return now;
}

describe('talk route circuit ownership', () => {
  afterEach(() => vi.restoreAllMocks());

  it('does not consume the half-open probe on budget 429', async () => {
    const f = makeCtx();
    await readyProbe(f);
    f.canAcceptTask.mockReturnValueOnce({ allowed: false, reason: 'budget exhausted' });
    expect((await talk(f.ctx)).statusCode).toBe(429);
    expect((await talk(f.ctx)).statusCode).toBe(200);
    expect(f.executeApiTask).toHaveBeenCalledTimes(2);
  });

  it('records a thrown synchronous execution as one primary failure', async () => {
    const f = makeCtx();
    await readyProbe(f);
    f.executeApiTask.mockRejectedValueOnce(new Error('provider disconnected'));
    await expect(talk(f.ctx)).rejects.toThrow('provider disconnected');
    expect(f.ctx.circuitBreaker.getStatus().bot).toEqual({ state: 'open', failures: 2 });
  });

  it('abandons when synchronous streaming setup throws before execution', async () => {
    const f = makeCtx();
    await readyProbe(f);
    f.ctx.ws = { handle: { subscriptions: { getSubscribers: () => { throw new Error('subscriptions unavailable'); } } } } as any;
    await expect(talk(f.ctx)).rejects.toThrow('subscriptions unavailable');
    f.ctx.ws = {};
    expect((await talk(f.ctx)).statusCode).toBe(200);
    expect(f.executeApiTask).toHaveBeenCalledTimes(2);
  });

  it('abandons when async task creation throws before execution', async () => {
    const f = makeCtx();
    await readyProbe(f);
    f.create.mockImplementationOnce(() => { throw new Error('store create failed'); });
    await expect(talk(f.ctx, { async: true })).rejects.toThrow('store create failed');
    expect((await talk(f.ctx)).statusCode).toBe(200);
    expect(f.executeApiTask).toHaveBeenCalledTimes(2);
  });

  it('abandons when the initial async running update throws', async () => {
    const f = makeCtx();
    await readyProbe(f);
    f.update.mockImplementationOnce(() => { throw new Error('store running failed'); });
    expect((await talk(f.ctx, { async: true })).statusCode).toBe(202);
    await flush();
    expect((await talk(f.ctx)).statusCode).toBe(200);
    expect(f.executeApiTask).toHaveBeenCalledTimes(2);
  });

  it('does not turn a successful primary into a failure when its result store throws', async () => {
    const f = makeCtx();
    await readyProbe(f);
    f.update.mockImplementation((_id, patch) => {
      if (patch.status === 'completed') throw new Error('store result failed');
    });
    expect((await talk(f.ctx, { async: true })).statusCode).toBe(202);
    await flush();
    expect(f.ctx.circuitBreaker.getStatus().bot).toEqual({ state: 'half-open', failures: 1 });
    expect((await talk(f.ctx)).statusCode).toBe(200);
    expect(f.ctx.circuitBreaker.getStatus().bot).toEqual({ state: 'closed', failures: 0 });
  });

  it('contains failures while persisting an async execution error', async () => {
    const f = makeCtx();
    await readyProbe(f);
    f.executeApiTask.mockRejectedValueOnce(new Error('provider disconnected'));
    f.update.mockImplementation((_id, patch) => {
      if (patch.status === 'failed') throw new Error('store failure failed');
    });
    expect((await talk(f.ctx, { async: true })).statusCode).toBe(202);
    await flush();
    expect(f.ctx.circuitBreaker.getStatus().bot).toEqual({ state: 'open', failures: 2 });
    expect(logger.error).toHaveBeenCalled();
  });

  it('does not let an old callback failure release or fail a newer probe', async () => {
    const f = makeCtx();
    await readyProbe(f);
    let rejectCallback = (_err: Error) => {};
    f.callback.mockImplementationOnce(() => new Promise((_resolve, reject) => { rejectCallback = reject; }));
    await talk(f.ctx, { async: true, callbackChatId: 'callback-chat', callbackBotName: 'callback' });
    await flush();
    expect(f.callback).toHaveBeenCalledTimes(1);
    let finishProbe = (_result: { success: boolean; responseText: string }) => {};
    f.executeApiTask.mockImplementationOnce(() => new Promise((resolve) => { finishProbe = resolve; }));
    const probe = talk(f.ctx, { chatId: 'second' });
    await flush();
    rejectCallback(new Error('callback delivery failed'));
    await flush();
    expect((await talk(f.ctx, { chatId: 'third' })).statusCode).toBe(503);
    expect(f.ctx.circuitBreaker.getStatus().bot).toEqual({ state: 'half-open', failures: 1 });
    finishProbe({ success: true, responseText: 'recovered' });
    expect((await probe).statusCode).toBe(200);
    expect(f.ctx.circuitBreaker.getStatus().bot).toEqual({ state: 'closed', failures: 0 });
  });

  it('does not treat a WebSocket delivery failure as primary execution failure', async () => {
    const f = makeCtx();
    await readyProbe(f);
    f.ctx.ws = { handle: { subscriptions: {
      getSubscribers: () => new Set(['subscriber']),
      broadcast: () => { throw new Error('subscriber disconnected'); },
    } } } as any;
    f.executeApiTask.mockImplementationOnce(async ({ onUpdate }) => {
      onUpdate({ status: 'complete', responseText: 'done', toolCalls: [] }, 'message', true);
      return { success: true, responseText: 'done' };
    });
    expect((await talk(f.ctx)).statusCode).toBe(200);
    expect(f.ctx.circuitBreaker.getStatus().bot).toEqual({ state: 'half-open', failures: 1 });
  });

  it('ignores a closed-era success after a newer probe is acquired', async () => {
    const f = makeCtx();
    let finishOld = (_result: { success: boolean; responseText: string }) => {};
    f.executeApiTask.mockImplementationOnce(() => new Promise((resolve) => { finishOld = resolve; }));
    const old = talk(f.ctx, { chatId: 'old' });
    await flush();
    await readyProbe(f);
    let finishProbe = (_result: { success: boolean; responseText: string }) => {};
    f.executeApiTask.mockImplementationOnce(() => new Promise((resolve) => { finishProbe = resolve; }));
    const probe = talk(f.ctx, { chatId: 'probe' });
    await flush();
    finishOld({ success: true, responseText: 'old success' });
    await old;
    expect((await talk(f.ctx, { chatId: 'third' })).statusCode).toBe(503);
    finishProbe({ success: true, responseText: 'new success' });
    await probe;
  });

  it('ignores an execution failure after manual reset', async () => {
    const f = makeCtx();
    let finishOld = (_result: { success: boolean; responseText: string }) => {};
    f.executeApiTask.mockImplementationOnce(() => new Promise((resolve) => { finishOld = resolve; }));
    const old = talk(f.ctx);
    await flush();
    f.ctx.circuitBreaker.reset('bot');
    finishOld({ success: false, responseText: 'old failure' });
    await old;
    expect(f.ctx.circuitBreaker.getStatus()).toEqual({});
    expect((await talk(f.ctx)).statusCode).toBe(200);
  });
});
