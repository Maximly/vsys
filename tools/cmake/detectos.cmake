###############################################################################
#
# Abstract:
#   Detecting OS and process architecture
#    
# Author:
#   Maxim Lyadvinsky
#
# Revision History:
#   16/05/2023 - Maxim Lyadvinsky - Created
#
###############################################################################


###############################################################################
#   Detect host OS
###############################################################################
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(VSYS_HOST_MACOS 1)
    set(VSYS_HOST_TARGET "m")
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(VSYS_HOST_LINUX 1)
    set(VSYS_HOST_TARGET "l")
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(VSYS_HOST_WINDOWS 1)
    set(VSYS_HOST_TARGET "w")
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
elseif (VSYS_HOST_ARCH STREQUAL "x86_64")
    set(VSYS_HOST_PLATFORM "x64")
else()
    message(FATAL_ERROR "Process architecture not detected!")
endif()

set(VSYS_DETECTED_HOST_MESSAGE "Running on ${CMAKE_HOST_SYSTEM_NAME} ${VSYS_HOST_ARCH}")

# Check whether we are running under Rosetta on arm64 hardware
if (VSYS_HOST_MACOS EQUAL 1 AND NOT VSYS_HOST_PLATFORM STREQUAL "a64")
    execute_process(COMMAND sysctl -q hw.optional.arm64
    OUTPUT_VARIABLE _sysctl_stdout
    RESULT_VARIABLE _sysctl_result)
    if(_sysctl_result EQUAL 0 AND _sysctl_stdout MATCHES "hw.optional.arm64: 1")
        set(VSYS_HOST_ROSETTA 1)
        set(VSYS_DETECTED_HOST_MESSAGE "${VSYS_DETECTED_HOST_MESSAGE} on Rosetta")
    endif()
endif()
set(VSYS_HOST "${VSYS_HOST_TARGET}${VSYS_HOST_PLATFORM}")
add_compile_definitions(VSYS_HOST=${VSYS_HOST})
message("${VSYS_DETECTED_HOST_MESSAGE} (host=${VSYS_HOST})")
