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

#include <linux/rcupdate.h>
#include <linux/spinlock.h>
#include <linux/lockdep_types.h>
typedef struct lockdep_map *lockdep_map_p;
struct maple_tree {
    union {
        spinlock_t	ma_lock;
        lockdep_map_p	ma_external_lock;
    };
    void __rcu      *ma_root;
    unsigned int	ma_flags;
};

#define MT_FLAGS_LOCK_EXTERN	0x300
#define MT_FLAGS_LOCK_MASK	0x300

static inline bool mt_external_lock(const struct maple_tree *mt)
{
    return (mt->ma_flags & MT_FLAGS_LOCK_MASK) == MT_FLAGS_LOCK_EXTERN;
}

static inline __attribute__((__gnu_inline__)) __attribute__((__unused__)) __attribute__((patchable_function_entry(0, 0))) void mt_init_flags(struct maple_tree *mt, unsigned int flags)
{
    mt->ma_flags = flags;
    if (!mt_external_lock(mt))
        do { spinlock_check(&mt->ma_lock); *(&mt->ma_lock) = (spinlock_t) { { .rlock = { .raw_lock = { { .val = { (0) } } }, } } }; } while (0);
    do { uintptr_t _r_a_p__v = (uintptr_t)(((void *)0)); ; if (
        csupport::const_expr<typeof(
            ((void *)0)
        )>::result
                                                                           && (_r_a_p__v) == (uintptr_t)((void *)0)) do { do { __attribute__((__noreturn__)) extern void __compiletime_assert_252(void) __attribute__((__error__("Unsupported access size for {READ,WRITE}_ONCE()."))); if (!((sizeof((mt->ma_root)) == sizeof(char) || sizeof((mt->ma_root)) == sizeof(short) || sizeof((mt->ma_root)) == sizeof(int) || sizeof((mt->ma_root)) == sizeof(long)) || sizeof((mt->ma_root)) == sizeof(long long))) __compiletime_assert_252(); } while (0); do { *(volatile typeof((mt->ma_root)) *)&((mt->ma_root)) = ((typeof(mt->ma_root))(_r_a_p__v)); } while (0); } while (0); else do { do { } while (0); do { typeof(&mt->ma_root) __p = (&mt->ma_root); union { typeof(
                    (
                    (*&mt->ma_root)
                    )
                    ) __val; char __c[1]; } __u = { .__val = ( typeof(
                (
                (*&mt->ma_root)
                )
                )) ((
//                    typeof(*
//                        ((typeof(mt->ma_root))_r_a_p__v)
//                        )

                    typeof(*
                        ((typeof(mt->ma_root))_r_a_p__v)
                    )
                    *)
                    ((typeof(mt->ma_root))_r_a_p__v)) };
    do { __attribute__((__noreturn__)) extern void __compiletime_assert_253(void) __attribute__((__error__("Need native word sized stores/loads for atomicity."))); if (!((sizeof(*&mt->ma_root) == sizeof(char) || sizeof(*&mt->ma_root) == sizeof(short) || sizeof(*&mt->ma_root) == sizeof(int) || sizeof(*&mt->ma_root) == sizeof(long)))) __compiletime_assert_253(); } while (0); kasan_check_write(__p, sizeof(*&mt->ma_root)); switch (sizeof(*&mt->ma_root)) { case 1: asm volatile ("stlrb %w1, %0" : "=Q" (*__p) : "r" (*(__u8 *)__u.__c) : "memory"); break; case 2: asm volatile ("stlrh %w1, %0" : "=Q" (*__p) : "r" (*(__u16 *)__u.__c) : "memory"); break; case 4: asm volatile ("stlr %w1, %0" : "=Q" (*__p) : "r" (*(__u32 *)__u.__c) : "memory"); break; case 8: asm volatile ("stlr %1, %0" : "=Q" (*__p) : "r" (*(__u64 *)__u.__c) : "memory"); break; } } while (0); } while (0); } while (0);
}
