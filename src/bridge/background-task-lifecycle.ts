import type { CardState } from '../types.js';

export type ActivityLifecycleType = 'task_waiting' | 'task_completed' | 'task_failed';

export function hasRunningBackgroundWork(state: CardState): boolean {
  return state.hasRunningBackgroundWork
    ?? (state.backgroundEvents?.some((event) => event.kind === 'work' && event.status === 'running') === true
      || state.teamState?.tasks.some((task) => task.status === 'in_progress') === true
      || state.teamState?.teammates.some((member) => member.status === 'working') === true);
}

export function activityTypeForState(state: CardState): ActivityLifecycleType {
  if (state.status === 'error') return 'task_failed';
  if (hasRunningBackgroundWork(state)) return 'task_waiting';
  return state.status === 'complete' ? 'task_completed' : 'task_failed';
}
