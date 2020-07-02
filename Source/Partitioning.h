/*============================================================================*/
/* Partitioning.h -- Partitioning namespace                                   */
/*============================================================================*/
#pragma once

#include "ReadConsistency.h"

//------------------------------------------------------------------------------

namespace Partitioning
{  
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

//#define TRACE_UPSERT(smt) smt EndDefine
#define TRACE_UPSERT(smt) EndDefine

//------------------------------------------------------------------------------

#include "BucketMap.hpp"
#include "PtnMap.hpp"
#include "TablePtn.hpp"
#include "IndexStream.hpp"
#include "PtnStream.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
