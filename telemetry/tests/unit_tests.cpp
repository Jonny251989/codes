#include <gtest/gtest.h>
#include "protocol.hpp"


TEST(TelemetryProtocolTest, FullCyclePackingUnpacking) {
    TelemetryData original{};
    original.x = 31;
    original.y = 22;         // -10 + 32 = 22
    original.velocity = 150;
    original.mode = 2;
    original.state = 1;
    original.acceleration = 182; // (5.5f + 12.7f) * 10 = 182
    original.power = 90;
    original.reserved = 0xABCD;

    uint64_t packed = pack_data(original);
    TelemetryData unpacked = unpack_data(packed);

    EXPECT_EQ(original.x, unpacked.x);
    EXPECT_EQ(original.y, unpacked.y);
    EXPECT_EQ(original.velocity, unpacked.velocity);
    EXPECT_EQ(original.mode, unpacked.mode);
    EXPECT_EQ(original.state, unpacked.state);
    EXPECT_EQ(original.acceleration, unpacked.acceleration);
    EXPECT_EQ(original.power, unpacked.power);
    EXPECT_EQ(original.reserved, unpacked.reserved);
}

TEST(TelemetryProtocolTest, EdgeValues) {
    TelemetryData edge{};
    edge.x = 0;
    edge.y = 0;              // -32 + 32 = 0
    edge.velocity = 0;
    edge.mode = 0;
    edge.state = 0;
    edge.acceleration = 0;   // (-12.7f + 12.7f) * 10 = 0
    edge.power = 0;
    edge.reserved = 0x3FFFF; // Максимальное 18-битное значение

    uint64_t packed = pack_data(edge);
    TelemetryData unpacked = unpack_data(packed);

    EXPECT_EQ(edge.x, unpacked.x);
    EXPECT_EQ(edge.y, unpacked.y);
    EXPECT_EQ(edge.velocity, unpacked.velocity);
    EXPECT_EQ(edge.mode, unpacked.mode);
    EXPECT_EQ(edge.state, unpacked.state);
    EXPECT_EQ(edge.acceleration, unpacked.acceleration);
    EXPECT_EQ(edge.power, unpacked.power);
    EXPECT_EQ(edge.reserved, unpacked.reserved);
}

TEST(TelemetryProtocolTest, OverflowProtection) {
    TelemetryData overflow{};

    // Присваиваем значения, выходящие за пределы битовых полей
    overflow.x = 100;        // >63 -> ожидаем 100 % 64 = 36
    overflow.y = 100;        // >63 -> ожидаем 100 % 64 = 36
    overflow.velocity = 300; // >255 -> ожидаем 300 % 256 = 44
    overflow.mode = 5;       // >3 -> ожидаем 5 % 4 = 1
    overflow.state = 4;      // >3 -> ожидаем 4 % 4 = 0
    overflow.acceleration = 300; // >255 -> ожидаем 300 % 256 = 44
    overflow.power = 200;    // 200 < 255 -> без изменений
    overflow.reserved = 0x123456; // >0x3FFFF -> ожидаем 0x123456 % 0x40000 = 0x3456

    uint64_t packed = pack_data(overflow);
    TelemetryData unpacked = unpack_data(packed);

    EXPECT_EQ(36, unpacked.x);
    EXPECT_EQ(36, unpacked.y);
    EXPECT_EQ(44, unpacked.velocity);
    EXPECT_EQ(1, unpacked.mode);
    EXPECT_EQ(0, unpacked.state);
    EXPECT_EQ(44, unpacked.acceleration);
    EXPECT_EQ(200, unpacked.power);
    EXPECT_EQ(0x23456, unpacked.reserved);
}

TEST(TelemetryProtocolTest, ReservedField) {
    TelemetryData data{};
    
    // Максимальное значение (18 бит)
    data.reserved = 0x3FFFF;
    uint64_t packed = pack_data(data);
    TelemetryData unpacked = unpack_data(packed);
    EXPECT_EQ(0x3FFFF, unpacked.reserved);
    
    // Значение больше максимального
    data.reserved = 0x4FFFF;
    packed = pack_data(data);
    unpacked = unpack_data(packed);
    EXPECT_EQ(0x0FFFF, unpacked.reserved);  // 0x4FFFF & 0x3FFFF = 0x0FFFF
}

TEST(TelemetryProtocolTest, NegativeY) {
    TelemetryData data{};
    
    data.y = 0;  // -32
    uint64_t packed = pack_data(data);
    TelemetryData unpacked = unpack_data(packed);
    EXPECT_EQ(0, unpacked.y);
    
    data.y = 63; // 31
    packed = pack_data(data);
    unpacked = unpack_data(packed);
    EXPECT_EQ(63, unpacked.y);
    
    data.y = 32; // 0
    packed = pack_data(data);
    unpacked = unpack_data(packed);
    EXPECT_EQ(32, unpacked.y);
}