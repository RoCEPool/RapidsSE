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
  MN_CLASSIC_1B = 0b1001, // 9: classical era 1B
  MN_CLASSIC_2B = 0b1010, // A: classical era 2B
  MN_SILVER_1B  = 0b1011, // B: silver era 1B
  MN_SILVER_2B  = 0b1100, // C: silver era 2B
  MN_MODERN_1B  = 0b1101, // D: modern era 1B
  MN_MODERN_2B  = 0b1110, // E: modern era 2B

  MN_ALPHA_0 = 0b1001, // 9: m_caChr[0]
  MN_ALPHA_1 = 0b1010, // A: m_caChr[1]
  MN_ALPHA_2 = 0b1011, // B: m_caChr[2]
  MN_ALPHA_3 = 0b1100, // C: m_caChr[3]
  MN_ALPHA_4 = 0b1101, // D: m_caChr[4]
  MN_ALPHA_5 = 0b1110, // E: m_caChr[5]
  
  MN_DAYS_PLUS_1B    = 0b1001, // 9: +days 1B
  MN_DAYS_MINUS_1B   = 0b1010, // A: -days 1B
  MN_DAYS_CLASSIC_2B = 0b1011, // B: oops, its actually a classic era 2B
  MN_DAYS_SILVER_2B  = 0b1100, // C: oops, its actually a silver era 2B
  MN_DAYS_MODERN_2B  = 0b1101, // D: oops, its actually a modern era 2B

  MN_DIGIT_0  = 0b0001, // 1: digit 0
  MN_DIGIT_1  = 0b0010, // 2: digit 1
  MN_DIGIT_2  = 0b0011, // 3: digit 2
  MN_DIGIT_3  = 0b0100, // 4: digit 3
  MN_DIGIT_4  = 0b0101, // 5: digit 4
  MN_DIGIT_5  = 0b0110, // 6: digit 5
  MN_DIGIT_6  = 0b0111, // 7: digit 6
  MN_DIGIT_7  = 0b1000, // 8: digit 7
  MN_DIGIT_1B = 0b1001, // 9: data words contain 1B datum
  MN_DIGIT_2B = 0b1010, // A: data words contain 2B datum
  MN_DIGIT_3B = 0b1011, // B: data words contain 3B datum
  MN_DIGIT_4B = 0b1100, // C: data words contain 4B datum
  MN_DIGIT_6B = 0b1101, // D: data words contain 6B datum
  MN_DIGIT_8B = 0b1110, // E: data words contain 8B datum
    
  MN_DELTA_PLUS_0 = 0b1001, // 9: 0
  MN_DELTA_PLUS_1 = 0b1010, // A: 1
  MN_DELTA_PLUS_2 = 0b1011, // B: 2
  MN_DELTA_PLUS_3 = 0b1100, // C: 3
  MN_DELTA_PLUS_4 = 0b1101, // D: 4
  MN_DELTA_PLUS_5 = 0b1110, // E: 5
  
  MN_DELTA_MINUS_0 = 0b1001, // 9: 0
  MN_DELTA_MINUS_1 = 0b1010, // A: 1
  MN_DELTA_MINUS_2 = 0b1011, // B: 2
  MN_DELTA_MINUS_3 = 0b1100, // C: 3
  MN_DELTA_MINUS_4 = 0b1101, // D: 4
  MN_DELTA_MINUS_5 = 0b1110, // E: 5
  
  MN_TENTH_0  = 0b0001, // 1: tenth 0.00
  MN_TENTH_1  = 0b0010, // 2: tenth 0.01
  MN_TENTH_2  = 0b0011, // 3: tenth 0.02
  MN_TENTH_3  = 0b0100, // 4: tenth 0.03
  MN_TENTH_4  = 0b0101, // 5: tenth 0.04
  MN_TENTH_5  = 0b0110, // 6: tenth 0.05
  MN_TENTH_6  = 0b0111, // 7: tenth 0.06
  MN_TENTH_7  = 0b1000, // 8: tenth 0.07
  MN_TENTH_8  = 0b1001, // 9: tenth 0.08
  MN_TENTH_9  = 0b1010, // A: tenth 0.09
  MN_TENTH_10 = 0b1011, // B: tenth 1.00
  MN_TENTH_2B = 0b1100, // C: data words contain 2B datum
  MN_TENTH_4B = 0b1101, // D: data words contain 4B datum
  MN_TENTH_8B = 0b1110, // E: data words contain 8B datum
  
  MN_MINUS_ONE = 0b1001,  // 9: == -1 (no bytes in data words)
  MN_ZERO      = 0b1010,  // A: == 0 datum (no bytes in data words)
  MN_PLUS_ONE  = 0b1011,  // B: == 1 datum (no bytes in data words)
  MN_SAME      = 0b1100,  // C: same as aux int (no bytes in data words)
  
  MN_PLUS_1B   = 0b1101,  // D: aux int+1B
  MN_PLUS_2B   = 0b1110,  // E: aux int+1B

  MN_MINUS_1B  = 0b1101,  // D: aux int-1B
  MN_MINUS_2B  = 0b1110,  // E: aux int-1B

  MN_EMPTY       = 0b1011,  // B: empty string (no bytes in head)
  MN_SHOCO_FIT   = 0b1100,  // C: head: 1Blen,shoco
  MN_SHOCO_SPLIT = 0b1101,  // D: head: 2B->tail,1Blen,part-shoco till end of head
                            //    tail: has remaining shoco
  MN_SHOCO_2B    = 0b1110,  // E: head: 2B->tail,
                            //    tail: has 1Blen,shoco
  
  MN_OVERFLOW = 0b1111, // F: data words has 2B indicating overflow cacheline
  };
  
  //! we'll start with 3 era's and see what happens
  static const i8 C_CLASSIC_ERA  =  0; // 1900
  static const i8 C_SILVER_ERA   = 92; // 1992
  static const i8 C_MODERN_ERA   = 99; // 1999

  static const i8 C_CLASSIC_DAYS =  0 * 365; // 1900
  static const i8 C_SILVER_DAYS  = 92 * 365; // 1992
  static const i8 C_MODERN_DAYS  = 99 * 365; // 1999

  class Record;
}

using namespace Data;

//------------------------------------------------------------------------------

#include "SpaceUnit.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

static const i8 C_MAX_ROWS = (C_SIZE_LARGE_OBJECT - C_1KB) / sizeof(HeadLine);
static const i8 C_MAX_COLUMNS = 150;

#include "Coda.hpp"

typedef HV<Record,CN_RECORD> HV_Rec;

//typedef ArrayNL_t<CN_EDMARRAY,eDM,256> EdmAry;

typedef Nub_t<CN_ROWTRAY,HeadLine,TailLine,SpaceUnit,Table,Coda> RowNub;

typedef TrayNL_t<CN_ROWTRAY,HeadLine,TailLine,SpaceUnit,Table,Coda,
                (C_CACHELINES * sizeof(HeadLine))> RowParts;

typedef TrayNL_t<CN_ROWTRAY,HeadLine,TailLine,SpaceUnit,Table,Coda,
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
