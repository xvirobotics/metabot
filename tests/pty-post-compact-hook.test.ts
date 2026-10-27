import { rmSync } from 'node:fs';
import { afterAll, beforeAll, describe, expect, it } from 'vitest';
import { buildPostCompactHook } from '../src/engines/claude/pty/hook-bridge.js';
import { createHostileRuntime, runGeneratedHook } from './fixtures/pty-hook-test-utils.js';

let runtime: ReturnType<typeof createHostileRuntime>;
beforeAll(() => { runtime = createHostileRuntime(); });
afterAll(() => { rmSync(runtime.root, { recursive: true, force: true }); });

describe('PostCompact hook', () => {
  it('uses exec form with opaque arguments instead of a shell command string', () => {
    const hook = buildPostCompactHook("C:\\Users\\a$b\\it's ` ${CLAUDE_PROJECT_DIR}\\compact.flag", 'C:\\Program Files\\nodejs\\node.exe');
    expect(hook.command).toBe('C:\\Program Files\\nodejs\\node.exe');
    expect(hook).not.toHaveProperty('shell');
    expect(hook.args).toHaveLength(3);
    expect(hook.args[2]).toMatch(/^[0-9a-f]+$/);
    expect(hook.args.join(' ')).not.toMatch(/[$`%"!]/);
  });

  it.each([
    JSON.stringify({ hook_event_name: 'PostCompact', trigger: 'manual' }),
    '',
  ])('executes generated settings at literal hostile executable and temp paths with stdin %j', async payload => {
    const result = await runGeneratedHook(runtime, 'PostCompact', payload);
    expect(result).toEqual({
      code: 0,
      error: null,
      stderr: '',
      execForm: true,
      command: runtime.nodePath,
      opaquePath: true,
      payload: payload || 'compact',
      turns: 0,
      compactions: expect.any(Number),
      injectedFile: false,
      manualCompact: 'manual',
    });
    expect(result).not.toMatchObject({ compactions: 0 });
  }, 30000);
});
