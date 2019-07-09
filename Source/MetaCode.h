/*============================================================================*/
/* MetaCode.h -- Meta Codes for Row Coder Mark V                              */
/*============================================================================*/
#pragma once

namespace Rows
{
  enum eMC : u1 // meta code
  {
  /* 00 */ MC_NULL = 0,    // null datum (no bytes in data code )
  
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
  
  /* 26 */ MC_FLT_1B,      // data code contains 1B datum
  /* 27 */ MC_FLT_2B,      // data code contains 2B datum
  /* 28 */ MC_FLT_3B,      // data code contains 3B datum
  /* 29 */ MC_FLT_4B,      // data code contains 4B datum
  /* 30 */ MC_FLT_5B,      // data code contains 5B datum
  /* 31 */ MC_FLT_6B,      // data code contains 6B datum
  /* 32 */ MC_FLT_7B,      // data code contains 7B datum
  /* 33 */ MC_FLT_8B,      // data code contains 8B datum
  
  /* 34 */ MC_INT_1B,      // data code contains 1B datum
  /* 35 */ MC_INT_2B,      // data code contains 2B datum
  /* 36 */ MC_INT_3B,      // data code contains 3B datum
  /* 37 */ MC_INT_4B,      // data code contains 4B datum
  /* 38 */ MC_INT_5B,      // data code contains 5B datum
  /* 39 */ MC_INT_6B,      // data code contains 6B datum
  /* 40 */ MC_INT_7B,      // data code contains 7B datum
  /* 41 */ MC_INT_8B,      // data code contains 8B datum
  
  /* 42 */ MC_INT_M_ONE,   // -1 (no bytes in data code)
  /* 43 */ MC_INT_ZERO,    // 0  (no bytes in data code)
  /* 44 */ MC_INT_ONE,     // 1  (no bytes in data code)
  /* 45 */ MC_INT_TWO,     // 2  (no bytes in data code)
  /* 46 */ MC_INT_THREE,   // 3  (no bytes in data code)
  /* 47 */ MC_INT_FOUR,    // 4  (no bytes in data code)
  /* 48 */ MC_INT_FIVE,    // 5  (no bytes in data code)
  /* 49 */ MC_INT_SIX,     // 6  (no bytes in data code)
  /* 50 */ MC_INT_SEVEN,   // 7  (no bytes in data code)
  /* 51 */ MC_INT_EIGHT,   // 8  (no bytes in data code)
  /* 52 */ MC_INT_NINE,    // 9  (no bytes in data code)
  /* 53 */ MC_INT_TEN,     // 10 (no bytes in data code)
  
  /* 54 */ MC_IVL_1B,      // data code contains 1B datum
  /* 55 */ MC_IVL_2B,      // data code contains 2B datum
  /* 56 */ MC_IVL_3B,      // data code contains 3B datum
  /* 57 */ MC_IVL_4B,      // data code contains 4B datum
  /* 58 */ MC_IVL_5B,      // data code contains 5B datum
  /* 59 */ MC_IVL_6B,      // data code contains 6B datum
  /* 60 */ MC_IVL_7B,      // data code contains 7B datum
  /* 61 */ MC_IVL_8B,      // data code contains 8B datum
  
  /* 62 */ MC_STR_EMPTY,   // empty string (no bytes in data code)
  /* 63 */ MC_STR_FIT_1L,  // ascii string: 1B=len, then ascii with null term
  /* 64 */ MC_STR_FIT_2L,  // ascii string: 2B=len, then ascii with null term
  /* 65 */ MC_STR_TAIL_2T, // ascii string in tail:
                           //   head: 2B->tail
                           //   tail: has 1B=len, then ascii with null term
  
  /* 66 */ MC_TME_1B,      // data code contains 1B datum
  /* 67 */ MC_TME_2B,      // data code contains 2B datum
  /* 68 */ MC_TME_3B,      // data code contains 3B datum
  /* 69 */ MC_TME_4B,      // data code contains 4B datum
  /* 70 */ MC_TME_5B,      // data code contains 5B datum
  /* 71 */ MC_TME_6B,      // data code contains 6B datum
  /* 72 */ MC_TME_7B,      // data code contains 7B datum
  /* 73 */ MC_TME_8B,      // data code contains 8B datum

  /* 74 */ MC_TSP_8B,      // data code contains 8B datum

  /* 75 */ MC_OVERFLOW_2T, // overflow tail: 2B->overflow tail
  
  /* 76 */ MC_ENUMS,
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  static Stz* aMC[]
  {
  /* 00 */ "MC_NULL"_Stz,
  
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
  
  /* 26 */ "MC_FLT_1B"_Stz,
  /* 27 */ "MC_FLT_2B"_Stz,
  /* 28 */ "MC_FLT_3B"_Stz,
  /* 29 */ "MC_FLT_4B"_Stz,
  /* 30 */ "MC_FLT_5B"_Stz,
  /* 31 */ "MC_FLT_6B"_Stz,
  /* 32 */ "MC_FLT_7B"_Stz,
  /* 33 */ "MC_FLT_8B"_Stz,
  
  /* 34 */ "MC_INT_1B"_Stz,
  /* 35 */ "MC_INT_2B"_Stz,
  /* 36 */ "MC_INT_3B"_Stz,
  /* 37 */ "MC_INT_4B"_Stz,
  /* 38 */ "MC_INT_5B"_Stz,
  /* 39 */ "MC_INT_6B"_Stz,
  /* 40 */ "MC_INT_7B"_Stz,
  /* 41 */ "MC_INT_8B"_Stz,
  
  /* 42 */ "MC_INT_M_ONE"_Stz,
  /* 43 */ "MC_INT_ZERO"_Stz,
  /* 44 */ "MC_INT_ONE"_Stz,
  /* 45 */ "MC_INT_TWO"_Stz,
  /* 46 */ "MC_INT_THREE"_Stz,
  /* 47 */ "MC_INT_FOUR"_Stz,
  /* 48 */ "MC_INT_FIVE"_Stz,
  /* 49 */ "MC_INT_SIX"_Stz,
  /* 50 */ "MC_INT_SEVEN"_Stz,
  /* 51 */ "MC_INT_EIGHT"_Stz,
  /* 52 */ "MC_INT_NINE"_Stz,
  /* 53 */ "MC_INT_TEN"_Stz,
  
  /* 54 */ "MC_IVL_1B"_Stz,
  /* 55 */ "MC_IVL_2B"_Stz,
  /* 56 */ "MC_IVL_3B"_Stz,
  /* 57 */ "MC_IVL_4B"_Stz,
  /* 58 */ "MC_IVL_5B"_Stz,
  /* 59 */ "MC_IVL_6B"_Stz,
  /* 60 */ "MC_IVL_7B"_Stz,
  /* 61 */ "MC_IVL_8B"_Stz,
  
  /* 62 */ "MC_STR_EMPTY"_Stz,
  /* 63 */ "MC_STR_FIT_1L"_Stz,
  /* 64 */ "MC_STR_FIT_2L"_Stz,
  /* 65 */ "MC_STR_TAIL_2T"_Stz,

  /* 66 */ "MC_TME_1B"_Stz,
  /* 67 */ "MC_TME_2B"_Stz,
  /* 68 */ "MC_TME_3B"_Stz,
  /* 69 */ "MC_TME_4B"_Stz,
  /* 70 */ "MC_TME_5B"_Stz,
  /* 71 */ "MC_TME_6B"_Stz,
  /* 72 */ "MC_TME_7B"_Stz,
  /* 73 */ "MC_TME_8B"_Stz,
  
  /* 74 */ "MC_TSP_8B"_Stz,
  
  /* 75 */ "MC_OVERFLOW_2T"_Stz,
  
  /* 76 */ "MC_ENUMS"_Stz,
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  static i8 nMC[] = // size of meta code
  {
  /* 00 */ 0, // MC_NULL         // null datum (no bytes in data code )
    
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
  
  /* 26 */ 1, // MC_FLT_1B,      // data code contains 1B datum
  /* 27 */ 2, // MC_FLT_2B,      // data code contains 2B datum
  /* 28 */ 3, // MC_FLT_3B,      // data code contains 3B datum
  /* 29 */ 4, // MC_FLT_4B,      // data code contains 4B datum
  /* 30 */ 5, // MC_FLT_5B,      // data code contains 5B datum
  /* 31 */ 6, // MC_FLT_6B,      // data code contains 6B datum
  /* 32 */ 7, // MC_FLT_7B,      // data code contains 7B datum
  /* 33 */ 8, // MC_FLT_8B,      // data code contains 8B datum
  
  /* 34 */ 1, // MC_INT_1B,      // data code contains 1B datum
  /* 35 */ 2, // MC_INT_2B,      // data code contains 2B datum
  /* 36 */ 3, // MC_INT_3B,      // data code contains 3B datum
  /* 37 */ 4, // MC_INT_4B,      // data code contains 4B datum
  /* 38 */ 5, // MC_INT_5B,      // data code contains 5B datum
  /* 39 */ 6, // MC_INT_6B,      // data code contains 6B datum
  /* 40 */ 7, // MC_INT_7B,      // data code contains 7B datum
  /* 41 */ 8, // MC_INT_8B,      // data code contains 8B datum
  
  /* 42 */ 0, // MC_INT_M_ONE,   // -1 (no bytes in data code)
  /* 43 */ 0, // MC_INT_ZERO,    // 0  (no bytes in data code)
  /* 44 */ 0, // MC_INT_ONE,     // 1  (no bytes in data code)
  /* 45 */ 0, // MC_INT_TWO,     // 2  (no bytes in data code)
  /* 46 */ 0, // MC_INT_THREE,   // 3  (no bytes in data code)
  /* 47 */ 0, // MC_INT_FOUR,    // 4  (no bytes in data code)
  /* 48 */ 0, // MC_INT_FIVE,    // 5  (no bytes in data code)
  /* 49 */ 0, // MC_INT_SIX,     // 6  (no bytes in data code)
  /* 50 */ 0, // MC_INT_SEVEN,   // 7  (no bytes in data code)
  /* 51 */ 0, // MC_INT_EIGHT,   // 8  (no bytes in data code)
  /* 52 */ 0, // MC_INT_NINE,    // 9  (no bytes in data code)
  /* 53 */ 0, // MC_INT_TEN,     // 10 (no bytes in data code)
  
  /* 54 */ 1, // MC_IVL_1B,      // data code contains 1B datum
  /* 55 */ 2, // MC_IVL_2B,      // data code contains 2B datum
  /* 56 */ 3, // MC_IVL_3B,      // data code contains 3B datum
  /* 57 */ 4, // MC_IVL_4B,      // data code contains 4B datum
  /* 58 */ 5, // MC_IVL_5B,      // data code contains 5B datum
  /* 59 */ 6, // MC_IVL_6B,      // data code contains 6B datum
  /* 60 */ 7, // MC_IVL_7B,      // data code contains 7B datum
  /* 61 */ 8, // MC_IVL_8B,      // data code contains 8B datum
  
  /* 62 */ 0, // MC_STR_EMPTY,   // empty string (no bytes in data code)
  /* 63 */ 1, // MC_STR_FIT_1L,  // ascii string: 1B=len, then ascii with null term
  /* 64 */ 2, // MC_STR_FIT_2L,  // ascii string: 2B=len, then ascii with null term
  /* 65 */ 2, // MC_STR_TAIL_2T, // ascii string in tail:
                                 //   head: 2B->tail
                                 //   tail: has 1B=len, then ascii with null term
  
  /* 66 */ 1, // MC_TME_1B,      // data code contains 1B datum
  /* 67 */ 2, // MC_TME_2B,      // data code contains 2B datum
  /* 68 */ 3, // MC_TME_3B,      // data code contains 3B datum
  /* 69 */ 4, // MC_TME_4B,      // data code contains 4B datum
  /* 70 */ 5, // MC_TME_5B,      // data code contains 5B datum
  /* 71 */ 6, // MC_TME_6B,      // data code contains 6B datum
  /* 72 */ 7, // MC_TME_7B,      // data code contains 7B datum
  /* 73 */ 8, // MC_TME_8B,      // data code contains 8B datum
  
  /* 74 */ 8, // MC_TSP_8B,      // data code contains 8B datum
  
  /* 75 */ 2, // MC_OVERFLOW_2T, // overflow tail (2B->overflow tail)
  
  /* 76 */ 0  // MC_ENUMS,
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
