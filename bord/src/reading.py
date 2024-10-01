import time
import math
from shared.src.sample import Sample
from PIL import Image
from shared.src.encode import encode_img

def get_temp_reading() -> Sample:
    """Simulate a temperature oscillating between 10 and 30 degrees over a period of 30 seconds."""
    t = int(time.time())
    period = 30
    value = 10 * math.sin((2*math.pi*t)/period) + 20
    sample = Sample("temp", t, str(value).encode('utf-8'))
    return sample

def get_img_reading() -> Sample:
    image_path = "bord/src/mountain.webp"
    return Sample("img", int(time.time()), encode_img(Image.open(image_path)))
    