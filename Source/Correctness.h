/*============================================================================*/
/* Correctness.h -- Correctness Algebra namespace                             */
/*============================================================================*/
#pragma once

#include "Debugging.h"

//-----------------------------------------------------------------------------

namespace Correctness
{
  void vCheckAssumptions(void);
  void vTraceWith(void);
}

using namespace Correctness;

//-----------------------------------------------------------------------------

#include "AssertMacros.hpp"

#include "Footer.hpp"
#include "Header.hpp"
#include "Object.hpp"
#include "AlignedObject.hpp"
#include "SeeMe.hpp"

#include "Addressing.hpp"
#include "MP.hpp"
#include "HeapVar.hpp"
#include "SizedHeader.hpp"

#define IGNORE_RETURN(call) call

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
