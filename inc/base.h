/*++

Abstract:

    Basic types & defines

Revision History:

    16/06/2021 - Maxim Lyadvinsky - Created

--*/
#pragma once
#include <cstddef>
#include "targets.h"

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED

////////////////////////////////////////////////////////////////////////////////
//
//  User mode defines and global includes
//
////////////////////////////////////////////////////////////////////////////////
#ifdef VSYS_USER
int main(int argc, char* argv[]);
#endif // VSYS_USER
