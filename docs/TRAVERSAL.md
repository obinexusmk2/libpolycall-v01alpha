# LibPolyCall Traversal Map

This document maps the current repository tree so contributors can move from runtime internals to bindings and demos quickly.

## 1) Core C runtime (`libpolycall-v1/`)

## Entrypoint

- `libpolycall-v1/main.c`  
  Runtime bootstrap, command handlers, network hooks, and state-machine lifecycle integration.

## Runtime modules (`libpolycall-v1/src/*.c`)

- `polycall.c` – core context lifecycle and shared runtime primitives.
- `polycall_state_machine.c` – state creation, transitions, locking, snapshots.
- `polycall_protocol.c` – protocol framing, handshake/validation paths.
- `network.c` – socket and endpoint behavior.
- `polycall_parser.c` – parser implementation for Polycall config/text inputs.
- `polycall_tokenizer.c` and `polycall_token.c` – tokenization/token utilities.
- `polycall_micro.c` – micro-runtime orchestration component.

## Public headers (`libpolycall-v1/include/*.h`)

- `polycall.h`
- `polycall_state_machine.h`
- `polycall_protocol.h`
- `network.h`
- `polycall_parser.h`
- `polycall_tokenizer.h`
- `polycall_token.h`
- `polycall_micro.h`
- `polycall_file_parser.h`

These headers define the runtime API and internal subsystem contracts used by `main.c` and the C modules.

## 2) Bindings by language (`bindings/`)

## Java (`bindings/java-polycall`)

- CLI/program entry: `src/main/java/org/obinexus/cli/Main.java`
- Core adapter classes: `src/main/java/org/obinexus/core/`
- FFI bridge classes: `src/main/java/org/obinexus/ffi/`
- Native bridge layer: `native/src/polycall_bridge.c`

## Python (`bindings/pypolycall`)

- Console entrypoint: `pypolycall.cli.main:main`
- CLI module: `pypolycall/cli/main.py`
- Core binding: `pypolycall/core/binding.py`
- Config manager: `pypolycall/config/manager.py`

## Node.js (`bindings/node-polycall`)

- Package entry: `src/index.js`
- Core modules: `src/modules/`
- Demo server: `examples/server.js`

## Go (`bindings/go-polycall`)

- Core client package: `pkg/client.go`
- Config support: `config/src/polycall_client.go`
- Examples: `examples/test_client_api.go`

## Lua (`bindings/lua-polycall`)

- CLI executable shim: `bin/lua-polycall`
- CLI module: `polycall/cli/main.lua`
- Runtime modules: `polycall/core/`, `polycall/config/`, `polycall/utils/`

## Legacy binding folders (intentional historical artifacts)

- `bindings/node-polycall(outdated)`
- `bindings/pypolycall-outdated`
- `bindings/pypolycall/backup_20250603_233522`

## 3) Demo projects and purpose (`projects/`)

- `projects/banking-system`  
  Banking workflow demo with Python server, templates, and tests.

- `projects/banking-secure-bridge`  
  Secure bridge demo emphasizing zero-trust/auth flows and mixed-language bindings.

- `projects/edge-iot-mesh`  
  Edge/mesh propagation demo (routing, conflict resolution, trigger updates).

- `projects/telemetry-dashboard`  
  Telemetry/CIR visualization demo with multi-language binding clients.

## 4) Supporting navigation

- `README.md` – repository landing page and quick documentation index.
- `docs/PLAN.md` – docs index and suggested reading order.
- `docs/USAGE.md` – build/run/config guide for `libpolycall-v1`.
- `docs/architecture/ARCHITECTURE.md` – directory-level architecture map.
