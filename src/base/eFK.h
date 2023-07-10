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
  /* 5 */ FK_INTERNALERROR,
  /* 6 */ FK_LOWMEMORY,
  /* 7 */ FK_RASSERT,
  /* 8 */ FK_REACHED,
  /* 9 */ FK_THROW,
  /*10 */ FK_TYPE,
  /*11 */ FK_UNIMPLEMENTED,

  /*12 */ FK_ASTACK,
  /*13 */ FK_DSTACK,
  /*14 */ FK_RSTACK,
  
  /*15 */ FK_ENUMS
};

static Stz* aFK[]
{
  /* 0 */ "NONE"_Stz,
  
  /* 1 */ "ALWAYS"_Stz,
  /* 2 */ "ASSERT"_Stz,
  /* 3 */ "CHECK"_Stz,
  /* 4 */ "FATAL"_Stz,
  /* 5 */ "INTERNALERROR"_Stz,
  /* 6 */ "LOWMEMORY"_Stz,
  /* 7 */ "RASSERT"_Stz,
  /* 8 */ "REACHED"_Stz,
  /* 9 */ "THROW"_Stz,
  /*10 */ "TYPE"_Stz,
  /*11 */ "UNIMPLEMENTED"_Stz,

  /*12 */ "ASTACK"_Stz,
  /*13 */ "DSTACK"_Stz,
  /*14 */ "RSTACK"_Stz,

  /*15 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
