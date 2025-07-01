#pragma once

#include <boost/asio.hpp>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <bit>

uint64_t host_to_network_uint64(uint64_t value);
uint64_t network_to_host_uint64(uint64_t value);

void send_udp_packet(
    boost::asio::ip::udp::socket& socket,
    const boost::asio::ip::udp::endpoint& endpoint,
    const uint64_t data
);

uint8_t receive_udp_response(
    boost::asio::ip::udp::socket& socket,
    boost::asio::ip::udp::endpoint& endpoint
);