#!/usr/bin/env python3
"""Build deterministic repository mapping artifacts for libpolycall-v1 and bindings."""

from __future__ import annotations

import argparse
import csv
import json
import re
from pathlib import Path
from typing import Iterable

ROOT = Path(__file__).resolve().parents[3]

C_FILES = [ROOT / "libpolycall-v1" / "src", ROOT / "libpolycall-v1" / "include"]
BINDINGS_ROOT = ROOT / "bindings"

SYMBOL_PATTERNS = [
    re.compile(r"^\s*(?:def|async\s+def)\s+([A-Za-z_]\w*)\s*\(", re.MULTILINE),
    re.compile(r"^\s*class\s+([A-Za-z_]\w*)\b", re.MULTILINE),
    re.compile(r"^\s*function\s+([A-Za-z_$][\w$]*)\s*\(", re.MULTILINE),
    re.compile(r"^\s*const\s+([A-Za-z_$][\w$]*)\s*=\s*\(?.*?=>", re.MULTILINE),
    re.compile(r"^\s*func\s+([A-Za-z_]\w*)\s*\(", re.MULTILINE),
    re.compile(r"^\s*type\s+([A-Za-z_]\w*)\s+struct\b", re.MULTILINE),
    re.compile(r"^\s*public\s+class\s+([A-Za-z_]\w*)\b", re.MULTILINE),
    re.compile(r"^\s*local\s+function\s+([A-Za-z_]\w*)\s*\(", re.MULTILINE),
]

C_SYMBOL_PATTERN = re.compile(
    r"^\s*(?:[A-Za-z_]\w*\s+)*[A-Za-z_]\w*(?:\s*\*+\s*|\s+)([A-Za-z_]\w*)\s*\([^;{}]*\)\s*[;{]",
    re.MULTILINE,
)

INCLUDE_PATTERN = re.compile(r'^\s*#include\s+["<]([^">]+)[">]', re.MULTILINE)
IMPORT_PATTERNS = [
    re.compile(r"^\s*import\s+([A-Za-z0-9_./-]+)", re.MULTILINE),
    re.compile(r"^\s*from\s+([A-Za-z0-9_./-]+)\s+import\s+", re.MULTILINE),
    re.compile(r"require\(['\"]([^'\"]+)['\"]\)"),
]

ENTRYPOINT_NAMES = {
    "main.c",
    "main.py",
    "main.lua",
    "main.go",
    "main.java",
    "index.js",
    "index.ts",
    "server.js",
}


def relative(path: Path) -> str:
    return path.relative_to(ROOT).as_posix()


def iter_c_sources() -> Iterable[Path]:
    for base in C_FILES:
        for ext in ("*.c", "*.h"):
            for path in sorted(base.glob(ext)):
                yield path


def is_binding_entrypoint(path: Path) -> bool:
    rel = path.relative_to(BINDINGS_ROOT).as_posix()
    blocked_segments = {"target", "tests", "test", "backup_20250603_233522"}
    if any(segment in blocked_segments for segment in path.parts):
        return False
    if "(outdated)" in rel:
        return False

    if path.name in ENTRYPOINT_NAMES:
        return True
    if path.parent.name == "bin":
        return True
    if path.parent.name == "src" and path.stem in {"index", "main"}:
        return True

    binding_root = BINDINGS_ROOT / rel.split("/", 1)[0]
    return path.parent == binding_root and path.name == "__init__.py"


def iter_binding_entrypoints() -> Iterable[Path]:
    if not BINDINGS_ROOT.exists():
        return
    for path in sorted(BINDINGS_ROOT.rglob("*")):
        if not path.is_file():
            continue
        if path.suffix.lower() not in {".py", ".js", ".ts", ".go", ".java", ".lua", ".c", ".h"}:
            continue
        if is_binding_entrypoint(path):
            yield path


def extract_symbols(path: Path, content: str) -> list[str]:
    symbols: set[str] = set()
    if path.suffix in {".c", ".h"}:
        for name in C_SYMBOL_PATTERN.findall(content):
            if name not in {"if", "for", "while", "switch", "return", "sizeof"}:
                symbols.add(name)
    for pattern in SYMBOL_PATTERNS:
        symbols.update(pattern.findall(content))
    return sorted(symbols)


def extract_dependencies(content: str) -> list[str]:
    deps: set[str] = set()
    for inc in INCLUDE_PATTERN.findall(content):
        deps.add(inc)
    for pattern in IMPORT_PATTERNS:
        deps.update(pattern.findall(content))
    return sorted(deps)


def build_record(path: Path, category: str) -> dict[str, object]:
    content = path.read_text(encoding="utf-8", errors="ignore")
    return {
        "file": relative(path),
        "category": category,
        "symbols": extract_symbols(path, content),
        "dependencies": extract_dependencies(content),
    }


def write_json(records: list[dict[str, object]], out_path: Path) -> None:
    out_path.write_text(json.dumps(records, indent=2, sort_keys=True) + "\n", encoding="utf-8")


def write_csv(records: list[dict[str, object]], out_path: Path) -> None:
    with out_path.open("w", encoding="utf-8", newline="") as fh:
        writer = csv.writer(fh)
        writer.writerow(["file", "category", "symbols", "dependencies"])
        for record in records:
            writer.writerow(
                [
                    record["file"],
                    record["category"],
                    "|".join(record["symbols"]),
                    "|".join(record["dependencies"]),
                ]
            )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--out-dir",
        default=ROOT / "tools" / "scripts" / "v1-analysis" / "artifacts",
        type=Path,
        help="Output directory for JSON/CSV artifacts.",
    )
    args = parser.parse_args()
    out_dir: Path = args.out_dir
    out_dir.mkdir(parents=True, exist_ok=True)

    records = [build_record(path, "core") for path in iter_c_sources()]
    records.extend(build_record(path, "binding-entry") for path in iter_binding_entrypoints())
    records = sorted(records, key=lambda item: item["file"])

    write_json(records, out_dir / "repo-map.json")
    write_csv(records, out_dir / "repo-map.csv")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
