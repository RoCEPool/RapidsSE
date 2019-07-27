/*============================================================================*/
/* Correctness.h -- Correctness Algebra namespace                             */
/*============================================================================*/
#pragma once

#include "ThirdParty.h"

//-----------------------------------------------------------------------------

namespace Correctness
{
  void vCheckAssumptions(void);
  void vTraceWith(void);
}

using namespace Correctness;

//-----------------------------------------------------------------------------
#include "Process.hpp"
#include "Stz.hpp"
#include "LimitsFormats.h"
#include "ClassNumbers.hpp"
#include "Footer.hpp"
#include "Header.hpp"
#include "FreeChunk.hpp"
#include "Debug.hpp"

#include "StrLib.hpp"
#include "MemLib.hpp"
#include "ErrorCodes.hpp"

#include "AssertMacros.hpp"

#include "Object.hpp"
#include "AlignedObject.hpp"
#include "SeeMe.hpp"

#include "Addressing.hpp"
#include "MP.hpp"
#include "HeapVar.hpp"
#include "SizedHeader.hpp"
#include "Report.hpp"
#include "IntLib.hpp"

#define IGNORE_RETURN(call) call

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
