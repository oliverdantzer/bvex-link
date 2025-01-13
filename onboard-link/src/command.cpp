#include "command.hpp"
#include "boost/shared_ptr.hpp"
#include "sample.hpp"
#include <iostream>
#include <memory>
#include <optional>
#include <memory>

Command::Command(size_t bps, size_t max_packet_size_)
    : bps_(bps), metrics_() {};

void Command::add_sample(boost::shared_ptr<SampleData> sample)
{
    // std::cout << "add_sample. obj memaddr: " << this << std::endl;
    if(metric_exists(sample->metadata.metric_id)) {
        metrics_[sample->metadata.metric_id].sample_id++;
        metrics_[sample->metadata.metric_id].latest_sample = sample;
    } else {

        // TODO: Get the share values from the ground command instead of this
        // Set shares to uniform distribution
        float share = 1.0f / (get_num_metrics() + 1);
        for (auto& [metric_id, metric_info] : metrics_) {
            metric_info.share = share;
        }

        // Create metric_info, populate it with data from sample
        struct MetricInfo metric_info;
        metric_info.metric_id = sample->metadata.metric_id;
        metric_info.share = share;
        metric_info.latest_sample = sample;
        // TODO: find more intelligent way of getting max size
        metric_info.max_sample_size = sample->get_size();
        metric_info.sample_id = 0;

        // add metric_id:metric_info to map
        metrics_[sample->metadata.metric_id] = metric_info;
    }
}

std::optional<SampleDownlinkData> Command::pop_latest_sample(MetricId metric_id)
{
    // If metric exists and has sample
    if(metric_exists(metric_id) &&
       metrics_[metric_id].latest_sample != nullptr) {
        // create sample_downlink_data, populate it with
        // data from latest_sample of metric_id
        struct SampleDownlinkData sample_downlink_data;
        sample_downlink_data.sample = metrics_[metric_id].latest_sample;
        sample_downlink_data.sample_id = metrics_[metric_id].sample_id;

        // set latest_sample of metric_id to nullptr,
        // signifying it is gone
        metrics_[metric_id].latest_sample = nullptr;

        return sample_downlink_data;

    } else {
        // return nullopt to signify invalid metric or
        // no new sample
        return std::nullopt;
    }
}

size_t Command::get_bps() { return bps_; }

std::unique_ptr<std::vector<MetricId>> Command::get_all_metric_ids() {
    auto metric_ids = std::make_unique<std::vector<MetricId>>();

    // optimization to reserve enough space so we don't need
    // to continually increase the size of the vector
    metric_ids->reserve(metrics_.size());
    
    for (auto& [metric_id, _]: metrics_) {
        metric_ids->push_back(metric_id);
    }
    return metric_ids;
}

std::optional<MetricInfo> Command::get_metric_info(MetricId metric_id)
{
    if(metric_exists(metric_id)) {
        return metrics_[metric_id];
    } else {
        return std::nullopt;
    }
}

size_t Command::get_num_metrics() {
    return metrics_.size();
}

bool Command::metric_exists(MetricId metric_id)
{
    return metrics_.find(metric_id) != metrics_.end();
}

float Command::get_metric_share(MetricId metric_id)
{
    return metrics_[metric_id].share;
}

size_t Command::get_metric_max_sample_size(MetricId metric_id)
{
    return metrics_[metric_id].max_sample_size;
}

void Command::print_all_metric_ids()
{
    for(const auto& metric : metrics_) {
        std::cout << metric.first << std::endl;
    }
}
