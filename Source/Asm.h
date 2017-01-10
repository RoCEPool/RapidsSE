/*****************************************************************************/
/* Asm.h -- assembler routines                                               */
/*****************************************************************************/

#pragma once

#include "Platform.h"

namespace Platform
{
  //extern "C" u8 iRdtscp(void);
  extern "C" u8 iAtomicDec(i8*);
  extern "C" u8 iAtomicInc(i8*);
  extern "C" u8 iCpuid(u8 iEax, u8 iReg);
  extern "C" u8 iDropRead(u8*);
  extern "C" u8 iGrabRead(u8*);
  extern "C" u8 iDropWrite(u8*);
  extern "C" u8 iGrabWrite(u8*);
  extern "C" u8 iIsLocked(u8*);
  extern "C" u8 iTestSet(u8* pU64, u8 iIf, u8 iThen);
}

/*****************************************************************************/
/* end of file                                                               */
/*****************************************************************************/
