/*++

Abstract:

    Supported targets and options
    ------------------------------------------------------------
        OSes:
            Mac (m)         VSYS_MAC
            Windows (w)     VSYS_WIN
            Linux (l)       VSYS_LIN
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
    ------------------------------------------------------------
        _[k]{OS}{Processor}[d]
            f.e.:   _ma64d (user mode MacOS arm64 debug)
                    _klx64 (kernel mode Linux x64 release)

Author:

    Maxim Lyadvinsky

Revision History:

    16/05/2023 - Maxim Lyadvinsky - Created

--*/

////////////////////////////////////////////////////////////////////////////////
//
//  Generic checks and defines
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
