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




#include "Tx.hpp"
#include "TxSet.hpp"



//------------------------------------------------------------------------------

static const i8 C_WSP_ROWBUFFER = C_SIZE_LARGE_OBJECT - C_16KB; // ref NmlStr

#include "Ref.hpp"

#include "Row.hpp"
#include "NmlStr.hpp"
#include "Record.hpp"
typedef HV<Record,RowPix>       HV_Record;
#include "RowBox_t.hpp"


#include "RowSingle.hpp"

#include "CellSegment.hpp"
#include "CellSingle.hpp"

//------------------------------------------------------------------------------

typedef HV<RowSingle,StreamDef> HV_RowSingle;


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
