# PolyCall Usage

## Interactive mode (default)
Run without `-f` to use the CLI shell:

```bash
./bin/polycall
```

This mode stays attached to your terminal and supports commands like `help`, `start_network`, and `quit`.

## Non-interactive mode
Use `-f` to start from a config file:

```bash
./bin/polycall -f config.Polycallfile
```

Example config:

```ini
# config.Polycallfile
server node 3000:8080
server python 3001:8081
port 3000:8080
network start
```

## Detached daemon mode
Non-interactive mode can be daemonized with `--detach`:

```bash
./bin/polycall -f config.Polycallfile --detach
```

Optional daemon flags:

- `--pidfile <path>`: write daemon PID and hold an exclusive lock.
- `--logfile <path>`: redirect stdout/stderr to this file (append mode).

Example:

```bash
./bin/polycall -f config.Polycallfile --detach \
  --pidfile /var/run/polycall.pid \
  --logfile /var/log/polycall.log
```

### Daemon lifecycle
When `--detach` is used, PolyCall follows a dedicated daemonization flow:

1. `fork()` and parent exits.
2. `setsid()` to become session leader.
3. Second `fork()` and parent exits.
4. `umask(027)` and `chdir("/")`.
5. Redirect `stdin` to `/dev/null`; redirect `stdout/stderr` to logfile (or `/dev/null` if omitted).
6. Create and lock pidfile (if provided) and write daemon PID.

On shutdown (`SIGINT`, `SIGTERM`, or normal exit), runtime cleanup releases resources and removes the pidfile.

## Ops commands
Start daemon:

```bash
./bin/polycall -f config.Polycallfile --detach --pidfile /tmp/polycall.pid --logfile /tmp/polycall.log
```

Check status:

```bash
cat /tmp/polycall.pid
ps -fp "$(cat /tmp/polycall.pid)"
```

Tail logs:

```bash
tail -f /tmp/polycall.log
```

Stop daemon:

```bash
kill -TERM "$(cat /tmp/polycall.pid)"
```

Verify port listeners:

```bash
ss -ltnp | grep polycall
```
