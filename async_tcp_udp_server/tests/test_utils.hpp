#pragma once

#include <string>
#include <chrono>
#include <thread>

namespace TestUtils {

/**
 * Wait for server to become available
 */
void wait_for_server(const std::string& host, uint16_t port);

/**
 * Execute shell command and return output
 */
std::string execute_command(const std::string& command);

} // namespace TestUtils