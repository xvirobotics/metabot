import * as fs from 'node:fs';
import * as os from 'node:os';
import * as path from 'node:path';
import { createHash } from 'node:crypto';
import { execFile as execFileCallback } from 'node:child_process';
import { promisify } from 'node:util';
import type * as lark from '@larksuiteoapi/node-sdk';
import type { Logger } from '../utils/logger.js';
import { feishuConversationId, parseFeishuConversation } from './conversation.js';
import { getReplyMessageId } from '../bridge/reply-context.js';

const execFile = promisify(execFileCallback);

export class MessageSender {
  private chatOwnerCache = new Map<string, { ownerId?: string; expiresAt: number }>();

  constructor(
    private client: lark.Client,
    private logger: Logger,
  ) {}

  private async sendMessage(chatId: string, content: string, msgType: string) {
    const destination = parseFeishuConversation(chatId);
    const messageId = getReplyMessageId(chatId) || destination.rootMessageId;
    const response = messageId
      ? await this.client.im.v1.message.reply({
        path: { message_id: messageId },
        data: { content, msg_type: msgType, reply_in_thread: true },
      })
      : await this.client.im.v1.message.create({
        params: { receive_id_type: 'chat_id' },
        data: { receive_id: destination.chatId, content, msg_type: msgType },
      });
    if (response?.code) {
      throw new Error(`Feishu message delivery failed (${response.code}): ${response.msg}`);
    }
    return response;
  }

  async sendCard(chatId: string, cardContent: string): Promise<string | undefined> {
    try {
      const resp = await this.sendMessage(chatId, cardContent, 'interactive');

      const messageId = resp?.data?.message_id;
      if (!messageId) {
        this.logger.error({ resp }, 'Failed to get message_id from send response');
      }
      return messageId;
    } catch (err) {
      this.logger.error({ err, chatId }, 'Failed to send card');
      return undefined;
    }
  }

  async updateCard(messageId: string, cardContent: string): Promise<boolean> {
    try {
      await this.client.im.v1.message.patch({
        path: { message_id: messageId },
        data: { content: cardContent },
      });
      return true;
    } catch (err) {
      this.logger.error({ err, messageId }, 'Failed to update card');
      return false;
    }
  }

  async getMessageConversation(chatId: string, messageId: string): Promise<string | undefined> {
    try {
      const response = await this.client.im.v1.message.get({ path: { message_id: messageId } });
      const item = response?.data?.items?.[0];
      if (response?.code || item?.chat_id !== chatId) return undefined;
      return feishuConversationId({
        chatId, messageId, chatType: '', text: '', userId: '',
        rootMessageId: item.root_id, parentMessageId: item.parent_id, threadId: item.thread_id,
      });
    } catch (err) {
      this.logger.warn({ err, messageId }, 'Could not resolve card conversation');
      return undefined;
    }
  }

  async getMessageText(chatId: string, messageId: string): Promise<string | undefined> {
    try {
      const response = await this.client.im.v1.message.get({ path: { message_id: messageId } });
      const item = response?.data?.items?.[0];
      if (response?.code || item?.chat_id !== chatId || !item.body?.content) return undefined;
      const content: unknown = JSON.parse(item.body.content);
      const texts: string[] = [];
      const visit = (value: unknown, depth: number): void => {
        if (depth > 20 || !value || typeof value !== 'object') return;
        if (Array.isArray(value)) { value.forEach(child => visit(child, depth + 1)); return; }
        const node = value as Record<string, unknown>;
        if (typeof node.text === 'string') texts.push(node.text);
        if (typeof node.content === 'string') texts.push(node.content);
        for (const child of Object.values(node)) if (typeof child === 'object') visit(child, depth + 1);
      };
      visit(content, 0);
      return texts.length ? `Referenced message (available text only):\n${texts.join('\n').slice(0, 24000)}` : undefined;
    } catch (err) {
      this.logger.warn({ err, messageId }, 'Could not read the thread origin');
      return undefined;
    }
  }

  async downloadImage(messageId: string, imageKey: string, savePath: string): Promise<boolean> {
    try {
      const resp = await this.client.im.v1.messageResource.get({
        path: { message_id: messageId, file_key: imageKey },
        params: { type: 'image' },
      });

      if (resp) {
        await (resp as any).writeFile(savePath);
        this.logger.info({ messageId, imageKey, savePath }, 'Image downloaded');
        return true;
      }
      this.logger.error({ messageId, imageKey }, 'Empty response when downloading image');
      return false;
    } catch (err) {
      this.logger.error({ err, messageId, imageKey }, 'Failed to download image');
      return false;
    }
  }

  async downloadFile(messageId: string, fileKey: string, savePath: string): Promise<boolean> {
    try {
      const resp = await this.client.im.v1.messageResource.get({
        path: { message_id: messageId, file_key: fileKey },
        params: { type: 'file' },
      });

      if (resp) {
        await (resp as any).writeFile(savePath);
        this.logger.info({ messageId, fileKey, savePath }, 'File downloaded');
        return true;
      }
      this.logger.error({ messageId, fileKey }, 'Empty response when downloading file');
      return false;
    } catch (err) {
      this.logger.error({ err, messageId, fileKey }, 'Failed to download file');
      return false;
    }
  }

  async uploadImage(filePath: string): Promise<string | undefined> {
    try {
      const resp = await this.client.im.v1.image.create({
        data: {
          image_type: 'message',
          image: fs.createReadStream(filePath),
        },
      });
      const imageKey = resp?.image_key;
      if (imageKey) {
        this.logger.info({ filePath, imageKey }, 'Image uploaded to Feishu');
      }
      return imageKey;
    } catch (err) {
      this.logger.error({ err, filePath }, 'Failed to upload image');
      return undefined;
    }
  }

  async sendImage(chatId: string, imageKey: string): Promise<boolean> {
    try {
      await this.sendMessage(chatId, JSON.stringify({ image_key: imageKey }), 'image');
      return true;
    } catch (err) {
      this.logger.error({ err, chatId, imageKey }, 'Failed to send image');
      return false;
    }
  }

  async sendImageFile(chatId: string, filePath: string): Promise<boolean> {
    const imageKey = await this.uploadImage(filePath);
    if (!imageKey) return false;
    return this.sendImage(chatId, imageKey);
  }

  async uploadFile(filePath: string, fileName: string, fileType: string): Promise<string | undefined> {
    try {
      const resp = await this.client.im.v1.file.create({
        data: {
          file_type: fileType as any,
          file_name: fileName,
          file: fs.createReadStream(filePath),
        },
      });
      const fileKey = resp?.file_key;
      if (fileKey) {
        this.logger.info({ filePath, fileKey, fileType }, 'File uploaded to Feishu');
      }
      return fileKey;
    } catch (err) {
      this.logger.error({ err, filePath, fileType }, 'Failed to upload file');
      return undefined;
    }
  }

  async sendFile(chatId: string, fileKey: string): Promise<boolean> {
    try {
      await this.sendMessage(chatId, JSON.stringify({ file_key: fileKey }), 'file');
      return true;
    } catch (err) {
      this.logger.error({ err, chatId, fileKey }, 'Failed to send file');
      return false;
    }
  }

  async sendLocalFile(chatId: string, filePath: string, fileName: string, fileType: string): Promise<boolean> {
    const fileKey = await this.uploadFile(filePath, fileName, fileType);
    if (!fileKey) return false;
    return this.sendFile(chatId, fileKey);
  }

  async sendAudio(chatId: string, fileKey: string): Promise<boolean> {
    try {
      await this.sendMessage(chatId, JSON.stringify({ file_key: fileKey }), 'audio');
      return true;
    } catch (err) {
      this.logger.error({ err, chatId, fileKey }, 'Failed to send audio');
      return false;
    }
  }

  async sendAudioFile(chatId: string, filePath: string, fileName: string): Promise<boolean> {
    const fileKey = await this.uploadFile(filePath, fileName, 'opus');
    if (!fileKey) return false;
    return this.sendAudio(chatId, fileKey);
  }

  async getChatMemberCount(chatId: string): Promise<number | undefined> {
    try {
      const resp: any = await this.client.im.v1.chat.get({
        path: { chat_id: chatId },
      });
      const userCount = parseInt(resp?.data?.user_count, 10) || 0;
      const botCount = parseInt(resp?.data?.bot_count, 10) || 0;
      return userCount + botCount;
    } catch (err) {
      this.logger.error({ err, chatId }, 'Failed to get chat member count');
      return undefined;
    }
  }

  async getChatName(chatId: string): Promise<string | undefined> {
    try {
      const resp = await this.client.im.v1.chat.get({ path: { chat_id: chatId } });
      const name = resp?.data?.name;
      return typeof name === 'string' && name.trim() ? name.trim() : undefined;
    } catch (err) {
      this.logger.warn({ err, chatId }, 'Could not read Feishu chat name');
      return undefined;
    }
  }

  async isChatOwner(chatId: string, userId: string): Promise<boolean | undefined> {
    const cached = this.chatOwnerCache.get(chatId);
    if (cached && cached.expiresAt > Date.now()) {
      return cached.ownerId === userId;
    }

    try {
      const resp: any = await this.client.im.v1.chat.get({
        params: { user_id_type: 'open_id' },
        path: { chat_id: chatId },
      });
      const ownerId = resp?.data?.owner_id ?? resp?.owner_id;
      this.chatOwnerCache.set(chatId, {
        ownerId: typeof ownerId === 'string' ? ownerId : undefined,
        expiresAt: Date.now() + 5 * 60 * 1000,
      });
      return typeof ownerId === 'string' ? ownerId === userId : undefined;
    } catch (err) {
      this.logger.error({ err, chatId, userId }, 'Failed to verify Feishu chat owner');
      return undefined;
    }
  }

  async sendText(chatId: string, text: string): Promise<void> {
    try {
      await this.sendMessage(chatId, JSON.stringify({ text }), 'text');
    } catch (err) {
      this.logger.error({ err, chatId }, 'Failed to send text');
    }
  }

  async sendTextAsUser(chatId: string, text: string, idempotencyKey: string): Promise<string | undefined> {
    const token = process.env.METABOT_SHADOW_USER_ACCESS_TOKEN?.trim();
    if (!token) return this.sendTextAsAuthorizedCliUser(chatId, text, idempotencyKey);
    try {
      const destination = parseFeishuConversation(chatId);
      const replyId = getReplyMessageId(chatId) || destination.rootMessageId;
      const endpoint = replyId
        ? `/open-apis/im/v1/messages/${encodeURIComponent(replyId)}/reply`
        : '/open-apis/im/v1/messages?receive_id_type=chat_id';
      const response = await fetch(new URL(endpoint, this.client.domain || 'https://open.feishu.cn'), {
        method: 'POST',
        headers: {
          Authorization: `Bearer ${token}`,
          'Content-Type': 'application/json; charset=utf-8',
        },
        body: JSON.stringify({
          ...(replyId ? { reply_in_thread: true } : { receive_id: destination.chatId }),
          msg_type: 'text',
          content: JSON.stringify({ text }),
          uuid: createHash('sha256').update(idempotencyKey).digest('hex').slice(0, 50),
        }),
        signal: AbortSignal.timeout(30_000),
        redirect: 'error',
      });
      const payload = (await response.json().catch(() => undefined)) as
        | { code?: number; data?: { message_id?: unknown } }
        | undefined;
      const messageId = payload?.data?.message_id;
      if (!response.ok || payload?.code || typeof messageId !== 'string' || !messageId.trim()) {
        this.logger.error({ chatId, status: response.status, code: payload?.code }, 'Failed to send Shadow message as user');
        return undefined;
      }
      return messageId;
    } catch {
      this.logger.error({ chatId }, 'Failed to send Shadow message as user');
      return undefined;
    }
  }

  private async sendTextAsAuthorizedCliUser(
    chatId: string,
    text: string,
    idempotencyKey: string,
  ): Promise<string | undefined> {
    try {
      const destination = parseFeishuConversation(chatId);
      const replyId = getReplyMessageId(chatId) || destination.rootMessageId;
      const cli = findLarkCli();
      const { stdout } = await execFile(
        cli,
        [
          'im',
          ...(replyId
            ? ['+messages-reply', '--message-id', replyId, '--reply-in-thread']
            : ['+messages-send', '--chat-id', destination.chatId]),
          '--as', 'user',
          '--text', text,
          '--idempotency-key', idempotencyKey,
        ],
        {
          encoding: 'utf8',
          maxBuffer: 1024 * 1024,
          timeout: 30_000,
          env: { ...process.env, LARKSUITE_CLI_NO_UPDATE_NOTIFIER: '1', LARKSUITE_CLI_NO_SKILLS_NOTIFIER: '1' },
        },
      );
      const payload = JSON.parse(stdout) as { ok?: boolean; success?: boolean; data?: unknown; messageId?: unknown };
      const messageId = typeof payload.messageId === 'string' ? payload.messageId : findMessageId(payload.data);
      if ((payload.ok !== true && payload.success !== true) || !messageId?.trim()) {
        this.logger.error({ chatId, cli }, 'lark-cli did not return a Shadow message receipt');
        return undefined;
      }
      return messageId;
    } catch {
      this.logger.error({ chatId }, 'Failed to send Shadow message as authorized lark-cli user');
      return undefined;
    }
  }
}

function findLarkCli(): string {
  const configured = process.env.LARK_CLI_PATH?.trim();
  if (configured) return configured;
  const candidates = [
    path.join(os.homedir(), '.local', 'bin', 'lark-cli'),
    path.join(os.homedir(), '.npm-global', 'bin', 'lark-cli'),
    '/usr/local/bin/lark-cli',
    'lark-cli',
  ];
  return candidates.find((candidate) => candidate === 'lark-cli' || fs.existsSync(candidate)) || 'lark-cli';
}

function findMessageId(value: unknown): string | undefined {
  if (!value || typeof value !== 'object') return undefined;
  if ('message_id' in value && typeof value.message_id === 'string') return value.message_id;
  for (const nested of Object.values(value)) {
    const found = findMessageId(nested);
    if (found) return found;
  }
  return undefined;
}
