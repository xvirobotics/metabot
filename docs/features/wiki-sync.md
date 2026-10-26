# Wiki Sync

One-way sync from MetaMemory documents to a Feishu Wiki space. The folder tree in MetaMemory maps to wiki nodes; each document becomes a Feishu docx page.

## Overview

When enabled, MetaMemory content automatically syncs to a Feishu Wiki space:

- **Folder tree** maps to wiki node hierarchy
- **Documents** become Feishu docx pages
- **Change detection** uses hash comparison for incremental sync
- **Auto-sync** triggers on MetaMemory changes (5-second debounce)

## Chat Commands

| Command | Description |
|---------|-------------|
| `/sync` | Trigger full sync |
| `/sync status` | Show sync statistics |

## Configuration

| Variable | Default | Description |
|----------|---------|-------------|
| `WIKI_SYNC_ENABLED` | `true` | Enable wiki sync |
| `WIKI_SPACE_ID` | — | Feishu Wiki space ID |
| `WIKI_SPACE_NAME` | `MetaMemory` | Wiki space name (created if not exists) |
| `WIKI_AUTO_SYNC` | `true` | Auto-sync on MetaMemory changes |
| `WIKI_AUTO_SYNC_DEBOUNCE_MS` | `5000` | Debounce delay |
| `WIKI_SYNC_THROTTLE_MS` | `300` | Delay between API calls |
| `FEISHU_SERVICE_APP_ID` | — | Dedicated Feishu app for sync (falls back to first bot) |
| `FEISHU_SERVICE_APP_SECRET` | — | Service app secret |
| `FEISHU_SERVICE_DOMAIN` | `feishu` | Domain of the dedicated service app: `feishu` or `lark` |

### Lark (international)

Wiki sync calls the Open Platform domain of the service app:

- Without a dedicated service app, sync reuses the first Feishu bot's app and its `larkDomain`. If that bot sets `"larkDomain": "lark"`, sync requests go to `open.larksuite.com`.
- With `FEISHU_SERVICE_APP_ID`, set `FEISHU_SERVICE_DOMAIN=lark` when that app was created in the Lark developer console; otherwise it is treated as Feishu.
- `FEISHU_SERVICE_DOMAIN` accepts only `feishu` or `lark`; any other value fails startup. Without a dedicated service app, a value that contradicts the first bot's domain also fails startup, so one app's credentials are never sent to the other domain.

## Required Feishu Permissions

Add these in the Feishu Developer Console:

- `wiki:wiki` — Read/write wiki pages
- `docx:document` — Create/edit documents
- `docx:document:readonly` — Read documents
- `drive:drive` — Access drive files

## Auto-Sync Behavior

- Changes trigger sync after a 5-second debounce
- Multiple rapid changes are coalesced
- 1-10 document changes → incremental sync
- Bulk changes or folder structure changes → full sync fallback
- Manual `/sync` is always available

## API

| Method | Path | Description |
|--------|------|-------------|
| `POST` | `/api/sync` | Trigger full sync |
| `GET` | `/api/sync` | Sync status |
| `POST` | `/api/sync/document` | Sync single document by ID |
