# LibPolyCall Source Traversal

Use this map to navigate from runtime internals to bindings and example consumers.

## 1) Core C runtime path

Start here for execution flow:

1. `libpolycall-v1/main.c`
   - Process entrypoint.
   - Chooses interactive CLI vs non-interactive `-f` config flow.
   - Boots network programs and state machine lifecycle.
2. `libpolycall-v1/src/network.c`
   - Endpoint/client setup, runtime network loop helpers.
3. `libpolycall-v1/src/polycall_state_machine.c`
   - State definitions, transition registration, transition execution.
4. `libpolycall-v1/src/polycall_protocol.c`
   - Protocol framing/parsing and message-level behaviors.

Recommended reading order is exactly the list above.

---

## 2) Public C headers (`libpolycall-v1/include/`)

These headers define the C-facing surface area:

- `polycall.h` – core context and initialization API.
- `network.h` – network program/endpoints/client API.
- `polycall_state_machine.h` – state machine data types and operations.
- `polycall_protocol.h` – protocol message and handler APIs.
- `polycall_micro.h` – micro-runtime related interfaces.
- `polycall_parser.h` – parser and AST APIs.
- `polycall_file_parser.h` – file parser interfaces.
- `polycall_token.h` – token/value structures.
- `polycall_tokenizer.h` – tokenizer API.

When adding runtime features, update header contracts first, then the corresponding `src/*.c` implementation.

---

## 3) Binding directories

Primary bindings in this repo:

- `bindings/node-polycall/`
  - `src/index.js` and `src/modules/*` implement JS client components.
  - `examples/server.js` demonstrates runtime integration.
- `bindings/pypolycall/`
  - `pypolycall/` package contains Python adapter implementation.
  - `tests/` contains binding tests.

Additional language bindings available:

- `bindings/go-polycall/`
- `bindings/lua-polycall/`
- `bindings/java-polycall/`

---

## 4) Example projects vs runtime

### Top-level examples

- `examples/test_client_api.{py,js,go,lua}`
  - Minimal client-side usage patterns.
  - Useful for quick protocol smoke tests.

### Scenario projects

- `projects/banking-system/`
- `projects/telemetry-dashboard/`
- `projects/edge-iot-mesh/`
- `projects/banking-secure-bridge/`

Relationship to runtime:

- These projects are **consumers** and demonstrations.
- They do not replace `libpolycall-v1` as the authoritative runtime core.
- Binding code acts as adapter layers between project logic and runtime protocol behavior.

---

## 5) Fast onboarding sequence

1. Read `libpolycall-v1/main.c`.
2. Read `docs/USAGE.md` and run the runtime locally.
3. Inspect `bindings/node-polycall/examples/server.js` or Python binding docs.
4. Explore `examples/` and then `projects/` for end-to-end patterns.
