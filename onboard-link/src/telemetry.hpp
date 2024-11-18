#pragma once

#include "command.hpp"
#include "sample.hpp"
#include <boost/shared_ptr.hpp>
#include <map>
#include <string>
#include <vector>
#include <set>

class Telemetry
{
  public:
    Telemetry(Command& command);

    /**
     * @brief Retrieves the next telemetry data as a JSON string.
     *
     * This function pops the next telemetry data from the internal queue
     * and returns it as a JSON-formatted string. The max size of the
     * data is limited to this.command_.get_max_packet_size()
     *
     * @return std::string The next telemetry data in JSON format.
     */
    boost::shared_ptr<std::vector<uint8_t>> pop();

  private:
    Command& command_;
    std::map<MetricId, int> metric_token_limits_;
    std::map<MetricId, int> metric_token_counts_;
    std::map<MetricId, boost::shared_ptr<SampleData>> metric_samples_;
    unsigned int position_ptr;
    std::set<MetricId>::iterator metric_ids_iter;

    /**
     * @brief Returns the number of tokens a metric needs to be sent
     *
     * Calculates by taking (command.get_share(metric_id) * )
     *
     * @param metric_id The id of the metric
     */
    unsigned int calc_token_threshold(MetricId metric_id);

    /**
     * @brief Moves position ptr to next metric
     *
     * Loops back to the beginning if at the end
     *
     * @note This is a helper function for go_to_next_state.
     */
    void increment_position();

    /**
     * @brief Increments position and gives the metric at the new position a
     * token
     *
     */
    void go_to_next_state();

    /**
     * @brief Pops a sample from the current metric
     *
     *
     *
     * @throws x If the metrics does not have enough tokens to meet threshold
     */
    boost::shared_ptr<std::vector<uint8_t>> pop_current_metric();
};
