/*============================================================================*/
/* eCM.h -- Compiler Mode                                                     */
/*============================================================================*/
#pragma once

enum eCM // compiler mode
{
  /* 0 */ CM_NONE=0,
  
  /* 1 */ CM_COMPILING,
  /* 2 */ CM_CONDBEGIN,
  /* 4 */ CM_CONDCOMPILE,
  /* 3 */ CM_CONDDISCARD,
  /* 5 */ CM_INTERPRETING,
  /* 6 */ CM_POSTPONING,

  /* 7 */ CM_ENUMS
};

static Stz* aCM[]
{
  /* 0 */ "CM_NONE"_Stz,
  
  /* 1 */ "CM_COMPILING"_Stz,
  /* 2 */ "CM_CONDBEGIN"_Stz,
  /* 3 */ "CM_CONDCOMPILE"_Stz,
  /* 4 */ "CM_CONDDISCARD"_Stz,
  /* 5 */ "CM_INTERPRETING"_Stz,
  /* 6 */ "CM_POSTPONING"_Stz,

  /* 7 */ "CM_ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
