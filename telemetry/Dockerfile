FROM ubuntu:22.04

# Установка зависимостей
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    build-essential \
    cmake \
    libboost-system-dev \
    libgtest-dev \
    google-mock \
    libgmock-dev \
    git \
    qtbase5-dev \
    qt5-qmake \
    qtdeclarative5-dev \
    qttools5-dev \
    qttools5-dev-tools \
    libqt5charts5-dev \
    xvfb \
    nlohmann-json3-dev \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Установка conan для управления зависимостями (опционально)
RUN pip3 install conan

# Создание рабочей директории
WORKDIR /workspace

# Копирование скриптов сборки
COPY build.sh /usr/local/bin/build.sh
RUN chmod +x /usr/local/bin/build.sh

# Установка gtest
RUN cd /usr/src/googletest && \
    cmake CMakeLists.txt && \
    make && \
    find . -name '*.a' -exec cp {} /usr/lib \;