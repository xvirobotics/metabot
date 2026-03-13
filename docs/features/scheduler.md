# Task Scheduler

One-time delays and recurring cron jobs. Timezone-aware, persists across restarts, auto-retries when busy.

## Overview

The scheduler lets you automate agent tasks:

- **One-time tasks** — Execute after a delay (e.g., "in 30 minutes")
- **Recurring tasks** — Cron-based schedules (e.g., "weekdays at 8am")
- **Timezone-aware** — Defaults to `Asia/Shanghai`, configurable per task
- **Persistent** — Survives restarts
- **Auto-retry** — Reschedules if the bot is busy

## Usage

Send natural language scheduling requests in chat:

```
Schedule a daily task at 9am: search Hacker News and TechCrunch for AI news,
summarize the top 5 stories, and save the summary to MetaMemory.
```

```
Remind me in 30 minutes to check if the deployment succeeded.
```

```
Set up a weekly Monday 8am task: review last week's git commits, generate
a progress report, and save it to MetaMemory under /reports.
```

## API

| Method | Path | Description |
|--------|------|-------------|
| `POST` | `/api/schedule` | Schedule a task |
| `GET` | `/api/schedule` | List scheduled tasks |
| `PATCH` | `/api/schedule/:id` | Update a task |
| `DELETE` | `/api/schedule/:id` | Cancel a task |
| `POST` | `/api/schedule/:id/pause` | Pause a recurring task |
| `POST` | `/api/schedule/:id/resume` | Resume a paused task |

### Create a recurring task

```bash
curl -X POST http://localhost:9100/api/schedule \
  -H "Content-Type: application/json" \
  -d '{
    "botName": "metabot",
    "chatId": "oc_xxx",
    "prompt": "Check service health and report",
    "cron": "0 8 * * 1-5",
    "timezone": "Asia/Shanghai"
  }'
```

## CLI

```bash
mb schedule list                                              # list all
mb schedule cron metabot chatId '0 8 * * 1-5' "daily report" # create cron
mb schedule pause <id>                                        # pause
mb schedule resume <id>                                       # resume
```

## Configuration

| Variable | Default | Description |
|----------|---------|-------------|
| `SCHEDULER_ENABLED` | `true` | Enable scheduler |
| Default timezone | `Asia/Shanghai` | Per-task override available |
