#pragma once

#include "sample.hpp"
#include "utils/sample_transmitter.hpp"
#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Struct to hold information about a metric.
 */
struct MetricInfo {
    // Unique of the metric
    MetricId metric_id;
    // Amount of bps dedicated to metric telemetry data
    // Value in range [0-1]
    unsigned int token_threshold;
    // Latest sample of this metric recieved.
    // can be null if no sample has been recieved or if nullified after
    // pop_latest_sample
    boost::shared_ptr<SampleData> latest_sample;
    std::unique_ptr<SampleTransmitter> sample_transmitter;
};

class Command
{
  public:
    Command(size_t init_bps, size_t init_max_packet_size);
    // void add_tc_json(const std::string& telecommands_json);

    size_t get_bps();

    /**
     * @brief Adds a sample to the internal data structure.
     * @param sample Shared pointer to the sample data to be added.
     */
    void add_sample(boost::shared_ptr<SampleData> sample);

    boost::shared_ptr<std::vector<uint8_t>> get_sample_pkt(MetricId metric_id);

    size_t get_num_metrics();

    float get_metric_token_threshold(MetricId metric_id);

    void print_all_metric_ids();

    std::unique_ptr<std::vector<MetricId>> get_all_metric_ids();

    bool metric_exists(MetricId metric_id);

    size_t get_max_packet_size();

  private:
    /**
     * @brief Pop the latest sample data recieved for the given metric ID.
     *
     * Removes sample from internal data structure before returning it.
     *
     * @param metric_id ID of the metric.
     * @return Optional containing SampleDownlinkData if available.
     */
    boost::shared_ptr<SampleData> pop_latest_sample(MetricId metric_id);

    size_t bps_;
    size_t max_packet_size_;
    // std::vector<std::string> telecommands_;
    std::unordered_map<MetricId, std::unique_ptr<MetricInfo>> metrics_;
};