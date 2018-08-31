/*============================================================================*/
/* RapidsSE.hpp -- Rapids Storage Engine                                      */
/*============================================================================*/
#pragma once

#define RAPIDSSE_VERSION_STRING "0.4.1"

namespace StorageEngine
{
  typedef NP<Task,CN_TASK> NP_Task;
  
  class RapidsSE
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
  
    static void vAttachRegion(void);
    static void vChangeDirectory(void);
    static void vCheckPort(void);
    static void vCreateAttachMap(void);
    static void vDispatchJobStart(void);
    static void vErrorHandler(Error* pError);
    static void vKillPrev(void);
    static void vMainEntry(void);
    static void vPoolClose(void);
    static void vPoolOpen(void);
    static bool bPoolOpen(StrVal& sPoolName);
    static void vPrintExit(void);
    static void vPrintSizes(void);
    static void vPrintWelcome(void);
    static void vRunBoot(void);
    static void vRunCmdLine(void);
    static void vRunImmmediate(Task* pTask);
    static void vRunPrompt(void);
    static bool bRunUtilities(int argc, Stz* argv[]);
    static void vSEitfDelete(SEitf* pSEitf);
    static void vSetGlobals(void);
    static void vSignalHandler(int iSignal);
    static void vSystemStart(void);
    static void vSystemStatus(void);
    static void vSystemStop(void);
    static void vSystemUnlink(void);
    static void vSystemWait(void);
    static void vWriteRegionName(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
