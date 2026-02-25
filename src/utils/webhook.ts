import type { Logger } from './logger.js';

export interface WebhookPayload {
  event: 'task_complete' | 'task_error';
  botName: string;
  chatId: string;
  userId: string;
  success: boolean;
  responseText: string;
  costUsd?: number;
  durationMs?: number;
  error?: string;
  timestamp: string;
}

/**
 * Fire-and-forget webhook notifier.
 * Sends POST requests to configured URLs after task completion.
 * Failures are logged but never block the main flow.
 */
export class WebhookNotifier {
  private urls: string[] = [];

  constructor(private logger: Logger) {
    const envUrls = process.env.WEBHOOK_URLS;
    if (envUrls) {
      this.urls = envUrls.split(',').map((u) => u.trim()).filter(Boolean);
      if (this.urls.length > 0) {
        this.logger.info({ count: this.urls.length }, 'Webhook notifier configured');
      }
    }
  }

  get enabled(): boolean {
    return this.urls.length > 0;
  }

  async notify(payload: WebhookPayload): Promise<void> {
    if (this.urls.length === 0) return;

    const body = JSON.stringify(payload);

    for (const url of this.urls) {
      try {
        const response = await fetch(url, {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body,
          signal: AbortSignal.timeout(10000),
        });
        if (!response.ok) {
          this.logger.warn({ url, status: response.status }, 'Webhook returned non-OK status');
        }
      } catch (err) {
        this.logger.warn({ err, url }, 'Webhook notification failed');
      }
    }
  }
}
