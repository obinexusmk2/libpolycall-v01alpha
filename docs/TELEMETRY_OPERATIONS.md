# PolyCall Telemetry Operations Guide

This guide covers operator workflows for the PolyCall CLI telemetry controls and the JSON telemetry event stream.

## Telemetry CLI Commands

From `libpolycall-v1` interactive mode:

```bash
./build/bin/polycall
```

### 1) Inspect telemetry status

```text
> telemetry status
Telemetry status:
  stream_enabled: false
  export_enabled: false
  export_path: (not configured)
  emitted_events: 0
```

### 2) Stream telemetry to stdout (JSON lines)

```text
> telemetry stream
Telemetry stream enabled (stdout JSON lines)
```

After enabling stream, runtime events are emitted as stable JSON records, for example:

```json
{"timestamp_ms":1711637405123,"event_type":"connection","component":"network","action":"connect","decision_state":"YES","status":"ok","subject":"127.0.0.1:58422","from_state":-1,"to_state":-1,"checksum":0,"consensus_ratio":0.000,"details":"client accepted"}
```

### 3) Export telemetry to a file

```text
> telemetry export /tmp/polycall-telemetry.jsonl
Telemetry export enabled: /tmp/polycall-telemetry.jsonl
```

Each line in `/tmp/polycall-telemetry.jsonl` is one machine-parseable JSON event.

## Event Schema (stable fields)

All emitted telemetry events use these field names:

- `timestamp_ms`
- `event_type` (`connection`, `transition`, `integrity`, `consensus`)
- `component`
- `action`
- `decision_state` (`YES`, `NO`, `MAYBE`)
- `status`
- `subject`
- `from_state`
- `to_state`
- `checksum`
- `consensus_ratio`
- `details`

## Event Coverage

The runtime currently emits:

- **Connection events** from `network.c` for connect/disconnect outcomes.
- **Transition events** from `polycall_state_machine.c` for transition execution results.
- **Integrity events** from `polycall_state_machine.c` integrity verification checks.
- **Consensus events** from `polycall_state_machine.c` guard evaluations and unguarded transition decisions.
