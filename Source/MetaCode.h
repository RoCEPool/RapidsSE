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
  
  /* 01 */ MC_BCD_1B,      // data code contains 1B datum
  /* 02 */ MC_BCD_2B,      // data code contains 2B datum
  /* 03 */ MC_BCD_3B,      // data code contains 3B datum
  /* 04 */ MC_BCD_4B,      // data code contains 4B datum
  /* 05 */ MC_BCD_5B,      // data code contains 5B datum
  /* 06 */ MC_BCD_6B,      // data code contains 6B datum
  /* 07 */ MC_BCD_7B,      // data code contains 7B datum
  /* 08 */ MC_BCD_8B,      // data code contains 8B datum
  
  /* 09 */ MC_BCD_M_ONE,   // -1 (no bytes in data code)
  /* 10 */ MC_BCD_ZERO,    // 0  (no bytes in data code)
  /* 11 */ MC_BCD_ONE,     // 1  (no bytes in data code)
  /* 12 */ MC_BCD_TWO,     // 2  (no bytes in data code)
  /* 13 */ MC_BCD_THREE,   // 3  (no bytes in data code)
  /* 14 */ MC_BCD_FOUR,    // 4  (no bytes in data code)
  /* 15 */ MC_BCD_FIVE,    // 5  (no bytes in data code)
  /* 16 */ MC_BCD_SIX,     // 6  (no bytes in data code)
  /* 17 */ MC_BCD_SEVEN,   // 7  (no bytes in data code)
  /* 18 */ MC_BCD_EIGHT,   // 8  (no bytes in data code)
  /* 19 */ MC_BCD_NINE,    // 9  (no bytes in data code)
  /* 20 */ MC_BCD_TEN,     // 10 (no bytes in data code)
  
  /* 21 */ MC_BOL_FALSE,   // boolean false (no bytes in data code)
  /* 22 */ MC_BOL_TRUE,    // boolean true  (no bytes in data code)
  
  /* 23 */ MC_DTE_E0_2B,   // date within era 0 expressed as 2B in data code
  /* 24 */ MC_DTE_E1_2B,   // date within era 1 expressed as 2B in data code
  /* 25 */ MC_DTE_E2_2B,   // date within era 2 expressed as 2B in data code
  
  /* 01 */ MC_FLT_1B,      // data code contains 1B datum
  /* 02 */ MC_FLT_2B,      // data code contains 2B datum
  /* 03 */ MC_FLT_3B,      // data code contains 3B datum
  /* 04 */ MC_FLT_4B,      // data code contains 4B datum
  /* 05 */ MC_FLT_5B,      // data code contains 5B datum
  /* 06 */ MC_FLT_6B,      // data code contains 6B datum
  /* 07 */ MC_FLT_7B,      // data code contains 7B datum
  /* 08 */ MC_FLT_8B,      // data code contains 8B datum
  
  /* 01 */ MC_INT_1B,      // data code contains 1B datum
  /* 02 */ MC_INT_2B,      // data code contains 2B datum
  /* 03 */ MC_INT_3B,      // data code contains 3B datum
  /* 04 */ MC_INT_4B,      // data code contains 4B datum
  /* 05 */ MC_INT_5B,      // data code contains 5B datum
  /* 06 */ MC_INT_6B,      // data code contains 6B datum
  /* 07 */ MC_INT_7B,      // data code contains 7B datum
  /* 08 */ MC_INT_8B,      // data code contains 8B datum
  
  /* 09 */ MC_INT_M_ONE,   // -1 (no bytes in data code)
  /* 10 */ MC_INT_ZERO,    // 0  (no bytes in data code)
  /* 11 */ MC_INT_ONE,     // 1  (no bytes in data code)
  /* 12 */ MC_INT_TWO,     // 2  (no bytes in data code)
  /* 13 */ MC_INT_THREE,   // 3  (no bytes in data code)
  /* 14 */ MC_INT_FOUR,    // 4  (no bytes in data code)
  /* 15 */ MC_INT_FIVE,    // 5  (no bytes in data code)
  /* 16 */ MC_INT_SIX,     // 6  (no bytes in data code)
  /* 17 */ MC_INT_SEVEN,   // 7  (no bytes in data code)
  /* 18 */ MC_INT_EIGHT,   // 8  (no bytes in data code)
  /* 19 */ MC_INT_NINE,    // 9  (no bytes in data code)
  /* 20 */ MC_INT_TEN,     // 10 (no bytes in data code)
  
  /* 01 */ MC_IVL_1B,      // data code contains 1B datum
  /* 02 */ MC_IVL_2B,      // data code contains 2B datum
  /* 03 */ MC_IVL_3B,      // data code contains 3B datum
  /* 04 */ MC_IVL_4B,      // data code contains 4B datum
  /* 05 */ MC_IVL_5B,      // data code contains 5B datum
  /* 06 */ MC_IVL_6B,      // data code contains 6B datum
  /* 07 */ MC_IVL_7B,      // data code contains 7B datum
  /* 08 */ MC_IVL_8B,      // data code contains 8B datum
  
  /* 26 */ MC_STR_EMPTY,   // empty string (no bytes in data code)
  /* 27 */ MC_STR_FIT_1L,  // ascii string: 1B=len, then ascii with null term
  /* 28 */ MC_STR_FIT_2L,  // ascii string: 2B=len, then ascii with null term
  /* 29 */ MC_STR_TAIL_2T, // ascii string in tail:
                           //   head: 2B->tail
                           //   tail: has 1B=len, then ascii with null term
  
  /* 01 */ MC_TME_1B,      // data code contains 1B datum
  /* 02 */ MC_TME_2B,      // data code contains 2B datum
  /* 03 */ MC_TME_3B,      // data code contains 3B datum
  /* 04 */ MC_TME_4B,      // data code contains 4B datum
  /* 05 */ MC_TME_5B,      // data code contains 5B datum
  /* 06 */ MC_TME_6B,      // data code contains 6B datum
  /* 07 */ MC_TME_7B,      // data code contains 7B datum
  /* 08 */ MC_TME_8B,      // data code contains 8B datum

  /* 08 */ MC_TSP_8B,      // data code contains 8B datum

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
  
  /* 01 */ "MC_BCD_1B"_Stz,
  /* 02 */ "MC_BCD_2B"_Stz,
  /* 03 */ "MC_BCD_3B"_Stz,
  /* 04 */ "MC_BCD_4B"_Stz,
  /* 05 */ "MC_BCD_5B"_Stz,
  /* 06 */ "MC_BCD_6B"_Stz,
  /* 07 */ "MC_BCD_7B"_Stz,
  /* 08 */ "MC_BCD_8B"_Stz,
  
  /* 09 */ "MC_BCD_M_ONE"_Stz,
  /* 10 */ "MC_BCD_ZERO"_Stz,
  /* 11 */ "MC_BCD_ONE"_Stz,
  /* 12 */ "MC_BCD_TWO"_Stz,
  /* 13 */ "MC_BCD_THREE"_Stz,
  /* 14 */ "MC_BCD_FOUR"_Stz,
  /* 15 */ "MC_BCD_FIVE"_Stz,
  /* 16 */ "MC_BCD_SIX"_Stz,
  /* 17 */ "MC_BCD_SEVEN"_Stz,
  /* 18 */ "MC_BCD_EIGHT"_Stz,
  /* 19 */ "MC_BCD_NINE"_Stz,
  /* 20 */ "MC_BCD_TEN"_Stz,
  
  /* 21 */ "MC_BOL_FALSE"_Stz,
  /* 22 */ "MC_BOL_TRUE"_Stz,
  
  /* 23 */ "MC_DTE_E0_2B"_Stz,
  /* 24 */ "MC_DTE_E1_2B"_Stz,
  /* 25 */ "MC_DTE_E2_2B"_Stz,
  
  /* 01 */ "MC_FLT_1B"_Stz,
  /* 02 */ "MC_FLT_2B"_Stz,
  /* 03 */ "MC_FLT_3B"_Stz,
  /* 04 */ "MC_FLT_4B"_Stz,
  /* 05 */ "MC_FLT_5B"_Stz,
  /* 06 */ "MC_FLT_6B"_Stz,
  /* 07 */ "MC_FLT_7B"_Stz,
  /* 08 */ "MC_FLT_8B"_Stz,
  
  /* 01 */ "MC_INT_1B"_Stz,
  /* 02 */ "MC_INT_2B"_Stz,
  /* 03 */ "MC_INT_3B"_Stz,
  /* 04 */ "MC_INT_4B"_Stz,
  /* 05 */ "MC_INT_5B"_Stz,
  /* 06 */ "MC_INT_6B"_Stz,
  /* 07 */ "MC_INT_7B"_Stz,
  /* 08 */ "MC_INT_8B"_Stz,
  
  /* 09 */ "MC_INT_MS_ONE"_Stz,
  /* 10 */ "MC_INT_ZERO"_Stz,
  /* 11 */ "MC_INT_ONE"_Stz,
  /* 12 */ "MC_INT_TWO"_Stz,
  /* 13 */ "MC_INT_THREE"_Stz,
  /* 14 */ "MC_INT_FOUR"_Stz,
  /* 15 */ "MC_INT_FIVE"_Stz,
  /* 16 */ "MC_INT_SIX"_Stz,
  /* 17 */ "MC_INT_SEVEN"_Stz,
  /* 18 */ "MC_INT_EIGHT"_Stz,
  /* 19 */ "MC_INT_NINE"_Stz,
  /* 20 */ "MC_INT_TEN"_Stz,
  
  /* 01 */ "MC_IVL_1B"_Stz,
  /* 02 */ "MC_IVL_2B"_Stz,
  /* 03 */ "MC_IVL_3B"_Stz,
  /* 04 */ "MC_IVL_4B"_Stz,
  /* 05 */ "MC_IVL_5B"_Stz,
  /* 06 */ "MC_IVL_6B"_Stz,
  /* 07 */ "MC_IVL_7B"_Stz,
  /* 08 */ "MC_IVL_8B"_Stz,
  
  /* 26 */ "MC_STR_EMPTY"_Stz,
  /* 27 */ "MC_STR_FIT_1L"_Stz,
  /* 29 */ "MC_STR_FIT_2L"_Stz,
  /* 29 */ "MC_STR_TAIL_2T"_Stz,

  /* 01 */ "MC_TME_1B"_Stz,
  /* 02 */ "MC_TME_2B"_Stz,
  /* 03 */ "MC_TME_3B"_Stz,
  /* 04 */ "MC_TME_4B"_Stz,
  /* 05 */ "MC_TME_5B"_Stz,
  /* 06 */ "MC_TME_6B"_Stz,
  /* 07 */ "MC_TME_7B"_Stz,
  /* 08 */ "MC_TME_8B"_Stz,
  
  /* 08 */ "MC_TSP_8B"_Stz,
  
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
  
  /* 01 */ 1, // MC_BCD_1B,      // data code contains 1B datum
  /* 02 */ 2, // MC_BCD_2B,      // data code contains 2B datum
  /* 03 */ 3, // MC_BCD_3B,      // data code contains 3B datum
  /* 04 */ 4, // MC_BCD_4B,      // data code contains 4B datum
  /* 05 */ 5, // MC_BCD_5B,      // data code contains 5B datum
  /* 06 */ 6, // MC_BCD_6B,      // data code contains 6B datum
  /* 07 */ 7, // MC_BCD_7B,      // data code contains 7B datum
  /* 08 */ 8, // MC_BCD_8B,      // data code contains 8B datum
  
  /* 09 */ 0, // MC_BCD_M_ONE,   // -1 (no bytes in data code)
  /* 10 */ 0, // MC_BCD_ZERO,    // 0  (no bytes in data code)
  /* 11 */ 0, // MC_BCD_ONE,     // 1  (no bytes in data code)
  /* 12 */ 0, // MC_BCD_TWO,     // 2  (no bytes in data code)
  /* 13 */ 0, // MC_BCD_THREE,   // 3  (no bytes in data code)
  /* 14 */ 0, // MC_BCD_FOUR,    // 4  (no bytes in data code)
  /* 15 */ 0, // MC_BCD_FIVE,    // 5  (no bytes in data code)
  /* 16 */ 0, // MC_BCD_SIX,     // 6  (no bytes in data code)
  /* 17 */ 0, // MC_BCD_SEVEN,   // 7  (no bytes in data code)
  /* 18 */ 0, // MC_BCD_EIGHT,   // 8  (no bytes in data code)
  /* 19 */ 0, // MC_BCD_NINE,    // 9  (no bytes in data code)
  /* 20 */ 0, // MC_BCD_TEN,     // 10 (no bytes in data code)
  
  /* 21 */ 0, // MC_BOL_FALSE,   // boolean false (no bytes in data code)
  /* 22 */ 0, // MC_BOL_TRUE,    // boolean true  (no bytes in data code)
  
  /* 23 */ 2, // MC_DTE_E0_2B,   // date within era 0 expressed as 2B in data code
  /* 24 */ 2, // MC_DTE_E1_2B,   // date within era 1 expressed as 2B in data code
  /* 25 */ 2, // MC_DTE_E2_2B,   // date within era 2 expressed as 2B in data code
  
  /* 01 */ 1, // MC_FLT_1B,      // data code contains 1B datum
  /* 02 */ 2, // MC_FLT_2B,      // data code contains 2B datum
  /* 03 */ 3, // MC_FLT_3B,      // data code contains 3B datum
  /* 04 */ 4, // MC_FLT_4B,      // data code contains 4B datum
  /* 05 */ 5, // MC_FLT_5B,      // data code contains 5B datum
  /* 06 */ 6, // MC_FLT_6B,      // data code contains 6B datum
  /* 07 */ 7, // MC_FLT_7B,      // data code contains 7B datum
  /* 08 */ 8, // MC_FLT_8B,      // data code contains 8B datum
  
  /* 01 */ 1, // MC_INT_1B,      // data code contains 1B datum
  /* 02 */ 2, // MC_INT_2B,      // data code contains 2B datum
  /* 03 */ 3, // MC_INT_3B,      // data code contains 3B datum
  /* 04 */ 4, // MC_INT_4B,      // data code contains 4B datum
  /* 05 */ 5, // MC_INT_5B,      // data code contains 5B datum
  /* 06 */ 6, // MC_INT_6B,      // data code contains 6B datum
  /* 07 */ 7, // MC_INT_7B,      // data code contains 7B datum
  /* 08 */ 8, // MC_INT_8B,      // data code contains 8B datum
  
  /* 09 */ 0, // MC_INT_M_ONE,   // -1 (no bytes in data code)
  /* 10 */ 0, // MC_INT_ZERO,    // 0  (no bytes in data code)
  /* 11 */ 0, // MC_INT_ONE,     // 1  (no bytes in data code)
  /* 12 */ 0, // MC_INT_TWO,     // 2  (no bytes in data code)
  /* 13 */ 0, // MC_INT_THREE,   // 3  (no bytes in data code)
  /* 14 */ 0, // MC_INT_FOUR,    // 4  (no bytes in data code)
  /* 15 */ 0, // MC_INT_FIVE,    // 5  (no bytes in data code)
  /* 16 */ 0, // MC_INT_SIX,     // 6  (no bytes in data code)
  /* 17 */ 0, // MC_INT_SEVEN,   // 7  (no bytes in data code)
  /* 18 */ 0, // MC_INT_EIGHT,   // 8  (no bytes in data code)
  /* 19 */ 0, // MC_INT_NINE,    // 9  (no bytes in data code)
  /* 20 */ 0, // MC_INT_TEN,     // 10 (no bytes in data code)
  
  /* 01 */ 1, // MC_IVL_1B,      // data code contains 1B datum
  /* 02 */ 2, // MC_IVL_2B,      // data code contains 2B datum
  /* 03 */ 3, // MC_IVL_3B,      // data code contains 3B datum
  /* 04 */ 4, // MC_IVL_4B,      // data code contains 4B datum
  /* 05 */ 5, // MC_IVL_5B,      // data code contains 5B datum
  /* 06 */ 6, // MC_IVL_6B,      // data code contains 6B datum
  /* 07 */ 7, // MC_IVL_7B,      // data code contains 7B datum
  /* 08 */ 8, // MC_IVL_8B,      // data code contains 8B datum
  
  /* 26 */ 0, // MC_STR_EMPTY,   // empty string (no bytes in data code)
  /* 27 */ 1, // MC_STR_FIT_1L,  // ascii string: 1B=len, then ascii with null term
  /* 28 */ 2, // MC_STR_FIT_2L,  // ascii string: 2B=len, then ascii with null term
  /* 29 */ 2, // MC_STR_TAIL_2T, // ascii string in tail:
                                 //   head: 2B->tail
                                 //   tail: has 1B=len, then ascii with null term
  
  /* 01 */ 1, // MC_TME_1B,      // data code contains 1B datum
  /* 02 */ 2, // MC_TME_2B,      // data code contains 2B datum
  /* 03 */ 3, // MC_TME_3B,      // data code contains 3B datum
  /* 04 */ 4, // MC_TME_4B,      // data code contains 4B datum
  /* 05 */ 5, // MC_TME_5B,      // data code contains 5B datum
  /* 06 */ 6, // MC_TME_6B,      // data code contains 6B datum
  /* 07 */ 7, // MC_TME_7B,      // data code contains 7B datum
  /* 08 */ 8, // MC_TME_8B,      // data code contains 8B datum
  
  /* 08 */ 8, // MC_TSP_8B,      // data code contains 8B datum
  
  /* 30 */ 2, // MC_OVERFLOW_2T, // overflow tail (2B->overflow tail)
  
  /* 53 */ 0  // MC_ENUMS,
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
