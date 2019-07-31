/*============================================================================*/
/* Indexes.h -- Indexes namespace                                             */
/*============================================================================*/
#pragma once

#include "Rows.h"

//------------------------------------------------------------------------------

namespace Indexes
{  
  void vCheckAssumptions(void);
  
  static const i8 C_FLAT_LEVELS_MAX   = 16;
  static const i8 C_FLAT_OFFSETS_MAX  = C_FLAT_LEVELS_MAX - 1;
  static const i8 C_FLAT_FREQ         = 2;
}

//------------------------------------------------------------------------------

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
