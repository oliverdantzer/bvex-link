import express from "express";
import path from "path";
import { fileURLToPath } from "url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

export function StaticServer() {
  var router = express.Router();
  const rootDirectoryPath = path.join(__dirname, "../..");
  router.use("/", express.static(rootDirectoryPath));

  return router;
}
