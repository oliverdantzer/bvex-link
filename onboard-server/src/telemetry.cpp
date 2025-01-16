#include "telemetry.hpp"
#include "command.hpp"
#include "sample.hpp"
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

Telemetry::Telemetry(Command& command)
    : command_(command), current_metric_ids_(command_.get_all_metric_ids()),
      metric_iter_(current_metric_ids_->begin()) {};

std::unique_ptr<std::vector<uint8_t>> Telemetry::pop()
{
    if(current_metric_ids_->empty()) {
        refresh_metric_ids();
    }

    for(unsigned int retry_count = 0; retry_count < current_metric_ids_->size();
        ++retry_count) {
        MetricId metric_id = get_current_metric_id();

        if(metric_token_counts_.find(metric_id) == metric_token_counts_.end()) {
            metric_token_counts_[metric_id] = 1;
        }

        int& tokens = metric_token_counts_[metric_id];
        int threshold = command_.get_metric_token_threshold(metric_id);
        metric_token_counts_[metric_id]++;
        if(tokens >= threshold) {
            auto payload = command_.get_sample_pkt(metric_id);
            if(payload != nullptr) {
                tokens = 0;
                go_to_next_metric();
                return payload;
            }
        }
        tokens++;
        go_to_next_metric();
    }

    return nullptr;
};

// Increments metric_iter_ through current_metric_ids_.
// Updates current_metric_ids_ after each loop through
// current_metric_ids_.
void Telemetry::go_to_next_metric()
{
    metric_iter_++;
    // if we've reached the end of the current_metric_ids_
    if(metric_iter_ == current_metric_ids_->end()) {
        refresh_metric_ids();
    }
};

std::string Telemetry::get_current_metric_id() { return *metric_iter_; };

void Telemetry::refresh_metric_ids()
{
    current_metric_ids_ = command_.get_all_metric_ids();
    metric_iter_ = current_metric_ids_->begin();
}