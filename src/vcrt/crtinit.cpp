/*++

Abstract:

    CRT support :: crt init / done functions

Revision History:

    07/08/2023 - Maxim Lyadvinsky - Created

--*/
#include "vcrt/crtinit.h"
#include "utils/debug.h"

typedef void(*Ctor)();
typedef void(*Dtor)(void*);
struct DtorLink
{
    Dtor dtor;
    void* param;
    void* dso;
    DtorLink* prev;
};


#ifdef VSYS_LINKERNEL

extern "C" void* __dso_handle __attribute__ ((__visibility__ ("hidden")));
extern "C" const Ctor vcrtCtorStart[];
extern "C" const Ctor vcrtCtorEnd[];
void* __dso_handle = nullptr;

bool
vsys::InitCrt()
{
    bool result = &vcrtCtorStart != &vcrtCtorEnd;
    if (result) {
        for (const Ctor* ctor = vcrtCtorStart; ctor < vcrtCtorEnd; ++ctor)
        {
            (*ctor)();
        }
    } else {
        DbgPrint(("error Ctors not detected!"));
    }
    return result;
}


static DtorLink* dtorList = nullptr;

extern "C"
int
__cxa_atexit(Dtor f, void* p, void* d)
{
    DtorLink* link = new DtorLink;
    bool result = link != nullptr;
    if (result) {
        link->dtor = f;
        link->param = p;
        link->dso = d;
        link->prev = dtorList;
        dtorList = link;
    }
    return result ? 0 : -1;
}


void
vsys::DoneCrt()
{
    DtorLink* link = dtorList;
    while (link != nullptr)
    {
        link->dtor(link->param);
        DtorLink* processed = link;
        link = link->prev;
        delete processed;
    }
    dtorList = nullptr;
}

#endif // VSYS_LINKERNEL
