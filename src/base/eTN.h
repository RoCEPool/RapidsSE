/*============================================================================*/
/* eTN.h -- Token Type                                                        */
/*============================================================================*/
#pragma once

enum eTN // token type
{
  /*  0 */ TN_NONE=0,  // none
  
  /*  1 */ TN_DECIMAL, // decimal literal, e.g. 12.34
  /*  2 */ TN_INTEGER, // integer literal, e.g. 1, 42
  /*  3 */ TN_NAME,    // name token, e.g. WASHER (or ++ in special mode)
  /*  4 */ TN_NULL,    // the \0 null, used to indicate end of input
  /*  5 */ TN_OBJECT,  // pointer to objects
  /*  6 */ TN_OTHER,   // other literal, e.g. {,~
  /*  7 */ TN_STRING,  // string literal, e.g. "the answer"
  /*  8 */ TN_TAG,     // tag token such as <tableNm>

  /*  9 */ TN_ENUMS
};

static Stz* aTN[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "DECIMAL"_Stz,
  /*  2 */ "INTEGER"_Stz,
  /*  3 */ "NAME"_Stz,
  /*  4 */ "NULL"_Stz,
  /*  5 */ "OBJECT"_Stz,
  /*  6 */ "OTHER"_Stz,
  /*  7 */ "STRING"_Stz,
  /*  8 */ "TAG"_Stz,

  /*  9 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
