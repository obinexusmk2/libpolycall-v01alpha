"""Extension manager for dynamic CLI command loading."""

from pathlib import Path


class ExtensionManager:
    """Minimal extension manager implementation."""

    async def load_extensions(self, extension_dir: str, command_registry: object) -> int:
        """Load extensions from a directory.

        Currently validates directory existence and performs no dynamic imports.
        Returns number of loaded extensions.
        """
        path = Path(extension_dir)
        if not path.exists() or not path.is_dir():
            return 0
        return 0
