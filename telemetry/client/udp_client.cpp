#include "udp_client.hpp"
#include "network_utils.hpp"



UdpClient::UdpClient(const char* host, const char* port)
    : resolver_(io_context_),
      socket_(io_context_)
{
    endpoints_ = resolver_.resolve(host, port);
    socket_.open(boost::asio::ip::udp::v4());
}

void UdpClient::run() {
    TelemetryData data = input_telemetry_data();
    uint64_t packet = pack_data(data);
    std::cout << "Packed data: 0x" << std::hex << packet << std::dec << "\n";
    
    send_udp_packet(packet);
    uint8_t response = receive_udp_response();
    analyse_server_response(response);
}

TelemetryData UdpClient::input_telemetry_data() {
    TelemetryData data{};
    uint8_t tmp_x;
    int tmp_y;
    uint8_t tmp_velocity;
    uint8_t tmp_mode;
    uint8_t tmp_state;
    float tmp_acceleration;
    uint8_t tmp_power;
    
    auto validate_input = [](auto& value, auto min, auto max, const std::string& field) {
        while (true) {
            std::cout << "Enter " << field << " [" << min << "-" << max << "]: ";
            if (!(std::cin >> value)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Invalid input. Please enter a valid number.\n";
                continue;
            }
            
            if (value < min || value > max) {
                std::cerr << "Error: Value must be between " << min << " and " << max << ".\n";
                continue;
            }
            break;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    };

    std::cerr << "\n=== Data Input ===\n";
    
    // Ввод во временные переменные
    validate_input(tmp_x, 0, 63, "X");
    validate_input(tmp_y, -32, 31, "Y");
    validate_input(tmp_velocity, 0, 255, "Velocity");
    validate_input(tmp_mode, 0, 3, "Mode");
    validate_input(tmp_state, 0, 3, "State");
    validate_input(tmp_acceleration, -12.7f, 12.8f, "Acceleration");
    validate_input(tmp_power, 0, 130, "Power");
    
    // Присвоение значений битовым полям
    data.x = tmp_x;
    data.y = tmp_y + 32;  // Преобразование для упаковки
    data.velocity = tmp_velocity;
    data.mode = tmp_mode;
    data.state = tmp_state;
    data.acceleration = static_cast<uint8_t>((tmp_acceleration + 12.7f) * 10);
    data.power = tmp_power;
    
    std::cerr << "=================\n";
    return data;
}


void UdpClient::analyse_server_response(uint8_t response) {
    if (response == 1) {
        std::cout << "\n[RESULT] Data is valid\n";
    } else if (response == 0) {
        std::cout << "\n[RESULT] Data is invalid\n";
    } else {
        std::cout << "\n[ERROR] Invalid server response code: 0x" 
                  << std::hex << static_cast<int>(response) << std::dec << "\n";
    }
}

void UdpClient::send_udp_packet(uint64_t data) {
    if (endpoints_.empty()) {
        throw std::runtime_error("No endpoints available for sending");
    }
    uint64_t net_data = host_to_network_uint64(data);
    
    // Получаем endpoint из результата резолвинга
    const auto endpoint = endpoints_.begin()->endpoint();
    
    std::cerr << "\n[NETWORK] Sending UDP packet to "
              << endpoint.address().to_string() << ":" << endpoint.port() << "\n"
              << "  Size: " << sizeof(net_data) << " bytes\n"
              << "  Data (network order): ";
    
    const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(&net_data);
    for (size_t i = 0; i < sizeof(net_data); ++i) {
        std::cerr << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(byte_ptr[i]) << " ";
    }
    std::cerr << std::dec << "\n";
    
    socket_.send_to(boost::asio::buffer(&net_data, sizeof(net_data)), endpoint);
}

uint8_t UdpClient::receive_udp_response() {
    uint8_t response[2] = {0};
    boost::system::error_code ec;

    std::cerr << "\n[NETWORK] Waiting for UDP response...\n";
    
    size_t length = socket_.receive_from(
        boost::asio::buffer(response, sizeof(response)), 
        server_endpoint_,
        0,
        ec
    );

    std::cerr << "  Received: " << length << " bytes\n"
              << "  Data: 0x" << std::hex 
              << static_cast<int>(response[0]) << " "
              << static_cast<int>(response[1]) << std::dec << "\n";

    if (ec) {
        std::cerr << "  Receive error: " << ec.message() << "\n";
        return 0xFF;
    }

    if (length != 2) {
        std::cerr << "  Error: Invalid response size. Expected 2 bytes, got " 
                  << length << "\n";
        return 0xFF;
    }

    if (response[0] != 0) {
        std::cerr << "  Error: Invalid response format. First byte: 0x" 
                  << std::hex << static_cast<int>(response[0]) << std::dec << "\n";
        return 0xFF;
    }

    return response[1];
}