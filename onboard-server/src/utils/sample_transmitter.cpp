#include "sample_transmitter.hpp"
#include "chunker.hpp"
#include "encode_downlink_telemetry/encode_sample_frame.hpp"
#include "encode_downlink_telemetry/size_constants.hpp"
#include <boost/iterator/counting_iterator.hpp>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

SampleTransmitter::SampleTransmitter(
    std::function<std::unique_ptr<SampleData>()> pop_latest_sample,
    std::function<size_t()> get_max_pkt_size, MetricId metric_id)
    : pop_latest_sample_(pop_latest_sample),
      get_max_pkt_size_(get_max_pkt_size), sample_metadata_({
                                               .metric_id = metric_id,
                                               .timestamp = 0.0f,
                                           }),
      sample_chunker_(nullptr), unacked_seqnums_() {};

bool SampleTransmitter::set_new_sample()
{
    std::unique_ptr<SampleData> sample = pop_latest_sample_();
    if(sample == nullptr) {
        return false;
    } else {
        size_t overhead =
            SAMPLE_FRAME_OVERHEAD + IPV4_HEADER_OVERHEAD + UDP_HEADER_OVERHEAD;

        size_t max_segment_size = get_max_pkt_size_() - overhead;

        data_type_ = sample->type;

        sample_chunker_ = std::make_unique<Chunker>(Chunker(sample->encode_data(), max_segment_size));
        unsigned int num_chunks = sample_chunker_->get_num_chunks();

        // set all seqnums to unacked
        unacked_seqnums_.clear();
        std::copy(boost::counting_iterator<int>(0),
                  boost::counting_iterator<int>(num_chunks),
                  std::inserter(unacked_seqnums_, unacked_seqnums_.end()));

        // set iterator to first unacked seqnum
        unacked_seqnums_itr_ = unacked_seqnums_.begin();

        // increment to next sample id
        sample_id_++;

        return true;
    }
}

std::unique_ptr<std::vector<uint8_t>> SampleTransmitter::get_pkt()
{
    if(sample_chunker_ == nullptr || unacked_seqnums_.size() == 0) {
        // Get a new sample to downlink
        bool got_new_sample = set_new_sample();
        if(!got_new_sample) {
            return nullptr;
        }
    }
    unsigned int seq_num = get_itr_val();
    Chunk chunk = sample_chunker_->get_chunk(seq_num);
    increment_itr();
    SampleFrameData segment_data = {.metadata = sample_metadata_,
                                    .data_type = data_type_,
                                    .sample_id = sample_id_,
                                    .num_segments =
                                        sample_chunker_->get_num_chunks(),
                                    .seqnum = seq_num,
                                    .data = std::move(chunk.data)};
    std::unique_ptr<std::vector<uint8_t>> pkt =
        encode_sample_frame(std::move(segment_data));
#ifdef DEBUG
    if(pkt->size() > get_max_pkt_size_()) {
        std::cerr << "Packet size exceeds maximum packet size" << std::endl;
        std::cerr << "Actual packet size: " << pkt->size() << std::endl;
        std::cerr << "Maximum packet size: " << get_max_pkt_size_()
                  << std::endl;
    }
#endif
    return pkt;
}

void SampleTransmitter::ack_seqnum(SeqNum seqnum, SampleId sample_id)
{
    if(sample_id != sample_id_) {
        return;
    }

    if(get_itr_val() == seqnum) {
        increment_itr();
    }
    unacked_seqnums_.erase(seqnum);
}

void SampleTransmitter::increment_itr()
{
    if(unacked_seqnums_itr_ == unacked_seqnums_.end()) {
        unacked_seqnums_itr_ = unacked_seqnums_.begin();
    }
    unacked_seqnums_itr_++;
}

unsigned int SampleTransmitter::get_itr_val() { return *unacked_seqnums_itr_; }
