/*============================================================================*/
/* eFK.h -- Failure/Exception Kind                                            */
/*============================================================================*/
#pragma once

enum eFK // failure kind
{
  /* 0 */ FK_NONE=0,
  
  /* 1 */ FK_ASSERT,
  /* 2 */ FK_CHECK,
  /* 3 */ FK_FATAL,
  /* 4 */ FK_RASSERT,
  /* 5 */ FK_REACHED,
  /* 6 */ FK_THROW,
  /* 7 */ FK_TYPE,

  /* 8 */ FK_ASTACK,
  /* 9 */ FK_DSTACK,
  /*10 */ FK_RSTACK,
  
  /*11 */ FK_ENUMS
};

static Stz* aFK[]
{
  /* 0 */ "NONE"_Stz,
  
  /* 1 */ "ASSERT"_Stz,
  /* 2 */ "CHECK"_Stz,
  /* 3 */ "FATAL"_Stz,
  /* 4 */ "RASSERT"_Stz,
  /* 5 */ "REACHED"_Stz,
  /* 6 */ "THROW"_Stz,
  /* 7 */ "TYPE"_Stz,

  /* 8 */ "ASTACK"_Stz,
  /* 9 */ "DSTACK"_Stz,
  /*10 */ "RSTACK"_Stz,

  /*11 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
