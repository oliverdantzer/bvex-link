# error correction
# https://software.roffe.eu/ldpc/index.html

# IP add

from collections import deque
import math
from shared.src.sample import Sample





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

    def pop(self) -> bytes:
        if self.seq_num < self.num_segments:
            self.seq_num += 1
            return self.get_datagram(self.seq_num)
        elif self.retransmit_seq_nums:
            seq_num = self.retransmit_seq_nums.pop()
            return self.get_datagram(seq_num)
        else:
            raise BufferError("Cannot pop empty data buffer")

    def pop_size(self):
        if self.seq_num < self.num_segments:
            return self.get_datagram(self.seq_num + 1)
        elif self.retransmit_seq_nums:
            seq_num = self.retransmit_seq_nums.pop()
            return self.get_datagram(seq_num)
        else:
            raise BufferError("Cannot pop empty data buffer")

type Sample = Class


class SendBuffer:
    def __init__(self, payload_size: int = 1024, sample_class: Sample = Sample):
        self.metrics_samples: dict[str, int] = {}
        self.samples_priority = deque()
        self.samples: dict[str, SampleDataBuffer] = {}
        
        self.payload_size = payload_size  # in bytes
        self.segment_size = payload_size - 28  # in bytes

    def set_payload_size(self, payload_size: int):
        self.payload_size = payload_size
        self.segment_size = min(self.segment_size, payload_size - 28)

    def set_segment_size(self, segment_size: int):
        self.segment_size = segment_size
        self.payload_size = max(segment_size + 28, self.payload_size)

    def add(self, sample: Sample):
        send_data = None
        if sample.get_singleton_datagram_size() < self.payload_size:
            send_data = SampleDataBuffer(sample)
        else:
            send_data = SampleDataSegmentedBuffer(sample, self.payload_size)
        self.samples_priority.append(sample.get_id())
        self.samples[sample.get_id()] = send_data

    def first_sample_id(self):
        return self.samples_priority[0]

    def get_payload(self):
        payload = bytes()
        while len(self.samples_priority):
            sample_id = self.first_sample_id()
            self.samples[sample_id].
            if 
