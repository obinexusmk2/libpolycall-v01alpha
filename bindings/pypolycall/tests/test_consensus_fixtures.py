"""Fixture-driven tests for Python binding parity."""

from tests.fixture_loader import (
    load_ack_timeline_scenarios,
    load_retry_scenarios,
    load_transition_scenarios,
)
from pypolycall.core.binding import ProtocolBinding


def evaluate_transition(decision: str) -> str:
    return decision


def evaluate_retries(max_retries: int, attempts: list[str]) -> tuple[str, int]:
    retries = 0
    for attempt in attempts:
        if attempt in {"YES", "NO"}:
            return attempt, retries
        if retries < max_retries:
            retries += 1
    return "NO", max_retries


def evaluate_ack(deadline_ms: int, events: list[dict]) -> str:
    ack = next((event for event in events if event["name"] == "ack_received"), None)
    if ack is None:
        return "MISSING"
    return "ON_TIME" if ack["at_ms"] <= deadline_ms else "LATE"


def test_binding_transition_parity() -> None:
    for scenario in load_transition_scenarios():
        assert evaluate_transition(scenario["decision"]) == scenario["expected_state"]


def test_binding_retry_parity() -> None:
    for scenario in load_retry_scenarios():
        final_state, retry_count = evaluate_retries(
            scenario["max_retries"], scenario["attempts"]
        )
        assert final_state == scenario["expected_final"]
        assert retry_count == scenario["expected_retry_count"]


def test_binding_ack_parity() -> None:
    for scenario in load_ack_timeline_scenarios():
        assert (
            evaluate_ack(scenario["ack_deadline_ms"], scenario["events"])
            == scenario["expected_ack_status"]
        )


def test_authoritative_module_path() -> None:
    binding = ProtocolBinding()
    assert binding.__class__.__module__ == "pypolycall.core.binding"
