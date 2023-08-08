/*++

Abstract:

    Basic types & defines

Revision History:

    16/06/2021 - Maxim Lyadvinsky - Created

--*/
#pragma once
#include "utils/csupport.h"
#include "targets/targets.h"
#include "utils/debug.h"
#ifdef VSYS_USER
#include <string.h>
#endif // VSYS_USER

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED
