import { mkdtempSync, rmSync, writeFileSync } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { afterEach, beforeEach, describe, it, expect, vi } from 'vitest';
import { CommandHandler } from '../src/bridge/command-handler.js';
import type { IncomingMessage } from '../src/types.js';
import type { UserSession } from '../src/engines/claude/session-manager.js';

/**
 * /help is the single user-discoverable entry point for "what can this bot
 * do?". When new top-level commands ship — particularly Claude Code
 * built-ins that pass through (/goal, /background) — /help must mention
 * them or users have no way to find them short of reading the source.
 *
 * Test guards against the recurring regression: a major feature ships but
 * is invisible because /help wasn't updated. Treat a missing entry here
 * as a release blocker, not a doc oversight.
 */

interface RecordedNotice {
  chatId: string;
  title:  string;
  content: string;
  color?: string;
}

function buildHandler(availableModels?: string[]) {
  const notices: RecordedNotice[] = [];
  const session: UserSession = {
    sessionId: undefined,
    workingDirectory: '/tmp',
    lastUsed: Date.now(),
    cumulativeTokens: 0,
    cumulativeCostUsd: 0,
    cumulativeDurationMs: 0,
  };
  const sender = {
    sendCard:        async () => undefined,
    updateCard:      async () => true,
    sendTextNotice:  async (chatId: string, title: string, content: string, color?: string) => {
      notices.push({ chatId, title, content, color });
    },
    sendText:        async () => {},
    sendImageFile:   async () => true,
    sendLocalFile:   async () => true,
    downloadImage:   async () => true,
    downloadFile:    async () => true,
  };
  const audit = { log: () => {} } as any;
  const handler = new CommandHandler(
    {
      name: 'test-bot',
      engine: 'claude',
      availableModels,
      claude: { model: 'claude-fable-5' },
    } as any,
    { info: () => {}, warn: () => {}, error: () => {}, debug: () => {} } as any,
    sender as any,
    {
      getSession: () => session,
      setSessionEngine: () => {},
      setSessionModel: (_chatId: string, model: string | undefined) => { session.model = model; },
    } as any,
    {} as any, // memoryClient — not touched
    audit,
    () => undefined, // getRunningTask
    () => {},        // stopTask
    () => 0,         // clearQueue — /help doesn't touch the queue
    async () => {},  // releaseExecutor
    () => [],        // listSessions
    async () => {},  // applyResume
  );
  return { handler, notices };
}

function helpMessage(): IncomingMessage {
  return {
    messageId:  'm1',
    chatId:     'c1',
    chatType:   'p2p',
    userId:     'u1',
    text:       '/help',
    timestamp:  Date.now(),
    isBotMentioned: true,
  } as IncomingMessage;
}

describe('CommandHandler endpoint catalog', () => {
  let directory: string;

  beforeEach(() => {
    directory = mkdtempSync(join(tmpdir(), 'metabot-handler-catalog-'));
    vi.stubEnv('CLAUDE_CONFIG_DIR', directory);
    for (const key of Object.keys(process.env)) {
      if (key.startsWith('ANTHROPIC_DEFAULT_') || key === 'ANTHROPIC_MODEL' || key === 'CLAUDE_MODEL') {
        vi.stubEnv(key, undefined);
      }
    }
    vi.stubEnv('ANTHROPIC_DEFAULT_OPUS_MODEL', 'shadowed-process-model');
    vi.stubEnv('ANTHROPIC_DEFAULT_OPUS_MODEL_NAME', 'Shadowed process label');
    vi.stubEnv('ANTHROPIC_DEFAULT_HAIKU_MODEL', 'inherited-process-model');
    vi.stubEnv('ANTHROPIC_DEFAULT_HAIKU_MODEL_NAME', 'Inherited process label');
    writeFileSync(join(directory, 'settings.json'), JSON.stringify({ env: {
      ANTHROPIC_DEFAULT_OPUS_MODEL: 'settings-model',
      ANTHROPIC_DEFAULT_OPUS_MODEL_NAME: 'Settings label',
    } }));
  });

  afterEach(() => {
    vi.unstubAllEnvs();
    rmSync(directory, { recursive: true, force: true });
  });

  it.each(['/model list', '/model ls', '/model'])(
    'passes process env and settings to the catalog for %s', async (text) => {
      const { handler, notices } = buildHandler();
      await handler.handle({ ...helpMessage(), text });
      const body = notices[0].content;
      expect(body).toContain('`settings-model`');
      expect(body).toContain('`inherited-process-model`');
      expect(body).not.toContain('shadowed-process-model');
      if (text !== '/model') {
        expect(body).toContain('Settings label');
        expect(body).toContain('Inherited process label');
        expect(body).not.toContain('Shadowed process label');
      }
    },
  );

  it('keeps per-handler overrides independent without changing execution defaults', async () => {
    const first = buildHandler(['first-model']);
    const second = buildHandler(['second-model']);
    for (const text of ['/model list', '/model']) {
      await first.handler.handle({ ...helpMessage(), text });
      await second.handler.handle({ ...helpMessage(), text });
    }
    for (const notice of first.notices) {
      expect(notice.content).toContain('`first-model`');
      expect(notice.content).not.toContain('`second-model`');
      expect(notice.content).not.toContain('`settings-model`');
      expect(notice.content).not.toContain('`inherited-process-model`');
    }
    for (const notice of second.notices) {
      expect(notice.content).toContain('`second-model`');
      expect(notice.content).not.toContain('`first-model`');
    }
    expect(first.notices[1].content).toContain('**Active:** `claude-fable-5`');
    await first.handler.handle({ ...helpMessage(), text: '/model selected-model' });
    await first.handler.handle({ ...helpMessage(), text: '/model' });
    expect(first.notices[3].content).toContain('**Active:** `selected-model` (session override)');
  });
});

describe('CommandHandler /help', () => {
  it('returns true and sends a notice on /help', async () => {
    const { handler, notices } = buildHandler();
    const handled = await handler.handle(helpMessage());
    expect(handled).toBe(true);
    expect(notices).toHaveLength(1);
    expect(notices[0].title).toContain('Help');
  });

  it('lists every bot-side command users can run', async () => {
    const { handler, notices } = buildHandler();
    await handler.handle(helpMessage());
    const body = notices[0].content;
    for (const cmd of ['/reset', '/new', '/stop', '/status', '/model', '/resume', '/memory', '/sync', '/help', '/?']) {
      expect(body, `help body missing ${cmd}`).toContain(cmd);
    }
  });

  it('/? is an alias for /help', async () => {
    const { handler, notices } = buildHandler();
    const handled = await handler.handle({ ...helpMessage(), text: '/?' });

    expect(handled).toBe(true);
    expect(notices).toHaveLength(1);
    expect(notices[0].title).toContain('Help');
  });

  // Regression — /goal and /background are Claude Code built-ins that pass
  // through to the agent. They are the user-visible surface for Agent Teams
  // continuity and background tasks; not mentioning them = users never find
  // them. Do not delete this assertion.
  it('mentions Claude Code passthrough commands /goal and /background (regression)', async () => {
    const { handler, notices } = buildHandler();
    await handler.handle(helpMessage());
    const body = notices[0].content;
    expect(body, '/goal must appear in /help').toContain('/goal');
    expect(body, '/background must appear in /help').toContain('/background');
  });

  it('returns false for non-slash messages so they reach the agent', async () => {
    const { handler } = buildHandler();
    const handled = await handler.handle({ ...helpMessage(), text: 'hello' });
    expect(handled).toBe(false);
  });

  it('keeps proxy context guidance provider-neutral and lists configured aliases', async () => {
    const directory = mkdtempSync(join(tmpdir(), 'metabot-handler-process-'));
    vi.stubEnv('CLAUDE_CONFIG_DIR', directory);
    vi.stubEnv('ANTHROPIC_DEFAULT_OPUS_MODEL', 'endpoint-test-model');
    try {
      const { handler, notices } = buildHandler();
      await handler.handle({ ...helpMessage(), text: '/model list' });
      const body = notices[0].content;
      expect(body).toContain('Available Claude models');
      expect(body).toContain('endpoint-test-model');
      expect(body).toContain('Context limits and model ID suffixes depend on your endpoint.');
      expect(body).not.toContain('append `[1m]`');
    } finally {
      vi.unstubAllEnvs();
      rmSync(directory, { recursive: true, force: true });
    }
  });
});
