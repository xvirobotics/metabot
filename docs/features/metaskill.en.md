# MetaSkill

Agent factory. Generate complete agent teams, individual agents, or custom skills with a single command.

## What It Does

`/metaskill` researches best practices, then generates a complete `.claude/` agent configuration:

- **Orchestrator** — Main agent that coordinates the team
- **Specialists** — Domain-specific agents (frontend, backend, infra, etc.)
- **Code Reviewer** — Reviews PRs and code quality
- **Skills** — Custom skills that extend agent capabilities

The generated agents use MetaMemory for shared knowledge across sessions.

## Usage

Send `/metaskill` followed by your request in chat:

```
/metaskill Create an agent team for this React Native project —
I need a frontend specialist, a backend API specialist, and a code reviewer.
```

```
/metaskill Create a skill that reads our Jira board and summarizes
open tickets.
```

## How It Works

1. Claude researches the project structure and best practices
2. Generates `.claude/` configuration files (agents, skills, settings)
3. Saves the configuration to the bot's working directory
4. The new agents/skills are immediately available for use

## Output

MetaSkill generates files in the `.claude/` directory:

```
.claude/
├── agents/
│   ├── orchestrator.md
│   ├── frontend.md
│   ├── backend.md
│   └── reviewer.md
├── skills/
│   └── custom-skill/
│       └── SKILL.md
└── settings.json
```
