cmake_minimum_required(VERSION 3.5)

project(test_module)
add_executable(${PROJECT_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/test_module.cpp")
target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME})
set(VSYS_MODULE_DESCRIPTION "VSYS Test module")
set(VSYS_MODULE_LICENSE "Public")
set(VSYS_MODULE_AUTHOR "Maxim Lyadvinsky")

include("../../tools/cmake/global.cmake")
