cmake_minimum_required(VERSION 3.0)

project(hellodrv)
add_executable(${PROJECT_NAME} hellodrv.cpp)
target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME})
include("../../tools/cmake/global.cmake")
