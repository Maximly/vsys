/*++

Abstract:

    Basic module (executable or driver)

Author:

    Maxim Lyadvinsky

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once
#include "base.h"
#include <vector>

namespace vsys {

class Module
{
public:
    ////////////////////////////////////////////////////////////////////////////////
    //  Module instance
    ////////////////////////////////////////////////////////////////////////////////
    static Module* Instance() { return module_; }

protected:
    ////////////////////////////////////////////////////////////////////////////////
    //  Init/run/done
    ////////////////////////////////////////////////////////////////////////////////
    Module();
    virtual ~Module();
    virtual bool OnLoad() { return true; }
    virtual int Run() { return 0; }
    virtual void OnUload() {}

    ////////////////////////////////////////////////////////////////////////////////
    //  Platform-specific entry points
    ////////////////////////////////////////////////////////////////////////////////
    #ifdef VSYS_USER
    friend int ::main(int argc, char* argv[]);
    #endif // VSYS_USER

private:
    static Module* module_;
    std::vector<std::wstring> args_;
};

} /* vsys */

