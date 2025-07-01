#pragma once

#include <boost/asio.hpp>
#include "data_validator.hpp"
#include "network_utils.hpp"
#include "protocol.hpp"
#include <iostream>

class UdpServer {
public:
    UdpServer(boost::asio::io_context& io_context, uint16_t port, const std::string& config_path);
    void start_receive();

private:
    void handle_receive(const boost::system::error_code& error, size_t bytes);
    void handle_send(boost::asio::ip::udp::endpoint endpoint, char response[2]);

    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    uint64_t recv_buffer_;
    char response_[2];
    DataValidator validator_;
};