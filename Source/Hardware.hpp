/*============================================================================*/
/* Hardare.hpp -- machine hardware                                           */
/*============================================================================*/
#pragma once

namespace System
{
#ifdef __APPLE__  
  #define SYSCTL_CORE_COUNT   "machdep.cpu.core_count"
#endif 
  
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
