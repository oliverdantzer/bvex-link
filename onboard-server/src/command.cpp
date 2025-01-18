#include "command.hpp"
#include "sample.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>

Command::Command(size_t init_bps, size_t init_max_packet_size)
    : bps_(init_bps), max_packet_size_(init_max_packet_size)
{
}

void Command::add_sample(std::unique_ptr<SampleData> sample)
{
    if(sample == nullptr) {
        throw std::invalid_argument("SampleData cannot be null");
    }
    if(metric_exists(sample->metadata.metric_id)) {
        metrics_[sample->metadata.metric_id].latest_sample = std::move(sample);
    } else {
        // Create metric_info, populate it with data from sample
        std::string metric_id = sample->metadata.metric_id;
        MetricInfo metric_info{
            .metric_id = metric_id,
            .token_threshold = 1,
            .latest_sample = std::move(sample),
            .sample_transmitter = std::make_unique<SampleTransmitter>(
                [this, metric_id]() {
                    return pop_latest_sample(metric_id);
                },
                [this]() { return get_max_packet_size(); },
                metric_id)};

        // add metric_id:metric_info to map
        metrics_[metric_id] = std::move(metric_info);
    }
}
std::unique_ptr<SampleData> Command::pop_latest_sample(MetricId metric_id)
{
    // If metric exists and has sample
    if(metric_exists(metric_id) &&
       metrics_[metric_id].latest_sample != nullptr) {
        std::unique_ptr<SampleData> sample =
            std::move(metrics_[metric_id].latest_sample);
        metrics_[metric_id].latest_sample = nullptr;
        return sample;

    } else {
        // return nullptr to signify invalid metric or
        // no new sample
        return nullptr;
    }
}

size_t Command::get_max_packet_size() { return max_packet_size_; }

size_t Command::get_bps() { return bps_; }

size_t Command::get_num_metrics() { return metrics_.size(); }

bool Command::metric_exists(MetricId metric_id)
{
    return metrics_.find(metric_id) != metrics_.end();
}

void Command::print_all_metric_ids()
{
    for(const auto& metric : metrics_) {
        std::cout << metric.first << std::endl;
    }
}

void MetricIterator::increment()
{
    if(empty()) {
        throw(std::out_of_range("Metrics are empty"));
    }
    ++current_iterator_;
}

unsigned int MetricIterator::get_token_threshold()
{
    if(empty()) {
        throw(std::out_of_range("Metrics are empty"));
    }
    return current_iterator_->second.token_threshold;
}

std::unique_ptr<std::vector<uint8_t>> MetricIterator::get_sample_pkt()
{
    if(empty()) {
        throw(std::out_of_range("Metrics are empty"));
    }

    if(current_iterator_->second.sample_transmitter != nullptr) {
        return current_iterator_->second.sample_transmitter->get_pkt();

    } else {
        // return nullptr to signify no new sample
        return nullptr;
    }
}

MetricIterator Command::get_metric_iterator()
{
    return MetricIterator([this]() { return get_metrics_begin_iter(); },
                          [this]() { return get_metrics_end_iter(); });
}

std::map<MetricId, MetricInfo>::iterator Command::get_metrics_begin_iter()
{
    return metrics_.begin();
}

std::map<MetricId, MetricInfo>::iterator Command::get_metrics_end_iter()
{
    return metrics_.end();
}

bool MetricIterator::empty()
{
    return get_begin_iterator() == get_end_iterator();
}

const std::string MetricIterator::get_id()
{
    if(empty()) {
        throw std::out_of_range("Iterator is at the end");
    }

    return current_iterator_->first;
}