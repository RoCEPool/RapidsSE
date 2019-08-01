/*============================================================================*/
/* eTP.h -- Transaction Phase                                                 */
/*============================================================================*/
#pragma once

enum eTP // Transaction Phase
{
  /* 0 */ TP_NONE=0,
  /* 1 */ TP_EXECUTING,
  /* 2 */ TP_COMMITTING,
  /* 3 */ TP_COMMITTED,
  /* 4 */ TP_PREPARING,
  /* 5 */ TP_PREPARED,
  /* 6 */ TP_ROLLINGBACK,
  /* 7 */ TP_ROLLEDBACK,
  /* 8 */ TP_ENUMS
};

static Stz* aTP[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "EXECUTING"_Stz,
  /* 2 */ "COMMITTING"_Stz,
  /* 3 */ "COMMITTED"_Stz,
  /* 4 */ "PREPARING"_Stz,
  /* 5 */ "PREPARED"_Stz,
  /* 6 */ "ROLLINGBACK"_Stz,
  /* 7 */ "ROLLEDBACK"_Stz,
  /* 8 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
