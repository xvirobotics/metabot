# Session Isolation

## How Sessions Work

Sessions are keyed by `chatId` (not `userId`), so each group chat and DM gets its own independent:

- **Working directory** — from bot config
- **Claude session ID** — conversation history
- **Task state** — one task at a time per chat

Sessions expire after **24 hours** of inactivity.

## Group Chat Behavior

- **Group chats** — Bot only responds when **@mentioned**
- **2-member groups** (1 user + 1 bot) — Treated like DMs, no @mention required
- **DMs** — Bot replies to all messages

The member count is cached for 5 minutes to avoid excessive API calls.

## Fork Groups

Users can "fork" a bot by creating multiple small group chats (2-member groups), each with its own session. This enables:

- **Parallel conversations** — Multiple independent Claude sessions with the same bot
- **Isolated contexts** — Each fork has its own conversation history and session state
- **No interference** — Work in one fork doesn't affect another

This is useful when you need to work on multiple tasks simultaneously with the same bot, without conversation contexts mixing.

## Bot Isolation

When running multiple bots (via `bots.json`), sessions are fully isolated between bots. Each bot:

- Has its own Feishu/Telegram app and receives only its own messages
- Maintains its own session store
- Uses its own working directory and configuration
