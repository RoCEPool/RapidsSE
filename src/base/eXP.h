/*============================================================================*/
/* eXP.h -- Transaction Phase                                                 */
/*============================================================================*/
#pragma once

enum eXP // Transaction Phase
{
  /* 0 */ XP_NONE=0,
  /* 1 */ XP_EXECUTING,
  /* 2 */ XP_COMMITTING,
  /* 3 */ XP_COMMITTED,
  /* 4 */ XP_PREPARING,
  /* 5 */ XP_PREPARED,
  /* 6 */ XP_ROLLINGBACK,
  /* 7 */ XP_ROLLEDBACK,
  /* 8 */ XP_ENUMS
};

static Stz* aXP[]
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
