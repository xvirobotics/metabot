import { rmSync } from 'node:fs';
import { afterAll, beforeAll, describe, expect, it } from 'vitest';
import { createHostileRuntime, runGeneratedHook } from './fixtures/pty-hook-test-utils.js';

let runtime: ReturnType<typeof createHostileRuntime>;
beforeAll(() => { runtime = createHostileRuntime(); });
afterAll(() => { rmSync(runtime.root, { recursive: true, force: true }); });

describe('Stop hook', () => {
  it.each([
    JSON.stringify({ hook_event_name: 'Stop', stop_hook_active: false, last_assistant_message: 'done $ ` ${literal}' }),
    '',
  ])('executes generated settings at literal hostile executable and temp paths with stdin %j', async payload => {
    expect(await runGeneratedHook(runtime, 'Stop', payload)).toEqual({
      code: 0,
      error: null,
      stderr: '',
      execForm: true,
      command: runtime.nodePath,
      opaquePath: true,
      payload: payload || 'stop',
      turns: 1,
      compactions: 0,
      injectedFile: false,
      manualCompact: 'manual',
    });
  }, 30000);
});
