cmake_minimum_required(VERSION 3.10)

project(bmpimageproject LANGUAGES CXX)

set(target bmpimageproject)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${target} main.cpp bmpimage.cpp)

if(WIN32)
    target_link_libraries(${target} PRIVATE ws2_32)
endif()