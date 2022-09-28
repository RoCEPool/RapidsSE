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
  /*  5 */ TN_INVALID, // invalid/malformed token
  /*  6 */ TN_LITERAL, // quoted literal, e.g. 'some literal'
  /*  7 */ TN_NAME,    // name token, e.g. WASHER (or ++ in special mode)
  /*  8 */ TN_NULL,    // the \0 null, used to indicate end of input
  /*  9 */ TN_OBJECT,  // pointer to objects
  /* 10 */ TN_OTHER,   // other literal, e.g. {,~
  /* 11 */ TN_STRING,  // string literal, e.g. "the answer"
  /* 11 */ TN_TAG,     // tag token such as <tableNm>

  /* 13 */ TN_ENUMS
};

static Stz* aTN[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "BOOLEAN"_Stz,
  /*  2 */ "DECIMAL"_Stz,
  /*  3 */ "FLOAT"_Stz,
  /*  4 */ "INTEGER"_Stz,
  /*  5 */ "INVALID"_Stz,
  /*  6 */ "LITERAL"_Stz,
  /*  7 */ "NAME"_Stz,
  /*  8 */ "NULL"_Stz,
  /*  9 */ "OBJECT"_Stz,
  /* 10 */ "OTHER"_Stz,
  /* 11 */ "STRING"_Stz,
  /* 12 */ "TAG"_Stz,

  /* 13 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
