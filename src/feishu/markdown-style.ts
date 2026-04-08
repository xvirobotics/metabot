/**
 * Markdown style optimization for Feishu card rendering.
 *
 * Ported from OpenClaw Lark (https://github.com/larksuite/openclaw-lark/)
 * src/card/markdown-style.ts and src/card/card-error.ts (MIT License).
 */

// ---------------------------------------------------------------------------
// optimizeMarkdownStyle
// ---------------------------------------------------------------------------

/**
 * Optimize markdown for Feishu card display:
 * - Heading demotion: H1 → H4, H2~H6 → H5 (only when H1-H3 exist)
 * - Add <br> spacing before/after tables and code blocks (cardVersion >= 2)
 * - Compress excessive blank lines (3+ → 2)
 * - Protect code block content from transformation
 * - Strip invalid image keys (non img_ prefixed)
 */
export function optimizeMarkdownStyle(text: string, cardVersion = 2): string {
  try {
    let r = _optimizeMarkdownStyle(text, cardVersion);
    r = stripInvalidImageKeys(r);
    return r;
  } catch {
    return text;
  }
}

function _optimizeMarkdownStyle(text: string, cardVersion = 2): string {
  // 1. Extract code blocks, protect with placeholders, restore after processing
  const MARK = '___CB_';
  const codeBlocks: string[] = [];
  let r = text.replace(/(^|\n)(`{3,})([^\n]*)\n[\s\S]*?\n\2(?=\n|$)/g, (m, prefix = '') => {
    const block = m.slice(String(prefix).length);
    return `${prefix}${MARK}${codeBlocks.push(block) - 1}___`;
  });

  // 2. Heading demotion — only when original text contains h1~h3 headings.
  //    Process H2~H6 → H5 first, then H1 → H4 (order matters to avoid
  //    double-matching H1→H4 as H2~H6).
  const hasH1toH3 = /^#{1,3} /m.test(text);
  if (hasH1toH3) {
    r = r.replace(/^#{2,6} (.+)$/gm, '##### $1'); // H2~H6 → H5
    r = r.replace(/^# (.+)$/gm, '#### $1');       // H1 → H4
  }

  if (cardVersion >= 2) {
    // 3. Add <br> between consecutive headings
    r = r.replace(/^(#{4,5} .+)\n{1,2}(#{4,5} )/gm, '$1\n<br>\n$2');

    // 4. Table spacing
    // 4a. Non-table line directly followed by table line — insert blank line
    r = r.replace(/^([^|\n].*)\n(\|.+\|)/gm, '$1\n\n$2');
    // 4b. Insert <br> before table blocks
    r = r.replace(/\n\n((?:\|.+\|[^\S\n]*\n?)+)/g, '\n\n<br>\n\n$1');
    // 4c. Append <br> after table blocks (skip if followed by hr/heading/bold/EOF)
    r = r.replace(/((?:^\|.+\|[^\S\n]*\n?)+)/gm, (m, _table, offset) => {
      const after = r.slice(offset + m.length).replace(/^\n+/, '');
      if (!after || /^(---|#{4,5} |\*\*)/.test(after)) return m;
      return m + '\n<br>\n';
    });
    // 4d. Clean up extra blank lines around <br> for plain text before tables
    r = r.replace(/^((?!#{4,5} )(?!\*\*).+)\n\n(<br>)\n\n(\|)/gm, '$1\n$2\n$3');
    // 4d2. Bold line before table
    r = r.replace(/^(\*\*.+)\n\n(<br>)\n\n(\|)/gm, '$1\n$2\n\n$3');
    // 4e. Clean up extra blank lines around <br> for plain text after tables
    r = r.replace(/(\|[^\n]*\n)\n(<br>\n)((?!#{4,5} )(?!\*\*))/gm, '$1$2$3');

    // 5. Restore code blocks with <br> spacing
    codeBlocks.forEach((block, i) => {
      r = r.replace(`${MARK}${i}___`, `\n<br>\n${block}\n<br>\n`);
    });
  } else {
    // 5. Restore code blocks without <br>
    codeBlocks.forEach((block, i) => {
      r = r.replace(`${MARK}${i}___`, block);
    });
  }

  // 6. Compress excessive blank lines (3+ newlines → 2)
  r = r.replace(/\n{3,}/g, '\n\n');

  return r;
}

// ---------------------------------------------------------------------------
// stripInvalidImageKeys
// ---------------------------------------------------------------------------

/** Matches complete markdown image syntax: `![alt](value)` */
const IMAGE_RE = /!\[([^\]]*)\]\(([^)\s]+)\)/g;

/**
 * Strip `![alt](value)` where value is not a valid Feishu image key
 * (`img_xxx`). Prevents CardKit error 200570.
 */
function stripInvalidImageKeys(text: string): string {
  if (!text.includes('![')) return text;
  return text.replace(IMAGE_RE, (fullMatch, _alt, value) => {
    if (value.startsWith('img_')) return fullMatch;
    return ''; // strip all non-img_ image references
  });
}

// ---------------------------------------------------------------------------
// Table limit protection
// ---------------------------------------------------------------------------

/** Empirical Feishu card table limit — 4+ triggers 230099/11310 */
export const FEISHU_CARD_TABLE_LIMIT = 3;

export interface MarkdownTableMatch {
  index: number;
  length: number;
  raw: string;
}

/**
 * Find markdown tables that are outside code blocks (only those are
 * rendered as card table elements by Feishu).
 */
export function findMarkdownTablesOutsideCodeBlocks(text: string): MarkdownTableMatch[] {
  const codeBlockRanges: Array<{ start: number; end: number }> = [];
  const codeBlockRegex = /```[\s\S]*?```/g;
  let codeBlockMatch = codeBlockRegex.exec(text);
  while (codeBlockMatch != null) {
    codeBlockRanges.push({
      start: codeBlockMatch.index,
      end: codeBlockMatch.index + codeBlockMatch[0].length,
    });
    codeBlockMatch = codeBlockRegex.exec(text);
  }

  const isInsideCodeBlock = (idx: number): boolean =>
    codeBlockRanges.some((range) => idx >= range.start && idx < range.end);

  const tableRegex = /\|.+\|[\r\n]+\|[-:| ]+\|[\s\S]*?(?=\n\n|\n(?!\|)|$)/g;
  const matches: MarkdownTableMatch[] = [];
  let tableMatch = tableRegex.exec(text);
  while (tableMatch != null) {
    if (!isInsideCodeBlock(tableMatch.index)) {
      matches.push({
        index: tableMatch.index,
        length: tableMatch[0].length,
        raw: tableMatch[0],
      });
    }
    tableMatch = tableRegex.exec(text);
  }

  return matches;
}

/**
 * Wrap markdown tables beyond the limit in code blocks to prevent
 * Feishu card error 230099/11310 (table element count exceeded).
 *
 * The first `tableLimit` tables are kept as-is for card rendering;
 * excess tables are wrapped in backtick fences.
 */
export function sanitizeTextForCard(text: string, tableLimit: number = FEISHU_CARD_TABLE_LIMIT): string {
  const matches = findMarkdownTablesOutsideCodeBlocks(text);
  if (matches.length <= tableLimit) return text;
  return wrapTablesBeyondLimit(text, matches, tableLimit);
}

function wrapTablesBeyondLimit(text: string, matches: readonly MarkdownTableMatch[], keepCount: number): string {
  if (matches.length <= keepCount) return text;

  // Back-to-front replacement keeps the original indices stable.
  let result = text;
  for (let i = matches.length - 1; i >= keepCount; i--) {
    const { index, length, raw } = matches[i];
    const replacement = `\`\`\`\n${raw}\n\`\`\``;
    result = result.slice(0, index) + replacement + result.slice(index + length);
  }

  return result;
}
