# Usage: practical "how to run"

This guide only includes commands validated from the repository root (`/workspace/libpolycall-v01alpha`) in this environment.

## 1) Run the prebuilt core CLI (`libpolycall-v1`)

```bash
chmod +x libpolycall-v1/bin/polycall
printf 'help\nquit\n' | libpolycall-v1/bin/polycall
```

What this does:
- marks the bundled CLI binary executable,
- starts it,
- prints command help,
- exits cleanly.

## 2) Smoke test the Python binding tests (`bindings/pypolycall`)

```bash
pytest -q bindings/pypolycall/tests/unit/config/test_manager.py
```

## 3) Smoke test the Node.js binding module export (`bindings/node-polycall`)

```bash
node -e "const m=require('./bindings/node-polycall/src/index.js'); console.log(Object.keys(m))"
```

Expected result: a list of exported modules such as `PolyCallClient`, `Router`, and `ProtocolHandler`.

## 4) Optional: inspect major docs quickly

```bash
sed -n '1,160p' README.md
sed -n '1,200p' docs/architecture/ARCHITECTURE.md
sed -n '1,240p' docs/REFERENCE.md
```

## Notes

- The build path exists at `libpolycall-v1/Makefile`, but in this repo snapshot `make -C libpolycall-v1 all` currently fails due duplicate/corrupted content in `libpolycall-v1/src/polycall_tokenizer.c`. That compile command is intentionally **not** part of the run baseline.
- All paths in this guide are repo-relative.
