/*++

Abstract:

    Basic module (executable or driver)

Author:

    Maxim Lyadvinsky

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#include "module.h"

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
            result = module->Run();
            module->OnUload();
        }
    }
    return result;
}
#endif // VSYS_USER
