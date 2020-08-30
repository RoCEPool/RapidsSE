/*============================================================================*/
/* MetaData.h -- MetaData namespace                                           */
/*============================================================================*/
#pragma once

#include "Datums.h"

//------------------------------------------------------------------------------

namespace MetaData
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "StreamDef.hpp"

#include "IndexDef.hpp"

typedef VectorNL_t<CN_IDXVEC,Index,C_MAX_INDEXES> IdxVec;
typedef VectorNL_t<CN_TABLEPTNVEC,Partitioning::TablePtn,C_MAX_SLOTS>TablePtnVec;

#include "Table.hpp"

#include "Index.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
