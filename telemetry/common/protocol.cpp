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
    
    // Дополнительное логирование
    std::cerr << "[UNPACK] Raw packet: 0x" << std::hex << packet << std::dec << "\n"
              << "  Bitfield analysis:\n"
              << "    X: " << data.x << " (bits 46-51)\n"
              << "    Y: " << data.y << " (bits 52-57) -> decoded: " << (static_cast<int>(data.y) - 32) << "\n"
              << "    State: " << data.state << " (bits 44-45)\n"
              << "    Mode: " << data.mode << " (bits 42-43)\n"
              << "    Velocity: " << static_cast<int>(data.velocity) << " (bits 34-41)\n"
              << "    Power: " << static_cast<int>(data.power) << " (bits 26-33)\n"
              << "    Acceleration: " << static_cast<int>(data.acceleration) 
              << " (bits 18-25) -> decoded: " << (static_cast<float>(data.acceleration)/10.0f - 12.7f) << "\n"
              << "    Reserved: 0x" << std::hex << data.reserved << std::dec << " (bits 0-17)\n";
    
    return data;
}