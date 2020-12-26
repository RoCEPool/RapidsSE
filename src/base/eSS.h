/*============================================================================*/
/* eSS.h -- Seed String                                                       */
/*============================================================================*/
#pragma once

enum eSS // seed string
{
  /*  0 */ SS_SEED_RUN=0,
  /*  1 */ SS_SEED_REQ,
  /*  2 */ SS_SEED_RCV,
  /*  3 */ SS_SEED_EXE,
  /*  4 */ SS_SEED_END,
  /*  5 */ SS_SEED_CAN,
  
  /*  6 */ SS_SEED_BEG,
  /*  7 */ SS_SEED_KID,

  /*  8 */ SS_ENUMS
};

static Stz* aSS[]
{
  /*  0 */ "SS_SEED_RUN"_Stz,
  /*  1 */ "SS_SEED_REQ"_Stz,
  /*  2 */ "SS_SEED_RCV"_Stz,
  /*  3 */ "SS_SEED_EXE"_Stz,
  /*  4 */ "SS_SEED_END"_Stz,
  /*  5 */ "SS_SEED_CAN"_Stz,
  
  /*  6 */ "SS_SEED_BEG"_Stz,
  /*  7 */ "SS_SEED_KID"_Stz,
  
  /*  8 */ "SS_ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
