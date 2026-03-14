# Voice Assistant (Jarvis Mode)

Talk to any MetaBot agent hands-free using AirPods and Siri. No app needed — just an iOS Shortcut.

## How It Works

```
"Hey Siri, Jarvis"
        ↓
  Siri dictates your voice → text
        ↓
  HTTP POST to MetaBot /api/talk
        ↓
  Agent executes (Claude Code)
        ↓
  Response spoken back through AirPods
```

Zero screen interaction. Works while walking, hiking, driving.

## Setup (5 minutes)

### Prerequisites

- iPhone with Siri enabled
- AirPods (or any earphones with Siri support)
- MetaBot server accessible from the internet (public IP + port 9100 open)
- Your `API_SECRET` from MetaBot's `.env`

### Step 1: Create the Shortcut

Open **Shortcuts** app on iPhone → tap **+** → name it **Jarvis**.

### Step 2: Add "Dictate Text"

Search and add the **Dictate Text** action:
- **Language**: Chinese (China) — or your preferred language
- **Stop listening**: After pause

### Step 3: Add "Get Contents of URL"

Search and add **Get Contents of URL**:

- **URL**: `http://YOUR_SERVER_IP:9100/api/talk`
- **Method**: `POST`
- **Headers**:
  - `Authorization` = `Bearer YOUR_API_SECRET`
  - `Content-Type` = `application/json`
- **Request Body**: `JSON`
  - `botName` → `quanwang` (or any bot name, text)
  - `chatId` → `voice_jarvis` (text — this creates a persistent session)
  - `prompt` → select the **Dictated Text** variable from step 2

### Step 4: Add "Get Dictionary Value"

Search and add **Get Dictionary Value**:
- Get **Value** for key `responseText`
- From: **Contents of URL** (previous step)

### Step 5: Add "Speak Text"

Search and add **Speak Text**:
- Input: **Dictionary Value** (previous step)

### Step 6: Test

1. Put on AirPods
2. Say **"Hey Siri, Jarvis"**
3. Wait for the dictation prompt, then speak your command
4. Wait a few seconds — the response will be spoken back

## Tips

### Talk to different bots

Create multiple shortcuts with different `botName` values:
- **"Hey Siri, Jarvis"** → `quanwang` (general assistant)
- **"Hey Siri, Goku"** → `goku` (motion control specialist)
- **"Hey Siri, Backend"** → `backend-bot` (backend developer)

### Persistent sessions

The `chatId` field (`voice_jarvis`) creates a persistent Claude session, just like a Feishu chat. Multi-turn conversations work — the agent remembers previous context.

Use different `chatId` values for different conversation threads:
- `voice_jarvis` — general tasks
- `voice_code_review` — code review sessions
- `voice_research` — research tasks

### Remote peers

If the bot is on a remote peer instance, use the qualified name syntax:
- `botName` = `lanqi/some-bot` — routes to the `lanqi` peer automatically

### Feishu cards

Set `sendCards` to `true` (boolean) in the JSON body if you also want to see the response as a Feishu card in your chat. Useful for code-heavy responses you want to read later.

## Limitations

- Each interaction requires saying "Hey Siri, Jarvis" again (no continuous conversation loop)
- Siri's dictation may truncate very long voice input
- Long agent responses (code, detailed analysis) are better consumed as text in Feishu
- Requires internet connectivity for both Siri STT and MetaBot API

## Security

- The API endpoint should be protected with `API_SECRET` (Bearer token auth)
- Consider using HTTPS (reverse proxy with Let's Encrypt) for production
- The `chatId` is fixed in the shortcut, so anyone with access to your phone could use it
