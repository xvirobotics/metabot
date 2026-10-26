import * as fs from 'node:fs';
import * as os from 'node:os';
import * as path from 'node:path';
import * as lark from '@larksuiteoapi/node-sdk';
import { afterEach, describe, expect, it, vi } from 'vitest';
import { loadAppConfig } from '../src/config.js';
import { DocSync } from '../src/sync/doc-sync.js';

const dirs: string[] = [];
function tempDir(): string {
  const dir = fs.mkdtempSync(path.join(os.tmpdir(), 'metabot-service-domain-'));
  dirs.push(dir);
  return dir;
}
function useBots(...domains: Array<'feishu' | 'lark' | undefined>): void {
  const dir = tempDir();
  const bots = domains.map((larkDomain, i) => ({
    name: `bot-${i}`, feishuAppId: `bot-app-${i}`, feishuAppSecret: 'bot-secret',
    defaultWorkingDirectory: dir, ...(larkDomain ? { larkDomain } : {}),
  }));
  const file = path.join(dir, 'bots.json');
  fs.writeFileSync(file, JSON.stringify(bots));
  vi.stubEnv('BOTS_CONFIG', file);
  vi.stubEnv('FEISHU_SERVICE_APP_ID', '');
  vi.stubEnv('FEISHU_SERVICE_APP_SECRET', '');
  vi.stubEnv('FEISHU_SERVICE_DOMAIN', '');
}
function useServiceApp(domain?: string): void {
  vi.stubEnv('FEISHU_SERVICE_APP_ID', 'service-app');
  vi.stubEnv('FEISHU_SERVICE_APP_SECRET', 'service-secret');
  if (domain !== undefined) vi.stubEnv('FEISHU_SERVICE_DOMAIN', domain);
}

afterEach(() => {
  vi.unstubAllEnvs();
  vi.restoreAllMocks();
  for (const dir of dirs.splice(0)) fs.rmSync(dir, { recursive: true, force: true });
});

describe('Feishu service app domain', () => {
  it('inherits the first bot domain when the service app falls back to that bot', () => {
    useBots('lark', 'feishu');
    expect(loadAppConfig().feishuService).toEqual({ appId: 'bot-app-0', appSecret: 'bot-secret', domain: 'lark' });
  });

  it('keeps the Feishu default when neither the bot nor the service app sets a domain', () => {
    useBots(undefined);
    expect(loadAppConfig().feishuService?.domain).toBeUndefined();
    useServiceApp();
    expect(loadAppConfig().feishuService).toEqual({ appId: 'service-app', appSecret: 'service-secret' });
  });

  it('uses FEISHU_SERVICE_DOMAIN for a dedicated service app, independent of the bots', () => {
    useBots('feishu');
    useServiceApp(' Lark ');
    expect(loadAppConfig().feishuService).toEqual({ appId: 'service-app', appSecret: 'service-secret', domain: 'lark' });
  });

  it('fails startup on an unknown FEISHU_SERVICE_DOMAIN instead of silently using Feishu', () => {
    useBots('lark');
    useServiceApp('larksuite');
    expect(() => loadAppConfig()).toThrow(/FEISHU_SERVICE_DOMAIN must be "feishu" or "lark"/);
  });

  it('rejects a FEISHU_SERVICE_DOMAIN that contradicts the fallback bot app', () => {
    useBots(undefined);
    vi.stubEnv('FEISHU_SERVICE_DOMAIN', 'lark');
    expect(() => loadAppConfig()).toThrow(/conflicts with the first Feishu bot's domain \(feishu\)/);
    useBots('lark');
    vi.stubEnv('FEISHU_SERVICE_DOMAIN', 'lark');
    expect(loadAppConfig().feishuService?.domain).toBe('lark');
  });
});

describe('Wiki sync API domain', () => {
  /** Run a sync with the real SDK client and capture the hosts it tries to reach. */
  async function hostsContacted(feishuDomain?: 'feishu' | 'lark'): Promise<string[]> {
    const urls: string[] = [];
    const capture = (config: any) => {
      urls.push(String(config?.url ?? config));
      return Promise.reject(new Error('network disabled in test'));
    };
    vi.spyOn(lark.defaultHttpInstance, 'request').mockImplementation(capture as any);
    vi.spyOn(lark.defaultHttpInstance, 'post').mockImplementation(((url: string) => capture({ url })) as any);
    vi.spyOn(lark.defaultHttpInstance, 'get').mockImplementation(((url: string) => capture({ url })) as any);
    const logger = { info: vi.fn(), warn: vi.fn(), debug: vi.fn(), error: vi.fn(), trace: vi.fn() } as any;
    logger.child = () => logger;
    const memory = { listFolderTree: vi.fn(), listDocuments: vi.fn(), getDocument: vi.fn() } as any;
    const sync = new DocSync(
      { feishuAppId: 'cli_test', feishuAppSecret: 'secret', databaseDir: tempDir(), throttleMs: 0, feishuDomain },
      memory, logger,
    );
    try {
      await sync.syncAll();
    } finally {
      sync.destroy();
    }
    return [...new Set(urls.map((url) => new URL(url).origin))];
  }

  it('sends Lark tenant requests to open.larksuite.com', async () => {
    expect(await hostsContacted('lark')).toEqual(['https://open.larksuite.com']);
  });

  it('keeps Feishu requests on open.feishu.cn by default', async () => {
    expect(await hostsContacted(undefined)).toEqual(['https://open.feishu.cn']);
  });
});
