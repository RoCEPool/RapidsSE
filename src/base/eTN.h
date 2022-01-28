/*============================================================================*/
/* eTN.h -- Token Type                                                        */
/*============================================================================*/
#pragma once

enum eTN // token type
{
  /*  0 */ TN_NONE=0,  // none
  
  /*  1 */ TN_DECIMAL, // decimal literal, e.g. 12.34
  /*  2 */ TN_FLOAT,   // float literal, e.g. TBA
  /*  3 */ TN_INTEGER, // integer literal, e.g. 1, 42
  /*  4 */ TN_NAME,    // name token, e.g. WASHER (or ++ in special mode)
  /*  5 */ TN_NULL,    // the \0 null, used to indicate end of input
  /*  6 */ TN_OBJECT,  // pointer to objects
  /*  7 */ TN_OTHER,   // other literal, e.g. {,~
  /*  8 */ TN_STRING,  // string literal, e.g. "the answer"
  /*  9 */ TN_TAG,     // tag token such as <tableNm>

  /* 10 */ TN_ENUMS
};

static Stz* aTN[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "DECIMAL"_Stz,
  /*  2 */ "FLOAT"_Stz,
  /*  3 */ "INTEGER"_Stz,
  /*  4 */ "NAME"_Stz,
  /*  5 */ "NULL"_Stz,
  /*  6 */ "OBJECT"_Stz,
  /*  7 */ "OTHER"_Stz,
  /*  8 */ "STRING"_Stz,
  /*  9 */ "TAG"_Stz,

  /* 10 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
