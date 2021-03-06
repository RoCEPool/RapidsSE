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
  // TODO: CT_INTERVAL_YM,  // year-month interval
  // TODO: CT_INTERVAL_DT,  // day-time interval
  /*  7 */ CT_INTERVAL_DT,     // TODO: Replace with the above
  /*  8 */ CT_TIME,
  /*  9 */ CT_TIMESTAMP,
  /* 10 */ CT_VARCHAR,
  // TODO: CT_UNKNOWN,
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
    // TODO: "INTERVAL YEAR TO MONTH"_Stz,
    // TODO: "INTERVAL DAY TO SECOND"_Stz,
    /*  7 */ "INTERVAL"_Stz,  // TODO: replace with the above
    /*  8 */ "TIME"_Stz,
    /*  9 */ "TIMESTAMP"_Stz,
    /* 10 */ "VARCHAR"_Stz,
    // TODO: "NULL"_Stz,
    /* 11 */ "ENUMS"_Stz
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
  // TODO: "CT_INTERVAL_YM"_Stz,
  // TODO: "CT_INTERVAL_DT"_Stz,
  /*  7 */ "CT_INTERVAL_DT"_Stz,   // TODO: replace with the above
  /*  8 */ "CT_TIME"_Stz,
  /*  9 */ "CT_TIMESTAMP"_Stz,
  /* 10 */ "CT_VARCHAR"_Stz,
  // TODO: "CT_UNKNOWN"_Stz,
  /* 11 */ "CT_ENUMS"_Stz
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
  // TODO: "ly"_Stz, // (???) interval year to month
  // TODO: "ld"_Stz, // (???) interval day to second
  /*  7 */ "ld"_Stz, // interval   // TODO: replace with the above
  /*  8 */ "t"_Stz, // time
  /*  9 */ "p"_Stz, // timestamp
  /* 10 */ "s"_Stz, // varchar
  // TODO: "x"_Stz, // unknown (???)
  /* 11 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
