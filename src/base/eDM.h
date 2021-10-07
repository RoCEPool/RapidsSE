/*============================================================================*/
/* eDM.h -- Dtm Kind                                                          */
/*============================================================================*/
#pragma once

#ifdef __TINYC__

// when being included by the tcc compiler
enum eDM // expression operators

#else

// when not being included by the tcc compiler
enum eDM : u1 // Dtm Kind

#endif

{
  /*  0 */ DM_NONE=0,
  
  /*  1 */ DM_BCD,
  /*  2 */ DM_BOL,
  /*  3 */ DM_DTE,
  /*  4 */ DM_FLT,
  /*  5 */ DM_INT,
  /*  6 */ DM_IVL,
  /*  7 */ DM_IVY,
  /*  8 */ DM_TME,
  /*  9 */ DM_TSP,
  /* 10 */ DM_VCH,
  /* 11 */ DM_UNK,

  /* 12 */ DM_ENUMS
};

#ifndef __TINYC__

// when not being included by the tcc compiler
static Stz* aDM[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "BCD"_Stz,
  /*  2 */ "BOL"_Stz,
  /*  3 */ "DTE"_Stz,
  /*  4 */ "FLT"_Stz,
  /*  5 */ "INT"_Stz,
  /*  6 */ "IVL"_Stz,
  /*  7 */ "IVY"_Stz,
  /*  8 */ "TME"_Stz,
  /*  9 */ "TSP"_Stz,
  /* 10 */ "VCH"_Stz,
  /* 11 */ "UNK"_Stz,

  /* 12 */ "ENUMS"_Stz,
};
#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
