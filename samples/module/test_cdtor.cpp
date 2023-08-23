/*++

Abstract:

    Test static ctors/dtors

Revision History:

    23/08/2023 - Maxim Lyadvinsky - Created

--*/
#include "test_cdtor.h"

int CtorTestBase::global_seq_ = CTORSEQ_START;

static CtorTest t1, t2, t3;
