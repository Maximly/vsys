/*++

Abstract:

    Seqprop redifinition to be included by patched seqlock.h

Revision History:

    27/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once

}

static inline seqcount_t* vsys_seqprop_ptr(seqcount_t* s) { return __seqprop_ptr(s); }
static inline seqcount_t* vsys_seqprop_ptr(seqcount_raw_spinlock_t* s) { return __seqprop_raw_spinlock_ptr(s); }
static inline seqcount_t* vsys_seqprop_ptr(seqcount_spinlock_t* s) { return __seqprop_spinlock_ptr(s); }
static inline seqcount_t* vsys_seqprop_ptr(seqcount_rwlock_t* s) { return __seqprop_rwlock_ptr(s); }
static inline seqcount_t* vsys_seqprop_ptr(seqcount_mutex_t* s) { return __seqprop_mutex_ptr(s); }
static inline unsigned vsys_seqprop_sequence(seqcount_t* s) { return __seqprop_sequence(s); }
static inline unsigned vsys_seqprop_sequence(seqcount_raw_spinlock_t* s) { return __seqprop_raw_spinlock_sequence(s); }
static inline unsigned vsys_seqprop_sequence(seqcount_spinlock_t* s) { return __seqprop_spinlock_sequence(s); }
static inline unsigned vsys_seqprop_sequence(seqcount_rwlock_t* s) { return __seqprop_rwlock_sequence(s); }
static inline unsigned vsys_seqprop_sequence(seqcount_mutex_t* s) { return __seqprop_mutex_sequence(s); }
static inline bool vsys_seqprop_preemptible(seqcount_t* s) { return __seqprop_preemptible(s); }
static inline bool vsys_seqprop_preemptible(seqcount_raw_spinlock_t* s) { return __seqprop_raw_spinlock_preemptible(s); }
static inline bool vsys_seqprop_preemptible(seqcount_spinlock_t* s) { return __seqprop_spinlock_preemptible(s); }
static inline bool vsys_seqprop_preemptible(seqcount_rwlock_t* s) { return __seqprop_rwlock_preemptible(s); }
static inline bool vsys_seqprop_preemptible(seqcount_mutex_t* s) { return __seqprop_mutex_preemptible(s); }
static inline void vsys_seqprop_assert(seqcount_t* s) { __seqprop_assert(s); }
static inline void vsys_seqprop_assert(seqcount_raw_spinlock_t* s) { __seqprop_raw_spinlock_assert(s); }
static inline void vsys_seqprop_assert(seqcount_spinlock_t* s) { __seqprop_spinlock_assert(s); }
static inline void vsys_seqprop_assert(seqcount_rwlock_t* s) { __seqprop_rwlock_assert(s); }
static inline void vsys_seqprop_assert(seqcount_mutex_t* s) { __seqprop_mutex_assert(s); }
#define __seqprop(s, prop) vsys_seqprop_ ## prop(s)

extern "C"
{