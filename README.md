# MetaBot

[![CI](https://github.com/xvirobotics/metabot/actions/workflows/ci.yml/badge.svg)](https://github.com/xvirobotics/metabot/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![GitHub stars](https://img.shields.io/github/stars/xvirobotics/metabot?style=social)](https://github.com/xvirobotics/metabot)

[English](#english) | [中文](#中文)

---

<a id="english"></a>

## English

MetaBot — A bridge service connecting IM bots (Feishu/Lark, Telegram) to Claude Code Agent SDK. Chat with Claude Code from Feishu or Telegram on any device (including mobile), with real-time streaming updates.

### Features

- **Multi-platform** - Supports both Feishu/Lark and Telegram, run them side by side in a single process
- **Remote access** - Use Claude Code from any device, including mobile
- **Streaming updates** - Real-time execution progress via message updates (Feishu interactive cards / Telegram editMessageText)
- **Multi-bot support** - Run multiple bots in one process, each bound to a different project directory
- **Multi-user parallel** - Independent sessions per chat (each group/DM has its own session)
- **Multi-turn conversations** - Automatic context persistence across messages
- **Image support** - Send images to Claude for analysis; Claude-generated images are sent back
- **MCP integration** - Automatically loads MCP server configs from Claude Code settings
- **Status cards** - Color-coded status, tool call tracking, cost/duration stats
- **HTTP API** - Inter-bot task delegation via REST API; Claude calls other bots with `curl`
- **Task Scheduler** - Schedule future tasks ("check back in 2 hours") with persistence and auto-retry

### Prerequisites

- **Node.js 18+**
- **Claude Code authentication** - Either logged in via `claude login` (subscription) or `ANTHROPIC_API_KEY` env var
- **Feishu Open Platform account** and/or **Telegram Bot Token** (from [@BotFather](https://t.me/BotFather))

### Feishu Bot Setup

1. Go to [Feishu Open Platform](https://open.feishu.cn/) and create an enterprise app
2. Go to **App Capabilities** → Add **Bot**
3. Go to **Permissions** and enable:
   - `im:message` - Send and receive messages
   - `im:message:readonly` - Read messages
   - `im:resource` - Upload images and files (required for sending output files back to chat)
4. **Start the service first** (`./setup.sh` or `pm2 start ecosystem.config.cjs`), then configure events:
   - Go to **Events & Callbacks** → **Event Configuration**
   - Select **"Use persistent connection to receive events"** (WebSocket mode, no public IP needed)
   - Feishu validates the WebSocket connection on save, so the service must be running
   - Add event: `im.message.receive_v1`
5. Publish the app version and get approval

### Telegram Bot Setup

1. Open Telegram and message [@BotFather](https://t.me/BotFather)
2. Send `/newbot`, follow the prompts to set a name and username
3. Copy the **bot token** (e.g. `123456:ABC-DEF...`)
4. Add it to `.env` as `TELEGRAM_BOT_TOKEN` or to `bots.json` (see below)

That's it — no webhooks needed. MetaBot uses **long polling**, so no public IP is required.

### Quick Start

```bash
git clone <your-repo-url>
cd metabot
./setup.sh
```

The setup script will check prerequisites, install dependencies, prompt for your Feishu app credentials, and start the service with PM2.

### Manual Configuration

```bash
cp bots.example.json bots.json   # edit with your bot configs
cp .env.example .env              # edit global settings
```

**`bots.json`** — defines bots for one or both platforms (see `bots.example.json`):

```json
{
  "feishuBots": [
    { "name": "feishu-project", "feishuAppId": "cli_xxx", "feishuAppSecret": "...", "defaultWorkingDirectory": "/path/to/project" }
  ],
  "telegramBots": [
    { "name": "tg-project", "telegramBotToken": "123456:ABC...", "defaultWorkingDirectory": "/path/to/project" }
  ]
}
```

> **Backward compatible:** If `bots.json` is a plain array (old format), all entries are treated as Feishu bots.

**Feishu bot fields:**

| Field | Required | Default | Description |
|-------|----------|---------|-------------|
| `name` | Yes | - | Bot identifier (used in logs) |
| `feishuAppId` | Yes | - | Feishu App ID |
| `feishuAppSecret` | Yes | - | Feishu App Secret |
| `defaultWorkingDirectory` | Yes | - | Fixed working directory for this bot |
| `authorizedUserIds` | No | (allow all) | Array of user open_ids |
| `authorizedChatIds` | No | (allow all) | Array of chat_ids |
| `allowedTools` | No | Read,Edit,Write,Glob,Grep,Bash | Allowed Claude tools |
| `maxTurns` | No | unlimited | Max conversation turns per query |
| `maxBudgetUsd` | No | unlimited | Max cost per query (USD) |
| `model` | No | SDK default | Claude model |

**Telegram bot fields:** Same as above, but replace `feishuAppId`/`feishuAppSecret` with `telegramBotToken`.

**`.env`** — global settings:

| Variable | Required | Default | Description |
|----------|----------|---------|-------------|
| `BOTS_CONFIG` | No | - | Path to `bots.json`. If unset, falls back to single-bot env vars |
| `API_PORT` | No | 9100 | HTTP API server port |
| `API_SECRET` | No | - | If set, listens on 0.0.0.0 with Bearer token auth; if empty, localhost only |
| `LOG_LEVEL` | No | info | Log level |

<details>
<summary>Single-bot mode (env var config)</summary>

If `BOTS_CONFIG` is not set, bots are configured from env vars. You can run Feishu, Telegram, or both:

| Variable | Required | Description |
|----------|----------|-------------|
| `FEISHU_APP_ID` | For Feishu | Feishu App ID |
| `FEISHU_APP_SECRET` | For Feishu | Feishu App Secret |
| `TELEGRAM_BOT_TOKEN` | For Telegram | Telegram bot token from BotFather |
| `CLAUDE_DEFAULT_WORKING_DIRECTORY` | Yes | Working directory |
| `AUTHORIZED_USER_IDS` | No | Feishu user open_ids (comma-separated) |
| `TELEGRAM_AUTHORIZED_USER_IDS` | No | Telegram user IDs (comma-separated) |
| `CLAUDE_ALLOWED_TOOLS` | No | Comma-separated tools |
| `CLAUDE_MAX_TURNS` | No | Max turns (unlimited if unset) |
| `CLAUDE_MAX_BUDGET_USD` | No | Max budget (unlimited if unset) |
| `CLAUDE_MODEL` | No | Claude model |

</details>

### Usage

```bash
# Development (hot reload)
npm run dev

# Production (PM2 — auto-restart on crash & code changes)
pm2 start ecosystem.config.cjs

# PM2 common commands
pm2 status              # View process status
pm2 logs                # Tail logs in real-time
pm2 restart all         # Manual restart
pm2 stop all            # Stop service
```

To enable auto-start on server reboot:

```bash
pm2 startup             # Follow the printed sudo command
pm2 save                # Save current process list
```

### Commands

| Command | Description |
|---------|-------------|
| `/reset` | Clear session, start fresh |
| `/stop` | Abort current running task |
| `/status` | Show current session info |
| `/memory` | Memory document commands (list, search, status) |
| `/help` | Show help message |

### Image Support

**Send images to Claude:** Send an image in chat, and Claude will analyze it (both Feishu and Telegram).

**Receive images from Claude:** When Claude generates or writes image files (via Write tool, Bash, or MCP tools), they are automatically uploaded and sent back to chat.

Supported formats: PNG, JPEG, GIF, WEBP, BMP, SVG, TIFF (max 10MB for Feishu, 50MB for Telegram).

### MetaMemory (Shared Knowledge Base)

MetaMemory is a document server for persistent shared memory. Claude autonomously reads/writes documents via the `metamemory` skill, and humans can browse via a Web UI.

**How it works:**
- A separate MetaMemory server (FastAPI + SQLite) stores documents as Markdown in a folder tree with full-text search
- Claude uses the `metamemory` skill to create/read/update documents via the server API
- The `/memory` commands provide quick access to folder listings and search results
- A Web UI at `http://localhost:8100` lets you browse and manage documents

**Commands:**
```
/memory list          — Show folder tree
/memory search query  — Search documents
/memory status        — Server health check
```

**Configuration:** Set `MEMORY_SERVER_URL` in `.env` (default: `http://localhost:8100`). Run the MetaMemory server via Docker: see `xvirobotics/metamemory`.

### MCP Server Configuration

MCP servers are loaded from Claude Code's standard config files — no extra setup needed:

- **Global (all projects):** `~/.claude/settings.json`
- **Per-project:** `<working-directory>/.claude/settings.json`

Example config:

```json
{
  "mcpServers": {
    "my-server": {
      "command": "npx",
      "args": ["-y", "my-mcp-server"],
      "env": { "API_KEY": "..." }
    }
  }
}
```

The bot loads MCP servers based on the bot's configured working directory. If you already have MCP servers configured for Claude Code CLI, they work automatically.

### HTTP API & Task Scheduler

MetaBot includes a built-in HTTP API server for inter-bot communication and task scheduling. Claude uses these features autonomously via `curl` — the API docs are injected into Claude's system prompt.

**API Server** — Starts automatically on `localhost:9100` (configurable via `API_PORT`). If `API_SECRET` is set, listens on `0.0.0.0` with Bearer token auth.

**Endpoints:**

| Method | Path | Description |
|--------|------|-------------|
| `GET` | `/api/health` | Service health (uptime, bot count, scheduled tasks) |
| `GET` | `/api/bots` | List registered bots (name, platform, workingDir, tools) |
| `POST` | `/api/tasks` | Submit a task to a bot (sync, returns result) |
| `POST` | `/api/schedule` | Schedule a future task |
| `GET` | `/api/schedule` | List pending scheduled tasks |
| `DELETE` | `/api/schedule/:id` | Cancel a scheduled task |

**Examples:**

```bash
# Health check
curl localhost:9100/api/health

# List bots
curl localhost:9100/api/bots

# Delegate a task to another bot
curl -X POST localhost:9100/api/tasks \
  -H 'Content-Type: application/json' \
  -d '{"botName":"alice","chatId":"oc_xxx","prompt":"check test results"}'

# Schedule a task for 1 hour later
curl -X POST localhost:9100/api/schedule \
  -H 'Content-Type: application/json' \
  -d '{"botName":"alice","chatId":"oc_xxx","prompt":"check experiment","delaySeconds":3600,"label":"experiment check"}'

# List pending scheduled tasks
curl localhost:9100/api/schedule

# Cancel a scheduled task
curl -X DELETE localhost:9100/api/schedule/<task-id>
```

**Task Scheduler** — Persists to `~/.metabot/scheduled-tasks.json`. On startup, pending tasks are restored. If the target chat is busy, retries every 30s (max 5 times). Tasks overdue by >24h are skipped as stale.

**How Claude uses it:** Claude sees its own `botName`, `chatId`, and the API port in the system prompt. It can delegate work to other bots or schedule future self-checks using `curl` via the Bash tool — no MCP configuration needed.

### Security Note

This service runs Claude Code in **`bypassPermissions` mode** — Claude can read, write, and execute commands without interactive approval, since there is no terminal for user confirmation in a chat bot context.

**Make sure you understand the implications before deploying:**

- Claude has full read/write access to the working directory
- Claude can execute arbitrary shell commands if `Bash` is in the allowed tools
- Use `CLAUDE_ALLOWED_TOOLS` to restrict capabilities (e.g. remove `Bash` for read-only use)
- Use `maxBudgetUsd` in bot config to cap per-request cost
- Use `authorizedUserIds` in bot config to restrict who can access the bot
- **Never point the bot at directories containing sensitive data without proper access controls**

### Architecture

```
Feishu User                          Telegram User
  → [WSClient] WebSocket               → [grammY] Long Polling
  → [Event Handler]                     → [Telegram Bot Handler]
  → [FeishuSenderAdapter]               → [TelegramSender]
       ↘                               ↙
        [Message Bridge] (shared, platform-agnostic)
          → Claude Code Agent SDK
          → Stream Processor → throttled updates (1.5s)
          → Session Manager, Rate Limiter, Outputs Manager

        [Bot Registry] ← registers all bots at startup
             ↕
        [HTTP API Server] :9100
          → POST /api/tasks     (inter-bot delegation)
          → POST /api/schedule  (future task scheduling)
             ↕
        [Task Scheduler] → fires tasks → Message Bridge
          → persists to ~/.metabot/scheduled-tasks.json
```

---

<a id="中文"></a>

## 中文

MetaBot — 多平台 IM Bot 连接 Claude Code 的桥接服务。支持飞书和 Telegram，在手机端通过聊天远程控制本机的 Claude Code，实时查看执行过程和结果。

### 功能特性

- **多平台支持** - 同时支持飞书（Feishu/Lark）和 Telegram，可在单进程中并行运行
- **远程访问** - 在任意设备上使用 Claude Code，手机也能写代码
- **流式更新** - 实时展示执行进度（飞书交互卡片 / Telegram 消息编辑）
- **多机器人支持** - 单进程运行多个 Bot，每个 Bot 绑定不同项目目录
- **多用户并行** - 每个会话（群聊/私聊）独立会话，互不干扰
- **多轮对话** - 自动维护对话上下文，支持连续交互
- **图片支持** - 发图片给 Claude 分析；Claude 生成的图片自动回传
- **MCP 集成** - 自动加载 Claude Code 配置文件中的 MCP 服务器
- **状态卡片** - 颜色标识状态、工具调用追踪、费用/耗时统计
- **HTTP API** - Bot 间任务委派，Claude 通过 `curl` 调用其他 Bot
- **定时任务** - 支持延时任务调度（"2小时后检查一下"），持久化存储，自动重试

### 前置条件

- **Node.js 18+**
- **Claude Code 认证** - 通过 `claude login` 登录（订阅用户），或设置 `ANTHROPIC_API_KEY` 环境变量
- **飞书开放平台账号**（飞书 Bot）和/或 **Telegram Bot Token**（从 [@BotFather](https://t.me/BotFather) 获取）

---

### 第一步：创建飞书机器人

#### 1.1 创建应用

1. 打开 [飞书开放平台](https://open.feishu.cn/)，登录后点击「创建企业自建应用」
2. 填写应用名称（如 `Claude Code Bot`）和描述，点击创建

#### 1.2 获取凭证

1. 进入应用详情页，左侧菜单找到「凭证与基础信息」
2. 记录 **App ID** 和 **App Secret**，后面配置要用

#### 1.3 开启机器人能力

1. 左侧菜单 →「应用能力」→「添加应用能力」
2. 选择「机器人」，点击添加

#### 1.4 配置权限

1. 左侧菜单 →「权限管理」
2. 搜索并开通以下权限：
   - `im:message` - 获取与发送单聊、群组消息
   - `im:message:readonly` - 读取消息（如已有 `im:message` 可跳过）
   - `im:resource` - 上传图片和文件（用于将 Claude 产出的文件发回聊天）

#### 1.5 配置事件订阅（需要先启动服务）

> **重要**：飞书在保存长连接配置时会验证 WebSocket 连接，因此必须**先启动服务**（第二步），再回来配置此步骤。

1. 左侧菜单 →「事件与回调」→「事件配置」
2. **订阅方式选择「使用长连接接收事件」**（不需要公网 IP，本地即可运行）
3. 点击保存 — 飞书会验证 WebSocket 连接
4. 添加事件：搜索并添加 `接收消息 im.message.receive_v1`

#### 1.6 发布应用

1. 左侧菜单 →「版本管理与发布」
2. 创建版本并提交审核
3. 管理员在飞书管理后台审核通过后即可使用

> **提示**：如果是个人测试，可以在「应用发布」中选择仅对自己可见的测试范围。

---

### Telegram 机器人配置（可选）

1. 在 Telegram 中找到 [@BotFather](https://t.me/BotFather)
2. 发送 `/newbot`，按提示设置名称和用户名
3. 复制 **bot token**（格式如 `123456:ABC-DEF...`）
4. 写入 `.env` 的 `TELEGRAM_BOT_TOKEN` 或加到 `bots.json`（见下方配置）

无需 Webhook，MetaBot 使用**长轮询（Long Polling）**模式，不需要公网 IP。

---

### 第二步：一键部署

```bash
git clone <your-repo-url>
cd metabot
./setup.sh
```

脚本会自动检查环境、安装依赖、引导配置飞书凭证，并通过 PM2 启动服务。

如需手动配置，继续看下面的步骤。

---

### 第三步：配置

```bash
cp bots.example.json bots.json   # 编辑 Bot 配置
cp .env.example .env              # 编辑全局设置
```

**`bots.json`** — 定义一个或多个 Bot（参考 `bots.example.json`），支持飞书和 Telegram 混合配置：

```json
{
  "feishuBots": [
    {
      "name": "feishu-project",
      "feishuAppId": "cli_xxx",
      "feishuAppSecret": "xxxxxxxxxx",
      "defaultWorkingDirectory": "/path/to/project",
      "authorizedUserIds": ["ou_xxxx"]
    }
  ],
  "telegramBots": [
    {
      "name": "tg-project",
      "telegramBotToken": "123456:ABC-DEF...",
      "defaultWorkingDirectory": "/path/to/project",
      "authorizedUserIds": ["12345678"]
    }
  ]
}
```

> **向后兼容**：如果 `bots.json` 是数组格式（旧格式），所有条目作为飞书 Bot 处理。

**飞书 Bot 字段：**

| 字段 | 必填 | 默认值 | 说明 |
|------|------|--------|------|
| `name` | 是 | - | Bot 标识名（用于日志区分） |
| `feishuAppId` | 是 | - | 飞书应用 App ID |
| `feishuAppSecret` | 是 | - | 飞书应用 App Secret |
| `defaultWorkingDirectory` | 是 | - | 固定工作目录 |
| `authorizedUserIds` | 否 | 不限制 | 允许使用的用户 open_id 数组 |
| `authorizedChatIds` | 否 | 不限制 | 允许使用的 chat_id 数组 |
| `allowedTools` | 否 | Read,Edit,Write,Glob,Grep,Bash | Claude 可用工具 |
| `maxTurns` | 否 | 不限制 | 每次请求最大对话轮数 |
| `maxBudgetUsd` | 否 | 不限制 | 每次请求最大花费（美元） |
| `model` | 否 | SDK 默认 | 指定 Claude 模型 |

**Telegram Bot 字段：** 与飞书相同，但用 `telegramBotToken` 替代 `feishuAppId`/`feishuAppSecret`。

**`.env`** — 全局设置：

```bash
# 指向 Bot 配置文件
BOTS_CONFIG=./bots.json

# HTTP API（Bot 间通信和定时任务）
API_PORT=9100
# API_SECRET=xxx   # 设置后监听 0.0.0.0 并启用 Bearer 认证；不设则仅监听 localhost

# 日志级别
LOG_LEVEL=info
```

> **多 Bot 模式**：在 `bots.json` 中定义多个条目即可。飞书和 Telegram Bot 可以混合配置，在单个进程内同时运行。

#### 关于 Claude Code 认证

有两种方式，任选其一：

**方式一：订阅用户（推荐）**

如果你有 Claude Pro/Max 订阅，直接登录即可：

```bash
claude login
```

**方式二：API Key**

设置 `ANTHROPIC_API_KEY` 环境变量：

```bash
# 加到你的 ~/.bashrc 或 ~/.zshrc 中
export ANTHROPIC_API_KEY=sk-ant-xxxxxxxxxx
```

---

### 第四步：启动服务

#### 开发模式（推荐先用这个测试）

```bash
npm run dev
```

#### 生产模式（PM2）

使用 PM2 进程管理器，支持崩溃自动重启、代码变更自动重启：

```bash
# 安装 PM2（如未安装）
npm install -g pm2

# 启动服务
pm2 start ecosystem.config.cjs
```

PM2 常用命令：

```bash
pm2 status              # 查看进程状态
pm2 logs                # 实时查看日志
pm2 restart all         # 手动重启
pm2 stop all            # 停止服务
```

设置开机自启：

```bash
pm2 startup             # 按照提示执行打印出的 sudo 命令
pm2 save                # 保存当前进程列表
```

看到类似以下日志表示启动成功：

```
[INFO] All bots started — bots: ["metabot"]
[ws] ws client ready
```

---

### 第五步：开始使用

#### 基本流程

1. 在飞书或 Telegram 中找到你的机器人（私聊或拉入群组）
2. 直接发送消息开始和 Claude Code 对话（工作目录已在配置中固定）
3. 消息会实时更新执行进度，完成后显示最终结果

#### 可用命令

| 命令 | 说明 |
|------|------|
| `/reset` | 清除对话历史，重新开始 |
| `/stop` | 中止当前正在执行的任务 |
| `/status` | 查看当前会话状态 |
| `/memory` | 知识文档命令（list、search、status） |
| `/help` | 显示帮助信息 |

#### 使用示例

```
你：帮我看看这个项目的结构，有哪些主要模块
Bot：🔵 Thinking... → 🔵 Running... → 🟢 Complete
    （卡片实时更新，展示 Claude 正在读取哪些文件、分析结果等）

你：给 utils.ts 加上单元测试
Bot：（Claude 会读取文件、编写测试、运行测试，全程卡片更新）

你：/reset
Bot：✅ Session Reset - 开始新对话
```

---

### 卡片状态说明

| 颜色 | 状态 | 含义 |
|------|------|------|
| 🔵 蓝色 | Thinking | Claude 正在思考 |
| 🔵 蓝色 | Running | 正在执行工具（读文件、写代码、运行命令等） |
| 🟢 绿色 | Complete | 执行完成 |
| 🔴 红色 | Error | 执行出错 |

卡片底部会显示：耗时、API 花费等统计信息。

---

### 多用户 / 多 Bot 说明

- 会话按**聊天**（chat_id）隔离，每个群聊和私聊都有独立的会话
- 每个 Bot 绑定固定的工作目录，不同 Bot 对应不同项目
- 不同聊天的任务可以同时并行执行
- 飞书 Bot 通过 WebSocket 长连接，Telegram Bot 通过长轮询，均不需要公网 IP
- 多个 Bot（可跨平台）在单进程中运行，共享 Claude 执行引擎

---

### 图片支持

**发送图片给 Claude：** 在飞书或 Telegram 中直接发送图片，Claude 会自动分析图片内容。

**接收 Claude 生成的图片：** 当 Claude 通过工具（Write、Bash、MCP 等）生成图片文件时，图片会自动上传并发送到聊天中。

支持格式：PNG、JPEG、GIF、WEBP、BMP、SVG、TIFF（飞书最大 10MB，Telegram 最大 50MB）。

---

### MetaMemory（共享知识库）

MetaMemory 是一个文档服务器，提供持久化共享记忆。Claude 通过 `metamemory` skill 自主读写文档，人通过 Web UI 浏览。

**工作原理：**
- 独立的 MetaMemory 服务器（FastAPI + SQLite）以文件夹树形式存储 Markdown 文档，支持全文搜索
- Claude 通过 `metamemory` skill 自动调用服务器 API 创建/读取/更新文档
- `/memory` 命令提供快速查询文件夹列表和搜索结果
- Web UI `http://localhost:8100` 支持浏览和管理文档

**命令：**
```
/memory list          — 显示文件夹树
/memory search 关键词  — 搜索文档
/memory status        — 服务器健康检查
```

**配置：** 在 `.env` 中设置 `MEMORY_SERVER_URL`（默认 `http://localhost:8100`）。通过 Docker 运行 MetaMemory 服务器：见 `xvirobotics/metamemory`。

---

### MCP 服务器配置

MCP 服务器配置直接复用 Claude Code 的标准配置文件，无需额外设置：

- **全局配置（所有项目共用）：** `~/.claude/settings.json`
- **项目配置（特定目录生效）：** `<工作目录>/.claude/settings.json`

配置示例：

```json
{
  "mcpServers": {
    "my-server": {
      "command": "npx",
      "args": ["-y", "my-mcp-server"],
      "env": { "API_KEY": "..." }
    }
  }
}
```

Bot 会根据配置中的工作目录加载对应的 MCP 配置。如果你已经为 Claude Code CLI 配置过 MCP 服务器，它们会自动生效。

---

### HTTP API 与定时任务

MetaBot 内置 HTTP API 服务器，支持 Bot 间任务委派和定时任务调度。Claude 通过 `curl` 自主使用这些功能 —— API 文档会自动注入 Claude 的系统提示词中。

**API 服务器** — 随服务自动启动在 `localhost:9100`（可通过 `API_PORT` 配置）。设置 `API_SECRET` 后监听 `0.0.0.0` 并要求 Bearer 认证。

**接口：**

| 方法 | 路径 | 说明 |
|------|------|------|
| `GET` | `/api/health` | 服务健康检查（运行时长、Bot 数量、定时任务数） |
| `GET` | `/api/bots` | 列出所有注册的 Bot |
| `POST` | `/api/tasks` | 向指定 Bot 提交任务（同步，返回结果） |
| `POST` | `/api/schedule` | 创建定时任务 |
| `GET` | `/api/schedule` | 查看待执行的定时任务 |
| `DELETE` | `/api/schedule/:id` | 取消定时任务 |

**使用示例：**

```bash
# 健康检查
curl localhost:9100/api/health

# 列出所有 Bot
curl localhost:9100/api/bots

# 委派任务给其他 Bot
curl -X POST localhost:9100/api/tasks \
  -H 'Content-Type: application/json' \
  -d '{"botName":"alice","chatId":"oc_xxx","prompt":"检查测试结果"}'

# 1 小时后执行定时任务
curl -X POST localhost:9100/api/schedule \
  -H 'Content-Type: application/json' \
  -d '{"botName":"alice","chatId":"oc_xxx","prompt":"检查实验结果","delaySeconds":3600,"label":"实验检查"}'

# 查看待执行的定时任务
curl localhost:9100/api/schedule

# 取消定时任务
curl -X DELETE localhost:9100/api/schedule/<task-id>
```

**定时任务调度器** — 任务持久化存储在 `~/.metabot/scheduled-tasks.json`，服务重启后自动恢复。如果目标聊天正在忙，每 30 秒重试一次（最多 5 次）。超过 24 小时未执行的任务视为过期跳过。

**Claude 如何使用：** Claude 的系统提示词中会自动注入当前 `botName`、`chatId` 和 API 端口信息。Claude 可以通过 Bash 工具执行 `curl` 来委派任务给其他 Bot 或给自己创建定时任务 —— 无需配置 MCP。

---

### 架构概览

```
飞书用户                              Telegram 用户
  → [WSClient 长连接]                    → [grammY 长轮询]
  → [Event Handler]                     → [Telegram Bot Handler]
  → [FeishuSenderAdapter]               → [TelegramSender]
       ↘                               ↙
        [Message Bridge] 共享核心（平台无关）
          → Claude Code Agent SDK
          → 流式处理 → 节流更新（1.5s 间隔）
          → 会话管理、限流、输出文件管理

        [Bot Registry] ← 启动时注册所有 Bot
             ↕
        [HTTP API Server] :9100
          → POST /api/tasks     (Bot 间任务委派)
          → POST /api/schedule  (定时任务调度)
             ↕
        [Task Scheduler] → 触发任务 → Message Bridge
          → 持久化存储 ~/.metabot/scheduled-tasks.json
```

---

### 安全说明

本服务以 **`bypassPermissions`（跳过权限确认）模式** 运行 Claude Code —— Claude 可以直接读写文件和执行命令，无需交互式确认（因为聊天机器人场景下没有终端来审批）。

**部署前请确保你了解以下影响：**

- Claude 对工作目录拥有完整的读写权限
- 如果允许工具列表中包含 `Bash`，Claude 可以执行任意 Shell 命令
- 通过 `allowedTools` 限制可用工具（例如去掉 `Bash` 实现只读模式）
- 通过 `maxBudgetUsd` 限制单次请求的最大花费
- 通过 `authorizedUserIds` 限制谁可以使用机器人
- **不要将机器人指向包含敏感数据的目录，除非已做好访问控制**

---

### 常见问题

**Q: 需要公网 IP 吗？**

不需要。飞书使用 **WebSocket 长连接**，Telegram 使用**长轮询**，两者都只需能访问外网即可，无需域名或公网 IP。

**Q: 为什么消息没有响应？**

检查以下几点：
1. 应用是否已发布并审核通过
2. 事件订阅是否选择了「长连接」模式
3. 是否添加了 `im.message.receive_v1` 事件
4. 权限 `im:message` 是否已开通
5. 如果配置了 `authorizedUserIds`，确认你的 open_id 在列表中

**Q: 如何获取用户的 open_id？**

发送 `/status` 命令，卡片中会显示当前用户的 open_id。

**Q: 如何限制只有特定人可以使用？**

在 `bots.json` 中设置 `authorizedUserIds`：

```json
{
  "name": "my-bot",
  "authorizedUserIds": ["ou_xxxx1", "ou_xxxx2"],
  ...
}
```

**Q: Claude 执行超时了怎么办？**

默认 10 分钟超时。如果任务确实需要更长时间，可以拆分为多个小任务。也可以用 `/stop` 手动中止后重试。

---

## License

[MIT](LICENSE)
