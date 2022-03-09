/*============================================================================*/
/* ePJ.h -- project kind                                                      */
/*============================================================================*/
#pragma once

enum ePE // predicate expression
{
  /*   */ PE_NONE=0,
  /*   */ PE_AGG, // aggregate
  /*   */ PE_ALL, // count *
  /*   */ PE_BET, // between
  /*   */ PE_CNT, // count
  /*   */ PE_CSE, // case
  /*   */ PE_CST, // cast
  /*   */ PE_EXP, // expression
  /*   */ PE_EXT, // extract
  /*   */ PE_GBY, // group by (TO GO)
  /*   */ PE_GKY, // group by key
  /*   */ PE_LK2, // like with 2 params
  /*   */ PE_LK3, // like with 3 params
  /*   */ PE_MAX, // max
  /*   */ PE_MIN, // min
  /*   */ PE_MLE, // match leading ABD%
  /*   */ PE_MMD , // match middle %ABC%
  /*   */ PE_MM1, // match with one & in the middle %ABC%DEF%
  /*   */ PE_MM2, // match with 2 % in the middle %ABC%DEF%GHI%
  /*   */ PE_MTR, // match trailing %ABC
  /*   */ PE_INX, // in expression list
  /*   */ PE_SRT, // sort
  /*   */ PE_SS2, // substring with 2 params
  /*   */ PE_SS3, // substring with 3 params
  /*   */ PE_SUM, // sum
  /*   */ PE_TOP, // top
  /*   */ PE_XDY, // extract day
  /*   */ PE_XHR, // extract hour
  /*   */ PE_XMN, // extract min
  /*   */ PE_XMT, // extract month
  /*   */ PE_XSC, // extract second
  /*   */ PE_XYR, // extract year
  /*   */ PE_ENUMS
};

static Stz* aPE[]
{
  /*   */ "NONE"_Stz,
  /*   */ "AGG"_Stz,
  /*   */ "ALL"_Stz,
  /*   */ "BET"_Stz,
  /*   */ "CNT"_Stz,
  /*   */ "CSE"_Stz,
  /*   */ "CST"_Stz,
  /*   */ "EXP"_Stz,
  /*   */ "EXT"_Stz,
  /*   */ "GBY"_Stz,
  /*   */ "GKY"_Stz,
  /*   */ "LK2"_Stz,
  /*   */ "LK3"_Stz,
  /*   */ "MAX"_Stz,
  /*   */ "MIN"_Stz,
  /*   */ "MLE"_Stz,
  /*   */ "MMD"_Stz,
  /*   */ "MM1"_Stz,
  /*   */ "MM2"_Stz,
  /*   */ "MTR"_Stz,
  /*   */ "INX"_Stz,
  /*   */ "SRT"_Stz,
  /*   */ "SS2"_Stz,
  /*   */ "SS3"_Stz,
  /*   */ "SUM"_Stz,
  /*   */ "TOP"_Stz,
  /*   */ "XDY"_Stz,
  /*   */ "XHR"_Stz,
  /*   */ "XMN"_Stz,
  /*   */ "XMT"_Stz,
  /*   */ "XSC"_Stz,
  /*   */ "XYR"_Stz,
  /*   */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
