#pragma once

#include "sample.hpp"
#include <boost/shared_ptr.hpp>
#include <map>
#include <set>
#include <string>
#include <vector>

class Command
{
  public:
    Command(size_t bps, size_t max_packet_size_);
    // void add_tc_json(const std::string& telecommands_json);
    size_t get_bps();
    size_t get_max_packet_size();
    void add_sample(boost::shared_ptr<SampleData> sample);
    boost::shared_ptr<SampleData> get_latest_sample(MetricId metric_id);
    float get_metric_share(MetricId metric_id);

    const std::set<MetricId>& get_metric_ids() const; // Getter for metric_ids

  private:
    std::set<MetricId> metric_ids_;
    size_t bps_;
    size_t max_packet_size_;
    // std::vector<std::string> telecommands_;
    std::map<MetricId, boost::shared_ptr<SampleData>> latest_samples_;
    std::map<MetricId, float> metric_shares_;
};
