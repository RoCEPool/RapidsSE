/*============================================================================*/
/* Engine.hpp -- Rapids Storage Engine                                        */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class Engine
  {
  public:
  
    static void vMainEntry(void);
    static void vPreliminary(void);
    static void vRunCommandLine(void);
    static void vConcluding(void);

    static void vAttachRegion(void);
    static void vBootCheck(void);
    static void vCallBoot(void);
    static void vCallImmmediate(Task* pTask);
    static void vCallPrompt(void);
    static void vCallStart(void);
    static void vChangeDirectory(void);
    static void vCheckGrid(void);
    static void vCheckNodes(void);
    static void vCheckPort(void);
    static void vClosePool(void);
    static void vCloseTrace(void);
    static void vCreateAttachMap(void);
    static void vCreateFileNames(void);
    static void vCreateTmpDir(void);
    static eBO  eDecodeCommandLine(void);
    static void vDeleteImmmediate(Task** ppTask);
    static void vDeleteSession(Session* pSession);
    static void vEnvFlags(void);
    static void vErrorHandler(Error* pError);
    static void vGetCopyright(s256& sCopyright);
    static void vGetWelcome(s256& sWelcome);
    static void vKillPrev(void);
    static void vMonitorPortrait(s48K& sRapidsPortrait);
    static void vMonitorProfile(s48K& sPortrait);
    static void vMungeCheck(void);
    static void vOperandQueueReport(void);
    static void vOpenLog(void);
    static void vOpenPool(void);
    static bool bOpenPool(s32& sPoolName);
    static void vOpenTrace(void);
    static void vPrintBuild(void);
    static void vPrintConstants(void);
    static void vPrintExit(void);
    static void vPrintFormats(void);
    static void vPrintMachine(void);
    static void vPrintRLimits(void);
    static void vPrintStatus(void);
    static void vPrintTime(void);
    static void vPrintWelcome(void);
    static bool bRunUtilities(int argc, Stz* argv[]);
    static void vSanityCheck(void);
    static void vSignalHandler(int iSignal);
    static void vStartSystem(void);
    static void vStopSystem(void);
    static void vTestPort(i8 iPort);
    static void vUnlinkSystem(void);
    static void vWaitStop(void);
    static void vWriteRegionName(void);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
