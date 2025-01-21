import json
from typing import Callable

from sample_data import Ack, Sample, SampleMetadata
from segment_assembler import SegmentAssembler


class MetricReceiver:
    metric_id: str
    handle_completion: Callable[[bytes], None]
    current_sample_assembler: SegmentAssembler
    current_sample_metadata: SampleMetadata

    def __init__(
        self,
        first_sample: Sample,
        handle_completion: Callable[[bytes, SampleMetadata], None],
    ):
        self.current_sample_metadata = first_sample.metadata
        self.handle_completion = lambda data: handle_completion(
            data, self.current_sample_metadata
        )

        # handle first sample
        self.current_sample_assembler = SegmentAssembler(
            first_sample.segment.metadata.num_segments,
            handle_completion=self.handle_completion,
        )
        self.current_sample_assembler.add_segment(first_sample.segment)

    def add_sample(self, sample: Sample):
        if sample.metadata.metric_id != self.current_sample_metadata.metric_id:
            raise ValueError("""A single metric receiver object 
                             can only receive packets for one metric""")

        # if new sample_id recieved,
        # this means onboard server has started downlinking
        # a new sample for this metric, and we need to create
        # a new SegmentAssember
        if sample.metadata.sample_id != self.current_sample_metadata.sample_id:
            self.current_sample_metadata = sample.metadata
            self.current_sample_assembler = SegmentAssembler(
                sample.segment.metadata.num_segments,
                handle_completion=self.handle_completion,
            )
        # if sample id was the same, but any other metadata are different,
        # we have encountered an error
        # (may have to remove due to not guaranteed packet arrival order)
        elif self.current_sample_metadata != sample.metadata:
            raise ValueError(
                "Segments with the same sample_id cannot have any differing metadata"
            )
        
        self.current_sample_assembler.add_segment(sample.segment)

    def get_ack(self) -> Ack:
        assert self.current_sample_assembler is not None
        metadata = self.current_sample_metadata
        assert metadata is not None
        return Ack(
            metadata.metric_id,
            metadata.sample_id,
            self.current_sample_assembler.get_received_segments(),
        )


class SampleReceiver:
    receivers: dict[str, MetricReceiver]
    i: int | None

    def __init__(self):
        self.receivers = {}
        self.i = None

    def handle_sample(self, sample: Sample):
        def store_sample(data, metadata):
            return print(json.loads(data))

        metric_id = sample.metadata.metric_id
        if metric_id not in self.receivers:
            self.receivers[metric_id] = MetricReceiver(sample, store_sample)
        self.receivers[metric_id].add_sample(sample)

    def get_ack(self) -> Ack:
        if len(self.receivers) == 0:
            raise ValueError("No received samples to ack")
        if self.i is None:
            self.i = 0
        assert self.i is not None
        _, MetricReceiver = list(self.receivers.items())[self.i]
        self.i += 1
        if self.i >= len(self.receivers):
            self.i = 0
        return MetricReceiver.get_ack()
