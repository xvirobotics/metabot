# fd CLI (Feishu Document Reader)

The `fd` command reads Feishu documents and converts them to Markdown. Only available for Feishu bots.

## Installation

Installed automatically by the MetaBot installer to `~/.local/bin/fd`.

## Commands

```bash
fd read <feishu-url>                # read document by URL (docx or wiki)
fd read-id <docId>                  # read document by ID
fd info <feishu-url>                # get document metadata
```

## Options

| Flag | Description |
|------|-------------|
| `--bot <name>` | Specify which bot's credentials to use |

## Supported URL Formats

- `https://xxx.feishu.cn/docx/xxxxx` — Standalone docx
- `https://xxx.feishu.cn/wiki/xxxxx` — Wiki page
- `https://xxx.larksuite.com/docx/xxxxx` — Lark docx
- `https://xxx.larksuite.com/wiki/xxxxx` — Lark wiki

## Example

```bash
fd read https://your-org.feishu.cn/docx/ABC123
```

Output is Markdown text printed to stdout.
