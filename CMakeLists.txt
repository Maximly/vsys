cmake_minimum_required(VERSION 3.0)

project(vsys)
file(GLOB VSYS_SRC "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${VSYS_SRC})
add_subdirectory("samples/hellodrv")

include("tools/cmake/global.cmake")
