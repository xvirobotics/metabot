import { describe, it, expect, beforeEach } from 'vitest';
import { CostTracker } from '../src/utils/cost-tracker.js';

describe('CostTracker', () => {
  let tracker: CostTracker;

  beforeEach(() => {
    tracker = new CostTracker();
  });

  it('starts with empty stats', () => {
    const stats = tracker.getStats();
    expect(stats.global.totalTasks).toBe(0);
    expect(Object.keys(stats.byBot)).toHaveLength(0);
    expect(Object.keys(stats.byUser)).toHaveLength(0);
  });

  it('records a successful task', () => {
    tracker.record({ botName: 'bot1', userId: 'user1', success: true, costUsd: 0.5, durationMs: 1000 });
    const stats = tracker.getStats();
    expect(stats.global.totalTasks).toBe(1);
    expect(stats.global.completedTasks).toBe(1);
    expect(stats.global.failedTasks).toBe(0);
    expect(stats.global.totalCostUsd).toBe(0.5);
    expect(stats.global.totalDurationMs).toBe(1000);
  });

  it('records a failed task', () => {
    tracker.record({ botName: 'bot1', userId: 'user1', success: false, costUsd: 0.1, durationMs: 500 });
    const stats = tracker.getStats();
    expect(stats.global.failedTasks).toBe(1);
    expect(stats.global.completedTasks).toBe(0);
  });

  it('aggregates by bot name', () => {
    tracker.record({ botName: 'bot1', userId: 'user1', success: true, costUsd: 1.0 });
    tracker.record({ botName: 'bot2', userId: 'user1', success: true, costUsd: 2.0 });
    tracker.record({ botName: 'bot1', userId: 'user2', success: false, costUsd: 0.5 });

    const stats = tracker.getStats();
    expect(stats.byBot['bot1'].totalTasks).toBe(2);
    expect(stats.byBot['bot1'].totalCostUsd).toBe(1.5);
    expect(stats.byBot['bot2'].totalTasks).toBe(1);
    expect(stats.byBot['bot2'].totalCostUsd).toBe(2.0);
  });

  it('aggregates by user', () => {
    tracker.record({ botName: 'bot1', userId: 'user1', success: true, costUsd: 1.0 });
    tracker.record({ botName: 'bot1', userId: 'user2', success: true, costUsd: 2.0 });
    tracker.record({ botName: 'bot2', userId: 'user1', success: true, costUsd: 3.0 });

    const stats = tracker.getStats();
    expect(stats.byUser['user1'].totalTasks).toBe(2);
    expect(stats.byUser['user1'].totalCostUsd).toBe(4.0);
    expect(stats.byUser['user2'].totalTasks).toBe(1);
  });

  it('resets all stats', () => {
    tracker.record({ botName: 'bot1', userId: 'user1', success: true, costUsd: 1.0 });
    tracker.reset();

    const stats = tracker.getStats();
    expect(stats.global.totalTasks).toBe(0);
    expect(Object.keys(stats.byBot)).toHaveLength(0);
  });

  it('handles zero cost and duration', () => {
    tracker.record({ botName: 'bot1', userId: 'user1', success: true });
    const stats = tracker.getStats();
    expect(stats.global.totalCostUsd).toBe(0);
    expect(stats.global.totalDurationMs).toBe(0);
  });
});
