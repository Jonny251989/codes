#include "test_utils.hpp"
#include <boost/asio.hpp>
#include <array>
#include <memory>

namespace TestUtils {

void wait_for_server(const std::string& host, uint16_t port) {
    using namespace boost::asio;
    using ip::tcp;
    
    io_context io;
    tcp::resolver resolver(io);
    
    for (int i = 0; i < 30; ++i) {
        try {
            tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
            tcp::socket socket(io);
            connect(socket, endpoints);
            return; // Successfully connected
        } catch (const boost::system::system_error& e) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    throw std::runtime_error("Server not available after 30 attempts");
}

std::string execute_command(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    // Remove trailing newline
    if (!result.empty() && result[result.length()-1] == '\n') {
        result.erase(result.length()-1);
    }
    
    return result;
}

} // namespace TestUtils