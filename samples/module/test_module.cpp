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

class CtorTest
{
public:
    CtorTest() { DbgPrint(("CtorTest::Ctor() this = %px", this)); }
    ~CtorTest() { DbgPrint(("CtorTest::Dtor() this = %px", this)); }
};

CtorTest t1, t2, t3;

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
