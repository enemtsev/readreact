cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 20)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(readreact)

# Add application sources
add_subdirectory(src)

if(NOT TEST_BUILD)
# Create the main executable
    target_sources(app PRIVATE src/main.cpp)
    target_link_libraries(app PRIVATE app_obj zephyr_interface)
else()
    add_subdirectory(tests)
endif()