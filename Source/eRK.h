/*============================================================================*/
/* eRK.h -- Resource                                                          */
/*============================================================================*/
#pragma once

enum eRK // Resource kind
{
  RK_NONE,
  
  RK_DIR,
  RK_FILE,
  RK_MUTEX,
  RK_PIPE,
  RK_PTHREAD,
  RK_SOCKET,
  
  RK_ENUMS
};
  
static Stz* aRK[]
{
  "NONE"_Stz,
  
  "DIR"_Stz,
  "FILE"_Stz,
  "MUTEX"_Stz,
  "PIPE"_Stz,
  "PTHREAD"_Stz,
  "SOCKET"_Stz,
  
  "ENUMS"_Stz,
};

static Stz* aRK_Short[]
{
  "NONE"_Stz,

  "DIR"_Stz,
  "FIL"_Stz,
  "MTX"_Stz,
  "PIP"_Stz,
  "PTH"_Stz,
  "SKT"_Stz,

  "ENUMS"_Stz,
};
  
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
