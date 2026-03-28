# Repository Architecture

Accurate repo-relative structure focused on onboarding and traversal.

```text
.
├── README.md
├── docs/
│   ├── USAGE.md
│   ├── REFERENCE.md
│   ├── PLAN.md
│   ├── FEATURES.md
│   ├── TRIAL.md
│   └── architecture/
│       └── ARCHITECTURE.md
├── libpolycall-v1/
│   ├── Makefile
│   ├── main.c
│   ├── config.Polycallfile
│   ├── include/
│   │   ├── polycall.h
│   │   ├── polycall_protocol.h
│   │   ├── polycall_state_machine.h
│   │   ├── polycall_micro.h
│   │   ├── polycall_parser.h
│   │   ├── polycall_tokenizer.h
│   │   └── network.h
│   ├── src/
│   │   ├── polycall.c
│   │   ├── polycall_protocol.c
│   │   ├── polycall_state_machine.c
│   │   ├── polycall_micro.c
│   │   ├── polycall_parser.c
│   │   ├── polycall_tokenizer.c
│   │   ├── polycall_token.c
│   │   └── network.c
│   ├── test/
│   │   ├── test_polystate.c
│   │   └── test_polystate_machine.c
│   └── bin/
│       └── polycall
├── bindings/
│   ├── node-polycall/
│   ├── pypolycall/
│   ├── go-polycall/
│   ├── java-polycall/
│   ├── lua-polycall/
│   ├── node-polycall(outdated)/
│   └── pypolycall-outdated/
├── projects/
│   ├── banking-system/
│   ├── banking-secure-bridge/
│   ├── telemetry-dashboard/
│   └── edge-iot-mesh/
├── tools/
│   └── scripts/
│       ├── clone_commit.py
│       └── fix_for_powershell.ps1
├── examples/
├── ports/
└── images/
```

## Traversal order (recommended)

1. `README.md` (entrypoint)
2. `docs/USAGE.md` (verified run commands)
3. `docs/REFERENCE.md` (source/binding map)
4. `libpolycall-v1/include/` then `libpolycall-v1/src/` (C API + implementation)
5. `bindings/` (language adapters)
6. `projects/` (end-to-end demos)
7. `tools/scripts/` (maintenance utilities)
