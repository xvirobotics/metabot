import { describe, it, expect } from 'vitest';
import { buildCard, buildHelpCard, buildStatusCard, buildTextCard } from '../src/feishu/card-builder.js';
import { optimizeMarkdownStyle, sanitizeTextForCard, findMarkdownTablesOutsideCodeBlocks } from '../src/feishu/markdown-style.js';
import type { CardState } from '../src/types.js';

// ---------------------------------------------------------------------------
// buildCard — CardKit v2 structure
// ---------------------------------------------------------------------------

describe('buildCard', () => {
  it('outputs CardKit v2 structure with schema and body.elements', () => {
    const state: CardState = {
      status: 'complete',
      userPrompt: 'hello',
      responseText: 'Done.',
      toolCalls: [],
    };
    const json = JSON.parse(buildCard(state));
    expect(json.schema).toBe('2.0');
    expect(json.body).toBeDefined();
    expect(json.body.elements).toBeInstanceOf(Array);
    // v1 top-level `elements` should NOT exist
    expect(json.elements).toBeUndefined();
  });

  it('builds thinking card', () => {
    const state: CardState = {
      status: 'thinking',
      userPrompt: 'hello',
      responseText: '',
      toolCalls: [],
    };
    const json = JSON.parse(buildCard(state));
    expect(json.header.template).toBe('blue');
    expect(json.header.title.content).toContain('Thinking');
    expect(json.body.elements.some((e: any) => e.tag === 'markdown' && e.content.includes('thinking'))).toBe(true);
  });

  it('builds running card with tool calls', () => {
    const state: CardState = {
      status: 'running',
      userPrompt: 'fix bug',
      responseText: 'Looking at the code...',
      toolCalls: [
        { name: 'Read', detail: '`src/index.ts`', status: 'done' },
        { name: 'Edit', detail: '`src/index.ts`', status: 'running' },
      ],
    };
    const json = JSON.parse(buildCard(state));
    expect(json.header.template).toBe('blue');
    const md = json.body.elements.find((e: any) => e.tag === 'markdown' && e.content.includes('Read'));
    expect(md).toBeDefined();
    expect(md.content).toContain('✅');
    expect(md.content).toContain('⏳');
  });

  it('builds complete card with stats', () => {
    const state: CardState = {
      status: 'complete',
      userPrompt: 'task',
      responseText: 'All done!',
      toolCalls: [],
      durationMs: 5000,
      costUsd: 0.03,
    };
    const json = JSON.parse(buildCard(state));
    expect(json.header.template).toBe('green');
    const footer = json.body.elements.find(
      (e: any) => e.tag === 'markdown' && e.text_size === 'notation_small_v2'
    );
    expect(footer).toBeDefined();
    expect(footer.content).toContain('5.0s');
  });

  it('builds error card with error message', () => {
    const state: CardState = {
      status: 'error',
      userPrompt: 'task',
      responseText: '',
      toolCalls: [],
      errorMessage: 'Process crashed',
    };
    const json = JSON.parse(buildCard(state));
    expect(json.header.template).toBe('red');
    const errEl = json.body.elements.find((e: any) => e.tag === 'markdown' && e.content.includes('Process crashed'));
    expect(errEl).toBeDefined();
  });

  it('builds waiting_for_input card with question', () => {
    const state: CardState = {
      status: 'waiting_for_input',
      userPrompt: 'deploy',
      responseText: 'Before deploying...',
      toolCalls: [],
      pendingQuestion: {
        toolUseId: 'q1',
        questions: [{
          question: 'Which env?',
          header: 'Deploy',
          options: [
            { label: 'Production', description: 'Live environment' },
            { label: 'Staging', description: 'Test environment' },
          ],
          multiSelect: false,
        }],
      },
    };
    const json = JSON.parse(buildCard(state));
    expect(json.header.template).toBe('yellow');
    const qEl = json.body.elements.find((e: any) => e.tag === 'markdown' && e.content.includes('Which env?'));
    expect(qEl).toBeDefined();
    expect(qEl.content).toContain('Production');
    expect(qEl.content).toContain('Staging');
  });

  it('truncates long content', () => {
    const state: CardState = {
      status: 'complete',
      userPrompt: 'task',
      responseText: 'x'.repeat(30000),
      toolCalls: [],
    };
    const json = JSON.parse(buildCard(state));
    const md = json.body.elements.find((e: any) => e.tag === 'markdown' && e.content.includes('truncated'));
    expect(md).toBeDefined();
  });
});

describe('buildHelpCard', () => {
  it('returns valid v2 card JSON', () => {
    const json = JSON.parse(buildHelpCard());
    expect(json.schema).toBe('2.0');
    expect(json.header.title.content).toContain('Help');
    expect(json.body.elements.length).toBeGreaterThan(0);
  });
});

describe('buildStatusCard', () => {
  it('shows session info', () => {
    const json = JSON.parse(buildStatusCard('user123', '/home/user/project', 'sess-abc-12345678', true));
    expect(json.schema).toBe('2.0');
    const md = json.body.elements[0].content;
    expect(md).toContain('user123');
    expect(md).toContain('/home/user/project');
    expect(md).toContain('sess-abc');
    expect(md).toContain('Yes');
  });

  it('shows no session', () => {
    const json = JSON.parse(buildStatusCard('user', '/home', undefined, false));
    const md = json.body.elements[0].content;
    expect(md).toContain('None');
    expect(md).toContain('No');
  });
});

describe('buildTextCard', () => {
  it('builds simple text card with v2 structure', () => {
    const json = JSON.parse(buildTextCard('Title', 'Some content', 'green'));
    expect(json.schema).toBe('2.0');
    expect(json.header.template).toBe('green');
    expect(json.header.title.content).toBe('Title');
    expect(json.body.elements[0].content).toBe('Some content');
  });
});

// ---------------------------------------------------------------------------
// optimizeMarkdownStyle
// ---------------------------------------------------------------------------

describe('optimizeMarkdownStyle', () => {
  it('demotes headings when H1-H3 are present', () => {
    const input = '# Title\n## Section\n### Sub\ntext';
    const result = optimizeMarkdownStyle(input);
    expect(result).toContain('#### Title');
    expect(result).toContain('##### Section');
    expect(result).toContain('##### Sub');
  });

  it('does not demote headings when only H4+ are present', () => {
    const input = '#### Already small\n##### Tiny';
    const result = optimizeMarkdownStyle(input);
    expect(result).toContain('#### Already small');
    expect(result).toContain('##### Tiny');
  });

  it('adds <br> spacing around tables for cardVersion >= 2', () => {
    const input = 'Some text\n\n| A | B |\n|---|---|\n| 1 | 2 |\n\nMore text';
    const result = optimizeMarkdownStyle(input, 2);
    expect(result).toContain('<br>');
  });

  it('does not add <br> for cardVersion 1', () => {
    const input = 'Some text\n\n| A | B |\n|---|---|\n| 1 | 2 |\n\nMore text';
    const result = optimizeMarkdownStyle(input, 1);
    expect(result).not.toContain('<br>');
  });

  it('protects code block content from heading demotion', () => {
    const input = '# Title\n\n```\n# Not a heading\n## Also not\n```\n\nEnd';
    const result = optimizeMarkdownStyle(input);
    expect(result).toContain('#### Title');
    // Code block content should be preserved
    expect(result).toContain('# Not a heading');
    expect(result).toContain('## Also not');
  });

  it('compresses excessive blank lines', () => {
    const input = 'A\n\n\n\n\nB';
    const result = optimizeMarkdownStyle(input);
    expect(result).toBe('A\n\nB');
  });

  it('strips invalid image keys', () => {
    const input = '![alt](https://example.com/img.png)\n![ok](img_abc123)';
    const result = optimizeMarkdownStyle(input);
    expect(result).not.toContain('https://example.com');
    expect(result).toContain('![ok](img_abc123)');
  });
});

// ---------------------------------------------------------------------------
// sanitizeTextForCard
// ---------------------------------------------------------------------------

describe('sanitizeTextForCard', () => {
  const makeTable = (label: string) =>
    `| ${label} | Value |\n|---|---|\n| a | 1 |`;

  it('passes through text with tables within limit', () => {
    const text = `${makeTable('T1')}\n\n${makeTable('T2')}\n\n${makeTable('T3')}`;
    expect(sanitizeTextForCard(text)).toBe(text);
  });

  it('wraps excess tables in code blocks', () => {
    const tables = Array.from({ length: 5 }, (_, i) => makeTable(`T${i + 1}`));
    const text = tables.join('\n\n');
    const result = sanitizeTextForCard(text);

    // First 3 tables should remain as markdown tables
    const remainingTables = findMarkdownTablesOutsideCodeBlocks(result);
    expect(remainingTables.length).toBe(3);

    // Tables 4 and 5 should be wrapped in code blocks
    expect(result).toContain('```\n| T4');
    expect(result).toContain('```\n| T5');
  });

  it('ignores tables inside code blocks', () => {
    const text = '```\n| A | B |\n|---|---|\n| 1 | 2 |\n```';
    expect(sanitizeTextForCard(text)).toBe(text);
  });

  it('returns text unchanged when no tables exist', () => {
    const text = 'Just some plain text without tables.';
    expect(sanitizeTextForCard(text)).toBe(text);
  });
});
