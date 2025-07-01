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
        return;
    }

    uint64_t host_data = network_to_host_uint64(recv_buffer_);
    TelemetryData data = unpack_data(host_data);
    
    // Валидируем ИСХОДНЫЕ значения
    bool is_valid = validator_.validate(data);
    response_[0] = 0;
    response_[1] = is_valid ? 1 : 0;
    
    socket_.async_send_to(
        boost::asio::buffer(response_, 2),
        remote_endpoint_,
        [this](const boost::system::error_code& error, size_t) {
            start_receive();
        }
    );
}