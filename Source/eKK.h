/*============================================================================*/
/* eKK.h -- Stack Kind                                                        */
/*============================================================================*/
#pragma once

enum eKK // stack kind
{
  /* 0 */ KK_NONE=0,
  /* 1 */ KK_AUX_STACK,
  /* 2 */ KK_DATA_STACK,
  /* 3 */ KK_JOB_STACK,
  /* 4 */ KK_RETURN_STACK,
  /* 5 */ KK_PARSER_STACK,
  /* 6 */ KK_ENUMS
};

static Stz* aKK[]
{
  /* 0 */ "none"_Stz,
  /* 1 */ "AUX_STACK"_Stz,
  /* 2 */ "DATA_STACK"_Stz,
  /* 3 */ "JOB_STACK"_Stz,
  /* 4 */ "RETURN_STACK"_Stz,
  /* 5 */ "PARSER_STACK"_Stz,
  /* 6 */ "enums"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
