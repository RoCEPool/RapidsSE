/*============================================================================*/
/* eCT.h -- Column Type                                                      */
/*============================================================================*/
#pragma once

// NOTE: Any additions/subtractions or reordering of eCT will affect the expression type
// calculus in a bunch of files under store/sqlCompiler/shadowNodes/expressions/operators

enum eCT // Column Type
{
  /*  0 */ CT_NONE=0,
  /*  1 */ CT_BOOLEAN,
  /*  2 */ CT_CHAR,
  /*  3 */ CT_DATE,
  /*  4 */ CT_DECIMAL,
  /*  5 */ CT_FLOAT,
  /*  6 */ CT_INTEGER,
  /*  7 */ CT_INTERVAL_DT,  // day-time interval
  /*  8 */ CT_INTERVAL_YM,  // year-month interval
  /*  9 */ CT_TIME,
  /* 10 */ CT_TIMESTAMP,
  /* 11 */ CT_VARCHAR,
  /* 12 */ CT_UNKNOWN,
  /* 13 */ CT_ENUMS
};

extern Stz* aCT[CT_ENUMS];
extern Stz* szCT_char[CT_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
