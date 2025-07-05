#!/bin/bash
set -e

TEST_NAME=$1
INPUT_FILE="/test_data/${TEST_NAME}_input.txt"
OUTPUT_FILE="/test_data/${TEST_NAME}_output.txt"

echo "Starting test: $TEST_NAME"
echo "Input file: $INPUT_FILE"

# Создаем выходной файл, если не существует
touch "$OUTPUT_FILE"

# Запускаем клиент
timeout 10s /app/telemetry_client telemetry_server 12345 < "$INPUT_FILE" > "$OUTPUT_FILE" 2>&1
EXIT_CODE=$?

if [ $EXIT_CODE -eq 124 ]; then
    echo "Client timed out after 10 seconds"
elif [ $EXIT_CODE -ne 0 ]; then
    echo "Client exited with error: $EXIT_CODE"
fi

echo "Test completed. Exit code: $EXIT_CODE"
exit $EXIT_CODE