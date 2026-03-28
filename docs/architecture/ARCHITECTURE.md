# LibPolyCall Repository Architecture (Current)

This document reflects only directories and modules that exist in this repository snapshot.

## Top-level layout

```text
.
├── libpolycall-v1/          # Core C runtime and protocol/state/network implementation
├── bindings/                # Language bindings (Node, Python, Go, Lua, Java)
├── projects/                # Example vertical projects built around PolyCall concepts
├── examples/                # Small cross-language client examples
├── docs/                    # Documentation set
├── tools/                   # Utility scripts
├── ports/libpolycall/       # Packaging metadata (vcpkg port)
├── images/                  # Repository images/assets
└── LICENSE
```

## Runtime core (`libpolycall-v1/`)

- `main.c`
  - CLI entry point; interactive and `-f` non-interactive runtime flows.
- `src/polycall.c`
  - Core context lifecycle (`polycall_init_with_config`, `polycall_cleanup`).
- `src/network.c`
  - Network program/endpoints/client lifecycle.
- `src/polycall_state_machine.c`
  - State machine creation, states, transitions, execution.
- `src/polycall_protocol.c`
  - Protocol message handling and wire-level operations.
- `include/`
  - Public C headers consumed by runtime/tests and external integrations.
- `test/`
  - C tests for state/state-machine behavior.

## Bindings (`bindings/`)

- `node-polycall/` (JavaScript modules + examples)
- `pypolycall/` (Python package and tests)
- `go-polycall/` (Go package/config/examples)
- `lua-polycall/` (Lua modules + CLI)
- `java-polycall/` (Java binding + native JNI bridge)

Repository also contains `*-outdated` directories; treat them as historical/reference rather than primary integration targets.

## Example and project layers

- `examples/`
  - Lightweight language client examples (`.py`, `.js`, `.go`, `.lua`).
- `projects/`
  - Multi-file scenario projects (e.g., banking system, telemetry dashboard, edge IoT mesh, banking secure bridge).

These are consumers of PolyCall ideas and integration patterns; the authoritative runtime implementation remains in `libpolycall-v1/`.

## Documentation map

- `docs/USAGE.md` – build/run and runtime operation.
- `docs/TRAVERSAL.md` – source-level navigation map.
- `docs/architecture/ARCHITECTURE.md` – this file.
