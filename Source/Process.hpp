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
  
    [[ noreturn ]] static void vUnrecoverableError(Stz* szErrorMessage,
                                                   Stz* szInfoText);

    static i8   iGetPid(void);
    static bool bIsSeProcess(void);
    static bool bKillPid(i8 iPid);
    static bool bTestPid(i8 iPid);
  
    static i8   iResidentSize(void);
    static i8   iVirtualSize(void);
  
    static i8   iSystemUsec(void);
    static i8   iUserUsec(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
