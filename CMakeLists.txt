cmake_minimum_required(VERSION 3.10)

project(word_count LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(word_count_lib STATIC FileReader.cpp LineParser.cpp WordStatistics.cpp CsvWriter.cpp WordCountController.cpp)
target_include_directories(word_count_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(word_count main.cpp)
target_link_libraries(word_count PRIVATE word_count_lib)

enable_testing()
add_subdirectory(googletest)

add_executable(run_tests tests/tests.cpp)
target_link_libraries(run_tests PRIVATE word_count_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)