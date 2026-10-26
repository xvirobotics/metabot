import * as path from 'node:path';
import { FeishuTurnStore } from './turn-store.js';
import type { Logger } from '../utils/logger.js';
import { parseFeishuConversation } from './conversation.js';
import type { IMessageSender } from '../bridge/message-sender.interface.js';
import type { CardState } from '../types.js';
import { MessageSender } from './message-sender.js';
import { buildCard, buildTextCard } from './card-builder.js';
import { buildCardV2, buildTextCardV2 } from './card-builder-v2.js';
import { OutputsManager } from '../bridge/outputs-manager.js';

// v2 (native table + lark_md headings + grey footer) is the default.
// Set CARD_SCHEMA_V2=false to opt out and fall back to v1.
const USE_V2 = process.env.CARD_SCHEMA_V2 !== 'false';

/**
 * Adapts the Feishu-specific MessageSender to the platform-agnostic IMessageSender interface.
 * Handles card building (CardState → Feishu JSON) internally.
 */
export class FeishuSenderAdapter implements IMessageSender {
  constructor(private sender: MessageSender, private turns?: FeishuTurnStore, private logger?: Logger) {}

  getTransportChatId(conversationId: string): string {
    return parseFeishuConversation(conversationId).chatId;
  }

  getChatName(conversationId: string): Promise<string | undefined> {
    return this.sender.getChatName(parseFeishuConversation(conversationId).chatId);
  }

  async resolveCardConversation(chatId: string, messageId: string): Promise<string | undefined> {
    const turn = this.turns?.get(messageId);
    if (turn) return parseFeishuConversation(turn.chatId).chatId === chatId ? turn.chatId : undefined;
    return this.sender.getMessageConversation(chatId, messageId);
  }

  async getThreadContext(conversationId: string): Promise<string | undefined> {
    const destination = parseFeishuConversation(conversationId);
    if (!destination.rootMessageId) return undefined;
    const turn = this.turns?.get(destination.rootMessageId);
    if (turn && parseFeishuConversation(turn.chatId).chatId === destination.chatId) {
      return `User: ${turn.userPrompt}\nAssistant: ${turn.responseText}`;
    }
    return this.sender.getMessageText(destination.chatId, destination.rootMessageId);
  }

  private remember(messageId: string, state: CardState, chatId?: string): void {
    // Delivery has already succeeded; a snapshot write must not cause a duplicate send.
    try { this.turns?.save(messageId, state, chatId); }
    catch (err) { this.logger?.warn({ err, messageId }, 'Failed to persist Feishu turn context'); }
  }

  async sendCard(chatId: string, state: CardState): Promise<string | undefined> {
    const messageId = await this.sender.sendCard(chatId, USE_V2 ? buildCardV2(state) : buildCard(state));
    if (messageId) this.remember(messageId, state, chatId);
    return messageId;
  }

  async updateCard(messageId: string, state: CardState): Promise<boolean> {
    const updated = await this.sender.updateCard(messageId, USE_V2 ? buildCardV2(state) : buildCard(state));
    if (updated) this.remember(messageId, state);
    return updated;
  }

  /**
   * AskUserQuestion card — always Schema 1.0, regardless of CARD_SCHEMA_V2.
   *
   * Why: Feishu mobile App silently drops `tag: action` button blocks under
   * Schema 2.0, so v2 question cards show up with NO buttons on iOS/Android.
   * v1 button rendering is verified working on mobile (PR #199 tested it).
   *
   * Why a SEPARATE card rather than switching the main streaming card's
   * schema mid-life: Feishu rejects `updateCard` with a different schema
   * than the original create ("ErrCode 200830: schemaV2 card can not change
   * schemaV1"). So the main streaming card stays v2 throughout, and the
   * question gets its own dedicated v1 card sent alongside.
   *
   * See memory: bug-feishu-v2-mobile-action-buttons.
   */
  async sendQuestionCard(chatId: string, state: CardState): Promise<string | undefined> {
    const messageId = await this.sender.sendCard(chatId, buildCard(state));
    if (messageId) this.remember(messageId, state, chatId);
    return messageId;
  }

  async updateQuestionCard(messageId: string, state: CardState): Promise<boolean> {
    const updated = await this.sender.updateCard(messageId, buildCard(state));
    if (updated) this.remember(messageId, state);
    return updated;
  }

  async sendTextNotice(chatId: string, title: string, content: string, color: string = 'blue'): Promise<void> {
    await this.sender.sendCard(chatId, USE_V2 ? buildTextCardV2(title, content, color) : buildTextCard(title, content, color));
  }

  async sendText(chatId: string, text: string): Promise<void> {
    return this.sender.sendText(chatId, text);
  }

  sendTextAsUser(chatId: string, text: string, idempotencyKey: string): Promise<string | undefined> {
    return this.sender.sendTextAsUser(chatId, text, idempotencyKey);
  }

  async sendImageFile(chatId: string, filePath: string): Promise<boolean> {
    return this.sender.sendImageFile(chatId, filePath);
  }

  async sendLocalFile(chatId: string, filePath: string, fileName: string): Promise<boolean> {
    const ext = path.extname(fileName).toLowerCase();
    const feishuType = OutputsManager.feishuFileType(ext);
    return this.sender.sendLocalFile(chatId, filePath, fileName, feishuType);
  }

  async sendAudioFile(chatId: string, filePath: string, fileName?: string): Promise<boolean> {
    return this.sender.sendAudioFile(chatId, filePath, fileName ?? path.basename(filePath));
  }

  async downloadImage(messageId: string, imageKey: string, savePath: string): Promise<boolean> {
    return this.sender.downloadImage(messageId, imageKey, savePath);
  }

  async downloadFile(messageId: string, fileKey: string, savePath: string): Promise<boolean> {
    return this.sender.downloadFile(messageId, fileKey, savePath);
  }
}
