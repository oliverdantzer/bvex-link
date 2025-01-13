#include "sample_transmitter.hpp"
#include "chunker.hpp"
#include "encode_downlink_telemetry/serialize_segment.hpp"
#include <boost/iterator/counting_iterator.hpp>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>


SampleTransmitter::SampleTransmitter(
    std::function<boost::shared_ptr<SampleData>()> pop_latest_sample,
    std::function<size_t()> get_max_pkt_size, MetricId metric_id)
    : pop_latest_sample_(pop_latest_sample), sample_metadata_({
                                                 .metric_id = metric_id,
                                             }),
      get_max_pkt_size_(get_max_pkt_size_), sample_chunker_(nullptr),
      unacked_seqnums_() {};

bool SampleTransmitter::set_new_sample()
{
    boost::shared_ptr<SampleData> sample = pop_latest_sample_();
    if(sample == nullptr) {
        return false;
    } else {
        delete sample_chunker_;

        uint32_t overhead =
            SAMPLE_FRAME_OVERHEAD + IPV4_HEADER_OVERHEAD + UDP_HEADER_OVERHEAD;

        uint32_t max_segment_size = get_max_pkt_size_() - overhead;

        sample_chunker_ =
            new Chunker(sample->data_serialized(), max_segment_size);
        unsigned int num_chunks = sample_chunker_->get_num_chunks();

        // set all seqnums to unacked
        unacked_seqnums_.clear();
        std::copy(boost::counting_iterator<int>(0),
                  boost::counting_iterator<int>(num_chunks),
                  std::back_inserter(unacked_seqnums_));

        // set iterator to first unacked seqnum
        unacked_seqnums_itr_ = unacked_seqnums_.begin();

        // increment to next sample id
        sample_id_++;

        return true;
    }
}

boost::shared_ptr<std::vector<uint8_t>> SampleTransmitter::get_pkt()
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
    // TODO: SERIALIZE CHUNK AND RETURN
    SegmentData segment_data = {
        .metadata = sample_metadata_,
        .num_segments = sample_chunker_->get_num_chunks(),
        .seqnum = seq_num,
        .data = chunk.data};
    boost::shared_ptr<std::vector<uint8_t>> pkt =
        serialize_segment(segment_data);
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

void SampleTransmitter::ack_seqnum(int seqnum, SampleId sample_id)
{
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