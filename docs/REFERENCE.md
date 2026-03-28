# Source & Binding Reference

This reference maps:
- each major C source in `libpolycall-v1/src/`, and
- each binding directory in `bindings/`

to purpose, main entry points, and related tests.

## Core C sources (`libpolycall-v1/src/`)

| File | Purpose | Main entry points | Related tests |
|---|---|---|---|
| `libpolycall-v1/src/polycall.c` | Core context lifecycle and version/error API. | `polycall_init_with_config`, `polycall_cleanup`, `polycall_get_version`, `polycall_get_last_error` | `libpolycall-v1/test/test_polystate.c`, `libpolycall-v1/test/test_polystate_machine.c` |
| `libpolycall-v1/src/polycall_protocol.c` | Protocol context, message validation, checksum/handshake flow, send/process loop. | `polycall_protocol_init`, `polycall_protocol_send`, `polycall_protocol_process`, `polycall_protocol_update`, `polycall_protocol_start_handshake`, `polycall_protocol_complete_handshake` | Indirect coverage via state-machine-oriented tests in `libpolycall-v1/test/` |
| `libpolycall-v1/src/polycall_state_machine.c` | State machine creation, state/transition registration, execution and integrity helpers. | `polycall_sm_create_with_integrity`, `initialize_state_machine`, `polycall_sm_add_state`, `polycall_sm_add_transition`, `polycall_sm_execute_transition`, `polycall_sm_destroy` | `libpolycall-v1/test/test_polystate.c`, `libpolycall-v1/test/test_polystate_machine.c` |
| `libpolycall-v1/src/polycall_micro.c` | Microservice context orchestration around protocol + state machine + command processing. | `polycall_micro_init`, `polycall_micro_create_service`, `polycall_micro_destroy_service`, `polycall_micro_batch_process`, `polycall_micro_collect_garbage`, `polycall_micro_cleanup` | No dedicated `polycall_micro` unit test file found under `libpolycall-v1/test/` |
| `libpolycall-v1/src/polycall_parser.c` | Token-to-AST parser, AST traversal, transform/filter/optimization utilities. | `polycall_parser_create`, `polycall_parser_parse_file`, `polycall_ast_create_node`, `polycall_ast_visit`, `polycall_ast_optimize`, `polycall_ast_validate` | No parser-specific test file found under `libpolycall-v1/test/` |
| `libpolycall-v1/src/polycall_tokenizer.c` | Tokenizer construction and token matching (identifier, number, string, operator). | `polycall_tokenizer_create`, `polycall_tokenizer_reset`, `polycall_tokenizer_match_identifier`, `polycall_tokenizer_match_number`, `polycall_tokenizer_match_string`, `polycall_tokenizer_match_operator` | No tokenizer-specific test file found under `libpolycall-v1/test/` |
| `libpolycall-v1/src/polycall_token.c` | Token array/utilities implementation unit used by tokenizer/parser pipeline. | (token-array functions consumed by tokenizer, e.g. create/destroy array APIs from tokenizer call sites) | No token-specific test file found under `libpolycall-v1/test/` |
| `libpolycall-v1/src/network.c` | Socket endpoint lifecycle, client registration, non-blocking network runtime loop. | `net_init`, `net_close`, `net_send`, `net_receive`, `net_add_client`, `net_remove_client`, `net_run` | No network-specific unit test file found under `libpolycall-v1/test/` |

## Bindings (`bindings/`)

| Binding directory | Purpose | Main entry points/classes/functions | Related tests |
|---|---|---|---|
| `bindings/node-polycall/` | Current Node.js adapter for protocol/state/network abstractions. | `src/index.js` exports `PolyCallClient`, `Router`, `StateMachine`, `State`, `NetworkEndpoint`, `ProtocolHandler` | No dedicated Node test suite directory found in this folder |
| `bindings/pypolycall/` | Current Python adapter with CLI, config, and protocol binding modules. | `pypolycall/core/binding.py`, `pypolycall/cli/main.py`, package entry in `setup.py` | `bindings/pypolycall/tests/...`, plus nested `bindings/pypolycall/pypolycall/tests/...` |
| `bindings/go-polycall/` | Go adapter/client modules and configuration package. | `pkg/client.go`, `config/polycall.go`, `config/src/polycall_client.go` | `bindings/go-polycall/config/tests/polycall_test.go` |
| `bindings/java-polycall/` | Java adapter + JNI bridge to native layer. | `src/main/java/org/obinexus/ffi/LibPolyCallJNI.java`, `.../NativeBinding.java`, `.../core/ProtocolBinding.java`, CLI `.../cli/Main.java` | `src/test/java/org/obinexus/core/ProtocolBindingTest.java`, `src/test/java/org/obinexus/integration/RuntimeConnectionTest.java` |
| `bindings/lua-polycall/` | Lua adapter with CLI, core protocol/state modules, validators/config. | `polycall/init.lua`, `polycall/cli/main.lua`, `polycall/core/*.lua`, `bin/lua-polycall` | No dedicated Lua tests directory found |
| `bindings/node-polycall(outdated)/` | Legacy Node binding snapshot kept for historical/reference purposes. | `examples/` scripts and legacy source layout | No dedicated tests directory found |
| `bindings/pypolycall-outdated/` | Legacy Python binding snapshot (pre-current package layout). | `src/modules/polycall_client.py`, `src/modules/protocol_handler.py`, `src/modules/state_machine.py` | `bindings/pypolycall-outdated/tests/` |

## Notes for contributors

- Treat `bindings/*-outdated/` as historical unless a task explicitly targets them.
- For active Python work, prefer `bindings/pypolycall/` (not `backup_*` snapshots).
- For C core work, start with headers in `libpolycall-v1/include/` and map to implementation in `libpolycall-v1/src/`.
