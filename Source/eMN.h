/*============================================================================*/
/* eMN.h -- New Meta Code                                                     */
/*============================================================================*/
#pragma once

enum eMN : u1 // meta code
{
  /* 0 0 */ MN_NULL = 0,    // null datum (no bytes in data code )
  
  /* 0 1 */ MN_BCD_1B,      // data code contains 1B datum
  /* 0 2 */ MN_BCD_2B,      // data code contains 2B datum
  /* 0 3 */ MN_BCD_3B,      // data code contains 3B datum
  /* 0 4 */ MN_BCD_4B,      // data code contains 4B datum
  /* 0 5 */ MN_BCD_5B,      // data code contains 5B datum
  /* 0 6 */ MN_BCD_6B,      // data code contains 6B datum
  /* 0 7 */ MN_BCD_7B,      // data code contains 7B datum
  /* 0 8 */ MN_BCD_8B,      // data code contains 8B datum
  
  /* 0 9 */ MN_BCD_M_ONE,   // -1 (no bytes in data code)
  /* 1 0 */ MN_BCD_ZERO,    // 0  (no bytes in data code)
  /* 1 1 */ MN_BCD_ONE,     // 1  (no bytes in data code)
  /* 1 2 */ MN_BCD_TWO,     // 2  (no bytes in data code)
  /* 1 3 */ MN_BCD_THREE,   // 3  (no bytes in data code)
  /* 1 4 */ MN_BCD_FOUR,    // 4  (no bytes in data code)
  /* 1 5 */ MN_BCD_FIVE,    // 5  (no bytes in data code)
  /* 1 6 */ MN_BCD_SIX,     // 6  (no bytes in data code)
  /* 1 7 */ MN_BCD_SEVEN,   // 7  (no bytes in data code)
  /* 1 8 */ MN_BCD_EIGHT,   // 8  (no bytes in data code)
  /* 1 9 */ MN_BCD_NINE,    // 9  (no bytes in data code)
  /* 2 0 */ MN_BCD_TEN,     // 10 (no bytes in data code)
  
  /* 2 1 */ MN_BOL_FALSE,   // boolean false (no bytes in data code)
  /* 2 2 */ MN_BOL_TRUE,    // boolean true  (no bytes in data code)
  
  /* 2 3 */ MN_DTE_E0_2B,   // date within era 0 expressed as 2B in data code
  /* 2 4 */ MN_DTE_E1_2B,   // date within era 1 expressed as 2B in data code
  /* 2 5 */ MN_DTE_E2_2B,   // date within era 2 expressed as 2B in data code
  
  /* 2 6 */ MN_FLT_1B,      // data code contains 1B datum
  /* 2 7 */ MN_FLT_2B,      // data code contains 2B datum
  /* 2 8 */ MN_FLT_3B,      // data code contains 3B datum
  /* 2 9 */ MN_FLT_4B,      // data code contains 4B datum
  /* 3 0 */ MN_FLT_5B,      // data code contains 5B datum
  /* 3 1 */ MN_FLT_6B,      // data code contains 6B datum
  /* 3 2 */ MN_FLT_7B,      // data code contains 7B datum
  /* 3 3 */ MN_FLT_8B,      // data code contains 8B datum
  
  /* 3 4 */ MN_INT_1B,      // data code contains 1B datum
  /* 3 5 */ MN_INT_2B,      // data code contains 2B datum
  /* 3 6 */ MN_INT_3B,      // data code contains 3B datum
  /* 3 7 */ MN_INT_4B,      // data code contains 4B datum
  /* 3 8 */ MN_INT_5B,      // data code contains 5B datum
  /* 3 9 */ MN_INT_6B,      // data code contains 6B datum
  /* 4 0 */ MN_INT_7B,      // data code contains 7B datum
  /* 4 1 */ MN_INT_8B,      // data code contains 8B datum
  
  /* 4 2 */ MN_INT_M_ONE,   // -1 (no bytes in data code)
  /* 4 3 */ MN_INT_ZERO,    // 0  (no bytes in data code)
  /* 4 4 */ MN_INT_ONE,     // 1  (no bytes in data code)
  /* 4 5 */ MN_INT_TWO,     // 2  (no bytes in data code)
  /* 4 6 */ MN_INT_THREE,   // 3  (no bytes in data code)
  /* 4 7 */ MN_INT_FOUR,    // 4  (no bytes in data code)
  /* 4 8 */ MN_INT_FIVE,    // 5  (no bytes in data code)
  /* 4 9 */ MN_INT_SIX,     // 6  (no bytes in data code)
  /* 5 0 */ MN_INT_SEVEN,   // 7  (no bytes in data code)
  /* 5 1 */ MN_INT_EIGHT,   // 8  (no bytes in data code)
  /* 5 2 */ MN_INT_NINE,    // 9  (no bytes in data code)
  /* 5 3 */ MN_INT_TEN,     // 10 (no bytes in data code)
  
  /* 5 4 */ MN_IVL_1B,      // data code contains 1B datum
  /* 5 5 */ MN_IVL_2B,      // data code contains 2B datum
  /* 5 6 */ MN_IVL_3B,      // data code contains 3B datum
  /* 5 7 */ MN_IVL_4B,      // data code contains 4B datum
  /* 5 8 */ MN_IVL_5B,      // data code contains 5B datum
  /* 5 9 */ MN_IVL_6B,      // data code contains 6B datum
  /* 6 0 */ MN_IVL_7B,      // data code contains 7B datum
  /* 6 1 */ MN_IVL_8B,      // data code contains 8B datum
  
  /* 6 2 */ MN_STR_EMPTY,   // empty string (no bytes in data code)
  /* 6 3 */ MN_STR_FIT_1L,  // ascii string: 1B=len, then ascii with null term
  /* 6 4 */ MN_STR_FIT_2L,  // ascii string: 2B=len, then ascii with null term
  /* 6 5 */ MN_STR_TAIL_2T, // ascii string in tail:
                           //   head: 2B->tail
                           //   tail: has 1B=len, then ascii with null term
  
  /* 6 6 */ MN_TME_1B,      // data code contains 1B datum
  /* 6 7 */ MN_TME_2B,      // data code contains 2B datum
  /* 6 8 */ MN_TME_3B,      // data code contains 3B datum
  /* 6 9 */ MN_TME_4B,      // data code contains 4B datum
  /* 7 0 */ MN_TME_5B,      // data code contains 5B datum
  /* 7 1 */ MN_TME_6B,      // data code contains 6B datum
  /* 7 2 */ MN_TME_7B,      // data code contains 7B datum
  /* 7 3 */ MN_TME_8B,      // data code contains 8B datum

  /* 7 4 */ MN_TSP_8B,      // data code contains 8B datum

  /* 7 5 */ MN_OVERFLOW_2T, // overflow tail: 2B->overflow tail
  
  /* 76 */ MN_ENUMS,
};
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
static Stz* aMN[]
{
  /* 00 */ "MN_NULL"_Stz,
  
  /* 01 */ "MN_BCD_1B"_Stz,
  /* 02 */ "MN_BCD_2B"_Stz,
  /* 03 */ "MN_BCD_3B"_Stz,
  /* 04 */ "MN_BCD_4B"_Stz,
  /* 05 */ "MN_BCD_5B"_Stz,
  /* 06 */ "MN_BCD_6B"_Stz,
  /* 07 */ "MN_BCD_7B"_Stz,
  /* 08 */ "MN_BCD_8B"_Stz,
  
  /* 09 */ "MN_BCD_M_ONE"_Stz,
  /* 10 */ "MN_BCD_ZERO"_Stz,
  /* 11 */ "MN_BCD_ONE"_Stz,
  /* 12 */ "MN_BCD_TWO"_Stz,
  /* 13 */ "MN_BCD_THREE"_Stz,
  /* 14 */ "MN_BCD_FOUR"_Stz,
  /* 15 */ "MN_BCD_FIVE"_Stz,
  /* 16 */ "MN_BCD_SIX"_Stz,
  /* 17 */ "MN_BCD_SEVEN"_Stz,
  /* 18 */ "MN_BCD_EIGHT"_Stz,
  /* 19 */ "MN_BCD_NINE"_Stz,
  /* 20 */ "MN_BCD_TEN"_Stz,
  
  /* 21 */ "MN_BOL_FALSE"_Stz,
  /* 22 */ "MN_BOL_TRUE"_Stz,
  
  /* 23 */ "MN_DTE_E0_2B"_Stz,
  /* 24 */ "MN_DTE_E1_2B"_Stz,
  /* 25 */ "MN_DTE_E2_2B"_Stz,
  
  /* 26 */ "MN_FLT_1B"_Stz,
  /* 27 */ "MN_FLT_2B"_Stz,
  /* 28 */ "MN_FLT_3B"_Stz,
  /* 29 */ "MN_FLT_4B"_Stz,
  /* 30 */ "MN_FLT_5B"_Stz,
  /* 31 */ "MN_FLT_6B"_Stz,
  /* 32 */ "MN_FLT_7B"_Stz,
  /* 33 */ "MN_FLT_8B"_Stz,
  
  /* 34 */ "MN_INT_1B"_Stz,
  /* 35 */ "MN_INT_2B"_Stz,
  /* 36 */ "MN_INT_3B"_Stz,
  /* 37 */ "MN_INT_4B"_Stz,
  /* 38 */ "MN_INT_5B"_Stz,
  /* 39 */ "MN_INT_6B"_Stz,
  /* 40 */ "MN_INT_7B"_Stz,
  /* 41 */ "MN_INT_8B"_Stz,
  
  /* 42 */ "MN_INT_M_ONE"_Stz,
  /* 43 */ "MN_INT_ZERO"_Stz,
  /* 44 */ "MN_INT_ONE"_Stz,
  /* 45 */ "MN_INT_TWO"_Stz,
  /* 46 */ "MN_INT_THREE"_Stz,
  /* 47 */ "MN_INT_FOUR"_Stz,
  /* 48 */ "MN_INT_FIVE"_Stz,
  /* 49 */ "MN_INT_SIX"_Stz,
  /* 50 */ "MN_INT_SEVEN"_Stz,
  /* 51 */ "MN_INT_EIGHT"_Stz,
  /* 52 */ "MN_INT_NINE"_Stz,
  /* 53 */ "MN_INT_TEN"_Stz,
  
  /* 54 */ "MN_IVL_1B"_Stz,
  /* 55 */ "MN_IVL_2B"_Stz,
  /* 56 */ "MN_IVL_3B"_Stz,
  /* 57 */ "MN_IVL_4B"_Stz,
  /* 58 */ "MN_IVL_5B"_Stz,
  /* 59 */ "MN_IVL_6B"_Stz,
  /* 60 */ "MN_IVL_7B"_Stz,
  /* 61 */ "MN_IVL_8B"_Stz,
  
  /* 62 */ "MN_STR_EMPTY"_Stz,
  /* 63 */ "MN_STR_FIT_1L"_Stz,
  /* 64 */ "MN_STR_FIT_2L"_Stz,
  /* 65 */ "MN_STR_TAIL_2T"_Stz,

  /* 66 */ "MN_TME_1B"_Stz,
  /* 67 */ "MN_TME_2B"_Stz,
  /* 68 */ "MN_TME_3B"_Stz,
  /* 69 */ "MN_TME_4B"_Stz,
  /* 70 */ "MN_TME_5B"_Stz,
  /* 71 */ "MN_TME_6B"_Stz,
  /* 72 */ "MN_TME_7B"_Stz,
  /* 73 */ "MN_TME_8B"_Stz,
  
  /* 74 */ "MN_TSP_8B"_Stz,
  
  /* 75 */ "MN_OVERFLOW_2T"_Stz,
  
  /* 76 */ "MN_ENUMS"_Stz,
};
  
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
