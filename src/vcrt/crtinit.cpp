/*++

Abstract:

    CRT support :: crt init / done functions

Revision History:

    07/08/2023 - Maxim Lyadvinsky - Created

--*/
#include "vcrt/crtinit.h"

#ifdef VSYS_LINKERNEL

typedef void (*Desctructor)(void*);

void *__dso_handle {};

extern "C"
int
__cxa_atexit(Desctructor f, void *p, void *d)
{
    return 0;
}

bool
vsys::InitCrt()
{
    return true;
}

void
vsys::DoneCrt()
{

}

#endif // VSYS_LINKERNEL
