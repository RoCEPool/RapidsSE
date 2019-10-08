/*============================================================================*/
/* eWT.h -- SingleWord Type                                                         */
/*============================================================================*/
#pragma once

enum eWT // word type
{
  /*  0 */ WT_NONE=0,     // none
  /*  3 */ WT_INTRINSIC,  // word is a intrinsic function (eg +, return)
  /*  1 */ WT_LOCAL,      // word is a local
  /*  2 */ WT_OBJECT,     // word is an object
  /*  4 */ WT_ROUTINE,    // word is a routine
  /*  5 */ WT_ENUMS
};

static Stz* aWT[]
{
  /*  0 */ "NONE"_Stz,
  /*  3 */ "INTRINSIC"_Stz,
  /*  1 */ "LOCAL"_Stz,
  /*  2 */ "OBJECT"_Stz,
  /*  4 */ "ROUTINE"_Stz,
  /*  5 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
