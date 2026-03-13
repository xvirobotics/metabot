# Chat Commands

Commands you can send to MetaBot in Feishu or Telegram.

## Available Commands

| Command | Description |
|---------|-------------|
| `/reset` | Clear session — starts a fresh conversation |
| `/stop` | Abort the currently running task |
| `/status` | Show session info (session ID, working directory) |
| `/memory list` | Browse MetaMemory knowledge tree |
| `/memory search <query>` | Search MetaMemory knowledge base |
| `/sync` | Trigger MetaMemory → Feishu Wiki sync |
| `/sync status` | Show wiki sync statistics |
| `/help` | Show available commands |
| `/metaskill ...` | Generate agent teams, agents, or skills |
| `/metabot` | Load Agent Bus docs (scheduling, bot management, cross-instance talk) |
| `/anything` | Any unrecognized command is forwarded to Claude Code as a skill |

## Notes

- In **group chats**, the bot only responds when **@mentioned** (except in 2-member groups)
- In **DMs**, the bot replies to all messages
- Commands like `/memory` and `/sync` respond quickly without spawning Claude
- `/metaskill` and `/metabot` are skills that get loaded into Claude's context on demand
