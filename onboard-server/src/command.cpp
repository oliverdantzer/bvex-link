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
                [this, metric_id]() { return pop_latest_sample(metric_id); },
                [this]() { return get_max_packet_size(); }, metric_id)};

        // add metric_id:metric_info to map
        metrics_.insert(std::make_pair(metric_id, std::move(metric_info)));
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

MetricIterator Command::get_metric_iterator()
{
    auto begin = [this]() { return metrics_.begin(); };
    auto end = [this]() { return metrics_.end(); };
    return MetricIterator(begin, end);
}

MetricIterator::MetricIterator(
    std::function<std::map<MetricId, MetricInfo>::iterator()>
        get_begin_iterator,
    std::function<std::map<MetricId, MetricInfo>::iterator()> get_end_iterator)
    : get_begin_iterator(get_begin_iterator),
      get_end_iterator(get_end_iterator),
      current_iterator_(get_begin_iterator())
{
}

std::optional<SampleInfo> MetricIterator::get_next_metric_sample()
{
    // condition can be true because metrics starts off empty
    if(!is_iterator_valid()) {
        if(metrics_empty()) {
            return std::nullopt;
        }
        current_iterator_ = get_begin_iterator();
    }

    auto return_iter = current_iterator_;
    auto get_pkt = [return_iter]() -> std::unique_ptr<std::vector<uint8_t>> {
        if(return_iter->second.sample_transmitter != nullptr) {
            return return_iter->second.sample_transmitter->get_pkt();
        } else {
            // return nullptr to signify no new sample
            return nullptr;
        }
    };

    ++current_iterator_;
    if(!is_iterator_valid()) {
        // iterator has incremented past end, roll back to start
        current_iterator_ = get_begin_iterator();
    }

    return SampleInfo{
        .metric_id = return_iter->first,
        .token_threshold = return_iter->second.token_threshold,
        .get_pkt = get_pkt,
    };
}

bool MetricIterator::is_iterator_valid()
{
    return current_iterator_ != get_end_iterator();
}

bool MetricIterator::metrics_empty()
{
    return get_begin_iterator() == get_end_iterator();
}