#pragma once
#include <stdbool.h>

// Runs a telemetry loop that sends simulated sensor data
// If measure_timing is true, prints timing statistics every 1000 samples
void run_telemetry_loop(const char* node, const char* service,
                        bool measure_timing);
