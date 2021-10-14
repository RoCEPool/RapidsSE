/*============================================================================*/
/* eMC_tcc.h -- Meta Code (tiny-c)                                            */
/*============================================================================*/
#pragma once

  /* 0 */ MC_NUMERIC = 0, // numeric data 0B .. 8B
  /* 1 */ MC_DATE_E0,     // date era 0
  /* 2 */ MC_DATE_E1,     // date era 1
  /* 3 */ MC_DATE_E2,     // date era 2
  /* 4 */ MC_STRING_x1,   // short string 0B .. 15B
  /* 5 */ MC_NULL,        // null value
  /* 6 */ MC_SPARE0,      // unused
  /* 7 */ MC_OVERFLOW,    // 2B overflow
  
  /* 8 */ MC_STRING_x2,   // 2 x string (see macro)
  /* 9 */ MC_STRING_x4,   // 4 x string (see macro)
  /* A */ MC_STRING_x8,   // 8 x string (see macro)
  /* B */ MC_STRING_TAIL, // string in tail
  /* C */ MC_BLOB_0,      // blob type 0
  /* D */ MC_BLOB_1,      // blob type 1
  /* E */ MC_NEGATIVE,    // -ve digit -15 .. -0
  /* F */ MC_POSITIVE,    // +ve digit   0 .. 15

  /* 16 */ MC_ENUMS,

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
