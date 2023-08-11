/*++

Abstract:

    Basic module (executable or driver)

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#include "module.h"
#include "vcrt/crtinit.h"

using namespace vsys;

////////////////////////////////////////////////////////////////////////////////
//  Init/done
////////////////////////////////////////////////////////////////////////////////
Module* Module::module_ = nullptr;
Module::Module()
    #ifdef VSYS_USER
    :
    args_count_(0),
    args_(nullptr)
    #endif // VSYS_USER
{
    if (Instance() == nullptr)
        module_ = this;
}


Module::~Module()
{
    if (Instance() == this)
        module_ = nullptr;
    #ifdef VSYS_USER
    for (int i = 0; i < args_count_; i++)
    {
        delete args_[i];
    }
    delete args_;
    #endif // VSYS_USER
}


bool
#ifdef VSYS_USER
vsys::ModuleStart(int argc, char* argv[])
#else
vsys::ModuleStart()
#endif // VSYS_USER
{
    Module* module = Module::Instance();
    bool result = module != nullptr;
    #ifdef VSYS_USER
    if (result) {
        DbgSetPref(("%s: ", module->Name()));
    }
    if (result && argc > 0) {
        module->args_count_ = argc;
        module->args_ = new char*[argc];
        if (module->args_) {
            for (int arg = 0; arg < argc; arg++)
            {
                module->args_[arg] = new char[strlen(argv[arg]) + 1];
                    if (module->args_[arg]) {
                        strcpy(module->args_[arg], argv[arg]);
                    } else {
                        result = false;
                        break;
                    }
            }
        } else {
            result = false;
        }
    }
    #endif // VSYS_USER
    if (result)
        result = module->OnLoad();
    return result;
}


bool
vsys::ModuleRun()
{
    return Module::Instance() != nullptr && Module::Instance()->OnRun();
}


void
vsys::ModuleExit()
{
    if (Module::Instance())
        Module::Instance()->OnUnload();
}


////////////////////////////////////////////////////////////////////////////////
//  Module/platform info
////////////////////////////////////////////////////////////////////////////////
const char* Module::binary_info_ =
    #ifdef VSYS_MAC
    "Darwin "
    #elif defined VSYS_WIN
    "Windows "
    #elif defined VSYS_LIN
    "Linux "
    #else
    "Undefined "
    #endif // VSYS_MAC
    #ifdef VSYS_USER
    "user mode "
    #else
    "kernel mode "
    #endif // VSYS_USER
    #ifdef VSYS_X64
    "x64";
    #elif defined VSYS_A64
    "arm64";
    #endif // VSYS_X64
const char* Module::not_supported_ = "not supported";

const char*
Module::Name()
{
    const char* name;
    #ifdef VSYS_LINKERNEL
    name = THIS_MODULE->name;
    #else
    name = "VSYS";
    #endif // VSYS_LINKERNEL
    return name;
}


////////////////////////////////////////////////////////////////////////////////
//  Platform-specific entry points: VSYS_USER
////////////////////////////////////////////////////////////////////////////////
#ifdef VSYS_USER
int
main(int argc, char* argv[])
{
    bool result = ModuleStart(argc, argv);
    if (result) {
        result = ModuleRun();
        ModuleExit();
    }
    return result ? 0 : -1;
}
#endif // VSYS_USER


////////////////////////////////////////////////////////////////////////////////
//  Platform-specific entry points: VSYS_LINKERNEL
////////////////////////////////////////////////////////////////////////////////
#if defined VSYS_LINKERNEL

extern "C"
{

int __init kbuild_init(void)
{
    DbgSetPref(("%s: ", THIS_MODULE->name));
    DbgPrint(("module_init entered"));
    bool result = InitCrt();
    if (result)
        result = ModuleStart();
    DbgPrint(("module_init status is %s", result ? "ok (loaded)" : "failed (not loaded)"));
    return result ? 0 : -1;
}

void __exit kbuild_exit(void)
{
    DbgPrint(("module_exit enetered"));
    ModuleExit();
    DoneCrt();
    DbgPrint(("module_exit exited (unloaded)"));
}

module_init(kbuild_init);
module_exit(kbuild_exit);

}

#endif //  VSYS_LINKERNEL
