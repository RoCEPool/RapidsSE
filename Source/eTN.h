/*============================================================================*/
/* eTT.hpp -- token type                                                      */
/*============================================================================*/
#pragma once

enum eTN // token type
{
  /*  0 */ TN_NONE=0,  // none
  /*  1 */ TN_INTEGER, // integer literal, e.g. 1, 42
  /*  2 */ TN_NAME,    // name token, e.g. WASHER (or ++ in special mode)
  /*  3 */ TN_OTHER,   // other literal, e.g. {,~
  /*  4 */ TN_STRING,  // string literal, e.g. "the answer"
  /*  5 */ TN_ENUMS
};

static Stz* aTN[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "INTEGER"_Stz,
  /*  2 */ "NAME"_Stz,
  /*  3 */ "OTHER"_Stz,
  /*  4 */ "STRING"_Stz,
  /*  5 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
