from typing import TypedDict
import struct

# sample header (16 bytes):
# 0-6: 7 byte metric_id (utf-8)
# 7-14: 4 byte sample_time (32-bit int)
# 15: 1 byte options:
#   0th bit: is_segment (bool)


def construct_sample_header(metric_id: str, sample_time: int, is_segment: bool = False):
    header = bytes()

    # Convert to 7-byte utf-8 string
    metric_id_bytes = metric_id.encode('utf-8')
    assert len(metric_id.encode('utf-8')) == 7
    header += metric_id_bytes

    # Convert to 32-bit unsigned integer in big-endian format
    sample_time_bytes = struct.pack('>I', sample_time)
    header += sample_time_bytes

    # Convert to 1-byte unsigned integer in big-endian format
    options = int(is_segment).to_bytes(1, 'big')
    header += options

    return header


class SegmentationParameters(TypedDict):
    seq_num: int
    segment_size: int
    num_segments: int


class SegmentDatagram:
    def __init__(self, segmentation_params: SegmentationParameters, segment_data: bytes):
        self.segment_data = segment_data
        self.segmentation_params = segmentation_params

    # segment header (12 bytes):
    def get_segment_header(self) -> bytes:
        header = bytes()
        header += self.segmentation_params['seq_num'].to_bytes(4, 'big')
        header += self.segmentation_params['segment_size'].to_bytes(4, 'big')
        header += self.segmentation_params['num_segments'].to_bytes(4, 'big')
        return header

    def to_bytes(self):
        return self.get_segment_header() + self.segment_data


class SampleDatagram:
    def __init__(self, metric_id: str, sample_time: int, sample_data: bytes | SegmentDatagram):
        self.metric_id = metric_id
        self.sample_time = sample_time
        self.sample_data = sample_data
        self.is_segment = isinstance(self.sample_data, SegmentDatagram)

    def get_id(self) -> str:
        return self.metric_id + str(self.sample_time)

    def get_sample_header(self) -> bytes:
        return construct_sample_header(
            self.metric_id,
            self.sample_time,
            self.is_segment
        )

    def to_bytes(self) -> bytes:
        header = self.get_sample_header()
        if self.is_segment:
            assert isinstance(self.sample_data, SegmentDatagram)
            header += self.sample_data.to_bytes()
        else:
            assert isinstance(self.sample_data, bytes)
            header += self.sample_data
        return header
    
    def size(self):
        return len(self.to_bytes())
