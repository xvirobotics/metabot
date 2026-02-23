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

### API Quick Reference

**List folder tree:**
```bash
curl -s $MEMORY_SERVER_URL/api/folders
```

**Search documents:**
```bash
curl -s "$MEMORY_SERVER_URL/api/search?q=QUERY"
```

**Get document:**
```bash
curl -s $MEMORY_SERVER_URL/api/documents/DOC_ID
```

**Get document by path:**
```bash
curl -s "$MEMORY_SERVER_URL/api/documents/by-path?path=/folder/doc-slug"
```

**List documents in folder:**
```bash
curl -s "$MEMORY_SERVER_URL/api/documents?folder_id=FOLDER_ID&limit=50"
```

**Create document:**
```bash
curl -s -X POST $MEMORY_SERVER_URL/api/documents \
  -H "Content-Type: application/json" \
  -d '{"title":"Doc Title", "folder_id":"root", "content":"# Markdown content...", "tags":["tag1"], "created_by":"bot-name"}'
```

**Update document:**
```bash
curl -s -X PUT $MEMORY_SERVER_URL/api/documents/DOC_ID \
  -H "Content-Type: application/json" \
  -d '{"content":"# Updated content..."}'
```

**Create folder:**
```bash
curl -s -X POST $MEMORY_SERVER_URL/api/folders \
  -H "Content-Type: application/json" \
  -d '{"name":"folder-name", "parent_id":"root"}'
```

**Delete document:**
```bash
curl -s -X DELETE $MEMORY_SERVER_URL/api/documents/DOC_ID
```

**Health check:**
```bash
curl -s $MEMORY_SERVER_URL/api/health
```

### Guidelines
- Write documents as structured Markdown with clear headings
- Use descriptive titles and relevant tags
- Organize into folders by project or topic
- Before creating, search first to avoid duplicates
- Update existing docs rather than creating new ones when appropriate
- Include created_by to track which agent wrote the document
