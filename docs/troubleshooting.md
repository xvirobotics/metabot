# Troubleshooting

## "Error: Claude Code process exited with code 1"

The bot starts but replies with this error when you message it.

**Cause:** Claude CLI is not authenticated. The SDK spawns `claude` as a child process — if it has no valid credentials, it exits immediately with code 1.

**Fix** (run in a **separate terminal**, not inside Claude Code):

```bash
# Option A: OAuth login
claude login

# Option B: API key — add to .env
echo 'ANTHROPIC_API_KEY=sk-ant-your-key' >> /path/to/metabot/.env
```

Then restart the service:

```bash
metabot restart
# or: pkill -f "tsx src/index.ts" && cd /path/to/metabot && npm run dev
```

!!! warning
    You cannot run `claude login` or `claude auth status` from inside a Claude Code session (nested sessions are blocked). Always use a separate terminal.

## Service Won't Connect to Feishu

If the service starts but Feishu events don't arrive:

1. Ensure the Feishu app event subscription mode is **"persistent connection"** (WebSocket), not HTTP callback
2. The service must be **running before** you save the event subscription config — Feishu validates the WS connection on save
3. Check that `im.message.receive_v1` event is subscribed
4. Ensure the app version is **published and enabled** in the Feishu dev console

## Bot Doesn't Reply in Group Chats

The bot only responds when **@mentioned** in group chats. In DMs it replies to all messages. This is by design.

Exception: **2-member groups** (1 user + 1 bot) are treated like DMs — no @mention required.

## FAQ

**No public IP needed?**
:   Correct. Feishu uses WebSocket, Telegram uses long polling. No incoming ports needed.

**Non-Claude models?**
:   Yes. Any Anthropic-compatible API works (Kimi, DeepSeek, GLM, etc.). Set `ANTHROPIC_BASE_URL` and `ANTHROPIC_AUTH_TOKEN`.

**Agent communication?**
:   Currently synchronous request-response via the Agent Bus. Agents talk to each other using `mb talk` or the `/api/talk` endpoint. Async bidirectional protocols are on the roadmap.
