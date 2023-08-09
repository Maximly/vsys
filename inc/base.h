/*++

Abstract:

    Basic types & defines

Revision History:

    16/06/2021 - Maxim Lyadvinsky - Created

--*/
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "utils/csupport.h"
#include "targets/targets.h"
#ifdef VSYS_USER
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#endif // VSYS_USER
#include "utils/debug.h"

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED
