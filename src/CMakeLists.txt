add_library(app_obj OBJECT
    reactled.cpp
    reactclass.cpp
    readclass.cpp
    zbusmanager.cpp
)

target_include_directories(app_obj PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(app_obj PUBLIC zephyr_interface)