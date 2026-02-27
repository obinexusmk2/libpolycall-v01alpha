const http = require("http");

http.createServer((req, res) => {
  res.writeHead(200, { "Content-Type": "application/json" });
  res.end(JSON.stringify({ binding: "SQUARE", status: "ok", endpoint: req.url }));
}).listen(8080, () => {
  console.log("js_polycall_bank listening on :8080");
});
