"""Telemetry Command - Display telemetry observer availability and state."""

import argparse
from typing import Any, Optional


class TelemetryCommand:
    """Inspect telemetry integration state for the active binding."""

    def __init__(self) -> None:
        self._core_binding: Optional[Any] = None

    def set_core_binding(self, core_binding: Any) -> None:
        self._core_binding = core_binding

    def get_help(self) -> str:
        return "Inspect telemetry integration and status"

    def add_arguments(self, parser: argparse.ArgumentParser) -> None:
        parser.add_argument(
            "--enabled-only",
            action="store_true",
            help="Return non-zero if telemetry appears disabled",
        )

    async def execute(self, args: argparse.Namespace) -> int:
        telemetry_obj = getattr(self._core_binding, "_telemetry_observer", None) if self._core_binding else None
        enabled = telemetry_obj is not None

        print("Telemetry status")
        print(f"Enabled: {'yes' if enabled else 'no'}")

        if args.enabled_only and not enabled:
            return 1
        return 0
