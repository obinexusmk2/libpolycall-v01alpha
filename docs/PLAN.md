# Documentation Consistency Plan

## Goal
Keep onboarding and architecture docs accurate to the current repository layout and runnable paths.

## Required pre-merge checklist (docs changes)

- [ ] **Path validation completed:** every file/dir path mentioned in docs exists in the repository at review time.
- [ ] **Command validation completed:** every command in `docs/USAGE.md` was executed from repo root and behavior confirmed.
- [ ] **No absolute local paths:** docs use repo-relative paths unless an explicit external system path is required.
- [ ] **No duplicated guidance:** command snippets and setup steps are defined once and linked, not copied across files.
- [ ] **Architecture tree refreshed:** `docs/architecture/ARCHITECTURE.md` reflects current top-level structure.
- [ ] **Reference map refreshed:** `docs/REFERENCE.md` still maps major C sources and binding directories to entry points/tests.
- [ ] **README Start Here links checked:** links resolve and match current docs filenames.

## Suggested validation commands

```bash
# repo root
find docs libpolycall-v1 bindings projects tools -maxdepth 4 -type d | head

# verify docs links target existing files
test -f docs/USAGE.md
test -f docs/architecture/ARCHITECTURE.md
test -f docs/REFERENCE.md
test -f docs/PLAN.md

# optional: smoke-check usage commands
printf 'help\nquit\n' | libpolycall-v1/bin/polycall
pytest -q bindings/pypolycall/tests/unit/config/test_manager.py
node -e "const m=require('./bindings/node-polycall/src/index.js'); console.log(Object.keys(m).length)"
```

## Merge gate rule
A docs PR should not be merged until the **Path validation completed** checkbox is checked.
