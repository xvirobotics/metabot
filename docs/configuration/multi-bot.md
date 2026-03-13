# Multi-Bot Mode

Run multiple Feishu and Telegram bots in a single MetaBot process.

## Setup

Set `BOTS_CONFIG=./bots.json` in `.env` to enable multi-bot mode:

```json
{
  "feishuBots": [
    {
      "name": "metabot",
      "feishuAppId": "cli_xxx",
      "feishuAppSecret": "...",
      "defaultWorkingDirectory": "/home/user/project-a"
    },
    {
      "name": "backend-bot",
      "feishuAppId": "cli_yyy",
      "feishuAppSecret": "...",
      "defaultWorkingDirectory": "/home/user/project-b"
    }
  ],
  "telegramBots": [
    {
      "name": "tg-bot",
      "telegramBotToken": "123456:ABC...",
      "defaultWorkingDirectory": "/home/user/project-c"
    }
  ]
}
```

## Bot Config Fields

| Field | Required | Default | Description |
|-------|----------|---------|-------------|
| `name` | Yes | — | Bot identifier |
| `defaultWorkingDirectory` | Yes | — | Working directory for Claude |
| `feishuAppId` / `feishuAppSecret` | Feishu | — | Feishu app credentials |
| `telegramBotToken` | Telegram | — | Telegram bot token |
| `maxTurns` | No | unlimited | Max turns per request |
| `maxBudgetUsd` | No | unlimited | Max cost per request |
| `model` | No | SDK default | Claude model |
| `allowedTools` | No | `Read,Edit,Write,Glob,Grep,Bash` | Claude tools whitelist |
| `outputsBaseDir` | No | `/tmp/metabot-outputs` | Output files directory |

## How It Works

- Each bot gets its own Feishu/Telegram connection
- Sessions are isolated per `chatId` — no collision between bots
- Each bot uses its own working directory and configuration
- Environment variables serve as defaults for any field not specified in JSON

When `BOTS_CONFIG` is set, `FEISHU_APP_ID` / `FEISHU_APP_SECRET` env vars are ignored.

## Peers Configuration

You can also configure [Peers](../features/peers.md) in `bots.json`:

```json
{
  "feishuBots": [{ "..." }],
  "peers": [
    {
      "name": "alice",
      "url": "http://localhost:9200",
      "secret": "alice-api-secret"
    }
  ]
}
```

## Single-Bot Mode

Without `BOTS_CONFIG`, MetaBot runs in single-bot mode using environment variables:

```bash
FEISHU_APP_ID=cli_xxx
FEISHU_APP_SECRET=...
DEFAULT_WORKING_DIRECTORY=/home/user/project
```
