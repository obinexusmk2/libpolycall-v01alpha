# LibPolyCall Architecture Documentation

## Project Structure Overview

This document outlines the systematic architecture of the LibPolyCall Trial implementation, maintaining clear separation of concerns between binding logic and application projects.

### Directory Architecture

```
libpolycall-trial/
├── bindings/                    # Language binding implementations
│   ├── node-polycall/          # Node.js binding with core modules
│   ├── pypolycall/             # Python binding with enhanced modules
│   └── dual-polycall-experiment/ # Experimental dual-binding protocols
├── projects/                    # Concrete use case implementations
│   ├── banking-system/         # Financial transaction processing
│   ├── todolist-manager/       # Task management system
│   └── books-catalog/          # Library management system
├── examples/                   # Language-specific test clients
├── docs/                       # Comprehensive documentation
└── tools/                      # Development and deployment utilities
```

### Separation of Concerns

**Binding Layer**: 
- Core LibPolyCall communication protocols
- Language-specific binding implementations
- State management and synchronization
- Zero-trust security enforcement

**Application Layer**:
- Concrete business logic implementations
- Professional web interfaces
- Database integration and management
- Comprehensive testing frameworks

### Development Workflow

1. **Binding Development**: Enhance language-specific bindings in `bindings/`
2. **Application Development**: Implement business logic in `projects/`
3. **Integration Testing**: Validate binding-application communication
4. **Production Deployment**: Systematic deployment using documented protocols

### Technical Standards

- **Code Quality**: Professional implementation standards with comprehensive error handling
- **Testing**: Systematic test coverage for all components
- **Documentation**: Clear documentation for all architectural components
- **Security**: Zero-trust principles consistently applied across all layers

Generated: 2025-06-01T04:11:53.099960

## Daemon Lifecycle (libpolycall-v1 CLI)

The `libpolycall-v1` binary now supports foreground and daemonized operation:

1. **Foreground startup (default)**:
   - Process remains attached to terminal.
   - Interactive mode (`stdin`) and non-interactive mode (`-f <config>`) both work as before.

2. **Daemon startup (`--detach`)**:
   - Uses a **double-fork** pattern.
   - Creates a new session via `setsid()`.
   - Redirects `stdin`, `stdout`, and `stderr` to `/dev/null`.
   - Optionally writes a PID file (`--pid-file <path>`).
   - Parent process exits only after child readiness is signaled through an internal pipe.

3. **Shutdown/cleanup behavior**:
   - `SIGINT` and `SIGTERM` set runtime shutdown flags.
   - Main event loops exit naturally, then centralized cleanup runs:
     - network program teardown
     - state machine/context cleanup
     - PID file removal (when daemon mode + PID file is active)

This model keeps startup deterministic while preserving signal-driven resource cleanup.
