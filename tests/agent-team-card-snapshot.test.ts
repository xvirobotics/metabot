import { describe, expect, it } from 'vitest';
import { buildAgentTeamCardSnapshot } from '../src/agent-teams/card-snapshot.js';

describe('buildAgentTeamCardSnapshot', () => {
  it('maps MetaBot Agent Team status into card teamState and backgroundEvents', () => {
    const snapshot = buildAgentTeamCardSnapshot({
      team: { name: 'demo', status: 'active', createdAt: 1, updatedAt: 2 },
      agents: [
        { teamName: 'demo', name: 'lead', role: 'orchestrator', engine: 'codex', status: 'idle', createdAt: 1, updatedAt: 1 },
        { teamName: 'demo', name: 'reviewer', role: 'review', engine: 'kimi', status: 'working', createdAt: 1, updatedAt: 1 },
      ],
      tasks: [
        { teamName: 'demo', id: 1, subject: 'Plan work', status: 'pending', owner: 'lead', blockedBy: [], createdAt: 1, updatedAt: 1 },
        { teamName: 'demo', id: 2, subject: 'Review output', status: 'in_progress', owner: 'reviewer', blockedBy: [], createdAt: 1, updatedAt: 1 },
        { teamName: 'demo', id: 3, subject: 'Done already', status: 'completed', owner: 'lead', blockedBy: [], createdAt: 1, updatedAt: 3 },
      ],
      runs: [
        { id: 'run-abc123', teamName: 'demo', agentName: 'reviewer', taskId: 2, status: 'running', output: 'checking', startedAt: 1, updatedAt: 1 },
        { id: 'run-done', teamName: 'demo', agentName: 'lead', taskId: 3, status: 'completed', output: 'done', startedAt: 1, updatedAt: 4 },
      ],
    });

    expect(snapshot.teamState?.name).toBe('demo');
    expect(snapshot.teamState?.teammates).toEqual([
      { name: 'lead', status: 'idle' },
      { name: 'reviewer', status: 'working' },
    ]);
    expect(snapshot.teamState?.tasks).toEqual([
      { taskId: '1', subject: 'Plan work', status: 'pending', teammate: 'lead' },
      { taskId: '2', subject: 'Review output', status: 'in_progress', teammate: 'reviewer' },
    ]);
    expect(snapshot.backgroundEvents?.[0]).toMatchObject({
      taskId: 'run-abc123',
      description: 'reviewer: Review output',
      status: 'running',
      lastEvent: 'checking',
    });
  });

  it('keeps the card compact by hiding completed work and stale completed background runs', () => {
    const snapshot = buildAgentTeamCardSnapshot({
      team: { name: 'demo', status: 'active', createdAt: 1, updatedAt: 2 },
      agents: [
        { teamName: 'demo', name: 'lead', role: 'orchestrator', engine: 'codex', status: 'idle', createdAt: 1, updatedAt: 1 },
      ],
      tasks: [
        { teamName: 'demo', id: 1, subject: 'Open implementation', status: 'in_progress', owner: 'lead', blockedBy: [], createdAt: 1, updatedAt: 3 },
        { teamName: 'demo', id: 2, subject: 'Completed review', status: 'completed', owner: 'lead', blockedBy: [], createdAt: 1, updatedAt: 4 },
      ],
      runs: [
        { id: 'run-failed-open', teamName: 'demo', agentName: 'lead', taskId: 1, status: 'failed', error: 'needs retry', startedAt: 1, updatedAt: 10 },
        { id: 'run-running', teamName: 'demo', agentName: 'lead', taskId: 1, status: 'running', output: 'working', startedAt: 1, updatedAt: 9 },
        { id: 'run-failed-done', teamName: 'demo', agentName: 'lead', taskId: 2, status: 'failed', error: 'old failure', startedAt: 1, updatedAt: 11 },
        { id: 'run-completed-open', teamName: 'demo', agentName: 'lead', taskId: 1, status: 'completed', output: 'done', startedAt: 1, updatedAt: 12 },
        { id: 'run-orphan-failed', teamName: 'demo', agentName: 'lead', status: 'failed', error: 'old orphan failure', startedAt: 1, updatedAt: 13 },
      ],
    });

    expect(snapshot.teamState?.tasks).toEqual([
      { taskId: '1', subject: 'Open implementation', status: 'in_progress', teammate: 'lead' },
    ]);
    expect(snapshot.backgroundEvents).toEqual([
      { kind: 'work', taskId: 'run-failed-open', description: 'lead: Open implementation', status: 'failed', lastEvent: 'needs retry' },
      { kind: 'work', taskId: 'run-running', description: 'lead: Open implementation', status: 'running', lastEvent: 'working' },
    ]);
  });
});
