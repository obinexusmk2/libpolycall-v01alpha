# LibPolyCall Usage Guide (`libpolycall-v1`)

This guide is a linear, repository-accurate setup for the C runtime in `libpolycall-v1/` and the language bindings in `bindings/`.

## 1) Prerequisites

- C toolchain with `gcc`/`make`.
- OpenSSL development libraries (`-lssl`, `-lcrypto`) and pthread support (used by the Makefile).
- Optional per-binding tools:
  - Java 17+ + Maven (`bindings/java-polycall/`)
  - Python 3.8+ + `pip` (`bindings/pypolycall/`)
  - Node.js + npm (`bindings/node-polycall/`)
  - Go 1.21+ (`bindings/go-polycall/`)
  - Lua interpreter + LuaRocks-compatible environment (`bindings/lua-polycall/`)

## 2) Build and run `libpolycall-v1`

From the repo root:

```bash
cd libpolycall-v1
make all
```

Build outputs:

- Static library: `libpolycall-v1/build/lib/libpolycall.a`
- Shared library: `libpolycall-v1/build/lib/libpolycall.so` (or `.dll` on Windows)
- Runtime binary: `libpolycall-v1/build/bin/polycall`

Run the runtime (interactive mode):

```bash
./build/bin/polycall
```

Run with an explicit configuration file:

```bash
./build/bin/polycall -f config.Polycallfile
```

Useful build variants:

```bash
make debug
make release
make clean
```

## 3) Configuration examples (matching `libpolycall-v1/config.Polycallfile`)

The checked-in baseline file is:

```ini
# Language Server Definitions
server node 8080:8084
server python 3001:8084
server java 3002:8082
server go 3003:8083

# Network Configuration
network start
network_timeout=5000
max_connections=1000

# Global Settings
log_directory=/var/log/polycall
workspace_root=/opt/polycall

# Service Discovery
auto_discover=true
discovery_interval=60

# Security Configuration
tls_enabled=true
cert_file=/etc/polycall/cert.pem
key_file=/etc/polycall/key.pem

# Resource Limits
max_memory_per_service=1G
max_cpu_per_service=2

# Monitoring
enable_metrics=true
metrics_port=9090
```

Minimal local-development variant (same keys, local paths):

```ini
server node 8080:8084
server python 3001:8084
server java 3002:8082
server go 3003:8083

network start
network_timeout=5000
max_connections=1000

log_directory=./logs
workspace_root=.

auto_discover=true
discovery_interval=60

tls_enabled=false
cert_file=./certs/dev-cert.pem
key_file=./certs/dev-key.pem

max_memory_per_service=1G
max_cpu_per_service=2

enable_metrics=true
metrics_port=9090
```

## 4) Binding entry points by language

These are the primary runtime or CLI entry points visible in the current tree.

### C runtime
- Binary entry: `libpolycall-v1/main.c`
- Build target: `libpolycall-v1/build/bin/polycall`

### Java
- CLI main class: `org.obinexus.cli.Main`
- Source entry file: `bindings/java-polycall/src/main/java/org/obinexus/cli/Main.java`
- Packaged execution (after Maven build):
  ```bash
  java -jar bindings/java-polycall/target/java-polycall-1.0.0-jar-with-dependencies.jar
  ```

### Python
- Console script: `pypolycall`
- Setup mapping: `pypolycall=pypolycall.cli.main:main`
- Module entry file: `bindings/pypolycall/pypolycall/cli/main.py`

### Node.js
- Package main: `bindings/node-polycall/src/index.js`
- Start command:
  ```bash
  npm --prefix bindings/node-polycall start
  ```

### Go
- Public client surface: `bindings/go-polycall/pkg/client.go`
- Example entrypoint:
  `bindings/go-polycall/examples/test_client_api.go`

### Lua
- CLI shim: `bindings/lua-polycall/bin/lua-polycall`
- Lua CLI module: `bindings/lua-polycall/polycall/cli/main.lua`

## Related docs

- [Architecture](architecture/ARCHITECTURE.md)
- [Traversal map](TRAVERSAL.md)
- [Plan/index](PLAN.md)
