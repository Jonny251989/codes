#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char **argv) {
    std::cout << "=========================================" << std::endl;
    std::cout << "Running Functional Tests for Async TCP/UDP Server" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "Note: These tests require the server to be built and" << std::endl;
    std::cout << "will start/stop it automatically during testing." << std::endl;
    std::cout << "=========================================" << std::endl;
    
    ::testing::InitGoogleTest(&argc, argv);
    
    int result = RUN_ALL_TESTS();
    
    std::cout << "=========================================" << std::endl;
    std::cout << "Functional Tests Finished" << std::endl;
    std::cout << "=========================================" << std::endl;
    
    return result;
}