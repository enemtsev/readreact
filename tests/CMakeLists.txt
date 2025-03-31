cmake_minimum_required(VERSION 3.20.0)

# Define test executable (no main.cpp)
target_sources(app PRIVATE
    test_main.cpp
    test_zbusmanager.cpp
    # test_readclass.cpp 
    test_reactclass.cpp
    # mocks/gpio_mock.cpp
)

message(STATUS "GOOGLETEST_LIB_DIR: ${GOOGLETEST_LIB_DIR}")

target_link_directories(app PRIVATE ${GOOGLETEST_LIB_DIR})
target_include_directories(app PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(app PRIVATE app_obj gtest gmock)