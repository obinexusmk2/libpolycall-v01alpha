# LibPolyCall Usage Guide

## Document map
- Trial guide: [`docs/TRIAL.md`](./TRIAL.md)
- Core architecture: [`docs/architecture/ARCHITECTURE.md`](./architecture/ARCHITECTURE.md)
- Binding references:
  - Node.js: [`bindings/node-polycall/README.md`](../bindings/node-polycall/README.md)
  - Go: [`bindings/go-polycall/README.md`](../bindings/go-polycall/README.md)
  - Java: [`bindings/java-polycall/README.md`](../bindings/java-polycall/README.md)
  - Python: [`bindings/pypolycall/README.md`](../bindings/pypolycall/README.md)

## 1) Prerequisites
- Build (or otherwise provide) the LibPolyCall runtime binary at `libpolycall-v1/bin/polycall`.
- Use the repository-root working directory for commands that start with `./libpolycall-v1/...`.
- Ensure binding runtimes are installed as needed:
  - Node.js for `bindings/node-polycall`
  - Go toolchain for `bindings/go-polycall`
  - Java (and Maven if building) for `bindings/java-polycall`
  - Python for `bindings/pypolycall`

```bash
# from repository root
[ -x ./libpolycall-v1/bin/polycall ] && echo "polycall runtime found" || echo "polycall runtime missing"
```

## 2) Config file examples

### 2.1 Main runtime config (`libpolycall-v1/config.Polycallfile`)

```ini
# Language server declarations
server node 8080:8084
server python 3001:8084
server java 3002:8082
server go 3003:8083

# Network startup
network start
network_timeout=5000
```

### 2.2 Minimal non-interactive config
Use this minimal form when you only want to verify startup behavior quickly.

```ini
port 8080:8084
network start
```

### 2.3 Optional binding-local config examples

```ini
# Example: bindings/go-polycall/config/go.polycallrc
port=3003:8083
server_type=go
workspace=/opt/polycall/services/go
```

```ini
# Example: /opt/polycall/services/node/.polycallrc
port=8080:8084
server_type=node
workspace=/opt/polycall/services/node
```

## 3) Interactive mode
Start the CLI without `-f`.

```bash
./libpolycall-v1/bin/polycall
```

Expected startup banner:

```text
PolyCall CLI v1.0.0 - Type 'help' for commands
```

Useful interactive commands:

```text
help
start_network
list_endpoints
list_clients
status
quit
```

## 4) Non-interactive mode
Run the runtime with a config file path.

```bash
./libpolycall-v1/bin/polycall -f ./libpolycall-v1/config.Polycallfile
```

Expected startup lines include:

```text
Network services started on port 8080
Running in non-interactive mode...
```

## 5) Binding startup
Start bindings from repository-correct paths under `bindings/*`.

### Node.js binding

```bash
cd ./bindings/node-polycall
npm install
node ./examples/server.js
```

### Go binding (example client)

```bash
cd ./bindings/go-polycall
go run ./config/examples/example_client.go
```

### Java binding (CLI jar)

```bash
cd ./bindings/java-polycall
mvn -q package
java -jar ./target/java-polycall-1.0.0-jar-with-dependencies.jar info
```

### Python binding (CLI module)

```bash
cd ./bindings/pypolycall
python -m pypolycall.cli.main info --detailed
```

## 6) Troubleshooting

### Runtime binary not found

```bash
ls -l ./libpolycall-v1/bin/polycall
```

### Config file path issues

```bash
ls -l ./libpolycall-v1/config.Polycallfile
```

### Port already in use

```bash
ss -ltnp | rg ':8080|:8084|:3001|:3002|:3003'
```

### Node.js dependency or startup errors

```bash
cd ./bindings/node-polycall && npm install && node ./examples/server.js
```

### Python module resolution errors

```bash
cd ./bindings/pypolycall && python -m pypolycall.cli.main test --host localhost --port 8084
```

## Quick verification
Run these checks from repository root after setup.

```bash
# 1) Runtime exists
[ -x ./libpolycall-v1/bin/polycall ] && echo "OK: runtime present"

# 2) Config exists
[ -f ./libpolycall-v1/config.Polycallfile ] && echo "OK: config present"

# 3) Interactive banner check (first line)
./libpolycall-v1/bin/polycall 2>&1 | head -n 1

# 4) Non-interactive startup check (expect startup lines)
./libpolycall-v1/bin/polycall -f ./libpolycall-v1/config.Polycallfile 2>&1 | head -n 5
```

Expected outcomes:

```text
OK: runtime present
OK: config present
PolyCall CLI v1.0.0 - Type 'help' for commands
Network services started on port 8080
Running in non-interactive mode...
```
