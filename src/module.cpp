/*++

Abstract:

    Basic module (executable or driver)

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
//#include "base.h"
#include "module.h"
#include "strings.h"

using namespace vsys;

////////////////////////////////////////////////////////////////////////////////
//  Init/done
////////////////////////////////////////////////////////////////////////////////
Module* Module::module_ = nullptr;
Module::Module()
{
    if (Instance() == nullptr)
        module_ = this;
}


Module::~Module()
{
    if (Instance() == this)
        module_ = nullptr;
}



////////////////////////////////////////////////////////////////////////////////
//  Platform info
////////////////////////////////////////////////////////////////////////////////
std::string
Module::GetBinaryInfoA()
{
    std::string info;
    #ifdef VSYS_MAC
    info = "Darwin ";
    #elif defined VSYS_WIN
    info = "Windows ";
    #elif defined VSYS_LIN
    info = "Linux ";
    #else
    info = "Undefined ";
    #endif // VSYS_MAC
    #ifdef VSYS_USER
    info += "user mode ";
    #else
    info += "kernel mode ";
    #endif // VSYS_USER
    #ifdef VSYS_X64
    info += "x64";
    #elif defined VSYS_A64
    info += "arm64";
    #endif // VSYS_MAC
    return info;
}



////////////////////////////////////////////////////////////////////////////////
//  Platform-specific entry points
////////////////////////////////////////////////////////////////////////////////
#ifdef VSYS_USER
int
main(int argc, char* argv[])
{
    int result = -1;
    Module* module = Module::Instance();
    if (module) {
        for (int arg = 0; arg < argc; arg++) {
            module->args_.push_back(A2W(argv[arg]));
        }
        if (module->OnLoad()) {
            result = module->OnRun();
            module->OnUnload();
        }
    }
    return result;
}
#endif // VSYS_USER

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
