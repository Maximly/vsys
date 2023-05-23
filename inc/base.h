/*++

Abstract:

    Basic types & defines

Author:

    Maxim Lyadvinsky

Revision History:

    16/06/2021 - Maxim Lyadvinsky - Created

--*/
#pragma once
#include <cstddef>
#include <string>
#include "targets.h"

namespace vsys {

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED

////////////////////////////////////////////////////////////////////////////////
//  Strings support
////////////////////////////////////////////////////////////////////////////////
std::string W2A(const wchar_t* s);
std::wstring A2W(const char* s);

} /* vsys */
