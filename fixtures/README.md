# HITL Fixture Contract

This folder contains reusable Human-in-the-Loop (HITL) scenarios for tests across `libpolycall-v1` and language bindings.

## Schema version

- Current fixture schema: `1.0`
- Every fixture **must** declare `schema_version`.
- Tests should reject unknown major versions.

## Required keys

Each top-level scenario file (`happy_yes`, `rejection_no`, `deferred_maybe`) must include:

- `schema_version` (string)
- `scenario_id` (string, globally unique)
- `domain` (string; one of `consensus`, `telemetry`, `daemon`)
- `decision` (string; `YES`, `NO`, or `MAYBE`)
- `status` (string; e.g. `approved`, `rejected`, `deferred`)
- `includes` (array of behavior fixture files to compose)
- `input` (object/map)
- `expected` (object/map)

For deferred (`MAYBE`) scenarios, `expected` must also include:

- `timeout_ms` (integer)
- `retry_count` (integer)
- `ack_required` (boolean)

## Composition and extension rules

1. Keep scenario files small and focused on intent.
2. Put reusable behavior details in separate files (for example `retry_policy.yaml`, `ack_delayed.yaml`).
3. Use `includes` to declare composition order.
4. Additive extension only:
   - Existing keys must keep their semantic meaning.
   - New optional keys may be added under `input`/`expected` without breaking old tests.
5. Prefer new behavior files over editing unrelated scenarios.

## Format support

- `.json` and `.yaml` are both supported.
- Naming convention:
  - `happy_yes.*`
  - `rejection_no.*`
  - `deferred_maybe.*`
