import json
import base64
from dataclasses import dataclass
from typing import Generic, TypeVar
from enum import Enum


class SampleType(str, Enum):
    primitive = "primitive"
    file = "file"

Primitive = int | float | str | bytes | bool

PrimitiveType = TypeVar("PrimitiveType", bound=Primitive)


@dataclass
class Sample(Generic[PrimitiveType]):
    metric_id: str
    timestamp: float
    type: SampleType
    value: PrimitiveType


def encode_primitive(value: Primitive) -> str:
    if isinstance(value, bytes):
        obj = {"type": "bytes", "value": base64.b64encode(value).decode("utf-8")}
    elif isinstance(value, str):
        obj = {"type": "str", "value": value}
    elif isinstance(value, int):
        obj = {"type": "int", "value": value}
    elif isinstance(value, float):
        obj = {"type": "float", "value": value}
    elif isinstance(value, bool):
        obj = {"type": "bool", "value": value}
    else:
        raise ValueError(f"Invalid primitive type: {type(value)}")
    return json.dumps(obj)


def decode_primitive(value: str) -> Primitive:
    obj = json.loads(value)

    if obj["type"] == "bytes":
        assert isinstance(obj["value"], str)
        return base64.b64decode(obj["value"].encode("utf-8"))
    elif obj["type"] == "str":
        assert isinstance(obj["value"], str)
        return obj["value"]
    elif obj["type"] == "int":
        assert isinstance(obj["value"], int)
        return obj["value"]
    elif obj["type"] == "float":
        assert isinstance(obj["value"], float)
        return obj["value"]
    elif obj["type"] == "bool":
        assert isinstance(obj["value"], bool)
        return obj["value"]
    else:
        raise ValueError(f"Invalid primitive type: {obj['type']}")


def encode_sample(metric_id: str, timestamp: float, value: Primitive) -> str:
    sample = Sample(
        metric_id=metric_id, timestamp=timestamp, type=SampleType.primitive, value=value
    )
    obj = {
        "metric_id": sample.metric_id,
        "timestamp": sample.timestamp,
        "type": sample.type,
        "value": encode_primitive(sample.value),
    }
    return json.dumps(obj)


def decode_sample(value: str) -> Sample:
    obj = json.loads(value)
    return Sample(
        metric_id=obj["metric_id"],
        timestamp=obj["timestamp"],
        type=obj["type"],
        value=decode_primitive(obj["value"]),
    )
