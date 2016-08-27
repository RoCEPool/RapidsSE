/*============================================================================*/
/* Values.h -- Self-Describing Values                                         */
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
namespace Values
{
  void vCheckAssumptions(void);
}

using namespace Values;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
