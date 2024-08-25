# Устанавливаем минимальную версию CMake и название проекта
cmake_minimum_required(VERSION 3.17)

# Устанавливаем стандарт C++ для проекта
set(CMAKE_CXX_STANDARD 17)

# Список исходных файлов для основного исполняемого файла
set(SOURCE_FILES
    main.cpp
    ConverterJSON.cpp
    InvertedIndex.cpp
    ThreadPool.cpp
    SearchServer.cpp
)

# Создаем исполняемый файл для основного приложения
add_executable(search_engine ${SOURCE_FILES})

# Указываем путь к заголовочным файлам для основного приложения
target_include_directories(search_engine PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Связываем основное приложение с необходимыми библиотеками
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

# Указываем, что в проекте будут применяться тесты
enable_testing()

# Список исходных файлов для исполняемого файла тестов
set(TEST_SOURCES
    test.cpp
    ConverterJSON.cpp
    InvertedIndex.cpp
    SearchServer.cpp
    ThreadPool.cpp
)

# Создаем исполняемый файл для тестов, добавляя исходные файлы тестов и основные файлы
# Основные файлы здесь нужны для возможности тестирования отдельных компонентов
add_executable(tests test.cpp ${TEST_SOURCES})

# Указываем путь к заголовочным файлам для тестов
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Связываем тесты с библиотеками Google Test и нереализуемыми компонентами
target_link_libraries(tests PRIVATE gtest_main)

# Связываем тесты с необходимыми библиотеками
target_link_libraries(tests PRIVATE gtest_main nlohmann_json::nlohmann_json)

# Автоматическое обнаружение тестов и их выполнение с помощью Google Test
include(GoogleTest)
gtest_discover_tests(tests)
