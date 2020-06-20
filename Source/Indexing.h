/*============================================================================*/
/* Indexing.h -- Indexing namespace                                       */
/*============================================================================*/
#pragma once

#include "Entries.h"

//------------------------------------------------------------------------------

namespace Indexing
{  
  void vCheckAssumptions(void);
  
  static const i8 C_FLAT_FREQ         = 2;
}

//------------------------------------------------------------------------------

//#define TRACE_COMPACT(x) x EndDefine
#define TRACE_COMPACT(x) EndDefine

// - - - - - - - - - - - -

#include "KeyBuilder.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
