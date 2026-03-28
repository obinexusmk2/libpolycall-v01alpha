# Trinary Consensus Canonical State Model

This document defines the core-facing canonical state model for telemetry consensus events.

## Canonical States

The consensus state is strict and trinary:

- `YES`
- `NO`
- `MAYBE`

`MAYBE` is first-class and MUST NOT be mapped to `unknown`, `error`, or omitted.

## Required Event Fields

All bindings and adapters MUST emit the same field names:

| Field | Type | Required | Description |
| --- | --- | --- | --- |
| `event_type` | string | yes | Must be `telemetry.consensus`. |
| `state` | enum | yes | One of `YES`, `NO`, `MAYBE`. |
| `session_id` | string | yes | Session correlation identifier. |
| `ack_status` | string | yes | Delivery acknowledgement state (`pending`, `accepted`, `rejected`, etc.). |
| `persisted` | boolean | yes | Whether the event has been durably persisted. |
| `storage_key` | string/null | yes | Persistence key/location when persisted, otherwise null. |
| `timestamp` | RFC3339 string | yes | Event creation time in UTC. |

## CLI Surface (Binding Contract)

Bindings MUST support the same command surface and flag names:

```bash
telemetry consensus --state yes|no|maybe --session <id>
```

Optional stable flags:

- `--ack-status <status>`
- `--persisted`
- `--storage-key <key>`

## Shared Schema

The machine-readable schema is maintained at:

- `docs/schema/telemetry-consensus.schema.json`
