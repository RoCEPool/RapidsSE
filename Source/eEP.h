/*============================================================================*/
/* eEP.h -- Task Entry Point                                                  */
/*============================================================================*/
#pragma once

enum eEP // Entry Point
{
  /*  0 */ EP_NONE=0,
  /*  1 */ EP_DEDICATED,
  /*  2 */ EP_CONNECTOR,
  /*  3 */ EP_INTERNAL,
  /*  4 */ EP_OP,
  /*  5 */ EP_OTHER,
  /*  6 */ EP_PROMPT,
  /*  7 */ EP_RECEIVED,
  /*  8 */ EP_2PC,
  /*  9 */ EP_ENUMS
};

static Stz* aEP[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "DEDICATED"_Stz,
  /*  2 */ "CONNECTOR"_Stz,
  /*  3 */ "INTERNAL"_Stz,
  /*  4 */ "OP"_Stz,
  /*  5 */ "OTHER"_Stz,
  /*  6 */ "PROMPT"_Stz,
  /*  7 */ "RECEIVED"_Stz,
  /*  8 */ "2PC"_Stz,
  /*  9 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
