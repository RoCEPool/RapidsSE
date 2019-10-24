/*============================================================================*/
/* eFK.h -- Failure/Exception Kind                                            */
/*============================================================================*/
#pragma once

enum eFK // failure kind
{
  /* 0 */ FK_NONE=0,
  
  /* 1 */ FK_ASSERT,
  /* 2 */ FK_FAILED,
  /* 3 */ FK_FATAL,
  /* 4 */ FK_REACHED,
  
  /* 5 */ FK_ASTACK,
  /* 6 */ FK_DSTACK,
  /* 7 */ FK_RSTACK,
  
  /* 8 */ FK_ENUMS
};

static Stz* aFK[]
{
  /* 0 */ "NONE"_Stz,
  
  /* 1 */ "ASSERT"_Stz,
  /* 2 */ "FAILED"_Stz,
  /* 3 */ "FATAL"_Stz,
  /* 4 */ "REACHED"_Stz,

  /* 5 */ "ASTACK"_Stz,
  /* 6 */ "DSTACK"_Stz,
  /* 7 */ "RSTACK"_Stz,

  /* 8 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
