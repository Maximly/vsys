/*++

Abstract:

    C++ support of C-specific defines and declarations

Revision History:

    06/06/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once

#include <stddef.h>

namespace csupport {

////////////////////////////////////////////////////////////////////////////////
//  Variadic macro parameters count
////////////////////////////////////////////////////////////////////////////////
#define VSYS_ARG1(a1, ...) a1
#define VSYS_ARG2(a1, a2, ...) a2
#define VSYS_ARG3(a1, a2, a3, ...) a3
#define VSYS_ARG4(a1, a2, a3, a4, ...) a4
#define VSYS_ARG5(a1, a2, a3, a4, a5, ...) a5
#define VSYS_ARG6(a1, a2, a3, a4, a5, a6, ...) a6
#define VSYS_ARG7(a1, a2, a3, a4, a5, a6, a7, ...) a7
#define VSYS_ARG8(a1, a2, a3, a4, a5, a6, a7, a8, ...) a8
#define VSYS_ARG9(a1, a2, a3, a4, a5, a6, a7, a8, a9, ...) a9
#define VSYS_ARG10(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) a10
#define VSYS_ARG50( \
    a1,  a2,  a3,  a4,  a5,  a6,  a7,  a8,  a9,  a10, \
    a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,    \
    a21, a22, a23, a24, a25, a26, a27, a28, a29, a30,    \
    a31, a32, a33, a34, a35, a36, a37, a38, a39, a40,    \
    a41, a42, a43, a44, a45, a46, a47, a48, a49, a50, ...) a50

#define VSYS_ARG_COUNT(...) VSYS_ARG50(dummy, ## __VA_ARGS__, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30,                                                 \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20,                                                 \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10,                                                 \
     9,  8,  7,  6,  5,  4,  3,  2,  1,  0)


////////////////////////////////////////////////////////////////////////////////
//  _Generic quick work-around, a smarter solution is needed here
////////////////////////////////////////////////////////////////////////////////
#define _Generic(x, ...) (x)


////////////////////////////////////////////////////////////////////////////////
//  _Static_assert quick work-around, a smarter solution is needed here
////////////////////////////////////////////////////////////////////////////////
#define _Static_assert(x, ...)


////////////////////////////////////////////////////////////////////////////////
//  GCC C extensions
////////////////////////////////////////////////////////////////////////////////

// __builtin_choose_expr
#define __builtin_choose_expr(const_exp, exp1, exp2) ((const_exp) ? (exp1) : (exp2))

// __builtin_constant_p
template<typename> struct const_expr {  enum { result = 0 }; };
template<typename T> struct const_expr<T volatile> { enum { result = 0 }; };
template<typename T> struct const_expr<const T> { enum { result = 1 }; };
template<typename T> struct const_expr<const volatile T> { enum { result = 1 }; };
#define __builtin_constant_p(x) csupport::const_expr<typeof(x)>::result

// __builtin_types_compatible_p
template<typename, typename> struct types_compatible { enum { result = 0 }; };
template<typename T> struct types_compatible<T, T> { enum { result = 1 }; };
template<typename T, size_t N> struct types_compatible<T[], T[N]> { enum { result = 1 }; };
template<typename T, size_t N> struct types_compatible<T[N], T[]> { enum { result = 1 }; };
template<typename T1, typename T2> struct types_compatible<T1, const T2> : types_compatible<T1, T2> {};
template<typename T1, typename T2> struct types_compatible<T1, volatile T2> : types_compatible<T1, T2> {};
template<typename T1, typename T2> struct types_compatible<T1, const volatile T2> : types_compatible<T1, T2> {};
template<typename T1, typename T2> struct types_compatible<const T1, T2> : types_compatible<T1, T2> {};
template<typename T1, typename T2> struct types_compatible<const T1, volatile T2> : types_compatible<T1, T2> {};
template<typename T1, typename T2> struct types_compatible<const T1, const volatile T2> : types_compatible<T1, T2> {};
template<typename T1, typename T2> struct types_compatible<volatile T1, T2> : types_compatible<T1, T2> {};
template<typename T1, typename T2> struct types_compatible<volatile T1, const T2> : types_compatible<T1, T2> {};
template<typename T1, typename T2> struct types_compatible<volatile T1, const volatile T2> : types_compatible<T1, T2> {};
template<typename T1, typename T2> struct types_compatible<const volatile T1, T2> : types_compatible<T1, T2> {};
template<typename T1, typename T2> struct types_compatible<const volatile T1, const T2> : types_compatible<T1, T2> {};
template<typename T1, typename T2> struct types_compatible<const volatile T1, volatile T2> : types_compatible<T1, T2> {};
#define __builtin_types_compatible_p(type1, type2) csupport::types_compatible<type1, type2>::result

} // csupport
