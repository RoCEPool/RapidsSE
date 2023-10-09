/*============================================================================*/
/* eFK.cpp -- Failure/Exception Kind                                          */
/*============================================================================*/
#include "BASE.h"
#include "eFK.h"

Stz* aFK[FK_ENUMS]
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

//  /*15 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
