#pragma once

#include <cstdint>
#include <cstring> // Для memcpy
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <optional>

#pragma pack(push, 1)

struct TelemetryData {
    // Младшие 16 бит (биты 15-0)
    uint64_t reserved : 16;   // R15-R0
    
    // Байты 2-3 (биты 23-16)
    uint64_t acceleration : 8; // A7-A0
    
    // Байты 4-5 (биты 31-24)
    uint64_t power : 8;        // P7-P0
    
    // Байты 6-7 (биты 39-32)
    uint64_t velocity : 8;     // V7-V0
    
    // Биты 41-40
    uint64_t mode : 2;         // M1-M0
    
    // Биты 43-42: Должны быть 00
    uint64_t zero1 : 2;        // Обязательно 0
    
    // Биты 45-44
    uint64_t state : 2;        // S1-S0
    
    // Биты 47-46: Должны быть 00
    uint64_t zero2 : 2;        // Обязательно 0
    
    // Биты 53-48
    uint64_t x : 6;            // X5-X0
    
    // Биты 55-54: Должны быть 00
    uint64_t zero3 : 2;        // Обязательно 0
    
    // Биты 61-56
    uint64_t y : 6;            // Y5-Y0

    // Биты 63-62: Должны быть 00
    uint64_t zero4 : 2;        // Обязательно 0
};
#pragma pack(pop)

uint64_t pack_data(const TelemetryData& data);
std::optional<TelemetryData> unpack_data(uint64_t packet);