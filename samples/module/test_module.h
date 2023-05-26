/*++

Abstract:

    Hello driver sample

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once
#include "module.h"

class MyModule : public vsys::Module
{
protected:
    virtual bool OnLoad();
    virtual int OnRun();
    virtual void OnUnload();
};
