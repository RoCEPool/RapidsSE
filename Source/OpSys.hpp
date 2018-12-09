/*============================================================================*/
/* OpSys.hpp -- operating system utility                                      */
/*============================================================================*/
#pragma once

namespace System
{
  class OpSys
  {
  public:
    static void vGetInfo(void);
    static i8   iSchedulerPeriod_us(void);
    static void vTraceInfo(void);
  
    static char* aMachine(void);
    static char* aNodeName(void);
    static char* aRelease(void);
    static char* aSysName(void);
    static char* aVersion(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
