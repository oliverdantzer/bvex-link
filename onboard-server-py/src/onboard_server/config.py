from pydantic import BaseModel
import os

class Config(BaseModel):
    ONBOARD_SERVER_PORT: int = 8888

config = Config.model_validate(os.environ)