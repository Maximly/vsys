/*++

Abstract:

    Debugging support

Revision History:

    26/05/2023 - Maxim Lyadvinsky - Created

--*/
#include "base.h"
#include <cstdarg>
#include <cstdio>
#include "debug.h"

#ifdef VSYS_DBG

static const int vsysDbgPref = 64;
static char vsysPref[vsysDbgPref] = {};

void
_DbgPrintRawVA(const char* p, va_list args)
{
    vprintf(p, args);
}


void
_DbgPrintRaw(const char* p, ...)
{
    va_list args;
    va_start(args, p);
    _DbgPrintRawVA(p, args);
    va_end(args);
}


void
_DbgPrint(const char* p, ...)
{
    va_list args;
    va_start(args, p);
    printf("%s", vsysPref);
    _DbgPrintRawVA(p, args);
    va_end(args);
    printf("\n");
}


void
_DbgSetPref(const char* p, ...)
{
    va_list args;
    va_start(args, p);
    vsnprintf(vsysPref, vsysDbgPref, p, args);
    va_end(args);
}




#endif // VSYS_DBG
