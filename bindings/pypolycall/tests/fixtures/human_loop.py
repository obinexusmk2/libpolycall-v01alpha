"""Composable factories for human-loop protocol fixtures.

The builders here intentionally stay small and composable so tests can combine
state transitions without creating monolithic setup code.
"""

from __future__ import annotations

from typing import Any, Callable, Dict, List

import pytest

DecisionEvent = Dict[str, Any]
TelemetryEvent = Dict[str, Any]


def decision_event(decision: str, *, actor: str = "human") -> DecisionEvent:
    """Create a normalized decision event for YES/NO/MAYBE responses."""
    normalized = decision.upper()
    if normalized not in {"YES", "NO", "MAYBE"}:
        raise ValueError(f"Unsupported decision: {decision}")

    return {
        "type": "decision",
        "actor": actor,
        "decision": normalized,
    }


def delayed_ack_event(*, delay_seconds: float = 1.0, ack: bool = True) -> DecisionEvent:
    """Create a delayed acknowledgment event."""
    return {
        "type": "ack",
        "status": "ACK" if ack else "NACK",
        "delay_seconds": delay_seconds,
    }


def non_response_event(*, timeout_seconds: float = 30.0, retries: int = 1) -> DecisionEvent:
    """Create a non-response event with timeout metadata."""
    return {
        "type": "non_response",
        "timeout_seconds": timeout_seconds,
        "retries": retries,
    }


def build_flow(*events: DecisionEvent, flow_id: str = "flow-001") -> Dict[str, Any]:
    """Build a human-loop flow payload from independent events."""
    return {
        "flow_id": flow_id,
        "events": list(events),
    }


def evaluate_flow(flow: Dict[str, Any]) -> Dict[str, Any]:
    """Evaluate fixture flow into expected transition + telemetry markers."""
    state = "START"
    transitions: List[str] = [state]
    telemetry: List[TelemetryEvent] = []

    for event in flow["events"]:
        event_type = event["type"]

        if event_type == "decision":
            if event["decision"] == "YES":
                state = "APPROVED"
                telemetry.append({"kind": "decision", "value": "yes"})
            elif event["decision"] == "NO":
                state = "REJECTED"
                telemetry.append({"kind": "decision", "value": "no"})
            else:
                state = "PENDING_HUMAN_REVIEW"
                telemetry.append({"kind": "decision", "value": "maybe"})

        elif event_type == "ack":
            if state == "PENDING_HUMAN_REVIEW":
                state = "PENDING_ACK"
            telemetry.append(
                {
                    "kind": "ack",
                    "status": event["status"].lower(),
                    "delay_seconds": event["delay_seconds"],
                }
            )

        elif event_type == "non_response":
            state = "PENDING_TIMEOUT"
            telemetry.append(
                {
                    "kind": "timeout",
                    "timeout_seconds": event["timeout_seconds"],
                    "retries": event["retries"],
                }
            )

        transitions.append(state)

    return {
        "flow_id": flow["flow_id"],
        "transitions": transitions,
        "final_state": state,
        "telemetry": telemetry,
    }


@pytest.fixture
def decision_factory() -> Callable[..., DecisionEvent]:
    """Parameterized builder for YES/NO/MAYBE decision events."""

    def _build(decision: str, **kwargs: Any) -> DecisionEvent:
        return decision_event(decision, **kwargs)

    return _build


@pytest.fixture
def yes_event(decision_factory: Callable[..., DecisionEvent]) -> DecisionEvent:
    """Reusable YES decision fixture."""
    return decision_factory("YES")


@pytest.fixture
def no_event(decision_factory: Callable[..., DecisionEvent]) -> DecisionEvent:
    """Reusable NO decision fixture."""
    return decision_factory("NO")


@pytest.fixture
def maybe_event(decision_factory: Callable[..., DecisionEvent]) -> DecisionEvent:
    """Reusable MAYBE decision fixture."""
    return decision_factory("MAYBE")


@pytest.fixture
def delayed_ack_factory() -> Callable[..., DecisionEvent]:
    """Factory for delayed ACK / NACK events."""

    def _build(**kwargs: Any) -> DecisionEvent:
        return delayed_ack_event(**kwargs)

    return _build


@pytest.fixture
def non_response_factory() -> Callable[..., DecisionEvent]:
    """Factory for timeout/non-response events."""

    def _build(**kwargs: Any) -> DecisionEvent:
        return non_response_event(**kwargs)

    return _build


@pytest.fixture
def flow_builder() -> Callable[..., Dict[str, Any]]:
    """Composable flow builder fixture."""

    def _build(*events: DecisionEvent, flow_id: str = "flow-001") -> Dict[str, Any]:
        return build_flow(*events, flow_id=flow_id)

    return _build


@pytest.fixture
def flow_evaluator() -> Callable[[Dict[str, Any]], Dict[str, Any]]:
    """Evaluate transitions + telemetry for a flow payload."""

    def _evaluate(flow: Dict[str, Any]) -> Dict[str, Any]:
        return evaluate_flow(flow)

    return _evaluate
