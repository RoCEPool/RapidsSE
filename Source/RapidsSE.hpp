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
  
    static void vAttachObjects(void);
    static void vChangeDirectory(void);
    static void vCheckPort(void);
    static void vCreateObjects(void);
    static void vDicCompile(NP_Task npBootTask);
    static void vErrorHandler(Error* pError);
    static void vExecuteCmdLine(void);
    static void vExecuteSystem(void);
    static void vInitJumboHeap(void);
    static void vKillPrev(void);
    static void vPoolClose(void);
    static void vPoolOpen(void);
    static bool bPoolOpen(StrVal& sPoolName);
    static void vPrintExit(void);
    static void vPrintSizes(void);
    static void vPrintWelcome(void);
    static bool bRunUtilities(int argc, Stz* argv[]);
    static void vSEitfDelete(SEitf* pSEitf);
    static void vSetGlobals(void);
    static void vSignalHandler(int iSignal);
    static void vSpawnPrompt(void);
    static void vStartSystemTasks(void);
    static void vSystemInit(void);
    static void vSystemPrompt(void);
    static void vSystemStart(void);
    static void vSystemStatus(void);
    static void vSystemStop(void);
    static void vSystemUnlink(void);
    static void vWaitFlag(void);
    static void vWriteRegionName(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
