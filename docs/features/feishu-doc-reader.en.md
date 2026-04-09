# Feishu Document Reader

Read Feishu documents (standalone docx and wiki pages) and convert them to Markdown.

## Overview

When users share a Feishu document URL in chat, Claude can automatically read its contents. The reader supports:

- **Standalone docx documents**
- **Wiki pages**
- **Block types**: headings, code blocks, lists, tables, quotes, todos, inline formatting

## Usage

Share a Feishu URL in chat and Claude will read it automatically. Or use the CLI:

```bash
fd read <feishu-url>                # read document by URL
fd read-id <docId>                  # read document by ID
fd info <feishu-url>                # get document metadata
```

## Supported URL Formats

- `https://xxx.feishu.cn/docx/xxxxx` — Standalone docx
- `https://xxx.feishu.cn/wiki/xxxxx` — Wiki page
- `https://xxx.larksuite.com/docx/xxxxx` — Lark docx
- `https://xxx.larksuite.com/wiki/xxxxx` — Lark wiki

## API

```
GET /api/feishu/document?url=<feishu-url>&botName=<name>
GET /api/feishu/document?docId=<id>&botName=<name>
```

## Configuration

| Variable | Description |
|----------|-------------|
| `FEISHU_SERVICE_APP_ID` | Dedicated Feishu app (falls back to first bot) |
| `FEISHU_SERVICE_APP_SECRET` | Service app secret |

## Required Feishu Permissions

- `docx:document:readonly` — Read documents
- `wiki:wiki` — Read wiki pages (for wiki URLs)
