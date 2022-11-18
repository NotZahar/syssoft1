cmake_minimum_required(VERSION 3.10)

project(syssoft1 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
find_package(GTest REQUIRED)

# Main Target
add_executable(
    ssoft1
    src/main.cpp
    src/algorithm/translator.hpp src/algorithm/translator.cpp 
    src/controllers/mainWindowC.hpp src/controllers/mainWindowC.cpp
    src/middleware/validate.hpp src/middleware/validate.cpp
    src/errors/error.hpp src/errors/error.cpp
    src/UI/mainWindow.hpp src/UI/mainWindow.cpp
)

# Test Target
add_executable(
    tests
    tests/alg.cpp
)

target_include_directories(
    tests PRIVATE 
    ${GTEST_INCLUDE_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/middleware"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/algorithm")

target_link_libraries(ssoft1 PRIVATE Qt5::Widgets)
target_link_libraries(tests PRIVATE ${GTEST_BOTH_LIBRARIES} Qt5::Widgets)

target_compile_options(
    ssoft1 PRIVATE
    -Wall -Wextra -pedantic -Werror
)

target_compile_options(
    tests PRIVATE
    -Wall -Wextra -pedantic -Werror
)