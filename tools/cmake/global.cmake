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

set(CMAKE_TOOLS_DIR ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_DEBUG_POSTFIX d)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_TOOLS_DIR}/../../inc")
 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

include("${CMAKE_TOOLS_DIR}/detectos.cmake")

if (VSYS_HOST_LINUX EQUAL 1)
    include("${CMAKE_TOOLS_DIR}/linux.cmake")
endif()


