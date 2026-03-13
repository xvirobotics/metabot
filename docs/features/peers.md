# Peers Federation

Cross-instance bot discovery and task routing. Connect multiple MetaBot instances — on the same machine or across remote servers.

## Overview

Peers enables a **federated architecture** where multiple MetaBot instances discover each other's bots and route tasks automatically. This is useful when:

- Multiple users on the same machine run separate MetaBot instances
- Teams deploy MetaBot on different servers
- You want to share specialized bots across environments

## How It Works

1. **Discovery** — Each instance periodically polls its peers' `GET /api/bots` endpoint (every 30 seconds)
2. **Caching** — Bot lists are cached locally for fast lookups
3. **Routing** — When a bot name isn't found locally, the request is forwarded to the peer that has it
4. **Anti-loop** — Forwarded requests carry `X-MetaBot-Origin` header to prevent circular delegation
5. **Anti-transitive** — Bots that are themselves from a peer are filtered out (no transitive forwarding)

## Configuration

=== "bots.json"

    ```json
    {
      "feishuBots": [{ "..." }],
      "peers": [
        {
          "name": "alice",
          "url": "http://localhost:9200",
          "secret": "alice-api-secret"
        },
        {
          "name": "bob",
          "url": "http://192.168.1.50:9100",
          "secret": "bob-api-secret"
        }
      ]
    }
    ```

=== "Environment Variables"

    ```bash
    METABOT_PEERS=http://localhost:9200,http://192.168.1.50:9100
    METABOT_PEER_SECRETS=alice-secret,bob-secret
    METABOT_PEER_NAMES=alice,bob
    METABOT_PEER_POLL_INTERVAL_MS=30000
    ```

The `secret` field is the peer's `API_SECRET` — needed if the peer has authentication enabled.

Peer names are optional. If not specified, they are auto-derived from the URL (e.g., `localhost-9200`).

## Qualified Names

Use `peerName/botName` syntax for precise routing:

```bash
# Auto-routing — searches local first, then peers in order
mb talk backend-bot chatId "fix the bug"

# Explicit peer — routes directly to alice's backend-bot
mb talk alice/backend-bot chatId "fix the bug"
```

## API

| Method | Path | Description |
|--------|------|-------------|
| `GET` | `/api/peers` | List peers and their health status |
| `GET` | `/api/bots` | List all bots (local + peer) |
| `POST` | `/api/talk` | Talk to a bot (auto-routes to peers) |

## CLI

```bash
mb peers                            # list peers and status
mb bots                             # list all bots (includes peer bots)
mb talk alice/bot chatId "prompt"    # talk to a specific peer's bot
```

## Health Monitoring

Each peer is polled every 30 seconds. The `GET /api/peers` endpoint returns health status:

```json
[
  {
    "name": "alice",
    "url": "http://localhost:9200",
    "healthy": true,
    "lastChecked": 1710000000000,
    "lastHealthy": 1710000000000,
    "botCount": 3
  }
]
```

Unhealthy peers are retried on the next poll cycle. Their cached bot lists are cleared when they become unreachable.
