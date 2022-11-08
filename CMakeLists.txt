cmake_minimum_required(VERSION 3.10)

project(syssoft1 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(GTest REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

# Main Target
add_executable(
    ssoft
    src/main.cpp
    src/print.hpp src/print.cpp
)

# Test Target
add_executable(
    tests
    tests/alg.cpp
    src/print.cpp
)

target_include_directories(
    tests PRIVATE 
    ${GTEST_INCLUDE_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(ssoft PRIVATE Qt5::Widgets)
target_link_libraries(tests PRIVATE ${GTEST_BOTH_LIBRARIES})

target_compile_options(
    ssoft PRIVATE
    -Wall -Wextra -pedantic -Werror
)

target_compile_options(
    tests PRIVATE
    -Wall -Wextra -pedantic -Werror
)