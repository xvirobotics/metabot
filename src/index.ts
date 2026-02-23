import * as lark from '@larksuiteoapi/node-sdk';
import { loadAppConfig, type BotConfig } from './config.js';
import { createLogger, type Logger } from './utils/logger.js';
import { createEventDispatcher } from './feishu/event-handler.js';
import { MessageSender } from './feishu/message-sender.js';
import { FeishuSenderAdapter } from './feishu/feishu-sender-adapter.js';
import { MessageBridge } from './bridge/message-bridge.js';
import { startTelegramBot, type TelegramBotHandle } from './telegram/telegram-bot.js';

interface FeishuBotHandle {
  name: string;
  bridge: MessageBridge;
  wsClient: lark.WSClient;
}

async function startFeishuBot(botConfig: BotConfig, logger: Logger, memoryServerUrl: string): Promise<FeishuBotHandle> {
  const botLogger = logger.child({ bot: botConfig.name });

  botLogger.info('Starting Feishu bot...');

  // Create Feishu API client
  const client = new lark.Client({
    appId: botConfig.feishu.appId,
    appSecret: botConfig.feishu.appSecret,
    disableTokenCache: false,
  });

  // Fetch bot info to get bot's open_id for accurate @mention detection
  let botOpenId: string | undefined;
  try {
    const botInfo: any = await client.request({ method: 'GET', url: '/open-apis/bot/v3/info' });
    botOpenId = botInfo?.bot?.open_id;
    if (botOpenId) {
      botLogger.info({ botOpenId }, 'Bot info fetched');
    } else {
      botLogger.warn('Could not get bot open_id, group @mention filtering may be less accurate');
    }
  } catch (err) {
    botLogger.warn({ err }, 'Failed to fetch bot info, group @mention filtering may be less accurate');
  }

  // Create sender and bridge (FeishuSenderAdapter wraps the Feishu-specific MessageSender)
  const rawSender = new MessageSender(client, botLogger);
  const sender = new FeishuSenderAdapter(rawSender);
  const bridge = new MessageBridge(botConfig, botLogger, sender, memoryServerUrl);

  // Create event dispatcher wired to the bridge
  const dispatcher = createEventDispatcher(botConfig, botLogger, (msg) => {
    bridge.handleMessage(msg).catch((err) => {
      botLogger.error({ err, msg }, 'Unhandled error in message bridge');
    });
  }, botOpenId);

  // Create WebSocket client
  const wsClient = new lark.WSClient({
    appId: botConfig.feishu.appId,
    appSecret: botConfig.feishu.appSecret,
    loggerLevel: lark.LoggerLevel.info,
  });

  // Start WebSocket connection with event dispatcher
  await wsClient.start({ eventDispatcher: dispatcher });

  botLogger.info('Feishu bot is running');
  botLogger.info({
    defaultWorkingDirectory: botConfig.claude.defaultWorkingDirectory,
    allowedTools: botConfig.claude.allowedTools,
    maxTurns: botConfig.claude.maxTurns ?? 'unlimited',
    maxBudgetUsd: botConfig.claude.maxBudgetUsd ?? 'unlimited',
  }, 'Configuration');

  return { name: botConfig.name, bridge, wsClient };
}

async function main() {
  const appConfig = loadAppConfig();
  const logger = createLogger(appConfig.log.level);

  const feishuCount = appConfig.feishuBots.length;
  const telegramCount = appConfig.telegramBots.length;
  logger.info({ feishuBots: feishuCount, telegramBots: telegramCount, memoryServerUrl: appConfig.memoryServerUrl }, 'Starting MetaBot bridge...');

  // Start all bots in parallel
  const feishuHandles: FeishuBotHandle[] = feishuCount > 0
    ? await Promise.all(appConfig.feishuBots.map((bot) => startFeishuBot(bot, logger, appConfig.memoryServerUrl)))
    : [];

  const telegramHandles: TelegramBotHandle[] = telegramCount > 0
    ? await Promise.all(appConfig.telegramBots.map((bot) => startTelegramBot(bot, logger, appConfig.memoryServerUrl)))
    : [];

  const allNames = [...feishuHandles.map((h) => h.name), ...telegramHandles.map((h) => h.name)];
  logger.info({ bots: allNames }, 'All bots started');

  // Graceful shutdown
  const shutdown = () => {
    logger.info('Shutting down...');
    for (const handle of feishuHandles) {
      handle.bridge.destroy();
    }
    for (const handle of telegramHandles) {
      handle.bridge.destroy();
      handle.bot.stop();
    }
    process.exit(0);
  };

  process.on('SIGINT', shutdown);
  process.on('SIGTERM', shutdown);
}

main().catch((err) => {
  console.error('Fatal error:', err);
  process.exit(1);
});
