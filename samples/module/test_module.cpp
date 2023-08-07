/*++

Abstract:

    Basic module sample

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#include "test_module.h"

static MyModule myModule;

bool
MyModule::OnLoad()
{
    DbgSetPref(("VSYSTEST: "));
    DbgPrint(("OnLoad with %i argument(s)\n"
        "    Path: %s\n"
        "    Binary type: %s",
        GetArgs(),
        GetArg(),
        GetBinaryInfo()));
    return true;
}

bool
MyModule::OnRun()
{
    DbgPrint(("OnRun"));
    return true;
}


void
MyModule::OnUnload()
{
    DbgPrint(("OnUnload"));
}
