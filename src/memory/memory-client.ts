import type { Logger } from '../utils/logger.js';

export interface FolderTreeNode {
  id: string;
  name: string;
  path: string;
  children: FolderTreeNode[];
  document_count: number;
}

export interface DocumentSummary {
  id: string;
  title: string;
  path: string;
  tags: string[];
  created_by: string;
  updated_at: string;
}

export interface SearchResult {
  id: string;
  title: string;
  path: string;
  snippet: string;
  tags: string[];
  updated_at: string;
}

export interface HealthStatus {
  status: string;
  document_count: number;
  folder_count: number;
}

export class MemoryClient {
  constructor(
    private baseUrl: string,
    private logger: Logger,
  ) {}

  private async request<T>(path: string, options?: RequestInit): Promise<T> {
    const url = `${this.baseUrl}${path}`;
    const res = await fetch(url, {
      headers: { 'Content-Type': 'application/json', ...options?.headers },
      ...options,
    });
    if (!res.ok) {
      const body = await res.text().catch(() => '');
      throw new Error(`Memory API ${res.status}: ${body}`);
    }
    return res.json() as Promise<T>;
  }

  async health(): Promise<HealthStatus> {
    return this.request<HealthStatus>('/api/health');
  }

  async listFolderTree(): Promise<FolderTreeNode> {
    return this.request<FolderTreeNode>('/api/folders');
  }

  async listDocuments(folderId?: string, limit = 50): Promise<DocumentSummary[]> {
    const params = new URLSearchParams();
    if (folderId) params.set('folder_id', folderId);
    params.set('limit', String(limit));
    return this.request<DocumentSummary[]>(`/api/documents?${params}`);
  }

  async search(query: string, limit = 20): Promise<SearchResult[]> {
    return this.request<SearchResult[]>(`/api/search?q=${encodeURIComponent(query)}&limit=${limit}`);
  }

  /** Format folder tree as indented text for Feishu card display */
  formatFolderTree(node: FolderTreeNode, depth = 0): string {
    const indent = '  '.repeat(depth);
    const icon = node.children.length > 0 ? '📂' : '📁';
    const count = node.document_count > 0 ? ` (${node.document_count})` : '';
    let result = `${indent}${icon} ${node.name}${count}\n`;
    for (const child of node.children) {
      result += this.formatFolderTree(child, depth + 1);
    }
    return result;
  }

  /** Format search results as text for Feishu card display */
  formatSearchResults(results: SearchResult[]): string {
    if (results.length === 0) return 'No results found.';
    return results.map((r, i) => {
      const tags = r.tags.length > 0 ? ` [${r.tags.join(', ')}]` : '';
      // Strip HTML tags from snippet
      const snippet = r.snippet.replace(/<[^>]*>/g, '');
      return `${i + 1}. **${r.title}**${tags}\n   ${snippet}`;
    }).join('\n\n');
  }
}
