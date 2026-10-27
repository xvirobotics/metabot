import { afterEach, describe, expect, it, vi } from 'vitest';
import { CircuitBreaker } from '../src/api/circuit-breaker.js';

const logger = {
  info: vi.fn(), warn: vi.fn(), child: () => logger,
} as any;

function createBreaker(): CircuitBreaker {
  return new CircuitBreaker(logger, {
    failureThreshold: 1, resetTimeoutMs: 1000, halfOpenMaxAttempts: 2,
  });
}

function acquire(breaker: CircuitBreaker) {
  const admission = breaker.tryAcquire('bot');
  expect(admission).toBeDefined();
  if (!admission) throw new Error('Expected circuit admission');
  return admission;
}

function readyProbe(breaker: CircuitBreaker) {
  const now = vi.spyOn(Date, 'now').mockReturnValue(0);
  acquire(breaker).settle('failure');
  now.mockReturnValue(1000);
  return now;
}

describe('CircuitBreaker owned admissions', () => {
  afterEach(() => vi.restoreAllMocks());

  it('reports availability without allocating, transitioning, or acquiring', () => {
    const breaker = createBreaker();
    expect(breaker.isAvailable('bot')).toBe(true);
    expect(breaker.getStatus()).toEqual({});
    readyProbe(breaker);
    expect(breaker.isAvailable('bot')).toBe(true);
    expect(breaker.isAvailable('bot')).toBe(true);
    expect(breaker.getStatus().bot).toEqual({ state: 'open', failures: 1 });
    acquire(breaker);
    expect(breaker.isAvailable('bot')).toBe(false);
    expect(breaker.tryAcquire('bot')).toBeUndefined();
  });

  it('allows only one probe and closes after two successful probes', () => {
    const breaker = createBreaker();
    const now = vi.spyOn(Date, 'now').mockReturnValue(0);
    acquire(breaker).settle('failure');
    expect(breaker.tryAcquire('bot')).toBeUndefined();
    now.mockReturnValue(1000);
    const first = acquire(breaker);
    expect(breaker.tryAcquire('bot')).toBeUndefined();
    first.settle('success');
    expect(breaker.getStatus().bot.state).toBe('half-open');
    acquire(breaker).settle('success');
    expect(breaker.getStatus().bot).toEqual({ state: 'closed', failures: 0 });
  });

  it('reopens after a failed probe and gates the next recovery window', () => {
    const breaker = createBreaker();
    const now = readyProbe(breaker);
    acquire(breaker).settle('failure');
    expect(breaker.getStatus().bot).toEqual({ state: 'open', failures: 2 });
    expect(breaker.tryAcquire('bot')).toBeUndefined();
    now.mockReturnValue(2000);
    acquire(breaker);
    expect(breaker.tryAcquire('bot')).toBeUndefined();
  });

  it('abandons neutrally and never releases the replacement probe twice', () => {
    const breaker = createBreaker();
    readyProbe(breaker);
    const old = acquire(breaker);
    old.settle('abandoned');
    const replacement = acquire(breaker);
    old.settle('success');
    old.settle('failure');
    expect(breaker.getStatus().bot).toEqual({ state: 'half-open', failures: 1 });
    expect(breaker.tryAcquire('bot')).toBeUndefined();
    replacement.settle('success');
    expect(breaker.isAvailable('bot')).toBe(true);
  });

  it('accepts only the first settlement even after a new probe acquires', () => {
    const breaker = createBreaker();
    readyProbe(breaker);
    const old = acquire(breaker);
    old.settle('success');
    const replacement = acquire(breaker);
    old.settle('failure');
    old.settle('success');
    old.settle('abandoned');
    expect(breaker.tryAcquire('bot')).toBeUndefined();
    expect(breaker.getStatus().bot).toEqual({ state: 'half-open', failures: 1 });
    replacement.settle('success');
    expect(breaker.getStatus().bot).toEqual({ state: 'closed', failures: 0 });
  });

  it.each(['success', 'failure'] as const)('ignores a closed-era %s while a recovery probe is owned', (outcome) => {
    const breaker = createBreaker();
    const old = acquire(breaker);
    readyProbe(breaker);
    const probe = acquire(breaker);
    old.settle(outcome);
    expect(breaker.tryAcquire('bot')).toBeUndefined();
    expect(breaker.getStatus().bot).toEqual({ state: 'half-open', failures: 1 });
    probe.settle('success');
    expect(breaker.getStatus().bot.state).toBe('half-open');
  });

  it('ignores a closed-era failure even after the circuit recovers', () => {
    const breaker = createBreaker();
    const old = acquire(breaker);
    readyProbe(breaker);
    acquire(breaker).settle('success');
    acquire(breaker).settle('success');
    old.settle('failure');
    expect(breaker.getStatus().bot).toEqual({ state: 'closed', failures: 0 });
  });

  it.each(['success', 'failure', 'abandoned'] as const)('does not recreate a reset circuit on late %s', (outcome) => {
    const breaker = createBreaker();
    readyProbe(breaker);
    const old = acquire(breaker);
    breaker.reset('bot');
    old.settle(outcome);
    expect(breaker.getStatus()).toEqual({});
  });

  it('does not let a reset-era completion settle a replacement circuit probe', () => {
    const breaker = createBreaker();
    readyProbe(breaker);
    const old = acquire(breaker);
    breaker.reset('bot');
    readyProbe(breaker);
    acquire(breaker);
    old.settle('failure');
    expect(breaker.tryAcquire('bot')).toBeUndefined();
    expect(breaker.getStatus().bot).toEqual({ state: 'half-open', failures: 1 });
  });
});
