# Build Command Validation

Use `make -n` to preview link commands without compiling and verify that each target emits a single executable link line with platform libraries applied once.

## Unix-like preview

```bash
make -n bin
```

Expected executable link characteristics:
- Links `build/obj/main.o` with `build/lib/libpolycall.a`.
- Includes Unix-like link libraries once (for example: `-pthread -lssl -lcrypto`).
- Does **not** include duplicated executable link recipes.

## Windows_NT preview

```bash
make -n OS=Windows_NT bin
```

Expected executable link characteristics:
- Produces `build/bin/polycall.exe`.
- Includes `-lws2_32` once through platform-specific libraries.
- Keeps the same single executable link recipe used by other platforms.
