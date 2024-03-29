/*++

Abstract:

    Empty linux kernel module to catch compiler/linker options

Revision History:

    27/05/2023 - Maxim Lyadvinsky - Created

--*/
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int __init kbuild_init(void)
{
	return 0;
}

static void __exit kbuild_exit(void)
{
}

module_init(kbuild_init);
module_exit(kbuild_exit);

MODULE_DESCRIPTION("Empty linux kernel module to catch compiler/linker options");
MODULE_LICENSE("Public");
MODULE_AUTHOR("Maxim Lyadvinsky");
