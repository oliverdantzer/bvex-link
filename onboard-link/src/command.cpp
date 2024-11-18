#include "command.hpp"
#include "boost/shared_ptr.hpp"

Command::Command(size_t bps, size_t max_packet_size_)
    : bps_(bps), max_packet_size_(max_packet_size_), metric_ids_(),
      latest_samples_(), metric_shares_() {};

void Command::add_sample(boost::shared_ptr<SampleData> sample)
{
    latest_samples_[sample->metadata.metric_id] = sample;
    metric_ids_.insert(sample->metadata.metric_id);
    // TODO: Get the share from the ground command instead of automatically
    // setting it to uniform distribution
    metric_shares_[sample->metadata.metric_id] = 1.0f / metric_ids_.size();
}

boost::shared_ptr<SampleData> Command::get_latest_sample(MetricId metric_id)
{
    return latest_samples_[metric_id];
}

float Command::get_metric_share(MetricId metric_id)
{
    return metric_shares_[metric_id];
}

size_t Command::get_bps() { return bps_; }

const std::set<MetricId>& Command::get_metric_ids() const
{
    return metric_ids_;
}

// std::vector<MetricId> Command::get_metric_ids()
// {
//     std::vector<MetricId> metric_ids;
//     for(auto const& metric_id : metric_ids_) {
//         metric_ids.push_back(metric_id);
//     }
//     return metric_ids;
// }