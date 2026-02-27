# Banking Secure Bridge

This sample demonstrates the **RECTANGLE driver** pattern (COBOL/Python/Node interfaces) around a C core daemon.

## Highlights
- Zero-trust header (`PolyCall_Header`) with SHA-256 seed and CRC16 integrity check.
- Telemetry state logging (`Parse -> Validate -> Marshal`) with session + sequence fields.
- Multi-language bindings that treat every binding as a driver surface.

## Build
```bash
make
./banking_bridge
```

## vcpkg/CMake usage
```cmake
find_package(libpolycall CONFIG REQUIRED)
target_link_libraries(banking_bridge PRIVATE libpolycall::libpolycall)
```
