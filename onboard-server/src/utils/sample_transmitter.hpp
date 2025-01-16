#pragma once

#include "../command.hpp"
#include "../sample.hpp"
#include "chunker.hpp"
#include <cstdint>
#include <functional>
#include <memory>
#include <set>
#include <vector>

typedef uint32_t SampleId;

// Class to handle the segmented transmission of samples
class SampleTransmitter
{
  public:
    SampleTransmitter(
        std::function<std::unique_ptr<SampleData>()> pop_latest_sample,
        std::function<size_t()> get_max_pkt_size, MetricId metric_id);

    // Get the next payload to downlink
    std::unique_ptr<std::vector<uint8_t>> get_pkt();

    // Mark a sequence number as succesfully recieved
    void ack_seqnum(int seqnum, SampleId sample_id);

    // // Mark the sample as succesfully recieved
    // void signal_sample_recieved(SampleId sample_id);

  private:
    bool set_new_sample();
    void increment_itr();
    unsigned int get_itr_val();
    SampleMetadata sample_metadata_;
    SampleId sample_id_;
    Chunker* sample_chunker_;
    std::string data_type_;
    std::optional<std::string> file_extension_;
    std::function<std::unique_ptr<SampleData>()> pop_latest_sample_;
    std::function<size_t()> get_max_pkt_size_;
    std::set<unsigned int> unacked_seqnums_;
    std::set<unsigned int>::iterator unacked_seqnums_itr_;
};