import type { CardState } from '../types.js';

/**
 * Platform-agnostic message sender interface.
 * Implemented by each IM platform (Feishu, Telegram, etc.).
 */
export interface IMessageSender {
  /** Send a new streaming card/message for a CardState. Returns messageId for subsequent updates. */
  sendCard(chatId: string, state: CardState): Promise<string | undefined>;

  /**
   * Update an existing streaming card/message with new CardState.
   * Returns true if the update was delivered, false on transport failure.
   * Implementations MUST NOT throw — they should catch internal errors and
   * signal failure via the boolean. Callers that don't care about delivery
   * (throttled streaming updates, best-effort status changes) can ignore
   * the returned value; callers that guard user-visible state transitions
   * (final result card, frozen card before recreate) must branch on it.
   */
  updateCard(messageId: string, state: CardState): Promise<boolean>;

  /** Send a simple notice message (for command responses: /help, /reset, /stop, etc.). */
  sendTextNotice(chatId: string, title: string, content: string, color?: string): Promise<void>;

  /** Send a plain text message. */
  sendText(chatId: string, text: string): Promise<void>;

  /** Send a local image file to the chat. */
  sendImageFile(chatId: string, filePath: string): Promise<boolean>;

  /** Send a local file to the chat. */
  sendLocalFile(chatId: string, filePath: string, fileName: string): Promise<boolean>;

  /** Download a user-sent image to a local path. */
  downloadImage(messageId: string, imageKey: string, savePath: string): Promise<boolean>;

  /** Download a user-sent file to a local path. */
  downloadFile(messageId: string, fileKey: string, savePath: string): Promise<boolean>;

  /** If true, the bridge will not send a separate "Task completed" text after the card update. */
  skipCompletionNotice?: boolean;
}
