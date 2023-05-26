cmake_minimum_required(VERSION 3.0)

project(test_module)
add_executable(${PROJECT_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/test_module.cpp")
target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME})
include("../../tools/cmake/global.cmake")
