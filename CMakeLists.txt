cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(readreact)

target_sources(app PRIVATE src/main.cpp)
target_include_directories(app PRIVATE include)

enable_testing()

# Add testing support
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
find_package(GTest REQUIRED)

# Mock classes
add_library(gpio_mock STATIC
    tests/mocks/gpio_mock.cpp
)

target_include_directories(gpio_mock PUBLIC
    tests/mocks
)

# Main test executable
add_executable(test_readreact
    tests/test_main.cpp
    tests/test_readclass.cpp
    tests/test_reactclass.cpp
)

target_link_libraries(test_readreact
    app
    gpio_mock
    GTest::GTest
    GTest::Main
)

add_test(NAME test_readreact COMMAND test_readreact)