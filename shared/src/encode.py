import json

def encode_json(data) -> bytes:
    json_string = json.dumps(data)
    return json_string.encode('utf-8')