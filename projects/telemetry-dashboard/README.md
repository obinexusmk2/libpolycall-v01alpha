# Telemetry Dashboard

This sample demonstrates a **SQUARE binding** flow where Python/Go/Lua/JS observers consume equivalent Canonical IR values.

## Highlights
- Canonical IR value definition with GUID trace field.
- Placeholder Huffman-AVL transform hook and state snapshot logger.
- Reproduction script to replay a GUID trail.

## Build
```bash
make
./telemetry_server
```

## vcpkg/CMake usage
```cmake
find_package(libpolycall CONFIG REQUIRED)
target_link_libraries(telemetry_server PRIVATE libpolycall::libpolycall)
```
