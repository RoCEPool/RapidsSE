/*============================================================================*/
/* DBobjects.h -- DBobjects namespace                                         */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

//------------------------------------------------------------------------------

namespace DBobjects
{
  void vCheckAssumptions(void);
}

using namespace DBobjects;

//------------------------------------------------------------------------------

#include "Row.hpp"
#include "RowBuffer.hpp"
#include "RowStrand.hpp"

#include "Data.hpp"

#include "RowId.hpp"
#include "KeyWsp.hpp"
#include "Key.hpp"
#include "Levels.hpp"
#include "EntWsp.hpp"
#include "Entry.hpp"
#include "IndexBuffer.hpp"
#include "IndexLevels.hpp"

#include "Partition.hpp"
#include "Slots.hpp"

#include "Ring.hpp"

#include "Index.hpp"
#include "Table.hpp"

#include "RbCursor.hpp"
#include "SelCursor.hpp"
#include "RowCursor.hpp"

#include "LoadMap.hpp"

#include "LoadChunk.hpp"
#include "Loader.hpp"
#include "LoaderSub.hpp"

#include "User.hpp"

#include "RowWsp.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
