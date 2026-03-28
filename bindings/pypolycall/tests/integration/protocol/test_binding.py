"""Integration-level parity checks driven by consensus fixtures."""

from tests.fixture_loader import load_transition_scenarios


def test_consensus_transition_fixtures_are_consumable() -> None:
    scenarios = load_transition_scenarios()
    assert len(scenarios) >= 3
    assert {scenario["expected_state"] for scenario in scenarios} == {"YES", "NO", "MAYBE"}
