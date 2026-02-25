/**
 * Lightweight Prometheus-compatible metrics collector.
 * No external deps — generates text/plain exposition format.
 */

export class MetricsCollector {
  private counters = new Map<string, { value: number; help: string; labels: Map<string, number> }>();
  private gauges = new Map<string, { value: number; help: string }>();
  private histogramBuckets = new Map<string, { help: string; buckets: number[]; counts: number[]; sum: number; count: number }>();

  counter(name: string, help: string): void {
    if (!this.counters.has(name)) {
      this.counters.set(name, { value: 0, help, labels: new Map() });
    }
  }

  gauge(name: string, help: string): void {
    if (!this.gauges.has(name)) {
      this.gauges.set(name, { value: 0, help });
    }
  }

  histogram(name: string, help: string, buckets: number[]): void {
    if (!this.histogramBuckets.has(name)) {
      this.histogramBuckets.set(name, { help, buckets: [...buckets], counts: new Array(buckets.length + 1).fill(0), sum: 0, count: 0 });
    }
  }

  incCounter(name: string, label?: string, amount = 1): void {
    const c = this.counters.get(name);
    if (!c) return;
    c.value += amount;
    if (label) {
      c.labels.set(label, (c.labels.get(label) || 0) + amount);
    }
  }

  setGauge(name: string, value: number): void {
    const g = this.gauges.get(name);
    if (g) g.value = value;
  }

  observeHistogram(name: string, value: number): void {
    const h = this.histogramBuckets.get(name);
    if (!h) return;
    h.sum += value;
    h.count++;
    // counts[i] stores the cumulative count for bucket[i], updated directly
    for (let i = 0; i < h.buckets.length; i++) {
      if (value <= h.buckets[i]) h.counts[i]++;
    }
  }

  /** Generate Prometheus exposition format text. */
  serialize(): string {
    const lines: string[] = [];

    for (const [name, c] of this.counters) {
      lines.push(`# HELP ${name} ${c.help}`);
      lines.push(`# TYPE ${name} counter`);
      if (c.labels.size > 0) {
        for (const [label, val] of c.labels) {
          lines.push(`${name}{status="${label}"} ${val}`);
        }
      } else {
        lines.push(`${name} ${c.value}`);
      }
    }

    for (const [name, g] of this.gauges) {
      lines.push(`# HELP ${name} ${g.help}`);
      lines.push(`# TYPE ${name} gauge`);
      lines.push(`${name} ${g.value}`);
    }

    for (const [name, h] of this.histogramBuckets) {
      lines.push(`# HELP ${name} ${h.help}`);
      lines.push(`# TYPE ${name} histogram`);
      for (let i = 0; i < h.buckets.length; i++) {
        // counts[i] is already cumulative (every observation <= bucket[i] increments it)
        lines.push(`${name}_bucket{le="${h.buckets[i]}"} ${h.counts[i]}`);
      }
      lines.push(`${name}_bucket{le="+Inf"} ${h.count}`);
      lines.push(`${name}_sum ${h.sum}`);
      lines.push(`${name}_count ${h.count}`);
    }

    return lines.join('\n') + '\n';
  }
}

// Singleton metrics instance
export const metrics = new MetricsCollector();

// Register default metrics
metrics.counter('metabot_tasks_total', 'Total number of tasks processed');
metrics.counter('metabot_tasks_by_status', 'Tasks by completion status');
metrics.counter('metabot_commands_total', 'Total slash commands processed');
metrics.counter('metabot_auth_denied_total', 'Total auth denied events');
metrics.counter('metabot_api_tasks_total', 'Total API tasks processed');
metrics.gauge('metabot_active_tasks', 'Currently running tasks');
metrics.gauge('metabot_uptime_seconds', 'Process uptime in seconds');
metrics.histogram('metabot_task_duration_seconds', 'Task duration in seconds', [1, 5, 10, 30, 60, 120, 300, 600, 1800, 3600]);
metrics.histogram('metabot_task_cost_usd', 'Task cost in USD', [0.01, 0.05, 0.1, 0.5, 1, 5, 10]);
