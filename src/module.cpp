/*++

Abstract:

    Basic module (executable or driver)

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#include "module.h"
#include "vcrt/string.h"

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
//  Platform-specific entry points
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
//  Platform-specific entry points
////////////////////////////////////////////////////////////////////////////////
#if defined VSYS_KERNEL && defined VSYS_LIN
extern "C"
{

int __init kbuild_init(void)
{
    return 0;
}

void __exit kbuild_exit(void)
{
}

module_init(kbuild_init);
module_exit(kbuild_exit);

MODULE_DESCRIPTION("Empty linux kernel module to catch compiler/linker options");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Maxim Lyadvinsky");

}

#endif //  VSYS_KERNEL && VSYS_LIN
