/*++

Abstract:

    Basic driver

Author:

    Maxim Lyadvinsky

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#include "driver.h"

class MyDriver : vsys::Driver {};

int
main()
{
    MyDriver drv;
    return 0;
}
