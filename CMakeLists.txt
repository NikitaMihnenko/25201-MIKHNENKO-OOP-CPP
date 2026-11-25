
cmake_minimum_required(VERSION 3.16)

project(BitArrayProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(googletest)

add_executable(tests
        tests.cpp
        BitArray.cpp
)

target_link_libraries(tests
        PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   target_compile_options(tests PRIVATE --coverage)
   target_link_options(tests PRIVATE --coverage)
endif()

