#include "network_utils.hpp"

uint64_t host_to_network_uint64(uint64_t value) {
    uint64_t result;
    uint8_t* src = reinterpret_cast<uint8_t*>(&value);
    uint8_t* dst = reinterpret_cast<uint8_t*>(&result);
    
    dst[0] = src[7];
    dst[1] = src[6];
    dst[2] = src[5];
    dst[3] = src[4];
    dst[4] = src[3];
    dst[5] = src[2];
    dst[6] = src[1];
    dst[7] = src[0];
    
    return result;
}

// Обратное преобразование идентично
uint64_t network_to_host_uint64(uint64_t value) {
    return host_to_network_uint64(value);
}

void send_udp_packet(
    boost::asio::ip::udp::socket& socket,
    const boost::asio::ip::udp::endpoint& endpoint,
    const uint64_t data
) {
    uint64_t net_data = host_to_network_uint64(data);
    
    std::cerr << "[NETWORK] Sending UDP packet to "
              << endpoint.address().to_string() << ":" << endpoint.port() << "\n";
    std::cerr << "  Size: " << sizeof(net_data) << " bytes\n";
    
    // Преобразуем для читаемого вывода
    const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(&net_data);
    std::cerr << "  Data (network order): ";
    for (size_t i = 0; i < sizeof(net_data); ++i) {
        std::cerr << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(byte_ptr[i]) << " ";
    }
    std::cerr << std::dec << "\n";
    
    socket.send_to(boost::asio::buffer(&net_data, sizeof(net_data)), endpoint);
}

uint8_t receive_udp_response(
    boost::asio::ip::udp::socket& socket,
    boost::asio::ip::udp::endpoint& endpoint
) {
    uint8_t response[2] = {0};
    boost::system::error_code ec;

    std::cerr << "[NETWORK] Waiting for UDP response...\n";
    
    size_t length = socket.receive_from(
        boost::asio::buffer(response, sizeof(response)), 
        endpoint,
        0,
        ec
    );

    std::cerr << "  Received: " << length << " bytes\n";
    std::cerr << "  Data: 0x" 
              << std::hex << static_cast<int>(response[0]) 
              << " " << static_cast<int>(response[1]) 
              << std::dec << "\n";

    if (ec) {
        std::cerr << "  Receive error: " << ec.message() << "\n";
        return 0xFF;
    }

    if (length != 2) {
        std::cerr << "  Error: Invalid response size. Expected 2 bytes, got " << length << "\n";
        return 0xFF;
    }

    if (response[0] != 0) {
        std::cerr << "  Error: Invalid response format. First byte: 0x" 
                  << std::hex << static_cast<int>(response[0]) << std::dec << "\n";
        return 0xFF;
    }

    return response[1];
}