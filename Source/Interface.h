/*============================================================================*/
/* Interface.h -- Interface namespace                                         */
/*============================================================================*/

#pragma once

#include "Scheduling.h"

//-----------------------------------------------------------------------------

namespace Interface
{
  void vCheckAssumptions(void);
  
  static const i8 C_ROWS_PER_BUFFER
    = (C_SIZE_LARGE_OBJECT - C_1KB*4)/sizeof(CacheLine*);
}

using namespace Interface;

//-----------------------------------------------------------------------------


#include "ApiMacros.hpp"

#include "Api.hpp"
#include "ApiLog.hpp"
#include "ApiState.hpp"
#include "RapidsSE.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
