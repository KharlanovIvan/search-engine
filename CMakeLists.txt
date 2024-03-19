cmake_minimum_required(VERSION 3.17)

project(Search_Engine)

set(CMAKE_CXX_STANDARD 17)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

add_executable(Search_Engine search_engine.cpp)

target_link_libraries(Search_Engine PRIVATE nlohmann_json::nlohmann_json)