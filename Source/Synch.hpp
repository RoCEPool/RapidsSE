/*============================================================================*/
/* Synch.hpp -- platform independent synchronisation                          */
/*============================================================================*/

#pragma once

#include "Platform.h"

namespace Platform
{
  class Synch
  {
  public:
    static u8   uAtomicDec(i8* pWord);
    static u8   uAtomicInc(i8* pWord);
    
    static bool bTestAndSet(u8* pWord, u8 uIf, u8 uThen);
    
    static i8   iTicks(void);
    static i8   iTickFrequency(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
