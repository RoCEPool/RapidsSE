/*============================================================================*/
/* eTN.h -- Token Type                                                        */
/*============================================================================*/
#pragma once

enum eTN // token type
{
  /*  0 */ TN_NONE=0,  // none
  
  /*  1 */ TN_INTEGER, // integer literal, e.g. 1, 42
  /*  2 */ TN_NAME,    // name token, e.g. WASHER (or ++ in special mode)
  /*  3 */ TN_NULL,    // the \0 null, used to indicate end of input
  /*  4 */ TN_OBJECT,  // pointer to objects
  /*  5 */ TN_OTHER,   // other literal, e.g. {,~
  /*  6 */ TN_STRING,  // string literal, e.g. "the answer"
  /*  7 */ TN_TAG,     // tag token such as <tableNm>

  /*  8 */ TN_ENUMS
};

static Stz* aTN[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "INTEGER"_Stz,
  /*  2 */ "NAME"_Stz,
  /*  3 */ "NULL"_Stz,
  /*  5 */ "OBJECT"_Stz,
  /*  6 */ "OTHER"_Stz,
  /*  7 */ "STRING"_Stz,
  /*  8 */ "TAG"_Stz,

  /*  9 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
