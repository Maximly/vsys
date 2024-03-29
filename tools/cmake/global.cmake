###############################################################################
#
# Abstract:
#   Global VSYS building setup
#    
# Revision History:
#   16/05/2023 - Maxim Lyadvinsky - Created
#
###############################################################################

###############################################################################
#   Current target type
###############################################################################
get_target_property(VSYS_TARGET_TYPE ${PROJECT_NAME} TYPE)
if (VSYS_TARGET_TYPE STREQUAL "EXECUTABLE")
    set(VSYS_MOD 1)
else()
    set(VSYS_LIB 1)
endif()


###############################################################################
#   Global tools path (vsys/tools/cmake)
###############################################################################
set(CMAKE_TOOLS_DIR ${CMAKE_CURRENT_LIST_DIR})


###############################################################################
#   Global compiler options
###############################################################################
set(VSYS_INCLUDE "${CMAKE_TOOLS_DIR}/../../inc")
set(VSYS_SOURCE "${CMAKE_TOOLS_DIR}/../../src")
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${VSYS_INCLUDE}
    ${VSYS_INCLUDE}/targets)
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<NOT:$<COMPILE_LANGUAGE:C>>>:-Wall -Wextra -Werror>)


###############################################################################
#   Global debug/release settings
###############################################################################
if (NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "Cmake is misconfigured: CMAKE_BUILD_TYPE must be specified!")
endif()
string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOW)
if (CMAKE_BUILD_TYPE_LOW STREQUAL "debug")
    set(VSYS_DBG 1)
    target_compile_definitions(${PROJECT_NAME} PRIVATE VSYS_DBG)
endif()
if (CMAKE_BUILD_TYPE_LOW STREQUAL "release")
    set(VSYS_REL 1)
    target_compile_definitions(${PROJECT_NAME} PRIVATE VSYS_REL)
endif()
if (CMAKE_BUILD_TYPE_LOW STREQUAL "minsizerel")
    set(VSYS_REL 1)
    target_compile_definitions(${PROJECT_NAME} PRIVATE VSYS_REL VSYS_MINSIZEREL)
endif()


###############################################################################
#   Global user/kernel settings
###############################################################################
if (VSYS_KERNEL AND VSYS_USER)
    message(FATAL_ERROR "Cmake is misconfigured: both kernel and user mode cannot be specified!")
else()
    if (VSYS_KERNEL)
        set(CMAKE_VSYS_MODE "kernel")
        target_compile_definitions(${PROJECT_NAME} PRIVATE VSYS_KERNEL)
    elseif (VSYS_USER)
        set(CMAKE_VSYS_MODE "user")
        target_compile_definitions(${PROJECT_NAME} PRIVATE VSYS_USER)
    else()
        message(FATAL_ERROR "Cmake is misconfigured: either kernel and user mode must be specified!")
    endif()
endif()


###############################################################################
#   Global OS/processor architecture settings
###############################################################################
include("${CMAKE_TOOLS_DIR}/detectos.cmake")


###############################################################################
#   Platform-specific settings
###############################################################################
if (VSYS_HOST_WINDOWS)
    target_compile_options(${PROJECT_NAME} PRIVATE -FIbase.h)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -include base.h)
endif()

#   Linux kernel settings
if (VSYS_HOST_LINUX AND VSYS_KERNEL)
    include("${CMAKE_TOOLS_DIR}/linux_kernel.cmake")
endif()


###############################################################################
#   Strip binary
###############################################################################
if (VSYS_MOD AND VSYS_HOST_LINUX AND VSYS_REL)
    if (VSYS_KERNEL)
        set(VSYS_STRIP --strip-unneeded)
    else()
        set(VSYS_STRIP --strip-all)
    endif()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND echo "Striping debug symbols [${VSYS_STRIP}] from" $<TARGET_FILE:${PROJECT_NAME}>
        COMMAND ${CMAKE_STRIP} ${VSYS_STRIP} $<TARGET_FILE:${PROJECT_NAME}>)
endif()