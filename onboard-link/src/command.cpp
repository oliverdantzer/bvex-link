#include "command.hpp"
#include "boost/shared_ptr.hpp"

Command::Command(size_t bps, size_t max_packet_size_)
    : bps_(bps), max_packet_size_(max_packet_size_), metric_ids_(),
      latest_samples_() {};

void Command::add_sample(boost::shared_ptr<SampleData> sample)
{
    metric_ids_.insert(sample->metadata.metric_id);
    latest_samples_[sample->metadata.metric_id] = sample;
}

boost::shared_ptr<SampleData> Command::get_latest_sample(MetricId metric_id)
{
    return latest_samples_[metric_id];
}