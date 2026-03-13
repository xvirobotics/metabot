# Output Files

When Claude produces output files (images, PDFs, documents, etc.), they are automatically sent to the user in chat.

## How It Works

1. **Per-chat outputs directory** — Before each execution, a fresh directory is created at `/tmp/metabot-outputs/<chatId>/`
2. **System prompt injection** — Claude is told to `cp` output files to this directory
3. **Post-execution scan** — After execution completes, the bridge scans the directory and sends all files found
4. **File type routing** — Images are uploaded via the image API, other files via the file API

## File Type Support

| Type | Extensions | Feishu API | Size Limit |
|------|-----------|------------|------------|
| Images | png, jpg, gif, webp, bmp, tiff | `im.v1.image.create` | 10 MB |
| Files | pdf, docx, zip, xlsx, pptx, etc. | `im.v1.file.create` | 30 MB |

## Configuration

| Variable | Default | Description |
|----------|---------|-------------|
| `outputsBaseDir` | `/tmp/metabot-outputs` | Base directory for output files (per-bot in `bots.json`) |

## Fallback

The legacy image detection method (tracking `Write` tool file paths + response text regex) still works as a fallback for images not placed in the outputs directory.
