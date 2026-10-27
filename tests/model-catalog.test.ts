import { mkdtempSync, mkdirSync, rmSync, writeFileSync } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { afterEach, beforeEach, describe, it, expect } from 'vitest';
import { claudeModelCatalog } from '../src/bridge/model-catalog.js';

describe('claudeModelCatalog', () => {
  let directory: string;

  beforeEach(() => {
    directory = mkdtempSync(join(tmpdir(), 'metabot-model-catalog-'));
  });

  afterEach(() => {
    rmSync(directory, { recursive: true, force: true });
  });

  function settings(env: unknown): string {
    writeFileSync(join(directory, 'settings.json'), JSON.stringify({ env }));
    return directory;
  }

  it('overrides process aliases by key before extracting and deduplicating IDs', () => {
    expect(claudeModelCatalog({
      env: {
        CLAUDE_CONFIG_DIR: settings({
          ANTHROPIC_DEFAULT_OPUS_MODEL: 'settings-model',
          ANTHROPIC_DEFAULT_SONNET_MODEL: 'settings-model',
        }),
        ANTHROPIC_DEFAULT_OPUS_MODEL: 'shadowed-opus',
        ANTHROPIC_DEFAULT_SONNET_MODEL: 'shadowed-sonnet',
        ANTHROPIC_DEFAULT_HAIKU_MODEL: 'inherited-haiku',
        ANTHROPIC_MODEL: 'inherited-anthropic',
        CLAUDE_MODEL: 'inherited-claude',
      },
    }).map(({ id }) => id)).toEqual([
      'settings-model', 'inherited-haiku', 'inherited-anthropic', 'inherited-claude',
    ]);
  });

  it('merges label keys before extracting models even when settings defines no ID', () => {
    expect(claudeModelCatalog({
      env: {
        CLAUDE_CONFIG_DIR: settings({ ANTHROPIC_DEFAULT_OPUS_MODEL_NAME: 'Settings label' }),
        ANTHROPIC_DEFAULT_OPUS_MODEL: 'process-opus',
        ANTHROPIC_DEFAULT_OPUS_MODEL_NAME: 'Shadowed label',
      },
    })).toEqual([expect.objectContaining({ id: 'process-opus', label: 'Settings label' })]);
  });

  it('retains an inherited label key when settings overrides only its model ID', () => {
    expect(claudeModelCatalog({
      env: {
        CLAUDE_CONFIG_DIR: settings({ ANTHROPIC_DEFAULT_OPUS_MODEL: 'settings-opus' }),
        ANTHROPIC_DEFAULT_OPUS_MODEL: 'shadowed-opus',
        ANTHROPIC_DEFAULT_OPUS_MODEL_NAME: 'Inherited label',
      },
    })).toEqual([expect.objectContaining({ id: 'settings-opus', label: 'Inherited label' })]);
  });

  it.each([
    ['missing file', undefined],
    ['invalid JSON', '{'],
    ['null document', 'null'],
    ['missing env', '{}'],
    ['null env', '{"env":null}'],
    ['non-object env', '{"env":42}'],
    ['non-string aliases', '{"env":{"ANTHROPIC_DEFAULT_OPUS_MODEL":42,"ANTHROPIC_DEFAULT_OPUS_MODEL_NAME":false}}'],
  ])('retains process models with %s settings', (_name, contents) => {
    if (contents !== undefined) writeFileSync(join(directory, 'settings.json'), contents);
    expect(claudeModelCatalog({ env: {
      CLAUDE_CONFIG_DIR: directory,
      ANTHROPIC_DEFAULT_OPUS_MODEL: 'process-opus',
      ANTHROPIC_DEFAULT_OPUS_MODEL_NAME: 'Process label',
    } })).toEqual([expect.objectContaining({ id: 'process-opus', label: 'Process label' })]);
  });

  it('reads settings from USERPROFILE when HOME and CLAUDE_CONFIG_DIR are absent', () => {
    mkdirSync(join(directory, '.claude'));
    writeFileSync(join(directory, '.claude', 'settings.json'), JSON.stringify({ env: {
      ANTHROPIC_DEFAULT_OPUS_MODEL: 'windows-profile-model',
    } }));
    expect(claudeModelCatalog({ env: { USERPROFILE: directory } }).map(({ id }) => id))
      .toEqual(['windows-profile-model']);
  });

  it('derives the list from ANTHROPIC_DEFAULT_*_MODEL aliases', () => {
    const models = claudeModelCatalog({
      env: {
        ANTHROPIC_DEFAULT_OPUS_MODEL: 'kimi-k3',
        ANTHROPIC_DEFAULT_OPUS_MODEL_NAME: 'Kimi K3',
        ANTHROPIC_DEFAULT_SONNET_MODEL: 'glm-5.3',
      },
    });
    const ids = models.map((m) => m.id);
    expect(ids).toEqual(['kimi-k3', 'glm-5.3']);
    expect(models[0].label).toBe('Kimi K3');
  });

  it('preserves configured Fable IDs and variants across catalog sources', () => {
    for (const id of ['claude-fable-5', 'claude-fable-5[1m]', 'claude-fable-5[1M]']) {
      expect(claudeModelCatalog({ env: { ANTHROPIC_DEFAULT_FABLE_MODEL: id } })
        .map((model) => model.id)).toEqual([id]);
      const env = {
        CLAUDE_CONFIG_DIR: settings({ ANTHROPIC_DEFAULT_FABLE_MODEL: id }),
        ANTHROPIC_DEFAULT_FABLE_MODEL: 'shadowed-model',
        ANTHROPIC_DEFAULT_FABLE_MODEL_NAME: 'Inherited label',
      };
      expect(claudeModelCatalog({ env })).toEqual([expect.objectContaining({ id, label: 'Inherited label' })]);
      expect(claudeModelCatalog({ override: [id, id], env })
        .map((model) => model.id)).toEqual([id]);
    }
  });

  it('advertises the configured endpoint models without applying a global blacklist', () => {
    const models = claudeModelCatalog({
      env: {
        ANTHROPIC_DEFAULT_FABLE_MODEL: 'claude-fable-5',
        ANTHROPIC_DEFAULT_OPUS_MODEL: 'claude-opus-4-8',
      },
    });
    expect(models.map((m) => m.id)).toEqual(['claude-opus-4-8', 'claude-fable-5']);
  });

  it.each(['claude-fable-5[1m]', 'claude-fable-5[1M]'])(
    'preserves variant %s alongside another configured model', (variant) => {
      expect(claudeModelCatalog({ env: {
        ANTHROPIC_DEFAULT_FABLE_MODEL: variant,
        ANTHROPIC_DEFAULT_OPUS_MODEL: 'live-model',
      } }).map(({ id }) => id)).toEqual(['live-model', variant]);
      expect(claudeModelCatalog({ override: [variant, 'live-model'] }).map(({ id }) => id))
        .toEqual([variant, 'live-model']);
    },
  );

  it('per-bot override wins and is deduped', () => {
    const models = claudeModelCatalog({
      override: ['glm-5.3', 'glm-5.3', ' kimi-k3 '],
      env: { ANTHROPIC_DEFAULT_OPUS_MODEL: 'should-not-appear' },
    });
    expect(models.map((m) => m.id)).toEqual(['glm-5.3', 'kimi-k3']);
  });

  it('preserves native and proxy models in an explicit override', () => {
    const models = claudeModelCatalog({ override: ['claude-fable-5', 'glm-5.3'] });
    expect(models.map((m) => m.id)).toEqual(['claude-fable-5', 'glm-5.3']);
  });

  it('keeps upstream fallback IDs and native Fable 1M metadata when nothing is configured', () => {
    const models = claudeModelCatalog({ env: {} });
    expect(models.map((m) => m.id)).toEqual([
      'claude-fable-5', 'claude-opus-4-8', 'claude-opus-4-8[1m]',
      'claude-opus-4-7', 'claude-opus-4-7[1m]', 'claude-opus-4-6', 'claude-opus-4-6[1m]',
      'claude-sonnet-4-6', 'claude-sonnet-4-6[1m]', 'claude-haiku-4-5',
    ]);
    expect(models[0]).toEqual({ id: 'claude-fable-5', label: 'Fable 5',
      note: 'Latest Claude Code model · 1M context · 128k max output · adaptive thinking' });
  });
});
