import { normalizeCodexReasoningEffort, type BotConfigBase, type CodexReasoningEffort } from '../config.js';
import type { Logger } from '../utils/logger.js';
import type { IncomingMessage } from '../types.js';
import type { IMessageSender } from './message-sender.interface.js';
import { resolveEngineName, SessionManager } from '../engines/index.js';
import type { EngineName } from '../engines/index.js';
import type { SessionSummary } from '../engines/claude/session-lister.js';
import { MemoryClient } from '../memory/memory-client.js';
import { AuditLogger } from '../utils/audit-logger.js';
import type { DocSync } from '../sync/doc-sync.js';
import { claudeModelCatalog, type ModelOption } from './model-catalog.js';

export class CommandHandler {
  private docSync: DocSync | null = null;

  constructor(
    private config: BotConfigBase,
    private logger: Logger,
    private sender: IMessageSender,
    private sessionManager: SessionManager,
    private memoryClient: MemoryClient,
    private audit: AuditLogger,
    private getActiveTask: (chatId: string) => { startTime: number } | undefined,
    private stopTask: (chatId: string) => void,
    /**
     * Drain the chat's queued-message buffer, returning the number of
     * messages discarded. Called from /stop so the user's "stop" intent
     * isn't immediately undone by the next queued message — without this
     * the bridge's processQueue would start the next one as soon as the
     * aborted task's finally block runs.
     */
    private clearQueue: (chatId: string) => number,
    /**
     * Release the persistent Claude process associated with this chat
     * (no-op if the persistent-executor feature flag is off or no
     * executor exists). Called on /reset so teammates and /goal state
     * tied to the old session are torn down with the conversation.
     */
    private releaseExecutor: (chatId: string, reason: string) => Promise<void>,
    /**
     * List the recent sessions for this chat's active engine and working directory
     * (newest first). Backs the direct `/resume <id>` form. Read-only.
     */
    private listSessions: (chatId: string) => SessionSummary[] | Promise<SessionSummary[]>,
    /**
     * Swap the chat into a previous engine session: re-point the sessionId,
     * reset usage counters, release the persistent executor so the next turn
     * resumes via `claude --resume`. Backs both `/resume` forms.
     */
    private applyResume: (chatId: string, sessionId: string) => Promise<void>,
    private handleAuto?: (msg: IncomingMessage, args: string) => Promise<void>,
  ) {}

  /** Set the doc sync service (optional, only available for Feishu bots). */
  setDocSync(docSync: DocSync): void {
    this.docSync = docSync;
  }

  /** Returns true if the message was handled as a command, false otherwise. */
  async handle(msg: IncomingMessage): Promise<boolean> {
    const { text } = msg;
    if (!text.startsWith('/')) return false;

    const { userId, chatId } = msg;
    const [cmd] = text.split(/\s+/);

    this.audit.log({ event: 'command', botName: this.config.name, chatId, userId, prompt: cmd });

    switch (cmd.toLowerCase()) {
      case '/?':
      case '/help':
        await this.sender.sendTextNotice(chatId, '📖 Help', [
          '**Bot Commands:**',
          '`/reset` or `/new` - Clear session and /goal, start fresh (working directory preserved)',
          '`/stop` - Abort current running task',
          '`/compact` - Compact the current session while idle (Codex / Claude Code PTY); messages queue',
          '`/append <text>` or `/steer <text>` - Steer the active task (Codex app-server / Claude Code PTY)',
          '`/status` - Show current session info',
          '`/model` - Show current engine/model; `/model list` - Available options',
          '`/model claude`, `/model kimi`, or `/model codex` - Switch engine (resets session)',
          '`/model <name>` - Set model for current engine',
          '`/effort low|medium|high|xhigh|max|ultra` - Set Codex reasoning effort for this chat',
          '`/resume` - List & switch to a previous Claude/Codex/Kimi session',
          '`/resume <id>` - Resume a session directly by id prefix',
          '`/memory` - Memory document commands',
          '`@Bot /group-reply mention|all|status` - Feishu group reply mode (scoped to this Agent and group)',
          '`/auto [goal]` - Let Shadow Mode send the next user message toward an optional goal (it stops itself when the goal is reached); your messages are inserted without stopping it; `/auto stop` stops it',
          '`/help` or `/?` - Show this help message',
          '',
          '**Agent Commands:**',
          '`/goal <description>` - Set a goal the agent keeps pursuing across turns (Claude native, Codex bridge-managed)',
          '`/goal status|clear` - Show or clear the current goal',
          '`/background <prompt>` - Run a task in the background while you continue chatting (Claude native, Codex bridge-managed)',
          '`/background list|logs <id>|stop <id>` - Manage Codex background tasks',
          '',
          '**Usage:**',
          'Send any text message to start a conversation with the configured agent engine.',
          'Each chat has an independent session with a fixed working directory.',
          '',
          '**Memory Commands:**',
          '`/memory list` - Show folder tree',
          '`/memory search <query>` - Search documents',
          '`/memory status` - Server health check',
          '',
          '**Sync Commands:**',
          '`/sync` - Sync MetaMemory to Feishu Wiki',
          '`/sync status` - Show sync status',
        ].join('\n'));
        return true;

      case '/new':
      case '/reset': {
        {
          const task = this.getActiveTask(chatId);
          const cleared = this.clearQueue(chatId);
          if (task) {
            this.audit.log({
              event: 'task_stopped',
              botName: this.config.name,
              chatId,
              userId,
              durationMs: Date.now() - task.startTime,
              meta: { reason: 'reset', clearedQueue: cleared },
            });
            this.stopTask(chatId);
          } else if (cleared > 0) {
            this.audit.log({
              event: 'queue_cleared',
              botName: this.config.name,
              chatId,
              userId,
              meta: { reason: 'reset', clearedQueue: cleared },
            });
          }
        }
        const hadGoal = !!this.sessionManager.getSession(chatId).activeGoal;
        this.sessionManager.resetSession(chatId);
        // Tear down the persistent Claude process for this chat (Stage 3b).
        // Otherwise the old long-lived executor would keep running with its
        // stale (now-cleared) sessionId mapping. No-op when persistent mode
        // is off. Best-effort — log but don't fail the /reset on shutdown errors.
        try {
          await this.releaseExecutor(chatId, 'reset-command');
        } catch (err) {
          this.logger.warn({ err, chatId }, 'Failed to release persistent executor on /reset');
        }
        await this.sender.sendTextNotice(
          chatId,
          '✅ Session Reset',
          hadGoal
            ? 'Conversation cleared. Working directory preserved; active /goal cleared.'
            : 'Conversation cleared. Working directory preserved.',
          'green',
        );
        return true;
      }

      case '/stop': {
        const task = this.getActiveTask(chatId);
        // Always drain the queue first — otherwise the running task's
        // finally block immediately picks the next queued message via
        // processQueue and the user's "stop" intent silently fails.
        const cleared = this.clearQueue(chatId);
        if (task) {
          this.audit.log({ event: 'task_stopped', botName: this.config.name, chatId, userId, durationMs: Date.now() - task.startTime, meta: { clearedQueue: cleared } });
          this.stopTask(chatId);
          const body = cleared > 0
            ? `Current task aborted. Discarded **${cleared}** queued message${cleared === 1 ? '' : 's'}.`
            : 'Current task has been aborted.';
          await this.sender.sendTextNotice(chatId, '🛑 Stopped', body, 'orange');
        } else if (cleared > 0) {
          // No running task but queued messages existed — clear them too.
          this.audit.log({ event: 'queue_cleared', botName: this.config.name, chatId, userId, meta: { clearedQueue: cleared } });
          await this.sender.sendTextNotice(
            chatId,
            '🛑 Queue Cleared',
            `No task was running. Discarded **${cleared}** queued message${cleared === 1 ? '' : 's'}.`,
            'orange',
          );
        } else {
          await this.sender.sendTextNotice(chatId, 'ℹ️ No Running Task', 'There is no task to stop.', 'blue');
        }
        return true;
      }

      case '/status': {
        const session = this.sessionManager.getSession(chatId);
        const isRunning = !!this.getActiveTask(chatId);
        const botEngine = resolveEngineName(this.config);
        const activeEngine = session.engine ?? botEngine;
        const defaultModel = this.defaultModelForEngine(activeEngine) || '_default_';
        const activeModel = session.model || defaultModel;
        await this.sender.sendTextNotice(chatId, '📊 Status', [
          `**User:** \`${userId}\``,
          `**Engine:** \`${activeEngine}\`${session.engine ? ' (session override)' : ''}`,
          `**Working Directory:** \`${session.workingDirectory}\``,
          `**Session:** ${session.sessionId ? `\`${session.sessionId.slice(0, 8)}...\`` : '_None_'}`,
          `**Model:** \`${activeModel}\`${session.model ? ' (session override)' : ''}`,
          `**Effort:** \`${session.reasoningEffort || this.config.codex?.reasoningEffort || 'codex default'}\`${session.reasoningEffort ? ' (session override)' : ''}`,
          `**Running:** ${isRunning ? 'Yes ⏳' : 'No'}`,
        ].join('\n'));
        return true;
      }

      case '/auto': {
        if (!this.handleAuto) {
          await this.sender.sendTextNotice(chatId, 'Shadow Mode', 'Shadow Mode is unavailable on this channel.', 'red');
          return true;
        }
        await this.handleAuto(msg, text.slice('/auto'.length).trim());
        return true;
      }

      case '/memory': {
        const args = text.slice('/memory'.length).trim();
        await this.handleMemoryCommand(chatId, args);
        return true;
      }

      case '/sync': {
        const args = text.slice('/sync'.length).trim();
        await this.handleSyncCommand(chatId, args);
        return true;
      }

      case '/model': {
        const args = text.slice('/model'.length).trim();
        await this.handleModelCommand(chatId, args);
        return true;
      }

      case '/effort': {
        const args = text.slice('/effort'.length).trim();
        await this.handleEffortCommand(chatId, args);
        return true;
      }

      case '/resume': {
        const arg = text.slice('/resume'.length).trim();
        await this.handleResumeCommand(msg, arg);
        return true;
      }

      default:
        // Unrecognized /xxx commands — not handled here, pass through to Claude
        return false;
    }
  }

  private async handleMemoryCommand(chatId: string, args: string): Promise<void> {
    const [subCmd, ...rest] = args.split(/\s+/);

    if (!subCmd) {
      await this.sender.sendTextNotice(
        chatId,
        '📝 Memory',
        'Usage:\n- `/memory list` — Show folder tree\n- `/memory search <query>` — Search documents\n- `/memory status` — Health check',
      );
      return;
    }

    try {
      switch (subCmd.toLowerCase()) {
        case 'list': {
          const tree = await this.memoryClient.listFolderTree();
          const formatted = this.memoryClient.formatFolderTree(tree);
          await this.sender.sendTextNotice(chatId, '📂 Memory Folders', formatted);
          break;
        }
        case 'search': {
          const query = rest.join(' ').trim();
          if (!query) {
            await this.sender.sendTextNotice(chatId, '📝 Memory', 'Usage: `/memory search <query>`');
            return;
          }
          const results = await this.memoryClient.search(query);
          const formatted = this.memoryClient.formatSearchResults(results);
          await this.sender.sendTextNotice(chatId, `🔍 Search: ${query}`, formatted);
          break;
        }
        case 'status': {
          const health = await this.memoryClient.health();
          await this.sender.sendTextNotice(
            chatId,
            '📝 Memory Status',
            `Status: ${health.status}\nDocuments: ${health.document_count}\nFolders: ${health.folder_count}`,
            'green',
          );
          break;
        }
        default:
          await this.sender.sendTextNotice(chatId, '📝 Memory', `Unknown sub-command: \`${subCmd}\`\nUse \`/memory\` for help.`, 'orange');
      }
    } catch (err: any) {
      this.logger.error({ err, chatId }, 'Memory command error');
      await this.sender.sendTextNotice(chatId, '❌ Memory Error', `Failed to connect to memory server: ${err.message}`, 'red');
    }
  }

  private async handleSyncCommand(chatId: string, args: string): Promise<void> {
    if (!this.docSync) {
      await this.sender.sendTextNotice(chatId, '❌ Sync Unavailable', 'Wiki sync is not configured for this bot.', 'red');
      return;
    }

    const [subCmd] = args.split(/\s+/);

    if (!subCmd) {
      // Default: trigger full sync
      if (this.docSync.isSyncing()) {
        await this.sender.sendTextNotice(chatId, '⏳ Sync In Progress', 'A sync is already running. Please wait.', 'orange');
        return;
      }

      await this.sender.sendTextNotice(chatId, '🔄 Sync Started', 'Syncing MetaMemory documents to Feishu Wiki...', 'blue');

      try {
        const result = await this.docSync.syncAll();
        const lines = [
          `**Created:** ${result.created}`,
          `**Updated:** ${result.updated}`,
          `**Skipped:** ${result.skipped} (unchanged)`,
          `**Deleted:** ${result.deleted}`,
          `**Duration:** ${(result.durationMs / 1000).toFixed(1)}s`,
        ];
        if (result.errors.length > 0) {
          lines.push('', `**Errors (${result.errors.length}):**`);
          for (const err of result.errors.slice(0, 5)) {
            lines.push(`- ${err}`);
          }
          if (result.errors.length > 5) {
            lines.push(`- ... and ${result.errors.length - 5} more`);
          }
        }
        const color = result.errors.length > 0 ? 'orange' : 'green';
        await this.sender.sendTextNotice(chatId, '✅ Sync Complete', lines.join('\n'), color);
      } catch (err: any) {
        this.logger.error({ err, chatId }, 'Sync command error');
        await this.sender.sendTextNotice(chatId, '❌ Sync Failed', err.message, 'red');
      }
      return;
    }

    switch (subCmd.toLowerCase()) {
      case 'status': {
        const stats = this.docSync.getStats();
        const spaceId = stats.wikiSpaceId || 'Not configured';
        await this.sender.sendTextNotice(chatId, '📊 Sync Status', [
          `**Wiki Space:** \`${spaceId}\``,
          `**Synced Documents:** ${stats.documentCount}`,
          `**Synced Folders:** ${stats.folderCount}`,
          `**Currently Syncing:** ${this.docSync.isSyncing() ? 'Yes' : 'No'}`,
        ].join('\n'));
        break;
      }
      default:
        await this.sender.sendTextNotice(chatId, '📝 Sync', 'Usage:\n- `/sync` — Sync all documents to Feishu Wiki\n- `/sync status` — Show sync status', 'blue');
    }
  }

  private async handleModelCommand(chatId: string, args: string): Promise<void> {
    const session = this.sessionManager.getSession(chatId);
    const botEngine = resolveEngineName(this.config);
    const activeEngine = session.engine ?? botEngine;
    const botDefault = this.defaultModelForEngine(activeEngine);
    if (!args) {
      const active = session.model || botDefault || '_default_';
      const exampleModels = this.exampleModelsForEngine(activeEngine);
      const lines = [
        `**Engine:** \`${activeEngine}\`${session.engine ? ' (session override)' : ''}`,
        `**Active:** \`${active}\`${session.model ? ' (session override)' : ''}`,
        `**Bot default:** \`${botDefault || '_unset_'}\``,
        '',
        'Usage:',
        '- `/model list` — Show available engines + models',
        '- `/model claude`, `/model kimi`, or `/model codex` — Switch engine (resets session)',
        `- \`/model <name>\` — Set session model (e.g. ${exampleModels})`,
        '- `/model reset` — Clear overrides, use bot defaults',
      ];
      await this.sender.sendTextNotice(chatId, '🤖 Model', lines.join('\n'));
      return;
    }

    const normalized = args.toLowerCase();
    if (normalized !== 'list' && normalized !== 'ls' && this.getActiveTask(chatId)) {
      await this.sender.sendTextNotice(
        chatId,
        '⏳ Task In Progress',
        'A task is active. Use `/stop` first, then change the model.',
        'orange',
      );
      return;
    }
    if (activeEngine === 'codex' && normalized === 'fast') {
      this.sessionManager.setServiceTier(chatId, 'fast');
      await this.sender.sendTextNotice(
        chatId,
        '✅ Fast',
        '当前模型保持不变，下一轮启用 Fast。关闭：`/model <model>|<effort>|default`。',
        'green',
      );
      return;
    }
    if (activeEngine === 'kimi' && normalized === 'fast') {
      const fastModel = kimiFastModelId(session.model || botDefault);
      this.sessionManager.setSessionModel(chatId, fastModel, activeEngine);
      await this.sender.sendTextNotice(
        chatId,
        '✅ Fast',
        `Session model set to \`${fastModel}\`（高速版，下一轮生效）。关闭：\`/model <model>\` 或 \`/model reset\`。`,
        'green',
      );
      return;
    }
    if (isEngineName(normalized)) {
      if (activeEngine === normalized) {
        await this.sender.sendTextNotice(
          chatId,
          'ℹ️ Already using ' + normalized,
          `This chat is already on the \`${normalized}\` engine.`,
          'blue',
        );
        return;
      }
      this.sessionManager.setSessionEngine(chatId, normalized);
      try {
        await this.releaseExecutor(chatId, 'engine-switch');
      } catch (err) {
        this.logger.warn({ err, chatId }, 'Failed to release executor on engine switch');
      }
      await this.sender.sendTextNotice(
        chatId,
        `✅ Engine switched to ${normalized}`,
        [
          `Next message will run on the **${normalized}** engine.`,
          '',
          '_Session ID and model override cleared — a fresh conversation starts on the next turn._',
          this.authTipForEngine(normalized),
        ].join('\n'),
        'green',
      );
      return;
    }
    if (normalized === 'list' || normalized === 'ls') {
      const active = session.model || botDefault;
      const claudeModels: ModelOption[] = claudeModelCatalog({
        override: this.config.availableModels,
        env: process.env,
      });
      const kimiModels = [
        { id: 'fast', label: 'Fast', note: 'Switch to the highspeed variant of the current model' },
        { id: 'kimi-code/k3.1-pro', label: 'Kimi K3.1 Pro', note: 'Latest flagship · 1M context · thinking required' },
        {
          id: 'kimi-code/k3.1-pro-highspeed',
          label: 'Kimi K3.1 Pro Highspeed',
          note: '~2-3× output speed · 1M context',
        },
        { id: 'kimi-code/k3', label: 'Kimi K3', note: '1M context' },
        { id: 'kimi-code/k3-256k', label: 'Kimi K3 256K', note: '256K context · lower quota usage' },
        { id: 'kimi-code/kimi-for-coding', label: 'Kimi for Coding', note: 'K2.8 Preview · compatible coding model' },
        {
          id: 'kimi-code/kimi-for-coding-highspeed',
          label: 'Kimi for Coding Highspeed',
          note: 'Low-latency coding model when enabled for your account',
        },
      ];
      const codexModels = [
        { id: 'fast', label: 'Fast', note: 'Current Codex model with service tier fast' },
        { id: 'gpt-6.1-sol', label: 'GPT 6.1 Sol', note: 'Flagship capability model' },
        {
          id: 'gpt-6-astra',
          label: 'GPT 6 Astra',
          note: 'Flagship Codex model · persistent context across windows (Codex ≥0.153)',
        },
        {
          id: 'gpt-6-astra-pro',
          label: 'GPT 6 Astra Pro',
          note: 'Higher-capability Astra tier for Pro/Business/Enterprise accounts',
        },
        { id: 'gpt-5.6', label: 'GPT 5.6', note: 'General GPT-5.6 Codex model' },
        { id: 'gpt-5.6-sol', label: 'GPT 5.6 Sol', note: 'Legacy flagship capability model' },
        { id: 'gpt-5.6-terra', label: 'GPT 5.6 Terra', note: 'Stronger speed/cost balance for Codex workers' },
        { id: 'gpt-5.6-luna', label: 'GPT 5.6 Luna', note: 'Efficient high-volume Codex workloads' },
        { id: 'gpt-5.5', label: 'GPT 5.5', note: 'Legacy Codex model' },
        {
          id: 'gpt-5.5-codex',
          label: 'GPT 5.5 Codex',
          note: 'Legacy Codex coding model, when available in your Codex account',
        },
        { id: 'gpt-5.2-codex', label: 'GPT-5.2 Codex', note: 'Legacy Codex coding model' },
      ];
      const models =
        activeEngine === 'kimi'
          ? kimiModels
          : activeEngine === 'codex'
            ? codexModels
            : claudeModels;
      const header =
        activeEngine === 'kimi'
          ? '**Available Kimi models:**'
          : activeEngine === 'codex'
            ? '**Common Codex models:**'
            : '**Available Claude models (from this endpoint\'s configured aliases):**';
      const lines = [
        `**Current engine:** \`${activeEngine}\`${session.engine ? ' (session override)' : ''}`,
        '',
        '**Engines:** `/model claude`, `/model kimi`, or `/model codex` to switch.',
        '',
        header,
        '',
      ];
      for (const m of models) {
        const marker = m.id === active || (active && m.id.endsWith(`/${active}`)) ? ' ✅' : '';
        lines.push(`- \`${m.id}\`${m.note ? ` — ${m.label} · ${m.note}` : ` — ${m.label}`}${marker}`);
      }
      lines.push('');
      if (activeEngine === 'claude') {
        lines.push('_Tip: Context limits and model ID suffixes depend on your endpoint._');
      } else if (activeEngine === 'codex') {
        lines.push('_Tip: leave unset to use the Codex CLI default from `~/.codex/config.toml`._');
      } else {
        lines.push(
          '_Tip: leave unset to use the Kimi Code Server default (recommended for subscription users)._',
        );
      }
      lines.push('Use `/model <name>` to set the model for the current engine.');
      if (activeEngine === 'codex')
        lines.push(
          '组合：`/model gpt-6-astra|medium|fast`\nEffort: low / medium / high / xhigh / max / ultra\nSpeed: default / fast / flex\n空 effort 恢复默认；空 speed 关闭 Fast。例：`/model gpt-6-astra|high|`',
        );
      if (activeEngine === 'kimi')
        lines.push(
          'Fast：`/model fast` 切换到当前模型的高速版（k3.1-pro → k3.1-pro-highspeed，kimi-for-coding → kimi-for-coding-highspeed）。恢复：`/model <model>` 或 `/model reset`。',
        );
      await this.sender.sendTextNotice(chatId, '🤖 Available Models', lines.join('\n'));
      return;
    }
    if (normalized === 'reset' || normalized === 'clear' || normalized === 'default') {
      this.sessionManager.setSessionModel(chatId, undefined);
      this.sessionManager.setSessionEngine(chatId, undefined);
      this.sessionManager.setReasoningEffort(chatId, undefined);
      this.sessionManager.setServiceTier(chatId, this.config.codex?.serviceTier ?? 'default');
      const fallback = this.defaultModelForEngine(botEngine) || '_default_';
      await this.sender.sendTextNotice(
        chatId,
        '✅ Overrides Cleared',
        `Session engine and model overrides cleared. Using bot defaults: engine \`${botEngine}\`, model \`${fallback}\`.`,
        'green',
      );
      return;
    }
    const parts = args.split('|').map((part) => part.trim());
    const newModelRaw = parts[0].split(/\s+/)[0];
    const effort = parts[1] ? normalizeCodexReasoningEffort(parts[1]) : undefined;
    const tier = parts[2] === 'fast' || parts[2] === 'flex' || parts[2] === 'default' ? parts[2] : undefined;
    if (
      parts.length > 1 &&
      (activeEngine !== 'codex' || parts.length > 3 || !newModelRaw || (parts[1] && !effort) || (parts[2] && !tier))
    ) {
      await this.sender.sendTextNotice(
        chatId,
        '❌ Invalid model options',
        'Use `/model <model>|<effort>|<speed>`.',
        'red',
      );
      return;
    }
    const newModel = newModelRaw;
    this.sessionManager.setSessionModel(chatId, newModel, activeEngine);
    if (parts.length > 1) this.sessionManager.setReasoningEffort(chatId, effort);
    if (parts.length > 2) this.sessionManager.setServiceTier(chatId, tier ?? 'default');
    await this.sender.sendTextNotice(
      chatId,
      '✅ Model Set',
      `Session model set to \`${newModel}\` on engine \`${activeEngine}\`${effort ? `, effort \`${effort}\`` : ''}${tier ? `, speed \`${tier}\`` : ''}. It will take effect on the next message.`,
      'green',
    );
  }

  private async handleEffortCommand(chatId: string, args: string): Promise<void> {
    const session = this.sessionManager.getSession(chatId);
    const activeEngine = session.engine ?? resolveEngineName(this.config);
    const normalized = normalizeCodexEffort(args);

    if (!args) {
      const current = session.reasoningEffort || this.config.codex?.reasoningEffort || '_codex default_';
      await this.sender.sendTextNotice(
        chatId,
        '🧠 Effort',
        [
          `**Engine:** \`${activeEngine}\``,
          `**Current:** \`${current}\`${session.reasoningEffort ? ' (session override)' : ''}`,
          '',
          'Usage:',
          '- `/effort low` — fastest',
          '- `/effort medium` — balanced',
          '- `/effort high` — deeper reasoning',
          '- `/effort xhigh` — extra-high reasoning',
          '- `/effort max` — maximum reasoning depth',
          '- `/effort ultra` — maximum reasoning with automatic task delegation',
          '- `/effort reset` — clear session override',
        ].join('\n'),
      );
      return;
    }

    if (activeEngine !== 'codex') {
      await this.sender.sendTextNotice(
        chatId,
        'ℹ️ Codex effort only',
        `This chat is on \`${activeEngine}\`. Switch with \`/model codex\`, then use \`/effort high\`, \`/effort max\`, or \`/effort ultra\`.`,
        'blue',
      );
      return;
    }

    if (normalized === 'reset') {
      this.sessionManager.setReasoningEffort(chatId, undefined);
      await this.sender.sendTextNotice(
        chatId,
        '✅ Effort Reset',
        `Codex reasoning effort override cleared. Using \`${this.config.codex?.reasoningEffort || 'codex default'}\`.`,
        'green',
      );
      return;
    }

    if (!normalized) {
      await this.sender.sendTextNotice(
        chatId,
        '❌ Invalid Effort',
        'Use one of: `low`, `medium`, `high`, `xhigh`, `max`, `ultra`.',
        'red',
      );
      return;
    }

    this.sessionManager.setReasoningEffort(chatId, normalized);
    await this.sender.sendTextNotice(
      chatId,
      '✅ Effort Set',
      `Codex reasoning effort set to \`${normalized}\`. It will take effect on the next message.`,
      'green',
    );
  }

  /**
   * `/resume <id-or-prefix>` — switch the chat directly into a previous engine
   * session by (a prefix of) its session id. Bare `/resume` is intercepted by
   * the bridge picker before reaching here; we keep a usage notice as a
   * defensive fallback.
   *
   * Supported for Claude, Codex, and Kimi, and refused
   * while a turn is running (the swap would race the in-flight executor).
   */
  private async handleResumeCommand(msg: IncomingMessage, arg: string): Promise<void> {
    const { chatId } = msg;
    const session = this.sessionManager.getSession(chatId);
    const activeEngine = session.engine ?? resolveEngineName(this.config);
    if (activeEngine !== 'claude' && activeEngine !== 'codex' && activeEngine !== 'kimi') {
      await this.sender.sendTextNotice(
        chatId,
        '❌ /resume Unsupported',
        `This chat is on the \`${activeEngine}\` engine. Session resume is available for Claude, Codex, and Kimi.`,
        'red',
      );
      return;
    }

    if (this.getActiveTask(chatId)) {
      await this.sender.sendTextNotice(
        chatId,
        '⏳ Task In Progress',
        'A task is running. Use `/stop` first, then `/resume`.',
        'orange',
      );
      return;
    }

    if (!arg) {
      await this.sender.sendTextNotice(
        chatId,
        '📝 Resume',
        'Usage: `/resume <session-id-prefix>`, or send a bare `/resume` to pick from a list.',
        'blue',
      );
      return;
    }

    const sessions = await this.listSessions(chatId);
    if (sessions.length === 0) {
      await this.sender.sendTextNotice(
        chatId,
        'ℹ️ No Previous Sessions',
        `No earlier ${activeEngine} sessions were found for this chat's working directory.`,
        'blue',
      );
      return;
    }

    const exact = sessions.find((s) => s.sessionId === arg);
    const matches = exact ? [exact] : sessions.filter((s) => s.sessionId.startsWith(arg));

    if (matches.length === 0) {
      await this.sender.sendTextNotice(
        chatId,
        '❌ No Match',
        `No session id starts with \`${arg}\`. Send a bare \`/resume\` to see the list.`,
        'red',
      );
      return;
    }
    if (matches.length > 1) {
      const ids = matches.slice(0, 8).map((s) => `\`${s.sessionId.slice(0, 8)}\``).join(', ');
      await this.sender.sendTextNotice(
        chatId,
        '⚠️ Ambiguous',
        `\`${arg}\` matches ${matches.length} sessions: ${ids}. Add more characters.`,
        'orange',
      );
      return;
    }

    const target = matches[0];
    await this.applyResume(chatId, target.sessionId);
    await this.sender.sendTextNotice(
      chatId,
      '✅ Session Resumed',
      `Resumed \`${target.sessionId.slice(0, 8)}\`. Your next message continues that conversation.`,
      'green',
    );
  }

  private defaultModelForEngine(engine: EngineName): string | undefined {
    switch (engine) {
      case 'claude':
        return this.config.claude.model;
      case 'kimi':
        return this.config.kimi?.model;
      case 'codex':
        return this.config.codex?.model || this.config.codex?.displayModel;
    }
  }

  private exampleModelsForEngine(engine: EngineName): string {
    switch (engine) {
      case 'claude': {
        const ids = claudeModelCatalog({ override: this.config.availableModels, env: process.env })
          .slice(0, 3)
          .map((model) => `\`${model.id}\``);
        return ids.length > 0 ? ids.join(', ') : '`/model list`';
      }
      case 'kimi':
        return '`kimi-code/k3.1-pro`, `kimi-code/k3.1-pro-highspeed`, `kimi-code/k3`, `fast`';
      case 'codex':
        return '`gpt-6.1-sol`, `gpt-6-astra`, `gpt-6-astra-pro`, `gpt-5.6`, `gpt-5.6-sol`, `gpt-5.6-terra`, `gpt-5.6-luna`';
    }
  }

  private authTipForEngine(engine: EngineName): string {
    switch (engine) {
      case 'claude':
        return '_Make sure Claude Code is authenticated (`claude login`)._';
      case 'kimi':
        return '_Make sure `kimi login` has been completed on this host._';
      case 'codex':
        return '_Make sure Codex CLI is authenticated (`codex login`) or configured with an API key._';
    }
  }
}

function isEngineName(value: string): value is EngineName {
  return value === 'claude' || value === 'kimi' || value === 'codex';
}

const KIMI_HIGHSPEED_VARIANTS: Record<string, string> = {
  'kimi-code/k3.1-pro': 'kimi-code/k3.1-pro-highspeed',
  'kimi-code/kimi-for-coding': 'kimi-code/kimi-for-coding-highspeed',
};

function kimiFastModelId(current?: string): string {
  const normalized = current?.trim();
  if (normalized) {
    if (normalized.endsWith('-highspeed')) return normalized;
    const variant = KIMI_HIGHSPEED_VARIANTS[normalized] ?? KIMI_HIGHSPEED_VARIANTS[`kimi-code/${normalized}`];
    if (variant) return variant;
  }
  return 'kimi-code/k3.1-pro-highspeed';
}

function normalizeCodexEffort(value: string): CodexReasoningEffort | 'reset' | undefined {
  const normalized = value.trim().toLowerCase();
  if (normalized === 'reset' || normalized === 'clear' || normalized === 'default') return 'reset';
  return normalizeCodexReasoningEffort(normalized);
}
