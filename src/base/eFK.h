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

extern Stz* aFK[FK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
