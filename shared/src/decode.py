from PIL import Image
import io


def decode_img(data: bytes) -> Image.Image:
    byte_stream = io.BytesIO(data)
    reconstructed_image = Image.open(byte_stream, formats=["webp"])
    return reconstructed_image
