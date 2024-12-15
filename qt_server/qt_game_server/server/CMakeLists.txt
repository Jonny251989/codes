cmake_minimum_required(VERSION 3.5)

project(qt_game_server)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем исходные файлы
set(SOURCES
    server.cpp
    main.cpp
)

# Для правильной работы с Qt и Q_OBJECT, используем специальную команду для moc
qt_add_executable(qt_game_server ${SOURCES})

# Линковка с необходимыми модулями Qt
target_link_libraries(qt_game_server PRIVATE Qt6::Core Qt6::Network)
