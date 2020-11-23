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

//#define EB_TRACE_INSERT
//#define EB_LOCAL_VERIFY
//#define EB_SPLIT_RIGHT
#include "EntBuffer.hpp"


#include "EntBufSorter.hpp"

#include "IndexTree.hpp"
typedef VectorNL_t<CN_INDEXTREEVEC, IndexTree, C_MAX_INDEXES> IndexTreeVec;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
