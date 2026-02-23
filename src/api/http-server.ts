import * as http from 'node:http';
import type { Logger } from '../utils/logger.js';
import type { BotRegistry } from './bot-registry.js';
import type { TaskScheduler } from '../scheduler/task-scheduler.js';

interface ApiServerOptions {
  port: number;
  secret?: string;
  registry: BotRegistry;
  scheduler: TaskScheduler;
  logger: Logger;
}

interface JsonBody {
  [key: string]: unknown;
}

const startTime = Date.now();

function jsonResponse(res: http.ServerResponse, status: number, body: unknown): void {
  const json = JSON.stringify(body);
  res.writeHead(status, { 'Content-Type': 'application/json' });
  res.end(json);
}

function readBody(req: http.IncomingMessage): Promise<string> {
  return new Promise((resolve, reject) => {
    const chunks: Buffer[] = [];
    req.on('data', (chunk: Buffer) => chunks.push(chunk));
    req.on('end', () => resolve(Buffer.concat(chunks).toString()));
    req.on('error', reject);
  });
}

async function parseJsonBody(req: http.IncomingMessage): Promise<JsonBody> {
  const raw = await readBody(req);
  return JSON.parse(raw) as JsonBody;
}

export function startApiServer(options: ApiServerOptions): http.Server {
  const { port, secret, registry, scheduler, logger } = options;
  const host = secret ? '0.0.0.0' : '127.0.0.1';

  const server = http.createServer(async (req, res) => {
    const method = req.method || 'GET';
    const url = req.url || '/';

    // Auth check if secret is configured
    if (secret) {
      const auth = req.headers.authorization;
      if (auth !== `Bearer ${secret}`) {
        jsonResponse(res, 401, { error: 'Unauthorized' });
        return;
      }
    }

    try {
      // Route: GET /api/health
      if (method === 'GET' && url === '/api/health') {
        jsonResponse(res, 200, {
          status: 'ok',
          uptime: Math.floor((Date.now() - startTime) / 1000),
          bots: registry.list().length,
          scheduledTasks: scheduler.taskCount(),
        });
        return;
      }

      // Route: GET /api/bots
      if (method === 'GET' && url === '/api/bots') {
        jsonResponse(res, 200, { bots: registry.list() });
        return;
      }

      // Route: POST /api/tasks
      if (method === 'POST' && url === '/api/tasks') {
        const body = await parseJsonBody(req);
        const botName = body.botName as string;
        const chatId = body.chatId as string;
        const prompt = body.prompt as string;
        const sendCards = body.sendCards as boolean | undefined;

        if (!botName || !chatId || !prompt) {
          jsonResponse(res, 400, { error: 'Missing required fields: botName, chatId, prompt' });
          return;
        }

        const bot = registry.get(botName);
        if (!bot) {
          jsonResponse(res, 404, { error: `Bot not found: ${botName}` });
          return;
        }

        logger.info({ botName, chatId, promptLength: prompt.length }, 'API task request');

        const result = await bot.bridge.executeApiTask({
          prompt,
          chatId,
          userId: 'api',
          sendCards: sendCards ?? false,
        });

        jsonResponse(res, result.success ? 200 : 500, result);
        return;
      }

      // Route: POST /api/schedule
      if (method === 'POST' && url === '/api/schedule') {
        const body = await parseJsonBody(req);
        const botName = body.botName as string;
        const chatId = body.chatId as string;
        const prompt = body.prompt as string;
        const delaySeconds = body.delaySeconds as number;
        const sendCards = body.sendCards as boolean | undefined;
        const label = body.label as string | undefined;

        if (!botName || !chatId || !prompt || typeof delaySeconds !== 'number' || delaySeconds <= 0) {
          jsonResponse(res, 400, { error: 'Missing or invalid fields: botName, chatId, prompt, delaySeconds (positive number)' });
          return;
        }

        const bot = registry.get(botName);
        if (!bot) {
          jsonResponse(res, 404, { error: `Bot not found: ${botName}` });
          return;
        }

        const task = scheduler.scheduleTask({
          botName,
          chatId,
          prompt,
          delaySeconds,
          sendCards,
          label,
        });

        jsonResponse(res, 201, {
          id: task.id,
          botName: task.botName,
          chatId: task.chatId,
          prompt: task.prompt,
          executeAt: new Date(task.executeAt).toISOString(),
          sendCards: task.sendCards,
          label: task.label,
          status: task.status,
        });
        return;
      }

      // Route: GET /api/schedule
      if (method === 'GET' && url === '/api/schedule') {
        const tasks = scheduler.listTasks().map((t) => ({
          id: t.id,
          botName: t.botName,
          chatId: t.chatId,
          prompt: t.prompt,
          executeAt: new Date(t.executeAt).toISOString(),
          sendCards: t.sendCards,
          label: t.label,
          status: t.status,
          createdAt: new Date(t.createdAt).toISOString(),
        }));
        jsonResponse(res, 200, { tasks });
        return;
      }

      // Route: PATCH /api/schedule/:id
      if (method === 'PATCH' && url.startsWith('/api/schedule/')) {
        const id = url.slice('/api/schedule/'.length);
        if (!id) {
          jsonResponse(res, 400, { error: 'Missing task ID' });
          return;
        }

        const body = await parseJsonBody(req);
        const updated = scheduler.updateTask(id, {
          prompt: body.prompt as string | undefined,
          delaySeconds: body.delaySeconds as number | undefined,
          label: body.label as string | undefined,
          sendCards: body.sendCards as boolean | undefined,
        });

        if (updated) {
          jsonResponse(res, 200, {
            id: updated.id,
            botName: updated.botName,
            chatId: updated.chatId,
            prompt: updated.prompt,
            executeAt: new Date(updated.executeAt).toISOString(),
            sendCards: updated.sendCards,
            label: updated.label,
            status: updated.status,
          });
        } else {
          jsonResponse(res, 404, { error: `Task not found or not updatable: ${id}` });
        }
        return;
      }

      // Route: DELETE /api/schedule/:id
      if (method === 'DELETE' && url.startsWith('/api/schedule/')) {
        const id = url.slice('/api/schedule/'.length);
        if (!id) {
          jsonResponse(res, 400, { error: 'Missing task ID' });
          return;
        }

        const cancelled = scheduler.cancelTask(id);
        if (cancelled) {
          jsonResponse(res, 200, { id, status: 'cancelled' });
        } else {
          jsonResponse(res, 404, { error: `Task not found or not cancellable: ${id}` });
        }
        return;
      }

      // 404 fallback
      jsonResponse(res, 404, { error: 'Not found' });
    } catch (err: any) {
      logger.error({ err, method, url }, 'API request error');
      jsonResponse(res, 500, { error: err.message || 'Internal server error' });
    }
  });

  server.listen(port, host, () => {
    logger.info({ host, port }, 'API server started');
  });

  return server;
}
