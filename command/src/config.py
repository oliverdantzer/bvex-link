from pydantic import BaseModel
import os

class Config(BaseModel):
    TELEMETRY_SERVER_PORT: int = 8000
    ONBOARD_SERVER_IP: str = "localhost"
    ONBOARD_SERVER_PORT: int = 8888
    DEFAULT_MAX_BPS: int = 100000
    DEFAULT_BPS_DISTRIBUTION_MODE: str = "fair"

config = Config.model_validate(os.environ)