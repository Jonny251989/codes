#include "cli_interface.hpp"
#include "protocol.hpp"
#include "network_utils.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>\n";
            return 1;
        }

        boost::asio::io_context io_context;
        boost::asio::ip::udp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        
        boost::asio::ip::udp::socket socket(io_context);
        socket.open(boost::asio::ip::udp::v4());
        
        // Ввод данных
        TelemetryData data = input_telemetry_data();
        
        // Упаковка данных
        uint64_t packet = pack_data(data);

        std::cout << "Packed data: 0x" << std::hex << packet << std::dec << "\n";
        
        // Отправка данных
        send_udp_packet(socket, *endpoints.begin(), packet);
        
        // Получение ответа
        boost::asio::ip::udp::endpoint server_endpoint;
        uint8_t response = receive_udp_response(socket, server_endpoint);
        
        // Вывод результата
        print_server_response(response);
        
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}