/*============================================================================*/
/* eSR.h -- Library Resource                                                   */
/*============================================================================*/
#pragma once

enum eSR // Library resource
{
  SR_NONE,
  
  SR_DIR,
  SR_FILE,
  SR_MALLOC,
  SR_MUTEX,
  SR_PTHREAD,
  SR_SHM,
  SR_SOCKET,
  
  SR_ENUMS
};
  
static Stz* aSR[]
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

static Stz* aSR_Short[]
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
