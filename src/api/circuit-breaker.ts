import type { Logger } from '../utils/logger.js';

type CircuitState = 'closed' | 'open' | 'half-open';
type CircuitOutcome = 'success' | 'failure' | 'abandoned';

export interface CircuitAdmission {
  settle(outcome: CircuitOutcome): void;
}

interface CircuitConfig {
  failureThreshold: number;
  resetTimeoutMs: number;
  halfOpenMaxAttempts: number;
}

interface BotCircuit {
  state: CircuitState;
  generation: symbol;
  failures: number;
  lastFailure: number;
  halfOpenSuccesses: number;
  probe?: symbol;
}

export class CircuitBreaker {
  private circuits = new Map<string, BotCircuit>();
  private config: CircuitConfig;
  private logger: Logger;

  constructor(logger: Logger, config?: Partial<CircuitConfig>) {
    this.logger = logger.child({ module: 'circuit-breaker' });
    this.config = {
      failureThreshold: config?.failureThreshold ?? 5,
      resetTimeoutMs: config?.resetTimeoutMs ?? 60_000,
      halfOpenMaxAttempts: config?.halfOpenMaxAttempts ?? 2,
    };
  }

  isAvailable(botName: string): boolean {
    const circuit = this.circuits.get(botName);
    if (!circuit || circuit.state === 'closed') return true;
    if (circuit.state === 'open') {
      return Date.now() - circuit.lastFailure >= this.config.resetTimeoutMs;
    }
    return circuit.probe === undefined;
  }

  tryAcquire(botName: string): CircuitAdmission | undefined {
    if (!this.isAvailable(botName)) return undefined;
    let circuit = this.circuits.get(botName);
    if (!circuit) {
      circuit = {
        state: 'closed', generation: Symbol(), failures: 0,
        lastFailure: 0, halfOpenSuccesses: 0,
      };
      this.circuits.set(botName, circuit);
    }
    if (circuit.state === 'open') {
      circuit.state = 'half-open';
      circuit.generation = Symbol();
      circuit.halfOpenSuccesses = 0;
      this.logger.info({ botName }, 'Circuit half-open, allowing probe request');
    }
    const entry = circuit;
    const generation = entry.generation;
    const probe = entry.state === 'half-open' ? Symbol() : undefined;
    if (probe) entry.probe = probe;
    let settled = false;

    return {
      settle: (outcome) => {
        if (settled) return;
        settled = true;
        if (this.circuits.get(botName) !== entry || entry.generation !== generation) return;
        if (entry.state === 'half-open') {
          if (entry.probe !== probe) return;
          entry.probe = undefined;
        }
        if (outcome === 'abandoned') return;
        if (outcome === 'success') {
          if (entry.state === 'half-open') {
            entry.halfOpenSuccesses++;
            if (entry.halfOpenSuccesses < this.config.halfOpenMaxAttempts) return;
            entry.state = 'closed';
            entry.generation = Symbol();
            this.logger.info({ botName }, 'Circuit closed (recovered)');
          }
          entry.failures = 0;
          return;
        }
        entry.failures++;
        entry.lastFailure = Date.now();
        if (entry.state === 'half-open' || entry.failures >= this.config.failureThreshold) {
          entry.state = 'open';
          entry.generation = Symbol();
          this.logger.warn({ botName, failures: entry.failures }, 'Circuit opened (execution failed)');
        }
      },
    };
  }

  getStatus(): Record<string, { state: CircuitState; failures: number }> {
    const status: Record<string, { state: CircuitState; failures: number }> = {};
    for (const [name, circuit] of this.circuits) {
      status[name] = { state: circuit.state, failures: circuit.failures };
    }
    return status;
  }

  reset(botName: string): void {
    this.circuits.delete(botName);
    this.logger.info({ botName }, 'Circuit manually reset');
  }
}
