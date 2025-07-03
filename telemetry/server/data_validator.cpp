#include "data_validator.hpp"

DataValidator::DataValidator(const std::string& config_path) {
    std::ifstream f(config_path);   
    if (!f) {
        throw std::runtime_error("Cannot open limits file: " + config_path);
    }
    try {
        f >> limits;
        std::cerr << "Loaded validation limits:\n";
        for (auto& [key, val] : limits.items()) {
            std::cerr << "  " << key << ": [" << val[0] << ", " << val[1] << "]\n";
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing limits file: " + std::string(e.what()));
    }
}

bool DataValidator::validate(const TelemetryData& data) {
    bool valid = true;
        
        // Преобразование упакованных значений в исходные для валидации
        int original_y = static_cast<int>(data.y) - 32;
        float original_acceleration = static_cast<float>(data.acceleration) / 10.0f - 12.7f;

        auto check = [&](auto value, const std::string& field, const nlohmann::json& lim) {
            // Проверка существования поля в конфигурации
            if (!lim.contains(field)) {
                std::cerr << "  ! Field '" << field << "' not found in configuration\n";
                return false;
            }

            // Проверка формата ограничений
            if (!lim[field].is_array() || lim[field].size() != 2) {
                std::cerr << "  ! Invalid format for field '" << field << "' in configuration\n";
                return false;
            }

            // Определение типа для преобразования JSON-значений
            using ValueType = decltype(value);
            try {
                const ValueType min = lim[field][0].get<ValueType>();
                const ValueType max = lim[field][1].get<ValueType>();

                // Проверка выхода за границы диапазона
                if (value < min || value > max) {
                    std::cerr << "  ! " << field << " out of range: " 
                              << std::fixed << std::setprecision(2) << value
                              << " not in [" << min << ", " << max << "]\n";
                    return false;
                }
            } catch (const std::exception& e) {
                std::cerr << "  ! Type mismatch for field '" << field << "': " << e.what() << "\n";
                return false;
            }
            return true;
        };

        try {
            // Проверка всех полей с преобразованием упакованных значений
            valid &= check(data.x, "X", limits);  // X не требует преобразования
            valid &= check(original_y, "Y", limits);
            valid &= check(data.velocity, "V", limits);  // Velocity не требует преобразования
            valid &= check(data.mode, "M", limits);     // Mode не требует преобразования
            valid &= check(data.state, "S", limits);     // State не требует преобразования
            valid &= check(original_acceleration, "A", limits);
            valid &= check(data.power, "P", limits);     // Power не требует преобразования
            
        } catch (const std::exception& e) {
            std::cerr << "Validation error: " << e.what() << "\n";
            return false;
        }
        
        return valid;
    }