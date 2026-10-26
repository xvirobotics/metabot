import { mkdtempSync } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { describe, expect, it } from 'vitest';
import { buildAgentTeamPromptContext, buildAgentTeamPromptContextForChat } from '../src/agent-teams/prompt-context.js';
import { AgentTeamStore } from '../src/agent-teams/team-store.js';
import type { BotConfigBase } from '../src/config.js';
import { ClaudeExecutor } from '../src/engines/claude/executor.js';
import { buildClaudeSystemPromptAppend } from '../src/engines/claude/prompt-context.js';
import { CodexExecutor } from '../src/engines/codex/executor.js';
import { KimiExecutor } from '../src/engines/kimi/executor.js';

const logger = {
  child: () => logger,
  info: () => {},
  warn: () => {},
  error: () => {},
  debug: () => {},
} as any;

function config(): BotConfigBase {
  return {
    name: 'metabot',
    engine: 'codex',
    claude: {
      defaultWorkingDirectory: '/tmp',
      maxTurns: undefined,
      maxBudgetUsd: undefined,
      model: undefined,
      apiKey: undefined,
      outputsBaseDir: '/tmp',
      downloadsDir: '/tmp',
      backend: 'pty',
    },
  };
}

describe('Agent Team prompt context', () => {
  it('contains only the compact active roster and dispatch hint', () => {
    const dir = mkdtempSync(join(tmpdir(), 'metabot-team-prompt-'));
    const store = new AgentTeamStore(logger, join(dir, 'teams.db'));
    store.createTeam('metabot-dev', 'Prompt details must stay out', { displayChatIds: ['oc_team'] });
    store.createAgent('metabot-dev', {
      name: 'kimi-frontend',
      role: 'frontend',
      engine: 'kimi',
      prompt: 'SECRET member instructions',
    });
    store.createAgent('metabot-dev', { name: 'reviewer-codex', role: 'reviewer', engine: 'codex' });
    store.createAgent('metabot-dev', { name: 'retired' });
    store.setAgentStatus('metabot-dev', 'kimi-frontend', 'working');
    store.setAgentStatus('metabot-dev', 'retired', 'stopped');
    const task = store.createTask('metabot-dev', {
      subject: 'SECRET task subject',
      description: 'SECRET task description',
      owner: 'kimi-frontend',
    });
    store.sendMessage('metabot-dev', {
      fromName: 'lead',
      toName: 'kimi-frontend',
      body: 'SECRET message body',
    });
    store.createRun('metabot-dev', { agentName: 'kimi-frontend', taskId: task.id, output: 'SECRET run output' });

    const context = buildAgentTeamPromptContext(store, 'metabot-dev');
    expect(context).toContain('## Team Context');
    expect(context).toContain('- kimi-frontend — frontend · kimi · working');
    expect(context).toContain('- reviewer-codex — reviewer · codex · idle');
    expect(context).toContain('metabot teams dispatch metabot-dev <member> "<subject>" --description "..."');
    expect(context).not.toContain('retired');
    expect(context).not.toContain('SECRET');
    expect(buildAgentTeamPromptContextForChat(store, 'oc_team')).toBe(context);
    expect(buildAgentTeamPromptContextForChat(store, 'oc_unbound')).toBeUndefined();
    store.close();
  });

  it('keeps bridge context out of Claude while preserving it for Codex and Kimi', () => {
    const teamContext = ['## Team Context', 'Team: metabot-dev', 'Members:', '- worker — backend · codex · idle'].join(
      '\n',
    );
    const apiContext = {
      botName: 'metabot',
      chatId: 'oc_team',
      engine: 'kimi' as const,
      sessionId: 'session_current',
      teamContext,
    };
    const botConfig = config();
    const privateOutputsDir = '/tmp/bridge-outputs/oc_sensitive_chat';

    const claudeOptions = (new ClaudeExecutor(botConfig, logger) as any).buildQueryOptions(
      '/tmp',
      undefined,
      new AbortController(),
      privateOutputsDir,
      apiContext,
    );
    const claudeAppend = claudeOptions.systemPrompt.append as string;
    const persistentClaudeAppend = buildClaudeSystemPromptAppend({
      outputsDirAvailable: true,
      persistent: true,
    })!;
    const codexPrompt = (new CodexExecutor(botConfig, logger) as any).buildPromptWithContext(
      'hello',
      undefined,
      apiContext,
    );
    const kimiPrompt = (new KimiExecutor(botConfig, logger, {} as any) as any).buildPromptWithContext(
      'hello',
      undefined,
      apiContext,
    );

    for (const prompt of [claudeAppend, persistentClaudeAppend]) {
      expect(prompt).toContain('## Output Files');
      expect(prompt).not.toContain(teamContext);
      expect(prompt).not.toContain('MetaBot');
      expect(prompt).not.toContain('metabot');
      expect(prompt).not.toContain('Feishu');
      expect(prompt).not.toContain('Agent: metabot');
      expect(prompt).not.toContain('Chat ID: oc_team');
      expect(prompt).not.toContain('Session ID: session_current');
      expect(prompt).not.toContain(privateOutputsDir);
      expect(prompt).not.toContain('oc_sensitive_chat');
      expect(prompt).toContain('$ASSISTANT_OUTPUTS_DIR');
    }

    for (const prompt of [codexPrompt, kimiPrompt]) {
      expect(prompt).toContain(teamContext);
      expect(prompt).toContain('## Current MetaBot Context');
      expect(prompt).toContain('Agent: metabot');
      expect(prompt).toContain('Chat ID: oc_team');
      expect(prompt).toContain('Engine: kimi');
      expect(prompt).toContain('Session ID: session_current');
      expect(prompt).toContain('metabot schedule add metabot oc_team <delaySeconds> "<prompt>"');
      expect(prompt).toContain('metabot schedule cron metabot oc_team "<cronExpr>" "<prompt>"');
    }
  });

  it('explains missing Session creation without changing the schedule target', () => {
    const prompt = (new CodexExecutor(config(), logger) as any).buildPromptWithContext('hello', undefined, {
      botName: 'metabot',
      chatId: 'oc_new',
      engine: 'codex',
    });
    expect(prompt).toContain('Session ID: not established yet (a new Session will be created automatically)');
    expect(prompt).toContain('metabot schedule add metabot oc_new');
  });
});
