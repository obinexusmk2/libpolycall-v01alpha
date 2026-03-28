"""Fixture-driven core protocol/state checks for human-in-loop workflows."""

from fixture_loader import (
    load_ack_timeline_scenarios,
    load_retry_scenarios,
    load_transition_scenarios,
)


def apply_decision(initial_state: str, decision: str) -> str:
    _ = initial_state
    return decision


def resolve_retry_final(max_retries: int, attempts: list[str]) -> tuple[str, int]:
    retries = 0
    for attempt in attempts:
        if attempt in {"YES", "NO"}:
            return attempt, retries
        if retries < max_retries:
            retries += 1
    return "NO", max_retries


def evaluate_ack_status(deadline_ms: int, events: list[dict]) -> str:
    ack_event = next((event for event in events if event["name"] == "ack_received"), None)
    if not ack_event:
        return "MISSING"
    return "ON_TIME" if ack_event["at_ms"] <= deadline_ms else "LATE"


def test_state_transition_fixtures() -> None:
    for scenario in load_transition_scenarios():
        actual = apply_decision(scenario["initial_state"], scenario["decision"])
        assert actual == scenario["expected_state"], scenario["id"]


def test_retry_fixtures() -> None:
    for scenario in load_retry_scenarios():
        final_state, retry_count = resolve_retry_final(
            scenario["max_retries"], scenario["attempts"]
        )
        assert final_state == scenario["expected_final"], scenario["id"]
        assert retry_count == scenario["expected_retry_count"], scenario["id"]


def test_ack_timeline_fixtures() -> None:
    for scenario in load_ack_timeline_scenarios():
        status = evaluate_ack_status(scenario["ack_deadline_ms"], scenario["events"])
        assert status == scenario["expected_ack_status"], scenario["id"]
