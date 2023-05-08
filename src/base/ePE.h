/*============================================================================*/
/* ePE.h -- project kind                                                      */
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
  /*   */ PE_MMD, // match middle %ABC%
  /*   */ PE_MM1, // match with one % in the middle %ABC%DEF%
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
  /*   */ "PE_NONE"_Stz,
  /*   */ "PE_AGG"_Stz,
  /*   */ "PE_ALL"_Stz,
  /*   */ "PE_BET"_Stz,
  /*   */ "PE_CNT"_Stz,
  /*   */ "PE_CSE"_Stz,
  /*   */ "PE_CST"_Stz,
  /*   */ "PE_EXP"_Stz,
  /*   */ "PE_EXT"_Stz,
  /*   */ "PE_GBY"_Stz,
  /*   */ "PE_GKY"_Stz,
  /*   */ "PE_LK2"_Stz,
  /*   */ "PE_LK3"_Stz,
  /*   */ "PE_MAX"_Stz,
  /*   */ "PE_MIN"_Stz,
  /*   */ "PE_MLE"_Stz,
  /*   */ "PE_MMD"_Stz,
  /*   */ "PE_MM1"_Stz,
  /*   */ "PE_MM2"_Stz,
  /*   */ "PE_MTR"_Stz,
  /*   */ "PE_INX"_Stz,
  /*   */ "PE_SRT"_Stz,
  /*   */ "PE_SS2"_Stz,
  /*   */ "PE_SS3"_Stz,
  /*   */ "PE_SUM"_Stz,
  /*   */ "PE_TOP"_Stz,
  /*   */ "PE_XDY"_Stz,
  /*   */ "PE_XHR"_Stz,
  /*   */ "PE_XMN"_Stz,
  /*   */ "PE_XMT"_Stz,
  /*   */ "PE_XSC"_Stz,
  /*   */ "PE_XYR"_Stz,
  /*   */ "PE_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
