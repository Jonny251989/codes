#!/bin/bash
set -e

# Создаем директорию сборки
mkdir -p /workspace/build
cd /workspace/build

# Конфигурация проекта
cmake -DCMAKE_BUILD_TYPE=Release /workspace/src

# Сборка
make -j$(nproc)

# Копирование артефактов
mkdir -p /workspace/bin
cp server/telemetry_server /workspace/bin/
cp client/telemetry_client /workspace/bin/
cp tests/unit_tests /workspace/bin/

echo "Build completed successfully!"