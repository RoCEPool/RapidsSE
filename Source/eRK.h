/*============================================================================*/
/* eRK.h -- Resource                                                          */
/*============================================================================*/
#pragma once

enum eRK // Resource kind
{
  RK_NONE,
  
  RK_DIR,
  RK_FIFO,
  RK_FILE,
  RK_MUTEX,
  RK_PTHREAD,
  RK_SOCKET,
  
  RK_ENUMS
};
  
static Stz* aRK[]
{
  "NONE"_Stz,
  
  "DIR"_Stz,
  "FIFO"_Stz,
  "FILE"_Stz,
  "MUTEX"_Stz,
  "PTHREAD"_Stz,
  "SOCKET"_Stz,
  
  "ENUMS"_Stz,
};

static Stz* aRK_Short[]
{
  "NONE"_Stz,

  "DIR"_Stz,
  "FIO"_Stz,
  "FIL"_Stz,
  "MTX"_Stz,
  "PTH"_Stz,
  "SKT"_Stz,

  "ENUMS"_Stz,
};
  
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
