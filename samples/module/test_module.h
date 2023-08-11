/*++

Abstract:

    Basic module sample

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once
#include "module.h"

class MyModule : public vsys::Module
{
public:
    MyModule();
    ~MyModule();
    
protected:
    virtual const char* Name() { return "VSYSTEST"; }
    virtual bool OnLoad();
    virtual bool OnRun();
    virtual void OnUnload();
};
