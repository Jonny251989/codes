#include "udp_client.hpp"

UdpClient::UdpClient(const char* host, const char* port)
    : resolver_(io_context_),
      socket_(io_context_)
{
    endpoints_ = resolver_.resolve(host, port);
    socket_.open(boost::asio::ip::udp::v4());
}

void UdpClient::run() {
    while (true) {
        TelemetryData data = input_telemetry_data();
        uint64_t packet = pack_data(data);
        std::cout << "Packed data: 0x" << std::hex << packet << std::dec << "\n";
        
        send_udp_packet(packet);
        std::array<uint8_t, 2> response = receive_udp_response();
        analyse_server_response(response);
        // Запрос на продолжение
        std::cout << "\nContinue? (y/n): ";
        std::string answer;
        std::getline(std::cin, answer);
        
        if (answer == "n" || answer == "N") {
            std::cout << "Exiting client...\n";
            break;
        }
    }
}
template <typename T>
void UdpClient::validate_input(T& value_ref, T min, T max, const std::string& field){
    using ValueType = std::decay_t<decltype(value_ref)>;
        
    while (true) {
        std::cout << "Enter " << field << " [" << min << "-" << max << "]: ";
        
        std::string input_line;
        
        if (!std::getline(std::cin, input_line)) {
            if (std::cin.eof()) {
                std::cerr << "Error: End of input reached. Exiting.\n";
                exit(1);
            }
            std::cerr << "Error reading input. Please try again.\n";
            continue;
        }
        
        // Пропуск пустых строк
        if (input_line.empty()) {
            std::cerr << "Empty input. Please enter a value.\n";
            continue;
        }
        
        size_t pos;
        long value = std::stol(input_line, &pos);
        value_ref = static_cast<T>(value);

        if (value_ref < min || value_ref > max) {
            std::cerr << "Error: Value must be between " << min << " and " << max << ".\n";
            continue;
        }
        break;
    }
}

TelemetryData UdpClient::input_telemetry_data() {
    TelemetryData data{};
    int tmp_int; // Используем int для всех целочисленных значений
    float tmp_float;
    
    // Для X (0-63)
    validate_input(tmp_int, 0, 63, "X");
    data.x = tmp_int;
    
    // Для Y (-32-31)
    validate_input(tmp_int, -32, 31, "Y");
    data.y = tmp_int + 32;
    
    // Для Velocity (0-255)
    validate_input(tmp_int, 0, 255, "Velocity");
    data.velocity = tmp_int;
    
    // Для Mode (0-3)
    validate_input(tmp_int, 0, 3, "Mode");
    data.mode = tmp_int;
    
    // Для State (0-3)
    validate_input(tmp_int, 0, 3, "State");
    data.state = tmp_int;
    
    // Для Acceleration
    validate_input(tmp_float, -12.7f, 12.8f, "Acceleration");
    data.acceleration = static_cast<uint8_t>((tmp_float + 12.7f) * 10);
    
    // Для Power (0-130)
    validate_input(tmp_int, 0, 130, "Power");
    data.power = tmp_int;
    
    return data;
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

void UdpClient::analyse_server_response(std::array<uint8_t, 2> response) {
    if (response[0] == 0) {
        if (response[1] == 1) {
            std::cout << "  [RESULT] Data is valid\n";
        } else if (response[1] == 0) {
            std::cout << "  [RESULT] Data is invalid\n";
        } else {
            std::cout << "  [ERROR] Unknown status code: " 
                      << static_cast<int>(response[1]) << "\n";
        }
    } else {
        std::cout << "  [ERROR] Invalid header byte: " 
                  << static_cast<int>(response[0]) << "\n";
    }
}


std::array<uint8_t, 2> UdpClient::receive_udp_response() {
    std::array<uint8_t,2> response = {0};
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
    }
    return response;
}