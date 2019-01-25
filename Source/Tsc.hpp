/*============================================================================*/
/* Tsc.hpp -- Time Stamp Counter                                              */
/*============================================================================*/
#pragma once

namespace System
{
  class Tsc
  {
  public:
    static i8   iTicks(void);
    static i8   iToMicroseconds(i8 iTickDelta);
    static i8   iTickFrequency(void);
    static i8   iRdTscp(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
