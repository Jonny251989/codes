#include <gtest/gtest.h>
#include "protocol.hpp"

TEST(TelemetryProtocolTest, FullCyclePackingUnpacking) {
    TelemetryData original{
        31,         // x
        -10,        // y
        150,        // velocity
        2,          // mode
        1,          // state
        5.5f,       // acceleration
        90,         // power
        0xABCD      // reserved
    };

    uint64_t packed = pack_data(original);
    TelemetryData unpacked = unpack_data(packed);

    EXPECT_EQ(original.x, unpacked.x);
    EXPECT_EQ(original.y, unpacked.y);
    EXPECT_EQ(original.velocity, unpacked.velocity);
    EXPECT_EQ(original.mode, unpacked.mode);
    EXPECT_EQ(original.state, unpacked.state);
    EXPECT_FLOAT_EQ(original.acceleration, unpacked.acceleration);
    EXPECT_EQ(original.power, unpacked.power);
    EXPECT_EQ(original.reserved, unpacked.reserved);
}

TEST(TelemetryProtocolTest, EdgeValues) {
    TelemetryData edge{
        0,          // min x
        -32,        // min y
        0,          // min velocity
        0,          // min mode
        0,          // min state
        -12.7f,     // min acceleration
        0,          // min power
        0xFFFF      // max reserved
    };

    uint64_t packed = pack_data(edge);
    TelemetryData unpacked = unpack_data(packed);

    EXPECT_EQ(edge.x, unpacked.x);
    EXPECT_EQ(edge.y, unpacked.y);
    EXPECT_EQ(edge.velocity, unpacked.velocity);
    EXPECT_EQ(edge.mode, unpacked.mode);
    EXPECT_EQ(edge.state, unpacked.state);
    EXPECT_NEAR(edge.acceleration, unpacked.acceleration, 0.05f);
    EXPECT_EQ(edge.power, unpacked.power);
    EXPECT_EQ(edge.reserved, unpacked.reserved);
}

TEST(TelemetryProtocolTest, OverflowProtection) {
    TelemetryData overflow{};
    overflow.x = 100;        // >63
    overflow.y = -50;        // < -32
    overflow.velocity = 300; // >255
    overflow.mode = 5;       // >3
    overflow.state = 4;      // >3
    overflow.acceleration = 20.0f; // >12.8
    overflow.power = 200;    // >130
    overflow.reserved = 0x12345; // >0xFFFF

    uint64_t packed = pack_data(overflow);
    TelemetryData unpacked = unpack_data(packed);

    EXPECT_LE(unpacked.x, 63);
    EXPECT_GE(unpacked.y, -32);
    EXPECT_LE(unpacked.velocity, 255);
    EXPECT_LE(unpacked.mode, 3);
    EXPECT_LE(unpacked.state, 3);
    EXPECT_LE(unpacked.acceleration, 12.8f);
    EXPECT_EQ(unpacked.power, 130);  // Изменено с LE на EQ
    EXPECT_EQ(unpacked.reserved, 0x2345);
}