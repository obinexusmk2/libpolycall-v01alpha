# PolyCall Protocol Reference

## Trinary consensus model

Shared decision enum:

- `POLYCALL_DECISION_NO = 0x00`
- `POLYCALL_DECISION_MAYBE = 0x01`
- `POLYCALL_DECISION_YES = 0x02`

## Message types

- `POLYCALL_MSG_CONSENSUS_ECHO = 0x07`
- `POLYCALL_MSG_CONSENSUS_ACK = 0x08`

## Consensus payload layout

`polycall_consensus_payload_t` wire payload fields:

1. `decision` (`uint8`) — trinary decision value.
2. `correlation_id` (`uint32`) — message sequence/correlation id.
3. `persisted` (`uint8`) — `1` only when MAYBE is persisted.

## Wire-format examples

### Consensus Echo (MAYBE)

- Header: `type=0x07` (`CONSENSUS_ECHO`), `flags=0x08` (`RELIABLE`)
- Payload:
  - `decision=0x01` (`MAYBE`)
  - `correlation_id=42`
  - `persisted=1`

### Consensus Ack for Echo 42

- Header: `type=0x08` (`CONSENSUS_ACK`), `flags=0x08` (`RELIABLE`)
- Payload:
  - `decision=0x01` (`MAYBE`)
  - `correlation_id=42`
  - `persisted=1`

### Consensus Echo (YES)

- Header: `type=0x07`
- Payload:
  - `decision=0x02` (`YES`)
  - `correlation_id=73`
  - `persisted=0`
