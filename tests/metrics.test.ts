import { describe, it, expect } from 'vitest';
import { MetricsCollector } from '../src/utils/metrics.js';

describe('MetricsCollector', () => {
  it('increments a counter', () => {
    const m = new MetricsCollector();
    m.counter('test_total', 'test counter');
    m.incCounter('test_total');
    m.incCounter('test_total');
    const output = m.serialize();
    expect(output).toContain('test_total 2');
    expect(output).toContain('# TYPE test_total counter');
  });

  it('tracks counter labels', () => {
    const m = new MetricsCollector();
    m.counter('tasks', 'by status');
    m.incCounter('tasks', 'success');
    m.incCounter('tasks', 'error');
    m.incCounter('tasks', 'success');
    const output = m.serialize();
    expect(output).toContain('tasks{status="success"} 2');
    expect(output).toContain('tasks{status="error"} 1');
  });

  it('sets a gauge value', () => {
    const m = new MetricsCollector();
    m.gauge('active', 'active tasks');
    m.setGauge('active', 5);
    const output = m.serialize();
    expect(output).toContain('active 5');
    expect(output).toContain('# TYPE active gauge');
  });

  it('records histogram observations', () => {
    const m = new MetricsCollector();
    m.histogram('duration', 'task duration', [1, 5, 10]);
    m.observeHistogram('duration', 3);
    m.observeHistogram('duration', 7);
    m.observeHistogram('duration', 0.5);
    const output = m.serialize();
    expect(output).toContain('duration_count 3');
    expect(output).toContain('duration_bucket{le="1"} 1');
    expect(output).toContain('duration_bucket{le="5"} 2');
    expect(output).toContain('duration_bucket{le="10"} 3');
    expect(output).toContain('duration_bucket{le="+Inf"} 3');
  });

  it('ignores operations on unregistered metrics', () => {
    const m = new MetricsCollector();
    m.incCounter('nope');
    m.setGauge('nope', 1);
    m.observeHistogram('nope', 1);
    expect(m.serialize()).toBe('\n');
  });
});
