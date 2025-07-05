#include "udp_server.hpp"

UdpServer::UdpServer(boost::asio::io_context& io_context, uint16_t port, const std::string& config_path)
    : socket_(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
      validator_(config_path) {
    start_receive();
}

void UdpServer::start_receive() {
    socket_.async_receive_from(
        boost::asio::buffer(&recv_buffer_, sizeof(recv_buffer_)),
        remote_endpoint_,
        [this](const boost::system::error_code& error, size_t bytes) {
            handle_receive(error, bytes);
        }
    );
}

void UdpServer::handle_receive(const boost::system::error_code& error, size_t bytes) {
    if (error) {
        std::cerr << "Receive error: " << error.message() << "\n";
        return;
    }

    const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(&recv_buffer_);

    uint64_t host_data = network_to_host_uint64(recv_buffer_);
    TelemetryData data = unpack_data(host_data);
    
    // Логирование распакованных данных
    std::cout << "  Unpacked data:\n"
              << "    X: " << data.x << "\n"
              << "    Y (raw): " << data.y << " (decoded: " << (static_cast<int>(data.y)) - 32 << ")\n"
              << "    Velocity: " << static_cast<int>(data.velocity) << "\n"
              << "    Mode: " << static_cast<int>(data.mode) << "\n"
              << "    State: " << static_cast<int>(data.state) << "\n"
              << "    Acceleration (raw): " << static_cast<int>(data.acceleration) 
              << " (decoded: " << static_cast<float>(data.acceleration)/10.0f - 12.7f << ")\n"
              << "    Power: " << static_cast<int>(data.power) << "\n"
              << "    Reserved: 0x" << std::hex << data.reserved << std::dec << "\n";
    
    // Валидируем данные
    bool is_valid = validator_.validate(data);
    response_[0] = 0;
    response_[1] = is_valid ? 1 : 0;
    
    std::cout << "  Validation result: " << (is_valid ? "VALID" : "INVALID") << "\n";
    
    socket_.async_send_to(
        boost::asio::buffer(response_, 2),
        remote_endpoint_,
        [this](const boost::system::error_code& error, size_t) {
            if (error) {
                std::cout << "  Send response error: " << error.message() << "\n";
            } else {
                std::cout << "  Response sent: 0x" << std::hex 
                          << static_cast<int>(response_[0]) << " "
                          << static_cast<int>(response_[1]) << std::dec << "\n";
            }
            start_receive();
        }
    );
}