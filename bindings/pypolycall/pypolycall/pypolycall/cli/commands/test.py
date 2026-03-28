"""Test Command - Validate Runtime Connectivity and Binding Health."""

import argparse
from typing import Any, Optional

from ...utils import Logger

logger = Logger.get_logger(__name__)


class TestCommand:
    """Run lightweight runtime and binding checks."""

    def __init__(self) -> None:
        self._core_binding: Optional[Any] = None

    def set_core_binding(self, core_binding: Any) -> None:
        """Inject initialized ProtocolBinding from CLI runtime."""
        self._core_binding = core_binding

    def get_help(self) -> str:
        return "Validate runtime connectivity and binding readiness"

    def add_arguments(self, parser: argparse.ArgumentParser) -> None:
        parser.add_argument(
            "--skip-runtime",
            action="store_true",
            help="Skip runtime-dependent checks",
        )

    async def execute(self, args: argparse.Namespace) -> int:
        """Execute test checks."""
        print("PyPolyCall Test Command")
        print("=" * 40)
        print("✓ Command module import successful")

        if args.skip_runtime:
            print("✓ Runtime checks skipped by --skip-runtime")
            return 0

        if self._core_binding is None:
            print("⚠ Core binding not initialized")
            return 1

        print("✓ Core binding initialized")
        print(f"✓ Runtime target: {self._core_binding.polycall_host}:{self._core_binding.polycall_port}")
        logger.info("Test command checks completed")
        return 0
