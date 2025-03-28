cmake_minimum_required(VERSION 3.20.0)

# Define test executable (no main.cpp)
target_sources(app PRIVATE
    test_main.cpp 
    # test_readclass.cpp 
    # test_reactclass.cpp
    # mocks/gpio_mock.cpp
)

message(STATUS "GOOGLETEST_LIB_DIR: ${GOOGLETEST_LIB_DIR}")

target_link_directories(app PRIVATE ${GOOGLETEST_LIB_DIR})

target_include_directories(app PRIVATE /usr/include/c++/13/)
target_include_directories(app PRIVATE /usr/include/x86_64-linux-gnu/c++/13)
# Link test executable to test framework and app logic

target_link_libraries(app PRIVATE 
    ${GOOGLETEST_LIB_DIR}/libgtest.a 
    ${GOOGLETEST_LIB_DIR}/libgtest_main.a 
    ${GOOGLETEST_LIB_DIR}/libgmock.a 
    ${GOOGLETEST_LIB_DIR}/libgmock_main.a app_obj stdc++)
