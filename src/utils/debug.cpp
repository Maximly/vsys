/*++

Abstract:

    Debugging support

Revision History:

    26/05/2023 - Maxim Lyadvinsky - Created

--*/
#ifdef VSYS_DBG

static const int vsysDbgPref = 64;
static char vsysPref[vsysDbgPref] = {};

#ifdef VSYS_LINKERNEL
#define printf printk
#endif // VSYS_LINKERNEL

void
_DbgPrintRawVa(bool ln, const char* p, va_list args)
{
    char buf[256];
    int result = vsnprintf(buf, sizeof(buf), p, args);
    if (result > 0) {
        if (ln && result < (static_cast<int>(sizeof(buf)) - 1)) {
            buf[result] = '\n';
            buf[result + 1] = 0;
        }
        printf("%s", buf);
    }
}


void
_DbgPrintRaw(const char* p, ...)
{
    va_list args;
    va_start(args, p);
    _DbgPrintRawVa(false, p, args);
    va_end(args);
}


void
_DbgPrint(const char* p, ...)
{
    va_list args;
    va_start(args, p);
    if (vsysPref[0] != 0)
        printf("%s", vsysPref);
    _DbgPrintRawVa(true, p, args);
    va_end(args);
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
