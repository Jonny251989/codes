CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -O2 -Icommon -Iserver -Itests
LDFLAGS = -pthread

# Directories
COMMON_DIR = common
SERVER_DIR = server
CLIENT_DIR = client
TEST_DIR = tests
BUILD_DIR = build

# Source files
COMMON_SOURCES = $(COMMON_DIR)/protocol.cpp $(COMMON_DIR)/network_utils.cpp
SERVER_SOURCES = $(SERVER_DIR)/main.cpp $(SERVER_DIR)/server.cpp \
                 $(SERVER_DIR)/tcp_handler.cpp $(SERVER_DIR)/udp_handler.cpp \
                 $(SERVER_DIR)/tcp_connection.cpp $(SERVER_DIR)/command_processor.cpp \
                 $(SERVER_DIR)/session_manager.cpp
CLIENT_SOURCES = $(CLIENT_DIR)/tcp_client.cpp $(CLIENT_DIR)/udp_client.cpp

# Test source files
UNIT_TEST_SOURCES = $(TEST_DIR)/unit/unit_tests.cpp $(TEST_DIR)/unit/main.cpp
FUNCTIONAL_TEST_SOURCES = $(TEST_DIR)/functional/functional_tests.cpp $(TEST_DIR)/functional/main.cpp
TEST_UTILS_SOURCES = $(TEST_DIR)/test_utils.cpp

# Object files
COMMON_OBJS = $(patsubst $(COMMON_DIR)/%.cpp,$(BUILD_DIR)/common/%.o,$(COMMON_SOURCES))
SERVER_OBJS = $(patsubst $(SERVER_DIR)/%.cpp,$(BUILD_DIR)/server/%.o,$(SERVER_SOURCES))
CLIENT_OBJS = $(patsubst $(CLIENT_DIR)/%.cpp,$(BUILD_DIR)/client/%.o,$(CLIENT_SOURCES))

# Test object files
UNIT_TEST_OBJS = $(BUILD_DIR)/tests/unit/unit_tests.o $(BUILD_DIR)/tests/unit/main.o
FUNCTIONAL_TEST_OBJS = $(BUILD_DIR)/tests/functional/functional_tests.o $(BUILD_DIR)/tests/functional/main.o
TEST_UTILS_OBJS = $(BUILD_DIR)/tests/test_utils.o

# Targets
TARGET_SERVER = $(BUILD_DIR)/telemetry_server
TARGET_TCP_CLIENT = $(BUILD_DIR)/tcp_client
TARGET_UDP_CLIENT = $(BUILD_DIR)/udp_client
TARGET_UNIT_TESTS = $(BUILD_DIR)/unit_tests
TARGET_FUNCTIONAL_TESTS = $(BUILD_DIR)/functional_tests

# Test libraries
GTEST_LIBS = -lgtest -lpthread
BOOST_LIBS = -lboost_system

.PHONY: all clean tests unit_tests functional_tests run_tests install_test_deps

all: $(TARGET_SERVER) $(TARGET_TCP_CLIENT) $(TARGET_UDP_CLIENT)

$(TARGET_SERVER): $(COMMON_OBJS) $(SERVER_OBJS)
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(LDFLAGS)

$(TARGET_TCP_CLIENT): $(BUILD_DIR)/client/tcp_client.o
	@mkdir -p $(@D)
	$(CXX) $^ -o $@

$(TARGET_UDP_CLIENT): $(BUILD_DIR)/client/udp_client.o
	@mkdir -p $(@D)
	$(CXX) $^ -o $@

$(BUILD_DIR)/common/%.o: $(COMMON_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/server/%.o: $(SERVER_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/client/%.o: $(CLIENT_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test targets
tests: unit_tests functional_tests

unit_tests: $(TARGET_UNIT_TESTS)

$(TARGET_UNIT_TESTS): $(UNIT_TEST_OBJS) $(COMMON_OBJS) $(filter-out $(BUILD_DIR)/server/main.o, $(SERVER_OBJS))
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(GTEST_LIBS) $(LDFLAGS)

functional_tests: $(TARGET_FUNCTIONAL_TESTS)

$(TARGET_FUNCTIONAL_TESTS): $(FUNCTIONAL_TEST_OBJS) $(TEST_UTILS_OBJS)
	@mkdir -p $(@D)
	$(CXX) $^ -o $@ $(GTEST_LIBS) $(BOOST_LIBS) $(LDFLAGS)

$(BUILD_DIR)/tests/unit/%.o: $(TEST_DIR)/unit/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/functional/%.o: $(TEST_DIR)/functional/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/tests/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run tests
run_tests: tests
	@echo "========================================="
	@echo "Running All Tests"
	@echo "========================================="
	@echo "Running unit tests..."
	@./$(TARGET_UNIT_TESTS)
	@echo ""
	@echo "Running functional tests..."
	@./$(TARGET_FUNCTIONAL_TESTS)

run_unit_tests: unit_tests
	@./$(TARGET_UNIT_TESTS)

run_functional_tests: functional_tests
	@./$(TARGET_FUNCTIONAL_TESTS)

# Test with specific filters
test_echo: unit_tests
	@./$(TARGET_UNIT_TESTS) --gtest_filter="*Echo*"

test_commands: unit_tests
	@./$(TARGET_UNIT_TESTS) --gtest_filter="*Command*"

test_session: unit_tests
	@./$(TARGET_UNIT_TESTS) --gtest_filter="*Session*"

# Clean
clean:
	rm -rf $(BUILD_DIR)

clean_tests:
	rm -rf $(BUILD_DIR)/tests

# Dependencies installation
install_test_deps:
	@echo "Installing test dependencies..."
	sudo apt-get update
	sudo apt-get install -y libgtest-dev libboost-system-dev cmake
	@echo "Building GTest..."
	cd /usr/src/gtest && sudo cmake CMakeLists.txt && sudo make && sudo cp lib/*.a /usr/lib
	@echo "Making test scripts executable..."
	chmod +x $(TEST_DIR)/functional/run_client.sh
	@echo "Test dependencies installed successfully!"

# Create test data directory and files
setup_test_data:
	@mkdir -p $(TEST_DIR)/functional/test_data
	@echo "Hello TCP Test Message" > $(TEST_DIR)/functional/test_data/echo_tcp_input.txt
	@echo "Hello UDP Test Message" > $(TEST_DIR)/functional/test_data/echo_udp_input.txt
	@echo "/time" > $(TEST_DIR)/functional/test_data/commands_tcp_input.txt
	@echo "/time" > $(TEST_DIR)/functional/test_data/commands_udp_input.txt
	@echo "/stats" > $(TEST_DIR)/functional/test_data/stats_input.txt
	@echo "Test data setup complete!"

# Full setup (dependencies + test data)
setup_tests: install_test_deps setup_test_data
	@echo "Test environment setup complete!"

# Help
help:
	@echo "Available targets:"
	@echo "  all              - Build server and clients"
	@echo "  tests            - Build all tests"
	@echo "  unit_tests       - Build unit tests"
	@echo "  functional_tests - Build functional tests"
	@echo "  run_tests        - Build and run all tests"
	@echo "  run_unit_tests   - Build and run unit tests"
	@echo "  run_functional_tests - Build and run functional tests"
	@echo "  test_echo        - Run only echo-related tests"
	@echo "  test_commands    - Run only command-related tests"
	@echo "  test_session     - Run only session-related tests"
	@echo "  install_test_deps - Install test dependencies"
	@echo "  setup_test_data  - Create test data files"
	@echo "  setup_tests      - Complete test environment setup"
	@echo "  clean            - Remove all build files"
	@echo "  clean_tests      - Remove test build files"

# Debug info
debug:
	@echo "Common objects: $(COMMON_OBJS)"
	@echo "Server objects: $(SERVER_OBJS)"
	@echo "Unit test objects: $(UNIT_TEST_OBJS)"
	@echo "Functional test objects: $(FUNCTIONAL_TEST_OBJS)"