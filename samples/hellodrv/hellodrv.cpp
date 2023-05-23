/*++

Abstract:

    Basic driver

Author:

    Maxim Lyadvinsky

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#include "hellodrv.h"

static MyDriver driver;

bool
MyDriver::OnLoad()
{
    return true;
}


void
MyDriver::OnUnload()
{
}
