#include "udp_client.hpp"
#include "protocol.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include <stdexcept>

int main(int argc, char* argv[]) {
    try {
        // 1. Проверка аргументов командной строки
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
            return 1;
        }

        // 2. Создание и запуск клиента
        UdpClient client(argv[1], argv[2]);
        client.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Client exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}