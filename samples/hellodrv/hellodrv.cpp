/*++

Abstract:

    Basic driver

Author:

    Maxim Lyadvinsky

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#include "driver.h"
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
class MyDriver : vsys::Driver {};

int
main()
{
int a = 0;
#ifdef VSYS_MAC
    a = 1;
#pragma war  ning ("Mac")
#elif defined VSYS_LIN
    a = 2;
#pragma w ccarning ("Lin")
#endif

    MyDriver drv;
    return 0;
}
