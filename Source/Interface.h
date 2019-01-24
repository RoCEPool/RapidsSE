/*============================================================================*/
/* Interface.h -- Interface namespace                                         */
/*============================================================================*/

#pragma once

#include "Grids.h"

//-----------------------------------------------------------------------------

namespace Interface
{
  void vCheckAssumptions(void);
  
  static const i8 C_ROWS_PER_BUFFER = (C_SIZE_LARGE_OBJECT - C_1KB*4)/sizeof(Row*);
}

using namespace Interface;

//-----------------------------------------------------------------------------

#include "CommandProcessor.hpp"
#include "Session.hpp"
#include "ApiMacros.hpp"

#include "Api.hpp"
#include "ApiState.hpp"
#include "RapidsSE.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
