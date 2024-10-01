# error correction
# https://software.roffe.eu/ldpc/index.html

# IP add

from collections import deque
import math
from shared.src.sample import Sample
from shared.src.encode_telemetry import SampleDatagram, TelemetryPayload





# class SampleDataBuffer:
#     def __init__(self, sample: Sample):
#         self.sample: Sample = sample
#         self.empty = True

#     def read(self):
#         if not self.empty:
#             return self.sample.get_datagram()
#         else:
#             raise BufferError("Cannot read empty data buffer")

#     def pop(self):
#         datagram = self.read()
#         self.empty = True

#     def retransmit(self):
#         self.empty = False

#     def is_empty(self):
#         return self.empty


class SampleDataSegmentedBuffer:
    def __init__(self, sample: Sample, segment_size: int):
        self.sample = sample
        self.seq_num = 0
        self.retransmit_seq_nums = set()
        self.segment_size = segment_size  # in bytes
        self.num_segments = math.ceil(len(sample.sample_data) / segment_size)
        print("Num segments: ", self.num_segments)
        print("Segment size: ", self.segment_size)
        print("Sample data: ", sample.sample_data)
        print("Sample data size: ", len(sample.sample_data))

    def get_datagram(self, seq_num: int) -> SampleDatagram:
        return self.sample.get_datagram(
            seq_num,
            self.segment_size,
            self.num_segments
        )

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

    # TODO: fix peek will not return same as pop
    
    def peek(self) -> SampleDatagram:
        if self.seq_num < self.num_segments:
            return self.get_datagram(self.seq_num)
        elif self.retransmit_seq_nums:
            retransmit_seq_num = self.retransmit_seq_nums.pop()
            return self.get_datagram(retransmit_seq_num)
        else:
            raise BufferError("Cannot pop empty data buffer")

class PopEmptySendBufferError(BufferError):
    pass

class SendBuffer:
    def __init__(self, payload_size: int = 1024):
        # For demo, we will use a single sample metric
        self.metric_id = 'img'
        self.sample_buffers: list[SampleDataSegmentedBuffer] = []
        self.max_payload_size = payload_size  # in bytes
        self.max_segment_data_size = 10  # in bytes

    def set_payload_size(self, payload_size: int):
        self.max_payload_size = payload_size
        self.sample_buffers = []

    def set_max_segment_data_size(self, segment_size: int):
        self.max_segment_data_size = segment_size
        self.sample_buffers = []

    def add(self, sample: Sample):
        buffer = SampleDataSegmentedBuffer(sample, self.max_segment_data_size)
        self.sample_buffers.append(buffer)

    def get_payload(self) -> bytes | None:
        if len(self.sample_buffers):
            payload = TelemetryPayload(self.max_payload_size)
            for sample_buffer in self.sample_buffers:
                while not sample_buffer.is_empty():
                    # --------- debug -------------
                    # print("Payload", payload.to_bytes().decode())
                    # print("Payload size: ", payload.size())
                    # print("Sample buffer size: ", sample_buffer.peek().size())
                    # print("Peek segment json: ", sample_buffer.peek())
                    # peek_segment_data = sample_buffer.peek().data['sample_data_segment']["segment_data"]
                    # print("Peek segment data: ", peek_segment_data.decode("utf-8"))
                    # print("Max payload size: ", self.max_payload_size)
                    # print("Max segment size: ", self.max_segment_data_size)
                    # --------- debug -------------
                    if payload.size() + sample_buffer.peek().size() > self.max_payload_size:
                        if payload.samples:
                            return payload.to_bytes()
                        else:
                            raise PopEmptySendBufferError
                    else:
                        payload.add(sample_buffer.pop())
            return payload.to_bytes()
        else:
            raise PopEmptySendBufferError
