/*============================================================================*/
/* Data.h -- Data namespace                                                   */
/*============================================================================*/
#pragma once

#include "MetaData.h"

namespace Data
{
  static const i8 C_CACHELINES = 256;
  
  void vCheckAssumptions(void);

  static const i8 C_FLAT_LEVELS_MAX   = 16;
  static const i8 C_FLAT_OFFSETS_MAX  = C_FLAT_LEVELS_MAX - 1;
  static const i8 C_FLAT_FREQ         = 2;
  static const i8 C_MAX_INDEX_COLUMNS = 16;
  
  static const i8 C_MAX_ROWS = (C_SIZE_LARGE_OBJECT - C_1KB) / C_BYTES_PER_CACHE_LINE;
  
  enum eMN : u1 // metadata nibbles
  {
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //! warning
  //! warning : do not define values which override this set
  //! warning

  MN_NULL = 0b0000,       // 0: null datum (no bytes in data words)
  
  MN_1B   = 0b0001,       // 1: data words contain 1B datum
  MN_2B   = 0b0010,       // 2: data words contain 2B datum
  MN_3B   = 0b0011,       // 3: data words contain 3B datum
  MN_4B   = 0b0100,       // 4: data words contain 4B datum
  MN_5B   = 0b0101,       // 5: data words contain 5B datum
  MN_6B   = 0b0110,       // 6: data words contain 6B datum
  MN_7B   = 0b0111,       // 7: data words contain 7B datum
  MN_8B   = 0b1000,       // 8: data words contain 8B datum
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  MN_FALSE = 0b1101,      // D: false datum (no bytes in data words)
  MN_TRUE  = 0b1110,      // E: true datum (no bytes in data words)
  
  // the following are to be renamed
  MN_CLASSICAL_1B = 0b1001, // 9: classical era 1B
  MN_CLASSICAL_2B = 0b1010, // A: classical era 2B
  MN_SILVER_1B    = 0b1011, // B: silver era 1B
  MN_SILVER_2B    = 0b1100, // C: silver era 2B
  MN_MODERN_1B    = 0b1101, // D: modern era 1B
  MN_MODERN_2B    = 0b1110, // E: modern era 2B

  // the following are to be renamed
  MN_ALPHA_0      = 0b1001, // 9: m_caChr[0]
  MN_ALPHA_1      = 0b1010, // A: m_caChr[1]
  MN_ALPHA_2      = 0b1011, // B: m_caChr[2]
  MN_ALPHA_3      = 0b1100, // C: m_caChr[3]
  MN_ALPHA_4      = 0b1101, // D: m_caChr[4]
  MN_ALPHA_5      = 0b1110, // E: m_caChr[5]

  MN_MINUS_ONE = 0b1100,  // C: -1 datum (no bytes in data words)
  MN_ZERO      = 0b1101,  // D: 0 datum (no bytes in data words)
  MN_PLUS_ONE  = 0b1110,  // E: 1 datum (no bytes in data words)
  
  MN_REV_2B = 0b1001, // 9: reverse 2B
  MN_REV_3B = 0b1010, // A: reverse 3B
  MN_REV_4B = 0b1011, // B: reverse 4B
  MN_REV_5B = 0b1100, // C: reverse 5B
  MN_REV_6B = 0b1101, // D: reverse 6B
  MN_REV_7B = 0b1110, // E: reverse 7B

  MN_EMPTY     = 0b1100,  // C: empty string
  MN_SHOCO_FIT = 0b1101,  // D: len+shoco in data words
  MN_SHOCO_2B  = 0b1110,  // E: 2B in data words indicating shoco overflow
  
  //! the following are to be renamed
  MN_COLTAIL_V5 = 0b1001, // 9: classical era 1B
  MN_COLTAIL_V4 = 0b1010, // A: classical era 2B
  MN_COLTAIL_V3 = 0b1011, // B: silver era 1B
  MN_COLTAIL_V2 = 0b1100, // C: silver era 2B
  MN_COLTAIL_V1 = 0b1101, // D: modern era 1B
  MN_COLTAIL_V0 = 0b1110, // E: modern era 2B

  MN_OVERFLOW  = 0b1111,  // F: data words has 2B indicating overflow cacheline
  };
  
  //! we'll start with 3 era's and see what happens
  static const i8 C_CLASSICAL_ERA  =  0; // 1900
  static const i8 C_SILVER_ERA     = 92; // 1992
  static const i8 C_MODERN_ERA     = 99; // 1999

  static const i8 C_CLASSICAL_DAYS =  0 * 365; // 1900
  static const i8 C_SILVER_DAYS    = 92 * 365; // 1992
  static const i8 C_MODERN_DAYS    = 99 * 365; // 1999

  class Record;
}

using namespace Data;

//------------------------------------------------------------------------------

#include "HeadLine.hpp"
#include "TailLine.hpp"
#include "Coda.hpp"


typedef HV<Record,CN_RECORD> HV_Rec;

//typedef ArrayNL_t<CN_EDMARRAY,eDM,256> EdmAry;

typedef Nub_t<CN_ROWTRAY,HeadLine,TailLine,DoubleWord,Table,Coda> RowNub;

typedef TrayNL_t<CN_ROWTRAY,HeadLine,TailLine,DoubleWord,Table,Coda,
                (C_CACHELINES * sizeof(HeadLine))> RowParts;

typedef TrayNL_t<CN_ROWTRAY,HeadLine,TailLine,DoubleWord,Table,Coda,
                 (C_MAX_ROWS * sizeof(HeadLine))> RowTray;

#include "Datum.hpp"
#include "EntWrk.hpp"
#include "KeyWrk.hpp"
#include "Key.hpp"
#include "Record.hpp"
#include "RowCoder.hpp"
#include "SkipRoots.hpp"

#include "TablePtn.hpp"
#include "EntryBuffer.hpp"
#include "RowBuffer.hpp"
#include "IndexTree.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
