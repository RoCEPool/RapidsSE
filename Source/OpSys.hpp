/*============================================================================*/
/* OpSys.hpp -- operating system utility                                      */
/*============================================================================*/
#pragma once

namespace Host
{
  class OpSys
  {
  public:
    static void vGetInfo(void);
    static i8   iSchedulerPeriod_us(void);
    static void vTraceInfo(void);
  
    static char* azMachine(void);
    static char* azNodeName(void);
    static char* azRelease(void);
    static char* azSysName(void);
    static char* azVersion(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
