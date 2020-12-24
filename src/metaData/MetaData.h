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

#include "BucketMap.hpp"
#include "PtnMap.hpp"

#include "StreamDef.hpp"

#include "IndexDef.hpp"

typedef VectorNL_t<CN_IDXVEC,Index,C_MAX_INDEXES> IdxVec;
typedef VectorNL_t<CN_TABLEPTNVEC,DataBuffers::TablePtn,C_MAX_TABLEPTNS>TablePtnVec;

#include "Table.hpp"

#include "Index.hpp"



#include "Folder.hpp"
#include "Schema.hpp"
#include "Catalog.hpp"
#include "Tenant.hpp"
#include "Tenants.hpp"



#include "Marker.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
