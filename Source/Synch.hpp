/*============================================================================*/
/* Synch.hpp -- Threading independent synchronisation                          */
/*============================================================================*/
#pragma once

namespace System
{
  class Synch
  {
  public:
        
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
