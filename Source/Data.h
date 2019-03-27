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
  
  enum eMN // metadata nibbles
  {
  MN_NULL = 0b0000,          // null datum (no bytes in data words)
  MN_1B   = 0b0001,          // data words contain 1B datum
  MN_2B   = 0b0010,          // data words contain 2B datum
  MN_3B   = 0b0011,          // data words contain 3B datum
  MN_4B   = 0b0100,          // data words contain 4B datum
  MN_5B   = 0b0101,          // data words contain 5B datum
  MN_6B   = 0b0110,          // data words contain 6B datum
  MN_7B   = 0b0111,          // data words contain 7B datum
  MN_8B   = 0b1000,          // data words contain 8B datum
  
  MN_OVERFLOW = 0b1001,      // data words has 2B indicating overflow cacheline
  MN_REPEATS  = 0b1010,      // next nibble contains nibble repeat count
  
  MN_FALSE = 0b1110,         // false datum (no bytes in data words)
  MN_TRUE  = 0b1111,         // true datum (no bytes in data words)
  
  MN_MINUS_ONE = 0b1101,     // -1 datum (no bytes in data words)
  MN_ZERO      = 0b1110,     // 0 datum (no bytes in data words)
  MN_PLUS_ONE  = 0b1111,     // 1 datum (no bytes in data words)
  
  MN_DIFF_4B   = 0b1100,     // 4B in data words indicating difference
  MN_DIFF_3B   = 0b1101,     // 3B in data words datum indicating difference
  MN_DIFF_2B   = 0b1110,     // 2B in data words datum indicating difference
  MN_DIFF_1B   = 0b1111,     // 1B in data words datum indicating difference
  
  MN_COL_ARRAY   = 0b1011,   // 1B in data words subscript into col array
  MN_SHOCO_CL    = 0b1100,   // len+shoco in data words
  MN_SHOCO_2B    = 0b1101,   // 2B in data words indicating shoco overflow
  MN_LOCAL_HMAP  = 0b1110,   // 2B in data words indicating local hashmap
  MN_GLOBAL_HMAP = 0b1111,   // 2B in data words indicating global hashmap
  };
  

}

using namespace Data;

//------------------------------------------------------------------------------

typedef ArrayNL_t<CN_ROWAGG,eRO,1024> RelOpAry;
typedef TrayNL_t<CN_ROWTRAY,CacheLine,C_MAX_ROWS> RowTray;

#include "Datum.hpp"
#include "EntWrk.hpp"
#include "KeyWrk.hpp"
#include "Key.hpp"
#include "Record.hpp"
#include "Row.hpp"
#include "RowCoder.hpp"
#include "RowEncoder.hpp"
#include "RowDecoder.hpp"
#include "RowWrk.hpp"

#include "SkipRoots.hpp"
#include "TablePtn.hpp"
#include "EntryBuffer.hpp"
#include "RowBuffer.hpp"
#include "IndexTree.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
