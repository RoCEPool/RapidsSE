/*============================================================================*/
/* TickingDevice.hpp -- TickingDevice reader (Clock or Timestamp Counter)     */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class TickingDevice
  {
  public:
    static i8 iGetTick(void);
    static i8 iTicksPerSec(void);
  
    static i8 iToMsec(i8 iTickDelta);
    static i8 iToNsec(i8 iTickDelta);
    static i8 iToUsec(i8 iTickDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
