import { readFileSync } from 'node:fs';
import { join } from 'node:path';

export interface ModelOption {
  id: string;
  label: string;
  note?: string;
}

const DEFAULT_CLAUDE_MODELS: readonly ModelOption[] = [
  { id: 'claude-fable-5', label: 'Fable 5', note: 'Latest Claude Code model · 1M context · 128k max output · adaptive thinking' },
  { id: 'claude-opus-4-8', label: 'Opus 4.8', note: 'High-capability legacy default · 200k context · 128k max output' },
  { id: 'claude-opus-4-8[1m]', label: 'Opus 4.8 (1M)', note: '1M context window' },
  { id: 'claude-opus-4-7', label: 'Opus 4.7', note: '200k context' },
  { id: 'claude-opus-4-7[1m]', label: 'Opus 4.7 (1M)', note: '1M context window' },
  { id: 'claude-opus-4-6', label: 'Opus 4.6', note: '200k context' },
  { id: 'claude-opus-4-6[1m]', label: 'Opus 4.6 (1M)', note: '1M context window' },
  { id: 'claude-sonnet-4-6', label: 'Sonnet 4.6', note: 'Balanced · 200k context' },
  { id: 'claude-sonnet-4-6[1m]', label: 'Sonnet 4.6 (1M)', note: '1M context window' },
  { id: 'claude-haiku-4-5', label: 'Haiku 4.5', note: 'Fastest · 200k context' },
];

const DEFAULT_MODEL_ENV_KEYS = [
  ['ANTHROPIC_DEFAULT_OPUS_MODEL', 'ANTHROPIC_DEFAULT_OPUS_MODEL_NAME'],
  ['ANTHROPIC_DEFAULT_SONNET_MODEL', 'ANTHROPIC_DEFAULT_SONNET_MODEL_NAME'],
  ['ANTHROPIC_DEFAULT_HAIKU_MODEL', 'ANTHROPIC_DEFAULT_HAIKU_MODEL_NAME'],
  ['ANTHROPIC_DEFAULT_FABLE_MODEL', 'ANTHROPIC_DEFAULT_FABLE_MODEL_NAME'],
  ['ANTHROPIC_MODEL', undefined],
  ['CLAUDE_MODEL', undefined],
] as const;

function clean(value: unknown): string | undefined {
  return typeof value === 'string' && value.trim() ? value.trim() : undefined;
}

function fromEnvironment(environment: Readonly<Record<string, string | undefined>>): ModelOption[] {
  const models: ModelOption[] = [];
  for (const [idKey, nameKey] of DEFAULT_MODEL_ENV_KEYS) {
    const id = clean(environment[idKey]);
    if (!id) continue;
    const label = (nameKey ? clean(environment[nameKey]) : undefined) ?? id;
    models.push({ id, label, note: 'Available via the configured Anthropic-compatible endpoint' });
  }
  return models;
}

function readSettingsEnvironment(environment: Readonly<Record<string, string | undefined>>): Record<string, string | undefined> {
  const home = clean(environment.HOME) ?? clean(environment.USERPROFILE);
  const configDir = clean(environment.CLAUDE_CONFIG_DIR)
    ?? (home ? join(home, '.claude') : undefined);
  if (!configDir) return {};
  try {
    const parsed: unknown = JSON.parse(readFileSync(join(configDir, 'settings.json'), 'utf8'));
    if (!parsed || typeof parsed !== 'object' || !('env' in parsed)) return {};
    const env = parsed.env;
    if (!env || typeof env !== 'object') return {};
    const result: Record<string, string | undefined> = {};
    for (const [key, value] of Object.entries(env)) {
      if (typeof value === 'string') result[key] = value;
    }
    return result;
  } catch {
    return {};
  }
}

function dedupe(models: readonly ModelOption[]): ModelOption[] {
  const seen = new Set<string>();
  const result: ModelOption[] = [];
  for (const model of models) {
    if (seen.has(model.id)) continue;
    seen.add(model.id);
    result.push(model);
  }
  return result;
}

export function claudeModelCatalog(options: {
  override?: readonly string[];
  env?: Readonly<Record<string, string | undefined>>;
}): ModelOption[] {
  const environment = options.env ?? {};
  const override = (options.override ?? [])
    .map((id) => clean(id))
    .filter((id): id is string => Boolean(id));
  if (override.length > 0) {
    return dedupe(override.map((id) => ({ id, label: id })));
  }
  const effectiveEnvironment = { ...environment, ...readSettingsEnvironment(environment) };
  const configured = dedupe(fromEnvironment(effectiveEnvironment));
  if (configured.length > 0) return configured;
  return dedupe(DEFAULT_CLAUDE_MODELS);
}
