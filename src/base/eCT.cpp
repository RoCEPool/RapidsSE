/*============================================================================*/
/* eCT.cpp -- Column Type                                                     */
/*============================================================================*/
#include "BASE.h"
#include "eCT.h"

Stz* aCT[CT_ENUMS]
{
    /*  0 */ "NONE"_Stz,
    /*  1 */ "BOOLEAN"_Stz,
    /*  2 */ "CHAR"_Stz,
    /*  3 */ "DATE"_Stz,
    /*  4 */ "DECIMAL"_Stz,
    /*  5 */ "FLOAT"_Stz,
    /*  6 */ "INTEGER"_Stz,
    /*  7 */ "INTERVAL_DT"_Stz,
    /*  8 */ "INTERVAL_YM"_Stz,
    /*  9 */ "TIME"_Stz,
    /* 10 */ "TIMESTAMP"_Stz,
    /* 11 */ "VARCHAR"_Stz,
    /* 12 */ "NULL"_Stz,
//    /* 13 */ "ENUMS"_Stz
};

Stz* szCT_char[CT_ENUMS]
{
  /*  0 */ "!"_Stz,
  /*  1 */ "b"_Stz, // boolean
  /*  2 */ "c"_Stz, // char
  /*  3 */ "d"_Stz, // date
  /*  4 */ "l"_Stz, // decimal
  /*  5 */ "f"_Stz, // float
  /*  6 */ "i"_Stz, // integer
  /*  7 */ "s"_Stz, // interval day to second
  /*  8 */ "m"_Stz, // interval year to month
  /*  9 */ "t"_Stz, // time
  /* 10 */ "p"_Stz, // timestamp
  /* 11 */ "v"_Stz, // varchar
  /* 12 */ "x"_Stz, // unknown
//  /* 13 */ "?"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
