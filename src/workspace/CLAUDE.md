# MetaBot Workspace

This workspace is managed by **MetaBot** — an AI assistant accessible via Feishu/Telegram that runs Claude Code with full tool access.

## Available Skills

### /metaskill — AI Agent Team Generator
Create AI agent teams, individual agents, or custom skills for any project.

```
/metaskill ios app          → generates full .claude/ agent team
/metaskill a security agent → creates a single agent
/metaskill a deploy skill   → creates a custom slash command
```

### /metamemory — Shared Knowledge Store
Read and write persistent memory documents across sessions. Use this to save research findings, project notes, and knowledge that should survive session resets.

```
# Search existing knowledge
curl -s "$MEMORY_SERVER_URL/api/search?q=QUERY"

# Save a document
curl -s -X POST $MEMORY_SERVER_URL/api/documents \
  -H "Content-Type: application/json" \
  -d '{"title":"Title", "folder_id":"root", "content":"# Content...", "tags":["tag"]}'

# Browse all folders
curl -s $MEMORY_SERVER_URL/api/folders
```

Memory server URL: `http://localhost:8100` (embedded in MetaBot, always available).

## Scheduled Tasks

MetaBot supports scheduling tasks via its HTTP API. Use `curl` from Bash to schedule:

```bash
# Schedule a task (runs after delaySeconds)
curl -s -X POST http://localhost:9100/api/schedule \
  -H "Authorization: Bearer $API_SECRET" \
  -H "Content-Type: application/json" \
  -d '{"botName":"BOT_NAME", "chatId":"CHAT_ID", "prompt":"your task", "delaySeconds":3600, "label":"optional label"}'

# List pending tasks
curl -s http://localhost:9100/api/schedule -H "Authorization: Bearer $API_SECRET"

# Cancel a task
curl -s -X DELETE http://localhost:9100/api/schedule/TASK_ID -H "Authorization: Bearer $API_SECRET"
```

## Guidelines

- **Search before creating** — always check if a file or document already exists before creating new ones.
- **Use metamemory** — when you discover important knowledge, project patterns, or user preferences, save them to memory so future sessions can benefit.
- **Output files** — when generating files the user needs (images, PDFs, reports), copy them to the outputs directory provided in the system prompt so they get sent to the chat automatically.
- **Be concise in chat** — responses appear as Feishu/Telegram cards with limited space. Keep answers focused and use markdown formatting.
