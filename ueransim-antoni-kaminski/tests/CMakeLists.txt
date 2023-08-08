cmake_minimum_required(VERSION 3.17)
project(ueransim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(src)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(
    gnb_tests
    gnb_tests.cc
)
target_link_libraries(
    gnb_tests
    PRIVATE
    GTest::GTest # GTest::gtest_main
    ext
    utils
)
target_include_directories(gnb_tests PUBLIC src src/config)

add_test(gnb_gtests gnb_tests)

# include(GoogleTest)
# gtest_discover_tests(gnb_tests)