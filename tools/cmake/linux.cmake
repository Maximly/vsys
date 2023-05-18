###############################################################################
#
# Abstract:
#   Linux specific setup
#    
# Author:
#   Maxim Lyadvinsky
#
# Revision History:
#   16/05/2023 - Maxim Lyadvinsky - Created
#
###############################################################################

add_definitions(-D__KERNEL__ -DMODULE)

# Find the kernel release
execute_process(
    COMMAND uname -r 
    OUTPUT_VARIABLE KERNEL_RELEASE 
    OUTPUT_STRIP_TRAILING_WHITESPACE)

# Find the headers
find_path(
    KERNEL_HEADERS
    include/linux/user.h
    PATHS /usr/src/linux-headers-${KERNEL_RELEASE}
    REQUIRED)
message("  Kernel release: ${KERNEL_RELEASE}")
message("  Kernel headers: ${KERNEL_HEADERS}")
# Add header files
include_directories(${KERNEL_HEADERS}/include)

# Modules path
set(KERNEL_MODULES "/lib/modules/${KERNEL_RELEASE}/build")
find_file(KERNEL_MAKEFILE NAMES Makefile
    PATHS ${KERNEL_MODULES} NO_DEFAULT_PATH)
if(NOT KERNEL_MAKEFILE)
    message(FATAL_ERROR "There is no Makefile in ${KERNEL_MODULES}!")
endif()