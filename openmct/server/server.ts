/**
 * Basic implementation of a history and realtime server.
 */

import { StaticServer } from "./static-server.js";
import { createProxyMiddleware } from "http-proxy-middleware";
import "dotenv/config";
import express from "express";

const TELEMETRY_SERVER_ADDR = `${process.env.TELEMETRY_SERVER_IP}:${process.env.TELEMETRY_SERVER_PORT}`;
const TELEMETRY_SERVER_URL = `http://${TELEMETRY_SERVER_ADDR}`;
const TELEMETRY_REALTIME_SERVER_URL = `ws://${TELEMETRY_SERVER_ADDR}/realtime`;
console.log("telemetry server url", TELEMETRY_SERVER_URL);

const app = express();

await fetch(`${TELEMETRY_SERVER_URL}/test`, {
  signal: AbortSignal.timeout(2000),
}).catch((e) => {
  console.error("Unable to connect to history server");
  process.exit(1);
});

const historyServerProxyMiddleware = createProxyMiddleware({
  target: TELEMETRY_SERVER_URL,
  changeOrigin: true,
  // on: {
  //   proxyReq: (proxyReq, req, res) => {
  //     console.log(
  //       `[PROXY] ${req.method} ${req.url} -> ${TELEMETRY_SERVER_URL}${req}`
  //     );
  //   },
  // },
  pathRewrite: {
    "^": "history", // Re-add history to path because this it is initially removed
  },
});
// proxy requests for /history to the history server
app.use("/history", historyServerProxyMiddleware);
console.log(`/history proxied to ${TELEMETRY_SERVER_URL}/history`);

const realtimeServerProxyMiddleware = createProxyMiddleware({
  target: TELEMETRY_REALTIME_SERVER_URL,
  changeOrigin: true,
  ws: true,
  pathRewrite: {
    "^/realtime": "",
  },
});

// proxy requests for /realtime to the realtime server
app.use("/realtime", realtimeServerProxyMiddleware);

var staticServer = StaticServer();

app.use("/", staticServer);

var port = process.env.PORT || 8080;

const server = app.listen(port, function () {
  console.log("Open MCT hosted at http://localhost:" + port);
  console.log("History server proxied at /history");
  console.log("Realtime server proxied at /realtime");
});

// subscribe to http 'upgrade'
server.on("upgrade", realtimeServerProxyMiddleware.upgrade);
