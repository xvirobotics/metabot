# 语音助手（Jarvis 模式）

通过 AirPods 和 Siri 免手免屏与任意 MetaBot Agent 语音交流。无需安装 App，只用 iOS 快捷指令。

## 工作原理

```
"Hey Siri, Jarvis"
        ↓
  Siri 听写语音 → 文字
        ↓
  HTTP POST 到 MetaBot /api/talk
        ↓
  Agent 执行任务（Claude Code）
        ↓
  通过 AirPods 语音回复
```

全程不用看屏幕。走路、爬山、开车时都能用。

## 设置方法（5 分钟）

### 前置条件

- 开启 Siri 的 iPhone
- AirPods（或任何支持 Siri 的耳机）
- MetaBot 服务器可从外网访问（公网 IP + 9100 端口开放）
- MetaBot `.env` 中的 `API_SECRET`

### 第 1 步：创建快捷指令

打开 iPhone **快捷指令** App → 右上角 **+** → 命名为 **Jarvis**

### 第 2 步：添加「听写文本」

搜索添加 **听写文本** 动作：
- **语言**：中文（中国）
- **停止聆听**：暂停之后

### 第 3 步：添加「获取 URL 内容」

搜索添加 **获取 URL 内容** 动作：

- **URL**：`http://你的服务器IP:9100/api/talk`
- **方法**：`POST`
- **头部**：
  - `Authorization` = `Bearer 你的API_SECRET`
  - `Content-Type` = `application/json`
- **请求体**：`JSON`
  - `botName` → `quanwang`（文本，或你想对话的 bot 名）
  - `chatId` → `voice_jarvis`（文本，用于保持会话连续性）
  - `prompt` → 选择上一步的 **听写的文本** 变量

> **注意**：URL 字段填固定地址，不要把「听写的文本」放进 URL 里。「听写的文本」只放在请求体的 `prompt` 字段。

### 第 4 步：添加「获取词典值」

搜索添加 **获取词典值** 动作：
- 获取 `responseText` 的 **值**
- 从：**URL 的内容**（上一步结果）

### 第 5 步：添加「朗读文本」

搜索添加 **朗读文本** 动作：
- 输入选择上一步的 **词典值**

### 第 6 步：测试

1. 戴上 AirPods
2. 说 **"Hey Siri, Jarvis"**
3. 等待听写提示，说出你的指令
4. 等几秒 — 回复会通过耳机播放

## 使用技巧

### 跟不同的 Bot 对话

创建多个快捷指令，设置不同的 `botName`：
- **"Hey Siri, Jarvis"** → `quanwang`（通用助手）
- **"Hey Siri, Goku"** → `goku`（运动控制专家）
- **"Hey Siri, 后端"** → `backend-bot`（后端开发）

### 持久会话

`chatId`（`voice_jarvis`）创建持久的 Claude 会话，和飞书聊天一样。多轮对话有效 — Agent 记得上下文。

不同场景用不同的 `chatId`：
- `voice_jarvis` — 日常任务
- `voice_code_review` — 代码审查
- `voice_research` — 调研任务

### 远程 Peer

如果 Bot 在远程 Peer 实例上，使用限定名语法：
- `botName` = `lanqi/some-bot` — 自动路由到 `lanqi` peer

### 飞书卡片同步

在 JSON 请求体中加 `sendCards` = `true`（布尔值），回复会同时发送到飞书聊天卡片。适合代码等需要稍后阅读的内容。

## 限制

- 每次交互需要重新说 "Hey Siri, Jarvis"（无法持续对话循环）
- Siri 听写对很长的语音输入可能截断
- 长回复（代码、详细分析）更适合在飞书中阅读
- 需要网络连接（Siri STT + MetaBot API）

## 安全

- API 端点通过 `API_SECRET`（Bearer token）保护
- 生产环境建议用 HTTPS（反向代理 + Let's Encrypt）
- `chatId` 固定在快捷指令中，有手机权限即可使用
