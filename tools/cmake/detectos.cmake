###############################################################################
#
# Abstract:
#   Detecting OS and process architecture
#    
# Revision History:
#   16/05/2023 - Maxim Lyadvinsky - Created
#
###############################################################################


###############################################################################
#   Detect host OS
###############################################################################
string(TOLOWER ${CMAKE_HOST_SYSTEM_NAME} CMAKE_HOST_SYSTEM_NAME_LOW)
if (CMAKE_HOST_SYSTEM_NAME_LOW STREQUAL "darwin")
    set(VSYS_HOST_MACOS 1)
    set(VSYS_HOST_LETTER "m")
    add_definitions(-D VSYS_MAC)
elseif (CMAKE_HOST_SYSTEM_NAME_LOW STREQUAL "linux")
    set(VSYS_HOST_LINUX 1)
    set(VSYS_HOST_LETTER "l")
    add_definitions(-D VSYS_LIN)
elseif (CMAKE_HOST_SYSTEM_NAME_LOW STREQUAL "windows")
    set(VSYS_HOST_WINDOWS 1)
    set(VSYS_HOST_LETTER "w")
    add_definitions(-D VSYS_WIN)
endif()


###############################################################################
#   Detect host process architecture
###############################################################################
execute_process(COMMAND uname -m OUTPUT_VARIABLE _VSYS_HOST_ARCH OUTPUT_STRIP_TRAILING_WHITESPACE)
if (_VSYS_HOST_ARCH STREQUAL "")
    set(_VSYS_HOST_ARCH ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif()

string(TOLOWER ${_VSYS_HOST_ARCH} VSYS_HOST_ARCH)
if (VSYS_HOST_ARCH STREQUAL "arm64" OR VSYS_HOST_ARCH STREQUAL "aarch64")
    set(VSYS_HOST_PLATFORM "a64")
    add_definitions(-D VSYS_A64)
elseif (VSYS_HOST_ARCH STREQUAL "x86_64")
    set(VSYS_HOST_PLATFORM "x64")
    add_definitions(-D VSYS_X64)
else()
    message(FATAL_ERROR "Process architecture not detected!")
endif()

set(VSYS_DETECTED_HOST_MESSAGE
    "Running ${CMAKE_VSYS_MODE} mode ${CMAKE_BUILD_TYPE_LOW} '${PROJECT_NAME}' on ${CMAKE_HOST_SYSTEM_NAME} ${VSYS_HOST_ARCH}")

# Check whether we are running under Rosetta on arm64 hardware
if (VSYS_HOST_MACOS AND NOT VSYS_HOST_PLATFORM STREQUAL "a64")
    execute_process(COMMAND sysctl -q hw.optional.arm64
    OUTPUT_VARIABLE _sysctl_stdout
    RESULT_VARIABLE _sysctl_result)
    if(_sysctl_result EQUAL 0 AND _sysctl_stdout MATCHES "hw.optional.arm64: 1")
        set(VSYS_HOST_ROSETTA 1)
        set(VSYS_DETECTED_HOST_MESSAGE "${VSYS_DETECTED_HOST_MESSAGE} on Rosetta")
        add_definitions(-D VSYS_ROSETTA)
    endif()
endif()


###############################################################################
#   Set platform postfix for binaries: _[k]{OS}{Processor}[d]
#       f.e.:   _ma64d (user mode MacOS arm64 debug)
#               _klx64 (kernel mode Linux x64 release)
###############################################################################
set(VSYS_HOST "${VSYS_HOST_LETTER}${VSYS_HOST_PLATFORM}")
if (VSYS_KERNEL)
    set(CMAKE_POSTFIX _k${VSYS_HOST})
else()
    set(CMAKE_POSTFIX _${VSYS_HOST})
endif()
if (VSYS_DBG)
    set(CMAKE_POSTFIX ${CMAKE_POSTFIX}d)
endif()
set(CMAKE_DEBUG_POSTFIX ${CMAKE_POSTFIX})
set(CMAKE_RELEASE_POSTFIX ${CMAKE_POSTFIX})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
set_target_properties(${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX ${CMAKE_RELEASE_POSTFIX})
set(VSYS_OUTPUT_NAME ${PROJECT_NAME}${CMAKE_POSTFIX})


###############################################################################
#   Print detected platform parameters
###############################################################################
message("${VSYS_DETECTED_HOST_MESSAGE} (${VSYS_OUTPUT_NAME})")
