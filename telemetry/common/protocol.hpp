#pragma once

#include <cstdint>
#include <cstring> // Для memcpy
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>

#pragma pack(push, 1)
struct TelemetryData {
    uint64_t reserved : 18;   // 18 бит (0-17)
    uint64_t acceleration : 8; // 8 бит (18-25)
    uint64_t power : 8;        // 8 бит (26-33)
    uint64_t velocity : 8;     // 8 бит (34-41)
    uint64_t mode : 2;         // 2 бита (42-43)
    uint64_t state : 2;        // 2 бита (44-45)
    uint64_t x : 6;            // 6 бит (46-51)
    uint64_t y : 6;            // 6 бит (52-57)
    uint64_t unused : 6;       // 6 неиспользуемых бит (58-63)
};
#pragma pack(pop)

uint64_t pack_data(const TelemetryData& data);
TelemetryData unpack_data(uint64_t packet);