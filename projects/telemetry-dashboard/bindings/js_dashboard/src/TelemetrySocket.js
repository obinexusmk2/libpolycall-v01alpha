export function connectTelemetry(onMessage) {
  const ws = new WebSocket("ws://localhost:9090/telemetry");
  ws.onmessage = (event) => onMessage(JSON.parse(event.data));
  return ws;
}
