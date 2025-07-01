#pragma once

#include "protocol.hpp"
#include <iostream>
#include <limits>

TelemetryData input_telemetry_data();
void print_server_response(uint8_t response);