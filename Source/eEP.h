/*============================================================================*/
/* eEP.h -- Task Entry Point                                                  */
/*============================================================================*/
#pragma once

enum eEP // Entry Point
{
  /*  0 */ EP_NONE=0,
  /*  1 */ EP_DEDICATED,
  /*  2 */ EP_CONNECTOR,
  /*  3 */ EP_OP,
  /*  4 */ EP_OTHER,
  /*  5 */ EP_PROMPT,
  /*  6 */ EP_RECEIVED,
  /*  7 */ EP_SYSTEM,
  /*  8 */ EP_NEXTPHASE,
  /*  9 */ EP_ENUMS
};

static Stz* aEP[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "DEDICATED"_Stz,
  /*  2 */ "CONNECTOR"_Stz,
  /*  3 */ "OP"_Stz,
  /*  4 */ "OTHER"_Stz,
  /*  5 */ "PROMPT"_Stz,
  /*  6 */ "RECEIVED"_Stz,
  /*  7 */ "SYSTEM"_Stz,
  /*  8 */ "NEXTPHASE"_Stz,
  /*  9 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
