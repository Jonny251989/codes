#pragma once

#include "network_utils.hpp"
#include <boost/asio.hpp>
#include "protocol.hpp"
#include <iomanip>
#include <limits>

class UdpClient {
public:
    // Конструктор с аргументами командной строки
    UdpClient(const char* host, const char* port);
    
    // Основной метод запуска клиента
    void run();

    TelemetryData input_telemetry_data();
    void analyse_server_response(uint8_t response);

private:
    // Отправка UDP-пакета
    void send_udp_packet(uint64_t data);

    
    // Прием UDP-ответа
    uint8_t receive_udp_response();
    
    // Члены класса
    boost::asio::io_context io_context_;
    boost::asio::ip::udp::resolver resolver_;
    boost::asio::ip::udp::resolver::results_type endpoints_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint server_endpoint_;
};