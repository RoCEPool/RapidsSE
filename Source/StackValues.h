/*============================================================================*/
/* StackValues.h - things that can be pushed on param/return stacks       */
/*============================================================================*/

#pragma once

#include "MemoryManagement.h"

#include "OnStack.hpp"
#include "ValWsp.hpp"

#include "IntVal.hpp"
#include "ObjVal.hpp"
#include "PtrVal.hpp"
#include "StrVal.hpp"
#include "ValSet.hpp"

//-----------------------------------------------------------------------------
namespace StackValues
{
  void vCheckAssumptions(void);
}

using namespace StackValues;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
