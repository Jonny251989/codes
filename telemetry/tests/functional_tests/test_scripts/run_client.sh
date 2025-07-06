#!/bin/bash

TEST_NAME=$1
export LC_ALL=C
INPUT_FILE="/test_data/${TEST_NAME}_input.txt"
OUTPUT_FILE="/test_data/${TEST_NAME}_output.txt"

echo "Starting test: $TEST_NAME"
echo "Input file: $INPUT_FILE"

# Проверяем существование входного файла
if [ ! -f "$INPUT_FILE" ]; then
    echo "Error: Input file $INPUT_FILE not found!" >&2
    exit 1
fi

# Запускаем клиент с таймаутом и подачей входных данных
timeout 10s /app/telemetry_client telemetry_server 12345 < "$INPUT_FILE" > "$OUTPUT_FILE" 2>&1
EXIT_CODE=$?

echo "Client output:"
cat "$OUTPUT_FILE"

exit $EXIT_CODE