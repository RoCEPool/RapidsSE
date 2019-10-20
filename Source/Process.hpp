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
    [[ noreturn ]] static void vExit(i8 iExitStatus);
    [[ noreturn ]] static void vExitFailure(void);
    [[ noreturn ]] static void vExitSuccess(void);
    static i8   iGetPid(void);
    static bool bIsSeProcess(void);
    static bool bKillPid(i8 iPid);
    static bool bTestPid(i8 iPid);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
