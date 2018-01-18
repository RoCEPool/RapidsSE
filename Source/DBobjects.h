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

typedef Vector<CN_COLUMNVEC,Column,CN_COLUMN,1024,u8> ColumnVec;
typedef Array<CN_ROARRAY,eRO,1024> RelOpAry;
typedef Vector<CN_IDXCOLVEC,Column,CN_COLUMN,1024,u8> IdxColVec;
typedef Vector<CN_PTNCOLVEC,Column,CN_COLUMN,1024,u8> PtnColVec;

#include "Id.hpp"
#include "KeyWrk.hpp"
#include "ln.hpp"
#include "on.hpp"
#include "RightVec.hpp"
#include "Key.hpp"
#include "KeyVal.hpp"
#include "KeyTest.hpp"
#include "Row.hpp"
#include "EntWrk.hpp"
typedef Vector<CN_ENTRYVEC,Entry,CN_ENTRY,C_FLAT_LEVELS_MAX,ln> EntryVec;
#include "Item.hpp"
#include "ItemBuffer.hpp"
#include "EntryBuffer.hpp"
#include "IndexData.hpp"
#include "Entry.hpp"

#include "ResultRow.hpp"
#include "ResultBuffer.hpp"

#include "Partition.hpp"
typedef Vector<CN_PARTITIONVEC,Partition,CN_PATHVEC,C_SLOTS_PER_NODE,u8> PartitionVec;

#include "Slots.hpp"

#include "Ring.hpp"


typedef Vector<CN_KEYVEC,KeyVal,CN_KEYVAL,1024,u8> KeyVec;

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
#include "RowData.hpp"

#include "User.hpp"

#include "BndLib.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
