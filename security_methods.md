# Feishu-ClaudeCode 安全权限鉴定方法

> 基于当前项目交互逻辑（飞书 WebSocket 事件 → Event Handler → Message Bridge → Claude Executor）提出的三种安全鉴权方案。

---

## 当前项目安全现状分析

### 现有机制
| 机制 | 实现位置 | 说明 |
|------|----------|------|
| 飞书 SDK 认证 | `index.ts` | `appId` + `appSecret` 建立 WebSocket 连接 |
| 用户/群组白名单 | `event-handler.ts` | `AUTHORIZED_USER_IDS` / `AUTHORIZED_CHAT_IDS` 静态白名单 |
| 群聊 @mention 过滤 | `event-handler.ts` | 群聊中仅响应 @机器人 的消息 |
| 预算限制 | `executor.ts` | `maxBudgetUsd` 单次请求费用上限 |
| 工具限制 | `executor.ts` | `allowedTools` 限制 Claude 可用工具 |

### 核心风险点
1. **`bypassPermissions` 模式**：Claude 可直接执行文件读写和 Shell 命令，无需用户确认
2. **白名单为静态配置**：无法动态管理权限，修改需重启服务
3. **无操作审计**：缺乏对敏感操作（如 Bash 命令、文件修改）的审计日志
4. **会话粒度粗**：以 `chatId` 为单位，同一群内所有授权用户共享相同权限级别

---

## 方法一：基于 RBAC 的多级角色权限控制

### 核心思想
引入 **角色分级（Role-Based Access Control）**，不同角色对应不同的 Claude 工具集和操作范围，从根源限制不同用户的能力边界。

### 架构设计

```
用户消息 → Event Handler → [角色识别] → [权限映射] → Message Bridge → Claude Executor
                              │                │
                              ▼                ▼
                        角色配置表        工具白名单动态注入
```

### 角色定义

| 角色 | 工具权限 | 目录权限 | 预算限制 | 适用场景 |
|------|----------|----------|----------|----------|
| `viewer` | `Read`, `Glob`, `Grep` | 只读 | $0.1/请求 | 代码浏览、问题查询 |
| `developer` | `Read`, `Edit`, `Write`, `Glob`, `Grep` | 指定项目目录 | $0.5/请求 | 日常开发 |
| `admin` | 全部工具（含 `Bash`） | 无限制 | $1.0/请求 | 系统管理、DevOps |

### 实现方案

#### 1. 新增角色配置文件 `roles.json`

```json
{
  "roles": {
    "viewer": {
      "allowedTools": ["Read", "Glob", "Grep", "WebSearch"],
      "allowedDirectories": ["*"],
      "maxBudgetUsd": 0.1,
      "maxTurns": 10,
      "deniedCommands": ["rm", "dd", "mkfs", "chmod", "chown"]
    },
    "developer": {
      "allowedTools": ["Read", "Edit", "Write", "Glob", "Grep", "WebSearch", "WebFetch"],
      "allowedDirectories": ["/home/projects/*"],
      "maxBudgetUsd": 0.5,
      "maxTurns": 30,
      "deniedCommands": ["rm -rf /", "dd", "mkfs"]
    },
    "admin": {
      "allowedTools": ["Read", "Edit", "Write", "Glob", "Grep", "Bash", "WebSearch", "WebFetch"],
      "allowedDirectories": ["*"],
      "maxBudgetUsd": 1.0,
      "maxTurns": 50,
      "deniedCommands": []
    }
  },
  "userRoles": {
    "ou_admin_user_001": "admin",
    "ou_dev_user_002": "developer",
    "ou_viewer_user_003": "viewer"
  },
  "chatRoles": {
    "oc_dev_chat_001": "developer",
    "oc_readonly_chat_002": "viewer"
  },
  "defaultRole": "viewer"
}
```

#### 2. 改造 `event-handler.ts` 中的权限检查

```typescript
// 替换原有 isAuthorized() 函数
function resolveUserRole(config: Config, userId: string, chatId: string): Role | null {
  const roles = loadRoles(); // 从 roles.json 加载

  // 用户级角色优先
  if (roles.userRoles[userId]) {
    return roles.roles[roles.userRoles[userId]];
  }

  // 群组级角色次之
  if (roles.chatRoles[chatId]) {
    return roles.roles[roles.chatRoles[chatId]];
  }

  // 默认角色（如配置了白名单但用户不在内则拒绝）
  if (config.auth.authorizedUserIds.length > 0 || config.auth.authorizedChatIds.length > 0) {
    return null; // 未授权
  }

  return roles.roles[roles.defaultRole];
}
```

#### 3. 改造 `executor.ts` 动态注入权限

```typescript
// 根据角色动态设置 Claude 执行参数
async execute(prompt: string, cwd: string, sessionId: string, role: Role) {
  const queryOptions = {
    allowedTools: role.allowedTools,      // 角色对应的工具集
    maxTurns: role.maxTurns,              // 角色对应的轮次上限
    maxBudgetUsd: role.maxBudgetUsd,      // 角色对应的预算上限
    permissionMode: 'bypassPermissions',
    cwd: this.validateDirectory(cwd, role.allowedDirectories),
    abortController,
  };
  // ...
}
```

### 优势
- **最小权限原则**：每个角色只获得完成工作所需的最低权限
- **灵活可扩展**：可随时添加新角色，修改 JSON 无需改代码
- **向下兼容**：`defaultRole` 机制兼容现有无配置的使用方式

### 劣势
- 角色分配仍依赖配置文件，大规模部署需要管理平台
- 需要维护 `roles.json` 配置的一致性

---

## 方法二：飞书 OAuth 2.0 + JWT Token 动态鉴权

### 核心思想
利用飞书 Open API 的 **OAuth 2.0 用户认证** 获取用户真实身份信息（部门、职级等），签发 **JWT Token** 用于会话级的细粒度动态权限控制。

### 架构设计

```
┌─────────────┐     ┌──────────────────┐     ┌───────────────────┐
│  飞书用户    │────▶│  飞书 OAuth 2.0  │────▶│  JWT Token 签发    │
│  首次使用    │     │  用户身份验证     │     │  携带权限声明      │
└─────────────┘     └──────────────────┘     └────────┬──────────┘
                                                       │
                    ┌──────────────────────────────────▼──────────┐
                    │  后续每次消息                                │
                    │  Event Handler → Token 验证 → 权限提取       │
                    │  → Message Bridge → Claude Executor         │
                    └─────────────────────────────────────────────┘
```

### 实现方案

#### 1. 用户首次使用时触发 OAuth 认证

```typescript
// 新增 auth-service.ts
import jwt from 'jsonwebtoken';

const JWT_SECRET = process.env.JWT_SECRET || 'your-secure-secret-key';
const TOKEN_EXPIRY = '24h';

interface UserClaims {
  userId: string;           // 飞书 open_id
  employeeId: string;       // 工号
  department: string;       // 部门
  role: 'viewer' | 'developer' | 'admin';
  allowedTools: string[];
  allowedDirectories: string[];
  maxBudgetUsd: number;
}

// 通过飞书 API 获取用户详细信息
async function getUserInfo(client: lark.Client, userId: string): Promise<FeishuUserInfo> {
  const resp = await client.contact.user.get({
    path: { user_id: userId },
    params: { user_id_type: 'open_id' },
  });
  return resp.data.user;
}

// 根据部门/职级映射角色
function mapUserToRole(userInfo: FeishuUserInfo): UserClaims {
  const departmentRules = loadDepartmentRules(); // 从配置加载部门-角色映射

  // 根据部门自动分配角色
  for (const rule of departmentRules) {
    if (userInfo.department_ids?.includes(rule.departmentId)) {
      return {
        userId: userInfo.open_id,
        employeeId: userInfo.employee_no,
        department: userInfo.department_ids[0],
        role: rule.role,
        allowedTools: rule.allowedTools,
        allowedDirectories: rule.allowedDirectories,
        maxBudgetUsd: rule.maxBudgetUsd,
      };
    }
  }

  // 默认只读权限
  return { ...defaultViewerClaims, userId: userInfo.open_id };
}

// 签发 JWT Token
function issueToken(claims: UserClaims): string {
  return jwt.sign(claims, JWT_SECRET, { expiresIn: TOKEN_EXPIRY });
}

// 验证并解析 Token
function verifyToken(token: string): UserClaims | null {
  try {
    return jwt.verify(token, JWT_SECRET) as UserClaims;
  } catch (e) {
    return null; // Token 过期或无效
  }
}
```

#### 2. 在 Event Handler 中集成 Token 验证

```typescript
// event-handler.ts 改造
async function handleIncomingMessage(data: MessageEvent) {
  const userId = data.sender.sender_id.open_id;
  const chatId = data.message.chat_id;

  // 1. 检查是否已有有效 Token
  let claims = tokenCache.get(userId);

  if (!claims) {
    // 2. 首次使用 / Token 过期：获取用户信息并签发
    const userInfo = await getUserInfo(feishuClient, userId);
    claims = mapUserToRole(userInfo);
    const token = issueToken(claims);
    tokenCache.set(userId, claims, TOKEN_EXPIRY);

    logger.info({ userId, role: claims.role }, 'New token issued');
  }

  // 3. 携带权限声明传递给下游
  onMessage({
    messageId, chatId, chatType, userId,
    text, imageKey,
    permissions: claims,  // 新增权限字段
  });
}
```

#### 3. 部门-角色映射配置

```json
{
  "departmentRules": [
    {
      "departmentId": "od_engineering",
      "departmentName": "工程部",
      "role": "developer",
      "allowedTools": ["Read", "Edit", "Write", "Glob", "Grep"],
      "allowedDirectories": ["/home/projects/*"],
      "maxBudgetUsd": 0.5
    },
    {
      "departmentId": "od_devops",
      "departmentName": "运维部",
      "role": "admin",
      "allowedTools": ["Read", "Edit", "Write", "Glob", "Grep", "Bash"],
      "allowedDirectories": ["*"],
      "maxBudgetUsd": 1.0
    },
    {
      "departmentId": "od_product",
      "departmentName": "产品部",
      "role": "viewer",
      "allowedTools": ["Read", "Glob", "Grep"],
      "allowedDirectories": ["*"],
      "maxBudgetUsd": 0.1
    }
  ]
}
```

### 优势
- **零配置用户管理**：权限随飞书组织架构自动同步，员工调岗/离职自动生效
- **Token 缓存高效**：无需每次请求都调用飞书 API，JWT 自包含权限声明
- **细粒度控制**：可基于部门、职级、工号等多维度映射权限
- **时效性强**：Token 过期自动重新获取用户信息，权限动态更新

### 劣势
- 需要飞书应用具有 `contact:user.base:readonly` 权限（读取用户部门信息）
- JWT Secret 需要安全存储（建议使用环境变量或密钥管理服务）
- 首次请求有额外延迟（~200ms 获取用户信息）

---

## 方法三：操作审计 + 敏感命令实时拦截网关

### 核心思想
在 Claude 执行层与实际操作之间插入一个 **安全审计网关（Security Gateway）**，对所有 Claude 输出的操作进行 **实时审计、风险评估和敏感命令拦截**，实现事中防护和事后追溯。

### 架构设计

```
┌────────────────────────────────────────────────────────────────────┐
│                     安全审计网关 (Security Gateway)                 │
│                                                                    │
│  Claude Stream  ──▶  [操作解析器]  ──▶  [风险评估引擎]             │
│                          │                    │                    │
│                          ▼                    ▼                    │
│                     [审计日志]          [拦截/放行决策]             │
│                          │                    │                    │
│                          ▼                    ▼                    │
│                     [持久化存储]        [告警通知]                  │
│                                         (飞书群/管理员私聊)        │
└────────────────────────────────────────────────────────────────────┘
```

### 实现方案

#### 1. 新增安全审计网关 `security-gateway.ts`

```typescript
// src/security/security-gateway.ts

interface AuditRecord {
  timestamp: string;
  userId: string;
  chatId: string;
  sessionId: string;
  action: 'tool_use' | 'text_response' | 'command';
  toolName?: string;
  toolInput?: Record<string, any>;
  riskLevel: 'low' | 'medium' | 'high' | 'critical';
  decision: 'allow' | 'block' | 'alert';
  reason?: string;
}

// 危险模式定义
const DANGEROUS_PATTERNS = {
  critical: [
    /rm\s+(-rf?|--recursive)\s+\//,      // rm -rf /
    /dd\s+if=.*of=\/dev\//,               // dd 写设备
    /mkfs\./,                              // 格式化文件系统
    />(\/etc\/passwd|\/etc\/shadow)/,      // 覆写系统文件
    /chmod\s+777\s+\//,                    // 全局权限修改
    /curl.*\|\s*(bash|sh)/,               // 远程代码执行
    /wget.*&&.*chmod.*\+x/,              // 下载并执行
  ],
  high: [
    /rm\s+-rf?\s+/,                        // 删除操作
    /DROP\s+(TABLE|DATABASE)/i,            // 数据库删除
    /TRUNCATE\s+TABLE/i,                   // 数据库清空
    /git\s+push\s+.*--force/,             // 强制推送
    /git\s+reset\s+--hard/,               // 硬重置
    /npm\s+publish/,                       // 发布包
    /docker\s+rm/,                         // 删除容器
    /\.env|password|secret|token|key/i,    // 敏感文件访问
  ],
  medium: [
    /git\s+(commit|push|merge)/,           // Git 写操作
    /npm\s+(install|update)/,              // 包管理
    /pip\s+install/,                       // Python 包安装
    /chmod|chown/,                         // 权限修改
    /sudo\s+/,                             // 提权操作
  ],
};

class SecurityGateway {
  private auditLog: AuditRecord[] = [];
  private alertCallback: (record: AuditRecord) => Promise<void>;

  constructor(alertCallback: (record: AuditRecord) => Promise<void>) {
    this.alertCallback = alertCallback;
  }

  // 评估工具调用的风险等级
  assessRisk(toolName: string, toolInput: Record<string, any>): AuditRecord['riskLevel'] {
    if (toolName === 'Bash') {
      const command = toolInput.command || '';
      for (const pattern of DANGEROUS_PATTERNS.critical) {
        if (pattern.test(command)) return 'critical';
      }
      for (const pattern of DANGEROUS_PATTERNS.high) {
        if (pattern.test(command)) return 'high';
      }
      for (const pattern of DANGEROUS_PATTERNS.medium) {
        if (pattern.test(command)) return 'medium';
      }
    }

    if (toolName === 'Write' || toolName === 'Edit') {
      const filePath = toolInput.file_path || '';
      if (/\.(env|pem|key|secret|credentials)/.test(filePath)) return 'high';
      if (/\/etc\/|\/sys\/|\/proc\//.test(filePath)) return 'critical';
    }

    return 'low';
  }

  // 拦截决策
  async evaluate(
    userId: string, chatId: string, sessionId: string,
    toolName: string, toolInput: Record<string, any>
  ): Promise<{ allowed: boolean; reason?: string }> {
    const riskLevel = this.assessRisk(toolName, toolInput);

    const record: AuditRecord = {
      timestamp: new Date().toISOString(),
      userId, chatId, sessionId,
      action: 'tool_use',
      toolName, toolInput,
      riskLevel,
      decision: 'allow',
    };

    // 拦截策略
    switch (riskLevel) {
      case 'critical':
        record.decision = 'block';
        record.reason = `Critical risk operation blocked: ${toolName}`;
        this.auditLog.push(record);
        await this.alertCallback(record); // 立即告警
        return { allowed: false, reason: record.reason };

      case 'high':
        record.decision = 'alert';
        this.auditLog.push(record);
        await this.alertCallback(record); // 告警但放行（可配置为拦截）
        return { allowed: true, reason: 'High risk - admin notified' };

      case 'medium':
        record.decision = 'allow';
        this.auditLog.push(record);
        return { allowed: true };

      default:
        record.decision = 'allow';
        this.auditLog.push(record);
        return { allowed: true };
    }
  }

  // 导出审计日志
  getAuditLog(filters?: { userId?: string; riskLevel?: string }): AuditRecord[] {
    return this.auditLog.filter(r => {
      if (filters?.userId && r.userId !== filters.userId) return false;
      if (filters?.riskLevel && r.riskLevel !== filters.riskLevel) return false;
      return true;
    });
  }
}
```

#### 2. 在 `stream-processor.ts` 中集成审计网关

```typescript
// 改造 stream-processor.ts 中的工具调用处理
async processToolUse(message: AssistantMessage, gateway: SecurityGateway, context: MessageContext) {
  for (const block of message.content) {
    if (block.type === 'tool_use') {
      // 在工具执行前进行安全评估
      const evaluation = await gateway.evaluate(
        context.userId,
        context.chatId,
        context.sessionId,
        block.name,
        block.input,
      );

      if (!evaluation.allowed) {
        // 拦截操作，向用户发送提示卡片
        await messageSender.sendCard(
          context.chatId,
          buildSecurityBlockCard(block.name, evaluation.reason)
        );
        // 中止当前执行
        context.abortController.abort();
        return;
      }
    }
  }
}
```

#### 3. 实时告警通知

```typescript
// 告警回调 - 发送飞书消息给管理员
async function alertAdmin(record: AuditRecord): Promise<void> {
  const adminChatId = process.env.SECURITY_ALERT_CHAT_ID;
  if (!adminChatId) return;

  const alertCard = {
    header: {
      title: { content: `⚠️ 安全告警 [${record.riskLevel.toUpperCase()}]`, tag: 'plain_text' },
      template: record.riskLevel === 'critical' ? 'red' : 'orange',
    },
    elements: [
      { tag: 'div', text: { content: `**用户:** ${record.userId}`, tag: 'lark_md' } },
      { tag: 'div', text: { content: `**群组:** ${record.chatId}`, tag: 'lark_md' } },
      { tag: 'div', text: { content: `**操作:** ${record.toolName}`, tag: 'lark_md' } },
      { tag: 'div', text: { content: `**内容:** \`${JSON.stringify(record.toolInput).slice(0, 200)}\``, tag: 'lark_md' } },
      { tag: 'div', text: { content: `**决策:** ${record.decision}`, tag: 'lark_md' } },
      { tag: 'div', text: { content: `**时间:** ${record.timestamp}`, tag: 'lark_md' } },
    ],
  };

  await messageSender.sendCard(adminChatId, alertCard);
}
```

#### 4. 新增 `/audit` 管理命令

```typescript
// 在 message-bridge.ts 中添加 /audit 命令
case '/audit': {
  const logs = securityGateway.getAuditLog({ userId });
  const summary = {
    total: logs.length,
    byRisk: {
      critical: logs.filter(l => l.riskLevel === 'critical').length,
      high: logs.filter(l => l.riskLevel === 'high').length,
      medium: logs.filter(l => l.riskLevel === 'medium').length,
      low: logs.filter(l => l.riskLevel === 'low').length,
    },
    blocked: logs.filter(l => l.decision === 'block').length,
  };
  await messageSender.sendCard(chatId, buildAuditSummaryCard(summary));
  break;
}
```

### 优势
- **事中防护**：在操作执行前拦截危险命令，防止不可逆损害
- **全量审计**：所有操作可追溯，满足合规要求
- **实时告警**：管理员通过飞书即时收到高危操作通知
- **不影响正常使用**：低风险操作透明放行，用户无感知

### 劣势
- Stream 处理增加了中间层，可能有轻微延迟（<50ms）
- 正则匹配存在误判可能（需持续调优规则）
- 审计日志存储在内存中，需扩展为持久化方案（如 SQLite/文件）

---

## 三种方法对比

| 维度 | 方法一：RBAC 角色控制 | 方法二：OAuth + JWT | 方法三：审计网关 |
|------|----------------------|--------------------|--------------------|
| **防护阶段** | 事前（入口控制） | 事前（身份验证） | 事中（操作拦截） |
| **权限粒度** | 角色级 | 用户级（基于组织架构） | 操作级（单条命令） |
| **动态性** | 需手动修改配置 | 自动跟随飞书组织架构 | 实时风险评估 |
| **实现复杂度** | ⭐⭐ 低 | ⭐⭐⭐ 中 | ⭐⭐⭐⭐ 较高 |
| **对现有代码改动** | 小（新增配置 + 改造两个文件） | 中（新增服务 + 改造三个文件） | 中（新增网关 + 改造 Stream 处理） |
| **运维成本** | 低（JSON 配置） | 低（自动同步） | 中（需维护规则库） |
| **最佳适用场景** | 小团队，角色固定 | 中大型企业，组织架构完善 | 高安全要求，需合规审计 |

---

## 推荐组合方案

**最佳实践：三种方法组合使用，构建纵深防御体系**

```
Layer 1 (身份层)：方法二 - OAuth + JWT 动态鉴权
    ↓ 确认用户身份和基础权限
Layer 2 (权限层)：方法一 - RBAC 角色控制
    ↓ 根据角色限制可用工具和目录
Layer 3 (操作层)：方法三 - 审计网关 + 命令拦截
    ↓ 实时监控并拦截危险操作
```

### 分步实施建议

| 阶段 | 方法 | 优先级 | 预估工时 |
|------|------|--------|----------|
| **Phase 1** | 方法一 RBAC | 🔴 高 | 2-3 天 |
| **Phase 2** | 方法三 审计网关 | 🟡 中 | 3-5 天 |
| **Phase 3** | 方法二 OAuth + JWT | 🟢 可选 | 5-7 天 |

> Phase 1 可以快速落地基础权限控制；Phase 2 补充运行时安全防护；Phase 3 在企业规模扩大后实现自动化权限管理。

---

## 附录：环境变量扩展

```bash
# 方法一：RBAC
ROLES_CONFIG_PATH=./roles.json          # 角色配置文件路径

# 方法二：OAuth + JWT
JWT_SECRET=your-256-bit-secret          # JWT 签名密钥
TOKEN_EXPIRY=24h                        # Token 有效期
DEPARTMENT_RULES_PATH=./dept-rules.json # 部门-角色映射配置

# 方法三：审计网关
SECURITY_ALERT_CHAT_ID=oc_xxxxxx       # 安全告警群 chat_id
AUDIT_LOG_PATH=./logs/audit.jsonl       # 审计日志持久化路径
BLOCK_CRITICAL_COMMANDS=true            # 是否拦截 critical 级命令
BLOCK_HIGH_COMMANDS=false               # 是否拦截 high 级命令
```
