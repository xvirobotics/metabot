# MetaBot

**构建受监督的、自我进化的 Agent 组织的基础设施。**

[![CI](https://img.shields.io/github/actions/workflow/status/xvirobotics/metabot/ci.yml?branch=main&style=flat-square)](https://github.com/xvirobotics/metabot/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg?style=flat-square)](https://opensource.org/licenses/MIT)
[![GitHub stars](https://img.shields.io/github/stars/xvirobotics/metabot?style=flat-square)](https://github.com/xvirobotics/metabot)

中文 | [English](README_EN.md) | [文档站](https://xvirobotics.com/metabot/zh/)

---

![MetaBot 架构图](resources/metabot.png)

## 为什么做 MetaBot

Claude Code 是最强的 AI 编码 Agent —— 但它被锁在笔记本终端里。

MetaBot 解放了它。给每个 Agent 一个 Claude Code 大脑、持久化的共享记忆、创建新 Agent 的能力、以及通信总线。全部可以从**飞书、Telegram、浏览器、iPhone** 随时随地控制。

我们做 MetaBot 是为了把 [XVI Robotics](https://xvirobotics.com) 打造成一个 **Agent Native 公司** —— 一个小团队的人类，监督一个自我进化的 AI Agent 组织。这是让这一切成为可能的基础设施。

## 多端接入

MetaBot 支持 4 种方式与你的 Agent 团队交互：

| 客户端 | 场景 | 特色功能 |
|--------|------|---------|
| **飞书/Lark** | 工作场景，团队协作 | 流式交互卡片、@mention 路由、知识库自动同步 |
| **Telegram** | 个人/国际用户 | 30 秒配置、长轮询无需公网 IP、群聊 + 私聊 |
| **Web UI** | 浏览器端，语音对话 | 电话语音模式（VAD）、RTC 实时通话、MetaMemory 浏览器、团队看板 |
| **iOS App** | iPhone/iPad 原生体验 | CallKit 来电界面、VoIP 推送、语音识别、iPad 分栏视图 |

## Web UI

浏览器端全功能聊天界面，部署即可用。访问地址：`https://your-server/web/`

- **实时流式聊天** — WebSocket 推送，Markdown 渲染，工具调用展示
- **电话语音模式** — 点击电话图标，全屏免手对话，VAD 自动检测说完
- **RTC 实时通话** — 基于火山引擎 RTC 的双向语音/视频通话
- **群聊模式** — 多个 Agent 在一个对话中协作，@mention 路由
- **MetaMemory 浏览器** — 搜索和浏览共享知识库
- **团队看板** — 查看 Agent 组织状态概览
- **文件支持** — 上传/下载文件，内联预览
- **明暗主题** — 跟随系统或手动切换

**技术栈**：React 19 + Vite + Zustand + react-markdown

> 语音功能需要 HTTPS。推荐用 Caddy 反向代理，自动管理证书。详见 [Web UI 文档](https://xvirobotics.com/metabot/zh/features/web-ui/)。

## iOS App

原生 Swift iOS 应用，提供完整的移动端 Agent 交互体验。

- **原生通话体验** — CallKit 集成，来电界面和电话 App 中显示通话记录
- **VoIP 推送通知** — Agent 完成任务时推送到手机，即使 App 在后台
- **语音输入** — Apple SFSpeechRecognizer 实时语音转文字
- **电话语音模式** — 全屏语音对话，VAD 自动检测，类似打电话
- **RTC 通话** — 基于火山引擎 RTC 的实时语音/视频
- **MetaMemory 浏览器** — 搜索和浏览知识库，支持文件夹导航
- **群聊** — 多 Agent 对话，@mention 路由
- **团队看板** — 查看所有 Agent 状态
- **iPad 分栏** — iPad 三栏布局，充分利用大屏
- **Markdown 渲染** — swift-markdown-ui 富文本展示
- **Keychain 认证** — Token 安全存储

**要求**：iOS 17.0+，Xcode 15+，需要 Apple 开发者账户

> iOS App 配置较复杂（需注册开发者账户、配置推送证书等）。详见 [iOS App 文档](https://xvirobotics.com/metabot/zh/features/ios-app/)。

## 核心能力

| 组件 | 说明 |
|------|------|
| **Claude Code 内核** | 每个 Bot 都是完整的 Claude Code 实例 — Read, Write, Edit, Bash, Glob, Grep, WebSearch, MCP 等。`bypassPermissions` 模式自主运行。 |
| **MetaSkill** | Agent 工厂。`/metaskill ios app` 调研最佳实践后生成完整的 Agent 团队。 |
| **MetaMemory** | 内嵌 SQLite 知识库，全文搜索，Web UI。所有 Agent 共享，变更自动同步飞书知识库。 |
| **Agent 总线** | REST API。Agent 通过 `mb talk` 互相对话，运行时创建/删除 Bot。 |
| **定时任务** | cron 周期任务 + 一次性延迟。支持时区，跨重启持久化。 |
| **Peers 联邦** | 跨实例 Bot 发现和任务路由，连接多个 MetaBot 实例。 |
| **飞书文档** | 读取飞书文档/知识库页面并转为 Markdown。 |
| **语音助手** | Jarvis 模式 — AirPods 说 "Hey Siri, Jarvis" 语音控制 Agent。 |

## 安装

```bash
# Linux / macOS
curl -fsSL https://raw.githubusercontent.com/xvirobotics/metabot/main/install.sh | bash

# Windows (PowerShell)
irm https://raw.githubusercontent.com/xvirobotics/metabot/main/install.ps1 | iex
```

安装器引导：工作目录 → Claude 认证 → IM 平台凭证 → PM2 自动启动。

```bash
metabot update    # 随时更新：拉取 + 构建 + 重启
```

<details>
<summary><strong>手动安装</strong></summary>

```bash
git clone https://github.com/xvirobotics/metabot.git
cd metabot && npm install
cp bots.example.json bots.json   # 编辑 Bot 配置
cp .env.example .env              # 编辑全局设置
npm run dev
```

前置条件：Node.js 20+，[Claude Code CLI](https://github.com/anthropics/claude-code) 已安装并认证。

</details>

## 快速配置

**Telegram**（30 秒）：
1. 找 [@BotFather](https://t.me/BotFather) → `/newbot` → 复制 token
2. 写入 `bots.json` → 完成

**飞书**：
1. [open.feishu.cn](https://open.feishu.cn/) 创建应用 → 添加「机器人」能力
2. 开通权限：`im:message`、`im:message:readonly`、`im:resource`、`im:chat:readonly`、`docx:document:readonly`、`wiki:wiki`
3. 先启动 MetaBot，再开启「长连接」+ `im.message.receive_v1` 事件
4. 发布应用

**Web UI**：启动 MetaBot 后访问 `http://localhost:9100/web/`，输入 API_SECRET 即可使用。

**iOS App**：需要 Xcode 编译安装。详见 [iOS App 文档](https://xvirobotics.com/metabot/zh/features/ios-app/)。

> 详细配置指南请参考 [文档站](https://xvirobotics.com/metabot/zh/)，包括环境变量、多 Bot 模式、HTTPS 设置、语音配置、iOS 推送证书等。

## 配置

**`bots.json`** — 定义你的 Bot：

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
<summary><strong>环境变量 (.env)</strong></summary>

| 变量 | 默认 | 说明 |
|------|------|------|
| `API_PORT` | 9100 | HTTP API 端口 |
| `API_SECRET` | — | Bearer 认证（同时保护 API 和 Web UI） |
| `MEMORY_ENABLED` | true | 启用 MetaMemory |
| `MEMORY_PORT` | 8100 | MetaMemory 端口 |
| `VOLCENGINE_TTS_APPID` | — | 豆包语音（TTS + STT） |
| `VOLCENGINE_TTS_ACCESS_KEY` | — | 豆包语音密钥 |

完整环境变量列表见 [配置文档](https://xvirobotics.com/metabot/zh/configuration/environment-variables/)。

</details>

<details>
<summary><strong>第三方 AI 服务商</strong></summary>

支持任何 Anthropic 兼容 API：

```bash
ANTHROPIC_BASE_URL=https://api.moonshot.ai/anthropic    # Kimi/月之暗面
ANTHROPIC_BASE_URL=https://api.deepseek.com/anthropic   # DeepSeek
ANTHROPIC_BASE_URL=https://api.z.ai/api/anthropic       # GLM/智谱
ANTHROPIC_AUTH_TOKEN=你的key
```

</details>

## 你可以构建什么

- **个人 AI 开发者** — 手机上用飞书/Telegram/iOS 远程写代码
- **多 Agent 团队** — 前端 Bot、后端 Bot、运维 Bot 通过 Agent 总线协作
- **自生长的组织** — 管理者 Bot 按需创建新 Agent，分配任务
- **自主研究流水线** — Agent 搜索、分析、存入 MetaMemory、安排下一步
- **语音助手** — AirPods "Hey Siri, Jarvis" 或 Web/iOS 电话模式免手对话

## CLI 工具

```bash
metabot update                      # 一键更新
mm search "关键词"                   # 搜索知识库
mb bots                             # 列出所有 Bot
mb talk <bot> <chatId> <prompt>     # 与 Bot 对话
mb schedule list                    # 列出定时任务
fd read <飞书链接>                    # 读取飞书文档
```

完整 CLI 参考见 [文档站](https://xvirobotics.com/metabot/zh/reference/cli-mb/)。

## 安全

- `bypassPermissions` 模式运行 — Claude 对工作目录有完整权限
- 通过飞书/Telegram 平台设置控制访问
- `API_SECRET` 保护 API 和 Web UI
- `maxBudgetUsd` 限制单次花费
- MetaMemory 支持文件夹级 ACL

## FAQ

**需要公网 IP 吗？** — 不需要。飞书用 WebSocket，Telegram 用长轮询。

**可以用国产模型吗？** — 可以。支持 Kimi、DeepSeek、GLM 等 Anthropic 兼容 API。

**iOS App 需要上架吗？** — 不需要。用 Xcode 编译后直接安装到自己的设备。

## 关于

MetaBot 由 [XVI Robotics](https://xvirobotics.com) 打造，我们做人形机器人大脑。我们在内部用 MetaBot 把公司打造成 Agent Native 组织 —— 一个小团队的人类，监督自我进化的 AI Agent。我们开源它，因为我们相信这是未来公司的运行方式。

## Star History

[![Star History Chart](https://api.star-history.com/svg?repos=xvirobotics/metabot&type=Date)](https://star-history.com/#xvirobotics/metabot&Date)

## License

[MIT](LICENSE)
