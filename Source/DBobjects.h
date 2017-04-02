/*============================================================================*/
/* DBobjects.h -- DBobjects namespace                                         */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

//------------------------------------------------------------------------------

namespace DBobjects
{
  void vCheckAssumptions(void);

  static const i8 C_FLAT_LEVELS = 3;
  static const i8 C_FLAT_FREQ = 2;
  static const i8 C_NIL_OFFSET = 0xFFFFFFFFFFFFFFFF;
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
#include "RightVec.hpp"
#include "EntryData.hpp"
#include "EntWsp.hpp"
#include "EntWspSeq.hpp"
#include "Entry.hpp"
#include "EntrySeq.hpp"
#include "IndexBuffer.hpp"
#include "IndexBufferSeq.hpp"
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
