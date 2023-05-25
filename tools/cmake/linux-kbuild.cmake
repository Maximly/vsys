###############################################################################
#
# Abstract:
#   Linux Kbuild template
#
# Revision History:
#   19/05/2023 - Maxim Lyadvinsky - Created
#
###############################################################################

obj-m := @PROJECT_NAME@.o
mymodule-objs := $(pathsubst %.cpp,%.o, $(filter %.cpp, @MODULE_SOURCES_STR@))
