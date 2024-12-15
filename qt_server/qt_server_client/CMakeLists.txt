cmake_minimum_required(VERSION 3.5)

project(qt_server_client)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

set(target qt_server_client)
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
qt_add_executable(${target} ${SOURCES})

# Линковка с необходимыми модулями Qt
target_link_libraries(${target} PRIVATE Qt6::Core Qt6::Network)