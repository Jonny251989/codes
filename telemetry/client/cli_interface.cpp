#include "cli_interface.hpp"


TelemetryData input_telemetry_data() {
    TelemetryData data{};
    int tmp_int;
    float tmp_float;
    
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
    
    // Для целочисленных значений
    validate_input(tmp_int, 0, 63, "X");
    data.x = static_cast<uint8_t>(tmp_int);
    
    validate_input(tmp_int, -32, 31, "Y");
    data.y = static_cast<int8_t>(tmp_int);
    
    validate_input(tmp_int, 0, 255, "Velocity");
    data.velocity = static_cast<uint8_t>(tmp_int);
    
    validate_input(tmp_int, 0, 3, "Mode");
    data.mode = static_cast<uint8_t>(tmp_int);
    
    validate_input(tmp_int, 0, 3, "State");
    data.state = static_cast<uint8_t>(tmp_int);
    
    validate_input(tmp_float, -12.7f, 12.8f, "Acceleration");
    data.acceleration = tmp_float;
    
    // Для power используем 0-130 как указано в задании
    validate_input(tmp_int, 0, 130, "Power");
    data.power = static_cast<uint8_t>(tmp_int);
    
    std::cerr << "=================\n";
    return data;
}

void print_server_response(uint8_t response) {
    if (response == 1) {
        std::cout << "Data is valid\n";
    } else if (response == 0) {
        std::cout << "Data is invalid\n";
    } else {
        std::cout << "Invalid server response\n";
    }
}