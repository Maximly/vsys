###############################################################################
#
# Abstract:
#   VSYS cross-platform system development library
#
# Revision History:
#   16/05/2023 - Maxim Lyadvinsky - Created
#
###############################################################################
cmake_minimum_required(VERSION 3.0)
#set(CMAKE_VERBOSE_MAKEFILE on)

project(vsys)
file(GLOB VSYS_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/vcrt/*.cpp")
add_library(${PROJECT_NAME} STATIC ${VSYS_SRC})
include("tools/cmake/global.cmake")

###############################################################################
#   VSYS samples
###############################################################################
add_subdirectory("samples/module")
