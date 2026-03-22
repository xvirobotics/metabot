# MetaBot

**Infrastructure for building a supervised, self-improving agent organization.**

[![CI](https://img.shields.io/github/actions/workflow/status/xvirobotics/metabot/ci.yml?branch=main&style=flat-square)](https://github.com/xvirobotics/metabot/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg?style=flat-square)](https://opensource.org/licenses/MIT)
[![GitHub stars](https://img.shields.io/github/stars/xvirobotics/metabot?style=flat-square)](https://github.com/xvirobotics/metabot)

[中文](README.md) | English | [Docs](https://xvirobotics.com/metabot/)

---

![MetaBot Architecture](resources/metabot.png)

## Why

Claude Code is the most capable AI coding agent — but it's trapped in your laptop terminal.

MetaBot sets it free. It gives every agent a Claude Code brain, persistent shared memory, the ability to create new agents, and a communication bus. All accessible from **Feishu, Telegram, browser, or iPhone**.

We built MetaBot to run [XVI Robotics](https://xvirobotics.com) as an **agent-native company** — a small team of humans supervising an organization of self-improving AI agents. This is the infrastructure that makes it possible.

## Multi-Platform Access

| Client | Use Case | Key Features |
|--------|----------|-------------|
| **Feishu/Lark** | Work, team collaboration | Streaming interactive cards, @mention routing, Wiki auto-sync |
| **Telegram** | Personal / international | 30-second setup, long polling (no public IP), group + private chat |
| **Web UI** | Browser, voice conversations | Phone call mode (VAD), RTC calls, MetaMemory browser, team dashboard |
| **iOS App** | Native iPhone/iPad | CallKit, VoIP push, speech recognition, iPad split view |

## Web UI

Full-featured browser-based chat interface. Access at `https://your-server/web/` after starting MetaBot.

- **Real-time streaming** — WebSocket, Markdown rendering, tool call display
- **Phone call mode** — Tap phone icon for fullscreen hands-free voice conversation with VAD
- **RTC calls** — Two-way voice/video calls via VolcEngine RTC
- **Group chat** — Multiple agents in one conversation, @mention routing
- **MetaMemory browser** — Search and browse shared knowledge base
- **Team dashboard** — Agent organization overview
- **File support** — Upload/download with inline preview
- **Dark/light themes** — System-aware with manual toggle

**Stack**: React 19 + Vite + Zustand + react-markdown

> Voice features require HTTPS. We recommend Caddy as a reverse proxy. See [Web UI docs](https://xvirobotics.com/metabot/features/web-ui/).

## iOS App

Native Swift app providing a complete mobile agent interaction experience.

- **Native call experience** — CallKit integration, call history in Phone app
- **VoIP push notifications** — Get notified when agents complete tasks, even in background
- **Voice input** — Apple SFSpeechRecognizer real-time transcription
- **Phone call mode** — Fullscreen voice conversation with VAD
- **RTC calls** — Real-time voice/video via VolcEngine RTC
- **MetaMemory browser** — Search and browse knowledge base with folder navigation
- **Group chat** — Multi-agent conversations with @mention routing
- **Team dashboard** — View all agent statuses
- **iPad split view** — Three-column layout for tablets
- **Markdown rendering** — Rich text via swift-markdown-ui
- **Keychain auth** — Secure token storage

**Requirements**: iOS 17.0+, Xcode 15+, Apple Developer Account

> iOS setup is more involved (developer account, push certificates, etc.). See [iOS App docs](https://xvirobotics.com/metabot/features/ios-app/).

## Core Components

| Component | Description |
|-----------|-------------|
| **Claude Code Kernel** | Every bot is a full Claude Code instance — Read, Write, Edit, Bash, Glob, Grep, WebSearch, MCP, and more. `bypassPermissions` mode for autonomous operation. |
| **MetaSkill** | Agent factory. `/metaskill ios app` generates a complete agent team after researching best practices. |
| **MetaMemory** | Embedded SQLite knowledge store with full-text search and Web UI. Shared by all agents. Auto-syncs to Feishu Wiki. |
| **Agent Bus** | REST API. Agents talk to each other via `mb talk`. Create/remove bots at runtime. |
| **Task Scheduler** | Cron jobs + one-time delays. Timezone-aware, persists across restarts. |
| **Peers Federation** | Cross-instance bot discovery and task routing across multiple MetaBot instances. |
| **Feishu Doc Reader** | Read Feishu documents and wiki pages as Markdown. |
| **Voice Assistant** | Jarvis mode — "Hey Siri, Jarvis" from AirPods for hands-free agent control. |

## Install

```bash
# Linux / macOS
curl -fsSL https://raw.githubusercontent.com/xvirobotics/metabot/main/install.sh | bash

# Windows (PowerShell)
irm https://raw.githubusercontent.com/xvirobotics/metabot/main/install.ps1 | iex
```

The installer walks you through: working directory → Claude auth → IM credentials → auto-start with PM2.

```bash
metabot update    # Pull, rebuild, restart anytime
```

<details>
<summary><strong>Manual install</strong></summary>

```bash
git clone https://github.com/xvirobotics/metabot.git
cd metabot && npm install
cp bots.example.json bots.json   # edit with your bot configs
cp .env.example .env              # edit global settings
npm run dev
```

Prerequisites: Node.js 20+, [Claude Code CLI](https://github.com/anthropics/claude-code) installed and authenticated.

</details>

## Quick Setup

**Telegram** (30 seconds):
1. Message [@BotFather](https://t.me/BotFather) → `/newbot` → copy token
2. Add to `bots.json` → done

**Feishu/Lark**:
1. Create app at [open.feishu.cn](https://open.feishu.cn/) → add Bot capability
2. Enable permissions: `im:message`, `im:message:readonly`, `im:resource`, `im:chat:readonly`, `docx:document:readonly`, `wiki:wiki`
3. Start MetaBot, then enable persistent connection + `im.message.receive_v1` event
4. Publish the app

**Web UI**: Visit `http://localhost:9100/web/` after starting MetaBot, enter your API_SECRET.

**iOS App**: Build with Xcode and install on your device. See [iOS App docs](https://xvirobotics.com/metabot/features/ios-app/).

> For detailed configuration (environment variables, multi-bot setup, HTTPS, voice, iOS push certificates), see the [documentation site](https://xvirobotics.com/metabot/).

## Configuration

**`bots.json`** — define your bots:

```json
{
  "feishuBots": [{
    "name": "metabot",
    "feishuAppId": "cli_xxx",
    "feishuAppSecret": "...",
    "defaultWorkingDirectory": "/home/user/project"
  }],
  "telegramBots": [{
    "name": "tg-bot",
    "telegramBotToken": "123456:ABC...",
    "defaultWorkingDirectory": "/home/user/project"
  }]
}
```

<details>
<summary><strong>Environment variables (.env)</strong></summary>

| Variable | Default | Description |
|----------|---------|-------------|
| `API_PORT` | 9100 | HTTP API port |
| `API_SECRET` | — | Bearer token auth (protects API + Web UI) |
| `MEMORY_ENABLED` | true | Enable MetaMemory |
| `MEMORY_PORT` | 8100 | MetaMemory port |
| `VOLCENGINE_TTS_APPID` | — | Doubao voice (TTS + STT) |
| `VOLCENGINE_TTS_ACCESS_KEY` | — | Doubao voice key |

Full variable list at [configuration docs](https://xvirobotics.com/metabot/configuration/environment-variables/).

</details>

<details>
<summary><strong>Third-party AI providers</strong></summary>

MetaBot supports any Anthropic-compatible API:

```bash
ANTHROPIC_BASE_URL=https://api.moonshot.ai/anthropic    # Kimi/Moonshot
ANTHROPIC_BASE_URL=https://api.deepseek.com/anthropic   # DeepSeek
ANTHROPIC_BASE_URL=https://api.z.ai/api/anthropic       # GLM/Zhipu
ANTHROPIC_AUTH_TOKEN=your-key
```

</details>

## What You Can Build

- **Solo AI developer** — full Claude Code from your phone via Feishu/Telegram/iOS
- **Multi-agent team** — frontend, backend, infra bots talking via Agent Bus
- **Self-growing organization** — a manager bot that creates agents on demand
- **Autonomous research pipeline** — agents that search, analyze, save to MetaMemory
- **Voice assistant** — "Hey Siri, Jarvis" or Web/iOS phone call mode

## CLI Tools

```bash
metabot update                      # one-command update
mm search "query"                   # search knowledge base
mb bots                             # list all bots
mb talk <bot> <chatId> <prompt>     # talk to a bot
mb schedule list                    # list scheduled tasks
fd read <feishu-url>                # read Feishu document
```

Full CLI reference at [docs](https://xvirobotics.com/metabot/reference/cli-mb/).

## Security

- Runs in `bypassPermissions` mode — Claude has full access to the working directory
- Control access via Feishu/Telegram platform settings
- `API_SECRET` protects both API and Web UI
- `maxBudgetUsd` caps cost per request
- MetaMemory supports folder-level ACL

## FAQ

**No public IP needed?** — Correct. Feishu uses WebSocket, Telegram uses long polling.

**Non-Claude models?** — Yes. Any Anthropic-compatible API (Kimi, DeepSeek, GLM, etc.)

**Do I need to publish the iOS app?** — No. Build with Xcode and install directly on your device.

## About

MetaBot is built by [XVI Robotics](https://xvirobotics.com), where we develop humanoid robot brains. We use MetaBot internally to run our company as an agent-native organization — a small team of humans supervising self-improving AI agents. We open-sourced it because we believe this is how companies will work in the future.

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=xvirobotics/metabot&type=Date)](https://star-history.com/#xvirobotics/metabot&Date)

## License

[MIT](LICENSE)
