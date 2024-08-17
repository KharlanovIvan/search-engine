cmake_minimum_required(VERSION 3.17)

project(search_engine)

set(CMAKE_CXX_STANDARD 14)

# Отключить сборку тестов для библиотеки nlohmann_json
set(JSON_BuildTests OFF CACHE INTERNAL "")

# Добавить поддиректорию с библиотекой nlohmann_json
add_subdirectory(nlohmann_json)

# Указать исходные файлы проекта
add_executable(search_engine main.cpp)

# Связать приложение с библиотекой nlohmann_json
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

# Подключение Google Test через FetchContent
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE # При установке этой опции в TRUE, CMake будет устанавливать метку времени на извлеченные файлы, соответствующую времени их извлечения. 
                                    # Это помогает избежать проблем с кэшированием: если URL содержимого изменится, извлеченные файлы будут обновлены, 
                                    # и сборка будет пересобрана при необходимости, поскольку время изменения файлов будет соответствовать времени их извлечения.
                                    # Без DOWNLOAD_EXTRACT_TIMESTAMP или если установлено в FALSE: По умолчанию, CMake использует старое поведение, при котором метки времени 
                                    # на извлеченных файлах сохраняются как в исходных архивах. Это может привести к проблемам с повторным сбором, если содержимое URL изменится, но метка времени останется прежн 
                                    # Почему это важно? Если ваш проект зависит от файлов, которые загружаются и извлекаются через FetchContent, установка DOWNLOAD_EXTRACT_TIMESTAMP TRUE 
                                    # помогает гарантировать, что изменения в этих файлах приводят к пересборке проекта. Это особенно важно, если вы используете сторонние зависимости, 
                                    # которые могут обновляться со временем, и вам нужно убедиться, что ваш проект использует их актуальную версию.
)

# Убедитесь, что gtest и gmock доступны
FetchContent_MakeAvailable(googletest)

# Если вы используете компилятор MinGW:
if(MINGW)
    set(gtest_disable_pthreads ON)
endif()

# Динамическое подключение библиотеки к проекту:
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Указать, что в проекте будут применяться тесты
enable_testing()

# Создать отдельное приложение для тестов
add_executable(tests test.cpp)

# Связать тесты с Google Test
target_link_libraries(tests PRIVATE gtest_main)

# Автоматическое обнаружение тестов
include(GoogleTest)
gtest_discover_tests(tests)
