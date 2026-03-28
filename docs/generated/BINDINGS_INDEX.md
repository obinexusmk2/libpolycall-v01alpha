# Bindings Reference Index

## Scope

- Directories scanned: `bindings/*`

## `bindings/go-polycall`

- Language: **Go**
- CLI entrypoints:
  - _(none detected)_
- Runtime assumptions:
  - go: 1.21
- Key modules/classes:
  - `bindings/go-polycall/config/examples/example_client.go`
  - `bindings/go-polycall/config/polycall.go`
  - `bindings/go-polycall/config/src/polycall_client.go`
  - `bindings/go-polycall/config/tests/polycall_test.go`
  - `bindings/go-polycall/examples/test_client_api.go`
  - `bindings/go-polycall/pkg/client.go`

## `bindings/java-polycall`

- Language: **Java**
- CLI entrypoints:
  - `bindings/java-polycall/scripts/build.sh`
  - `bindings/java-polycall/scripts/test-runtime.sh`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/Main.java`
- Runtime assumptions:
  - maven.compiler.source: 17
  - maven.compiler.target: 17
- Key modules/classes:
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/Main.java`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/Main.java::Main`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/commands/InfoCommand.java`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/commands/InfoCommand.java::InfoCommand`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/commands/TelemetryCommand.java`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/commands/TelemetryCommand.java::TelemetryCommand`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/commands/TestCommand.java`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/commands/TestCommand.java::TestCommand`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/consumer-core/CommandRegistry.java`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/consumer-core/CommandRegistry.java::CommandRegistry`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/consumer-core/ExtensionManager.java`
  - `bindings/java-polycall/src/main/java/org/obinexus/cli/consumer-core/ExtensionManager.java::ExtensionManager`
  - `bindings/java-polycall/src/main/java/org/obinexus/config/ConfigManager.java`
  - `bindings/java-polycall/src/main/java/org/obinexus/config/ConfigManager.java::ConfigManager`
  - `bindings/java-polycall/src/main/java/org/obinexus/core/ProtocolBinding.java`
  - `bindings/java-polycall/src/main/java/org/obinexus/core/ProtocolBinding.java::ProtocolBinding`
  - `bindings/java-polycall/src/main/java/org/obinexus/core/ProtocolHandler.java`
  - `bindings/java-polycall/src/main/java/org/obinexus/core/ProtocolHandler.java::ProtocolHandler`
  - `bindings/java-polycall/src/main/java/org/obinexus/core/StateManager.java`
  - `bindings/java-polycall/src/main/java/org/obinexus/core/StateManager.java::StateManager`

## `bindings/lua-polycall`

- Language: **Lua**
- CLI entrypoints:
  - `bindings/lua-polycall/bin/lua-polycall`
  - `bindings/lua-polycall/polycall/cli/main.lua`
- Runtime assumptions:
  - No explicit runtime metadata detected for Lua.
- Key modules/classes:
  - `bindings/lua-polycall/examples/lua-polycall-examples.lua`
  - `bindings/lua-polycall/examples/server.lua`
  - `bindings/lua-polycall/examples/server.lua::connections`
  - `bindings/lua-polycall/examples/server.lua::instance`
  - `bindings/lua-polycall/polycall/cli.lua`
  - `bindings/lua-polycall/polycall/cli.lua::cli`
  - `bindings/lua-polycall/polycall/cli/commands/info.lua`
  - `bindings/lua-polycall/polycall/cli/commands/info.lua::info_command`
  - `bindings/lua-polycall/polycall/cli/commands/telemetry.lua`
  - `bindings/lua-polycall/polycall/cli/commands/telemetry.lua::observations`
  - `bindings/lua-polycall/polycall/cli/commands/telemetry.lua::telemetry_command`
  - `bindings/lua-polycall/polycall/cli/commands/test.lua`
  - `bindings/lua-polycall/polycall/cli/commands/test.lua::test_command`
  - `bindings/lua-polycall/polycall/cli/main.lua`
  - `bindings/lua-polycall/polycall/cli/main.lua::cli`
  - `bindings/lua-polycall/polycall/cli/main.lua::command_args`
  - `bindings/lua-polycall/polycall/cli/main.lua::filtered_args`
  - `bindings/lua-polycall/polycall/cli/registry.lua`
  - `bindings/lua-polycall/polycall/cli/registry.lua::command_list`
  - `bindings/lua-polycall/polycall/cli/registry.lua::commands`

## `bindings/node-polycall`

- Language: **JavaScript/Node.js**
- CLI entrypoints:
  - `package.json:scripts:start`
- Runtime assumptions:
  - No explicit runtime metadata detected for JavaScript/Node.js.
- Key modules/classes:
  - `bindings/node-polycall/examples/server.js`
  - `bindings/node-polycall/src/index.js`
  - `bindings/node-polycall/src/modules/NetworkEndpoint.js`
  - `bindings/node-polycall/src/modules/NetworkEndpoint.js::NetworkEndpoint`
  - `bindings/node-polycall/src/modules/PolyCallClient.js`
  - `bindings/node-polycall/src/modules/PolyCallClient.js::PolyCallClient`
  - `bindings/node-polycall/src/modules/ProtocolHandler.js`
  - `bindings/node-polycall/src/modules/ProtocolHandler.js::ProtocolHandler`
  - `bindings/node-polycall/src/modules/Router.js`
  - `bindings/node-polycall/src/modules/Router.js::Router`
  - `bindings/node-polycall/src/modules/State.js`
  - `bindings/node-polycall/src/modules/State.js::State`
  - `bindings/node-polycall/src/modules/StateMachine.js`
  - `bindings/node-polycall/src/modules/StateMachine.js::StateMachine`

## `bindings/node-polycall(outdated)`

- Language: **JavaScript/Node.js**
- CLI entrypoints:
  - _(none detected)_
- Runtime assumptions:
  - No explicit runtime metadata detected for JavaScript/Node.js.
- Key modules/classes:
  - `bindings/node-polycall(outdated)/examples/banking-api/src/server.js`
  - `bindings/node-polycall(outdated)/examples/banking-api/src/server.js::BankingAPIServer`
  - `bindings/node-polycall(outdated)/examples/test_client_api.js`

## `bindings/pypolycall`

- Language: **Python**
- CLI entrypoints:
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/cli/main.py`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/pypolycall/cli/main.py`
  - `bindings/pypolycall/pypolycall/cli/main.py`
  - `bindings/pypolycall/pypolycall/pypolycall/cli/main.py`
  - `setup.py:entry_points -> "console_scripts": [ "pypolycall=pypolycall.cli.main:main", ],`
- Runtime assumptions:
  - No explicit runtime metadata detected for Python.
- Key modules/classes:
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/__init__.py`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/cli/__init__.py`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/cli/main.py`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/cli/main.py::CLI`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/cli/registry.py`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/cli/registry.py::CommandRegistry`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/config/__init__.py`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/config/manager.py`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/config/manager.py::ConfigManager`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/core/__init__.py`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/core/binding.py`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/core/binding.py::ProtocolBinding`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/core/protocol/__init__.py`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/core/protocol/__init__.py::MessageTypes`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/core/protocol/__init__.py::ProtocolHandler`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/core/protocol/__init__.py::StateTransitions`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/core/telemetry/__init__.py`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/core/telemetry/__init__.py::MetricsCollector`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/core/telemetry/__init__.py::TelemetryObserver`
  - `bindings/pypolycall/backup_20250603_233522/pypolycall/pypolycall/__init__.py`

## `bindings/pypolycall-outdated`

- Language: **Python**
- CLI entrypoints:
  - _(none detected)_
- Runtime assumptions:
  - No explicit runtime metadata detected for Python.
- Key modules/classes:
  - `bindings/pypolycall-outdated/__init__.py`
  - `bindings/pypolycall-outdated/examples/test_client_api.py`
  - `bindings/pypolycall-outdated/src/__init__.py`
  - `bindings/pypolycall-outdated/src/modules/__init__.py`
  - `bindings/pypolycall-outdated/src/modules/network_endpoint.py`
  - `bindings/pypolycall-outdated/src/modules/network_endpoint.py::NetworkEndpoint`
  - `bindings/pypolycall-outdated/src/modules/polycall_client.py`
  - `bindings/pypolycall-outdated/src/modules/polycall_client.py::PolycallClient`
  - `bindings/pypolycall-outdated/src/modules/protocol_handler.py`
  - `bindings/pypolycall-outdated/src/modules/protocol_handler.py::ProtocolHandler`
  - `bindings/pypolycall-outdated/src/modules/router.py`
  - `bindings/pypolycall-outdated/src/modules/router.py::Router`
  - `bindings/pypolycall-outdated/src/modules/state_machine.py`
  - `bindings/pypolycall-outdated/src/modules/state_machine.py::StateMachine`
