/*============================================================================*/
/* Process.hpp -- process utility                                             */
/*============================================================================*/
#pragma once

namespace Host
{
  class Process
  {
  public:
    static void vEntry(void);
    static void vExit(i8 iExitStatus);
    static void vExitFailure(void);
    static void vExitSuccess(void);
    static i8   iGetPid(void);
    static bool bKillPid(i8 iPid);
    static bool bTestPid(i8 iPid);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
