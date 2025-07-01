#pragma once

#include <cstdint>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>

struct TelemetryData {
    int x;           
    int y;           
    int velocity;    
    int state;       
    int mode;       
    float acceleration;
    int power;       
    uint16_t reserved;
};

uint64_t pack_data(const TelemetryData& data);
TelemetryData unpack_data(uint64_t packet);