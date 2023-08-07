/*++

Abstract:

    Strings helpers

Revision History:

    24/05/2023 - Maxim Lyadvinsky - Created

--*/
#include "base.h"
// Disable Windows CRT warnings
#define _CRT_SECURE_NO_WARNINGS
#include "utils/strings.h"

using namespace vsys;
/*
////////////////////////////////////////////////////////////////////////////////
//  Strings support
////////////////////////////////////////////////////////////////////////////////
std::string
vsys::W2A(const wchar_t* s)
{
    std::string str;
    size_t l = wcslen(s);
    char* buf = new char[l + 1];
    if (buf) {
        wcstombs(buf, s, l + 1);
        str = buf;
        delete[] buf;
    }
    return str;
}


std::wstring
vsys::A2W(const char* s)
{
    std::wstring str;
    size_t l = strlen(s);
    wchar_t* buf = new wchar_t[l + 1];
    if (buf) {
        mbstowcs(buf, s, l + 1);
        str = buf;
        delete[] buf;
    }
    return str;
}
*/