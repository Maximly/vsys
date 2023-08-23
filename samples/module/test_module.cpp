/*++

Abstract:

    Basic module sample

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#include "test_module.h"

static MyModule myModule;

MyModule::MyModule()
{
    DbgPrint(("MyModule::Ctor()"));
}

MyModule::~MyModule()
{
    DbgPrint(("MyModule::Dtor()"));
}

bool
MyModule::OnLoad()
{
    DbgPrint(("OnLoad(): binary type: %s, srcver: %s", GetBinaryInfo(), GetSrcVersion()));
    return true;
}

bool
MyModule::OnRun()
{
    DbgPrint(("OnRun()"));
    return true;
}


void
MyModule::OnUnload()
{
    DbgPrint(("OnUnload()"));
}
