"""Shared fixture loader for consensus workflow fixtures."""

from __future__ import annotations

import json
from pathlib import Path
from typing import Any, Dict, List

FIXTURE_DIR = Path(__file__).resolve().parent


def load_consensus_fixture(name: str) -> Dict[str, Any]:
    """Load a fixture by base file name from fixtures/consensus."""
    fixture_path = FIXTURE_DIR / f"{name}.json"
    with fixture_path.open("r", encoding="utf-8") as handle:
        return json.load(handle)


def load_transition_scenarios() -> List[Dict[str, Any]]:
    return load_consensus_fixture("state_transitions")["scenarios"]


def load_retry_scenarios() -> List[Dict[str, Any]]:
    return load_consensus_fixture("retries")["scenarios"]


def load_ack_timeline_scenarios() -> List[Dict[str, Any]]:
    return load_consensus_fixture("ack_timelines")["scenarios"]
