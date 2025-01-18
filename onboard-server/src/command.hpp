#pragma once

#include "sample.hpp"
#include "utils/sample_transmitter.hpp"
#include <cstdint>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

class SampleTransmitter;

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
    std::unique_ptr<SampleData> latest_sample;
    std::unique_ptr<SampleTransmitter> sample_transmitter;
};

class MetricIterator
{
  public:
    MetricIterator(std::function<std::map<MetricId, MetricInfo>::iterator()>
                       get_begin_iterator,
                   std::function<std::map<MetricId, MetricInfo>::iterator()>
                       get_end_iterator)
        : get_begin_iterator(get_begin_iterator),
          get_end_iterator(get_end_iterator),
          current_iterator_(get_begin_iterator())
    {
    }

    void increment();

    bool empty();

    const std::string get_id();

    unsigned int get_token_threshold();

    std::unique_ptr<std::vector<uint8_t>> get_sample_pkt();

  private:
    std::function<std::map<MetricId, MetricInfo>::iterator()>
        get_begin_iterator;
    std::function<std::map<MetricId, MetricInfo>::iterator()> get_end_iterator;
    std::map<MetricId, MetricInfo>::iterator current_iterator_;
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
    void add_sample(std::unique_ptr<SampleData> sample);

    size_t get_num_metrics();

    void print_all_metric_ids();

    bool metric_exists(MetricId metric_id);

    size_t get_max_packet_size();

    MetricIterator get_metric_iterator();

    std::map<MetricId, MetricInfo>::iterator get_metrics_begin_iter();

    std::map<MetricId, MetricInfo>::iterator get_metrics_end_iter();

  private:
    /**
     * @brief Pop the latest sample data recieved for the given metric ID.
     *
     * Removes sample from internal data structure before returning it.
     *
     * @param metric_id ID of the metric.
     * @return Optional containing SampleDownlinkData if available.
     */
    std::unique_ptr<SampleData> pop_latest_sample(MetricId metric_id);

    size_t bps_;
    size_t max_packet_size_;
    // std::vector<std::string> telecommands_;

    /**
     * @brief Stores metric_id:metric_info* pairs.
     *
     * std::map is used here because it can be iterated through in order,
     * and added to without iterator invalidation.
     *
     * @see https://en.cppreference.com/w/cpp/container#Iterator_invalidation
     */
    std::map<MetricId, MetricInfo> metrics_;
};
