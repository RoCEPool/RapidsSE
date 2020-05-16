/*============================================================================*/
/* Hardare.hpp -- machine hardware                                           */
/*============================================================================*/
#pragma once

namespace Host
{
  class Hardware
  {    
  public:
    static i8 iClockResNsec(void);
    static i8 iCoreCount(void);
    static i8 iCpuCount(void);
    static i8 iCpuFrequency(void);
    static i8 iPhysicalMemory(void);
    
    static void vHostName(s32& sName);
    static void vLoginName(s32& sName);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
