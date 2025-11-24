CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread -O2 -Icommon -Iserver
LDFLAGS = -pthread

# Directories
COMMON_DIR = common
SERVER_DIR = server
CLIENT_DIR = client
BUILD_DIR = build

# Source files
COMMON_SOURCES = $(COMMON_DIR)/protocol.cpp $(COMMON_DIR)/network_utils.cpp
SERVER_SOURCES = $(SERVER_DIR)/main.cpp $(SERVER_DIR)/server.cpp \
                 $(SERVER_DIR)/tcp_handler.cpp $(SERVER_DIR)/udp_handler.cpp \
                 $(SERVER_DIR)/tcp_connection.cpp $(SERVER_DIR)/command_processor.cpp \
                 $(SERVER_DIR)/session_manager.cpp
CLIENT_SOURCES = $(CLIENT_DIR)/tcp_client.cpp $(CLIENT_DIR)/udp_client.cpp

# Object files
COMMON_OBJS = $(patsubst $(COMMON_DIR)/%.cpp,$(BUILD_DIR)/common/%.o,$(COMMON_SOURCES))
SERVER_OBJS = $(patsubst $(SERVER_DIR)/%.cpp,$(BUILD_DIR)/server/%.o,$(SERVER_SOURCES))
CLIENT_OBJS = $(patsubst $(CLIENT_DIR)/%.cpp,$(BUILD_DIR)/client/%.o,$(CLIENT_SOURCES))

TARGET_SERVER = $(BUILD_DIR)/telemetry_server
TARGET_TCP_CLIENT = $(BUILD_DIR)/tcp_client
TARGET_UDP_CLIENT = $(BUILD_DIR)/udp_client

.PHONY: all clean test

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

clean:
	rm -rf $(BUILD_DIR)

test: $(TARGET_SERVER) $(TARGET_TCP_CLIENT) $(TARGET_UDP_CLIENT)
	@echo "Starting server in background..."
	@./$(TARGET_SERVER) 8080 &
	@SERVER_PID=$$!; \
	sleep 2; \
	echo "Testing TCP client..."; \
	./$(TARGET_TCP_CLIENT) 127.0.0.1 8080 "Hello World"; \
	echo "Testing UDP client..."; \
	./$(TARGET_UDP_CLIENT) 127.0.0.1 8080 "Hello UDP"; \
	echo "Testing commands..."; \
	./$(TARGET_TCP_CLIENT) 127.0.0.1 8080 "/time"; \
	./$(TARGET_TCP_CLIENT) 127.0.0.1 8080 "/stats"; \
	echo "Stopping server..."; \
	kill $$SERVER_PID; \
	wait $$SERVER_PID 2>/dev/null