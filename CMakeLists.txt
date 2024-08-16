cmake_minimum_required(VERSION 3.17)
project(final)

# Установите стандарт C++
set(CMAKE_CXX_STANDARD 14)

# Отключите сборку тестов для библиотеки nlohmann_json
set(JSON_BuildTests OFF CACHE INTERNAL "")

# Добавьте поддиректорию с библиотекой nlohmann_json
add_subdirectory(nlohmann_json)

# Укажите исходные файлы вашего проекта
add_executable(final main.cpp)

# Свяжите ваше приложение с библиотекой nlohmann_json
target_link_libraries(final PRIVATE nlohmann_json::nlohmann_json)
