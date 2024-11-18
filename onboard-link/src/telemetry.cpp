#include "telemetry.hpp"
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

Telemetry::Telemetry(Command& command)
    : command_(command), metric_token_limits_(), metric_token_counts_(),
      metric_samples_(), metric_ids_iter(command_.get_metric_ids().begin()) {};

boost::shared_ptr<std::vector<uint8_t>> Telemetry::pop()
{
    boost::shared_ptr<std::vector<uint8_t>> payload = nullptr;
    if(metric_ids_iter != command_.get_metric_ids().end()) {
        if(metric_token_counts_[*metric_ids_iter] >=
           calc_token_threshold(*metric_ids_iter)) {
            payload = pop_current_metric();
        }
    }
    go_to_next_state();
    return payload;
};

unsigned int Telemetry::calc_token_threshold(MetricId metric_id)
{
    size_t sum_sample_max_szs = 1;
    return (command_.get_metric_share(metric_id) *
            metric_samples_[metric_id]->get_size()) /
           sum_sample_max_szs;
};

void Telemetry::increment_position()
{
    // position_ptr = (position_ptr + 1) % command_.metric_ids.size();
    if(metric_ids_iter == command_.get_metric_ids().end()) {
        metric_ids_iter = command_.get_metric_ids().begin();
    } else {
        metric_ids_iter++;
    }
};

void Telemetry::go_to_next_state()
{
    increment_position();
    if(metric_ids_iter != command_.get_metric_ids().end()) {
        metric_token_counts_[*metric_ids_iter]++;
    }
};

boost::shared_ptr<std::vector<uint8_t>> Telemetry::pop_current_metric()
{
    // TODO: increment through sample ids instead of setting to 0
    auto payload = boost::make_shared<std::vector<uint8_t>>(
        metric_samples_[*metric_ids_iter]->to_bytes(0));
    metric_token_counts_[*metric_ids_iter] = 0;
    metric_samples_[*metric_ids_iter] = nullptr;
    return payload;
};

// std::map<std::string, int> stringToIntMap;
// metric_ids = list of metrics metric_index increment_position()
//     decrement_iters_till_send(metric_id)

//         void pop
// {

//     metric_id = metric_ids[position] decrement_iters_till_send(metric_id)
//         metric_packet_size metric_iters_until_send =
//             map[metric_id] increment_metric()
// }