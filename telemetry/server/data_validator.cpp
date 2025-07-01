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
    
    auto check = [&](auto value, const std::string& field, auto min, auto max) {
        if (value < min || value > max) {
            std::cerr << "  ! " << field << " out of range: " 
                      << std::fixed << std::setprecision(2) << value
                      << " not in [" << min << ", " << max << "]\n";
            return false;
        }
        return true;
    };

    try {
        valid &= check(data.x, "X", 
                      limits["X"][0].get<int>(), 
                      limits["X"][1].get<int>());
                      
        valid &= check(data.y, "Y", 
                      limits["Y"][0].get<int>(), 
                      limits["Y"][1].get<int>());
                      
        valid &= check(data.velocity, "Velocity", 
                      limits["V"][0].get<int>(), 
                      limits["V"][1].get<int>());
                      
        valid &= check(data.mode, "Mode", 
                      limits["M"][0].get<int>(), 
                      limits["M"][1].get<int>());
                      
        valid &= check(data.state, "State", 
                      limits["S"][0].get<int>(), 
                      limits["S"][1].get<int>());
                      
        valid &= check(data.acceleration, "Acceleration", 
                      limits["A"][0].get<float>(), 
                      limits["A"][1].get<float>());
                      
        valid &= check(data.power, "Power", 
                      limits["P"][0].get<int>(), 
                      limits["P"][1].get<int>());
        
    } catch (const std::exception& e) {
        std::cerr << "Validation error: " << e.what() << "\n";
        return false;
    }
    
    return valid;
}