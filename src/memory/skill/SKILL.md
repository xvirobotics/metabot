---
name: metamemory
description: Read and write shared memory documents. Use this when you need to save knowledge, notes, research findings, or project context for future reference across sessions. Also use it to look up previously stored information.
---

## MetaMemory Document Server

A shared memory server stores documents as organized Markdown files in a folder tree.
Server URL: !`echo ${MEMORY_SERVER_URL:-http://localhost:8100}`

### When to Use
- User asks to "remember", "save", "note down" something
- You discover important project knowledge worth preserving
- You need context from previous sessions or other agents' work
- After completing research or analysis that should be shared

### Quick Commands (mm shortcut)

The `mm` shell function is pre-installed and handles auth automatically. **Prefer `mm` over raw curl for read operations:**

```bash
mm search <query>       # Search documents
mm get <doc_id>         # Get document by ID
mm list [folder_id]     # List documents (default: root)
mm folders              # Browse folder tree
mm create <title> <md>  # Create a document (simple)
mm health               # Health check
```

### API Reference (for complex operations)

For create/update with full control (tags, folder, etc.), use the API directly.
Auth header: `-H "Authorization: Bearer $MEMORY_SECRET"`

**Create document:**
```bash
curl -s -X POST ${MEMORY_SERVER_URL:-http://localhost:8100}/api/documents \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $MEMORY_SECRET" \
  -d '{"title":"Doc Title", "folder_id":"root", "content":"# Markdown content...", "tags":["tag1"], "created_by":"bot-name"}'
```

**Update document:**
```bash
curl -s -X PUT ${MEMORY_SERVER_URL:-http://localhost:8100}/api/documents/DOC_ID \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $MEMORY_SECRET" \
  -d '{"content":"# Updated content..."}'
```

**Get document by path:**
```bash
curl -s -H "Authorization: Bearer $MEMORY_SECRET" "${MEMORY_SERVER_URL:-http://localhost:8100}/api/documents/by-path?path=/folder/doc-slug"
```

**Create folder:**
```bash
curl -s -X POST ${MEMORY_SERVER_URL:-http://localhost:8100}/api/folders \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $MEMORY_SECRET" \
  -d '{"name":"folder-name", "parent_id":"root"}'
```

**Delete document:**
```bash
curl -s -H "Authorization: Bearer $MEMORY_SECRET" -X DELETE ${MEMORY_SERVER_URL:-http://localhost:8100}/api/documents/DOC_ID
```

### Guidelines
- Write documents as structured Markdown with clear headings
- Use descriptive titles and relevant tags
- Organize into folders by project or topic
- Before creating, search first to avoid duplicates (`mm search <query>`)
- Update existing docs rather than creating new ones when appropriate
- Include created_by to track which agent wrote the document
