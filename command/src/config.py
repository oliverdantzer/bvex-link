from pydantic import BaseModel
import os

class Config(BaseModel):
    TELEMETRY_SERVER_PORT: int = 8000
    ONBOARD_SERVER_IP: str = "localhost"
    ONBOARD_SERVER_PORT: int = 8888

config = Config.model_validate(os.environ)