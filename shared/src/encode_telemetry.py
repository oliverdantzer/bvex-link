from typing import TypedDict, Union
import json
import base64


class SegmentationParams(TypedDict):
    seq_num: int
    segment_size: int
    num_segments: int
    segment_data: bytes


class SingletonDataDict(TypedDict):
    metric_id: str
    sample_time: int
    sample_data: bytes


class SegmentedDataDict(TypedDict):
    metric_id: str
    sample_time: int
    sample_data_segment: SegmentationParams


class SampleDatagram:
    # TODO: change to data: Union[SingletonDataDict, SegmentedDataDict]
    def __init__(self, data: SegmentedDataDict):
        self.data = data

    def get_id(self) -> str:
        return self.data['metric_id'] + str(self.data['sample_time'])

    def size(self):
        return len(json.dumps(self.data))

    def json_dict(self):
        return self.data.copy()


class BytesEncoder(json.JSONEncoder):
    def default(self, o):
        if isinstance(o, bytes):
            return base64.b64encode(o).decode("ascii")
        else:
            return super().default(o)


class TelemetryPayload:
    def __init__(self, max_size: int):
        self.samples: list[SampleDatagram] = []
        self.max_size = max_size

    def add(self, sample: SampleDatagram):
        # TODO: Make this more robust
        if self.size() + sample.size() + len(", ") > self.max_size:
            raise BufferError("Payload is full")
        self.samples.append(sample)

    def to_bytes(self) -> bytes:
        return json.dumps(
            [sample.json_dict() for sample in self.samples],
            cls=BytesEncoder
        ).encode()

    def size(self):
        return len(self.to_bytes())
