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
  /*  6 */ PE_CST, // cast
  /*  7 */ PE_EXP, // expression
  /*  8 */ PE_EXT, // extract
  /*  9 */ PE_GBY, // group by (TO GO)
  /* 10 */ PE_GKY, // group by key
  /* 11 */ PE_LIK, // like
  /* 12 */ PE_MAX, // max
  /* 13 */ PE_MIN, // min
  /* 14 */ PE_INX, // in expression list
  /* 15 */ PE_SS2, // substring with 2 params
  /*    */ PE_SS3, // substring with 3 params
  /* 16 */ PE_SUM, // sum
  /* 17 */ PE_XDY, // extract day
  /* 18 */ PE_XHR, // extract hour
  /* 19 */ PE_XMN, // extract min
  /* 20 */ PE_XMT, // extract month
  /* 21 */ PE_XSC, // extract second
  /* 22 */ PE_XYR, // extract year
  /* 23 */ PE_ENUMS
};

static Stz* aPE[]
{
    /*   0 */ "NONE"_Stz,
    /*   1 */ "AGG"_Stz,
    /*   2 */ "ALL"_Stz,
    /*   3 */ "BET"_Stz,
    /*   4 */ "CNT"_Stz,
    /*   5 */ "CSE"_Stz,
    /*   6 */ "CST"_Stz,
    /*   7 */ "EXP"_Stz,
    /*   8 */ "EXT"_Stz,
    /*   9 */ "GBY"_Stz,
    /*  10 */ "GKY"_Stz,
    /*  11 */ "LIK"_Stz,
    /*  12 */ "MAX"_Stz,
    /*  13 */ "MIN"_Stz,
    /*  14 */ "INX"_Stz,
    /*  15 */ "SS2"_Stz,
    /*     */ "SS3"_Stz,
    /*  16 */ "SUM"_Stz,
    /*  17 */ "XDY"_Stz,
    /*  18 */ "XHR"_Stz,
    /*  19 */ "XMN"_Stz,
    /*  20 */ "XMT"_Stz,
    /*  21 */ "XSC"_Stz,
    /*  22 */ "XYR"_Stz,
    /*  23 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
