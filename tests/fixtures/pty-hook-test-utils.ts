import { spawn } from 'node:child_process';
import { copyFileSync, existsSync, linkSync, mkdirSync, mkdtempSync, readFileSync } from 'node:fs';
import { tmpdir } from 'node:os';
import { dirname, join } from 'node:path';
import { setTimeout } from 'node:timers/promises';
import { createHookBridge } from '../../src/engines/claude/pty/hook-bridge.js';

export function createHostileRuntime(): { root: string; dir: string; nodePath: string } {
  const root = mkdtempSync(join(tmpdir(), 'metabot-hook-'));
  const dir = join(root, "it's $HOOK_SUFFIX $(touch pwned) `touch pwned` %HOOK_SUFFIX% !1 ${CLAUDE_PROJECT_DIR}");
  mkdirSync(dir);
  const nodePath = join(dir, process.platform === 'win32' ? 'node.exe' : 'node');
  try {
    linkSync(process.execPath, nodePath);
  } catch (error) {
    if (!(error instanceof Error) || !('code' in error) || error.code !== 'EXDEV') throw error;
    copyFileSync(process.execPath, nodePath);
  }
  return { root, dir, nodePath };
}

export async function runGeneratedHook(
  runtime: ReturnType<typeof createHostileRuntime>,
  event: 'Stop' | 'PostCompact',
  payload: string,
) {
  const originalNode = process.execPath;
  const originalTmp = { TMPDIR: process.env.TMPDIR, TEMP: process.env.TEMP, TMP: process.env.TMP };
  const bridge = (() => {
    process.env.TMPDIR = process.env.TEMP = process.env.TMP = runtime.dir;
    try {
      return createHookBridge();
    } finally {
      for (const [key, value] of Object.entries(originalTmp)) {
        if (value === undefined) delete process.env[key];
        else process.env[key] = value;
      }
    }
  })();
  let turns = 0;
  let compactions = 0;
  bridge.onTurnComplete(() => { turns++; });
  bridge.onCompactComplete?.(() => { compactions++; });
  try {
    let settingsPath: string;
    process.execPath = runtime.nodePath;
    try {
      settingsPath = await bridge.writeSettings();
    } finally {
      process.execPath = originalNode;
    }
    const settings = JSON.parse(readFileSync(settingsPath, 'utf8'));
    const hook = settings.hooks[event][0].hooks[0];
    const result = await new Promise<{ code: number | null; error: string | null; stderr: string }>(resolve => {
      const child = spawn(hook.command, hook.args ?? [], {
        cwd: runtime.dir, shell: false, timeout: 10000,
        env: { ...process.env, HOOK_SUFFIX: 'expanded', CLAUDE_PROJECT_DIR: 'expanded-project' },
        stdio: ['pipe', 'ignore', 'pipe'],
      });
      let stderr = '';
      let error: string | null = null;
      child.stderr.on('data', chunk => { stderr += chunk; });
      child.on('error', cause => { error = cause.message; });
      child.stdin.on('error', cause => { error = cause.message; });
      child.on('close', code => resolve({ code, error, stderr }));
      child.stdin.end(payload);
    });
    await setTimeout(200);
    const flag = join(dirname(settingsPath), event === 'Stop' ? 'stop.flag' : 'compact.flag');
    return {
      ...result,
      execForm: Array.isArray(hook.args),
      command: hook.command,
      opaquePath: /^[0-9a-f]+$/.test(hook.args?.[2] ?? ''),
      payload: existsSync(flag) ? readFileSync(flag, 'utf8') : null,
      turns,
      compactions,
      injectedFile: existsSync(join(runtime.dir, 'pwned')),
      manualCompact: settings.hooks.PostCompact[0].matcher,
    };
  } finally {
    await bridge.dispose();
  }
}
