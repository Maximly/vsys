###############################################################################
#
# Abstract:
#   Linux kernel specific setup
#    
# Author:
#   Maxim Lyadvinsky
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
    REQUIRED)
if (EXISTS ${KERNEL_HEADERS})
    include_directories(${KERNEL_HEADERS}/include)
else()
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
message("  Kernel modules: ${KERNEL_MODULES}, makefile: ${KERNEL_MAKEFILE}")


###############################################################################
#   Generating Kbuild file
###############################################################################
get_target_property(MODULE_SOURCES ${PROJECT_NAME} SOURCES)
string(REPLACE ";" " " MODULE_SOURCES_STR "${MODULE_SOURCES}")
configure_file(${CMAKE_TOOLS_DIR}/linux-kbuild.cmake Kbuild @ONLY)

foreach(src ${MODULE_SOURCES})
    configure_file(${src} ${src} COPYONLY)
endforeach()

add_definitions(-D__KERNEL__ -DMODULE)

set(MODULE_CMD ${CMAKE_MAKE_PROGRAM} -C ${KERNEL_MODULES} M=${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(OUTPUT ${VSYS_OUTPUT_NAME}.ko
    COMMAND ${MODULE_CMD} modules
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${MODULE_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/Kbuild
    VERBATIM)
