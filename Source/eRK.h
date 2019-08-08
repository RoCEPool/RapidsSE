/*============================================================================*/
/* eRK.h -- Resource                                                          */
/*============================================================================*/
#pragma once

enum eRK // Resource kind
{
  RK_NONE,
  
  RK_DIR,
  RK_FILE,
  RK_MALLOC,
  RK_MUTEX,
  RK_PTHREAD,
  RK_SHM,
  RK_SOCKET,
  
  RK_ENUMS
};
  
static Stz* aRK[]
{
  "NONE"_Stz,
  
  "DIR"_Stz,
  "FILE"_Stz,
  "MALLOC"_Stz,
  "MUTEX"_Stz,
  "PTHREAD"_Stz,
  "SHM"_Stz,
  "SOCKET"_Stz,
  
  "ENUMS"_Stz,
};

static Stz* aRK_Short[]
{
  "NONE"_Stz,

  "DIR"_Stz,
  "FIL"_Stz,
  "MEM"_Stz,
  "MTX"_Stz,
  "SHM"_Stz,
  "SKT"_Stz,
  "THD"_Stz,

  "ENUMS"_Stz,
};
  
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
