/*++

Abstract:

    Platform-specific defines and includes. Supported targets and options:
    ----------------------------------------------------------------------
        OSes:
            Mac (m)         VSYS_MAC, VSYS_MACUSER, VSYS_MACKERNEL
            Windows (w)     VSYS_WIN, VSYS_WINUSER, VSYS_WINKERNLE
            Linux (l)       VSYS_LIN, VSYS_LINUSER, VSYS_LINKERNEL
            EFI (e)         VSYS_EFI
        Processors:
            arm64 (a64)     VSYS_A64
            x64 (x64)       VSYS_X64
            x64 on Arm      VSYS_X64 && VSYS_ROSETTA
        Target mode:
            User:           VSYS_USER
            Kernel (k):     VSYS_KERNEL
        Build type:
            Debug (d)       VSYS_DBG
            Release         VSYS_REL
            MinSizeRel      VSYS_REL && VSYS_MINSIZEREL

    Target postfix for binaries/libs
    ----------------------------------------------------------------------
        _[k]{OS}{Processor}[d]
            f.e.:   _ma64d (user mode MacOS arm64 debug)
                    _klx64 (kernel mode Linux x64 release)

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once

////////////////////////////////////////////////////////////////////////////////
//
//  Generic platform checks
//
////////////////////////////////////////////////////////////////////////////////

// Check OS is known and defined
#if !defined VSYS_MAC && \
    !defined VSYS_WIN && \
    !defined VSYS_LIN && \
    !defined VSYS_EFI
#error OS is unknown or must be defined!
#endif

// Check processor architecture is known and defined
#if !defined VSYS_A64 && \
    !defined VSYS_X64
#error Processor architecture is unknown or must be defined!
#endif

// Check user/kernel mode is specified
#if !defined VSYS_USER && \
    !defined VSYS_KERNEL
#error User or kernel mode must be specified explicitly!
#endif

// Check user/kernel mode is specified
#if !defined VSYS_DBG && \
    !defined VSYS_REL
#error Debug or release must be defined explicitly!
#endif


////////////////////////////////////////////////////////////////////////////////
//
//  Additional defines
//
////////////////////////////////////////////////////////////////////////////////
#if defined VSYS_KERNEL
    #if defined VSYS_LIN
    #define VSYS_LINKERNEL
    #elif defined VSYS_WIN
    #define VSYS_WINKERNEL
    #elif defined VSYS_MAC
    #define VSYS_MACKERNEL
    #endif // VSYS_LIN
#elif defined VSYS_USER
    #if defined VSYS_LIN
    #define VSYS_LINUSER
    #elif defined VSYS_WIN
    #define VSYS_WINUSER
    #elif defined VSYS_MAC
    #define VSYS_MACUSER
    #endif // VSYS_LIN
#endif // VSYS_KERNEL


////////////////////////////////////////////////////////////////////////////////
//
//  Linux kernel defines
//
////////////////////////////////////////////////////////////////////////////////
#ifdef VSYS_LINKERNEL
#include "linux/kernel/base.h"
#endif // VSYS_LINKERNEL
