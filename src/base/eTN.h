/*============================================================================*/
/* eTN.h -- Token Type                                                        */
/*============================================================================*/
#pragma once

enum eTN // token type
{
  /*  0 */ TN_NONE=0,  // none
  
  /*  1 */ TN_BOOLEAN, // boolean literal, e.g. true, false
  /*  2 */ TN_DECIMAL, // decimal literal, e.g. 12.34
  /*  3 */ TN_FLOAT,   // float literal, e.g. 0.3141e1
  /*  4 */ TN_INTEGER, // integer literal, e.g. 1, 42
  /*  5 */ TN_NAME,    // name token, e.g. WASHER (or ++ in special mode)
  /*  6 */ TN_NULL,    // the \0 null, used to indicate end of input
  /*  7 */ TN_OBJECT,  // pointer to objects
  /*  8 */ TN_OTHER,   // other literal, e.g. {,~
  /*  9 */ TN_STRING,  // string literal, e.g. "the answer"
  /* 10 */ TN_TAG,     // tag token such as <tableNm>

  /* 11 */ TN_ENUMS
};

static Stz* aTN[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "BOOLEAN"_Stz,
  /*  2 */ "DECIMAL"_Stz,
  /*  3 */ "FLOAT"_Stz,
  /*  4 */ "INTEGER"_Stz,
  /*  5 */ "NAME"_Stz,
  /*  6 */ "NULL"_Stz,
  /*  7 */ "OBJECT"_Stz,
  /*  8 */ "OTHER"_Stz,
  /*  9 */ "STRING"_Stz,
  /* 10 */ "TAG"_Stz,

  /* 11 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
