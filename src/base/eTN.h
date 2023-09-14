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
  /* 12 */ TN_TAG,     // tag token such as <tableNm>

  /* 13 */ TN_ENUMS
};

extern Stz* aTN[TN_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
