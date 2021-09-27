/*============================================================================*/
/* Engine.hpp -- Rapids Storage Engine                                        */
/*============================================================================*/
#pragma once

namespace STORE { class Error; }

#include "eBO.h"

namespace STORE
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
    static void vCallPrompt(void);
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
    static void vDeleteSession(Session* pSession);
    static void vEnvFlags(void);
    static void vErrorHandler(Error* pError);
    static void vGetCopyright(s256& sCopyright);
    static void vGetWelcome(s256& sWelcome);
    static void vIgnoreMemoryLeak(void);
    static void vInternalTables(void);
    static void vKillPrev(void);
    static void vLowMemoryDiscard(void);
    static void vMungeCheck(void);
    static void vNoMemoryLogging(void);
    static void vOpenPool(void);
    static bool bOpenPool(s32& sPoolName);
    static void vOpenTrace(bool bPrompt);
    static void vPostRecvFns(void);
    static void vPrintBuild(void);
    static void vPrintConstants(void);
    static void vPrintExit(void);
    static void vPrintFormats(void);
    static void vPrintMachine(void);
    static void vPrintRLimits(void);
    static void vPrintStatus(void);
    static void vPrintTime(void);
    static void vPrintWelcome(void);
    static void vRunUtility(int argc, Stz* argv[]);
    static void vSanityCheck(void);
    static void vSignalHandler(int iSignal);
    static void vStartSystem(void);
    static void vStopSystem(void);
    static void vTestPort(i8 iPort);
    static void vUnlinkSystem(void);
    static void vWaitStop(void);
    static void vWriteRegionName(void);
    static void vCreateGridOfNodes(void); // newcode
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
