/*============================================================================*/
/* OpSys.hpp -- operating system utility                                      */
/*============================================================================*/
#pragma once

namespace Resources
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
  
    static void  vRLimitCore(i8& nSoft, i8& nHard);
    static void  vRLimitCpu(i8& nSoft, i8& nHard);
    static void  vRLimitData(i8& nSoft, i8& nHard);
    static void  vRLimitFileSize(i8& nSoft, i8& nHard);
    static void  vRLimitMemoryLocked(i8& nSoft, i8& nHard);
    static void  vRLimitOpenFiles(i8& nSoft, i8& nHard);
    static void  vRLimitProcesses(i8& nSoft, i8& nHard);
    static void  vRLimitRss(i8& nSoft, i8& nHard);
    static void  vRLimitStack(i8& nSoft, i8& nHard);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
