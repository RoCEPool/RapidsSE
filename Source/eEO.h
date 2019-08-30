/*============================================================================*/
/* eEO.h -- Expression Operators                                              */
/*============================================================================*/
#pragma once

enum eEO // expression operators
{
  /* 0 */ EO_NONE=0,
  
  /* 1 */ EO_ADD,
  /* 2 */ EO_COLUMN,
  /* 3 */ EO_DIVIDE,
  /* 4 */ EO_INTEGER,
  /* 5 */ EO_MULTIPLY,
  /* 6 */ EO_NEGATE,
  /* 7 */ EO_RETURN,
  /* 8 */ EO_SUBTRACT,
  
  /* 9 */ EO_ENUMS
};

static Stz* aEO[]
{
  /* 0 */ "NONE"_Stz,
  
  /* 1 */ "EO_ADD"_Stz,
  /* 2 */ "EO_COLUMN"_Stz,
  /* 3 */ "EO_DIVIDE"_Stz,
  /* 4 */ "EO_INTEGER"_Stz,
  /* 5 */ "EO_MULTIPLY"_Stz,
  /* 6 */ "EO_NEGATE"_Stz,
  /* 7 */ "EO_RETURN"_Stz,
  /* 8 */ "EO_SUBTRACT"_Stz,

  /* 9 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
