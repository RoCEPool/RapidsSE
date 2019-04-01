/*============================================================================*/
/* Interface.h -- Interface namespace                                         */
/*============================================================================*/

#pragma once

#include "Grids.h"

//-----------------------------------------------------------------------------

namespace Interface
{
  void vCheckAssumptions(void);
  
  static const i8 C_ROWS_PER_BUFFER
    = (C_SIZE_LARGE_OBJECT - C_1KB*4)/sizeof(CacheLine*);
}

using namespace Interface;

//-----------------------------------------------------------------------------

#include "RecordBuffer.hpp"

#include "Session.hpp"
#include "ApiMacros.hpp"

#include "Api.hpp"
#include "SessionState.hpp"
#include "ApiState.hpp"
#include "RapidsSE.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
