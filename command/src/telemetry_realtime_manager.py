from bvex_codec import Sample
from websockets.asyncio.server import serve, ServerConnection
from fastapi import WebSocket, WebSocketDisconnect
from typing import Dict
import asyncio

class RealtimeTelemetryConnectionManager:
    def __init__(self):
        self.active_connections: Dict[WebSocket, set] = {}

    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections[websocket] = set()

    def disconnect(self, websocket: WebSocket):
        del self.active_connections[websocket]

    async def notify_all_subscribers(self, sample: Sample):
        # Notify all connected clients.
        async with asyncio.TaskGroup() as tg:
            for websocket, subscribed_metric_ids in self.active_connections.items():
                if sample.metadata.metric_id in subscribed_metric_ids:
                    tg.create_task(websocket.send_text(sample.model_dump_json()))
    
    async def websocket_endpoint(self, websocket: WebSocket):
        await self.connect(websocket)
        print("connected")
        try:
            while True:
                message = await websocket.receive_text()
                if message.startswith("subscribe"):
                    metric_id = message.split(" ")[1]
                    self.active_connections[websocket].add(metric_id)
                elif message.startswith("unsubscribe"):
                    metric_id = message.split(" ")[1]
                    self.active_connections[websocket].discard(metric_id)
                else:
                    print(f"websocket received unknown message: {message}")
        except WebSocketDisconnect:
            self.disconnect(websocket)