/*============================================================================*/
/* Engine.hpp -- Rapids Storage Engine                                        */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class Engine
  {
  public:
  
    static void vAttachRegion(void);
    static void vCallBoot(void);
    static void vCallImmmediate(Task* pTask);
    static void vCallPrompt(void);
    static void vChangeDirectory(void);
    static void vCheckPort(void);
    static void vClosePool(void);
    static void vCreateAttachMap(void);
    static eBO  eDecodeCommandLine(void);
    static void vDeleteImmmediate(Task** ppTask);
    static void vDeleteSession(Session* pSession);
    static void vErrorHandler(Error* pError);
    static void vGetCopyright(s256& sCopyright);
    static void vGetWelcome(s256& sWelcome);
    static void vKillPrev(void);
    static void vMainEntry(void);
    static void vMonitorPortrait(s48K& sRapidsPortrait);
    static void vMonitorProfile(s48K& sPortrait);
    static void vOpenLog(void);
    static void vOpenPool(void);
    static bool bOpenPool(s32& sPoolName);
    static void vOpenTrace(void);
    static void vPrintExit(void);
    static void vPrintStatus(void);
    static void vPrintWelcome(void);
    static void vRunCommandLine(void);
    static bool bRunUtilities(int argc, Stz* argv[]);
    static void vSanityCheck(void);
    static void vSignalHandler(int iSignal);
    static void vSpawnJobStart(void);
    static void vStartSystem(void);
    static void vStopSystem(void);
    static void vUnlinkSystem(void);
    static void vWaitStop(void);
    static void vWriteRegionName(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
