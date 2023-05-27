###############################################################################
#
# Abstract:
#   Linux kernel module stub
#
# Revision History:
#   27/05/2023 - Maxim Lyadvinsky - Created
#
###############################################################################
KERNEL_DIR 	?= /lib/modules/$(shell uname -r)/build
PWD 	:= $(shell pwd)
obj-m 	:= kbuild.o

default:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod *.mod.c .tmp_versions *.order *.symvers
