/**
 * Skill Hub Store: SQLite-backed registry for shared skills.
 * Skills can be published, discovered, and installed across bot instances.
 */
import * as fs from 'node:fs';
import * as path from 'node:path';
import * as crypto from 'node:crypto';
import Database from 'better-sqlite3';
import type { Logger } from '../utils/logger.js';

export interface SkillRecord {
  id: string;
  name: string;
  description: string;
  version: number;
  author: string;
  tags: string[];
  userInvocable: boolean;
  context?: string;
  allowedTools?: string;
  skillMd: string;
  hasReferences: boolean;
  publishedAt: string;
  updatedAt: string;
}

export interface SkillSummary {
  id: string;
  name: string;
  description: string;
  version: number;
  author: string;
  tags: string[];
  publishedAt: string;
  updatedAt: string;
}

export interface SkillSearchResult extends SkillSummary {
  snippet: string;
}

export interface SkillPublishInput {
  name: string;
  skillMd: string;
  referencesTar?: Buffer;
  author?: string;
}

/**
 * Parse YAML-like frontmatter from SKILL.md content.
 * Extracts key: value pairs between --- delimiters.
 */
function parseFrontmatter(content: string): Record<string, string> {
  const meta: Record<string, string> = {};
  const match = content.match(/^---\s*\n([\s\S]*?)\n---/);
  if (!match) return meta;
  for (const line of match[1].split('\n')) {
    const idx = line.indexOf(':');
    if (idx > 0) {
      const key = line.slice(0, idx).trim();
      let value = line.slice(idx + 1).trim();
      // Strip surrounding quotes
      if ((value.startsWith('"') && value.endsWith('"')) || (value.startsWith("'") && value.endsWith("'"))) {
        value = value.slice(1, -1);
      }
      meta[key] = value;
    }
  }
  return meta;
}

export class SkillHubStore {
  private db: Database.Database;
  private logger: Logger;

  constructor(databaseDir: string, logger: Logger) {
    this.logger = logger.child({ module: 'skill-hub' });
    fs.mkdirSync(databaseDir, { recursive: true });
    const dbPath = path.join(databaseDir, 'skill-hub.db');
    this.db = new Database(dbPath);
    this.db.pragma('journal_mode = WAL');
    this.initSchema();
    this.logger.info({ dbPath }, 'Skill Hub store initialized');
  }

  private initSchema(): void {
    this.db.exec(`
      CREATE TABLE IF NOT EXISTS skills (
        id              TEXT PRIMARY KEY,
        name            TEXT NOT NULL UNIQUE,
        description     TEXT NOT NULL DEFAULT '',
        version         INTEGER NOT NULL DEFAULT 1,
        author          TEXT NOT NULL DEFAULT '',
        tags            TEXT NOT NULL DEFAULT '[]',
        user_invocable  INTEGER NOT NULL DEFAULT 1,
        context         TEXT,
        allowed_tools   TEXT,
        skill_md        TEXT NOT NULL,
        references_tar  BLOB,
        published_at    TEXT NOT NULL,
        updated_at      TEXT NOT NULL
      );
    `);

    // Create FTS5 virtual table if not exists
    // Check if table already exists to avoid errors on restart
    const ftsExists = this.db.prepare(
      "SELECT name FROM sqlite_master WHERE type='table' AND name='skills_fts'",
    ).get();

    if (!ftsExists) {
      this.db.exec(`
        CREATE VIRTUAL TABLE skills_fts USING fts5(
          name, description, tags, skill_md,
          content='skills',
          content_rowid='rowid'
        );

        CREATE TRIGGER skills_ai AFTER INSERT ON skills BEGIN
          INSERT INTO skills_fts(rowid, name, description, tags, skill_md)
          VALUES (new.rowid, new.name, new.description, new.tags, new.skill_md);
        END;

        CREATE TRIGGER skills_au AFTER UPDATE ON skills BEGIN
          INSERT INTO skills_fts(skills_fts, rowid, name, description, tags, skill_md)
          VALUES ('delete', old.rowid, old.name, old.description, old.tags, old.skill_md);
          INSERT INTO skills_fts(rowid, name, description, tags, skill_md)
          VALUES (new.rowid, new.name, new.description, new.tags, new.skill_md);
        END;

        CREATE TRIGGER skills_ad AFTER DELETE ON skills BEGIN
          INSERT INTO skills_fts(skills_fts, rowid, name, description, tags, skill_md)
          VALUES ('delete', old.rowid, old.name, old.description, old.tags, old.skill_md);
        END;
      `);
    }
  }

  /** Publish or update a skill. Returns the skill record. */
  publish(input: SkillPublishInput): SkillRecord {
    const meta = parseFrontmatter(input.skillMd);
    const name = input.name || meta['name'] || 'unnamed-skill';
    const description = meta['description'] || '';
    const tags = meta['tags'] ? meta['tags'].split(',').map((t) => t.trim()) : [];
    const userInvocable = meta['user-invocable'] !== 'false';
    const context = meta['context'] || undefined;
    const allowedTools = meta['allowed-tools'] || undefined;
    const now = new Date().toISOString();

    // Check if skill already exists (upsert)
    const existing = this.db.prepare('SELECT id, version FROM skills WHERE name = ?').get(name) as
      | { id: string; version: number }
      | undefined;

    if (existing) {
      this.db.prepare(`
        UPDATE skills SET
          description = ?, version = ?, author = ?, tags = ?,
          user_invocable = ?, context = ?, allowed_tools = ?,
          skill_md = ?, references_tar = ?, updated_at = ?
        WHERE name = ?
      `).run(
        description, existing.version + 1, input.author || '', JSON.stringify(tags),
        userInvocable ? 1 : 0, context || null, allowedTools || null,
        input.skillMd, input.referencesTar || null, now, name,
      );

      this.logger.info({ name, version: existing.version + 1 }, 'Skill updated');
      return this.get(name)!;
    }

    const id = crypto.randomUUID();
    this.db.prepare(`
      INSERT INTO skills (id, name, description, version, author, tags,
        user_invocable, context, allowed_tools, skill_md, references_tar,
        published_at, updated_at)
      VALUES (?, ?, ?, 1, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    `).run(
      id, name, description, input.author || '', JSON.stringify(tags),
      userInvocable ? 1 : 0, context || null, allowedTools || null,
      input.skillMd, input.referencesTar || null, now, now,
    );

    this.logger.info({ name, id }, 'Skill published');
    return this.get(name)!;
  }

  /** Get a skill by name (full record including SKILL.md content). */
  get(name: string): SkillRecord | undefined {
    const row = this.db.prepare('SELECT * FROM skills WHERE name = ?').get(name) as any;
    if (!row) return undefined;
    return this.rowToRecord(row);
  }

  /** Get skill content for installation (SKILL.md + optional references tar). */
  getContent(name: string): { skillMd: string; referencesTar?: Buffer } | undefined {
    const row = this.db.prepare('SELECT skill_md, references_tar FROM skills WHERE name = ?').get(name) as any;
    if (!row) return undefined;
    return {
      skillMd: row.skill_md,
      referencesTar: row.references_tar || undefined,
    };
  }

  /** List all skills (summary only, no SKILL.md content). */
  list(): SkillSummary[] {
    const rows = this.db.prepare(
      'SELECT id, name, description, version, author, tags, published_at, updated_at FROM skills ORDER BY updated_at DESC',
    ).all() as any[];
    return rows.map((row) => ({
      id: row.id,
      name: row.name,
      description: row.description,
      version: row.version,
      author: row.author,
      tags: JSON.parse(row.tags || '[]'),
      publishedAt: row.published_at,
      updatedAt: row.updated_at,
    }));
  }

  /** Full-text search across skill name, description, tags, and content. */
  search(query: string): SkillSearchResult[] {
    const escaped = this.escapeFts5Query(query);
    if (!escaped) return this.list().map((s) => ({ ...s, snippet: '' }));

    const rows = this.db.prepare(`
      SELECT s.id, s.name, s.description, s.version, s.author, s.tags,
             s.published_at, s.updated_at,
             snippet(skills_fts, 3, '<b>', '</b>', '...', 32) AS snippet
      FROM skills_fts f
      JOIN skills s ON s.rowid = f.rowid
      WHERE skills_fts MATCH ?
      ORDER BY rank
    `).all(escaped) as any[];

    return rows.map((row) => ({
      id: row.id,
      name: row.name,
      description: row.description,
      version: row.version,
      author: row.author,
      tags: JSON.parse(row.tags || '[]'),
      publishedAt: row.published_at,
      updatedAt: row.updated_at,
      snippet: row.snippet || '',
    }));
  }

  /** Remove a skill by name. Returns true if removed. */
  remove(name: string): boolean {
    const result = this.db.prepare('DELETE FROM skills WHERE name = ?').run(name);
    if (result.changes > 0) {
      this.logger.info({ name }, 'Skill removed');
      return true;
    }
    return false;
  }

  private rowToRecord(row: any): SkillRecord {
    return {
      id: row.id,
      name: row.name,
      description: row.description,
      version: row.version,
      author: row.author,
      tags: JSON.parse(row.tags || '[]'),
      userInvocable: row.user_invocable === 1,
      context: row.context || undefined,
      allowedTools: row.allowed_tools || undefined,
      skillMd: row.skill_md,
      hasReferences: !!row.references_tar,
      publishedAt: row.published_at,
      updatedAt: row.updated_at,
    };
  }

  /** Escape a user query for FTS5 — wrap each token in double-quotes. */
  private escapeFts5Query(query: string): string {
    return query
      .split(/\s+/)
      .filter(Boolean)
      .map((token) => `"${token.replace(/"/g, '')}"`)
      .join(' ');
  }
}
