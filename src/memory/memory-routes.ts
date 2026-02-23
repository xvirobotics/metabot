import type { MemoryStorage, DocumentCreateInput, DocumentUpdateInput } from './memory-storage.js';

interface RouteResult {
  status: number;
  body: unknown;
}

// --- Folder routes ---

export function handleGetFolders(storage: MemoryStorage): RouteResult {
  const tree = storage.getFolderTree();
  return { status: 200, body: tree };
}

export function handleCreateFolder(storage: MemoryStorage, body: Record<string, unknown>): RouteResult {
  const name = body.name as string | undefined;
  if (!name) {
    return { status: 400, body: { detail: 'name is required' } };
  }
  const parentId = (body.parent_id as string) || 'root';
  try {
    const folder = storage.createFolder(name, parentId);
    return { status: 201, body: folder };
  } catch (err: any) {
    return { status: 400, body: { detail: err.message } };
  }
}

export function handleDeleteFolder(storage: MemoryStorage, folderId: string): RouteResult {
  try {
    storage.deleteFolder(folderId);
    return { status: 200, body: { ok: true } };
  } catch (err: any) {
    if (err.message.includes('Cannot delete root')) {
      return { status: 400, body: { detail: err.message } };
    }
    if (err.message.includes('not found')) {
      return { status: 404, body: { detail: err.message } };
    }
    return { status: 400, body: { detail: err.message } };
  }
}

// --- Document routes ---

export function handleListDocuments(
  storage: MemoryStorage,
  query: URLSearchParams,
): RouteResult {
  const folderId = query.get('folder_id') || undefined;
  const limit = Math.min(Math.max(parseInt(query.get('limit') || '50', 10) || 50, 1), 200);
  const offset = Math.max(parseInt(query.get('offset') || '0', 10) || 0, 0);
  const docs = storage.listDocuments(folderId, limit, offset);
  return { status: 200, body: docs };
}

export function handleGetDocument(storage: MemoryStorage, docId: string): RouteResult {
  const doc = storage.getDocument(docId);
  if (!doc) return { status: 404, body: { detail: 'Document not found' } };
  return { status: 200, body: doc };
}

export function handleGetDocumentByPath(
  storage: MemoryStorage,
  query: URLSearchParams,
): RouteResult {
  const docPath = query.get('path');
  if (!docPath) return { status: 400, body: { detail: 'path query parameter is required' } };
  const doc = storage.getDocumentByPath(docPath);
  if (!doc) return { status: 404, body: { detail: 'Document not found' } };
  return { status: 200, body: doc };
}

export function handleCreateDocument(
  storage: MemoryStorage,
  body: Record<string, unknown>,
): RouteResult {
  const title = body.title as string | undefined;
  if (!title) {
    return { status: 400, body: { detail: 'title is required' } };
  }

  const data: DocumentCreateInput = {
    title,
    folder_id: (body.folder_id as string) || 'root',
    content: (body.content as string) || '',
    tags: Array.isArray(body.tags) ? body.tags : [],
    created_by: (body.created_by as string) || '',
  };

  try {
    const doc = storage.createDocument(data);
    return { status: 201, body: doc };
  } catch (err: any) {
    return { status: 400, body: { detail: err.message } };
  }
}

export function handleUpdateDocument(
  storage: MemoryStorage,
  docId: string,
  body: Record<string, unknown>,
): RouteResult {
  const data: DocumentUpdateInput = {};
  if (body.title !== undefined) data.title = body.title as string;
  if (body.content !== undefined) data.content = body.content as string;
  if (body.tags !== undefined) data.tags = Array.isArray(body.tags) ? body.tags : [];
  if (body.folder_id !== undefined) data.folder_id = body.folder_id as string;

  const doc = storage.updateDocument(docId, data);
  if (!doc) return { status: 404, body: { detail: 'Document not found' } };
  return { status: 200, body: doc };
}

export function handleDeleteDocument(storage: MemoryStorage, docId: string): RouteResult {
  const deleted = storage.deleteDocument(docId);
  if (!deleted) return { status: 404, body: { detail: 'Document not found' } };
  return { status: 200, body: { ok: true } };
}

// --- Search ---

export function handleSearch(
  storage: MemoryStorage,
  query: URLSearchParams,
): RouteResult {
  const q = query.get('q');
  if (!q || q.trim().length === 0) {
    return { status: 400, body: { detail: 'q query parameter is required' } };
  }
  const limit = Math.min(Math.max(parseInt(query.get('limit') || '20', 10) || 20, 1), 100);
  const results = storage.searchDocuments(q, limit);
  return { status: 200, body: results };
}

// --- Health ---

export function handleHealth(storage: MemoryStorage): RouteResult {
  const stats = storage.getStats();
  return { status: 200, body: { status: 'ok', ...stats } };
}
