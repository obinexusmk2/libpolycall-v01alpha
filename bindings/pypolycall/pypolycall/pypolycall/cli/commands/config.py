"""Config Command - Read and display CLI/runtime configuration."""

import argparse
from pathlib import Path


class ConfigCommand:
    """Display configuration metadata."""

    def get_help(self) -> str:
        return "Inspect configuration values and sources"

    def add_arguments(self, parser: argparse.ArgumentParser) -> None:
        parser.add_argument(
            "--path",
            default=None,
            help="Optional configuration file path",
        )

    async def execute(self, args: argparse.Namespace) -> int:
        if args.path:
            config_path = Path(args.path)
            print(f"Config path: {config_path}")
            print(f"Exists: {'yes' if config_path.exists() else 'no'}")
        else:
            print("Using default configuration resolution")
        return 0
