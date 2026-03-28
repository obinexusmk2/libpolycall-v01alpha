# Trinary Consensus in LibPolyCall

This document defines wire-level and telemetry behavior for trinary decisions.

## 1) Decision Semantics

Trinary decisions are represented by `polycall_trinary_decision_t`:

- `POLYCALL_TRINARY_NO (0)`: explicit rejection.
- `POLYCALL_TRINARY_MAYBE (1)`: undecided/deferred.
- `POLYCALL_TRINARY_YES (2)`: explicit approval.

`MAYBE` is not equivalent to abstain-by-silence; it is an explicit decision state that can later transition to `YES` or `NO`.

## 2) Protocol Messages

Two new message types are available in `polycall_message_type_t`:

- `POLYCALL_MSG_TRINARY_DECISION (0x07)`
- `POLYCALL_MSG_TRINARY_ACK (0x08)`

### Trinary decision payload

```c
typedef struct {
    uint32_t decision_id;
    uint32_t proposer_id;
    uint32_t ttl_ms;
    polycall_trinary_decision_t decision;
    uint8_t reserved[3];
} polycall_trinary_decision_message_t;
```

### Trinary ack payload

```c
typedef struct {
    uint32_t decision_id;
    uint32_t ack_sequence;
    uint32_t responder_id;
    polycall_trinary_decision_t echoed_decision;
    uint8_t accepted;
    uint8_t reserved[2];
} polycall_trinary_ack_message_t;
```

## 3) Decision Lifecycle Telemetry Schema

Telemetry events emitted through protocol callbacks use:

```c
typedef struct {
    uint32_t decision_id;
    uint32_t sequence;
    uint64_t timestamp_ms;
    polycall_trinary_decision_t decision;
    polycall_decision_event_t lifecycle_event;
    uint8_t source_message_type;
} polycall_decision_telemetry_event_t;
```

Lifecycle enum values:

- `POLYCALL_DECISION_EVENT_SUBMITTED`
- `POLYCALL_DECISION_EVENT_ECHOED`
- `POLYCALL_DECISION_EVENT_CONFIRMED`
- `POLYCALL_DECISION_EVENT_EXPIRED`

## 4) Event Emission Rules

- `submitted`: emitted when local endpoint sends `POLYCALL_MSG_TRINARY_DECISION`.
- `echoed`: emitted when a decision message from a peer is processed.
- `confirmed`: emitted when a trinary ACK is accepted (`accepted=1`).
- `expired`: emitted when ACK marks rejection/expiration (`accepted=0`).

## 5) Wire Examples

Decision submit:

```json
{
  "header": {"version":1,"type":7,"flags":"RELIABLE|URGENT","sequence":501},
  "payload": {"decision_id":42,"proposer_id":7,"ttl_ms":30000,"decision":"YES"}
}
```

Decision acknowledgment:

```json
{
  "header": {"version":1,"type":8,"flags":"RELIABLE","sequence":502},
  "payload": {"decision_id":42,"ack_sequence":501,"responder_id":19,"echoed_decision":"YES","accepted":1}
}
```

## 6) CLI Telemetry Exposure (Java binding)

The Java CLI telemetry command exposes the schema and wire examples:

```bash
java -jar java-polycall.jar telemetry --decision-lifecycle-schema
java -jar java-polycall.jar telemetry --wire-example
```

These options are intended for integration operators validating event contracts during rollout.
