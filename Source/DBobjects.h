/*============================================================================*/
/* DBobjects.h -- DBobjects namespace                                         */
/*============================================================================*/

#pragma once

#include "Dictionaries.h"

//------------------------------------------------------------------------------

namespace DBobjects
{
  void vCheckAssumptions(void);

  static const i8 C_FLAT_LEVELS  = 3;                        // number of levels
  static const i8 C_FLAT_OFFSETS = C_FLAT_LEVELS - 1;        // number of offsets
  static const i8 C_FLAT_FREQ    = 2;
  static const i8 C_NIL_OFFSET   = 0xFFFFFFFFFFFFFFFF;
  
  static const bool C_SPACE_BASED_ON_ENTRIES = false;
  static const i8   C_MAX_ENTRIES = 100;

}

using namespace DBobjects;

//------------------------------------------------------------------------------

#include "Column.hpp"

typedef Vector<CN_COLUMNVEC,Column,CN_COLUMN,1024,u8> ColumnVec;
typedef Array<CN_ROARRAY,eRO,1024> RelOpAry;
typedef Vector<CN_IDXCOLVEC,Column,CN_COLUMN,1024,u8> IdxColVec;

#include "Row.hpp"
#include "RowBuffer.hpp"
#include "RowStrand.hpp"

#include "Data.hpp"

#include "Id.hpp"
#include "KeyWsp.hpp"
#include "Key.hpp"
#include "KeyVal.hpp"
#include "KeyTest.hpp"
#include "ln.hpp"
#include "on.hpp"
#include "RightVec.hpp"
#include "EntryData.hpp"
#include "EntWsp.hpp"
#include "Entry.hpp"

typedef Vector<CN_ENTRYVEC,Entry,CN_ENTRY,C_FLAT_LEVELS,ln> EntryVec;

#include "IndexBuffer.hpp"
#include "IndexData.hpp"

#include "Partition.hpp"
#include "Slots.hpp"

#include "Ring.hpp"


typedef Vector<CN_KEYVEC,KeyVal,CN_KEYVAL,1024,u8> KeyVec;

#include "Table.hpp"
#include "Index.hpp"


#include "RbCursor.hpp"
#include "SelCursor.hpp"
#include "RowCursor.hpp"

#include "LoadMap.hpp"

#include "LoadChunk.hpp"
#include "Loader.hpp"
#include "LoaderSub.hpp"

#include "User.hpp"

#include "RowWsp.hpp"

#include "BndLib.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
