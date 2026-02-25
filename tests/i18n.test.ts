import { describe, it, expect, beforeEach } from 'vitest';
import { t, setLocale, getLocale } from '../src/utils/i18n.js';

describe('i18n', () => {
  beforeEach(() => {
    setLocale('zh');
  });

  it('returns zh string by default', () => {
    expect(t('session_reset')).toBe('会话已清除，工作目录保持不变。');
  });

  it('returns en string when locale set to en', () => {
    setLocale('en');
    expect(t('session_reset')).toBe('Conversation cleared. Working directory preserved.');
  });

  it('interpolates parameters', () => {
    expect(t('queue_full', { max: 5 })).toContain('5');
  });

  it('interpolates position parameter', () => {
    expect(t('queued', { pos: 3 })).toContain('#3');
  });

  it('gets current locale', () => {
    expect(getLocale()).toBe('zh');
    setLocale('en');
    expect(getLocale()).toBe('en');
  });

  it('returns key for unknown message key', () => {
    // @ts-expect-error testing invalid key
    expect(t('nonexistent_key')).toBe('nonexistent_key');
  });
});
