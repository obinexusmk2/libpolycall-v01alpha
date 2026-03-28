# Consensus Fixture Contract (Human-in-Loop)

This contract defines the shared fixture format used by core protocol/state tests and language binding parity suites.

## Location

- Canonical fixture root: `fixtures/consensus/`
- Canonical files:
  - `state_transitions.json`
  - `retries.json`
  - `ack_timelines.json`

## Format

- Each fixture file is UTF-8 JSON object with:
  - `version` (string)
  - `description` (string)
  - `scenarios` (array)
- Each scenario requires a unique `id` string within its file.

## Invariants

- Decision states are uppercase and constrained to `YES`, `NO`, or `MAYBE`.
- Retry scenarios must include:
  - `max_retries` (non-negative integer)
  - `attempts` (ordered state list)
  - deterministic `expected_final` and `expected_retry_count`
- Acknowledgment timeline scenarios must include:
  - `ack_deadline_ms` (non-negative integer)
  - ordered `events` with `name` and `at_ms`
  - deterministic `expected_ack_status` in `ON_TIME | LATE | MISSING`

## Extension Rules

1. Additive changes only for minor revisions:
   - New scenarios may be appended.
   - Existing scenario `id` semantics must not be changed.
2. Breaking semantic changes require `version` major bump.
3. Cross-language parity:
   - Python and Node binding tests must consume canonical fixtures directly.
   - Core protocol/state tests must consume the same canonical fixtures through dedicated loader wrappers.
4. Keep loaders small and composable; avoid monolithic test setup logic.
