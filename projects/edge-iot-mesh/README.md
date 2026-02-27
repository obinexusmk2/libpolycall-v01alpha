# Edge IoT Mesh

This sample demonstrates edge caching + trigger updates with **driver-oriented** mesh routing.

## Highlights
- Lightweight edge node process for chained regions (US -> UK -> France).
- Delta trigger update and conflict-resolution placeholder hooks.
- Multi-language sensor emitters (Python, Go, embedded C).

## Build
```bash
make
./edge_node
```

## vcpkg/CMake usage
```cmake
find_package(libpolycall CONFIG REQUIRED)
target_link_libraries(edge_node PRIVATE libpolycall::libpolycall)
```
