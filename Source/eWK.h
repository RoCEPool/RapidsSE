/*============================================================================*/
/* eWK.h -- Word Kind                                                         */
/*============================================================================*/
#pragma once

enum eWK // word kind
{
  /*  0 */ WK_NONE=0,     // none
  /*  1 */ WK_CPPOBJECT,  // word is a  c++ object in m_pObject
  /*  2 */ WK_EMBOBJECT,  // word is an object alloted in routine
  /*  3 */ WK_INTRINSIC,  // word is an intrinsic function (eg +, return)
  /*  4 */ WK_LOCAL,      // word is a  locals| | variable
  /*  5 */ WK_ROUTINE,    // word is a  : routine ... ; in m_xRoutine
  /*  6 */ WK_ENUMS
};

static Stz* aWK[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "CPPOBJECT"_Stz,
  /*  2 */ "EMBOBJECT"_Stz,
  /*  3 */ "INTRINSIC"_Stz,
  /*  4 */ "LOCAL"_Stz,
  /*  5 */ "ROUTINE"_Stz,
  /*  6 */ "ENUMS"_Stz,
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
