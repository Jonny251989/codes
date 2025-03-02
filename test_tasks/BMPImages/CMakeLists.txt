cmake_minimum_required(VERSION 3.10)

# Указываем название проекта и язык
project(BMPImageProject LANGUAGES CXX)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Добавляем исполняемый файл
add_executable(BMPImageApp main.cpp BMPImage.cpp)

# Подключаем Windows API (требуется для работы с BMP-файлами)
if(WIN32)
    target_link_libraries(BMPImageApp PRIVATE ws2_32)
endif()