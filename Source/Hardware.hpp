/*============================================================================*/
/* Hardare.hpp -- machine hardware                                           */
/*============================================================================*/
#pragma once

namespace System
{
  class Hardware
  {    
  public:
    static i8 iClockFrequency(void);
    static i8 iCoreCount(void);
    static i8 iCpuCount(void);
    static i8 iPhysicalMemory(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
