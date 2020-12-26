/*============================================================================*/
/* eCM.h -- Compiler Mode                                                     */
/*============================================================================*/
#pragma once

enum eCM // compiler mode
{
  /* 0 */ CM_NONE=0,
  
  /* 1 */ CM_COMPILING,
  /* 2 */ CM_CONDCOMPILING,
  /* 3 */ CM_DISCARDING,
  /* 4 */ CM_INTERPRETING,
  /* 5 */ CM_POSTPONING,

  /* 6 */ CM_ENUMS
};

static Stz* aCM[]
{
  /* 0 */ "CM_NONE"_Stz,
  
  /* 1 */ "CM_COMPILING"_Stz,
  /* 2 */ "CM_CONDCOMPILING"_Stz,
  /* 3 */ "CM_DISCARDING"_Stz,
  /* 4 */ "CM_INTERPRETING"_Stz,
  /* 5 */ "CM_POSTPONING"_Stz,

  /* 6 */ "CM_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
