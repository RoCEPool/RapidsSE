/*============================================================================*/
/* Arguments.h -- Arguments namespace                                         */
/*============================================================================*/
#pragma once

#include "Operands.h"

//------------------------------------------------------------------------------

namespace Arguments
{
  static const i8 C_DEF_LVL_LEVELS  = 3;
  static const i8 C_DEF_LVL_MAXIMUM = 3;
  static const i8 C_DEF_LVL_RATIO   = 3; // this means 1/2^3 i.e. reducing by 8

  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "Args.hpp"
#include "Limits.hpp"
#include "Licence.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
