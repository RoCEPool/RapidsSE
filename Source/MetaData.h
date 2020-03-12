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

static Stz* C_TDEF_LOCKSTATS   = "__lockStats"_Stz;
static Stz* C_TDEF_SEEKOFFSETS = "__seekOffsets"_Stz;

#include "StreamDef.hpp"
#include "TableDef.hpp"
#include "IndexDef.hpp"

#include "Slots.hpp"
#include "Table.hpp"
#include "Index.hpp"
#include "IdxColVec.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
