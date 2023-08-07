/*++

Abstract:

    Strings helpers

Revision History:

    24/05/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once
#include <string>

namespace vsys {

////////////////////////////////////////////////////////////////////////////////
//  Strings support
////////////////////////////////////////////////////////////////////////////////
std::string W2A(const wchar_t* s);
std::wstring A2W(const char* s);

} // vsys
