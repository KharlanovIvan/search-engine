# Указать, что исходные файлы в этом каталоге будут собраны в исполняемый файл
add_executable(search_engine main.cpp)

# Связать приложение с библиотекой nlohmann_json
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

# Указать, что в проекте будут применяться тесты
enable_testing()

# Создать отдельное приложение для тестов
add_executable(tests test.cpp)

# Связать тесты с Google Test
target_link_libraries(tests PRIVATE gtest_main)

# Автоматическое обнаружение тестов
include(GoogleTest)
gtest_discover_tests(tests)
