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

static Stz* aSC[]
{
  /* -2 */ "NOTSET"_Stz,
  /* -1 */ "ERROR"_Stz,
  /* 0  */ "FAILURE"_Stz,
  /* 1  */ "SUCCESS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
