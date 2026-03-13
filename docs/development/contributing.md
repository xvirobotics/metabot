# Contributing

Thanks for your interest in contributing to MetaBot!

## Development Setup

```bash
# 1. Clone the repo
git clone https://github.com/xvirobotics/metabot.git
cd metabot

# 2. Install dependencies
npm install

# 3. Copy environment config
cp .env.example .env
# Edit .env with your credentials

# 4. Build
npm run build

# 5. Run in development
npm run dev
```

**Prerequisites:** Node.js 20+, Claude Code CLI installed and authenticated.

## Development Commands

```bash
npm run dev          # Hot-reload dev server (tsx)
npm test             # Run tests (vitest)
npm run lint         # ESLint check
npm run format       # Prettier format
npm run build        # TypeScript compile to dist/
```

## How to Contribute

### Reporting Bugs

- Use the [Bug Report](https://github.com/xvirobotics/metabot/issues/new?template=bug_report.md) template
- Include logs (redact sensitive info) and steps to reproduce

### Suggesting Features

- Use the [Feature Request](https://github.com/xvirobotics/metabot/issues/new?template=feature_request.md) template
- Describe the use case, not just the solution

### Submitting Pull Requests

1. Fork the repo and create a branch from `main`
2. Make your changes with clear commit messages
3. Ensure `npm run build` passes with no errors
4. Run `npm test` and `npm run lint`
5. Open a PR with a clear description of what changed and why

## Code Style

- TypeScript strict mode
- Use `async/await` over raw promises
- Keep functions small and focused
- ESM imports with `.js` extensions
