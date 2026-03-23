import * as fs from 'node:fs';
import * as path from 'node:path';
import { generateWechatUin, encryptMedia, decryptMedia } from './wechat-crypto.js';
import type { Logger } from '../utils/logger.js';

const DEFAULT_BASE_URL = 'https://ilinkai.weixin.qq.com';
const CHANNEL_VERSION = '1.0.2';
const POLL_TIMEOUT_S = 35;

// --- Types ---

export interface ILinkMessageItem {
  type: number; // 1=text, 2=image, 3=voice, 4=file, 5=video
  text_item?: { text: string };
  image_item?: { aes_key: string; cdn_ref: string; url: string };
  voice_item?: { aes_key: string; cdn_ref: string; transcription?: string };
  file_item?: { aes_key: string; cdn_ref: string; filename: string };
  video_item?: { aes_key: string; cdn_ref: string; thumb_cdn_ref?: string };
}

export interface ILinkMessage {
  from_user_id: string;
  to_user_id: string;
  message_type: number;
  context_token: string;
  item_list: ILinkMessageItem[];
  timestamp: number;
  message_id?: string;
}

interface GetUpdatesResponse {
  message_list?: ILinkMessage[];
  get_updates_buf?: string;
}

interface QrCodeResponse {
  qrcode_id: string;
  qrcode_url?: string;
  qrcode_image?: string;
}

interface QrStatusResponse {
  status: string; // 'waiting', 'scanned', 'confirmed', 'expired'
  bot_token?: string;
  baseurl?: string;
}

interface GetConfigResponse {
  typing_ticket?: string;
}

interface UploadUrlResponse {
  upload_param?: {
    url: string;
    upload_key: string;
    headers?: Record<string, string>;
  };
  cdn_ref?: string;
}

interface TokenStore {
  tokens: Record<string, { botToken: string; typingTicket: string; loginTime: number }>;
}

// --- Client ---

export class WechatClient {
  private botToken: string | undefined;
  private cursor = '';
  private typingTicket = '';
  private contextTokens = new Map<string, string>();
  private baseUrl: string;
  private logger: Logger;

  constructor(baseUrl: string | undefined, logger: Logger) {
    this.baseUrl = (baseUrl || DEFAULT_BASE_URL).replace(/\/+$/, '');
    this.logger = logger;
  }

  get isAuthenticated(): boolean {
    return !!this.botToken;
  }

  /** Set token directly (from config or persisted store). */
  setToken(token: string): void {
    this.botToken = token;
  }

  /** Get the latest context_token for a chat. */
  getContextToken(chatId: string): string | undefined {
    return this.contextTokens.get(chatId);
  }

  // --- Auth ---

  async login(): Promise<{ botToken: string; qrUrl: string }> {
    this.logger.info('Starting WeChat iLink QR login...');

    const qrRes = await this.request<QrCodeResponse>('GET', '/ilink/bot/get_bot_qrcode?bot_type=3', undefined, false);
    const qrUrl = qrRes.qrcode_url || qrRes.qrcode_image || '';
    const qrId = qrRes.qrcode_id;

    this.logger.info({ qrUrl, qrId }, 'QR code generated — scan with WeChat');

    // Poll for scan status
    let token: string | undefined;
    const maxAttempts = 60; // ~5 minutes with 5s intervals
    for (let i = 0; i < maxAttempts; i++) {
      const statusRes = await this.request<QrStatusResponse>(
        'GET',
        `/ilink/bot/get_qrcode_status?qrcode=${encodeURIComponent(qrId)}`,
        undefined,
        false,
      );

      if (statusRes.status === 'confirmed' && statusRes.bot_token) {
        token = statusRes.bot_token;
        if (statusRes.baseurl) {
          this.baseUrl = statusRes.baseurl;
        }
        break;
      }

      if (statusRes.status === 'expired') {
        throw new Error('WeChat QR code expired. Please restart to try again.');
      }

      await sleep(5000);
    }

    if (!token) {
      throw new Error('WeChat QR login timed out after 5 minutes.');
    }

    this.botToken = token;
    this.logger.info('WeChat iLink login successful');

    // Fetch typing ticket
    await this.fetchConfig();

    return { botToken: token, qrUrl };
  }

  private async fetchConfig(): Promise<void> {
    try {
      const res = await this.request<GetConfigResponse>('POST', '/ilink/bot/getconfig', {
        base_info: { channel_version: CHANNEL_VERSION },
      });
      if (res.typing_ticket) {
        this.typingTicket = res.typing_ticket;
      }
    } catch (err) {
      this.logger.warn({ err }, 'Failed to fetch iLink config (typing ticket)');
    }
  }

  // --- Messages ---

  async getUpdates(timeoutS: number = POLL_TIMEOUT_S): Promise<ILinkMessage[]> {
    const body: Record<string, unknown> = {
      base_info: { channel_version: CHANNEL_VERSION },
    };
    if (this.cursor) {
      body.get_updates_buf = this.cursor;
    }

    const res = await this.request<GetUpdatesResponse>('POST', '/ilink/bot/getupdates', body, true, timeoutS * 1000 + 5000);

    // Update cursor
    if (res.get_updates_buf) {
      this.cursor = res.get_updates_buf;
    }

    const messages = res.message_list || [];

    // Track context tokens
    for (const msg of messages) {
      if (msg.context_token && msg.from_user_id) {
        // Key by from_user_id for private chats
        this.contextTokens.set(msg.from_user_id, msg.context_token);
      }
    }

    return messages;
  }

  async sendMessage(toUserId: string, items: ILinkMessageItem[]): Promise<void> {
    const contextToken = this.contextTokens.get(toUserId);

    const body: Record<string, unknown> = {
      to_user_id: toUserId,
      item_list: items,
    };
    if (contextToken) {
      body.context_token = contextToken;
    }

    await this.request('POST', '/ilink/bot/sendmessage', body);
  }

  async sendTextMessage(toUserId: string, text: string): Promise<void> {
    await this.sendMessage(toUserId, [
      { type: 1, text_item: { text } },
    ]);
  }

  async sendTyping(toUserId: string): Promise<void> {
    if (!this.typingTicket) return;
    try {
      await this.request('POST', '/ilink/bot/sendtyping', {
        to_user_id: toUserId,
        typing_ticket: this.typingTicket,
      });
    } catch {
      // Typing is best-effort
    }
  }

  // --- Media ---

  async uploadMedia(filePath: string, mediaType: number): Promise<{ cdnRef: string; aesKey: string } | undefined> {
    const fileData = fs.readFileSync(filePath);
    const fileSize = fileData.length;

    // Get upload URL
    const uploadRes = await this.request<UploadUrlResponse>('POST', '/ilink/bot/getuploadurl', {
      media_type: mediaType,
      file_size: fileSize,
    });

    if (!uploadRes.upload_param?.url || !uploadRes.cdn_ref) {
      this.logger.error('Failed to get upload URL from iLink');
      return undefined;
    }

    // Encrypt
    const { encrypted, key } = encryptMedia(fileData);

    // Upload to CDN
    const uploadUrl = uploadRes.upload_param.url;
    const headers: Record<string, string> = {
      ...this.buildHeaders(),
      ...(uploadRes.upload_param.headers || {}),
      'Content-Type': 'application/octet-stream',
    };
    if (uploadRes.upload_param.upload_key) {
      headers['X-Upload-Key'] = uploadRes.upload_param.upload_key;
    }

    const uploadResp = await fetch(uploadUrl, {
      method: 'PUT',
      headers,
      body: new Uint8Array(encrypted),
    });

    if (!uploadResp.ok) {
      this.logger.error({ status: uploadResp.status }, 'CDN upload failed');
      return undefined;
    }

    return {
      cdnRef: uploadRes.cdn_ref,
      aesKey: key.toString('base64'),
    };
  }

  async downloadMedia(cdnUrl: string, aesKeyBase64: string, savePath: string): Promise<boolean> {
    try {
      const resp = await fetch(cdnUrl, { headers: this.buildHeaders() });
      if (!resp.ok) return false;

      const encryptedBuf = Buffer.from(await resp.arrayBuffer());
      const key = Buffer.from(aesKeyBase64, 'base64');
      const decrypted = decryptMedia(encryptedBuf, key);

      fs.writeFileSync(savePath, decrypted);
      return true;
    } catch (err) {
      this.logger.error({ err, cdnUrl }, 'Failed to download WeChat media');
      return false;
    }
  }

  // --- Token persistence ---

  saveToken(dataDir: string, botName: string): void {
    if (!this.botToken) return;
    const filePath = path.join(dataDir, 'wechat-tokens.json');

    let store: TokenStore = { tokens: {} };
    try {
      if (fs.existsSync(filePath)) {
        store = JSON.parse(fs.readFileSync(filePath, 'utf-8')) as TokenStore;
      }
    } catch {
      // Start fresh
    }

    store.tokens[botName] = {
      botToken: this.botToken,
      typingTicket: this.typingTicket,
      loginTime: Date.now(),
    };

    fs.mkdirSync(dataDir, { recursive: true });
    fs.writeFileSync(filePath, JSON.stringify(store, null, 2));
    this.logger.info({ botName, filePath }, 'WeChat token saved');
  }

  static loadToken(dataDir: string, botName: string): { botToken: string; typingTicket: string } | undefined {
    const filePath = path.join(dataDir, 'wechat-tokens.json');
    try {
      if (!fs.existsSync(filePath)) return undefined;
      const store = JSON.parse(fs.readFileSync(filePath, 'utf-8')) as TokenStore;
      const entry = store.tokens[botName];
      if (!entry?.botToken) return undefined;
      return { botToken: entry.botToken, typingTicket: entry.typingTicket };
    } catch {
      return undefined;
    }
  }

  // --- HTTP helpers ---

  private buildHeaders(): Record<string, string> {
    const headers: Record<string, string> = {
      'Content-Type': 'application/json',
      'AuthorizationType': 'ilink_bot_token',
      'X-WECHAT-UIN': generateWechatUin(),
    };
    if (this.botToken) {
      headers['Authorization'] = `Bearer ${this.botToken}`;
    }
    return headers;
  }

  private async request<T>(
    method: string,
    endpoint: string,
    body?: unknown,
    requireAuth = true,
    timeoutMs = 15000,
  ): Promise<T> {
    if (requireAuth && !this.botToken) {
      throw new Error('WeChat client not authenticated');
    }

    const url = `${this.baseUrl}${endpoint}`;
    const controller = new AbortController();
    const timer = setTimeout(() => controller.abort(), timeoutMs);

    try {
      const resp = await fetch(url, {
        method,
        headers: this.buildHeaders(),
        body: body ? JSON.stringify(body) : undefined,
        signal: controller.signal,
      });

      if (!resp.ok) {
        const text = await resp.text().catch(() => '');
        throw new Error(`iLink API ${method} ${endpoint} failed: ${resp.status} ${text}`);
      }

      return (await resp.json()) as T;
    } finally {
      clearTimeout(timer);
    }
  }
}

function sleep(ms: number): Promise<void> {
  return new Promise((r) => setTimeout(r, ms));
}
