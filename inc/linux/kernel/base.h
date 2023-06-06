/*++

Abstract:

    Linux kernel support

Revision History:

    27/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once

////////////////////////////////////////////////////////////////////////////////
//
//  Magic defines to include kernel headers to C++
//
////////////////////////////////////////////////////////////////////////////////

// Redefine bool values
#define false _false
#define true _true
typedef bool _Bool;

// Undef asmlinkage
#define asmlinkage

//  Define empty undeclared enums or forward-declare enums
enum aarch64_insn_encoding_class {};
enum hrtimer_restart : int;

// Kernel __is_constexpr redesigned on templates
#define _LINUX_CONST_H
#define __is_constexpr(x) __builtin_constant_p(x)

// Define missing __attribute_const__
#define __attribute_const__ __attribute__((const))

////////////////////////////////////////////////////////////////////////////////
//
//  Include basic kernel headers
//
////////////////////////////////////////////////////////////////////////////////
extern "C" {
    #pragma GCC system_header
    #include <precompiler.h>
    #include <linux/init.h>
    #include <linux/kernel.h>
    #include <linux/module.h>
}


////////////////////////////////////////////////////////////////////////////////
//
//  Rollback some magic as not needed further
//
////////////////////////////////////////////////////////////////////////////////

// Rollback bool values
#undef false
#undef true
#define _false 0
#define _true 1
#undef swap
