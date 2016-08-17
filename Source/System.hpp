/*============================================================================*/
/* System.hpp - system                                                        */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Common
{
#ifdef __APPLE__  
  #define SYSCTL_CORE_COUNT   "machdep.cpu.core_count"
#endif 
  
  class System
  {
  private:
    
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
