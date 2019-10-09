/*============================================================================*/
/* eWK.h -- Word Kind                                                         */
/*============================================================================*/
#pragma once

enum eWK // word kind
{
  /*  0 */ WK_NONE=0,     // none
  /*  1 */ WK_INTRINSIC,  // word is a intrinsic function (eg +, return)
  /*  2 */ WK_LOCAL,      // word is a locals| | variable
  /*  3 */ WK_OBJECT,     // word is an object
  /*  4 */ WK_ROUTINE,    // word is a routine : word ... ;
  /*  5 */ WK_ENUMS
};

static Stz* aWK[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "INTRINSIC"_Stz,
  /*  2 */ "LOCAL"_Stz,
  /*  3 */ "OBJECT"_Stz,
  /*  4 */ "ROUTINE"_Stz,
  /*  5 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
