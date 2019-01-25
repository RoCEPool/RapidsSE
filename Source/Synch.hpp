/*============================================================================*/
/* Synch.hpp -- Threading independent synchronisation                          */
/*============================================================================*/
#pragma once

namespace System
{
  class Synch
  {
  public:
    static u8   uAtomicAdd(i8* pWord, i8 iValue);
    static u8   uAtomicDec(i8* pWord);
    static u8   uAtomicInc(i8* pWord);
    
    static bool bTestAndSet(u8* pWord, u8 uIf, u8 uThen);
        
    static u8   iLockDropRead(u8*);
    static u8   iLockGrabRead(u8*);
    static u8   iLockDropWrite(u8*);
    static u8   iLockGrabWrite(u8*);
    static u8   iLockIsLocked(u8*);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
