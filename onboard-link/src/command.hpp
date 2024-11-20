#pragma once

#include "sample.hpp"
#include <boost/shared_ptr.hpp>
#include <unordered_map>
#include <optional>
#include <set>
#include <string>
#include <vector>

/**
 * @brief Struct to hold data concerning a sample that is to be downlinked
 */
struct SampleDownlinkData {
    // Pointer to the sample's data.
    boost::shared_ptr<SampleData> sample;
    // Unique id of the sample (Unique in context of sample's metric)
    int sample_id;
};

/**
 * @brief Struct to hold information about a metric.
 */
struct MetricInfo {
    // Unique of the metric
    MetricId metric_id;
    // Amount of bps dedicated to metric telemetry data
    // Value in range [0-1]
    float share;
    // Latest sample of this metric recieved
    boost::shared_ptr<SampleData> latest_sample;
    // Sample id of latest sample recieved
    int sample_id;
    // Max size in bytes of a sample from this metric
    size_t max_sample_size;
};

class Command
{
  public:
    Command(size_t bps, size_t max_packet_size_);
    // void add_tc_json(const std::string& telecommands_json);

    size_t get_bps();

    /**
     * @brief Adds a sample to the internal data structure.
     * @param sample Shared pointer to the sample data to be added.
     */
    void add_sample(boost::shared_ptr<SampleData> sample);

    /**
     * @brief Pop the latest sample data recieved for the given metric ID.
     * 
     * Removes sample from internal data structure before returning it.
     * 
     * @param metric_id ID of the metric.
     * @return Optional containing SampleDownlinkData if available.
     */
    std::optional<SampleDownlinkData> pop_latest_sample(MetricId metric_id);

    std::optional<MetricInfo> get_metric_info(MetricId metric_id);

    size_t get_num_metrics();

    float get_metric_share(MetricId metric_id);

    void print_all_metric_ids();

    std::unique_ptr<std::vector<MetricId>> get_all_metric_ids();

    bool metric_exists(MetricId metric_id);
    
    size_t get_metric_max_sample_size(MetricId metric_id);

  private:
    size_t bps_;
    // std::vector<std::string> telecommands_;
    std::unordered_map<MetricId, MetricInfo> metrics_;
};
