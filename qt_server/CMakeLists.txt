cmake_minimum_required(VERSION 3.5)

project(Qt_server)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем исходные файлы
set(SOURCES
    client.cpp
    server.cpp
    main.cpp
)

# Для правильной работы с Qt и Q_OBJECT, используем специальную команду для moc
qt_add_executable(Qt_server ${SOURCES})

# Линковка с необходимыми модулями Qt
target_link_libraries(Qt_server PRIVATE Qt6::Core Qt6::Network)