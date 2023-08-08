/*++

Abstract:

    CRT support :: crt init / done functions

Revision History:

    07/08/2023 - Maxim Lyadvinsky - Created

--*/
#pragma once

namespace vsys {

bool InitCrt();
void DoneCrt();

} /* vsys */