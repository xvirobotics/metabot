# Wiki 同步

MetaMemory 文档单向同步到飞书知识库。MetaMemory 的文件夹树映射为知识库节点；每个文档变成一个飞书 docx 页面。

## 概述

启用后，MetaMemory 内容自动同步到飞书知识库：

- **文件夹树** 映射为知识库节点层级
- **文档** 变成飞书 docx 页面
- **变更检测** 使用 hash 对比实现增量同步
- **自动同步** 在 MetaMemory 变更时触发（5 秒防抖）

## 聊天命令

| 命令 | 说明 |
|------|------|
| `/sync` | 触发全量同步 |
| `/sync status` | 查看同步统计 |

## 配置

| 变量 | 默认 | 说明 |
|------|------|------|
| `WIKI_SYNC_ENABLED` | `true` | 启用知识库同步 |
| `WIKI_SPACE_ID` | — | 飞书知识库空间 ID |
| `WIKI_SPACE_NAME` | `MetaMemory` | 知识库空间名称（不存在则创建） |
| `WIKI_AUTO_SYNC` | `true` | MetaMemory 变更时自动同步 |
| `WIKI_AUTO_SYNC_DEBOUNCE_MS` | `5000` | 防抖延迟 |
| `WIKI_SYNC_THROTTLE_MS` | `300` | API 调用间隔 |
| `FEISHU_SERVICE_APP_ID` | — | 专用飞书应用（回退到第一个 Bot） |
| `FEISHU_SERVICE_APP_SECRET` | — | 服务应用密钥 |
| `FEISHU_SERVICE_DOMAIN` | `feishu` | 专用服务应用所在域名：`feishu` 或 `lark` |

### Lark 国际版

知识库同步使用服务应用所在的开放平台域名：

- 未配置专用服务应用时，沿用第一个飞书 Bot 的应用及其 `larkDomain`。该 Bot 配置了 `"larkDomain": "lark"` 时，同步请求发往 `open.larksuite.com`。
- 配置了 `FEISHU_SERVICE_APP_ID` 时，用 `FEISHU_SERVICE_DOMAIN=lark` 指定该应用创建于 Lark 国际版后台；不配置则为飞书。
- `FEISHU_SERVICE_DOMAIN` 只能是 `feishu` 或 `lark`，其他值会导致启动失败。未配置专用服务应用时，如果它与第一个 Bot 的域名不一致，同样启动失败，不会把一个应用的凭证发到另一个域名。

## 所需飞书权限

在飞书开发者控制台添加：

- `wiki:wiki` — 读写知识库页面
- `docx:document` — 创建/编辑文档
- `docx:document:readonly` — 读取文档
- `drive:drive` — 访问云文档

## 自动同步行为

- 变更触发同步，5 秒防抖
- 多次快速变更合并处理
- 1-10 个文档变更 → 增量同步
- 大批量变更或文件夹结构变更 → 全量同步
- 手动 `/sync` 始终可用

## API

| 方法 | 路径 | 说明 |
|------|------|------|
| `POST` | `/api/sync` | 触发全量同步 |
| `GET` | `/api/sync` | 同步状态 |
| `POST` | `/api/sync/document` | 按 ID 同步单个文档 |
