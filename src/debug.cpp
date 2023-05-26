/*++

Abstract:

    Debugging support

Revision History:

    26/05/2023 - Maxim Lyadvinsky - Created

--*/
#include <cstdarg>
#include <cstdio>
#include "debug.h"

#ifdef VSYS_DBG

static const int vsysDbgPref = 64;
static char vsysPref[vsysDbgPref] = {};

void
_DbgPrint(const char* p, ...)
{
    printf("%s", vsysPref);
    va_list args;
    va_start(args, p);
    _DbgPrintRaw(p, args);
    printf("\n");
}


void
_DbgSetPref(const char* p, ...)
{
    va_list args;
    va_start(args, p);
    vsnprintf(vsysPref, vsysDbgPref, p, args);
}


void
_DbgPrintRaw(const char* p, ...)
{
    va_list args;
    va_start(args, p);
    vprintf(p, args);
}


#endif // VSYS_DBG
