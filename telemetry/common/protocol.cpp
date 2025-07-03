#include "protocol.hpp"

uint64_t pack_data(const TelemetryData& data) {
    uint64_t packet = 0;
    memcpy(&packet, &data, sizeof(packet));
    
    std::cerr << "\n[PACK] Result: 0x" 
              << std::hex << std::setw(16) << std::setfill('0') 
              << packet << std::dec << "\n";
    return packet;
}

TelemetryData unpack_data(uint64_t packet) {
    TelemetryData data;
    memcpy(&data, &packet, sizeof(data));
    return data;
}