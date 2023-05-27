###############################################################################
#
# Abstract:
#   Collect linux kernel build parameters
#
# Revision History:
#   27/05/2023 - Maxim Lyadvinsky - Created
#
###############################################################################

# Setting up output beautifier
bold=$(tput bold)
normal=$(tput sgr0)
cleanup='rm -rf *.o *~ core .depend .*.cmd *.ko *.mod *.mod.c .tmp_versions *.order *.symvers'

echo $bold'VSYS: Building empty kernel module'$normal
$cleanup
make

echo $bold'VSYS: Detecting compiler parameters'$normal

echo $bold'VSYS: Cleaning up'$normal
#$cleanup


