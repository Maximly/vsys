/*++

Abstract:

    Basic module (executable or driver)

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once

#ifdef VSYS_USER
int main(int argc, char* argv[]);
#endif // VSYS_USER

namespace vsys {

class Module
{
public:
    ////////////////////////////////////////////////////////////////////////////////
    //  Module instance
    ////////////////////////////////////////////////////////////////////////////////
    static Module* Instance() { return module_; }

    ////////////////////////////////////////////////////////////////////////////////
    //  Startup arguments
    ////////////////////////////////////////////////////////////////////////////////
    int GetArgs() const { return args_count_; }
    const char* GetArg(int arg = 0) const { return arg < GetArgs() ? args_[arg] : nullptr; }

    ////////////////////////////////////////////////////////////////////////////////
    //  Platform and binary info
    ////////////////////////////////////////////////////////////////////////////////
    static const char* GetBinaryInfo() { return binary_info_; }

protected:
    ////////////////////////////////////////////////////////////////////////////////
    //  Init/run/done
    ////////////////////////////////////////////////////////////////////////////////
    Module();
    virtual ~Module();
    virtual bool OnLoad() { return true; }
    virtual bool OnRun() { return true; }
    virtual void OnUnload() {}

    ////////////////////////////////////////////////////////////////////////////////
    //  Platform-specific entry points
    ////////////////////////////////////////////////////////////////////////////////
    #ifdef VSYS_USER
    friend int ::main(int argc, char* argv[]);
    #endif // VSYS_USER

private:
    static Module* module_;
    int args_count_;
    char** args_;
    static const char* binary_info_;
};

} /* vsys */
