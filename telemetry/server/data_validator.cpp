#include "data_validator.hpp"

using json = nlohmann::json;

DataValidator::DataValidator(const std::string& config_path) {
    std::ifstream f(config_path);   
    if (!f) {
        throw std::runtime_error("Cannot open limits file: " + config_path);
    }
    
    try {
        json config;
        f >> config;
        
        // Проверка наличия поля "limits"
        if (!config.contains("limits")) {
            throw std::runtime_error("Missing 'limits' field in config");
        }
        
        // Проверка что "limits" является массивом
        if (!config["limits"].is_array()) {
            throw std::runtime_error("'limits' field must be an array");
        }
        
        // Проверка что массив не пустой
        if (config["limits"].empty()) {
            throw std::runtime_error("'limits' array is empty");
        }
        
        // Извлекаем первый элемент массива как объект с ограничениями
        limits = config["limits"][0];
        
        // Проверка что элемент является объектом
        if (!limits.is_object()) {
            throw std::runtime_error("First element of 'limits' array must be a JSON object");
        }
        
        std::cerr << "Loaded validation limits:\n";
        for (auto& [key, val] : limits.items()) {
            // Проверка формата каждого ограничения
            if (!val.is_array() || val.size() != 2) {
                std::cerr << "  ! [WARNING] Invalid format for field '" << key 
                          << "': expected array of two elements\n";
                continue;
            }
            std::cerr << "  " << key << ": [" << val[0] << ", " << val[1] << "]\n";
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing limits file: " + std::string(e.what()));
    }
}

bool DataValidator::validate(const TelemetryData& data) {
    bool valid = true;
    
    // Преобразование значений
    int original_y = static_cast<int>(data.y) - 32;
    float original_acceleration = static_cast<float>(data.acceleration) / 10.0f - 12.7f;
    
    // Вывод преобразованных значений
    std::cerr << "  Converted values for validation:\n"
              << "    Y: " << original_y << "\n"
              << "    Acceleration: " << original_acceleration << "\n";

    auto check = [&](auto value, const std::string& field) {
        // Проверка наличия поля
        if (!limits.contains(field)) {
            std::cerr << "  ! [ERROR] Field '" << field << "' not found in configuration\n";
            return false;
        }

        // Проверка формата
        if (!limits[field].is_array() || limits[field].size() != 2) {
            std::cerr << "  ! [ERROR] Invalid format for field '" << field 
                      << "': expected array of two elements\n";
            return false;
        }

        // Проверка типа и диапазона
        using ValueType = decltype(value);
        try {
            const ValueType min = limits[field][0].get<ValueType>();
            const ValueType max = limits[field][1].get<ValueType>();

            if (value < min || value > max) {
                std::cerr << "  ! [ERROR] " << field << " out of range: " 
                          << std::fixed << std::setprecision(2) << value
                          << " not in [" << min << ", " << max << "]\n";
                return false;
            }
        } catch (const std::exception& e) {
            std::cerr << "  ! [ERROR] Type mismatch for field '" << field << "': " << e.what() << "\n";
            return false;
        }
        return true;
    };

    try {
        valid &= check(data.x, "X");
        valid &= check(original_y, "Y");
        valid &= check(data.velocity, "V");
        valid &= check(data.mode, "M");
        valid &= check(data.state, "S");
        valid &= check(original_acceleration, "A");
        valid &= check(data.power, "P");
    } catch (const std::exception& e) {
        std::cerr << "Validation error: " << e.what() << "\n";
        return false;
    }
    
    return valid;
}