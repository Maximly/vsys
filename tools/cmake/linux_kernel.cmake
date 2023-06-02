###############################################################################
#
# Abstract:
#   Linux kernel specific setup
#    
# Revision History:
#   16/05/2023 - Maxim Lyadvinsky - Created
#
###############################################################################


###############################################################################
#   Find the kernel headers
###############################################################################
execute_process(
    COMMAND uname -r 
    OUTPUT_VARIABLE KERNEL_RELEASE 
    OUTPUT_STRIP_TRAILING_WHITESPACE)
find_path(
    KERNEL_HEADERS
    include/linux/user.h
    PATHS /usr/src/linux-headers-${KERNEL_RELEASE}
          /lib/modules/${KERNEL_RELEASE}/source
    REQUIRED)
if (NOT EXISTS ${KERNEL_HEADERS})
    message(FATAL_ERROR Kernel headers are not found!)
endif()


###############################################################################
#   Modules path
###############################################################################
set(KERNEL_MODULES "/lib/modules/${KERNEL_RELEASE}/build")
find_file(KERNEL_MAKEFILE NAMES Makefile
    PATHS ${KERNEL_MODULES} NO_DEFAULT_PATH)
if(NOT KERNEL_MAKEFILE)
    message(FATAL_ERROR "Makefile is not found in ${KERNEL_MODULES}!")
endif()
message("  Kernel headers: ${KERNEL_HEADERS}")
message("  Kernel modules: ${KERNEL_MODULES} (${KERNEL_MAKEFILE})")


###############################################################################
#   Generate kernel compiler and linker parameters
###############################################################################
set(CMAKE_KBUILD ${CMAKE_TOOLS_DIR}/generated/kbuild.cmake)
if (NOT EXISTS ${CMAKE_KBUILD})
    execute_process(
        COMMAND ./kbuild.sh
        WORKING_DIRECTORY ${CMAKE_TOOLS_DIR}/../kbuild)
endif()
if (EXISTS ${CMAKE_KBUILD})
    message("Kernel module compiler and linker parameters are detected in ${CMAKE_KBUILD}")
    include(${CMAKE_KBUILD})
else()
    message(FATAL_ERROR "Cannot detect kernel module linker parameters!")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-nostdinc>)

###############################################################################
#   Add Linux kernel specific sources
###############################################################################
file(GLOB VSYS_SRC_LINUX "${CMAKE_CURRENT_SOURCE_DIR}/src/kernel/linux/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${VSYS_SRC_LINUX})
