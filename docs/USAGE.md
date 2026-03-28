# PolyCall Daemon and Foreground Operation

## Default behavior (foreground)

PolyCall continues to run in foreground mode by default. If you do not pass `--detach`, it stays attached to the current terminal and logs to standard output/error.

## Daemon mode (explicit opt-in)

Daemon mode is **only enabled when `--detach` is passed**.

### CLI flags

- `--detach`: run as a daemon using the standard lifecycle (`fork`, `setsid`, second `fork`).
- `--pid-file <path>`: create and lock a PID file. Startup fails if another live process owns the PID file.
- `--log-file <path>`: redirect daemon stdout/stderr to this file. If omitted, stdout/stderr go to `/dev/null`.
- `-f <config>`: run non-interactive mode from config. Required together with `--detach`.

## Daemon lifecycle semantics

When `--detach` is used:

1. First `fork()`; parent exits immediately.
2. `setsid()` to create a new session.
3. Second `fork()`; intermediate parent exits.
4. `umask(027)` is applied.
5. Process changes directory to `/`.
6. `stdin` redirects to `/dev/null`.
7. `stdout` and `stderr` redirect to `--log-file` if provided, otherwise `/dev/null`.

## PID file behavior

- If `--pid-file` is provided, PolyCall creates the file atomically.
- If the file already exists and points to a dead process, PolyCall removes stale state and retries.
- If the file exists for a live process, startup fails.
- PID file is removed on normal shutdown and signal-triggered shutdown.

## Shutdown and failure semantics

- `SIGINT` and `SIGTERM` use a signal-safe path: the signal handler only sets shutdown flags.
- Runtime cleanup (network teardown, PolyCall cleanup, PID file removal) happens in the main execution path, not from the signal handler.
- Daemon startup fails fast if daemonization, log redirection, or PID lock acquisition fails.

## Examples

Foreground (default):

```bash
./build/bin/polycall -f config.Polycallfile
```

Daemon with PID lock and log file:

```bash
./build/bin/polycall -f config.Polycallfile --detach --pid-file /var/run/polycall.pid --log-file /var/log/polycall.log
```
