/*============================================================================*/
/* MetaData.h -- MetaData namespace                                           */
/*============================================================================*/

#pragma once

#include "Forth.h"


namespace Jobs      { class Job_CreateTable; class Job_CreateIndex; }
namespace Data { class EntryBuffer; class IndexTree; class RowBuffer; }

namespace MetaData
{

  void vCheckAssumptions(void);
  
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

using namespace MetaData;

//------------------------------------------------------------------------------

#include "RelationalOps.h"

#include "Column.hpp"
#include "Slots.hpp"
#include "Table.hpp"
#include "Index.hpp"

#include "Tenant.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
