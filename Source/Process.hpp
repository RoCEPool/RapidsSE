/*============================================================================*/
/* Process.hpp -- process utility                                             */
/*============================================================================*/
#pragma once

namespace System
{
  class Process
  {
  public:
    static void vExit(i8 iExitStatus);
    static i8   iGetPid(void);
    static bool bKillPid(i8 iPid);
    static bool bTestPid(i8 iPid);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
