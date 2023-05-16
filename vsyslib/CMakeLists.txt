cmake_minimum_required(VERSION 3.0)

project(vsyslib)
file(GLOB VSYS_SRC "src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${VSYS_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE "inc")

include("../tools/cmake/global.cmake")
