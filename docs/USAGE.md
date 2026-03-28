# LibPolyCall v1 Usage Guide

This guide documents the **current, deterministic** usage flow for the C runtime in `libpolycall-v1/`.

## 1) Build and run `libpolycall-v1`

From repository root:

```bash
cd libpolycall-v1
make clean
make bin
```

This produces the runtime executable at:

- `libpolycall-v1/build/bin/polycall`

Run in interactive mode:

```bash
./build/bin/polycall
```

Run in non-interactive mode (config-driven):

```bash
./build/bin/polycall -f config.Polycallfile
```

---

## 2) `config.Polycallfile` semantics

`main.c` currently parses only a **small command subset** when started with `-f`.

### Supported directives

- `port <host>:<container>`
  - Example: `port 8080:8084`
  - Runtime behavior: stores the container side (`8084`) as the active listening port.
- `network start`
  - Starts network services using the active port.

### Parsing behavior and constraints

- Blank lines and lines beginning with `#` are ignored.
- Directives are parsed token-by-token (`<command> <value>`).
- Unknown directives are ignored in non-interactive mode.
- If `network start` is never encountered, the runtime warns and stays without active network services.

> Note: The sample `config.Polycallfile` includes additional keys (`server`, `network_timeout`, etc.). Those values are not consumed by the non-interactive parser path in `main.c` today.

---

## 3) Interactive vs non-interactive flow

## Interactive flow (no `-f`)

Start:

```bash
./build/bin/polycall
```

Then use CLI commands such as:

- `help`
- `start_network`
- `stop_network`
- `list_endpoints`
- `list_clients`
- `init`
- `add_state NAME`
- `list_states`
- `list_transitions`
- `status`
- `history`
- `quit`

This mode is best for exploratory debugging and state-machine experimentation.

## Non-interactive flow (`-f config.Polycallfile`)

Start:

```bash
./build/bin/polycall -f config.Polycallfile
```

Boot sequence:

1. Runtime initializes core context/state.
2. Config file is read line-by-line.
3. `port` sets runtime port.
4. `network start` initializes and registers a network program.
5. Runtime enters its event loop (`net_run` in a loop) until interrupted.

This mode is best for repeatable process startup and binding integration.

---

## 4) Binding integration pointers

Use these directories as the canonical entry points:

- Node binding: `bindings/node-polycall/`
  - Docs: `bindings/node-polycall/README.md`
  - Example server: `bindings/node-polycall/examples/server.js`
- Python binding: `bindings/pypolycall/`
  - Docs: `bindings/pypolycall/README.md`

### Typical local integration sequence

1. Build runtime (`make bin` in `libpolycall-v1`).
2. Start runtime (`./build/bin/polycall -f config.Polycallfile`).
3. Start binding-side example/client in the target binding directory.
4. Validate connect/auth/request flow from the binding README examples.

If you add a new binding, keep the runtime startup unchanged and add binding-specific connection instructions in that binding's own README.
