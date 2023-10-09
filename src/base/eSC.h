/*============================================================================*/
/* eSC.h - Library Call kind                                                   */
/*============================================================================*/
#pragma once

enum eSC // system call
{
  /* -2 */ SC_NOTSET  = -2, // status code not yet set
  /* -1 */ SC_ERROR   = -1, // error status
  /* 0  */ SC_FAILURE = 0,  // failure status
  /* 1  */ SC_SUCCESS = 1,  // success status
};

const i8 C_SC_ENUMS = 4;

extern Stz* aSC[C_SC_ENUMS];

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
