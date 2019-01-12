/*============================================================================*/
/* DBobjects.h -- DBobjects namespace                                         */
/*============================================================================*/

#pragma once

#include "Forth.h"

namespace JobModel
{
  class Job_CreateTable;
  class Job_CreateIndex;
}

//------------------------------------------------------------------------------

namespace DBobjects
{
  void vCheckAssumptions(void);

  static const i8 C_FLAT_LEVELS_MAX = 16;
  static const i8 C_FLAT_OFFSETS_MAX = C_FLAT_LEVELS_MAX - 1;
  
  //static const i8 C_FLAT_LEVELS  = 3;                        // number of levels
  //static const i8 C_FLAT_OFFSETS = C_FLAT_LEVELS - 1;        // number of offsets
  

  static const i8 C_FLAT_FREQ    = 2;
  
  static const i8 C_MAX_INDEX_COLUMNS = 16;
  
  enum eDS // data source
  {
    /* 0 */ DS_NONE=0,
    /* 1 */ DS_INGEST,
    /* 2 */ DS_INSERT,
    /* 3 */ DS_LOAD,
    /* 3 */ DS_ENUMS
  };
  
  static Stz* aDS[]
  {
    /* 0 */ "none"_Stz,
    /* 1 */ "ingest"_Stz,
    /* 2 */ "insert"_Stz,
    /* 3 */ "load"_Stz,
    /* 4 */ "enums"_Stz
  };
}

using namespace DBobjects;

//------------------------------------------------------------------------------

#include "Column.hpp"

typedef Array<CN_ROWAGG,eRO,1024> RelOpAry;

#include "Id.hpp"
#include "EntWrk.hpp"
#include "KeyWrk.hpp"
#include "RightVec.hpp"
#include "Key.hpp"
#include "Row.hpp"
#include "RowAgg.hpp"
#include "RowWrk.hpp"

#include "Entry.hpp"
#include "EntryBuffer.hpp"
#include "IndexTree.hpp"
#include "FreeEntry.hpp"

#include "RecordBuffer.hpp"

#include "FetchBuffer.hpp"

#include "TablePtn.hpp"

#include "Slots.hpp"

#include "Ring.hpp"

#include "Table.hpp"
#include "Index.hpp"

#include "RbCursor.hpp"
#include "SelCursor.hpp"
#include "RowCursor.hpp"
#include "RowOnce.hpp"

#include "User.hpp"

#include "BndLib.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
