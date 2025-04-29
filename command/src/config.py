from pydantic import BaseModel
import os

class Config(BaseModel):
    TELEMETRY_SERVER_PORT: int = 8000
    REMOTE_IP_ADDR: str = "localhost"
    REMOTE_PORT: int = 8888

config = Config.model_validate(os.environ)