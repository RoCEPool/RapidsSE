/*============================================================================*/
/* Data.h -- Data namespace                                         */
/*============================================================================*/
#pragma once

#include "MetaData.h"

namespace Data
{
  void vCheckAssumptions(void);

  static const i8 C_FLAT_LEVELS_MAX   = 16;
  static const i8 C_FLAT_OFFSETS_MAX  = C_FLAT_LEVELS_MAX - 1;
  static const i8 C_FLAT_FREQ         = 2;
  static const i8 C_MAX_INDEX_COLUMNS = 16;
  
  static const i8 C_MAX_ROWS = (C_SIZE_LARGE_OBJECT - C_1KB) / C_BYTES_PER_CACHE_LINE;
  
}

using namespace Data;

//------------------------------------------------------------------------------

#define USE_VLQ_DATUMS // use vlq code for int and bcd datums

typedef ArrayNL_t<CN_ROWAGG,eRO,1024> RelOpAry;
typedef TrayNL_t<CN_ROWTRAY,CacheLine,C_MAX_ROWS> RowTray;

#include "EntWrk.hpp"
#include "KeyWrk.hpp"
#include "Key.hpp"
#include "Row.hpp"
#include "Row2.hpp"
#include "RowEncoder.hpp"
#include "RowWrk.hpp"

#include "SkipRoots.hpp"
#include "TablePtn.hpp"
#include "EntryBuffer.hpp"
#include "RowBuffer.hpp"
#include "IndexTree.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
