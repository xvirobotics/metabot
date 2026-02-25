/**
 * Simple i18n module for user-facing messages.
 * Supports 'en' and 'zh' locales. Defaults to 'zh' for Feishu users.
 */

export type Locale = 'en' | 'zh';

const messages = {
  queue_full: {
    en: 'Queue is full ({max} pending). Use `/stop` to abort the current task, or wait.',
    zh: '队列已满（{max} 条排队中）。使用 `/stop` 终止当前任务，或等待。',
  },
  queued: {
    en: 'Your message has been queued (position #{pos}). It will run after the current task finishes.',
    zh: '您的消息已排队（第 #{pos} 位）。当前任务完成后将自动执行。',
  },
  session_reset: {
    en: 'Conversation cleared. Working directory preserved.',
    zh: '会话已清除，工作目录保持不变。',
  },
  stopped: {
    en: 'Current task has been aborted.',
    zh: '当前任务已终止。',
  },
  no_running_task: {
    en: 'There is no task to stop.',
    zh: '没有正在运行的任务。',
  },
  task_in_progress: {
    en: 'You have a running task. Use `/stop` to abort it, or wait for it to finish.',
    zh: '有任务正在运行。使用 `/stop` 终止它，或等待完成。',
  },
  timeout_1h: {
    en: 'Task timed out (1 hour limit)',
    zh: '任务超时（1小时限制）',
  },
  idle_timeout: {
    en: 'Task aborted: no activity for 5 minutes',
    zh: '任务终止：5分钟无活动',
  },
  task_stopped: {
    en: 'Task was stopped',
    zh: '任务已被用户终止',
  },
  unexpected_end: {
    en: 'Claude session ended unexpectedly',
    zh: 'Claude 会话意外结束',
  },
  reply_text_only: {
    en: 'Please reply with text, or type a custom answer.',
    zh: '请用文字回复选择，或直接输入自定义答案。',
  },
} as const;

export type MessageKey = keyof typeof messages;

let currentLocale: Locale = 'zh';

export function setLocale(locale: Locale): void {
  currentLocale = locale;
}

export function getLocale(): Locale {
  return currentLocale;
}

export function t(key: MessageKey, params?: Record<string, string | number>): string {
  const template = messages[key]?.[currentLocale] || messages[key]?.['en'] || key;
  if (!params) return template;
  return template.replace(/\{(\w+)\}/g, (_, k) => String(params[k] ?? `{${k}}`));
}
