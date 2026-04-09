# Example Prompts

Real prompts you can send in Feishu/Telegram to unlock MetaBot's advanced features.

## MetaMemory — Persistent Knowledge

```
Remember the deployment guide we just discussed — save it to MetaMemory
under /projects/deployment.
```

```
Search MetaMemory for our API design conventions.
```

```
Summarize today's code review findings and save them to MetaMemory
for the team to reference later.
```

## MetaSkill — Agent & Skill Factory

```
/metaskill Create an agent team for this React Native project —
I need a frontend specialist, a backend API specialist, and a code reviewer.
```

```
/metaskill Create a skill that reads our Jira board and summarizes
open tickets.
```

## Scheduling — Automated Tasks

```
Schedule a daily task at 9am: search Hacker News and TechCrunch for AI news,
summarize the top 5 stories, and save the summary to MetaMemory.
```

```
Remind me in 30 minutes to check if the deployment succeeded.
```

```
Set up a weekly Monday 8am task: review last week's git commits, generate
a progress report, and save it to MetaMemory under /reports.
```

## Agent-to-Agent — Task Delegation

```
Delegate this bug fix to backend-bot: "Fix the null pointer exception
in /api/users/:id endpoint, see the error log in MetaMemory /logs/errors".
```

```
Ask frontend-bot to update the dashboard UI, and at the same time
ask backend-bot to add the new API endpoint. Both should save their
progress to MetaMemory under /projects/dashboard.
```

## Combined Workflows

```
Research best practices for WebSocket authentication, create a detailed
implementation plan, then save the plan to MetaMemory under
/architecture/websocket-auth for the team to review.
```

```
Read this Feishu doc [paste URL], extract the product requirements, break
them into tasks, and schedule a daily standup summary at 6pm that tracks
progress against these requirements.
```

```
/metaskill Create a "daily-ops" agent that runs every morning at 8am:
checks service health, reviews overnight error logs, and posts a summary.
```
