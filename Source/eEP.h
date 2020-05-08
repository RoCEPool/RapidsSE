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
  /*  4 */ EP_PROMPT,
  /*  5 */ EP_RECEIVED,
  /*  6 */ EP_SERVICE,
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
  /*  4 */ "PROMPT"_Stz,
  /*  5 */ "RECEIVED"_Stz,
  /*  6 */ "SERVICE"_Stz,
  /*  7 */ "SYSTEM"_Stz,
  /*  8 */ "NEXTPHASE"_Stz,
  /*  9 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
