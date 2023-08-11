/*++

Abstract:

    Basic module (executable or driver)

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once

namespace vsys {

class Module
{
public:
    ////////////////////////////////////////////////////////////////////////////////
    //  Module instance
    ////////////////////////////////////////////////////////////////////////////////
    static Module* Instance() { return module_; }
    virtual const char* Name();

    ////////////////////////////////////////////////////////////////////////////////
    //  Startup arguments
    ////////////////////////////////////////////////////////////////////////////////
    #ifdef VSYS_USER
    int GetArgs() const { return args_count_; }
    const char* GetArg(int arg = 0) const { return arg < GetArgs() ? args_[arg] : nullptr; }
    #elif defined VSYS_LINKERNEL
    int GetArgs() const { return THIS_MODULE->args != nullptr ? 1 : 0; }
    const char* GetArg(int arg = 0) const { return arg < GetArgs() ? THIS_MODULE->args : nullptr; }
    #endif // VSYS_USER

    ////////////////////////////////////////////////////////////////////////////////
    //  Platform and binary info
    ////////////////////////////////////////////////////////////////////////////////
    static const char* GetBinaryInfo() { return binary_info_; }
    #ifdef VSYS_LINKERNEL
    static const char* GetSrcVersion() { return THIS_MODULE->srcversion; }
    #else
    static const char* GetSrcVersion() { return not_supported_; }
    #endif // VSYS_LINKERNEL

protected:
    ////////////////////////////////////////////////////////////////////////////////
    //  Init/run/done
    ////////////////////////////////////////////////////////////////////////////////
    Module();
    virtual ~Module();
    virtual bool OnLoad() { return true; }
    virtual void OnUnload() {}
    virtual bool OnRun() { return true; }

private:
    static Module* module_;
    #ifdef VSYS_USER
    int args_count_;
    char** args_;
    #endif // VSYS_USER
    static const char* binary_info_;
    static const char* not_supported_;

    ////////////////////////////////////////////////////////////////////////////////
    //  VSYS lib main entrance/run/exit
    //  DO NOT CALL DIRECTLY!
    ////////////////////////////////////////////////////////////////////////////////
    #ifdef VSYS_USER
    friend bool ModuleStart(int argc, char* argv[]);
    #else
    friend bool ModuleStart();
    #endif // VSYS_USER
    friend bool ModuleRun();
    friend void ModuleExit();
};

#ifdef VSYS_USER
bool ModuleStart(int argc, char* argv[]);
#else
bool ModuleStart();
#endif // VSYS_USER
bool ModuleRun();
void ModuleExit();

} /* vsys */
