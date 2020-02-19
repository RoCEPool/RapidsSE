/*============================================================================*/
/* IndexTrees.h -- IndexTrees namespace                                       */
/*============================================================================*/
#pragma once

#include "Entries.h"

//------------------------------------------------------------------------------

namespace IndexTrees
{  
  void vCheckAssumptions(void);
  
  static const i8 C_FLAT_LEVELS_MAX   = 16;
  static const i8 C_FLAT_OFFSETS_MAX  = C_FLAT_LEVELS_MAX - 1;
  static const i8 C_FLAT_FREQ         = 2;
}

//------------------------------------------------------------------------------

//#define TRACE_COMPACT(x) x EndDefine
#define TRACE_COMPACT(x) EndDefine

// - - - - - - - - - - - -

#include "KeyBuilder.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexPtn.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
