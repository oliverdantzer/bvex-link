import os
from bvex_codec.sample import FileSample, FileData
import aiofiles
import numpy as np
from .config import config

    
async def store_file_sample(sample: FileSample):
        assert isinstance(sample.data, FileData)
        metric_dir = os.path.join(
            config.FILE_STORE_DIR_ABSPATH, f"metric-{sample.metadata.metric_id}")
        if not os.path.exists(metric_dir):
            os.makedirs(metric_dir)
        extension = sample.data.filename.split(".")[-1]
        sample_file = os.path.join(
            metric_dir, f"{sample.metadata.timestamp}.{extension}")
        async with aiofiles.open(sample_file, "wb") as f:
            await f.write(sample.data.data)