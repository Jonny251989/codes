#pragma once

#include <json.hpp>
//#include <nlohmann/json.hpp>
#include "protocol.hpp"
#include <map>
#include <string>
#include <utility>
#include <fstream>
#include <stdexcept>
#include <iostream>

class DataValidator {
public:
    explicit DataValidator(const std::string& config_path);
    bool validate(const TelemetryData& data);

private:

    std::map<std::string, std::pair<int, int>> int_limits;
    std::map<std::string, std::pair<float, float>> float_limits;
    nlohmann::json limits;
};