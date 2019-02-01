/*============================================================================*/
/* DBobjects.h -- DBobjects namespace                                         */
/*============================================================================*/
#pragma once

#include "MetaData.h"


//------------------------------------------------------------------------------

namespace DBobjects
{
  void vCheckAssumptions(void);

  static const i8 C_FLAT_LEVELS_MAX   = 16;
  static const i8 C_FLAT_OFFSETS_MAX  = C_FLAT_LEVELS_MAX - 1;
  static const i8 C_FLAT_FREQ         = 2;
  static const i8 C_MAX_INDEX_COLUMNS = 16;
  
}

using namespace DBobjects;

//------------------------------------------------------------------------------

#define USE_VLQ_DATUMS // use vlq code for int and bcd datums

typedef ArrayNL_t<CN_ROWAGG,eRO,1024> RelOpAry;

#include "EntWrk.hpp"
#include "EntWrkV2.hpp"
#include "KeyWrk.hpp"
#include "RightVec.hpp"
#include "Key.hpp"
#include "Row.hpp"
#include "RowWrk.hpp"

#include "Entry.hpp"
#include "EntryBuffer.hpp"
#include "IndexTree.hpp"



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
