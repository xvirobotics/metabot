import { mkdtempSync, rmSync, writeFileSync } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest';
import { loadAppConfig } from '../src/config.js';
import { claudeModelCatalog } from '../src/bridge/model-catalog.js';

const platforms = [
  ['feishuBots', { feishuAppId: 'test-app', feishuAppSecret: 'test-secret' }],
  ['telegramBots', { telegramBotToken: 'test-token' }],
  ['webBots', {}],
  ['wechatBots', { wechatBotToken: 'test-token' }],
  ['slackBots', { slackBotToken: 'test-token', slackSigningSecret: 'test-secret' }],
] as const;

describe('loadAppConfig model catalogs', () => {
  let directory: string;

  beforeEach(() => {
    directory = mkdtempSync(join(tmpdir(), 'metabot-model-config-'));
    vi.stubEnv('BOTS_CONFIG', join(directory, 'bots.json'));
    vi.stubEnv('CLAUDE_MODEL', 'execution-env-model');
  });

  afterEach(() => {
    vi.unstubAllEnvs();
    rmSync(directory, { recursive: true, force: true });
  });

  it.each(platforms)('keeps explicit and inherited catalogs independent for %s', (key, credentials) => {
    const bots = [
      { availableModels: [' first-model ', 'first-model', 'claude-fable-5[1M]', ' '] },
      { availableModels: ['second-model'] },
      { availableModels: [] },
      {},
      { availableModels: [' '] },
      { availableModels: ['claude-fable-5[1M]'] },
    ].map((entry, index) => ({
      ...credentials,
      ...entry,
      name: `bot-${index}`,
      engine: 'claude',
      defaultWorkingDirectory: directory,
      ...(index === 0 ? { model: 'execution-bot-model' } : {}),
    }));
    writeFileSync(join(directory, 'bots.json'), JSON.stringify({ [key]: bots }));
    writeFileSync(join(directory, 'settings.json'), JSON.stringify({ env: {
      ANTHROPIC_DEFAULT_OPUS_MODEL: 'settings-model',
    } }));

    const config = loadAppConfig();
    const loaded = config[key];
    const env = {
      CLAUDE_CONFIG_DIR: directory,
      ANTHROPIC_DEFAULT_HAIKU_MODEL: 'inherited-model',
    };
    const catalogs = loaded.map((bot) => claudeModelCatalog({ override: bot.availableModels, env })
      .map(({ id }) => id));

    expect(loaded).toHaveLength(6);
    expect(loaded.map((bot) => bot.availableModels)).toEqual([
      ['first-model', 'claude-fable-5[1M]'], ['second-model'], undefined, undefined, [],
      ['claude-fable-5[1M]'],
    ]);
    expect(catalogs).toEqual([
      ['first-model', 'claude-fable-5[1M]'], ['second-model'],
      ['settings-model', 'inherited-model'],
      ['settings-model', 'inherited-model'],
      ['settings-model', 'inherited-model'],
      ['claude-fable-5[1M]'],
    ]);
    expect(loaded.map((bot) => bot.claude.model)).toEqual([
      'execution-bot-model', 'execution-env-model', 'execution-env-model',
      'execution-env-model', 'execution-env-model', 'execution-env-model',
    ]);
    loaded[0].availableModels?.push('first-only');
    expect(loaded[1].availableModels).toEqual(['second-model']);
    expect(loadAppConfig()[key][0].availableModels).toEqual(['first-model', 'claude-fable-5[1M]']);
  });
});
