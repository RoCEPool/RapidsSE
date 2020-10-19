/*============================================================================*/
/* Indexing.h -- Indexing namespace                                           */
/*============================================================================*/
#pragma once

#include "SortBuffers.h"

//------------------------------------------------------------------------------

namespace Indexing
{  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

//#define TRACE_COMPACT(x) x EndDefine
#define TRACE_COMPACT(x) EndDefine

// - - - - - - - - - - - -

#include "KeyBuilder.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "EntBufSorter.hpp"

#include "IndexTree.hpp"
typedef VectorNL_t<CN_INDEXTREEVEC, IndexTree, C_MAX_INDEXES> IndexTreeVec;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
