import type { BackgroundEvent, CardState, TeamState, TeamTask as CardTeamTask } from '../types.js';
import type { AgentTeam, TeamAgent, TeamRun, TeamTask } from './team-store.js';

const MAX_BACKGROUND_EVENTS = 5;

export interface AgentTeamStatusSnapshot {
  team: AgentTeam;
  agents: TeamAgent[];
  tasks: TeamTask[];
  runs: TeamRun[];
}

export function buildAgentTeamCardSnapshot(snapshot: AgentTeamStatusSnapshot): Pick<CardState, 'teamState' | 'backgroundEvents' | 'hasRunningBackgroundWork'> {
  const taskById = new Map(snapshot.tasks.map((task) => [task.id, task]));
  const teamState: TeamState = {
    name: snapshot.team.name,
    teammates: snapshot.agents.map((agent) => ({
      name: agent.name,
      status: agent.status === 'working' ? 'working' : 'idle',
      ...(agent.status === 'stopped' ? { lastSubject: 'stopped' } : {}),
    })),
    tasks: snapshot.tasks
      .filter(isVisibleTask)
      .map((task): CardTeamTask => ({
        taskId: String(task.id),
        subject: task.subject,
        status: task.status,
        ...(task.owner ? { teammate: task.owner } : {}),
      })),
  };

  const backgroundEvents: BackgroundEvent[] = snapshot.runs
    .filter((run) => isVisibleRun(run, run.taskId ? taskById.get(run.taskId) : undefined))
    .sort((a, b) => b.updatedAt - a.updatedAt)
    .slice(0, MAX_BACKGROUND_EVENTS)
    .map((run) => {
      const task = run.taskId ? taskById.get(run.taskId) : undefined;
      const who = run.agentName ? `${run.agentName}` : 'agent';
      const description = task ? `${who}: ${task.subject}` : `${who}: ${run.id}`;
      return {
        kind: 'work',
        taskId: run.id,
        description,
        status: run.status,
        ...(run.output ? { lastEvent: run.output } : run.error ? { lastEvent: run.error } : {}),
      };
    });

  return {
    hasRunningBackgroundWork: snapshot.runs.some((run) => run.status === 'running')
      || snapshot.tasks.some((task) => task.status === 'in_progress')
      || snapshot.agents.some((agent) => agent.status === 'working'),
    teamState,
    ...(backgroundEvents.length > 0 ? { backgroundEvents } : {}),
  };
}

function isVisibleTask(task: TeamTask): task is TeamTask & { status: CardTeamTask['status'] } {
  return task.status === 'pending' || task.status === 'in_progress';
}

function isVisibleRun(run: TeamRun, task?: TeamTask): boolean {
  if (run.status === 'completed') {
    return false;
  }

  if (run.status === 'running') {
    return true;
  }

  return !!task && isVisibleTask(task);
}
