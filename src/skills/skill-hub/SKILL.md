---
name: skill-hub
description: "Discover, search, and install shared skills from the Skill Hub registry. Use when the user wants to find available skills, share a skill with other bots, or install a skill from the hub."
user-invocable: true
context: fork
allowed-tools: Bash, Read, Grep, Glob
argument-hint: "[list|search|publish|install] [args]"
---

# Skill Hub — Shared Skill Registry

Discover and install skills shared across MetaBot instances and bots.

## Quick Commands (mb shortcut)

The `mb` CLI handles auth automatically. **Always prefer `mb` over raw curl:**

```bash
# Browse available skills
mb skills                              # List all skills (local + peer)
mb skills search <query>               # Search by keyword

# Get skill details
mb skills get <name>                   # View full skill info and SKILL.md content

# Install a skill to a bot
mb skills install <skillName> <botName>       # Install from local hub
mb skills install <skillName> <botName> peer:<peerName>  # Install from peer

# Publish a skill (share with others)
mb skills publish <botName> <skillName>  # Publish a bot's skill to the hub

# Remove a skill from the hub
mb skills remove <name>                # Unpublish
```

## When to Use
- User asks "what skills are available?"
- User wants to install a skill to a bot
- User wants to share/publish a skill for others to use
- User asks to find skills for a specific purpose (e.g., "is there a skill for spreadsheets?")
- Cross-bot skill sharing: one bot has a useful skill that another bot needs

## Workflow Examples

### Finding and installing a skill
```bash
mb skills search "calendar"             # Find calendar-related skills
mb skills get lark-calendar             # See details
mb skills install lark-calendar mybot   # Install to mybot
```

### Publishing a bot's skill
```bash
mb skills publish whis data-analysis    # Share whis's data-analysis skill
mb skills                               # Verify it's listed
```

### Installing from a peer instance
```bash
mb skills                               # See all skills including peer skills
mb skills install data-viz mybot peer:alice  # Install from peer "alice"
```

## Guidelines
- Before publishing, ensure the skill is well-documented with clear SKILL.md
- Use descriptive names and tags for discoverability
- Search before creating to avoid duplicates
- Published skills are available to all bots on all connected instances
