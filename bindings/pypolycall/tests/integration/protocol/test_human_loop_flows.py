"""Integration-style protocol tests for human-loop indecision/pending flows."""

import pytest

from tests.fixtures.human_loop import evaluate_flow


@pytest.mark.parametrize(
    "decision,expected_state,expected_decision_telemetry",
    [
        ("YES", "APPROVED", {"kind": "decision", "value": "yes"}),
        ("NO", "REJECTED", {"kind": "decision", "value": "no"}),
        ("MAYBE", "PENDING_HUMAN_REVIEW", {"kind": "decision", "value": "maybe"}),
    ],
)
def test_decision_transitions_and_telemetry(
    flow_builder,
    decision_factory,
    flow_evaluator,
    decision,
    expected_state,
    expected_decision_telemetry,
):
    """Validate YES/NO/MAYBE transitions and telemetry output."""
    flow = flow_builder(decision_factory(decision), flow_id=f"decision-{decision.lower()}")

    result = flow_evaluator(flow)

    assert result["final_state"] == expected_state
    assert result["telemetry"] == [expected_decision_telemetry]


def test_maybe_with_delayed_ack_transitions_to_pending_ack(
    flow_builder,
    maybe_event,
    delayed_ack_factory,
    flow_evaluator,
):
    """Validate indecision + delayed acknowledgment pending flow telemetry."""
    flow = flow_builder(
        maybe_event,
        delayed_ack_factory(delay_seconds=2.5, ack=True),
        flow_id="indecision-delayed-ack",
    )

    result = flow_evaluator(flow)

    assert result["transitions"] == ["START", "PENDING_HUMAN_REVIEW", "PENDING_ACK"]
    assert result["final_state"] == "PENDING_ACK"
    assert result["telemetry"] == [
        {"kind": "decision", "value": "maybe"},
        {"kind": "ack", "status": "ack", "delay_seconds": 2.5},
    ]


def test_maybe_with_non_response_transitions_to_timeout(
    flow_builder,
    maybe_event,
    non_response_factory,
    flow_evaluator,
):
    """Validate indecision + non-response timeout pending flow telemetry."""
    flow = flow_builder(
        maybe_event,
        non_response_factory(timeout_seconds=45.0, retries=2),
        flow_id="indecision-timeout",
    )

    result = flow_evaluator(flow)

    assert result["transitions"] == ["START", "PENDING_HUMAN_REVIEW", "PENDING_TIMEOUT"]
    assert result["final_state"] == "PENDING_TIMEOUT"
    assert result["telemetry"] == [
        {"kind": "decision", "value": "maybe"},
        {"kind": "timeout", "timeout_seconds": 45.0, "retries": 2},
    ]


def test_evaluate_flow_can_be_used_directly_for_custom_composition(flow_builder, yes_event):
    """Demonstrate direct composition helpers remain usable without pytest fixtures."""
    custom_flow = flow_builder(yes_event, flow_id="custom-composition")

    result = evaluate_flow(custom_flow)

    assert result["flow_id"] == "custom-composition"
    assert result["final_state"] == "APPROVED"
