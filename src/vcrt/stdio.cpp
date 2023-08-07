/*++

Abstract:

    CRT support :: stdio.h

Revision History:

    07/08/2023 - Maxim Lyadvinsky - Created

--*/
#ifdef VSYS_KERNEL

#ifdef VSYS_LIN

int
vsprintf(char *buffer, const char *format, va_list argptr)
{
    return 0;
}

#endif // VSYS_LIN

#endif // VSYS_KERNEL
