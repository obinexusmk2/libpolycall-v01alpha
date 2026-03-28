# Traversal and Index Generation

This repository includes deterministic, stdlib-only indexers under `scripts/`.

## Commands

Run from repository root:

```bash
python3 scripts/index_core_c.py --output docs/generated/CORE_C_INDEX.md
python3 scripts/index_bindings.py --output docs/generated/BINDINGS_INDEX.md
python3 scripts/build_docs_index.py
```

## Expected generated files

- `docs/generated/CORE_C_INDEX.md`
  - Exported APIs from `libpolycall-v1/include/*.h`
  - Key structs from headers
  - File-level responsibility notes for `libpolycall-v1/src/*.c`
- `docs/generated/BINDINGS_INDEX.md`
  - Per-binding language classification
  - CLI entrypoint discovery
  - Runtime assumptions from common metadata files
  - Key modules/classes inventory
- `docs/REFERENCE_INDEX.md`
  - Combined top-level reference index built from the two indexers

## Notes

- Output ordering is deterministic (sorted paths/entries).
- All scripts use Python standard library only.
