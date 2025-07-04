#include <gtest/gtest.h>
#include <fstream>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>

class FunctionalTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Даем серверу время на запуск
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
};

std::string run_client_test(const std::string& test_data) {
    // Создаем уникальное имя для файла с тестовыми данными
    const std::string input_file = "/test_data/input_" + std::to_string(rand()) + ".txt";
    
    // Записываем тестовые данные в файл
    std::ofstream out(input_file);
    out << test_data;
    out.close();
    
    // Команда для запуска клиента с перенаправлением ввода
    const std::string command = 
        "/workspace/bin/telemetry_client telemetry_server 12345 < " + input_file + 
        " 2>&1";  // Перенаправляем stderr в stdout
    
    // Выполняем команду и захватываем вывод
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return "ERROR: Failed to run command";
    
    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe)) {
        result += buffer;
    }
    pclose(pipe);
    
    return result;
}

TEST_F(FunctionalTest, ValidData) {
    const std::string input = "25\n-5\n120\n1\n1\n0.0\n70\n";
    const std::string output = run_client_test(input);
    
    // Проверяем вывод клиента
    EXPECT_NE(output.find("Data is valid"), std::string::npos)
        << "Client output:\n" << output;
}

TEST_F(FunctionalTest, InvalidX) {
    const std::string input = "100\n-5\n120\n1\n1\n0.0\n70\n";
    const std::string output = run_client_test(input);
    
    EXPECT_NE(output.find("Data is invalid"), std::string::npos)
        << "Client output:\n" << output;
}

TEST_F(FunctionalTest, InvalidY) {
    const std::string input = "25\n-50\n120\n1\n1\n0.0\n70\n";
    const std::string output = run_client_test(input);
    
    EXPECT_NE(output.find("Data is invalid"), std::string::npos)
        << "Client output:\n" << output;
}

TEST_F(FunctionalTest, InvalidAcceleration) {
    const std::string input = "25\n-5\n120\n1\n1\n20.0\n70\n";
    const std::string output = run_client_test(input);
    
    EXPECT_NE(output.find("Data is invalid"), std::string::npos)
        << "Client output:\n" << output;
}