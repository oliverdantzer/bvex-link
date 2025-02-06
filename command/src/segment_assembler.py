from typing import Callable

from sample_data import Segment


class SegmentAssembler:
    num_segments: int
    segments: dict[int, bytes]
    handle_completion: Callable[[bytes], None]

    def __init__(self, num_segments: int, handle_completion: Callable[[bytes], None]):
        if num_segments <= 0:
            raise ValueError("Number of segments must be greater than 0")
        self.num_segments = num_segments
        self.segments: dict[int, bytes] = {}
        self.handle_completion = handle_completion

    def add_segment(self, segment: Segment):
        if segment.metadata.num_segments != self.num_segments:
            raise ValueError("Number of segments does not match")
        if segment.metadata.sequence_number >= self.num_segments:
            raise ValueError("Sequence number out of range")

        if segment.metadata.sequence_number not in self.segments:
            self.segments[segment.metadata.sequence_number] = segment.data
            if self.is_complete():
                self.handle_completion(self.assemble())

    def get_received_segments(self) -> list[int]:
        return list(self.segments.keys())

    def is_complete(self) -> bool:
        return len(self.segments) == self.num_segments

    def assemble(self) -> bytes:
        if not self.is_complete():
            raise ValueError("Not all segments have been received")
        return b"".join(self.segments.values())
