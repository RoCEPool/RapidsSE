/*============================================================================*/
/* eFK.h -- Failure/Exception Kind                                            */
/*============================================================================*/
#pragma once

enum eFK // failure kind
{
  /* 0 */ FK_NONE=0,
  
  /* 1 */ FK_ALWAYS,
  /* 2 */ FK_ASSERT,
  /* 3 */ FK_CHECK,
  /* 4 */ FK_FATAL,
  /* 5 */ FK_LOWMEMORY,
  /* 6 */ FK_RASSERT,
  /* 7 */ FK_REACHED,
  /* 8 */ FK_THROW,
  /* 9 */ FK_TYPE,
  /*10 */ FK_UNIMPLEMENTED,

  /*11 */ FK_ASTACK,
  /*12 */ FK_DSTACK,
  /*13 */ FK_RSTACK,
  
  /*14 */ FK_ENUMS
};

static Stz* aFK[]
{
  /* 0 */ "NONE"_Stz,
  
  /* 1 */ "ALWAYS"_Stz,
  /* 2 */ "ASSERT"_Stz,
  /* 3 */ "CHECK"_Stz,
  /* 4 */ "FATAL"_Stz,
  /* 5 */ "LOWMEMORY"_Stz,
  /* 6 */ "RASSERT"_Stz,
  /* 7 */ "REACHED"_Stz,
  /* 8 */ "THROW"_Stz,
  /* 9 */ "TYPE"_Stz,
  /*10 */ "UNIMPLEMENTED"_Stz,

  /*11 */ "ASTACK"_Stz,
  /*12 */ "DSTACK"_Stz,
  /*13 */ "RSTACK"_Stz,

  /*14 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
