/*============================================================================*/
/* Hardare.hpp -- machine hardware                                           */
/*============================================================================*/
#pragma once

namespace Host
{
  class Hardware
  {    
  public:
    static i8 iByteOrder(void);
    static i8 iCacheLine(void);
    static i8 iClockResNsec(void);
    static i8 iCoreCount(void);
    static i8 iCpuCount(void);
    static i8 iCpuFrequency(void);
    static i8 iL1Cache(void);
    static i8 iL2Cache(void);
    static i8 iL3Cache(void);
    static i8 iPhysicalMemory(void);
    static i8 iReadClock(void);
    static i8 iReadTscp(void);

    static void vHostName(s32& sName);
    static void vLoginName(s32& sName);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
