#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN_PATH="$ROOT_DIR/build/bin/polycall"

if [[ ! -x "$BIN_PATH" ]]; then
  echo "binary not found: $BIN_PATH" >&2
  echo "hint: run 'make -C $ROOT_DIR bin' first" >&2
  exit 1
fi

TMP_DIR="$(mktemp -d)"
PID_FILE="$TMP_DIR/polycall.pid"
LOG_FILE="$TMP_DIR/polycall.log"
CFG_FILE="$TMP_DIR/polycall.conf"
LAUNCHER="$TMP_DIR/launcher.sh"

cleanup() {
  if [[ -f "$PID_FILE" ]]; then
    pid="$(cat "$PID_FILE" || true)"
    if [[ -n "${pid:-}" ]] && kill -0 "$pid" 2>/dev/null; then
      kill "$pid" || true
      sleep 1
      kill -9 "$pid" 2>/dev/null || true
    fi
  fi
  rm -rf "$TMP_DIR"
}
trap cleanup EXIT

cat > "$CFG_FILE" <<'CFG'
network start
CFG

cat > "$LAUNCHER" <<EOF2
#!/usr/bin/env bash
exec "$BIN_PATH" --detach --pid-file "$PID_FILE" --log-file "$LOG_FILE" -f "$CFG_FILE"
EOF2
chmod +x "$LAUNCHER"

"$LAUNCHER"

for _ in $(seq 1 40); do
  if [[ -s "$PID_FILE" ]]; then
    break
  fi
  sleep 0.25
done

if [[ ! -s "$PID_FILE" ]]; then
  echo "pid file was not created" >&2
  exit 1
fi

DAEMON_PID="$(tr -d '[:space:]' < "$PID_FILE")"
if [[ -z "$DAEMON_PID" ]]; then
  echo "pid file was empty" >&2
  exit 1
fi

if ! kill -0 "$DAEMON_PID" 2>/dev/null; then
  echo "daemon process $DAEMON_PID is not alive" >&2
  exit 1
fi

PPID_VALUE="$(ps -o ppid= -p "$DAEMON_PID" | tr -d '[:space:]')"
if [[ -z "$PPID_VALUE" ]]; then
  echo "failed to read daemon parent pid" >&2
  exit 1
fi

if [[ "$PPID_VALUE" -ne 1 ]]; then
  echo "daemon was not re-parented to init (ppid=$PPID_VALUE)" >&2
  exit 1
fi

kill "$DAEMON_PID"
for _ in $(seq 1 40); do
  if [[ ! -e "$PID_FILE" ]]; then
    break
  fi
  sleep 0.25
done

if [[ -e "$PID_FILE" ]]; then
  echo "pid file was not removed on shutdown" >&2
  exit 1
fi

echo "daemon detach check passed"
