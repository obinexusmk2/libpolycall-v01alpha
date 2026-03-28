# Traversal Guide

This guide describes a predictable reading order for repository exploration.
It consumes the machine-generated index at `docs/generated/docs_index.md`.

## Recommended Order

1. Read high-level orientation in `README.md`.
2. Review architecture in `docs/architecture/ARCHITECTURE.md`.
3. Open `docs/generated/docs_index.md` for the canonical list of docs and decision states.
4. Use `docs/generated/component_inventory.md` to map runtime components to language bindings.

## Regenerating Inputs

Run the tooling before traversals to keep the index and inventory current:

```bash
python3 tools/scripts/analyze_components.py
python3 tools/scripts/generate_docs_index.py
```

## Generated Artifact

- [Documentation Index](generated/docs_index.md)
