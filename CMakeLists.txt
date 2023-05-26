cmake_minimum_required(VERSION 3.0)

project(vsys)
file(GLOB VSYS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${VSYS_SRC})
add_subdirectory("samples/module")

include("tools/cmake/global.cmake")
