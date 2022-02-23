/*============================================================================*/
/* ePJ.h -- project kind                                                      */
/*============================================================================*/
#pragma once

enum ePE // predicate expression
{
  /*  0 */ PE_NONE=0,
  /*  1 */ PE_AGG, // aggregate
  /*  2 */ PE_ALL, // count *
  /*  3 */ PE_BET, // between
  /*  4 */ PE_CNT, // count
  /*  5 */ PE_CSE, // case
  /*  5 */ PE_CST, // cast
  /*  6 */ PE_EXP, // expression
  /*  7 */ PE_EXT, // extract
  /*  8 */ PE_GBY, // group by
  /*  9 */ PE_LIK, // like
  /* 10 */ PE_MAX, // max
  /* 11 */ PE_MIN, // min
  /* 12 */ PE_INX, // in expression list
  /* 13 */ PE_SUB, // substring
  /* 14 */ PE_SUM, // sum
  /* 15 */ PE_ENUMS
};

static Stz* aPE[]
{
    /*  0 */ "NONE"_Stz,
    /*  1 */ "AGG"_Stz,
    /*  2 */ "ALL"_Stz,
    /*  3 */ "BET"_Stz,
    /*  4 */ "CNT"_Stz,
    /*  4 */ "CSE"_Stz,
    /*  5 */ "CST"_Stz,
    /*  6 */ "EXP"_Stz,
    /*  7 */ "EXT"_Stz,
    /*  8 */ "GBY"_Stz,
    /*  9 */ "LIK"_Stz,
    /* 10 */ "MAX"_Stz,
    /* 11 */ "MIN"_Stz,
    /* 12 */ "INX"_Stz,
    /* 13 */ "SUB"_Stz,
    /* 14 */ "SUM"_Stz,
    /* 15 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
