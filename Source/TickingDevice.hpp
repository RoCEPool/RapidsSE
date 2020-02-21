/*============================================================================*/
/* TickingDevice.hpp -- TickingDevice reader (Hardward Clock or Time Stamp Counter              */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class TickingDevice
  {
  public:
    static i8 iGetTick(void);
  
    static i8 iTicksPerSecond(void);
  
    static i8 iToMsec(i8 iTickDelta);
    static i8 iToNsec(i8 iTickDelta);
    static i8 iToUsec(i8 iTickDelta);
  
  //private:
    static i8 iClockResNsec(void);
    static i8 iReadClock(void);
    static i8 iReadTscp(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
