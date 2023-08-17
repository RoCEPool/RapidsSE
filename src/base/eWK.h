/*============================================================================*/
/* eWK.h -- Word Kind                                                         */
/*============================================================================*/
#pragma once

enum eWK // word kind
{
  /*  0 */ WK_NONE=0,     // none
  /*  1 */ WK_CPPOBJECT,  // word is a  c++ object in m_pObject
  /*  2 */ WK_INTRINSIC,  // word is an intrinsic function (eg +, return)
  /*  3 */ WK_LOCAL,      // word is a  locals| | variable
  /*  4 */ WK_COLON,      // word is a  : routine ... ; in m_xRoutine
  /*  5 */ WK_ENUMS
};

extern Stz* aWK[WK_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
