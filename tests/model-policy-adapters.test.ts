import { EventEmitter } from 'node:events';
import { PassThrough } from 'node:stream';
import { afterEach, describe, expect, it, vi } from 'vitest';
import type { BotConfigBase } from '../src/config.js';
import type { ExecutorOptions } from '../src/engines/claude/executor.js';

const external = vi.hoisted(() => ({ query: vi.fn(), spawn: vi.fn(), app: vi.fn(), persistent: vi.fn() }));
vi.mock('@anthropic-ai/claude-agent-sdk', () => ({ query: external.query }));
vi.mock('node:child_process', async importOriginal => ({ ...await importOriginal<typeof import('node:child_process')>(), spawn: external.spawn, execSync: vi.fn(() => 'codex-test.exe') }));
vi.mock('../src/engines/claude/resolve-claude.js', () => ({ resolveClaudePath: () => 'claude-test' }));
vi.mock('../src/engines/codex/app-server.js', () => ({ startAppServerRun: external.app }));
vi.mock('../src/engines/claude/persistent-executor.js', async () => {
  const { EventEmitter } = await import('node:events');
  return { PersistentClaudeExecutor: class extends EventEmitter {
    state = 'ready';
    constructor(options: unknown) { super(); external.persistent(options); }
    async start() {}
    getState() { return this.state; }
    getSessionId() { return 'persisted-session'; }
    getLastActivityAt() { return Date.now(); }
    hasActiveTurn() { return false; }
    async shutdown() { this.state = 'closed'; this.emit('closed'); }
  } };
});
import { ClaudeExecutor } from '../src/engines/claude/executor.js';
import { ExecutorRegistry } from '../src/engines/claude/executor-registry.js';
import { CodexExecutor } from '../src/engines/codex/executor.js';
import { KimiExecutor, type KimiClientLike } from '../src/engines/kimi/executor.js';

const logger = { debug() {}, info() {}, warn() {}, error() {} };
const config: BotConfigBase = {
  name: 'policy', engine: 'claude',
  claude: { defaultWorkingDirectory: process.cwd(), outputsBaseDir: process.cwd(), downloadsDir: process.cwd(), model: 'configured-claude', apiKey: undefined, maxTurns: undefined, maxBudgetUsd: undefined, backend: 'sdk' },
  codex: { executable: 'codex-test', model: 'configured-codex', transport: 'exec', serviceTier: 'fast' },
  kimi: { model: 'configured-kimi' },
};
function options(): ExecutorOptions { return { cwd: process.cwd(), prompt: 'work', abortController: new AbortController(), modelPolicy: { kind: 'endpoint-default' }, maxTurns: 1, allowedTools: [] }; }
afterEach(() => vi.clearAllMocks());

describe('model policy at external engine boundaries', () => {
  it.each(['turn', 'execute'] as const)('Claude %s omits MetaBot model but retains native settings and restrictions', async mode => {
    external.query.mockImplementation(() => (async function* () {})());
    const executor = new ClaudeExecutor(config, logger);
    if (mode === 'turn') { const handle = executor.startExecution(options()); handle.finish(); }
    else for await (const _message of executor.execute(options())) { void _message; }
    const queryOptions = external.query.mock.calls[0][0].options;
    expect(queryOptions).not.toHaveProperty('model');
    expect(queryOptions).toMatchObject({ maxTurns: 1, allowedTools: [], settingSources: ['user', 'project'] });
    executor.startExecution({ ...options(), modelPolicy: { kind: 'named', name: 'selected' } }).finish();
    expect(external.query.mock.calls.at(-1)?.[0].options.model).toBe('selected');
  });

  it('Codex exec omits -m instead of reapplying bot or metadata default and preserves native controls', () => {
    const child = Object.assign(new EventEmitter(), { stdout: new PassThrough(), stderr: new PassThrough(), kill: vi.fn(), killed: false, exitCode: null, signalCode: null });
    external.spawn.mockReturnValue(child);
    const executor = new CodexExecutor(config, logger);
    executor.startExecution({ ...options(), serviceTier: 'flex', reasoningEffort: 'high' }).finish();
    const [, args] = external.spawn.mock.calls[0];
    expect(args).not.toContain('-m'); expect(args).not.toContain('configured-codex');
    expect(args).toContain('service_tier="flex"'); expect(args).toContain('model_reasoning_effort="high"');
    executor.startExecution({ ...options(), modelPolicy: { kind: 'named', name: 'selected' } }).finish();
    expect(external.spawn.mock.calls[1][1]).toContain('selected');
  });

  it('Codex app-server omits model on both CLI and RPC boundary', () => {
    external.app.mockReturnValue({ finish: vi.fn() });
    const executor = new CodexExecutor({ ...config, codex: { ...config.codex, transport: 'app-server' } }, logger);
    executor.startExecution({ ...options(), serviceTier: 'flex', reasoningEffort: 'high' }).finish();
    const input = external.app.mock.calls[0][0];
    expect(input.model).toBeUndefined(); expect(input.args).not.toContain('-m');
    expect(input).toMatchObject({ serviceTier: 'flex', effort: 'high', prompt: 'work' });
    executor.startExecution({ ...options(), modelPolicy: { kind: 'named', name: 'selected' } }).finish();
    expect(external.app.mock.calls[1][0].model).toBe('selected');
  });

  it('Kimi resolves native default and uses it for session and prompt instead of bot model', async () => {
    const snapshot = { session: { busy: false }, messages: { items: [] }, pending_approvals: [], pending_questions: [], in_flight_turn: null, subagents: [] };
    const client = {
      resolveModel: vi.fn(async () => ({ id: 'native-kimi', displayName: 'Native' })),
      openSession: vi.fn(async () => ({ id: 'session' })), getSnapshot: vi.fn(async () => snapshot),
      getStatus: vi.fn(async () => { throw new Error('not needed'); }), getGoal: vi.fn(),
      submitPrompt: vi.fn(async () => ({ prompt_id: 'prompt', status: 'running' as const })),
      steer: vi.fn(), abortSession: vi.fn(), setGoal: vi.fn(), controlGoal: vi.fn(), approve: vi.fn(), respondQuestion: vi.fn(),
    } as unknown as KimiClientLike;
    const executor = new KimiExecutor(config, logger, client);
    for await (const _message of executor.execute(options())) { void _message; }
    expect(client.resolveModel).toHaveBeenCalledWith(undefined);
    expect(client.openSession).toHaveBeenCalledWith(process.cwd(), undefined, 'native-kimi');
    expect(client.submitPrompt).toHaveBeenCalledWith('session', 'work', expect.objectContaining({ model: 'native-kimi' }));
  });

  it('registry changes model binding, reuses endpoint-default, and preserves it across crash respawn', async () => {
    const registry = new ExecutorRegistry({ logger, defaultModel: 'configured-claude' });
    const named = await registry.acquire('chat', { cwd: process.cwd(), modelPolicy: { kind: 'named', name: 'selected' } });
    expect(external.persistent.mock.calls[0][0].model).toBe('selected');
    const native = await registry.acquire('chat', { cwd: process.cwd(), modelPolicy: { kind: 'endpoint-default' } });
    expect(native).not.toBe(named); expect(external.persistent.mock.calls[1][0].model).toBeUndefined();
    expect(await registry.acquire('chat', { cwd: process.cwd(), modelPolicy: { kind: 'endpoint-default' } })).toBe(native);
    native.emit('crashed');
    Object.assign(native, { state: 'closed' }); native.emit('closed');
    const entry = (registry as any).executors.get('chat'); entry.nextRespawnAt = 0;
    const respawned = await registry.acquire('chat', { cwd: process.cwd(), modelPolicy: { kind: 'endpoint-default' } });
    expect(respawned).not.toBe(native); expect(external.persistent.mock.calls[2][0]).toMatchObject({ model: undefined, resumeSessionId: 'persisted-session' });
    await registry.shutdownAll('test');
  });
});
