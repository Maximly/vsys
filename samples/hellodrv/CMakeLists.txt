cmake_minimum_required(VERSION 3.0)

project(hellodrv)
add_executable(${PROJECT_NAME} hellodrv.cpp)
add_dependencies(${PROJECT_NAME} vsys)
include("../../tools/cmake/global.cmake")
