cmake_minimum_required(VERSION 3.10)
project(25201_MIKHNENKO_OOP_CPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(googletest)

add_executable(main
        main.cpp
        ConvertToCSV.cpp
)

enable_testing()

add_executable(tests
        tests/tests.cpp
        ConvertToCSV.cpp
)

target_link_libraries(tests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)