import { afterEach, describe, expect, it, vi } from 'vitest';
import * as fs from 'node:fs';
import * as os from 'node:os';
import * as path from 'node:path';
import type { AddressInfo } from 'node:net';
import { startMemoryServer } from '../src/memory/memory-server.js';

function createLogger() {
  return { info: vi.fn(), warn: vi.fn(), debug: vi.fn(), error: vi.fn(), child: vi.fn() } as any;
}

describe('MetaMemory server request limits', () => {
  const cleanups: Array<() => void> = [];

  afterEach(() => {
    while (cleanups.length > 0) {
      const cleanup = cleanups.pop();
      cleanup?.();
    }
  });

  async function startTestServer() {
    const databaseDir = fs.mkdtempSync(path.join(os.tmpdir(), 'metamemory-test-'));
    cleanups.push(() => fs.rmSync(databaseDir, { recursive: true, force: true }));

    const { server, storage } = startMemoryServer({
      port: 0,
      databaseDir,
      logger: createLogger(),
    });

    cleanups.push(() => storage.close());
    cleanups.push(() => server.close());

    await new Promise<void>((resolve) => server.once('listening', resolve));
    const address = server.address() as AddressInfo;

    return {
      url: `http://127.0.0.1:${address.port}`,
    };
  }

  async function startAuthenticatedTestServer() {
    const databaseDir = fs.mkdtempSync(path.join(os.tmpdir(), 'metamemory-auth-test-'));
    cleanups.push(() => fs.rmSync(databaseDir, { recursive: true, force: true }));

    const { server, storage } = startMemoryServer({
      port: 0,
      databaseDir,
      secret: 'test-secret',
      logger: createLogger(),
    });

    cleanups.push(() => storage.close());
    cleanups.push(() => server.close());

    await new Promise<void>((resolve) => server.once('listening', resolve));
    const address = server.address() as AddressInfo;

    return {
      url: `http://127.0.0.1:${address.port}`,
    };
  }

  it('returns 400 for invalid JSON bodies', async () => {
    const { url } = await startTestServer();

    const response = await fetch(`${url}/api/folders`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: '{"name":',
    });

    expect(response.status).toBe(400);
    await expect(response.json()).resolves.toEqual({
      detail: 'Invalid JSON in request body',
    });
  });

  it('returns 413 for oversized JSON bodies', async () => {
    const { url } = await startTestServer();
    const oversizedPayload = JSON.stringify({
      name: 'x',
      description: 'a'.repeat(10 * 1024 * 1024),
    });

    const response = await fetch(`${url}/api/folders`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: oversizedPayload,
    });

    expect(response.status).toBe(413);
    await expect(response.json()).resolves.toEqual({
      detail: 'Request body too large (max 10 MB)',
    });
  });

  it('allows unauthenticated health checks while keeping other API routes protected', async () => {
    const { url } = await startAuthenticatedTestServer();

    const healthResponse = await fetch(`${url}/api/health`);
    expect(healthResponse.status).toBe(200);

    const foldersResponse = await fetch(`${url}/api/folders`);
    expect(foldersResponse.status).toBe(401);
    await expect(foldersResponse.json()).resolves.toEqual({
      detail: 'Unauthorized',
    });
  });
});
