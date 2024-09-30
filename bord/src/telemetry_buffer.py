# error correction
# https://software.roffe.eu/ldpc/index.html

# IP add

from collections import deque
import math
from shared.src.sample import Sample
from shared.src.encode_telemetry import SampleDatagram





class SampleDataBuffer:
    def __init__(self, sample: Sample):
        self.sample: Sample = sample
        self.empty = True

    def read(self):
        if not self.empty:
            return self.sample.get_datagram()
        else:
            raise BufferError("Cannot read empty data buffer")

    def pop(self):
        datagram = self.read()
        self.empty = True

    def retransmit(self):
        self.empty = False

    def is_empty(self):
        return self.empty


class SampleDataSegmentedBuffer(SampleDataBuffer):
    def __init__(self, sample: Sample, segment_size: int):
        super().__init__(sample)
        self.seq_num = 0
        self.retransmit_seq_nums = set()
        self.segment_size = segment_size  # in bytes
        self.num_segments = math.ceil(len(sample.sample_data) / segment_size)

    def get_datagram(self, seq_num: int):
        return self.sample.get_datagram({
            'seq_num': seq_num,
            'segment_size': self.segment_size,
            'num_segments': self.num_segments
        })

    def retransmit(self, seq_num: int):
        self.retransmit_seq_nums.add(seq_num)

    def is_empty(self):
        return self.seq_num >= self.num_segments and not self.retransmit_seq_nums

    def pop(self) -> SampleDatagram:
        datagram = None
        if self.seq_num < self.num_segments:
            datagram = self.get_datagram(self.seq_num)
            self.seq_num += 1
        elif self.retransmit_seq_nums:
            retransmit_seq_num = self.retransmit_seq_nums.pop()
            datagram = self.get_datagram(retransmit_seq_num)
        else:
            raise BufferError("Cannot pop empty data buffer")
        return datagram

    def peek(self) -> SampleDatagram:
        if self.seq_num < self.num_segments:
            return self.get_datagram(self.seq_num)
        elif self.retransmit_seq_nums:
            retransmit_seq_num = self.retransmit_seq_nums.pop()
            return self.get_datagram(retransmit_seq_num)
        else:
            raise BufferError("Cannot pop empty data buffer")

class SendBuffer:
    def __init__(self, payload_size: int = 1024):
        # For demo, we will use a single sample metric
        self.metric_id = 'img'
        self.sample_buffers: list[SampleDataSegmentedBuffer] = []
        self.payload_size = payload_size  # in bytes
        self.segment_size = payload_size - 28  # in bytes

    def set_payload_size(self, payload_size: int):
        self.payload_size = payload_size
        self.segment_size = min(self.segment_size, payload_size - 28)
        self.sample_buffers = []

    def set_segment_size(self, segment_size: int):
        self.segment_size = segment_size
        self.payload_size = max(segment_size + 28, self.payload_size)
        self.sample_buffers = []

    def add(self, sample: Sample):
        buffer = SampleDataSegmentedBuffer(sample, self.payload_size)
        self.sample_buffers.append(buffer)

    def get_payload(self):
        if len(self.sample_buffers):
            payload = bytes()
            for sample_buffer in self.sample_buffers:
                while not sample_buffer.is_empty():
                    if len(payload) + sample_buffer.peek().size() > self.payload_size:
                        return payload
                    else:
                        payload += sample_buffer.pop().to_bytes()
            return payload
