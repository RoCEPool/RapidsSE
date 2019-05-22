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
  
  MN_FALSE = 0b0001,      // 1: false datum (no bytes in data words)
  MN_TRUE  = 0b0010,      // 2: true datum (no bytes in data words)
  
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  enum eMC : u1 // meta code
  {
  /* 00 */ MC_NULL = 0,    // null datum (no bytes in data area)
  
  /* 01 */ MC_1B,          // data area contains 1B datum
  /* 02 */ MC_2B,          // data area contains 2B datum
  /* 03 */ MC_3B,          // data area contains 3B datum
  /* 04 */ MC_4B,          // data area contains 4B datum
  /* 05 */ MC_5B,          // data area contains 5B datum
  /* 06 */ MC_6B,          // data area contains 6B datum
  /* 07 */ MC_7B,          // data area contains 7B datum
  /* 08 */ MC_8B,          // data area contains 8B datum
  
  /* 09 */ MC_MINUS_ONE,   // -1 (no bytes in data area)
  /* 10 */ MC_ZERO,        // 0  (no bytes in data area)
  /* 11 */ MC_ONE,         // 1  (no bytes in data area)
  /* 12 */ MC_TWO,         // 2  (no bytes in data area)
  /* 13 */ MC_THREE,       // 3  (no bytes in data area)
  /* 14 */ MC_FOUR,        // 4  (no bytes in data area)
  /* 15 */ MC_FIVE,        // 5  (no bytes in data area)
  /* 16 */ MC_SIX,         // 6  (no bytes in data area)
  /* 17 */ MC_SEVEN,       // 7  (no bytes in data area)
  /* 18 */ MC_EIGHT,       // 8  (no bytes in data area)
  /* 19 */ MC_NINE,        // 9  (no bytes in data area)
  /* 20 */ MC_TEN,         // 10 (no bytes in data area)
  
  /* 21 */ MC_SAME,        // same as previous (no bytes in data area)
  /* 22 */ MC_ADD_1B,      // previous + 1B in data area
  /* 23 */ MC_ADD_2B,      // previous + 2B in data area
  /* 24 */ MC_SUB_1B,      // previous - 1B in data area
  /* 25 */ MC_SUB_2B,      // previous - 2B in data area
  
  /* 26 */ MC_FALSE,       // false (no bytes in data area)
  /* 27 */ MC_TRUE,        // true  (no bytes in data area)
  
  /* 28 */ MC_DATE_E0_2B,  // date in era 0 expressed as 2B in data area
  /* 29 */ MC_DATE_E1_2B,  // date in era 1 expressed as 2B in data area
  /* 30 */ MC_DATE_E2_2B,  // date in era 2 expressed as 2B in data area

  /* 31 */ MC_DAYS_ADD_1B, // first date +days 1B in data area
  /* 32 */ MC_DAYS_SUB_1B, // first date -days 1B in data area
  
  /* 33 */ MC_STR_EMPTY,   // empty string (no bytes in data area)
  /* 34 */ MC_STR_FIT_1L,  // ascii string: 1B=len, then ascii
  /* 35 */ MC_STR_2T,      // longer ascii string:
                           //    head: 2B->tail
                           //    tail: has 1B=len, then ascii
  /* 36 */ MC_STR_SPLIT_2T1L,// ascii string:
                             //   head: 2B->tail, 1B=len, then part-ascii
                             //   tail: has remaining ascii
  
  /* 37 */ MC_BSTR_FIT_2L,   // bigger ascii string: 2B=len then ascii
  /* 38 */ MC_BSTR_2T,       // bigger ascii string:
                             //    head: 2B->tail
                             //    tail: has 2B=len, then ascii
  /* 39 */ MC_BSTR_SPLIT_2T1L, // bigger ascii string:
                               //   head: 2B->tail, 2B=len, part-ascii
                               //   tail: has remaining ascii
  
  /* 40 */ MC_SHOCO_FIT_1L,    // shoco encoded string: 1B=len, then shoco
  /* 41 */ MC_SHOCO_2T,        // shoco encoded string:
                               //    head: 2B->tail
                               //    tail: has 1B=len, then shoco
  /* 42 */ MC_SHOCO_SPLIT_2T1L,// shoco encoded string:
                               //   head: 2B->tail, 1B=len, part-shoco
                               //   tail: has remaining shoco

  /* 43 */ MC_TWOS_1B,         // data area contains 1B two's complement datum
  /* 44 */ MC_TWOS_2B,         // data area contains 2B two's complement datum
  /* 45 */ MC_TWOS_3B,         // data area contains 3B two's complement datum
  /* 46 */ MC_TWOS_4B,         // data area contains 4B two's complement datum
  /* 47 */ MC_TWOS_5B,         // data area contains 5B two's complement datum
  /* 48 */ MC_TWOS_6B,         // data area contains 6B two's complement datum
  /* 49 */ MC_TWOS_7B,         // data area contains 7B two's complement datum
  
  /* 50 */ MC_OVERFLOW_2B,     // 2B in data area indicating overflow tail
  
  /* 51 */ MC_ENUMS,
  };
  
  static Stz* aMC[]
  {
  /* 00 */ "MC_NULL"_Stz,
  /* 01 */ "MC_1B"_Stz,
  /* 02 */ "MC_2B"_Stz,
  /* 03 */ "MC_3B"_Stz,
  /* 04 */ "MC_4B"_Stz,
  /* 05 */ "MC_5B"_Stz,
  /* 06 */ "MC_6B"_Stz,
  /* 07 */ "MC_7B"_Stz,
  /* 08 */ "MC_8B"_Stz,
  /* 09 */ "MC_MINUS_ONE"_Stz,
  /* 10 */ "MC_ZERO"_Stz,
  /* 11 */ "MC_ONE"_Stz,
  /* 12 */ "MC_TWO"_Stz,
  /* 13 */ "MC_THREE"_Stz,
  /* 14 */ "MC_FOUR"_Stz,
  /* 15 */ "MC_FIVE"_Stz,
  /* 16 */ "MC_SIX"_Stz,
  /* 17 */ "MC_SEVEN"_Stz,
  /* 18 */ "MC_EIGHT"_Stz,
  /* 19 */ "MC_NINE"_Stz,
  /* 20 */ "MC_TEN"_Stz,
  /* 21 */ "MC_SAME"_Stz,
  /* 22 */ "MC_ADD_1B"_Stz,
  /* 23 */ "MC_ADD_2B"_Stz,
  /* 24 */ "MC_SUB_1B"_Stz,
  /* 25 */ "MC_SUB_2B"_Stz,
  /* 26 */ "MC_FALSE"_Stz,
  /* 27 */ "MC_TRUE"_Stz,
  /* 28 */ "MC_DATE_E0_2B"_Stz,
  /* 29 */ "MC_DATE_E1_2B"_Stz,
  /* 30 */ "MC_DATE_E2_2B"_Stz,
  /* 31 */ "MC_DAYS_ADD_1B"_Stz,
  /* 32 */ "MC_DAYS_SUB_1B"_Stz,
  /* 33 */ "MC_STR_EMPTY"_Stz,
  /* 34 */ "MC_STR_FIT_1L"_Stz,
  /* 35 */ "MC_STR_2T"_Stz,
  /* 36 */ "MC_STR_SPLIT_2T1L"_Stz,
  /* 37 */ "MC_BSTR_FIT_2L"_Stz,
  /* 38 */ "MC_BSTR_2T"_Stz,
  /* 39 */ "MC_BSTR_SPLIT_2T1L"_Stz,
  /* 40 */ "MC_SHOCO_FIT_1L"_Stz,
  /* 41 */ "MC_SHOCO_2T"_Stz,
  /* 42 */ "MC_SHOCO_SPLIT_2T1L"_Stz,
  /* 43 */ "MC_TWOS_1B"_Stz,
  /* 44 */ "MC_TWOS_2B"_Stz,
  /* 45 */ "MC_TWOS_3B"_Stz,
  /* 46 */ "MC_TWOS_4B"_Stz,
  /* 47 */ "MC_TWOS_5B"_Stz,
  /* 48 */ "MC_TWOS_6B"_Stz,
  /* 49 */ "MC_TWOS_7B"_Stz,
  /* 50 */ "MC_OVERFLOW_2B"_Stz,
  /* 51 */ "MC_ENUMS"_Stz,
  };
  
  static i8 nMC[] = // size of meta code
  {
  /* 00 */ 0, // MC_NULL         // null datum (no bytes in data area)
  
  /* 01 */ 1, // MC_1B,          // data area contains 1B datum
  /* 02 */ 2, // MC_2B,          // data area contains 2B datum
  /* 03 */ 3, // MC_3B,          // data area contains 3B datum
  /* 04 */ 4, // MC_4B,          // data area contains 4B datum
  /* 05 */ 5, // MC_5B,          // data area contains 5B datum
  /* 06 */ 6, // MC_6B,          // data area contains 6B datum
  /* 07 */ 7, // MC_7B,          // data area contains 7B datum
  /* 08 */ 8, // MC_8B,          // data area contains 8B datum
  
  /* 09 */ 0, // MC_MINUS_ONE,   // -1 (no bytes in data area)
  /* 10 */ 0, // MC_ZERO,        // 0  (no bytes in data area)
  /* 11 */ 0, // MC_ONE,         // 1  (no bytes in data area)
  /* 12 */ 0, // MC_TWO,         // 2  (no bytes in data area)
  /* 13 */ 0, // MC_THREE,       // 3  (no bytes in data area)
  /* 14 */ 0, // MC_FOUR,        // 4  (no bytes in data area)
  /* 15 */ 0, // MC_FIVE,        // 5  (no bytes in data area)
  /* 16 */ 0, // MC_SIX,         // 6  (no bytes in data area)
  /* 17 */ 0, // MC_SEVEN,       // 7  (no bytes in data area)
  /* 18 */ 0, // MC_EIGHT,       // 8  (no bytes in data area)
  /* 19 */ 0, // MC_NINE,        // 9  (no bytes in data area)
  /* 20 */ 0, // MC_TEN,         // 10 (no bytes in data area)
  
  /* 21 */ 0, // MC_SAME,        // same as previous (no bytes in data area)
  /* 22 */ 1, // MC_ADD_1B,      // previous + 1B in data area
  /* 23 */ 2, // MC_ADD_2B,      // previous + 2B in data area
  /* 24 */ 1, // MC_SUB_1B,      // previous - 1B in data area
  /* 25 */ 2, // MC_SUB_2B,      // previous - 2B in data area
  
  /* 26 */ 0, // MC_FALSE,       // false (no bytes in data area)
  /* 27 */ 0, // MC_TRUE,        // true  (no bytes in data area)
  
  /* 28 */ 2, // MC_DATE_E0_2B,  // date in era 0 expressed as 2B in data area
  /* 29 */ 2, // MC_DATE_E1_2B,  // date in era 1 expressed as 2B in data area
  /* 30 */ 2, // MC_DATE_E2_2B,  // date in era 2 expressed as 2B in data area
  
  /* 31 */ 1, // MC_DAYS_ADD_1B, // first date +days 1B in data area
  /* 32 */ 2, // MC_DAYS_SUB_1B, // first date -days 1B in data area
  
  /* 33 */ 0, // MC_STR_EMPTY,   // empty string (no bytes in data area)
  /* 34 */ 1, // MC_STR_FIT_1L,  // ascii string: 1B=len, then ascii
  /* 35 */ 2, // MC_STR_2T,      // longer ascii string:
                                 //    head: 2B->tail
                                 //    tail: has 1B=len, then ascii
  /* 36 */ 3, // MC_STR_SPLIT_2T1L,// ascii string:
                                   //   head: 2B->tail, 1B=len, then part-ascii
                                   //   tail: has remaining ascii
  
  /* 37 */ 2, // MC_BSTR_FIT_2L,   // bigger ascii string: 2B=len then ascii
  /* 38 */ 2, // MC_BSTR_2T,       // bigger ascii string:
                                   //    head: 2B->tail
                                   //    tail: has 2B=len, then ascii
  /* 39 */ 3, // MC_BSTR_SPLIT_2T1L, // bigger ascii string:
                                     //   head: 2B->tail, 2B=len, part-ascii
                                     //   tail: has remaining ascii
  
  /* 40 */ 1, // MC_SHOCO_FIT_1L,    // shoco encoded string: 1B=len, then shoco
  /* 41 */ 2, // MC_SHOCO_2T,        // shoco encoded string:
                                     //    head: 2B->tail
                                     //    tail: has 1B=len, then shoco
  /* 42 */ 3, // MC_SHOCO_SPLIT_2T1L,// shoco encoded string:
                                     //   head: 2B->tail, 1B=len, part-shoco
                                     //   tail: has remaining shoco
  
  /* 43 */ 1, // MC_TWOS_1B,         // data area contains 1B two's complement datum
  /* 44 */ 2, // MC_TWOS_2B,         // data area contains 2B two's complement datum
  /* 45 */ 3, // MC_TWOS_3B,         // data area contains 3B two's complement datum
  /* 46 */ 4, // MC_TWOS_4B,         // data area contains 4B two's complement datum
  /* 47 */ 5, // MC_TWOS_5B,         // data area contains 5B two's complement datum
  /* 48 */ 6, // MC_TWOS_6B,         // data area contains 6B two's complement datum
  /* 49 */ 7, // MC_TWOS_7B,         // data area contains 7B two's complement datum
  
  /* 50 */ 2, // MC_OVERFLOW_2B,     // 2B in data area indicating overflow tail
  
  /* 51 */ 0  // MC_ENUMS,
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

#include "Datum.hpp"

#include "Line.hpp"
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

#include "EntWrk.hpp"
#include "Extractor.hpp"
#include "KeyWrk.hpp"
#include "Key.hpp"
#include "Record.hpp"
#include "SkipRoots.hpp"

#include "RowBuffer.hpp"
#include "RowCoder.hpp"
#include "TablePtn.hpp"
#include "EntryBuffer.hpp"
#include "IndexTree.hpp"

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
