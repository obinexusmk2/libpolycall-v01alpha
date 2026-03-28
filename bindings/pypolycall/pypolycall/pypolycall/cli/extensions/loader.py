"""Extension loader helpers."""

from pathlib import Path
from typing import List


class ExtensionLoader:
    """Discover extension module files in a directory."""

    def discover(self, extension_dir: str) -> List[Path]:
        path = Path(extension_dir)
        if not path.exists() or not path.is_dir():
            return []
        return sorted(p for p in path.glob("*.py") if p.is_file())
