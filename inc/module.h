/*++

Abstract:

    Basic module (executable or driver)

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once
#include <vector>
#include <string>
#include "base.h"
#include "strings.h"

namespace vsys {

class Module
{
public:
    ////////////////////////////////////////////////////////////////////////////////
    //  Module instance
    ////////////////////////////////////////////////////////////////////////////////
    static Module* Instance() { return module_; }

    ////////////////////////////////////////////////////////////////////////////////
    //  Start arguments
    ////////////////////////////////////////////////////////////////////////////////
    int GetArgs() const { return args_.size(); }
    std::wstring GetArg(int arg = 0) const { return args_[arg]; }
    std::string GetArgA(int arg = 0) const { return W2A(GetArg(arg).c_str());}

protected:
    ////////////////////////////////////////////////////////////////////////////////
    //  Init/run/done
    ////////////////////////////////////////////////////////////////////////////////
    Module();
    virtual ~Module();
    virtual bool OnLoad() { return true; }
    virtual int OnRun() { return 0; }
    virtual void OnUnload() {}

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
