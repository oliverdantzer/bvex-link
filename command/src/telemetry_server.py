import asyncio
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from uvicorn import Config, Server
from bvex_codec import Sample, FileData
import redis.asyncio as redis
import time
from fastapi import Query
from fastapi.staticfiles import StaticFiles
from datetime import datetime
import dateutil.parser as dp
from .telemetry_realtime_manager import RealtimeTelemetryConnectionManager
from .telemetry_history_manager import TelemetryHistoryManager
from .telemetry_realtime_interactive import http_endpoint_realtime_interactive
import os
from .store_sample import SampleStore

TELEMETRY_HISTORY_KEY = "telemetry_history"


class TelemetryServer:
    def __init__(self, port: int):
        self.app = FastAPI()
        # Enable CORS
        self.app.add_middleware(
            CORSMiddleware,
            allow_origins=["*"],  # Adjust this to restrict origins
            allow_credentials=True,
            allow_methods=["*"],
            allow_headers=["*"],
        )

        self.sample_store = SampleStore(os.path.relpath("data"))

        # Create ./files directory if it doesn't exist
        os.makedirs("./files", exist_ok=True)

        # exposes "http://0.0.0.0:port/images"
        self.app.mount("/files", StaticFiles(directory="./files"), name="images")

        # exposes "http://0.0.0.0:port/{metric_id}?start={num1}&end={num2}"
        self.history_manager = TelemetryHistoryManager()
        self.app.add_api_route(
            "/history/samples/{metric_id}",
            self.history_manager.http_endpoint,
            methods=["GET"],
        )

        self.realtime_manager = RealtimeTelemetryConnectionManager()
        self.app.add_api_websocket_route(
            "/realtime", self.realtime_manager.websocket_endpoint
        )

        realtime_interactive_endpoint = http_endpoint_realtime_interactive(port)
        self.app.add_api_route("/realtime-interactive", realtime_interactive_endpoint)

        self.app.add_api_route("/test", lambda: "Hello World!", methods=["GET"])

        self.config = Config(
            app=self.app, host="0.0.0.0", port=port, log_level="warning"
        )
        self.server = Server(self.config)

    async def add_sample(self, sample: Sample):
        async with asyncio.TaskGroup() as tg:
            tg.create_task(self.realtime_manager.notify_all_subscribers(sample))
            tg.create_task(self.history_manager.add_sample(sample))
            if isinstance(sample.data, FileData):
                tg.create_task(self.sample_store.store_sample(sample))

    async def run(self):
        await self.server.serve()
