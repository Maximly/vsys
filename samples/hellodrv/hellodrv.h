/*++

Abstract:

    Hello driver sample

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once
#include "module.h"

class MyDriver : public vsys::Module
{
protected:
    virtual bool OnLoad();
    virtual void OnUnload();
};
