/*============================================================================*/
/* MetaCode.h -- Meta Codes                                                   */
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
  
  /* 09 */ MC_MINUS_ONE,   // -1 (no bytes in data code )
  /* 10 */ MC_ZERO,        // 0  (no bytes in data code )
  /* 11 */ MC_ONE,         // 1  (no bytes in data code )
  /* 12 */ MC_TWO,         // 2  (no bytes in data code )
  /* 13 */ MC_THREE,       // 3  (no bytes in data code )
  /* 14 */ MC_FOUR,        // 4  (no bytes in data code )
  /* 15 */ MC_FIVE,        // 5  (no bytes in data code )
  /* 16 */ MC_SIX,         // 6  (no bytes in data code )
  /* 17 */ MC_SEVEN,       // 7  (no bytes in data code )
  /* 18 */ MC_EIGHT,       // 8  (no bytes in data code )
  /* 19 */ MC_NINE,        // 9  (no bytes in data code )
  /* 20 */ MC_TEN,         // 10 (no bytes in data code )
  
  /* 21 */ MC_SAME,        // same as previous (no bytes in data code )
  /* 22 */ MC_ADD_1B,      // previous + 1B in data code
  /* 23 */ MC_ADD_2B,      // previous + 2B in data code
  /* 24 */ MC_SUB_1B,      // previous - 1B in data code
  /* 25 */ MC_SUB_2B,      // previous - 2B in data code
  
  /* 26 */ MC_FALSE,       // false (no bytes in data code )
  /* 27 */ MC_TRUE,        // true  (no bytes in data code )
  
  /* 28 */ MC_DATE_E0_2B,  // date in era 0 expressed as 2B in data code
  /* 29 */ MC_DATE_E1_2B,  // date in era 1 expressed as 2B in data code
  /* 30 */ MC_DATE_E2_2B,  // date in era 2 expressed as 2B in data code

  /* 31 */ MC_DAYS_ADD_1B, // first date +days 1B in data code
  /* 32 */ MC_DAYS_SUB_1B, // first date -days 1B in data code
  
  /* 33 */ MC_STR_EMPTY,       // empty string (no bytes in data code )
  /* 34 */ MC_STR_DUP_2H,      // duplicate string (2B->head)
  /* 35 */ MC_STR_FIT_1L,      // ascii string: 1B=len, then ascii
  /* 36 */ MC_STR_TAIL_2T,     // ascii string in tail:
                               //    head: 2B->tail
                               //    tail: has 1B=len, then ascii
  /* 37 */ MC_STR_SPLIT_2T1L,  // ascii string split between head and tail:
                               //   head: 2B->tail, 1B=len, then part-ascii
                               //   tail: has remaining ascii
  
  /* 38 */ MC_BSTR_FIT_2L,     // bigger ascii string: 2B=len then ascii
  /* 39 */ MC_BSTR_TAIL_2T,    // bigger ascii string in tail:
                               //    head: 2B->tail
                               //    tail: has 2B=len, then ascii
  /* 40 */ MC_BSTR_SPLIT_2T2L, // bigger ascii string split between head and tail:
                               //   head: 2B->tail, 2B=len, part-ascii
                               //   tail: has remaining ascii
  
  /* 41 */ MC_SHOCO_DUP_3D,    // duplicate shoco string (3B->shoco)
  /* 42 */ MC_SHOCO_FIT_1C,    // shoco encoded string: 1B=len, then shoco
  /* 43 */ MC_SHOCO_TAIL_2T,   // shoco encoded string in tail:
                               //    head: 2B->tail
                               //    tail: has 1B=len, then shoco
  /* 44 */ MC_SHOCO_SPLIT_2T1C,// shoco encoded string split between head and tail:
                               //   head: 2B->tail, 1B=nCompressed, partA-shoco
                               //   tail: has remaining partB-shoco

  /* 45 */ MC_TWOS_1B,         // data code  contains 1B two's complement datum
  /* 46 */ MC_TWOS_2B,         // data code  contains 2B two's complement datum
  /* 47 */ MC_TWOS_3B,         // data code  contains 3B two's complement datum
  /* 48 */ MC_TWOS_4B,         // data code  contains 4B two's complement datum
  /* 49 */ MC_TWOS_5B,         // data code  contains 5B two's complement datum
  /* 50 */ MC_TWOS_6B,         // data code  contains 6B two's complement datum
  /* 51 */ MC_TWOS_7B,         // data code  contains 7B two's complement datum
  
  //       MC_ROW_REF_2B,      // nB in data code  being a row reference [RR]
  //       MC_RETURN,          // return
  /* 52 */ MC_OVERFLOW_2T,     // overflow tail (2B->overflow tail)
  
  /* 53 */ MC_ENUMS,
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
  /* 34 */ "MC_STR_DUP_2H"_Stz,
  /* 35 */ "MC_STR_FIT_1L"_Stz,
  /* 36 */ "MC_STR_TAIL_2T"_Stz,
  /* 37 */ "MC_STR_SPLIT_2T1L"_Stz,
  
  /* 38 */ "MC_BSTR_FIT_2L"_Stz,
  /* 39 */ "MC_BSTR_TAIL_2T"_Stz,
  /* 40 */ "MC_BSTR_SPLIT_2T2L"_Stz,
  
  /* 41 */ "MC_SHOCO_DUP_3D"_Stz,
  /* 42 */ "MC_SHOCO_FIT_1C"_Stz,
  /* 43 */ "MC_SHOCO_TAIL_2T"_Stz,
  /* 44 */ "MC_SHOCO_SPLIT_2T1C"_Stz,
  
  /* 45 */ "MC_TWOS_1B"_Stz,
  /* 46 */ "MC_TWOS_2B"_Stz,
  /* 47 */ "MC_TWOS_3B"_Stz,
  /* 48 */ "MC_TWOS_4B"_Stz,
  /* 49 */ "MC_TWOS_5B"_Stz,
  /* 50 */ "MC_TWOS_6B"_Stz,
  /* 51 */ "MC_TWOS_7B"_Stz,
  /*    */ //"MC_ROW_REF_2B"_Stz,
  /* 52 */ "MC_OVERFLOW_2T"_Stz,
  /* 53 */ "MC_ENUMS"_Stz,
  };
  
  static i8 nMC[] = // size of meta code
  {
  /* 00 */ 0, // MC_NULL         // null datum (no bytes in data code )
  
  /* 01 */ 1, // MC_1B,          // data code  contains 1B datum
  /* 02 */ 2, // MC_2B,          // data code  contains 2B datum
  /* 03 */ 3, // MC_3B,          // data code  contains 3B datum
  /* 04 */ 4, // MC_4B,          // data code  contains 4B datum
  /* 05 */ 5, // MC_5B,          // data code  contains 5B datum
  /* 06 */ 6, // MC_6B,          // data code  contains 6B datum
  /* 07 */ 7, // MC_7B,          // data code  contains 7B datum
  /* 08 */ 8, // MC_8B,          // data code  contains 8B datum
  
  /* 09 */ 0, // MC_MINUS_ONE,   // -1 (no bytes in data code )
  /* 10 */ 0, // MC_ZERO,        // 0  (no bytes in data code )
  /* 11 */ 0, // MC_ONE,         // 1  (no bytes in data code )
  /* 12 */ 0, // MC_TWO,         // 2  (no bytes in data code )
  /* 13 */ 0, // MC_THREE,       // 3  (no bytes in data code )
  /* 14 */ 0, // MC_FOUR,        // 4  (no bytes in data code )
  /* 15 */ 0, // MC_FIVE,        // 5  (no bytes in data code )
  /* 16 */ 0, // MC_SIX,         // 6  (no bytes in data code )
  /* 17 */ 0, // MC_SEVEN,       // 7  (no bytes in data code )
  /* 18 */ 0, // MC_EIGHT,       // 8  (no bytes in data code )
  /* 19 */ 0, // MC_NINE,        // 9  (no bytes in data code )
  /* 20 */ 0, // MC_TEN,         // 10 (no bytes in data code )
  
  /* 21 */ 0, // MC_SAME,        // same as previous (no bytes in data code )
  /* 22 */ 1, // MC_ADD_1B,      // previous + 1B in data code
  /* 23 */ 2, // MC_ADD_2B,      // previous + 2B in data code
  /* 24 */ 1, // MC_SUB_1B,      // previous - 1B in data code
  /* 25 */ 2, // MC_SUB_2B,      // previous - 2B in data code
  
  /* 26 */ 0, // MC_FALSE,       // false (no bytes in data code )
  /* 27 */ 0, // MC_TRUE,        // true  (no bytes in data code )
  
  /* 28 */ 2, // MC_DATE_E0_2B,  // date in era 0 expressed as 2B in data code
  /* 29 */ 2, // MC_DATE_E1_2B,  // date in era 1 expressed as 2B in data code
  /* 30 */ 2, // MC_DATE_E2_2B,  // date in era 2 expressed as 2B in data code
  
  /* 31 */ 1, // MC_DAYS_ADD_1B, // first date +days 1B in data code
  /* 32 */ 2, // MC_DAYS_SUB_1B, // first date -days 1B in data code
  
  /* 33 */ 0, // MC_STR_EMPTY,   // empty string (no bytes in data code )
  /* 34 */ 2, // MC_STR_DUP_2H,  // duplicate string (2B->head)
  /* 35 */ 1, // MC_STR_FIT_1L,  // ascii string: 1B=len, then ascii
  /* 36 */ 2, // MC_STR_TAIL_2T, // longer ascii string:
                                 //    head: 2B->tail
                                 //    tail: has 1B=len, then ascii
  /* 37 */ 3, // MC_STR_SPLIT_2T1L,// ascii string:
                                   //   head: 2B->tail, 1B=len, then part-ascii
                                   //   tail: has remaining ascii
  
  /* 38 */ 2, // MC_BSTR_FIT_2L,   // bigger ascii string: 2B=len then ascii
  /* 39 */ 2, // MC_BSTR_TAIL_2T,  // bigger ascii string:
                                   //    head: 2B->tail
                                   //    tail: has 2B=len, then ascii
  /* 40 */ 3, // MC_BSTR_SPLIT_2T2L, // bigger ascii string:
                                     //   head: 2B->tail, 2B=len, part-ascii
                                     //   tail: has remaining ascii
  
  /* 41 */ 3, // MC_SHOCO_DUP_3D,    // duplicate shoco string (3B->shoco)
  /* 42 */ 1, // MC_SHOCO_FIT_1C,    // shoco encoded string: 1B=len, then shoco
  /* 43 */ 2, // MC_SHOCO_TAIL_2T,   // shoco encoded string:
                                     //    head: 2B->tail
                                     //    tail: has 1B=len, then shoco
  /* 44 */ 3, // MC_SHOCO_SPLIT_2T1C,// shoco encoded string:
                                     //   head: 2B->tail, 1B=len, part-shoco
                                     //   tail: has remaining shoco
  
  /* 45 */ 1, // MC_TWOS_1B,         // data code  contains 1B two's complement datum
  /* 46 */ 2, // MC_TWOS_2B,         // data code  contains 2B two's complement datum
  /* 47 */ 3, // MC_TWOS_3B,         // data code  contains 3B two's complement datum
  /* 48 */ 4, // MC_TWOS_4B,         // data code  contains 4B two's complement datum
  /* 49 */ 5, // MC_TWOS_5B,         // data code  contains 5B two's complement datum
  /* 50 */ 6, // MC_TWOS_6B,         // data code  contains 6B two's complement datum
  /* 51 */ 7, // MC_TWOS_7B,         // data code  contains 7B two's complement datum
  
  /* 52 */ 2, // MC_OVERFLOW_2T,     // overflow tail (2B->overflow tail)
  
  /* 53 */ 0  // MC_ENUMS,
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
