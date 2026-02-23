import * as crypto from 'node:crypto';
import * as fs from 'node:fs';
import * as os from 'node:os';
import * as path from 'node:path';
import type { Logger } from '../utils/logger.js';
import type { BotRegistry } from '../api/bot-registry.js';

export interface ScheduledTask {
  id: string;
  botName: string;
  chatId: string;
  prompt: string;
  executeAt: number;       // Unix ms
  sendCards: boolean;
  label?: string;
  status: 'pending' | 'executing' | 'completed' | 'failed' | 'cancelled';
  createdAt: number;
  retryCount: number;
}

export interface ScheduleInput {
  botName: string;
  chatId: string;
  prompt: string;
  delaySeconds: number;
  sendCards?: boolean;
  label?: string;
}

export interface ScheduleUpdateInput {
  prompt?: string;
  delaySeconds?: number;
  label?: string;
  sendCards?: boolean;
}

const MAX_RETRIES = 5;
const RETRY_DELAY_MS = 30_000; // 30 seconds
const STALE_THRESHOLD_MS = 24 * 60 * 60 * 1000; // 24 hours
const PERSIST_DIR = path.join(os.homedir(), '.metabot');
const PERSIST_FILE = path.join(PERSIST_DIR, 'scheduled-tasks.json');

/**
 * Manages scheduled tasks with persistence and timers.
 * Tasks fire via setTimeout and call bridge.executeApiTask().
 */
export class TaskScheduler {
  private tasks = new Map<string, ScheduledTask>();
  private timers = new Map<string, ReturnType<typeof setTimeout>>();

  constructor(
    private registry: BotRegistry,
    private logger: Logger,
  ) {
    this.loadFromDisk();
  }

  scheduleTask(input: ScheduleInput): ScheduledTask {
    const now = Date.now();
    const task: ScheduledTask = {
      id: crypto.randomUUID(),
      botName: input.botName,
      chatId: input.chatId,
      prompt: input.prompt,
      executeAt: now + input.delaySeconds * 1000,
      sendCards: input.sendCards ?? true,
      label: input.label,
      status: 'pending',
      createdAt: now,
      retryCount: 0,
    };

    this.tasks.set(task.id, task);
    this.setTimer(task);
    this.saveToDisk();

    this.logger.info({ taskId: task.id, botName: task.botName, chatId: task.chatId, delaySeconds: input.delaySeconds, label: task.label }, 'Scheduled task created');
    return task;
  }

  updateTask(id: string, input: ScheduleUpdateInput): ScheduledTask | null {
    const task = this.tasks.get(id);
    if (!task || task.status !== 'pending') return null;

    if (input.prompt !== undefined) task.prompt = input.prompt;
    if (input.label !== undefined) task.label = input.label;
    if (input.sendCards !== undefined) task.sendCards = input.sendCards;

    if (input.delaySeconds !== undefined) {
      task.executeAt = Date.now() + input.delaySeconds * 1000;
      // Reset timer
      const timer = this.timers.get(id);
      if (timer) clearTimeout(timer);
      this.setTimer(task);
    }

    this.saveToDisk();
    this.logger.info({ taskId: id, updates: input }, 'Scheduled task updated');
    return task;
  }

  cancelTask(id: string): boolean {
    const task = this.tasks.get(id);
    if (!task || task.status !== 'pending') return false;

    task.status = 'cancelled';
    const timer = this.timers.get(id);
    if (timer) {
      clearTimeout(timer);
      this.timers.delete(id);
    }
    this.saveToDisk();

    this.logger.info({ taskId: id }, 'Scheduled task cancelled');
    return true;
  }

  listTasks(): ScheduledTask[] {
    return Array.from(this.tasks.values()).filter((t) => t.status === 'pending');
  }

  taskCount(): number {
    return this.listTasks().length;
  }

  destroy(): void {
    for (const timer of this.timers.values()) {
      clearTimeout(timer);
    }
    this.timers.clear();
    this.saveToDisk();
  }

  private setTimer(task: ScheduledTask): void {
    const delay = Math.max(0, task.executeAt - Date.now());
    const timer = setTimeout(() => this.fireTask(task.id), delay);
    this.timers.set(task.id, timer);
  }

  private async fireTask(id: string): Promise<void> {
    const task = this.tasks.get(id);
    if (!task || task.status !== 'pending') return;

    this.timers.delete(id);

    const bot = this.registry.get(task.botName);
    if (!bot) {
      this.logger.error({ taskId: id, botName: task.botName }, 'Scheduled task: bot not found');
      task.status = 'failed';
      this.saveToDisk();
      return;
    }

    // If chat is busy, retry
    if (bot.bridge.isBusy(task.chatId)) {
      if (task.retryCount < MAX_RETRIES) {
        task.retryCount++;
        this.logger.info({ taskId: id, retryCount: task.retryCount }, 'Chat busy, retrying scheduled task');
        const timer = setTimeout(() => this.fireTask(id), RETRY_DELAY_MS);
        this.timers.set(id, timer);
        this.saveToDisk();
        return;
      }

      // Max retries exceeded — notify user and mark failed
      this.logger.warn({ taskId: id }, 'Scheduled task failed after max retries (chat busy)');
      task.status = 'failed';
      this.saveToDisk();
      try {
        await bot.sender.sendTextNotice(
          task.chatId,
          'Scheduled Task Failed',
          `Task "${task.label || task.prompt.slice(0, 50)}" could not run because the chat was busy. Please retry manually.`,
          'red',
        );
      } catch (err) {
        this.logger.error({ err, taskId: id }, 'Failed to send task failure notification');
      }
      return;
    }

    // Execute the task
    task.status = 'executing';
    this.saveToDisk();
    this.logger.info({ taskId: id, botName: task.botName, chatId: task.chatId }, 'Firing scheduled task');

    try {
      const result = await bot.bridge.executeApiTask({
        prompt: task.prompt,
        chatId: task.chatId,
        userId: 'scheduler',
        sendCards: task.sendCards,
      });

      task.status = result.success ? 'completed' : 'failed';
      if (!result.success) {
        this.logger.warn({ taskId: id, error: result.error }, 'Scheduled task completed with error');
      }
    } catch (err: any) {
      this.logger.error({ err, taskId: id }, 'Scheduled task execution error');
      task.status = 'failed';
    }

    this.saveToDisk();
  }

  private saveToDisk(): void {
    try {
      fs.mkdirSync(PERSIST_DIR, { recursive: true });
      const data = Array.from(this.tasks.values());
      fs.writeFileSync(PERSIST_FILE, JSON.stringify(data, null, 2));
    } catch (err) {
      this.logger.error({ err }, 'Failed to save scheduled tasks to disk');
    }
  }

  private loadFromDisk(): void {
    try {
      if (!fs.existsSync(PERSIST_FILE)) return;

      const raw = fs.readFileSync(PERSIST_FILE, 'utf-8');
      const data = JSON.parse(raw) as ScheduledTask[];
      const now = Date.now();

      for (const task of data) {
        // Skip completed/cancelled/failed tasks
        if (task.status !== 'pending') continue;

        // Skip tasks that are more than 24h overdue (stale)
        if (task.executeAt < now - STALE_THRESHOLD_MS) {
          this.logger.info({ taskId: task.id }, 'Skipping stale scheduled task (>24h overdue)');
          continue;
        }

        this.tasks.set(task.id, task);
        this.setTimer(task);
      }

      const restored = this.listTasks().length;
      if (restored > 0) {
        this.logger.info({ count: restored }, 'Restored pending scheduled tasks from disk');
      }
    } catch (err) {
      this.logger.error({ err }, 'Failed to load scheduled tasks from disk');
    }
  }
}
