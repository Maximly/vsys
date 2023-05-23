###############################################################################
#
# Abstract:
#   Global VSYS building setup
#    
# Author:
#   Maxim Lyadvinsky
#
# Revision History:
#   16/05/2023 - Maxim Lyadvinsky - Created
#
###############################################################################


###############################################################################
#   Global tools path (vsys/tools/cmake)
###############################################################################
set(CMAKE_TOOLS_DIR ${CMAKE_CURRENT_LIST_DIR})


###############################################################################
#   Global compiler options
###############################################################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_TOOLS_DIR}/../../inc")


###############################################################################
#   Global debug/release settings
###############################################################################
if (NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "Cmake is misconfigured: CMAKE_BUILD_TYPE must be specified!")
endif()
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOW)
if (CMAKE_BUILD_TYPE_LOW STREQUAL "debug")
    set(VSYS_DBG 1)
    add_definitions(-D VSYS_DBG)
endif()
if (CMAKE_BUILD_TYPE_LOW STREQUAL "release")
    set(VSYS_REL 1)
    add_definitions(-D VSYS_REL)
endif()
if (CMAKE_BUILD_TYPE_LOW STREQUAL "minsizerel")
    set(VSYS_REL 1)
    add_definitions(-D VSYS_REL)
    add_definitions(-D VSYS_MINSIZEREL)
endif()


###############################################################################
#   Global user/kernel settings
###############################################################################
if (VSYS_KERNEL AND VSYS_USER)
    message(FATAL_ERROR "Cmake is misconfigured: both kernel and user mode cannot be specified!")
else()
    if (VSYS_KERNEL)
        add_definitions(-D VSYS_KERNEL)
        set(CMAKE_VSYS_MODE "kernel")
    elseif (VSYS_USER)
        add_definitions(-D VSYS_USER)
        set(CMAKE_VSYS_MODE "user")
    else()
        message(FATAL_ERROR "Cmake is misconfigured: either kernel and user mode must be specified!")
    endif()
endif()


###############################################################################
#   Global platform settings
###############################################################################
include("${CMAKE_TOOLS_DIR}/detectos.cmake")


###############################################################################
#   Linux kernel settings
###############################################################################
if (VSYS_HOST_LINUX AND VSYS_KERNEL)
    include("${CMAKE_TOOLS_DIR}/linux-kernel.cmake")
endif()


