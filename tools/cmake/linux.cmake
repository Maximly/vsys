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
