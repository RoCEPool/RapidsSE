/*============================================================================*/
/* MetaCode.h -- Meta Codes for Row Coder Mark V                              */
/*============================================================================*/
#pragma once

namespace Rows
{
  enum eMC : u1 // meta code
  {
  /* 00 */ MC_NULL = 0,    // null datum (no bytes in data code )
  
  /* 01 */ MC_1B,          // data code contains 1B datum
  /* 02 */ MC_2B,          // data code contains 2B datum
  /* 03 */ MC_3B,          // data code contains 3B datum
  /* 04 */ MC_4B,          // data code contains 4B datum
  /* 05 */ MC_5B,          // data code contains 5B datum
  /* 06 */ MC_6B,          // data code contains 6B datum
  /* 07 */ MC_7B,          // data code contains 7B datum
  /* 08 */ MC_8B,          // data code contains 8B datum
  
  /* 09 */ MC_MINUS_ONE,   // -1 (no bytes in data code)
  /* 10 */ MC_ZERO,        // 0  (no bytes in data code)
  /* 11 */ MC_ONE,         // 1  (no bytes in data code)
  /* 12 */ MC_TWO,         // 2  (no bytes in data code)
  /* 13 */ MC_THREE,       // 3  (no bytes in data code)
  /* 14 */ MC_FOUR,        // 4  (no bytes in data code)
  /* 15 */ MC_FIVE,        // 5  (no bytes in data code)
  /* 16 */ MC_SIX,         // 6  (no bytes in data code)
  /* 17 */ MC_SEVEN,       // 7  (no bytes in data code)
  /* 18 */ MC_EIGHT,       // 8  (no bytes in data code)
  /* 19 */ MC_NINE,        // 9  (no bytes in data code)
  /* 20 */ MC_TEN,         // 10 (no bytes in data code)
  
  /* 21 */ MC_BOL_FALSE,   // boolean false (no bytes in data code)
  /* 22 */ MC_BOL_TRUE,    // boolean true  (no bytes in data code)
  
  /* 23 */ MC_DTE_E0_2B,   // date within era 0 expressed as 2B in data code
  /* 24 */ MC_DTE_E1_2B,   // date within era 1 expressed as 2B in data code
  /* 25 */ MC_DTE_E2_2B,   // date within era 2 expressed as 2B in data code
  
  /* 26 */ MC_STR_EMPTY,   // empty string (no bytes in data code)
  /* 27 */ MC_STR_FIT_1L,  // ascii string: 1B=len, then ascii with null term
  /* 28 */ MC_STR_FIT_2L,  // ascii string: 2B=len, then ascii with null term
  /* 29 */ MC_STR_TAIL_2T, // ascii string in tail:
                           //   head: 2B->tail
                           //   tail: has 1B=len, then ascii with null term
  
  /* 30 */ MC_OVERFLOW_2T, // overflow tail: 2B->overflow tail
  
  /* 31 */ MC_ENUMS,
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
  
  /* 21 */ "MC_BOL_FALSE"_Stz,
  /* 22 */ "MC_BOL_TRUE"_Stz,
  
  /* 23 */ "MC_DTE_E0_2B"_Stz,
  /* 24 */ "MC_DTE_E1_2B"_Stz,
  /* 25 */ "MC_DTE_E2_2B"_Stz,
  
  /* 26 */ "MC_STR_EMPTY"_Stz,
  /* 27 */ "MC_STR_FIT_1L"_Stz,
  /* 29 */ "MC_STR_FIT_2L"_Stz,
  /* 29 */ "MC_STR_TAIL_2T"_Stz,

  /* 30 */ "MC_OVERFLOW_2T"_Stz,
  
  /* 31 */ "MC_ENUMS"_Stz,
  };
  
  static i8 nMC[] = // size of meta code
  {
  /* 00 */ 0, // MC_NULL         // null datum (no bytes in data code )
  
  /* 01 */ 1, // MC_1B,          // data code contains 1B datum
  /* 02 */ 2, // MC_2B,          // data code contains 2B datum
  /* 03 */ 3, // MC_3B,          // data code contains 3B datum
  /* 04 */ 4, // MC_4B,          // data code contains 4B datum
  /* 05 */ 5, // MC_5B,          // data code contains 5B datum
  /* 06 */ 6, // MC_6B,          // data code contains 6B datum
  /* 07 */ 7, // MC_7B,          // data code contains 7B datum
  /* 08 */ 8, // MC_8B,          // data code contains 8B datum
  
  /* 09 */ 0, // MC_MINUS_ONE,   // -1 (no bytes in data code)
  /* 10 */ 0, // MC_ZERO,        // 0  (no bytes in data code)
  /* 11 */ 0, // MC_ONE,         // 1  (no bytes in data code)
  /* 12 */ 0, // MC_TWO,         // 2  (no bytes in data code)
  /* 13 */ 0, // MC_THREE,       // 3  (no bytes in data code)
  /* 14 */ 0, // MC_FOUR,        // 4  (no bytes in data code)
  /* 15 */ 0, // MC_FIVE,        // 5  (no bytes in data code)
  /* 16 */ 0, // MC_SIX,         // 6  (no bytes in data code)
  /* 17 */ 0, // MC_SEVEN,       // 7  (no bytes in data code)
  /* 18 */ 0, // MC_EIGHT,       // 8  (no bytes in data code)
  /* 19 */ 0, // MC_NINE,        // 9  (no bytes in data code)
  /* 20 */ 0, // MC_TEN,         // 10 (no bytes in data code)
  
  /* 21 */ 0, // MC_BOL_FALSE,   // boolean false (no bytes in data code)
  /* 22 */ 0, // MC_BOL_TRUE,    // boolean true  (no bytes in data code)
  
  /* 23 */ 2, // MC_DTE_E0_2B,   // date within era 0 expressed as 2B in data code
  /* 24 */ 2, // MC_DTE_E1_2B,   // date within era 1 expressed as 2B in data code
  /* 25 */ 2, // MC_DTE_E2_2B,   // date within era 2 expressed as 2B in data code
  
  /* 26 */ 0, // MC_STR_EMPTY,   // empty string (no bytes in data code)
  /* 27 */ 1, // MC_STR_FIT_1L,  // ascii string: 1B=len, then ascii with null term
  /* 28 */ 2, // MC_STR_FIT_2L,  // ascii string: 2B=len, then ascii with null term
  /* 29 */ 2, // MC_STR_TAIL_2T, // longer ascii string:
                                 //   head: 2B->tail
                                 //   tail: has 1B=len, then ascii with null term
  
  /* 30 */ 2, // MC_OVERFLOW_2T, // overflow tail (2B->overflow tail)
  
  /* 53 */ 0  // MC_ENUMS,
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
