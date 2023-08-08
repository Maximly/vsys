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
Module::Module() :
    args_count_(0),
    args_(nullptr)
{
    if (Instance() == nullptr)
        module_ = this;
}


Module::~Module()
{
    if (Instance() == this)
        module_ = nullptr;
    for (int i = 0; i < args_count_; i++)
    {
        delete args_[i];
    }
    delete args_;
}


////////////////////////////////////////////////////////////////////////////////
//  Platform info
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


////////////////////////////////////////////////////////////////////////////////
//  Platform-specific entry points: VSYS_USER
////////////////////////////////////////////////////////////////////////////////
#ifdef VSYS_USER
int
main(int argc, char* argv[])
{
    Module* module = Module::Instance();
    bool result = module != nullptr;
    if (result && argc > 0) {
        module->args_count_ = argc;
        module->args_ = new char*[argc];
        if (module->args_) {
            for (int arg = 0; arg < argc; arg++) {
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
    if (result) {
        if (module->OnLoad()) {
            result = module->OnRun();
            module->OnUnload();
        }
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
    DbgPrint(("%s: module_init entered", THIS_MODULE->name));
    bool result = InitCrt();
    DbgPrint(("%s: module_init status is %s", THIS_MODULE->name, result ? "ok (loaded)" : "failed (not loaded)"));
    return result ? 0 : -1;
}

void __exit kbuild_exit(void)
{
    DbgPrint(("%s: module_exit enetered", THIS_MODULE->name));
    DoneCrt();
    DbgPrint(("%s: module_exit exited (unloaded)", THIS_MODULE->name));
}

module_init(kbuild_init);
module_exit(kbuild_exit);

}

#endif //  VSYS_LINKERNEL
