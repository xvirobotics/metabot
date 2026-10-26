import { afterEach, describe, expect, it, vi } from 'vitest';
import { createLogger } from '../src/utils/logger.js';
import { ClaudeExecutor } from '../src/engines/claude/executor.js';
import { PersistentClaudeExecutor } from '../src/engines/claude/persistent-executor.js';
import { ptyQuery } from '../src/engines/claude/pty/pty-query.js';
import { query } from '@anthropic-ai/claude-agent-sdk';
import { spawn } from 'node:child_process';

vi.mock('node:child_process', async (original) => ({
  ...await original<typeof import('node:child_process')>(),
  spawn: vi.fn(() => ({})),
}));
vi.mock('@anthropic-ai/claude-agent-sdk', () => ({
  query: vi.fn(({ prompt }) => (async function* () { for await (const _ of prompt) yield { type: 'result' }; })()),
}));
vi.mock('../src/engines/claude/pty/pty-query.js', () => ({
  ptyQuery: vi.fn(({ prompt }) => (async function* () { for await (const _ of prompt) yield { type: 'result' }; })()),
}));

const logger = createLogger('silent');
const apiContext = {
  botName: 'private-bot', chatId: 'oc_private', sessionId: 'session_private',
  teamContext: 'PRIVATE_TEAM_CONTEXT', groupMembers: ['private-bot', 'private-peer'], groupId: 'private-group',
};
const outputsDir = '/tmp/output/oc_private';
function assertAppend(append: string) {
  expect(append).toContain('$ASSISTANT_OUTPUTS_DIR');
  for (const value of [outputsDir, ...Object.values(apiContext).flat(), 'metabot schedule', 'metabot talk']) {
    expect(append).not.toContain(value);
  }
}
afterEach(() => vi.clearAllMocks());

describe('Claude executor context boundary', () => {
  it('passes the legacy SDK output directory through the real spawn callback only', () => {
    const executor = new ClaudeExecutor({ name: 'private-bot', claude: { defaultWorkingDirectory: '/tmp' } } as any, logger);
    const options = (executor as any).buildQueryOptions('/tmp', undefined, new AbortController(), outputsDir, apiContext);
    assertAppend(options.systemPrompt.append);
    options.spawnClaudeCodeProcess({ command: 'unused-claude', args: [], env: { ASSISTANT_OUTPUTS_DIR: '/wrong-session' } });
    expect(vi.mocked(spawn).mock.calls.at(-1)?.[2]?.env?.ASSISTANT_OUTPUTS_DIR).toBe(outputsDir);
  });

  it.each(['pty', 'sdk'] as const)('isolates context at the actual persistent %s startup boundary', async (backend) => {
    const executor = new PersistentClaudeExecutor({ cwd: '/tmp', logger, apiContext, outputsDir, backend, idleTimeoutMs: 0 });
    await executor.start();
    try {
      const options = backend === 'pty'
        ? vi.mocked(ptyQuery).mock.calls.at(-1)![0].options
        : vi.mocked(query).mock.calls.at(-1)![0].options;
      assertAppend((options as any).systemPrompt.append);
      if (backend === 'pty') {
        expect((options as any).env.ASSISTANT_OUTPUTS_DIR).toBe(outputsDir);
      } else {
        (options as any).spawnClaudeCodeProcess({ command: 'unused-claude', args: [], env: { ASSISTANT_OUTPUTS_DIR: '/wrong-session' } });
        expect(vi.mocked(spawn).mock.calls.at(-1)?.[2]?.env?.ASSISTANT_OUTPUTS_DIR).toBe(outputsDir);
      }
    } finally {
      await executor.shutdown();
    }
  });
});
