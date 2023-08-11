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
_DbgPrintRawVa(bool ln, bool pref, const char* p, va_list args)
{
    char buf[512];
    int prefResult = pref && vsysPref[0] != 0 ? snprintf(buf, sizeof(buf), "%s", vsysPref) : 0;
    int printResult = 0;
    if (prefResult >= 0)
        printResult = vsnprintf(buf + prefResult, sizeof(buf) - prefResult, p, args);
    if (printResult > 0) {
        printResult += prefResult;
        if (ln && printResult < (static_cast<int>(sizeof(buf)) - 1)) {
            buf[printResult] = '\n';
            buf[printResult + 1] = 0;
        }
        printf("%s", buf);
    }
}


void
_DbgPrintRaw(const char* p, ...)
{
    va_list args;
    va_start(args, p);
    _DbgPrintRawVa(false, false, p, args);
    va_end(args);
}


void
_DbgPrint(const char* p, ...)
{
    va_list args;
    va_start(args, p);
    _DbgPrintRawVa(true, true, p, args);
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
