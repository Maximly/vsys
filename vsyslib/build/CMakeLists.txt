cmake_minimum_required(VERSION 3.0)

project(vsyslib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

###############################################################################
#   Detect host process architecture
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

execute_process(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE VSYS_HOST_ARCH)
if (VSYS_HOST_ARCH STREQUAL "")
    set(VSYS_HOST_ARCH ${CMAKE_HOST_SYSTEM_PROCESSOR})
endif()

if (VSYS_HOST_ARCH STREQUAL "arm64")
    set(VSYS_HOST_PLATFORM "a64")
elseif (VSYS_HOST_ARCH STREQUAL "x86_64")
    set(VSYS_HOST_PLATFORM "x64")
else()
    message(FATAL_ERROR "Process architecture not detected!")
endif()

set(VSYS_DETECTED_HOST_MESSAGE "Running on ${CMAKE_HOST_SYSTEM_NAME} ${VSYS_HOST_ARCH}")

# Check whether we are running under Rosetta on arm64 hardware
if (VSYS_HOST_MACOS EQUAL 1 AND NOT VSYS_HOST_ARCH STREQUAL "arm64")
    execute_process(COMMAND sysctl -q hw.optional.arm64
    OUTPUT_VARIABLE _sysctl_stdout
    RESULT_VARIABLE _sysctl_result)
    if(_sysctl_result EQUAL 0 AND _sysctl_stdout MATCHES "hw.optional.arm64: 1")
        set(VSYS_HOST_ROSETTA 1)
        set(VSYS_DETECTED_HOST_MESSAGE "${VSYS_DETECTED_HOST_MESSAGE} on Rosetta")
    endif()
endif()

message("${VSYS_DETECTED_HOST_MESSAGE}")

###############################################################################
#   Set targets
###############################################################################

add_custom_target(kl64)     # linux kernel module x64 bit
add_custom_target(klarm64)  # linux kernel module arm64 bit
add_custom_target(kw64)     # windows kernel module x64 bit

# Linux kernel specific
target_compile_definitions(-D__KERNEL__ -DMODULE)
