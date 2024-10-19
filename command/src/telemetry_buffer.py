from shared.src.encode_telemetry import SegmentationParams, SampleDatagram, SegmentedDataDict

class TelemetryBuffer:
    def __init__(self):
        self.sample_id = None
        self.sample_time = -1
        self.seq_nums = set()
        self.data_buffer: dict[int, bytes] = {}
        self.highest_seq_num = -1
        self.total_num_segments = -1
    
    def add_segment(self, segment: SampleDatagram):
        if self.sample_time != segment.data['sample_time']:
            self.sample_id = segment.get_id()
            self.segment_buffer = {}
            self.seq_nums = set()
            self.highest_seq_num = -1
    
        seq_num = segment.data['sample_data_segment']['seq_num']
        self.seq_nums.add(segment.data['sample_data_segment']['seq_num'])
        self.sample_time = segment.data['sample_time']
        self.highest_seq_num = max(self.highest_seq_num, seq_num)
        self.total_num_segments = segment.data['sample_data_segment']['num_segments']
        self.data_buffer[seq_num] = segment.data['sample_data_segment']['segment_data']
    
    def is_initial_transmit_complete(self):
        return self.highest_seq_num == self.total_num_segments - 1
    
    def is_complete(self):
        return len(self.seq_nums) == self.total_num_segments
    
    def missing_seq_nums(self):
        return set(range(self.total_num_segments)) - self.seq_nums
    