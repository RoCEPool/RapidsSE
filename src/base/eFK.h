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
  /* 4 */ FK_LOWMEMORY,
  /* 5 */ FK_RASSERT,
  /* 6 */ FK_REACHED,
  /* 7 */ FK_THROW,
  /* 8 */ FK_TYPE,

  /* 9 */ FK_ASTACK,
  /*10 */ FK_DSTACK,
  /*11 */ FK_RSTACK,
  
  /*12 */ FK_ENUMS
};

static Stz* aFK[]
{
  /* 0 */ "NONE"_Stz,
  
  /* 1 */ "ASSERT"_Stz,
  /* 2 */ "CHECK"_Stz,
  /* 3 */ "FATAL"_Stz,
  /* 4 */ "LOWMEMORY"_Stz,
  /* 5 */ "RASSERT"_Stz,
  /* 6 */ "REACHED"_Stz,
  /* 7 */ "THROW"_Stz,
  /* 8 */ "TYPE"_Stz,

  /* 9 */ "ASTACK"_Stz,
  /*10 */ "DSTACK"_Stz,
  /*11 */ "RSTACK"_Stz,

  /*12 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
