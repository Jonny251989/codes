#include <gtest/gtest.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <sys/wait.h>
#include <boost/asio.hpp>

class FunctionalTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Start server in background
        server_pid = start_server();
        wait_for_server("localhost", 8080);
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

    void wait_for_server(const std::string& host, uint16_t port) {
        using namespace boost::asio;
        using ip::tcp;
        
        io_context io;
        tcp::resolver resolver(io);
        
        for (int i = 0; i < 30; ++i) {
            try {
                tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
                tcp::socket socket(io);
                connect(socket, endpoints);
                return;
            } catch (const boost::system::system_error& e) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        throw std::runtime_error("Server not available after 30 attempts");
    }

    std::string run_tcp_test(const std::string& message) {
        std::string command = "../../build/tcp_client 127.0.0.1 8080 \"" + message + "\"";
        return execute_command(command);
    }

    std::string run_udp_test(const std::string& message) {
        std::string command = "../../build/udp_client 127.0.0.1 8080 \"" + message + "\"";
        return execute_command(command);
    }

    std::string execute_command(const std::string& command) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
        
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        
        if (!result.empty() && result[result.length()-1] == '\n') {
            result.erase(result.length()-1);
        }
        
        return result;
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
    run_tcp_test("test1");
    run_tcp_test("test2");
    
    std::string response = run_tcp_test("/stats");
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