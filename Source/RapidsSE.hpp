/*============================================================================*/
/* RapidsSE.hpp -- Rapids Storage Engine                                      */
/*============================================================================*/
#pragma once

#define RAPIDSSE_VERSION_STRING "0.4.1"

namespace StorageEngine
{
  class RapidsSE
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
  
    static void vAttachRegion(void);
    static void vCallBoot(void);
    static void vCallImmmediate(Task* pTask);
    static void vCallPrompt(void);
    static void vChangeDirectory(void);
    static void vCheckPort(void);
    static void vClosePool(void);
    static void vCreateAttachMap(void);
    static void vDeleteImmmediate(Task** ppTask);
    static void vDeleteSession(Session* pSession);
    static void vErrorHandler(Error* pError);
    static void vKillPrev(void);
    static void vMainEntry(void);
    static void vOpenActivityMap(void);
    static void vOpenPool(void);
    static bool bOpenPool(s32& sPoolName);
    static void vOpenTraceFile(void);
    static void vPrintExit(void);
    static void vPrintStatus(void);
    static void vPrintWelcome(void);
    static void vRunCmdLine(void);
    static bool bRunUtilities(int argc, Stz* argv[]);
    static void vSanityCheck(void);
    static void vSetGlobals(void);
    static void vSignalHandler(int iSignal);
    static void vSpawnJobStart(void);
    static void vStartSystem(void);
    static void vStopSystem(void);
    static void vUnlinkSystem(void);
    static void vWaitStop(void);
    static void vWriteRegionName(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
