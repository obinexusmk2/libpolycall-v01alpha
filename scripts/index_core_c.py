#!/usr/bin/env python3
"""Index libpolycall-v1 C headers/sources into deterministic markdown."""

from __future__ import annotations

import argparse
import pathlib
import re
from typing import Iterable

ROOT = pathlib.Path(__file__).resolve().parents[1]
INCLUDE_DIR = ROOT / "libpolycall-v1" / "include"
SRC_DIR = ROOT / "libpolycall-v1" / "src"

FUNC_RE = re.compile(
    r"^\s*(?!typedef\b)(?!#)(?!static\b)([A-Za-z_][\w\s\*]+?)\s+([A-Za-z_]\w*)\s*\(([^;{}]*)\)\s*;\s*$",
    re.MULTILINE,
)
TYPEDEF_STRUCT_RE = re.compile(r"typedef\s+struct\s+([A-Za-z_]\w*)?\s*\{.*?\}\s*([A-Za-z_]\w*)\s*;", re.DOTALL)
NAMED_STRUCT_RE = re.compile(r"(?<!typedef\s)\bstruct\s+([A-Za-z_]\w*)\s*\{", re.DOTALL)
FUNC_DEF_RE = re.compile(
    r"^\s*(?!if\b|for\b|while\b|switch\b)([A-Za-z_][\w\s\*]+?)\s+([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{",
    re.MULTILINE,
)


def rel(path: pathlib.Path) -> str:
    return path.relative_to(ROOT).as_posix()


def read_text(path: pathlib.Path) -> str:
    return path.read_text(encoding="utf-8", errors="ignore")


def collapse_ws(value: str) -> str:
    return " ".join(value.split())


def extract_exported_apis(header_path: pathlib.Path) -> list[str]:
    text = read_text(header_path)
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    text = re.sub(r"//.*", "", text)

    prototypes: list[str] = []
    for ret_type, name, args in FUNC_RE.findall(text):
        sig = f"{collapse_ws(ret_type)} {name}({collapse_ws(args)})"
        prototypes.append(sig)

    return sorted(set(prototypes))


def extract_structs(header_path: pathlib.Path) -> list[str]:
    text = read_text(header_path)
    structs: set[str] = set()
    for tag, alias in TYPEDEF_STRUCT_RE.findall(text):
        chosen = alias or tag
        if chosen:
            structs.add(chosen)
    for tag in NAMED_STRUCT_RE.findall(text):
        if tag:
            structs.add(tag)
    return sorted(structs)


def extract_top_comment(path: pathlib.Path) -> str | None:
    lines = read_text(path).splitlines()
    i = 0
    while i < len(lines) and not lines[i].strip():
        i += 1
    if i >= len(lines):
        return None

    line = lines[i].strip()
    if line.startswith("//"):
        comment_lines = []
        while i < len(lines) and lines[i].strip().startswith("//"):
            comment_lines.append(lines[i].strip()[2:].strip())
            i += 1
        joined = " ".join(part for part in comment_lines if part)
        return joined or None

    if line.startswith("/*"):
        block = []
        while i < len(lines):
            block.append(lines[i])
            if "*/" in lines[i]:
                break
            i += 1
        text = "\n".join(block)
        text = re.sub(r"^\s*/\*+", "", text)
        text = re.sub(r"\*/\s*$", "", text)
        text = re.sub(r"^\s*\*\s?", "", text, flags=re.MULTILINE)
        cleaned = collapse_ws(text)
        return cleaned or None

    return None


def extract_defined_functions(source_path: pathlib.Path) -> list[str]:
    text = read_text(source_path)
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    text = re.sub(r"//.*", "", text)
    names = {name for _, name in FUNC_DEF_RE.findall(text) if name != "else"}
    return sorted(names)


def summarize_source_file(source_path: pathlib.Path) -> str:
    comment = extract_top_comment(source_path)
    if comment:
        return comment

    functions = extract_defined_functions(source_path)
    if functions:
        sample = ", ".join(functions[:5])
        suffix = "" if len(functions) <= 5 else f", +{len(functions) - 5} more"
        return f"Implements functions: {sample}{suffix}."

    return f"Implements {source_path.stem.replace('_', ' ')} logic."


def iter_files(path: pathlib.Path, pattern: str) -> Iterable[pathlib.Path]:
    return sorted(path.glob(pattern), key=lambda p: p.as_posix())


def render_markdown() -> str:
    headers = list(iter_files(INCLUDE_DIR, "*.h"))
    sources = list(iter_files(SRC_DIR, "*.c"))

    structs: dict[str, list[str]] = {rel(h): extract_structs(h) for h in headers}
    apis: dict[str, list[str]] = {rel(h): extract_exported_apis(h) for h in headers}
    responsibilities: dict[str, str] = {rel(s): summarize_source_file(s) for s in sources}

    lines: list[str] = []
    lines.append("# Core C Reference Index")
    lines.append("")
    lines.append("## Scope")
    lines.append("")
    lines.append("- Headers scanned: `libpolycall-v1/include/*.h`")
    lines.append("- Sources scanned: `libpolycall-v1/src/*.c`")
    lines.append("")

    lines.append("## Exported APIs")
    lines.append("")
    for header in sorted(apis):
        lines.append(f"### `{header}`")
        entries = apis[header]
        if not entries:
            lines.append("- _(no exported prototypes detected)_")
        else:
            for sig in entries:
                lines.append(f"- `{sig};`")
        lines.append("")

    lines.append("## Key Structs")
    lines.append("")
    any_structs = False
    for header in sorted(structs):
        entries = structs[header]
        if entries:
            any_structs = True
            lines.append(f"### `{header}`")
            for struct_name in entries:
                lines.append(f"- `{struct_name}`")
            lines.append("")
    if not any_structs:
        lines.append("- _(no structs detected)_")
        lines.append("")

    lines.append("## File-Level Responsibility")
    lines.append("")
    for source in sorted(responsibilities):
        lines.append(f"- `{source}`: {responsibilities[source]}")

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
