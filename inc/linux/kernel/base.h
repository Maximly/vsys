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
enum aarch64_insn_encoding_class : int;
enum hrtimer_restart : int;

// Kernel __is_constexpr redesigned on templates
#define _LINUX_CONST_H
#define __is_constexpr(x) __builtin_constant_p(x)

extern "C" unsigned long _compound_head(const struct page *page);
static inline const struct folio * _vsys_page_folio(const struct page* p) { return (const struct folio *)_compound_head(p); }
static inline struct folio * _vsys_page_folio(struct page* p) { return (struct folio *)_compound_head(p); }
// Update page_folio not using _Generic
#define vsys_page_folio(p) _vsys_page_folio(p)


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
// Rollback defines in kernel headers
#undef swap
#undef inline
#undef current
#undef __alloc_size__
#undef abs
#undef memcpy
#undef memmove
#undef memset


////////////////////////////////////////////////////////////////////////////////
//
//  Prevent STL headers for types already defined in Linux headers
//
////////////////////////////////////////////////////////////////////////////////
#ifdef _LINUX_TYPES_H
#define _BITS_STDINT_UINTN_H
#define _BITS_STDINT_INTN_H 1
#define _SYS_TYPES_H 1
#define __timer_t_defined 1
#endif // _LINUX_TYPES_H

#ifdef _LINUX_SCHED_H
#define _SCHED_H
#endif // _LINUX_SCHED_H
