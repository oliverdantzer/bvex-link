/**
 * Basic implementation of a history and realtime server.
 */

import { StaticServer } from "./static-server.js";
import { createProxyMiddleware } from "http-proxy-middleware";
import "dotenv/config";
import express from "express";

const TELEMETRY_SERVER_ADDR = `${process.env.TELEMETRY_SERVER_IP}:${process.env.TELEMETRY_SERVER_PORT}`;
const TELEMETRY_SERVER_URL = `http://${TELEMETRY_SERVER_ADDR}`;
console.log("Telemetry server url:", TELEMETRY_SERVER_URL);
const TELEMETRY_REALTIME_SERVER_URL = `ws://${TELEMETRY_SERVER_ADDR}/realtime`;

const app = express();

await fetch(`${TELEMETRY_SERVER_URL}/test`).catch((e) => {
  console.error("Unable to connect to history server");
  process.exit(1);
});

const historyServerProxyMiddleware = createProxyMiddleware({
  target: TELEMETRY_SERVER_URL,
  changeOrigin: true,
});
// proxy requests for /history to the history server
app.use("/history", historyServerProxyMiddleware);

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
