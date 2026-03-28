# Traversal semantics for trinary decisions

This document describes the state-machine behavior guarantees for `NO/YES/MAYBE` decisions.

## Decision persistence

Every state starts with decision `POLYCALL_DECISION_MAYBE` unless explicitly set.

The following APIs preserve/query unresolved state:

- `polycall_sm_set_state_decision(...)`
- `polycall_sm_get_state_decision(...)`
- `polycall_sm_state_is_unresolved(...)`

`polycall_sm_state_is_unresolved(...)` returns true only for `MAYBE`.

## Explicit transition guarantees

`MAYBE -> YES` and `MAYBE -> NO` are explicit, per-state updates through `polycall_sm_set_state_decision(...)`.

These updates do not require enabling forced-collapse policy.

## Forced binary collapse policy

Bulk collapse of unresolved states is intentionally gated:

- `polycall_sm_force_binary_collapse(...)` fails with `POLYCALL_SM_ERROR_INVALID_TRANSITION` when policy is disabled.
- Enable policy via `polycall_sm_set_binary_collapse_policy(sm, true)`.
- When enabled, only unresolved `MAYBE` states are rewritten to the requested binary value (`YES` or `NO`).

This prevents accidental global loss of uncertainty unless policy explicitly allows it.
