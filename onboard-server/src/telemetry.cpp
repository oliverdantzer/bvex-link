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
    : command_(command), metric_iter_(command.get_metric_iterator()) {};

std::unique_ptr<std::vector<uint8_t>> Telemetry::pop(
    unsigned int retry_depth)
{
    metric_iter_.increment();

    if(metric_iter_.empty() || retry_depth > command_.get_num_metrics()) {
        return nullptr;
    }

    MetricId metric_id = metric_iter_.get_id();

    // if metric id not in metric token counts, add it with initial value
    // of 1
    if(metric_token_counts_.find(metric_id) == metric_token_counts_.end()) {
        metric_token_counts_[metric_id] = 1;
    }

    int& tokens = metric_token_counts_[metric_id];
    int threshold = metric_iter_.get_token_threshold();
    metric_token_counts_[metric_id]++;
    if(tokens >= threshold) {
        auto payload = metric_iter_.get_sample_pkt();
        if(payload != nullptr) {
            tokens = 0;
            return payload;
        }
    }
    tokens++;
    return pop(retry_depth + 1);
};