# LibPolyCall v1 (alpha)

LibPolyCall is a program-first, cross-language runtime centered on a C core (`libpolycall-v1`) plus language bindings.

## Start Here

- **Repository traversal map**: [`docs/TRAVERSAL.md`](docs/TRAVERSAL.md)
- **Build and runtime usage**: [`docs/USAGE.md`](docs/USAGE.md)
- **Architecture snapshot**: [`docs/architecture/ARCHITECTURE.md`](docs/architecture/ARCHITECTURE.md)

## Quick Start (Core Runtime)

```bash
cd libpolycall-v1
make clean
make bin
./build/bin/polycall
```

For deterministic config-driven startup:

```bash
./build/bin/polycall -f config.Polycallfile
```

## Binding Docs

- Node.js: [`bindings/node-polycall/README.md`](bindings/node-polycall/README.md)
- Python: [`bindings/pypolycall/README.md`](bindings/pypolycall/README.md)
- Go: [`bindings/go-polycall/README.md`](bindings/go-polycall/README.md)
- Lua: `bindings/lua-polycall/` (module + CLI sources)
- Java: [`bindings/java-polycall/README.md`](bindings/java-polycall/README.md)

## Example Consumers

- Small clients: [`examples/`](examples/)
- Scenario projects: [`projects/`](projects/)

## License

MIT (see [`LICENSE`](LICENSE)).
