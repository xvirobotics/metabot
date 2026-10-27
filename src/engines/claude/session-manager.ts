import * as fs from 'node:fs';
import * as path from 'node:path';
import * as os from 'node:os';
import type { Logger } from '../../utils/logger.js';
import type { EngineName } from '../types.js';
import type { CodexReasoningEffort } from '../../config.js';

export interface UserSession {
  sessionId: string | undefined;
  /** Engine that owns sessionId. Engine session stores are not interchangeable. */
  sessionIdEngine?: EngineName;
  /** Origin context was consumed or explicitly cleared with /reset. */
  threadContextInitialized?: boolean;
  workingDirectory: string;
  lastUsed: number;
  /** Cumulative token usage across all queries in this session */
  cumulativeTokens: number;
  /** Cumulative cost (USD) across all queries in this session */
  cumulativeCostUsd: number;
  /** Cumulative duration (ms) across all queries in this session */
  cumulativeDurationMs: number;
  /** Per-session model override (e.g. "claude-fable-5"). Falls back to bot default when undefined. */
  model?: string;
  /** Engine that owns model. Model names are engine-specific. */
  modelEngine?: EngineName;
  /** Per-session Codex reasoning effort override. */
  reasoningEffort?: CodexReasoningEffort;
  serviceTier?: 'fast' | 'flex' | 'default';
  /** Per-session engine override. Falls back to bot default when undefined. */
  engine?: EngineName;
  /**
   * Mirrored Claude /goal condition. The actual goal mechanism runs inside
   * Claude Code (prompt-based Stop hook); we just remember the text so the
   * Feishu card can show a persistent "🎯 Goal" badge across turns.
   */
  activeGoal?: string;
  /** Wall-clock when the current goal was set (ms since epoch). */
  goalSetAt?: number;
  /** Codex bridge-level goal loop iteration counter. Claude owns its own /goal loop. */
  goalIterations?: number;
  /** Codex bridge-level goal loop safety cap. */
  goalMaxIterations?: number;
}

export interface ModelPreferenceSelection {
  readonly model: string;
  readonly engine: EngineName | undefined;
  readonly revision: symbol;
}

interface PersistedSession {
  sessionId: string;
  sessionIdEngine?: EngineName;
  threadContextInitialized?: boolean;
  workingDirectory: string;
  lastUsed: number;
  cumulativeTokens?: number;
  cumulativeCostUsd?: number;
  cumulativeDurationMs?: number;
  model?: string;
  modelEngine?: EngineName;
  reasoningEffort?: CodexReasoningEffort;
  serviceTier?: 'fast' | 'flex' | 'default';
  engine?: EngineName;
  activeGoal?: string;
  goalSetAt?: number;
  goalIterations?: number;
  goalMaxIterations?: number;
}

// Sessions never expire — user can /reset manually.
// IMPORTANT: When switching a bot's defaultWorkingDirectory, do NOT delete
// session files (~/.metabot/<bot>/sessions-*.json, sessions.db).
// Old sessions must be preserved so the user can switch back to a previous
// project and resume context. loadFromDisk() uses the new defaultWorkingDirectory
// from config, not from the persisted session, so old sessions don't interfere.
const SESSION_TTL_MS = Infinity;
const MAX_SESSIONS = 10_000;
export const DEFAULT_CODEX_GOAL_MAX_ITERATIONS = 25;

export class SessionManager {
  private sessions = new Map<string, UserSession>();
  private modelRevisions = new WeakMap<UserSession, symbol>();
  private cleanupTimer: ReturnType<typeof setInterval>;
  private persistPath: string;

  constructor(
    private defaultWorkingDirectory: string,
    private logger: Logger,
    botName: string = 'default',
  ) {
    // Persist sessions to a file under the project data dir
    const dataDir = process.env.SESSION_STORE_DIR
      || path.join(os.homedir(), '.metabot');
    fs.mkdirSync(dataDir, { recursive: true });
    this.persistPath = path.join(dataDir, `sessions-${botName}.json`);

    this.loadFromDisk();

    // Periodic cleanup every hour
    this.cleanupTimer = setInterval(() => this.cleanupExpired(), 60 * 60 * 1000);
  }

  getSession(chatId: string): UserSession {
    let session = this.sessions.get(chatId);
    if (!session) {
      // Evict least-recently-used session if at capacity
      if (this.sessions.size >= MAX_SESSIONS) {
        this.evictOldest();
      }
      session = {
        sessionId: undefined,
        workingDirectory: this.defaultWorkingDirectory,
        lastUsed: Date.now(),
        cumulativeTokens: 0,
        cumulativeCostUsd: 0,
        cumulativeDurationMs: 0,
      };
      this.sessions.set(chatId, session);
    }
    session.lastUsed = Date.now();
    if (!fs.existsSync(session.workingDirectory) && fs.existsSync(this.defaultWorkingDirectory)) {
      this.logger.warn(
        { chatId, workingDirectory: session.workingDirectory, fallback: this.defaultWorkingDirectory },
        'Session working directory no longer exists; falling back to bot default',
      );
      session.workingDirectory = this.defaultWorkingDirectory;
      session.sessionId = undefined;
      session.sessionIdEngine = undefined;
      this.saveToDisk();
    }
    return session;
  }

  private evictOldest(): void {
    let oldestKey: string | undefined;
    let oldestTime = Infinity;
    for (const [key, s] of this.sessions) {
      if (s.lastUsed < oldestTime) {
        oldestTime = s.lastUsed;
        oldestKey = key;
      }
    }
    if (oldestKey) {
      this.sessions.delete(oldestKey);
      this.logger.debug({ chatId: oldestKey }, 'Evicted oldest session (capacity limit)');
    }
  }

  markThreadContextInitialized(chatId: string): void {
    this.getSession(chatId).threadContextInitialized = true;
    this.saveToDisk();
  }

  setSessionId(chatId: string, sessionId: string, engine?: EngineName): void {
    const session = this.getSession(chatId);
    session.sessionId = sessionId;
    session.sessionIdEngine = engine;
    this.logger.debug({ chatId, sessionId: sessionId.slice(0, 8), engine }, 'Session ID updated');
    this.saveToDisk();
  }

  /** Set per-session model override. Pass undefined to clear. */
  setSessionModel(chatId: string, model: string | undefined, engine?: EngineName): void {
    const session = this.getSession(chatId);
    session.model = model;
    session.modelEngine = model ? engine : undefined;
    this.modelRevisions.set(session, Symbol());
    this.logger.info({ chatId, model, engine: session.modelEngine }, 'Session model override updated');
    this.saveToDisk();
  }

  captureModelPreference(chatId: string): ModelPreferenceSelection | undefined {
    const session = this.getSession(chatId);
    if (!session.model) return undefined;
    let revision = this.modelRevisions.get(session);
    if (!revision) {
      revision = Symbol();
      this.modelRevisions.set(session, revision);
    }
    return { model: session.model, engine: session.modelEngine, revision };
  }

  clearModelPreferenceIfCurrent(chatId: string, selected: ModelPreferenceSelection): boolean {
    const session = this.sessions.get(chatId);
    if (!session || session.model !== selected.model || session.modelEngine !== selected.engine
      || this.modelRevisions.get(session) !== selected.revision) return false;
    this.setSessionModel(chatId, undefined);
    return true;
  }

  /** Set per-session Codex reasoning effort override. Pass undefined to clear. */
  setReasoningEffort(chatId: string, effort: CodexReasoningEffort | undefined): void {
    const session = this.getSession(chatId);
    session.reasoningEffort = effort;
    this.logger.info({ chatId, effort }, 'Session reasoning effort override updated');
    this.saveToDisk();
  }

  setServiceTier(chatId: string, tier: 'fast' | 'flex' | 'default' | undefined): void {
    const session = this.getSession(chatId);
    session.serviceTier = tier;
    this.saveToDisk();
  }

  /**
   * Set per-session engine override. Pass undefined to clear and fall back
   * to the bot's configured engine. Switching engines also clears the prior
   * `sessionId` (engines track conversation state in different stores) and
   * any stale model override, so the next turn starts a fresh session.
   */
  setSessionEngine(chatId: string, engine: EngineName | undefined): void {
    const session = this.getSession(chatId);
    if (session.engine === engine) return;
    session.engine = engine;
    session.sessionId = undefined;
    session.sessionIdEngine = undefined;
    session.model = undefined;
    session.modelEngine = undefined;
    this.modelRevisions.set(session, Symbol());
    session.reasoningEffort = undefined;
    session.serviceTier = undefined;
    this.logger.info({ chatId, engine }, 'Session engine override updated (session reset)');
    this.saveToDisk();
  }

  /**
   * Set the mirrored /goal condition for this session. Pass undefined to
   * clear it. The actual goal mechanism runs inside Claude Code; this is
   * purely so the card can display a persistent badge.
   */
  setGoal(chatId: string, condition: string | undefined): void {
    const session = this.getSession(chatId);
    if (condition) {
      session.activeGoal = condition;
      session.goalSetAt = Date.now();
      session.goalIterations = 0;
      const maxIterations = Number(process.env.METABOT_CODEX_GOAL_MAX_ITERATIONS);
      session.goalMaxIterations = Number.isFinite(maxIterations) && maxIterations > 0
        ? maxIterations
        : DEFAULT_CODEX_GOAL_MAX_ITERATIONS;
    } else {
      session.activeGoal = undefined;
      session.goalSetAt = undefined;
      session.goalIterations = undefined;
      session.goalMaxIterations = undefined;
      session.reasoningEffort = undefined;
    }
    this.logger.info({ chatId, hasGoal: !!condition }, 'Session goal updated');
    this.saveToDisk();
  }

  incrementGoalIteration(chatId: string): number {
    const session = this.getSession(chatId);
    session.goalIterations = (session.goalIterations ?? 0) + 1;
    this.saveToDisk();
    return session.goalIterations;
  }

  /** Accumulate token/cost/duration from a completed query into the session totals. */
  addUsage(chatId: string, tokens: number, costUsd: number, durationMs: number): void {
    const session = this.getSession(chatId);
    session.cumulativeTokens += tokens;
    session.cumulativeCostUsd += costUsd;
    session.cumulativeDurationMs += durationMs;
    this.saveToDisk();
  }

  /**
   * Zero the cumulative token/cost/duration counters without touching the
   * session id, model, engine, or active goal. Called when switching into a
   * resumed session (via `/resume`): the prior session's usage totals would
   * otherwise carry over and mislead `/status`.
   */
  resetUsage(chatId: string): void {
    const session = this.sessions.get(chatId);
    if (session) {
      session.cumulativeTokens = 0;
      session.cumulativeCostUsd = 0;
      session.cumulativeDurationMs = 0;
      this.saveToDisk();
    }
  }

  resetSession(chatId: string): void {
    this.getSession(chatId).threadContextInitialized = true;
    const session = this.sessions.get(chatId);
    if (session) {
      session.sessionId = undefined;
      session.sessionIdEngine = undefined;
      session.cumulativeTokens = 0;
      session.cumulativeCostUsd = 0;
      session.cumulativeDurationMs = 0;
      session.activeGoal = undefined;
      session.goalSetAt = undefined;
      session.goalIterations = undefined;
      session.goalMaxIterations = undefined;
      // Keep working directory
      this.logger.info({ chatId }, 'Session reset');
      this.saveToDisk();
    }
  }

  private cleanupExpired(): void {
    const now = Date.now();
    let changed = false;
    for (const [chatId, session] of this.sessions) {
      if (now - session.lastUsed > SESSION_TTL_MS) {
        this.sessions.delete(chatId);
        this.logger.debug({ chatId }, 'Expired session cleaned up');
        changed = true;
      }
    }
    if (changed) {
      this.saveToDisk();
    }
  }

  private saveToDisk(): void {
    try {
      const data: Record<string, PersistedSession> = {};
      for (const [chatId, session] of this.sessions) {
        // Persist sessions that have a sessionId, model, engine override, effort override, or active goal
        if (session.sessionId || session.model || session.engine || session.reasoningEffort || session.serviceTier || session.activeGoal || session.threadContextInitialized) {
          data[chatId] = {
            sessionId: session.sessionId || '',
            sessionIdEngine: session.sessionIdEngine,
            threadContextInitialized: session.threadContextInitialized,
            workingDirectory: session.workingDirectory,
            lastUsed: session.lastUsed,
            cumulativeTokens: session.cumulativeTokens,
            cumulativeCostUsd: session.cumulativeCostUsd,
            cumulativeDurationMs: session.cumulativeDurationMs,
            model: session.model,
            modelEngine: session.modelEngine,
            reasoningEffort: session.reasoningEffort,
            serviceTier: session.serviceTier,
            engine: session.engine,
            activeGoal: session.activeGoal,
            goalSetAt: session.goalSetAt,
            goalIterations: session.goalIterations,
            goalMaxIterations: session.goalMaxIterations,
          };
        }
      }
      fs.writeFileSync(this.persistPath, JSON.stringify(data, null, 2), 'utf-8');
    } catch (err) {
      this.logger.warn({ err }, 'Failed to persist sessions to disk');
    }
  }

  private loadFromDisk(): void {
    try {
      if (!fs.existsSync(this.persistPath)) return;
      const raw = fs.readFileSync(this.persistPath, 'utf-8');
      const data: Record<string, PersistedSession> = JSON.parse(raw);
      const now = Date.now();
      let loaded = 0;
      for (const [chatId, persisted] of Object.entries(data)) {
        // Skip expired sessions
        if (now - persisted.lastUsed > SESSION_TTL_MS) continue;
        this.sessions.set(chatId, {
          sessionId: persisted.sessionId || undefined,
          sessionIdEngine: persisted.sessionIdEngine,
          threadContextInitialized: persisted.threadContextInitialized,
          workingDirectory: persisted.workingDirectory,
          lastUsed: persisted.lastUsed,
          cumulativeTokens: persisted.cumulativeTokens ?? 0,
          cumulativeCostUsd: persisted.cumulativeCostUsd ?? 0,
          cumulativeDurationMs: persisted.cumulativeDurationMs ?? 0,
          model: persisted.model,
          modelEngine: persisted.modelEngine,
          reasoningEffort: persisted.reasoningEffort,
          serviceTier: persisted.serviceTier,
          engine: persisted.engine,
          activeGoal: persisted.activeGoal,
          goalSetAt: persisted.goalSetAt,
          goalIterations: persisted.goalIterations,
          goalMaxIterations: persisted.goalMaxIterations,
        });
        loaded++;
      }
      if (loaded > 0) {
        this.logger.info({ loaded, path: this.persistPath }, 'Restored sessions from disk');
      }
    } catch (err) {
      this.logger.warn({ err }, 'Failed to load sessions from disk, starting fresh');
    }
  }

  destroy(): void {
    clearInterval(this.cleanupTimer);
    this.saveToDisk();
  }
}
