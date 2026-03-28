# LibPolyCall Repository Architecture

This document reflects the **current tree** rooted at `libpolycall-v01alpha` and uses `libpolycall-v1` as the canonical runtime name.

## Top-level layout

```text
/workspace/libpolycall-v01alpha/
├── libpolycall-v1/   # Core C runtime, parser/protocol/network/state components
├── bindings/         # Language bindings (Java, Python, Node, Go, Lua, and legacy/outdated snapshots)
├── projects/         # Demonstration and integration projects
├── tools/            # Utility scripts for repository maintenance
├── docs/             # Documentation index and architecture/traversal guides
├── examples/         # Lightweight multi-language client examples
└── ports/            # Packaging/port metadata (e.g., vcpkg)
```

## Directory responsibilities

## `libpolycall-v1/` (core runtime)

- `main.c`: runtime process entrypoint and command loop.
- `src/`: core runtime modules (protocol, parser, tokenizer, network, state machine, micro runtime).
- `include/`: public headers for runtime modules.
- `config.Polycallfile`: baseline multi-language service/network/security config.
- `Makefile`: build orchestration for static/shared libraries and runtime binary.
- `test/`: C-level tests.

## `bindings/` (language adapters)

Active binding directories in the tree:

- `java-polycall/`
- `pypolycall/`
- `node-polycall/`
- `go-polycall/`
- `lua-polycall/`

Legacy snapshots kept intentionally for historical compatibility/reference:

- `node-polycall(outdated)/`
- `pypolycall-outdated/`
- `pypolycall/backup_20250603_233522/`

## `projects/` (demo implementations)

Present project directories:

- `banking-system/`
- `banking-secure-bridge/`
- `edge-iot-mesh/`
- `telemetry-dashboard/`

These projects demonstrate integration patterns, testing, and domain-specific use of LibPolyCall components.

## `tools/` (utility scripts)

Contains repository maintenance/automation scripts, including:

- `tools/scripts/clone_commit.py`
- `tools/scripts/fix_for_powershell.ps1`

## `docs/` (documentation)

Primary docs for navigation:

- `docs/PLAN.md` (documentation index/start point)
- `docs/USAGE.md` (build/run/config workflow)
- `docs/TRAVERSAL.md` (code and project traversal map)
- `docs/architecture/ARCHITECTURE.md` (this file)

## Naming and terminology

- **Canonical runtime name**: `libpolycall-v1`
- **Legacy label**: `v1trial` appears in some historical files and bindings; treat as historical terminology unless explicitly called out.

## How to navigate quickly

1. Start with `docs/PLAN.md`.
2. Follow to `docs/USAGE.md` to build/run `libpolycall-v1`.
3. Use `docs/TRAVERSAL.md` to locate runtime files, language entrypoints, and demo projects.
