#include "command.hpp"
#include "sample.hpp"
#include <iostream>
#include <memory>
#include <optional>

Command::Command(size_t init_bps, size_t init_max_packet_size)
    : bps_(init_bps), metrics_(), max_packet_size_(init_max_packet_size) {};

void Command::add_sample(std::unique_ptr<SampleData> sample)
{
    // std::cout << "add_sample. obj memaddr: " << this << std::endl;
    if(metric_exists(sample->metadata.metric_id)) {
        metrics_[sample->metadata.metric_id]->latest_sample = std::move(sample);
    } else {
        // Create metric_info, populate it with data from sample
        std::unique_ptr<MetricInfo> metric_info =
            std::make_unique<MetricInfo>();
        metric_info->metric_id = sample->metadata.metric_id;
        metric_info->token_threshold = 1;
        metric_info->latest_sample = std::move(sample);
        metric_info->sample_transmitter = std::make_unique<SampleTransmitter>(
            [this, metric_id = sample->metadata.metric_id]() {
                return pop_latest_sample(metric_id);
            },
            [this]() { return get_max_packet_size(); },
            sample->metadata.metric_id);

        // add metric_id:metric_info to map
        metrics_[sample->metadata.metric_id] = std::move(metric_info);
    }
}
std::unique_ptr<SampleData> Command::pop_latest_sample(MetricId metric_id)
{
    // If metric exists and has sample
    if(metric_exists(metric_id) &&
       metrics_[metric_id]->latest_sample != nullptr) {
        std::unique_ptr<SampleData> sample =
            std::move(metrics_[metric_id]->latest_sample);
        metrics_[metric_id]->latest_sample = nullptr;
        return sample;

    } else {
        // return nullptr to signify invalid metric or
        // no new sample
        return nullptr;
    }
}

size_t Command::get_max_packet_size() { return max_packet_size_; }

std::unique_ptr<std::vector<uint8_t>> Command::get_sample_pkt(
    MetricId metric_id)
{
    // If metric exists and has sample
    if(metric_exists(metric_id) &&
       metrics_[metric_id]->sample_transmitter != nullptr) {
        return metrics_[metric_id]->sample_transmitter->get_pkt();

    } else {
        // return nullptr to signify invalid metric or
        // no new sample
        return nullptr;
    }
}

size_t Command::get_bps() { return bps_; }

std::unique_ptr<std::vector<MetricId>> Command::get_all_metric_ids()
{
    auto metric_ids = std::make_unique<std::vector<MetricId>>();

    // optimization to reserve enough space so we don't need
    // to continually increase the size of the vector
    metric_ids->reserve(metrics_.size());

    for(auto& [metric_id, _] : metrics_) {
        metric_ids->push_back(metric_id);
    }
    return metric_ids;
}

size_t Command::get_num_metrics() { return metrics_.size(); }

bool Command::metric_exists(MetricId metric_id)
{
    return metrics_.find(metric_id) != metrics_.end();
}

float Command::get_metric_token_threshold(MetricId metric_id)
{
    return metrics_[metric_id]->token_threshold;
}

void Command::print_all_metric_ids()
{
    for(const auto& metric : metrics_) {
        std::cout << metric.first << std::endl;
    }
}
