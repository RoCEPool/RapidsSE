/*============================================================================*/
/* Asm.h -- assembler routines                                                */
/*============================================================================*/
#pragma once

namespace Assembler
{
  extern "C" u8 iAtomicDec(i8*); // use posix calls
  extern "C" u8 iAtomicInc(i8*);
  extern "C" u8 iCpuid(u8 iEax, u8 iReg); // not used
  extern "C" u8 iDropRead(u8*); // use iTestSet
  extern "C" u8 iDropWrite(u8*);
  extern "C" u8 iGrabRead(u8*);
  extern "C" u8 iGrabWrite(u8*);
  extern "C" u8 iIsLocked(u8*);
  extern "C" u8 iPmcInit(void);
  extern "C" u8 iRdPmc(u4 uWhichCounter);
  extern "C" u8 iRdtscp(void); // replace with clock_gettime
  extern "C" u8 iTestSet(u8* pU64, u8 iIf, u8 iThen);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  extern "C" void vPrefetch(void*);
  extern "C" u8 iTestPrefetch(void);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
