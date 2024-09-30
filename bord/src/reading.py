import time
import math
from shared.src.sample import Sample

def get_temp_reading():
    """Simulate a temperature oscillating between 10 and 30 degrees over a period of 30 seconds."""
    t = int(time.time())
    period = 30
    value = 10 * math.sin((2*math.pi*t)/period) + 20
    sample = Sample("temp", t, str(value).encode('utf-8'))
    