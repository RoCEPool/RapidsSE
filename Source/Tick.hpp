/*============================================================================*/
/* Tick.hpp -- Tick reader (Hardward Clock or Time Stamp Counter              */
/*============================================================================*/
#pragma once

namespace System
{
  class Tick
  {
  public:
    static i8 iClockRes(void);
  
    static i8 iRdClock(void);
    static i8 iRdTscp(void);
  
    static i8 iTicksPerSecond(void);
    static i8 iToMsec(i8 iTickDelta);
    static i8 iToNsec(i8 iTickDelta);
    static i8 iToUsec(i8 iTickDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
