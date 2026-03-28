"""Connect Command - Probe runtime connectivity state."""

import argparse
from typing import Any, Optional


class ConnectCommand:
    """Check and report runtime connection information."""

    def __init__(self) -> None:
        self._core_binding: Optional[Any] = None

    def set_core_binding(self, core_binding: Any) -> None:
        self._core_binding = core_binding

    def get_help(self) -> str:
        return "Check connection status to polycall runtime"

    def add_arguments(self, parser: argparse.ArgumentParser) -> None:
        parser.add_argument(
            "--json",
            action="store_true",
            help="Print connection status as JSON-like output",
        )

    async def execute(self, args: argparse.Namespace) -> int:
        if self._core_binding is None:
            print("Connection unavailable: core binding not initialized")
            return 1

        is_connected = bool(getattr(self._core_binding, "is_connected", False))
        target = f"{self._core_binding.polycall_host}:{self._core_binding.polycall_port}"

        if args.json:
            print({"target": target, "connected": is_connected})
        else:
            print(f"Runtime target: {target}")
            print(f"Connected: {'yes' if is_connected else 'no'}")

        return 0 if is_connected else 1
