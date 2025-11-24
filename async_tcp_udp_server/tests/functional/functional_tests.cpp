#include <gtest/gtest.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <sys/wait.h>
#include <boost/asio.hpp>
#include "../test_utils.hpp"

class FunctionalTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Start server in background
        server_pid = start_server();
        TestUtils::wait_for_server("localhost", 8080);
    }

    void TearDown() override {
        stop_server();
    }

    pid_t start_server() {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process - start server
            execl("../../build/telemetry_server", "telemetry_server", "8080", nullptr);
            perror("execl");
            exit(1);
        }
        return pid;
    }

    void stop_server() {
        if (server_pid > 0) {
            kill(server_pid, SIGTERM);
            waitpid(server_pid, nullptr, 0);
        }
    }

    std::string run_tcp_test(const std::string& message) {
        std::string command = "../../build/tcp_client 127.0.0.1 8080 \"" + message + "\"";
        return TestUtils::execute_command(command);
    }

    std::string run_udp_test(const std::string& message) {
        std::string command = "../../build/udp_client 127.0.0.1 8080 \"" + message + "\"";
        return TestUtils::execute_command(command);
    }

private:
    pid_t server_pid;
};

TEST_F(FunctionalTest, TcpEcho) {
    const std::string message = "Hello TCP World";
    std::string response = run_tcp_test(message);
    EXPECT_EQ(response, message);
}

TEST_F(FunctionalTest, UdpEcho) {
    const std::string message = "Hello UDP World";
    std::string response = run_udp_test(message);
    EXPECT_EQ(response, message);
}

TEST_F(FunctionalTest, TcpTimeCommand) {
    std::string response = run_tcp_test("/time");
    EXPECT_GE(response.size(), 19);
    EXPECT_EQ(response[4], '-');
    EXPECT_EQ(response[7], '-');
}

TEST_F(FunctionalTest, UdpTimeCommand) {
    std::string response = run_udp_test("/time");
    EXPECT_GE(response.size(), 19);
    EXPECT_EQ(response[4], '-');
    EXPECT_EQ(response[7], '-');
}

TEST_F(FunctionalTest, TcpStatsCommand) {
    // First make some connections
    run_tcp_test("test1");
    run_tcp_test("test2");
    
    std::string response = run_tcp_test("/stats");
    EXPECT_NE(response.find("Total connections:"), std::string::npos);
    EXPECT_NE(response.find("Current connections:"), std::string::npos);
}

TEST_F(FunctionalTest, UdpStatsCommand) {
    // Make some UDP connections
    run_udp_test("test1");
    run_udp_test("test2");
    
    std::string response = run_udp_test("/stats");
    EXPECT_NE(response.find("Total connections:"), std::string::npos);
    EXPECT_NE(response.find("Current connections:"), std::string::npos);
}

TEST_F(FunctionalTest, TcpUnknownCommand) {
    std::string response = run_tcp_test("/unknown");
    EXPECT_EQ(response, "ERROR: Unknown command");
}

TEST_F(FunctionalTest, UdpUnknownCommand) {
    std::string response = run_udp_test("/unknown");
    EXPECT_EQ(response, "ERROR: Unknown command");
}

TEST_F(FunctionalTest, TcpShutdownCommand) {
    std::string response = run_tcp_test("/shutdown");
    EXPECT_EQ(response, "/SHUTDOWN_ACK");
    
    // Server should be shutting down, wait a bit
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TEST_F(FunctionalTest, MultipleTcpConnections) {
    // Test that server can handle multiple TCP connections
    std::string response1 = run_tcp_test("message1");
    std::string response2 = run_tcp_test("message2");
    std::string response3 = run_tcp_test("message3");
    
    EXPECT_EQ(response1, "message1");
    EXPECT_EQ(response2, "message2");
    EXPECT_EQ(response3, "message3");
}

TEST_F(FunctionalTest, MixedProtocols) {
    // Test mixing TCP and UDP
    std::string tcp_response = run_tcp_test("tcp_message");
    std::string udp_response = run_udp_test("udp_message");
    
    EXPECT_EQ(tcp_response, "tcp_message");
    EXPECT_EQ(udp_response, "udp_message");
}