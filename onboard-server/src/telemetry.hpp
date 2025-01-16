#pragma once

#include "command.hpp"
#include "sample.hpp"
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

class Telemetry
{
  public:
    Telemetry(Command& command);

    /**
     * @brief Retrieves the next telemetry packet.
     *
     * The max size of the
     * data is limited to this.command_.get_max_packet_size()
     *
     * @return ptr to raw bytes of sample frame packet.
     */
    std::unique_ptr<std::vector<uint8_t>> pop();

  private:
    Command& command_;
    std::map<MetricId, int> metric_token_counts_;

    /**
     * Copy of command's metric ids, updated after each full
     * iteration through
     */
    std::unique_ptr<std::vector<MetricId>> current_metric_ids_;

    /**
     * Iterator over *current_metric_ids_
     */
    std::vector<MetricId>::iterator metric_iter_;

    /**
     * @brief Moves metric_iter_ to the next metric
     * 
     * Loops back to the beginning and refreshes metric ids if at the end
     *
     */
    void go_to_next_metric();

    /**
     * @brief Gets a packet from the current metric
     */
    std::unique_ptr<std::vector<uint8_t>> get_pkt_from_current_metric();

    void refresh_metric_ids();

    std::string get_current_metric_id();
};
