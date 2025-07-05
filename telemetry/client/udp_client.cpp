#include "udp_client.hpp"

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
    int tmp_int; // Используем int для всех целочисленных значений
    float tmp_float;
    
    auto validate_input = [](auto& value_ref, auto min, auto max, const std::string& field) {
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
            
            // Удаление \r для кросс-платформенности
            input_line.erase(std::remove(input_line.begin(), input_line.end(), '\r'), input_line.end());
            
            // Пропуск пустых строк
            if (input_line.empty()) {
                std::cerr << "Empty input. Please enter a value.\n";
                continue;
            }
            
            std::istringstream iss(input_line);
            iss.imbue(std::locale("C"));
            
            if constexpr (std::is_floating_point_v<ValueType>) {
                double temp;
                if (!(iss >> temp)) {
                    std::cerr << "Invalid input. Please enter a valid floating point number.\n";
                    continue;
                }
                
                // Проверка на дополнительные символы
                char remaining;
                if (iss >> remaining) {
                    std::cerr << "Invalid input: extra characters detected.\n";
                    continue;
                }
                
                value_ref = static_cast<ValueType>(temp);
            } 
            else {
                long temp;  // Используем long для обработки больших чисел
                if (!(iss >> temp)) {
                    std::cerr << "Invalid input. Please enter a valid integer.\n";
                    continue;
                }
                
                // Проверка на дополнительные символы
                char remaining;
                if (iss >> remaining) {
                    std::cerr << "Invalid input: extra characters detected.\n";
                    continue;
                }
                
                // Проверка диапазона типа
                if (temp < std::numeric_limits<ValueType>::min() || 
                    temp > std::numeric_limits<ValueType>::max()) {
                    std::cerr << "Error: Value out of range for type.\n";
                    continue;
                }
                
                value_ref = static_cast<ValueType>(temp);
            }
            
            if (value_ref < min || value_ref > max) {
                std::cerr << "Error: Value must be between " << min << " and " << max << ".\n";
                continue;
            }
            break;
        }
    };
    
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