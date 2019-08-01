/*============================================================================*/
/* eCT.h -- Column Type                                                      */
/*============================================================================*/
#pragma once

enum eCT // Column Type
{
  /*  0 */ CT_NONE=0,
  /*  1 */ CT_BOOLEAN,
  /*  2 */ CT_CHAR,
  /*  3 */ CT_DATE,
  /*  4 */ CT_DECIMAL,
  /*  5 */ CT_FLOAT,
  /*  6 */ CT_INTEGER,
  /*  7 */ CT_INTERVAL,
  /*  8 */ CT_TIME,
  /*  9 */ CT_TIMESTAMP,
  /* 10 */ CT_VARCHAR,
  /* 11 */ CT_ENUMS
};

static Stz* aCT[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "BOOLEAN"_Stz,
  /*  2 */ "CHAR"_Stz,
  /*  3 */ "DATE"_Stz,
  /*  4 */ "DECIMAL"_Stz,
  /*  5 */ "FLOAT"_Stz,
  /*  6 */ "INTEGER"_Stz,
  /*  7 */ "INTERVAL"_Stz,
  /*  8 */ "TIME"_Stz,
  /*  9 */ "TIMESTAMP"_Stz,
  /* 10 */ "VARCHAR"_Stz,
  /* 11 */ "ENUMS"_Stz
};

static Stz* aCT_short[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "b"_Stz, // boolean
  /*  2 */ "c"_Stz, // char
  /*  3 */ "d"_Stz, // date
  /*  4 */ "m"_Stz, // decimal
  /*  5 */ "f"_Stz, // float
  /*  6 */ "i"_Stz, // integer
  /*  7 */ "l"_Stz, // interval
  /*  8 */ "t"_Stz, // time
  /*  9 */ "p"_Stz, // timestamp
  /* 10 */ "s"_Stz, // varchar
  /* 11 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
