import * as fs from 'node:fs';
import * as os from 'node:os';
import * as path from 'node:path';
import { fileURLToPath } from 'node:url';
import type { Logger } from '../utils/logger.js';

const SKILL_NAMES = ['metaskill', 'metamemory'];

// Repo root: two levels up from src/api/
const repoRoot = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..', '..');

export function installSkillsToWorkDir(workDir: string, logger: Logger): void {
  const repoSkillsDir = path.join(repoRoot, 'skills');
  const userSkillsDir = path.join(os.homedir(), '.claude', 'skills');
  const destSkillsDir = path.join(workDir, '.claude', 'skills');

  for (const skill of SKILL_NAMES) {
    // Try bundled repo skills first, then user-level
    const repoSrc = path.join(repoSkillsDir, skill);
    const userSrc = path.join(userSkillsDir, skill);
    const src = fs.existsSync(repoSrc) ? repoSrc : fs.existsSync(userSrc) ? userSrc : null;

    if (!src) {
      logger.debug({ skill }, 'Skill source not found, skipping');
      continue;
    }

    const dest = path.join(destSkillsDir, skill);
    fs.mkdirSync(dest, { recursive: true });
    fs.cpSync(src, dest, { recursive: true });
    logger.info({ skill, src, dest }, 'Skill installed to working directory');
  }
}
