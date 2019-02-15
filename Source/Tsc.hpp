/*============================================================================*/
/* Tsc.hpp -- Time Stamp Counter                                              */
/*============================================================================*/
#pragma once

namespace System
{
  class Tsc
  {
  public:
    static i8 iTicks(void);
    static i8 iTicksPerSecond(void);
    static i8 iToMsec(i8 iTickDelta);
    static i8 iToNsec(i8 iTickDelta);
    static i8 iToUsec(i8 iTickDelta);
    static i8 iRdTscp(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
