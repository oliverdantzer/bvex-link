from dataclasses import dataclass

@dataclass
class SampleMetadata:
    metric_id: str
    sample_id: int
    timestamp: float
    data_type: str


@dataclass
class SegmentMetadata:
    sequence_number: int
    num_segments: int


@dataclass
class Segment:
    metadata: SegmentMetadata
    data: bytes


@dataclass
class Sample:
    metadata: SampleMetadata
    segment: Segment


@dataclass
class Ack:
    metric_id: str
    sample_id: int
    seqnums: list[int]