/*++

Abstract:

    Linux kernel support

Revision History:

    27/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once

#define false _false
#define true _true
typedef bool _Bool;
#define bool _bool
#define _Generic(x, ...) (x)
#define new _new

extern "C" {
    #pragma GCC system_header
    #include <precompiler.h>
    #include <linux/init.h>
    #include <linux/kernel.h>
    #include <linux/module.h>
}

#undef new
#undef false
#undef true
#undef bool
#define _false 0
#define _true 1
