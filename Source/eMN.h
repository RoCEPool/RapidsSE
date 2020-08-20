/*============================================================================*/
/* eMN.h -- Meta Nibble (Meta Code v2)                                        */
/*============================================================================*/
#pragma once

enum eMN : u1 // meta nibble
{
  /* 0 */ MN_NUMERIC = 0, // numeric data 0B .. 8B
  /* 1 */ MN_DATE_E0,     // date era 0
  /* 2 */ MN_DATE_E1,     // date era 1
  /* 3 */ MN_DATE_E2,     // date era 2
  /* 4 */ MN_STRING_x1,   // short string 0B .. 15B
  /* 5 */ MN_NULL,        // null value
  /* 6 */ MN_SPARE0,      // unused
  /* 7 */ MN_OVERFLOW,    // 2B overflow
  
  /* 8 */ MN_STRING_x2,   // 2 x string (see macro)
  /* 9 */ MN_STRING_x4,   // 4 x string (see macro)
  /* A */ MN_STRING_x8,   // 8 x string (see macro)
  /* B */ MN_STRING_TAIL, // string in tail
  /* C */ MN_BLOB_0,      // blob type 0
  /* D */ MN_BLOB_1,      // blob type 1
  /* E */ MN_NEGATIVE,    // -ve digit -15 .. -0
  /* F */ MN_POSITIVE,    // +ve digit   0 .. 15

  /* 16 */ MN_ENUMS,
};
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
static Stz* aMN[]
{
  /* 0 */ "MN_NUMERIC"_Stz,
  /* 1 */ "MN_DATE_E0"_Stz,
  /* 2 */ "MN_DATE_E1"_Stz,
  /* 3 */ "MN_DATE_E2"_Stz,
  /* 4 */ "MN_STRING_x1"_Stz,
  /* 5 */ "MN_NULL"_Stz,
  /* 6 */ "MN_SPARE0"_Stz,
  /* 7 */ "MN_OVERFLOW"_Stz,
  
  /* 8 */ "MN_STRING_x2"_Stz,
  /* 9 */ "MN_STRING_x4"_Stz,
  /* A */ "MN_STRING_x8"_Stz,
  /* B */ "MN_STRING_TAIL"_Stz,
  /* C */ "MN_BLOB_0"_Stz,
  /* D */ "MN_BLOB_1"_Stz,
  /* E */ "MN_NEGATIVE"_Stz,
  /* F */ "MN_POSITIVE"_Stz,
  
  /* 16 */ "MN_ENUMS"_Stz,
};
  
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
