cmake_minimum_required(VERSION 3.14)
project(ToxicityClassifierClient)

set(target ToxicityClassifierClient)

# Настройка путей для Protobuf и gRPC
list(APPEND CMAKE_PREFIX_PATH "/usr/local/lib/cmake/protobuf")
set(Protobuf_PROTOC_EXECUTABLE /usr/local/bin/protoc)

# Поиск зависимостей
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)
find_package(utf8_range CONFIG REQUIRED)

# Источники и заголовки
set(PROTO_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.pb.cc)
set(PROTO_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.pb.h)
set(GRPC_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.grpc.pb.cc)
set(GRPC_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/toxicity_classifier.grpc.pb.h)

# Создаем статическую библиотеку
add_library(${target} STATIC client.cpp ${PROTO_SRCS} ${GRPC_SRCS})

# Указываем заголовочные файлы
target_include_directories(${target}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  # Для сборки
    $<INSTALL_INTERFACE:include>                   # Для установки
)

# Линкуем зависимости
target_link_libraries(${target}
    PUBLIC
    absl::cord
    absl::cord_internal
    absl::base
    absl::strings
    absl::log_internal_check_op
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
    utf8_range::utf8_validity
)

# Генерация файлов конфигурации для find_package
include(CMakePackageConfigHelpers)

# Создаем файл конфигурации
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/ToxicityClassifierClientConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ToxicityClassifierClientConfig.cmake
    INSTALL_DESTINATION lib/cmake/ToxicityClassifierClient
)

# Создаем файл версии
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ToxicityClassifierClientConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

# Устанавливаем библиотеку и заголовочные файлы
install(
    TARGETS ${target}
    EXPORT ToxicityClassifierClientTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Устанавливаем экспортированные цели
install(
    EXPORT ToxicityClassifierClientTargets
    FILE ToxicityClassifierClientTargets.cmake
    NAMESPACE ToxicityClassifierClient::
    DESTINATION lib/cmake/ToxicityClassifierClient
)

# Устанавливаем файлы конфигурации
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ToxicityClassifierClientConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ToxicityClassifierClientConfigVersion.cmake
    DESTINATION lib/cmake/ToxicityClassifierClient
)

# Устанавливаем заголовочные файлы
install(FILES client.hpp DESTINATION include)
install(FILES ${PROTO_HDRS} ${GRPC_HDRS} DESTINATION include)