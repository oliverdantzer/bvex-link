#pragma once

#include "sample.hpp"
#include "telemetry.hpp"
#include <boost/shared_ptr.hpp>
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

  private:
    size_t bps_;
    size_t max_packet_size_;
    // std::vector<std::string> telecommands_;
    std::set<MetricId> metric_ids_;
    std::map<MetricId, boost::shared_ptr<SampleData>> latest_samples_;
};
