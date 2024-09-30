import encode_telemetry


class Sample:
    def __init__(self, metric_id: str, sample_time: int, sample_data: bytes):
        self.metric_id = metric_id
        self.sample_data = sample_data
        self.sample_time = sample_time

    def get_id(self) -> str:
        return self.metric_id + str(self.sample_time)

    def get_sample_header(self, is_segment: bool = False) -> bytes:
        return encode_telemetry.construct_sample_header(
            self.metric_id,
            self.sample_time,
            is_segment
        )

    def get_data_segment(self, seq_num, segment_size) -> bytes:
        return self.sample_data[
            seq_num * segment_size:
            min(
                len(self.sample_data),
                (seq_num + 1) * segment_size
            )
        ]

    def get_datagram(self, segmentation_params: encode_telemetry.SegmentationParameters | None = None) -> encode_telemetry.SampleDatagram:
        sample_data = None

        if segmentation_params:
            sample_data = encode_telemetry.SegmentDatagram(
                segmentation_params,
                self.get_data_segment(
                    segmentation_params['seq_num'],
                    segmentation_params['segment_size']
                )
            )
        else:
            sample_data = self.sample_data

        return encode_telemetry.SampleDatagram(
            self.metric_id,
            self.sample_time,
            self.sample_data
        )
