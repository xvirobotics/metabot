import type { Logger } from '../utils/logger.js';

export interface UserSession {
  sessionId: string | undefined;
  workingDirectory: string;
  lastUsed: number;
}

const SESSION_TTL_MS = 24 * 60 * 60 * 1000; // 24 hours

export class SessionManager {
  private sessions = new Map<string, UserSession>();
  private cleanupTimer: ReturnType<typeof setInterval>;

  constructor(
    private defaultWorkingDirectory: string,
    private logger: Logger,
  ) {
    // Periodic cleanup every hour
    this.cleanupTimer = setInterval(() => this.cleanupExpired(), 60 * 60 * 1000);
  }

  getSession(chatId: string): UserSession {
    let session = this.sessions.get(chatId);
    if (!session) {
      session = {
        sessionId: undefined,
        workingDirectory: this.defaultWorkingDirectory,
        lastUsed: Date.now(),
      };
      this.sessions.set(chatId, session);
    }
    session.lastUsed = Date.now();
    return session;
  }

  setSessionId(chatId: string, sessionId: string): void {
    const session = this.getSession(chatId);
    session.sessionId = sessionId;
    this.logger.debug({ chatId, sessionId: sessionId.slice(0, 8) }, 'Session ID updated');
  }

  resetSession(chatId: string): void {
    const session = this.sessions.get(chatId);
    if (session) {
      session.sessionId = undefined;
      // Keep working directory
      this.logger.info({ chatId }, 'Session reset');
    }
  }

  private cleanupExpired(): void {
    const now = Date.now();
    for (const [userId, session] of this.sessions) {
      if (now - session.lastUsed > SESSION_TTL_MS) {
        this.sessions.delete(userId);
        this.logger.debug({ userId }, 'Expired session cleaned up');
      }
    }
  }

  destroy(): void {
    clearInterval(this.cleanupTimer);
  }
}
