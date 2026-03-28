#!/usr/bin/env python3
"""Index language bindings into deterministic markdown."""

from __future__ import annotations

import argparse
import json
import pathlib
import re
from typing import Iterable
import xml.etree.ElementTree as ET

ROOT = pathlib.Path(__file__).resolve().parents[1]
BINDINGS_DIR = ROOT / "bindings"

LANG_BY_DIR_HINT = {
    "node": "JavaScript/Node.js",
    "pypolycall": "Python",
    "go": "Go",
    "lua": "Lua",
    "java": "Java",
}

EXT_LANG = {
    ".py": "Python",
    ".js": "JavaScript/Node.js",
    ".mjs": "JavaScript/Node.js",
    ".cjs": "JavaScript/Node.js",
    ".ts": "TypeScript/Node.js",
    ".go": "Go",
    ".lua": "Lua",
    ".java": "Java",
}


CLASS_PATTERNS = {
    "Python": re.compile(r"^\s*class\s+([A-Za-z_]\w*)", re.MULTILINE),
    "Java": re.compile(r"\bclass\s+([A-Za-z_]\w*)"),
    "JavaScript/Node.js": re.compile(r"\bclass\s+([A-Za-z_]\w*)"),
    "TypeScript/Node.js": re.compile(r"\bclass\s+([A-Za-z_]\w*)"),
    "Lua": re.compile(r"([A-Za-z_]\w*)\s*=\s*\{\}"),
}


def rel(path: pathlib.Path) -> str:
    return path.relative_to(ROOT).as_posix()


def read_text(path: pathlib.Path) -> str:
    return path.read_text(encoding="utf-8", errors="ignore")


def iter_binding_dirs() -> list[pathlib.Path]:
    if not BINDINGS_DIR.exists():
        return []
    return sorted([p for p in BINDINGS_DIR.iterdir() if p.is_dir()], key=lambda p: p.name)


def detect_language(binding_dir: pathlib.Path) -> str:
    lowered = binding_dir.name.lower()
    for hint, language in LANG_BY_DIR_HINT.items():
        if hint in lowered:
            return language

    counts: dict[str, int] = {}
    for file in binding_dir.rglob("*"):
        if not file.is_file():
            continue
        language = EXT_LANG.get(file.suffix)
        if language:
            counts[language] = counts.get(language, 0) + 1
    if counts:
        return sorted(counts.items(), key=lambda item: (-item[1], item[0]))[0][0]
    return "Unknown"


def detect_cli_entrypoints(binding_dir: pathlib.Path) -> list[str]:
    results: set[str] = set()

    package_json = binding_dir / "package.json"
    if package_json.exists():
        try:
            payload = json.loads(read_text(package_json))
            bin_field = payload.get("bin")
            if isinstance(bin_field, str):
                results.add(f"package.json:bin -> {bin_field}")
            elif isinstance(bin_field, dict):
                for name, target in sorted(bin_field.items()):
                    results.add(f"package.json:bin:{name} -> {target}")
            scripts = payload.get("scripts", {})
            if isinstance(scripts, dict):
                for script_name in sorted(scripts):
                    if script_name in {"start", "cli"}:
                        results.add(f"package.json:scripts:{script_name}")
        except json.JSONDecodeError:
            pass

    pyproject = binding_dir / "pyproject.toml"
    if pyproject.exists():
        text = read_text(pyproject)
        for match in re.findall(r"^\s*([A-Za-z0-9_.-]+)\s*=\s*\"([^\"]+)\"", text, flags=re.MULTILINE):
            name, target = match
            if ":" in target:
                results.add(f"pyproject:scripts:{name} -> {target}")

    setup_py = binding_dir / "setup.py"
    if setup_py.exists():
        text = read_text(setup_py)
        for match in re.findall(r"entry_points\s*=\s*\{([^}]+)\}", text, flags=re.DOTALL):
            cleaned = " ".join(match.split())
            results.add(f"setup.py:entry_points -> {cleaned[:100]}")

    for pattern in ("bin/*", "scripts/*"):
        for file in sorted(binding_dir.glob(pattern), key=lambda p: p.as_posix()):
            if file.is_file():
                results.add(rel(file))

    for name in ("main.py", "main.go", "main.lua", "Main.java", "cli.py", "cli.js"):
        for file in sorted(binding_dir.rglob(name), key=lambda p: p.as_posix()):
            if file.is_file():
                results.add(rel(file))

    return sorted(results)


def detect_runtime_assumptions(binding_dir: pathlib.Path, language: str) -> list[str]:
    assumptions: list[str] = []

    package_json = binding_dir / "package.json"
    if package_json.exists():
        try:
            payload = json.loads(read_text(package_json))
            engines = payload.get("engines", {})
            if isinstance(engines, dict):
                for key in sorted(engines):
                    assumptions.append(f"{key}: {engines[key]}")
        except json.JSONDecodeError:
            assumptions.append("package.json exists (unparseable)")

    go_mod = binding_dir / "go.mod"
    if go_mod.exists():
        text = read_text(go_mod)
        match = re.search(r"^go\s+([0-9.]+)", text, flags=re.MULTILINE)
        if match:
            assumptions.append(f"go: {match.group(1)}")

    pyproject = binding_dir / "pyproject.toml"
    if pyproject.exists():
        text = read_text(pyproject)
        match = re.search(r"requires-python\s*=\s*\"([^\"]+)\"", text)
        if match:
            assumptions.append(f"python: {match.group(1)}")

    pom = binding_dir / "pom.xml"
    if pom.exists():
        try:
            root = ET.fromstring(read_text(pom))
            for tag in [
                "./{*}properties/{*}maven.compiler.source",
                "./{*}properties/{*}maven.compiler.target",
                "./{*}properties/{*}java.version",
            ]:
                found = root.find(tag)
                if found is not None and found.text:
                    assumptions.append(f"{tag.split('/')[-1].split('}')[-1]}: {found.text.strip()}")
        except ET.ParseError:
            assumptions.append("pom.xml exists (unparseable)")

    if not assumptions:
        assumptions.append(f"No explicit runtime metadata detected for {language}.")

    return sorted(set(assumptions))


def iter_source_files(binding_dir: pathlib.Path) -> Iterable[pathlib.Path]:
    for file in sorted(binding_dir.rglob("*"), key=lambda p: p.as_posix()):
        if file.is_file() and file.suffix in EXT_LANG:
            yield file


def detect_key_modules_classes(binding_dir: pathlib.Path, language: str) -> list[str]:
    entries: list[str] = []
    class_re = CLASS_PATTERNS.get(language)

    for file in iter_source_files(binding_dir):
        relative = rel(file)
        entries.append(relative)
        if class_re:
            text = read_text(file)
            for name in sorted(set(class_re.findall(text))):
                entries.append(f"{relative}::{name}")

    deduped = sorted(set(entries))
    return deduped[:20]


def render_markdown() -> str:
    lines: list[str] = []
    lines.append("# Bindings Reference Index")
    lines.append("")
    lines.append("## Scope")
    lines.append("")
    lines.append("- Directories scanned: `bindings/*`")
    lines.append("")

    binding_dirs = iter_binding_dirs()
    if not binding_dirs:
        lines.append("- _(no bindings directories found)_")
        lines.append("")
        return "\n".join(lines)

    for binding_dir in binding_dirs:
        language = detect_language(binding_dir)
        cli_entrypoints = detect_cli_entrypoints(binding_dir)
        runtime = detect_runtime_assumptions(binding_dir, language)
        key_modules = detect_key_modules_classes(binding_dir, language)

        lines.append(f"## `{rel(binding_dir)}`")
        lines.append("")
        lines.append(f"- Language: **{language}**")

        lines.append("- CLI entrypoints:")
        if cli_entrypoints:
            for item in cli_entrypoints:
                lines.append(f"  - `{item}`")
        else:
            lines.append("  - _(none detected)_")

        lines.append("- Runtime assumptions:")
        for item in runtime:
            lines.append(f"  - {item}")

        lines.append("- Key modules/classes:")
        if key_modules:
            for item in key_modules:
                lines.append(f"  - `{item}`")
        else:
            lines.append("  - _(none detected)_")
        lines.append("")

    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=pathlib.Path, help="Optional output markdown file")
    args = parser.parse_args()

    markdown = render_markdown()
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(markdown, encoding="utf-8")
    else:
        print(markdown)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
