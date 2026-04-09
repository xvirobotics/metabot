# Quick Setup

## Telegram (30 seconds)

1. Message [@BotFather](https://t.me/BotFather) → `/newbot` → copy token
2. Add to `bots.json` → done (long polling, no webhooks)

```json
{
  "telegramBots": [{
    "name": "my-tg-bot",
    "telegramBotToken": "123456:ABC...",
    "defaultWorkingDirectory": "/home/user/project"
  }]
}
```

## Feishu/Lark (4 steps)

1. Create app at [open.feishu.cn](https://open.feishu.cn/) → add Bot capability
2. Enable permissions: `im:message`, `im:message:readonly`, `im:resource`, `im:chat:readonly` (for group chat detection), `docx:document:readonly`, `wiki:wiki` (for doc reading & wiki sync)
3. Start MetaBot, then enable persistent connection + `im.message.receive_v1` event
4. Publish the app

```json
{
  "feishuBots": [{
    "name": "metabot",
    "feishuAppId": "cli_xxx",
    "feishuAppSecret": "...",
    "defaultWorkingDirectory": "/home/user/project"
  }]
}
```

!!! tip "No public IP needed"
    Feishu uses WebSocket (persistent connection), Telegram uses long polling. Both work behind NAT/firewalls.

For detailed Feishu configuration, see the [Feishu App Setup Guide](feishu-app-setup.md).
