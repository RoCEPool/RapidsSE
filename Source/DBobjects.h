/*============================================================================*/
/* DBobjects.h -- DBobjects namespace                                         */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

//------------------------------------------------------------------------------

namespace DBobjects
{
  void vCheckAssumptions(void);

  static const i8 C_FLAT_LEVELS_MAX = 16;
  static const i8 C_FLAT_OFFSETS_MAX = C_FLAT_LEVELS_MAX - 1;
  
  //static const i8 C_FLAT_LEVELS  = 3;                        // number of levels
  //static const i8 C_FLAT_OFFSETS = C_FLAT_LEVELS - 1;        // number of offsets
  

  static const i8 C_FLAT_FREQ    = 2;
  static const i8 C_NIL_OFFSET   = 0xFFFFFFFFFFFFFFFF;
  
  static const i8 C_MAX_INDEX_COLUMNS = 16;
}

using namespace DBobjects;

//------------------------------------------------------------------------------

#include "Column.hpp"

typedef Array<CN_ROARRAY,eRO,1024> RelOpAry;

#include "Id.hpp"
#include "EntWrk.hpp"
#include "KeyWrk.hpp"
#include "RightVec.hpp"
#include "Key.hpp"
#include "KeyVal.hpp"
#include "KeyTest.hpp"
#include "Row.hpp"
#include "RowWrk.hpp"
#include "Item.hpp"
#include "ItemBuffer.hpp"
#include "Entry.hpp"
#include "EntryBuffer.hpp"
#include "IndexTree.hpp"
#include "FreeEntry.hpp"
#include "Wilderness.hpp"

#include "WorkingRow.hpp"
#include "WorkingBuffer.hpp"

#include "TablePtn.hpp"

#include "Slots.hpp"

#include "Ring.hpp"

#include "Table.hpp"
#include "Index.hpp"


#include "RbCursor.hpp"
#include "SelCursor.hpp"
#include "RowCursor.hpp"
#include "RowOnce.hpp"

#include "LoadMap.hpp"

#include "LoadChunk.hpp"
#include "Loader.hpp"
#include "LoaderSub.hpp"

#include "User.hpp"

#include "BndLib.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
