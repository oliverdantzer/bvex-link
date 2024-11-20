#include "telemetry.hpp"
#include "command.hpp"
#include "sample.hpp"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <map>
#include <string>
#include <vector>

Telemetry::Telemetry(Command& command)
    : command_(command), metric_token_limits_(), metric_token_counts_(),
      metric_samples_(), current_metric_ids_(command_.get_all_metric_ids()),
      metric_iter_(current_metric_ids_->begin()) {};

boost::shared_ptr<std::vector<uint8_t>> Telemetry::pop()
{
    // if iter gets to end, current_metric_ids_ must be
    // empty
    if(metric_iter_ == (*current_metric_ids_).end()) {
        current_metric_ids_ = command_.get_all_metric_ids();
        if(current_metric_ids_->size() == 0) {
            return nullptr;
        } else {
            metric_iter_ = (*current_metric_ids_).begin();
        }
    }
    unsigned int retry_count = 0;
    boost::shared_ptr<std::vector<uint8_t>> payload = nullptr;
    while(retry_count < command_.get_num_metrics()) {
        MetricId metric_id = *metric_iter_;
        // if metric not yet added:
        //   start token count at 1
        //   add its max size to sum
        if(metric_token_counts_.find(metric_id) == metric_token_counts_.end()) {
            metric_token_counts_[metric_id] = 1;
            sum_max_sizes_ += command_.get_metric_max_sample_size(metric_id);
        }
        std::cout << "checking metric: " << metric_id << std::endl;
        int tokens = metric_token_counts_[metric_id];
        int threshold = calc_token_threshold(metric_id);
        std::cout << "count: " << std::to_string(tokens)
                  << ", threshold: " << std::to_string(threshold) << std::endl;
        if(tokens >= threshold) {
            payload = pop_current_metric();
            if(payload != nullptr) {
                return payload;
            }
        }
        retry_count++;
        go_to_next_state();
    }

    return nullptr;
};

unsigned int Telemetry::calc_token_threshold(MetricId metric_id)
{
    float share = command_.get_metric_share(metric_id);
    size_t max_sz = command_.get_metric_max_sample_size(metric_id);
    std::cout << "share: " << std::to_string(share) << ", max_sz: " << max_sz
              << std::endl;
    return (share * max_sz) / sum_max_sizes_;
};

void Telemetry::increment_position()
{
    metric_iter_++;
    if(metric_iter_ == current_metric_ids_->end()) {
        // update current_metric_ids to new pointer to
        // new vector of metric ids.
        // this invalidates our current metric_iter_
        current_metric_ids_ = command_.get_all_metric_ids();
        // set metric_iter_ to beginning of new metric id list
        metric_iter_ = current_metric_ids_->begin();
    }
};

void Telemetry::go_to_next_state()
{
    increment_position();
    metric_token_counts_[*metric_iter_]++;
};

boost::shared_ptr<std::vector<uint8_t>> Telemetry::pop_current_metric()
{
    // TODO: increment through sample ids instead of setting to 0
    boost::shared_ptr<std::vector<uint8_t>> payload = nullptr;
    MetricId metric_id = *metric_iter_;
    metric_token_counts_[metric_id] = 1;
    std::optional<SampleDownlinkData> sample_info =
        command_.pop_latest_sample(metric_id);
    if(sample_info.has_value()) {
        payload = boost::make_shared<std::vector<uint8_t>>(
            (*(sample_info->sample)).to_bytes(sample_info->sample_id));
    }
    return payload;
};