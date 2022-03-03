/*============================================================================*/
/* eCT.h -- Column Type                                                      */
/*============================================================================*/
#pragma once

// NOTE: Any additions/subtractions or reordering of eCT will affect the expression type
// calculus in a bunch of files under store/parser/shadowNodes/expressions/operators

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

static Stz* aCT[]
{
    /*  0 */ "NONE"_Stz,
    /*  1 */ "BOOLEAN"_Stz,
    /*  2 */ "CHAR"_Stz,
    /*  3 */ "DATE"_Stz,
    /*  4 */ "DECIMAL"_Stz,
    /*  5 */ "FLOAT"_Stz,
    /*  6 */ "INTEGER"_Stz,
    /*  7 */ "INTERVAL DAY TO SECOND"_Stz,
    /*  8 */ "INTERVAL YEAR TO MONTH"_Stz,
    /*  9 */ "TIME"_Stz,
    /* 10 */ "TIMESTAMP"_Stz,
    /* 11 */ "VARCHAR"_Stz,
    /* 12 */ "NULL"_Stz,
    /* 13 */ "ENUMS"_Stz
};

static Stz* aCT_long[]
{
  /*  0 */ "CT_NONE"_Stz,
  /*  1 */ "CT_BOOLEAN"_Stz,
  /*  2 */ "CT_CHAR"_Stz,
  /*  3 */ "CT_DATE"_Stz,
  /*  4 */ "CT_DECIMAL"_Stz,
  /*  5 */ "CT_FLOAT"_Stz,
  /*  6 */ "CT_INTEGER"_Stz,
  /*  7 */ "CT_INTERVAL_DT"_Stz,
  /*  8 */ "CT_INTERVAL_YM"_Stz,
  /*  9 */ "CT_TIME"_Stz,
  /* 10 */ "CT_TIMESTAMP"_Stz,
  /* 11 */ "CT_VARCHAR"_Stz,
  /* 12 */ "CT_UNKNOWN"_Stz,
  /* 13 */ "CT_ENUMS"_Stz
};

static Stz* aCT_short[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "b"_Stz,  // boolean
  /*  2 */ "c"_Stz,  // char
  /*  3 */ "d"_Stz,  // date
  /*  4 */ "m"_Stz,  // decimal
  /*  5 */ "f"_Stz,  // float
  /*  6 */ "i"_Stz,  // integer
  /*  7 */ "ld"_Stz, // interval day to second
  /*  8 */ "ly"_Stz, // interval year to month
  /*  9 */ "t"_Stz,  // time
  /* 10 */ "p"_Stz,  // timestamp
  /* 11 */ "s"_Stz,  // varchar
  /* 12 */ "x"_Stz,  // unknown
  /* 13 */ "ENUMS"_Stz
};

static char cCT_char[]
{
  /*  0 */ '!',
  /*  1 */ 'b', // boolean
  /*  2 */ 'c', // char
  /*  3 */ 'd', // date
  /*  4 */ 'm', // decimal
  /*  5 */ 'f', // float
  /*  6 */ 'i', // integer
  /*  7 */ 'l', // interval day to second
  /*  8 */ 'y', // interval year to month
  /*  9 */ 't', // time
  /* 10 */ 'p', // timestamp
  /* 11 */ 's', // varchar
  /* 12 */ 'x', // unknown
  /* 13 */ '?'
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
