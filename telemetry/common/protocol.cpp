#include "protocol.hpp"

uint64_t pack_data(const TelemetryData& data) {
    uint64_t packet = 0;
    
    // 1. Y: [-32,31] → [0,63] (6 бит, биты 52-57)
    int8_t y_val = data.y + 32;
    if (y_val < 0) y_val = 0;
    if (y_val > 63) y_val = 63;
    packet |= static_cast<uint64_t>(y_val) << 52;

    // 2. X: [0-63] (6 бит, биты 46-51)
    uint8_t x_val = data.x;
    if (x_val > 63) x_val = 63;
    packet |= static_cast<uint64_t>(x_val) << 46;

    // 3. State: 2 бита (биты 44-45)
    packet |= static_cast<uint64_t>(data.state & 0x03) << 44;

    // 4. Mode: 2 бита (биты 42-43)
    packet |= static_cast<uint64_t>(data.mode & 0x03) << 42;

    // 5. Velocity: 8 бит (биты 34-41)
    uint8_t velocity_val = data.velocity;
    if (velocity_val > 255) velocity_val = 255;  // Добавлено ограничение
    packet |= static_cast<uint64_t>(velocity_val) << 34;

    // 6. Power: 8 бит (биты 26-33) - ДОБАВЛЕНО ОГРАНИЧЕНИЕ
    uint8_t power_val = data.power;
    if (power_val > 130) power_val = 130;  // Максимальное значение по ТЗ
    packet |= static_cast<uint64_t>(power_val) << 26;

    // 7. Acceleration: 8 бит (биты 18-25)
    float accel = data.acceleration;
    if (accel < -12.7f) accel = -12.7f;
    if (accel > 12.8f) accel = 12.8f;
    uint8_t a_byte = static_cast<uint8_t>(std::round((accel + 12.7) * 10));
    packet |= static_cast<uint64_t>(a_byte) << 18;

    // 8. Reserve: 16 бит (биты 0-17)
    packet |= (data.reserved & 0xFFFF);

    // Логирование
    std::cerr << "\n[PACK] Result: 0x" 
              << std::hex << std::setw(16) << std::setfill('0') 
              << packet << std::dec << "\n";
    std::cerr << "  Y: " << static_cast<int>(data.y) << " → " 
              << static_cast<int>(y_val) << "\n";
    std::cerr << "  X: " << static_cast<int>(data.x) << " → " 
              << static_cast<int>(x_val) << "\n";
    std::cerr << "  Velocity: " << static_cast<int>(data.velocity) << " → " 
              << static_cast<int>(velocity_val) << "\n";  // Добавлено
    std::cerr << "  Power: " << static_cast<int>(data.power) << " → " 
              << static_cast<int>(power_val) << "\n";  // Добавлено
    std::cerr << "  Acceleration: " << data.acceleration << " → " 
              << static_cast<int>(a_byte) << "\n";
    
    return packet;
}
TelemetryData unpack_data(uint64_t packet) {
    TelemetryData data;
    
    // 1. Y: биты 52-57
    uint8_t y_raw = (packet >> 52) & 0x3F;
    data.y = static_cast<int8_t>(y_raw) - 32;
    
    // 2. X: биты 46-51
    data.x = (packet >> 46) & 0x3F;
    
    // 3. State: биты 44-45
    data.state = (packet >> 44) & 0x03;
    
    // 4. Mode: биты 42-43
    data.mode = (packet >> 42) & 0x03;
    
    // 5. Velocity: биты 34-41
    data.velocity = (packet >> 34) & 0xFF;
    
    // 6. Power: биты 26-33
    data.power = (packet >> 26) & 0xFF;
    
    // 7. Acceleration: биты 18-25
    uint8_t a_byte = (packet >> 18) & 0xFF;
    data.acceleration = static_cast<float>(a_byte) / 10.0f - 12.7f;
    
    // 8. Reserve: биты 0-17
    data.reserved = packet & 0x3FFFF;
    
    return data;
}