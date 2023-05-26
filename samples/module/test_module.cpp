/*++

Abstract:

    Basic driver

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#include "test_module.h"
#include "debug.h"

static MyModule myModule;

bool
MyModule::OnLoad()
{
    DbgSetPref(("VSYSTEST: "));
    DbgPrint(("OnLoad"));
    return true;
}


int
MyModule::OnRun()
{
    DbgPrint(("OnRun"));
    return 0;
}


void
MyModule::OnUnload()
{
    DbgPrint(("OnUnload"));
}
