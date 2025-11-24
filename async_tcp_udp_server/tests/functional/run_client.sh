#!/bin/bash

TEST_NAME=$1
SERVER_HOST=${2:-"127.0.0.1"}
SERVER_PORT=${3:-"8080"}
PROTOCOL=${4:-"tcp"}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
INPUT_FILE="${SCRIPT_DIR}/test_data/${TEST_NAME}_input.txt"
OUTPUT_FILE="${SCRIPT_DIR}/test_data/${TEST_NAME}_output.txt"

mkdir -p "$(dirname "$OUTPUT_FILE")"

if [[ -f "$INPUT_FILE" ]]; then
    INPUT_CONTENT=$(cat "$INPUT_FILE")
else
    case $TEST_NAME in
        "echo_tcp")
            INPUT_CONTENT="Hello TCP World"
            ;;
        "echo_udp")
            INPUT_CONTENT="Hello UDP World"
            ;;
        "time_command")
            INPUT_CONTENT="/time"
            ;;
        "stats_command")
            INPUT_CONTENT="/stats"
            ;;
        "shutdown_command")
            INPUT_CONTENT="/shutdown"
            ;;
        *)
            INPUT_CONTENT="test"
            ;;
    esac
fi

if [[ "$PROTOCOL" == "udp" ]]; then
    CLIENT_APP="${SCRIPT_DIR}/../../build/udp_client"
else
    CLIENT_APP="${SCRIPT_DIR}/../../build/tcp_client"
fi

echo "Running test: $TEST_NAME"
echo "Input: $INPUT_CONTENT"
echo "Protocol: $PROTOCOL"

"$CLIENT_APP" "$SERVER_HOST" "$SERVER_PORT" "$INPUT_CONTENT" > "$OUTPUT_FILE" 2>&1
EXIT_CODE=$?

echo "Output saved to: $OUTPUT_FILE"
echo "Exit code: $EXIT_CODE"

cat "$OUTPUT_FILE"
exit $EXIT_CODE