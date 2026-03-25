import * as fs from 'node:fs';
import * as path from 'node:path';
import * as url from 'node:url';
import type { Logger } from '../utils/logger.js';

export interface InstallSkillsOptions {
  /** Bot platform — feishu-only skills are skipped for other platforms. */
  platform?: 'feishu' | 'telegram' | 'wechat';
}

/**
 * Deploy workspace CLAUDE.md to a new bot's working directory.
 *
 * Skills (metaskill, metamemory, metabot, etc.) are NOT copied per-bot — they
 * live in ~/.claude/skills/ (global) and Claude Code makes them available to
 * all bots automatically.  Copying them per-bot created redundant duplicates
 * that diverged from the global versions over time.
 */
export function installSkillsToWorkDir(workDir: string, logger: Logger, _options?: InstallSkillsOptions): void {
  // Deploy workspace CLAUDE.md if not already present
  const destClaudeMd = path.join(workDir, 'CLAUDE.md');
  if (!fs.existsSync(destClaudeMd)) {
    const thisFile = url.fileURLToPath(import.meta.url);
    const thisDir = path.dirname(thisFile);
    // Try src/workspace/CLAUDE.md (tsx) or dist/workspace/CLAUDE.md (compiled)
    for (const candidate of [
      path.join(thisDir, '..', 'workspace', 'CLAUDE.md'),
      path.join(thisDir, '..', '..', 'src', 'workspace', 'CLAUDE.md'),
    ]) {
      if (fs.existsSync(candidate)) {
        fs.copyFileSync(candidate, destClaudeMd);
        logger.info({ dest: destClaudeMd }, 'CLAUDE.md deployed to working directory');
        break;
      }
    }
  }
}
