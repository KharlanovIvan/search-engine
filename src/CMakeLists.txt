# Список исходных файлов для основного исполняемого файла
set(SOURCE_FILES
    main.cpp
    ConverterJSON.cpp
    InvertedIndex.cpp
    ThreadPool.cpp
)

# Создаем исполняемый файл
add_executable(search_engine ${SOURCE_FILES})

# Указываем путь к заголовочным файлам
target_include_directories(search_engine PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Связываем с необходимыми библиотеками
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

# Указываем, что в проекте будут применяться тесты
enable_testing()

# Добавляем тесты
add_executable(tests test.cpp)

target_link_libraries(tests PRIVATE gtest_main)

# Автоматическое обнаружение тестов
include(GoogleTest)
gtest_discover_tests(tests)
