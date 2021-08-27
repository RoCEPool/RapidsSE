/*============================================================================*/
/* Engine.cpp -- Rapids Storage Engine                                        */
/*============================================================================*/

#include "BASE.h"

#include <signal.h>

#include "CmdLineArgs.hpp"

#include "PtnMap.hpp"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node.hpp"
#include "Grid.hpp"
#include "Session.hpp"
#include "AttachMap.hpp"
#include "Constants.hpp"
#include "Engine.hpp"
#include "EnvCheck.hpp"
#include "Error.hpp"
#include "Job.hpp"
#include "JobList.hpp"
#include "Jobs.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "Sessions.hpp"

#include "ShortCut.hpp"
#include "SeDeputy.hpp"
#include "SePrompt.hpp"

#include "Step.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Index.hpp"
#include "BucketMap.hpp"
#include "CrwBuffer.hpp"
#include "StreamSet.hpp"
#include "Marker.hpp"

#include "eBO.h"
#include "TagConstants.h"

#include "InternalTables.hpp"
#include "Parcel.hpp"

#include "Api.hpp"

#include "STORE.h"

CmdLineArgs  gxCmdLineArgs;
EnvFlags     gxEnvFlags;
FnCatalog    gxFnCatalog;

AttachMap*   gpAttachMap;
BufferPool*  gpBufferPool;
Header*      gaSeShmRegion;
File*        gpTraceFile;

Stz* gsBuildGitHash;
Stz* gsBuildDate;
Stz* gsBuildHostName;
Stz* gsBuildUserName;
Stz* gsBuildBranch;

extern SizeHeap gxaSizeHeap[];

extern Atomic gixDir;
extern Atomic gixFile;
extern Atomic gixMutex;
extern Atomic gixPsxThd;
extern Atomic gixSocket;

char giProgressPoint    = 1;
i8   giProgressSubPoint = 0;

extern i8     gi_argc;
extern s256   gi_szaArgv[];


pthread_key_t giTLS_ThreadKey  = C_UNKNOWN;
pthread_key_t giTLS_TaskKey    = C_UNKNOWN;
pthread_key_t giTLS_HandlerKey = C_UNKNOWN;
pthread_key_t giTLS_StepKey    = C_UNKNOWN;

/*----------------------------------------------------------------------------*/
/* main entry                                                                 */
/*----------------------------------------------------------------------------*/
void Engine::vMainEntry(void)
{
  vPreliminary();

  vRunCommandLine();
  
  vConcluding();
}

/*----------------------------------------------------------------------------*/
/* preliminary activity                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vPreliminary(void)
{
  //OpSys::vGetInfo();
  //OpSys::vTraceInfo();
  
  vChangeDirectory(); //! what happens if this fails?
  
  vCreateTmpDir();
  
  vOpenTrace();

  vPrintWelcome();
  
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {
    vTrace("\n"_Stz);
    vTrace("argv[]:\n"_Stz);
    for (i8 i = 0; i < gi_argc; i++)
      vTrace("  argv[%lld] %s\n"_Stz,i,gi_szaArgv[i].szStz());
  }
  
  vPrintTime();

  vPrintBuild();
  
  vPrintMachine();
  
  vPrintRLimits();

  vPrintConstants();
  vPrintFormats();

  gxCmdLineArgs.vPrintStartupInfo();

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
     vInfo("\n*\n* %lld) Internal Initialisation\n*\n"_Stz,giProgressPoint++);

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
     vInfo("Setting signal handlers...\n"_Stz);
  
  // Segmentation Violation
  signal(SIGSEGV, Engine::vSignalHandler);
  
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
     vInfo("Init H/W counters...\n"_Stz);
  HwCounters::bInit();
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
     vInfo("Checking assumptions...\n"_Stz);
  
  BASE::vCheckAssumptions();
  STORE::vCheckAssumptions();

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
     vInfo("Boot check...\n"_Stz);
  AM::vBootCheck();
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
     vInfo("Sanity check...\n"_Stz);
  vSanityCheck();
  
  //vWriteRegionName();
  
  EnvCheck::vCheckManifest(); //! what happens if this fails
 
  vKillPrev();
  
  vOpenPool();
  
//  gpAttachMap->m_bMarkResource[RK_SOCKET] = true;
//  gpAttachMap->m_bMarkResource[RK_FILE] = true;
//  gpAttachMap->m_bMarkResource[RK_DIR] = true;
  
  BASE::vInitAllGlobals();

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
     vInfo("\n*\n* %lld) Synthesise Names\n*\n"_Stz,giProgressPoint++);
  vCreateFileNames();
  
  //vOpenTrace();
  
  vCheckNodes();
  
  //vCheckPort();

  //vCheckGrid();

//  if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {
//    vInfo("\n*\n* %lld) Attach Map Contents\n*\n"_Stz,giProgressPoint++);
//    gpAttachMap->vPrintAttachMap();
//  }
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {
     vInfo("\n**\n** Preliminaries Complete \n**\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* run command line                                                           */
/*----------------------------------------------------------------------------*/
void Engine::vRunCommandLine(void)
{
  
  eBO eBootOption = eDecodeCommandLine();

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {
    vInfo("\n+\n+ %lld) Run Command Line\n+\n"_Stz,giProgressPoint++);
    switch (eBootOption) {
        
      case BO_ADD_NODE:     vInfo("BO_ADD_NODE\n"_Stz);     break;
      case BO_PROMPT:       vInfo("BO_PROMPT\n"_Stz);       break;
      case BO_SETH_PROMPT:  vInfo("BO_SETH_PROMPT\n"_Stz);  break;
      case BO_START:        vInfo("BO_START\n"_Stz);        break;
      case BO_START_PROMPT: vInfo("BO_START_PROMPT\n"_Stz); break;
      case BO_STATUS:       vInfo("BO_STATUS\n"_Stz);       break;
      case BO_STOP:         vInfo("BO_STOP\n"_Stz);         break;
      case BO_UNLINK:       vInfo("BO_UNLINK\n"_Stz);       break;
        
      default:
        vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
               "Unknown command line argument\n"_Stz);
    }

  }
  
  try {
    switch (eBootOption) {
        
      case BO_ADD_NODE:
        vStartSystem();
        AM::SYSTEM::vSystemReady(); // Process has completed the boot sequence
        vWaitStop();
        break;
        
      case BO_PROMPT:
        // wait for the attachmap to be ready
        if (gpAttachMap->uAttachMapState() != C_READY)
          vTrace("Waiting for SE to start...\n"_Stz);
        while (gpAttachMap->uAttachMapState() != C_READY) {
          AM::vNapMsec(1,CN_NONE);
        }
        vCallPrompt();
        break;
        
      case BO_SETH_PROMPT:
        // wait for the attachmap to be ready
        if (gpAttachMap->uAttachMapState() != C_READY)
          vTrace("Waiting for SE to start...\n"_Stz);
        while (gpAttachMap->uAttachMapState() != C_READY) {
          AM::vNapMsec(1,CN_NONE);
        }
        vCallPrompt();
        break;
        
      case BO_START:
        vStartSystem();
        AM::SYSTEM::vSystemReady(); // Process has completed the boot sequence
        vWaitStop();
        break;
        
      case BO_START_PROMPT:
        vStartSystem();
        AM::SYSTEM::vSystemReady(); // Process has completed the boot sequence
#ifdef WITH_APPLE
        if (! gxCmdLineArgs.m_xSysArgs.m_bNoMunge )
          vMungeCheck();
#endif
        vCallPrompt();
        break;
        
      case BO_STATUS:
        vPrintStatus();
        break;
        
      case BO_STOP:
        vStopSystem();
        break;
        
      case BO_UNLINK:
        vStopSystem();
        break;
        
      default:
        vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
               "Unknown command line argument\n"_Stz);
    }
  }
  
  catch (Error* pError) {
    vErrorHandler(pError);
  }
}

/*----------------------------------------------------------------------------*/
/* concluding activities                                                      */
/*----------------------------------------------------------------------------*/
void Engine::vConcluding(void)
{
  vCloseTrace();
  
  vClosePool();
  
  vPrintExit();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* attach to an existing shm region                                           */
/*----------------------------------------------------------------------------*/
void Engine::vAttachRegion(void)
{
  AttachMap* pAttachMap = pAdd<AttachMap*>((Object*)gaSeShmRegion,C_SIZE_POOL_BUFFER);
  
  AM::vAttachMap(pAttachMap);
  
  // get pointers to global objects from attach map
  pAttachMap->vGetGlobals();
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    AttachMap::vDump(CN_ATTACHMAP, pAttachMap, Stz::szTab(1));
}

/*----------------------------------------------------------------------------*/
/* things to check right at the beginning                                     */
/*----------------------------------------------------------------------------*/
void Engine::vBootCheck(void)
{
  ASSERT( 1 == gxaSizeHeap[CN__A__].m_nClassSize);
  ASSERT( 2 == gxaSizeHeap[CN__B__].m_nClassSize);
  ASSERT( 3 == gxaSizeHeap[CN__C__].m_nClassSize);
  ASSERT( 4 == gxaSizeHeap[CN__D__].m_nClassSize);
  ASSERT( 5 == gxaSizeHeap[CN__E__].m_nClassSize);
  ASSERT( 6 == gxaSizeHeap[CN__F__].m_nClassSize);
  ASSERT( 7 == gxaSizeHeap[CN__G__].m_nClassSize);
  ASSERT( 8 == gxaSizeHeap[CN__H__].m_nClassSize);
  ASSERT( 9 == gxaSizeHeap[CN__I__].m_nClassSize);
  ASSERT(10 == gxaSizeHeap[CN__J__].m_nClassSize);
  ASSERT(11 == gxaSizeHeap[CN__K__].m_nClassSize);
  ASSERT(12 == gxaSizeHeap[CN__L__].m_nClassSize);
  ASSERT(13 == gxaSizeHeap[CN__M__].m_nClassSize);
  ASSERT(14 == gxaSizeHeap[CN__N__].m_nClassSize);
  ASSERT(15 == gxaSizeHeap[CN__O__].m_nClassSize);
  ASSERT(16 == gxaSizeHeap[CN__P__].m_nClassSize);
  ASSERT(17 == gxaSizeHeap[CN__Q__].m_nClassSize);
  ASSERT(18 == gxaSizeHeap[CN__R__].m_nClassSize);
  ASSERT(19 == gxaSizeHeap[CN__S__].m_nClassSize);
  ASSERT(20 == gxaSizeHeap[CN__T__].m_nClassSize);
  ASSERT(21 == gxaSizeHeap[CN__U__].m_nClassSize);
  ASSERT(22 == gxaSizeHeap[CN__V__].m_nClassSize);
  ASSERT(23 == gxaSizeHeap[CN__W__].m_nClassSize);
  ASSERT(24 == gxaSizeHeap[CN__X__].m_nClassSize);
  ASSERT(25 == gxaSizeHeap[CN__Y__].m_nClassSize);
  ASSERT(26 == gxaSizeHeap[CN__Z__].m_nClassSize);
}

/*----------------------------------------------------------------------------*/
/* call the prompt loop                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vCallPrompt(void)
{
//  SePrompt* pSePrompt = new SePrompt(gxCmdLineArgs.m_xSysArgs.m_bCommand);
//
//  pSePrompt->vPromptLoop();
//
//  delete pSePrompt;
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("\n>\n> %lld) Calling PROMPT\n>\n"_Stz,giProgressPoint++);

  //AM::vPromptCall();

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Adding Ignore memory leaks\n"_Stz);
  vIgnoreMemoryLeak();

  SePrompt* pSePrompt = new SePrompt(gxCmdLineArgs.m_xSysArgs.m_bCommand);
  
  pSePrompt->vPromptLoop();
  
  delete pSePrompt;

}

/*----------------------------------------------------------------------------*/
/* change current directory                                                   */
/*----------------------------------------------------------------------------*/
void Engine::vChangeDirectory(void)
{
  s256 sOriginal;
  s256 sCurrent;

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("\n*\n* %lld) Change Working Directory\n*\n"_Stz,giProgressPoint++);
  
  if (Dir::bGetCurrent(sOriginal) && AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("Original working directory: %s\n"_Stz,sOriginal.szStz());
  
  // change working directory to commands dir
  //
  // **************************************************
  // ** WARNING! WARNING! WARNING! WARNING! WARNING! **
  // **************************************************
  // The TinyC implementation currently relies on the current working
  // directory being the commands directory as it uses relative paths
  // from the cwd to set the header and library search paths when 
  // compiling dynamic code. If you change this value you may need to
  // change these paths in LogicCompiler::bCompile().
  bool bResult = Dir::bChangeCurrent(gxCmdLineArgs.m_xSysArgs.m_sCommandsDir);
  
  if (! bResult)
    Process::vUnrecoverableError("Unable to change to command dir"_Stz,
                                 gxCmdLineArgs.m_xSysArgs.m_sCommandsDir);

  if (Dir::bGetCurrent(sCurrent) && AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("Current working directory: %s\n"_Stz,sCurrent.szStz());
}

/*----------------------------------------------------------------------------*/
/* check grid configuration                                                   */
/*----------------------------------------------------------------------------*/
void Engine::vCheckGrid(void)
{
  // nothing to check if not running a grid test
  if ( ! gxCmdLineArgs.m_xSysArgs.m_bStart)
    return;
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("\n*\n* %lld) Checking GridTest Configuration\n*\n"_Stz,giProgressPoint++);
  
   for (i8 i_Node = 0;
        i_Node < gxCmdLineArgs.m_xSysArgs.m_nDefinedNodes;
        i_Node++) {
     
     if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
       vInfo("Checking %s %s %lld\n"_Stz,
             gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sNodeName.szStz(),
             gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sIPAddress.szStz(),
             gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_iPortNo
             );
     
     if (! gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sIPAddress.bSame(C_LOOPBACK_IP))
       Process::vUnrecoverableError("Invalid loopback IP for grid test"_Stz,
                                    gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sIPAddress);
     
     if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
       vInfo("Loopback %s for %s is valid\n"_Stz,
             gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sIPAddress.szStz(),
             gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sNodeName.szStz());

     if (gxCmdLineArgs.m_xSysArgs.m_bGridTest)
       vTestPort(gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_iPortNo);
   }

}

/*----------------------------------------------------------------------------*/
/* check and see if the port has other listeneters                            */
/*----------------------------------------------------------------------------*/
void Engine::vCheckNodes(void)
{
  if (! gxCmdLineArgs.m_xSysArgs.m_bStart)
    return;
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("\n*\n* %lld) Checking Node Configurations\n*\n"_Stz,giProgressPoint++);

  if (gxCmdLineArgs.m_xSysArgs.m_nDefinedNodes < 1)
    Process::vUnrecoverableError("No nodelist was supplied (via --node)"_Stz,
                                 gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz());

  if (C_UNKNOWN == gxCmdLineArgs.m_xSysArgs.m_i_ThisNode)
    Process::vUnrecoverableError("This node is not in node list"_Stz,
                                 gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz());

  if (AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("This is node %lld of %lld nodes\n"_Stz,
          gxCmdLineArgs.m_xSysArgs.m_i_ThisNode,
          gxCmdLineArgs.m_xSysArgs.m_nDefinedNodes);
}

/*----------------------------------------------------------------------------*/
/* check and see if the port has other listeneters                            */
/*----------------------------------------------------------------------------*/
void Engine::vCheckPort(void)
{
  SysCall oSysCall;

  i8      iPort;
  
  // nothing to check if not starting SE
  if ( ! gxCmdLineArgs.m_xSysArgs.m_bStart)
    return;
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("\n*\n* %lld) Testing Listening Port\n*\n"_Stz,giProgressPoint++);
  
  iPort = gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[gxCmdLineArgs.m_xSysArgs.m_i_ThisNode].m_iPortNo;
  
  vTestPort(iPort);
  
//  Socket* pSocket = new Socket(gpAttachMap);
//  bool bResult = pSocket->bCreateServer((int)iPort, oSysCall);
//
//  if ( ! bResult ) {
//    if (oSysCall.iError() == EADDRINUSE) {
//      vTrace("Address in use (port: %lld). "
//             "CHECK IF ANOTHER INSTANCE OF RAPIDSSE IS RUNNING!\n"_Stz,
//             iPort);
//
//    }
//    else {
//      vTrace("Unable to listen for connections (port: %lld): Errno %d!\n"_Stz,
//             iPort, oSysCall.iError());
//    }
//    //Engine::vStopSystem();
//    Process::vExitFailure();
//  }
//
//  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
//    vInfo("Able to open port %lld\n"_Stz, iPort);
//
//  pSocket->bClose(oSysCall);
//
//  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
//    vInfo("Closed port %lld\n"_Stz, iPort);
//
//  delete pSocket;
}

/*----------------------------------------------------------------------------*/
/* close the shm region                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vClosePool(void)
{
  //delete gpBufferPool;
}

/*----------------------------------------------------------------------------*/
/* close trace file                                                           */
/*----------------------------------------------------------------------------*/
void Engine::vCloseTrace(void)
{
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("** trace file closed: %s\n"_Stz, gpAttachMap->m_sTraceFile.szStz());
  gpTraceFile->vClose();
  //delete gpTraceFile;
  
  gpTraceFile = nullptr;
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void Engine::vCreateAttachMap(void)
{
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("\n*\n* %lld) Create Attach Map\n*\n"_Stz,giProgressPoint++);
  
  // create the attach map (which records global pointers)
  new AttachMap;


}

/*----------------------------------------------------------------------------*/
/* create log and trace file name                                             */
/*----------------------------------------------------------------------------*/
void Engine::vCreateFileNames(void)
{
  
  // "SEgrid" to be gxCmdLineArgs.m_xSysArgs.m_sGridName.szStz()
#ifdef WITH_DEBUG_VERSION
  gpAttachMap->m_sTraceFile.vSprint("../tmp/SE_Trace_%s_%s_Debug.txt"_Stz,
                                    "SEgrid"_Stz, gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz());
  gpAttachMap->m_sLogRegion.vSprint("SE_Log_%s_Debug"_Stz,"SEgrid"_Stz);
  gpAttachMap->m_sLogCsvFile.vSprint("../tmp/SE_Log_%s_Debug.csv"_Stz,"SEgrid"_Stz);
  gpAttachMap->m_sLogTxtFile.vSprint("../tmp/SE_Log_%s_Debug.txt"_Stz,"SEgrid"_Stz);
#else
  gpAttachMap->m_sTraceFile.vSprint("../tmp/SE_Trace_%s_%s.txt"_Stz,
                                    "SEgrid"_Stz, gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz());
  gpAttachMap->m_sLogRegion.vSprint("SE_Log_%s"_Stz, "SEgrid"_Stz);
  gpAttachMap->m_sLogCsvFile.vSprint("../tmp/SE_Log_%s.csv"_Stz, "SEgrid"_Stz);
  gpAttachMap->m_sLogTxtFile.vSprint("../tmp/SE_Log_%s.txt"_Stz, "SEgrid"_Stz);
#endif
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {
    vInfo("Trace file path: %s\n"_Stz,gpAttachMap->m_sTraceFile.szStz());
    vInfo("Log file path (csv): %s\n"_Stz,gpAttachMap->m_sLogCsvFile.szStz());
    vInfo("Log file path (text): %s\n"_Stz,gpAttachMap->m_sLogTxtFile.szStz());

    vInfo("Log region name: %s\n"_Stz,gpAttachMap->m_sLogRegion.szStz());
  }
}

/*----------------------------------------------------------------------------*/
/* create temp dir                                                            */
/*----------------------------------------------------------------------------*/
void Engine::vCreateTmpDir(void)
{
  _static_
  
  if (! Dir::bExists("../tmp"_Stz)) {
    
    bool bMade = Dir::bMake("../tmp"_Stz);
    
    if (! bMade && !Dir::bExists("../tmp"_Stz)) {
      fprintf(stderr,"Unable to make ../tmp directory\n");
      Process::vExitFailure();
    }
  }
}

/*----------------------------------------------------------------------------*/
/* decode command line from arguments                                         */
/*----------------------------------------------------------------------------*/
eBO Engine::eDecodeCommandLine(void)
{
  if (gxCmdLineArgs.m_xSysArgs.m_bStart && gxCmdLineArgs.m_xSysArgs.m_bPrompt)   return BO_START_PROMPT;
  if (gxCmdLineArgs.m_xSysArgs.m_bStart)                       return BO_START;
  if (gxCmdLineArgs.m_xSysArgs.m_bPrompt)                      return BO_PROMPT;
  if (gxCmdLineArgs.m_xSysArgs.m_bStatus)                      return BO_STATUS;
  if (gxCmdLineArgs.m_xSysArgs.m_bStop)                        return BO_STOP;
  if (gxCmdLineArgs.m_xSysArgs.m_bUnlink)                      return BO_UNLINK;
  if (gxCmdLineArgs.m_xSysArgs.m_bSethScript)                  return BO_SETH_PROMPT;
  
  if (gxCmdLineArgs.m_xSysArgs.m_bAddNode)                     return BO_ADD_NODE;

  return BO_NONE;
}

/*----------------------------------------------------------------------------*/
/* delete an se session                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vDeleteSession(Session* pSession)
{
  //delete pSession;
  AM::pSessions()->vFreeSession(pSession);
}

/*----------------------------------------------------------------------------*/
/* translate debug flag env variables                                         */
/*----------------------------------------------------------------------------*/
void Engine::vEnvFlags(void)
{
  _static_
  
  s256 sFlagValue;
  
  if (Env::bGet("SE_ENV_ALKA"_Stz,sFlagValue))
    gxEnvFlags.m_iAlka = StrLib::iToI8(sFlagValue.azChar());
  if (Env::bGet("SE_ENV_RAY"_Stz,sFlagValue))
    gxEnvFlags.m_iRay = StrLib::iToI8(sFlagValue.azChar());
}

/*----------------------------------------------------------------------------*/
/* error handler                                                              */
/*----------------------------------------------------------------------------*/
void Engine::vErrorHandler(Error* pError)
{
  char caFileName[256];
  
  char* acEnd;
  
  fprintf(stderr, "%s: line %lld\n",
          pError->aErrorMsg()->azChar(),
          pError->iLineNo());
  
  fprintf(stderr, "%s: line %lld in %s\n",
         pError->aErrorMsg()->azChar(),
         pError->iLineNo(),
         (char*)pError->aFileName());
  
  if (pError) {
    acEnd = StrLib::acLast(pError->aFileName()->azChar());
    StrLib::bBackward(&acEnd,(char*)pError->aFileName(),'/');
    StrLib::vCopy(caFileName,acEnd+1);
    vTrace("%s: line %lld in %s\n"_Stz,
           pError->aErrorMsg(),
           pError->iLineNo(),
           caFileName);
  }
  else {
    vTrace("Engine::vErrorHandler: Unknown error\n"_Stz);
  }
           
  BASE::vThreadCallStackDump();
}

/*----------------------------------------------------------------------------*/
/* get copyright                                                              */
/*----------------------------------------------------------------------------*/
void Engine::vGetCopyright(s256& sCopyright)
{
  sCopyright = "(c) 2020 Rapids Data Technology Co. Ltd"_Stz;
}

/*----------------------------------------------------------------------------*/
/* get welcome                                                                */
/*----------------------------------------------------------------------------*/
void Engine::vGetWelcome(s256& sWelcome)
{
  Stz* szRelDbg;
#ifdef WITH_DEBUG_VERSION
  szRelDbg = "debug"_Stz;
#else
  szRelDbg = "release"_Stz;
#endif
  
  sWelcome.vSprint("RapidsSE %s (%s version, pid %lld, git %s)"_Stz,
                   
                   AM::szVersionString(),
                   szRelDbg,
                   Process::iGetPid(),
                   gsBuildGitHash
                  );
}

/*----------------------------------------------------------------------------*/
/* IgnoreMemoryLeak                                                           */
/*----------------------------------------------------------------------------*/
void Engine::vIgnoreMemoryLeak(void)
{
  gxaSizeHeap[CN_NAMEDBUFFER].m_bIgnoreMemoryLeak = true;
}

/*----------------------------------------------------------------------------*/
/* find and set stream def for internal tables                                */
/*----------------------------------------------------------------------------*/
void Engine::vInternalTables(void)
{
  StreamDef* pStreamDef;
  
  pStreamDef = AM::pStreamDefFind(C_TDEF_EVENTLOG);
  gpAttachMap->m_pInternalTables->vStreamDef(TI_EVENTLOG,pStreamDef);
  
  pStreamDef = AM::pStreamDefFind(C_TDEF_MEMORYLOG);
  gpAttachMap->m_pInternalTables->vStreamDef(TI_MEMORYLOG,pStreamDef);

  pStreamDef = AM::pStreamDefFind(C_TDEF_OPERANDLOG);
  gpAttachMap->m_pInternalTables->vStreamDef(TI_OPERANDLOG,pStreamDef);

  pStreamDef = AM::pStreamDefFind(C_TDEF_PROMPTOUT);
  gpAttachMap->m_pInternalTables->vStreamDef(TI_PROMPTOUT,pStreamDef);

  pStreamDef = AM::pStreamDefFind(C_TDEF_RESOURCELOG);
  gpAttachMap->m_pInternalTables->vStreamDef(TI_RESOURCELOG,pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* kill previous node instances                                               */
/*----------------------------------------------------------------------------*/
void Engine::vKillPrev(void)
{
  if (! gxCmdLineArgs.m_xSysArgs.m_bKillPrev)
    return;
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("\n*\n* %lld) Kill Previous Occurences\n*\n"_Stz,giProgressPoint++);
  
  // an assumption is that regions will be /SE_<node>
  
  void* aRegion;
  s32 sRegionNm;
  int iFD;
  
  for (i8 i_Node = 0;
       i_Node < gxCmdLineArgs.m_xSysArgs.m_nDefinedNodes;
       i_Node++) {
    
    sRegionNm.vSprint("%s_%s"_Stz,
                        gxCmdLineArgs.m_xSysArgs.m_sRegionPrefix.azChar(),
                        gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sNodeName.azChar());
    
    if (! Region::bTest(sRegionNm) )
      continue;

    if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM))
      vTrace("Engine::vKillPrev: region %s exists\n"_Stz,sRegionNm.szStz());
    
    if (! (aRegion = Region::aOpen(sRegionNm,gxCmdLineArgs.m_xSysArgs.m_aRegionAddr,iFD)))
      continue;
    
    AttachMap* pAttachMap = (AttachMap*)aAdd<AttachMap*>((Object*)aRegion,C_SIZE_POOL_BUFFER);
    
    i8 iPid = pAttachMap->iPid();
    if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM))
       vTrace("Engine::vKillPrev: prev pid %lld\n"_Stz,iPid);

    if (iPid &&
        iPid != Process::iGetPid() &&
        Process::bTestPid(iPid)) {
      
      bool bResult = Process::bKillPid(iPid);
      if (gxCmdLineArgs.m_xSysArgs.m_oArgTags.bHasName(C_SEE_SHM)) {
        if (bResult)
          vTrace("Engine::vKillPrev: process %lld killed\n"_Stz,iPid);
        else
          vTrace("Engine::vKillPrev: process %lld not killed\n"_Stz,iPid);
      }
    }
    
    Region::vClose(aRegion,iFD);
    
    Region::vUnlink(sRegionNm);
  }
}

/*----------------------------------------------------------------------------*/
/* set low-memory receive discards                                            */
/*----------------------------------------------------------------------------*/
void Engine::vLowMemoryDiscard(void)
{
  gxaSizeHeap[CN_CRWBUFFER].m_bLowMemoryDiscard = true;
  gxaSizeHeap[CN_CSVBUFFER].m_bLowMemoryDiscard = true;
  gxaSizeHeap[CN_EACHROWBUFFER].m_bLowMemoryDiscard = true;
  gxaSizeHeap[CN_PROFILE].m_bLowMemoryDiscard   = true;
  gxaSizeHeap[CN_ROWBUFFER].m_bLowMemoryDiscard = true;
}

/*----------------------------------------------------------------------------*/
/* munge check                                                                */
/*----------------------------------------------------------------------------*/
void Engine::vMungeCheck(void)
{
  // get the time of the Sen files in the Sen folder
  Dir* pDir = new Dir(nullptr);
  bool bResult = pDir->bOpen("../sen"_Stz);
  ASSERT(bResult);
  
  s256 sEntryNm;
  s256 sFilePath;
  s256 sLastModified;
  
  time_t tSenTime = 0;
  time_t tMungedTime;
  time_t tModTime;
  
  while (pDir->bRead()) {
    
    // get the entry
    sEntryNm = pDir->szEntryNm();
    
    // skip . and ..
    if (sEntryNm.bSame("."_Stz) || sEntryNm.bSame(".."_Stz))
      continue;
    
    // create the complete filepath
    sFilePath = "../sen/"_Stz;
    sFilePath.vConcat(sEntryNm);
    
    // get the last modification time of the file
    bResult = File::bModTime(sFilePath, tModTime);
    ASSERT(bResult);
    
    // get the last modified sen file time
    if (tModTime > tSenTime) {
      sLastModified = sEntryNm;
      tSenTime = tModTime;
    }
  }
  pDir->vClose();
  
  delete pDir;
  
  // get the time of the Munged file
  bResult = File::bModTime("../munge/Task_AddMunged.cpp"_Stz, tMungedTime);
  ASSERT(bResult);
  
  // is the last modified sen file time after the last munge ?
  if (tSenTime > tMungedTime) {
    
    vTrace("\n******************************************************************\n"_Stz);
    vTrace("\n MUNGING!!! (Last modified Sen file : %s)        \n\n"_Stz, sLastModified.azChar());
    vTrace("******************************************************************\n"_Stz);
    
    gxCmdLineArgs.m_xSysArgs.m_sCommand = "docol munge"_Stz;
    gxCmdLineArgs.m_xSysArgs.m_bCommand = true;
  }
}

/*----------------------------------------------------------------------------*/
/* set object classes which are not to be logged                              */
/*----------------------------------------------------------------------------*/
void Engine::vNoMemoryLogging(void)
{
}

/*----------------------------------------------------------------------------*/
/* operand queue report                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vOperandQueueReport(bool bStdOut)
{
  Now oNow;

  s256 sPath;
  
  sPath.vSprint("../tmp/OpdqReport_%s.txt"_Stz,oNow.szPath());
  
  File* pFile = new File(nullptr);

  pFile->bOpenMode(sPath,C_OPEN_WRITE);
  
  JobList* pJobPttList = AM::pParticipatingJobs();
  pJobPttList->vGrabLock();
  pJobPttList->vOperandQueueReport(pFile,bStdOut);
  pJobPttList->vDropLock();

  pFile->vClose();
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* attach or create the shm region                                            */
/*----------------------------------------------------------------------------*/
void Engine::vOpenPool(void)
{
  bool bResult;
  
  if (gxCmdLineArgs.m_xSysArgs.m_bUnlink) {
    //vUnlink();
    return;
  }
  
  if (gxCmdLineArgs.m_xSysArgs.m_bStart) {
    
    if (! gxCmdLineArgs.m_xSysArgs.m_bRestart && Region::bTest(gxCmdLineArgs.m_xSysArgs.m_sRegionName)) {
      vTrace("Error: Instance %s already started\n"_Stz,gxCmdLineArgs.m_xSysArgs.m_sRegionName.azChar());
      Process::vExitFailure();
      return;
    }
    
    if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
      vInfo("\n*\n* %lld) Create Shared Memory Region\n*\n"_Stz,giProgressPoint++);
    
    Region::vCreate(gxCmdLineArgs.m_xSysArgs.m_sRegionName,
                    gxCmdLineArgs.m_xSysArgs.m_bVerbose,
                    gxCmdLineArgs.m_xSysArgs.m_aRegionAddr,
                    gxCmdLineArgs.m_xSysArgs.m_iRegionSize,
                    gxCmdLineArgs.m_xSysArgs.m_bTouch,
                    gxCmdLineArgs.m_xSysArgs.m_bProgress,
                    gxCmdLineArgs.m_xSysArgs.m_bLock,
                    gxCmdLineArgs.m_xSysArgs.m_bRestart,
                    gxCmdLineArgs.m_xSysArgs.m_bDontFree,
                    gxCmdLineArgs.m_xSysArgs.m_bFreeSpecific);
    
    if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {
      
      s32 sRegionSize_magn;
      MagLib::vByteMag(gxCmdLineArgs.m_xSysArgs.m_iRegionSize,3,sRegionSize_magn);
      
       vInfo("Created new buffer pool region %p: %s size %s\n"_Stz,
            gxCmdLineArgs.m_xSysArgs.m_aRegionAddr,
            gxCmdLineArgs.m_xSysArgs.m_sRegionName.szStz(),
            sRegionSize_magn.szStz());
    }
    
    vCreateAttachMap();
    
    return;
  }
  
  if (gxCmdLineArgs.m_xSysArgs.m_bStatus) {
    return;
  }
  
  if (gxCmdLineArgs.m_xSysArgs.m_bStop) {
    if (! Region::bTest(gxCmdLineArgs.m_xSysArgs.m_sRegionName)) {
      vTrace("Error: Instance %s was not started\n"_Stz,
             gxCmdLineArgs.m_xSysArgs.m_sRegionName.azChar());
      Process::vExitFailure();
      return;
    }
    if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
      vInfo("\n*\n* Attach Region\n*\n"_Stz);
    bResult = Region::bAttach(gxCmdLineArgs.m_xSysArgs.m_sRegionName,
                              gxCmdLineArgs.m_xSysArgs.m_bVerbose,
                              gxCmdLineArgs.m_xSysArgs.m_aRegionAddr);
    ASSERT(bResult);
    vAttachRegion();
    return;
  }
  
  if (gxCmdLineArgs.m_xSysArgs.m_bPrompt || gxCmdLineArgs.m_xSysArgs.m_bSethScript) {
    if (! Region::bTest(gxCmdLineArgs.m_xSysArgs.m_sRegionName)) {
      vTrace("Error: Instance %s was not started\n"_Stz,gxCmdLineArgs.m_xSysArgs.m_sRegionName.azChar());
      Process::vExitFailure();
      return;
    }
    if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
      vInfo("\n*\n* Attach Region\n*\n"_Stz);
    bResult = Region::bAttach(gxCmdLineArgs.m_xSysArgs.m_sRegionName,
                              gxCmdLineArgs.m_xSysArgs.m_bVerbose,
                              gxCmdLineArgs.m_xSysArgs.m_aRegionAddr);
    if ( ! bResult ) {
      vTrace("Error: Unable to attach to instance %s\n"_Stz,gxCmdLineArgs.m_xSysArgs.m_sRegionName.azChar());
      Process::vExitFailure();
      return;
      
    }
    vAttachRegion();
    return;
  }
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("\n*\n* Attach Region\n*\n"_Stz);
  bResult = Region::bAttach(gxCmdLineArgs.m_xSysArgs.m_sRegionName,
                            gxCmdLineArgs.m_xSysArgs.m_bVerbose,
                            gxCmdLineArgs.m_xSysArgs.m_aRegionAddr);
  ASSERT(bResult);
  vAttachRegion();
}

/*----------------------------------------------------------------------------*/
/* attach or create the shm region                                            */
/*----------------------------------------------------------------------------*/
bool Engine::bOpenPool(s32& sPoolName)
{
  bool bResult = Region::bAttach(sPoolName,
                                 gxCmdLineArgs.m_xSysArgs.m_bVerbose,
                                 gxCmdLineArgs.m_xSysArgs.m_aRegionAddr);
  if (!bResult)
    return bResult;
  
  vAttachRegion();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* open trace file                                                            */
/*----------------------------------------------------------------------------*/
void Engine::vOpenTrace(void)
{
  s256 sTracePath;
  
  #ifdef WITH_DEBUG_VERSION
    sTracePath.vSprint("../tmp/SE_Trace_%s_%s_Debug.txt"_Stz,
                       "SEgrid"_Stz,
                       gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz());
  #else
    sTracePath.vSprint("../tmp/SE_Trace_%s_%s.txt"_Stz,
                       "SEgrid"_Stz,
                       gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz());
  #endif
  //if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
  //  vInfo("\n*\n* Opening Trace File\n*\n"_Stz);
  
  gpTraceFile = new(C_VIA_MALLOC) File(nullptr);
  
  bool bOpened;

  if (gxCmdLineArgs.m_xSysArgs.m_bAppendTrace)
    bOpened = gpTraceFile->bOpenAppend(sTracePath);
  else
    bOpened = gpTraceFile->bOpenWrite(sTracePath);
  RASSERT(bOpened);
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("Opened trace file for %s: %s\n"_Stz,
          (gxCmdLineArgs.m_xSysArgs.m_bAppendTrace) ? "append" : "write",
          sTracePath.szStz());
}

/*----------------------------------------------------------------------------*/
/* set post receive functions                                                 */
/*----------------------------------------------------------------------------*/
void Engine::vPostRecvFns(void)
{
  gxaSizeHeap[CN_BUCKETMAP].m_fpPostRecvFn = (fpPOSTFN*)BucketMap::vPostRecv;
  gxaSizeHeap[CN_CRWBUFFER].m_fpPostRecvFn = (fpPOSTFN*)CrwBuffer::vPostRecv;
  gxaSizeHeap[CN_CSVBUFFER].m_fpPostRecvFn = (fpPOSTFN*)CsvBuffer::vPostRecv;
  //gxaSizeHeap[CN_EACHROWBUFFER].m_fpPostRecvFn = (fpPOSTFN*)EachRowBuffer::vPostRecv;
  gxaSizeHeap[CN_INDEX].m_fpPostRecvFn     = (fpPOSTFN*)Index::vPostRecv;
  gxaSizeHeap[CN_INDEXDEF].m_fpPostRecvFn  = (fpPOSTFN*)IndexDef::vPostRecv;
  gxaSizeHeap[CN_INFOSET].m_fpPostRecvFn   = (fpPOSTFN*)InfoSet::vPostRecv;
  gxaSizeHeap[CN_JOB].m_fpPostRecvFn       = (fpPOSTFN*)Job::vPostRecv;
  gxaSizeHeap[CN_MARKER].m_fpPostRecvFn    = (fpPOSTFN*)Marker::vPostRecv;
  gxaSizeHeap[CN_PARCEL].m_fpPostRecvFn    = (fpPOSTFN*)Parcel::vPostRecv;
  gxaSizeHeap[CN_PROFILE].m_fpPostRecvFn   = (fpPOSTFN*)Profile::vPostRecv;
  gxaSizeHeap[CN_ROWBUFFER].m_fpPostRecvFn = (fpPOSTFN*)RowBuffer::vPostRecv;
  gxaSizeHeap[CN_STREAMDEF].m_fpPostRecvFn = (fpPOSTFN*)StreamDef::vPostRecv;
  gxaSizeHeap[CN_STREAMSET].m_fpPostRecvFn = (fpPOSTFN*)StreamSet::vPostRecv;
  gxaSizeHeap[CN_TABLE].m_fpPostRecvFn     = (fpPOSTFN*)Table::vPostRecv;
}

/*----------------------------------------------------------------------------*/
/* print build                                                                */
/*----------------------------------------------------------------------------*/
void Engine::vPrintBuild(void)
{
  if (! AM::AM::TRACE::bVerbose(C_VB_BOOT))
    return;
    
  vInfo("\n"_Stz);
  vInfo("Build:\n"_Stz);
  vInfo("  Git Hash  : %s\n"_Stz,gsBuildGitHash);
  vInfo("  Date      : %s\n"_Stz,gsBuildDate);
  vInfo("  Host Name : %s\n"_Stz,gsBuildHostName);
  vInfo("  User Name : %s\n"_Stz,gsBuildUserName);
  vInfo("  Branch    : %s\n"_Stz,gsBuildBranch);
}

/*----------------------------------------------------------------------------*/
/* print important constants                                                  */
/*----------------------------------------------------------------------------*/
void Engine::vPrintConstants(void)
{
  if (! AM::AM::TRACE::bVerbose(C_VB_BOOT))
    return;
    
  vInfo("\n"_Stz);
  vInfo("Constants:\n"_Stz);
  
  vTrace("  C_DEF_LVL_LEVELS     = %lld\n"_Stz,C_DEF_LVL_LEVELS);
  vTrace("  C_DEF_LVL_RATIO      = %lld\n"_Stz,C_DEF_LVL_RATIO);
  vTrace("  C_DIARY_ENTRIES      = %lld\n"_Stz,C_DIARY_ENTRIES);

  vTrace("  C_ENT_END_TICKET     = %llx\n"_Stz,(u8)C_ENT_END_TICKET);
  vTrace("  C_ENT_QUANTA         = %lld\n"_Stz,C_ENT_QUANTA);
  
  vTrace("  C_LOW_BUFFERS        = %lld\n"_Stz,C_LOW_BUFFERS);
  
  vTrace("  C_MAX_BCD_PRECISION  = %lld\n"_Stz,C_MAX_BCD_PRECISION);
  vTrace("  C_MAX_BUCKETS        = %lld\n"_Stz,C_MAX_BUCKETS);
  vTrace("  C_MAX_COLUMNS        = %lld\n"_Stz,C_MAX_COLUMNS);
  vTrace("  C_MAX_DEFCOLUMNS     = %lld\n"_Stz,C_MAX_DEFCOLUMNS);
  vTrace("  C_MAX_ENTRY_SIZE     = %lld\n"_Stz,C_MAX_ENTRY_SIZE);
  vTrace("  C_MAX_EXPR           = %lld\n"_Stz,C_MAX_EXPR);
  vTrace("  C_MAX_FLT_PRECISION  = %lld\n"_Stz,C_MAX_FLT_PRECISION);
  vTrace("  C_MAX_FROMVEC        = %lld\n"_Stz,C_MAX_FROMVEC);
  vTrace("  C_MAX_IDXCOLVEC      = %lld\n"_Stz,C_MAX_IDXCOLVEC);
  vTrace("  C_MAX_INDEXES        = %lld\n"_Stz,C_MAX_INDEXES);
  vTrace("  C_MAX_INDEX_COLUMNS  = %lld\n"_Stz,C_MAX_INDEX_COLUMNS);
  vTrace("  C_MAX_INT_PRECISION  = %lld\n"_Stz,C_MAX_INT_PRECISION);
  vTrace("  C_MAX_KEY            = %lld\n"_Stz,C_MAX_KEY);
  vTrace("  C_MAX_LOG_ENTRIES    = %lld\n"_Stz,C_MAX_LOG_ENTRIES);
  vTrace("  C_MAX_LOG_NOTE       = %lld\n"_Stz,C_MAX_LOG_NOTE);
  vTrace("  C_MAX_LVL_LEVELS     = %lld\n"_Stz,C_MAX_LVL_LEVELS);
  vTrace("  C_MAX_LVL_RATIO      = %lld\n"_Stz,C_MAX_LVL_RATIO);
  vTrace("  C_MAX_NESTED_WORDS   = %lld\n"_Stz,C_MAX_NESTED_WORDS);
  vTrace("  C_MAX_NODES          = %lld\n"_Stz,C_MAX_NODES);
  vTrace("  C_MAX_OPS            = %lld\n"_Stz,C_MAX_OPS);
  vTrace("  C_MAX_PTNSINGRID     = %lld\n"_Stz,C_MAX_PTNSINGRID);
  vTrace("  C_MAX_PATH           = %lld\n"_Stz,C_MAX_PATH);
  vTrace("  C_MAX_PAY            = %lld\n"_Stz,C_MAX_PAY);
  vTrace("  C_MAX_PREDICATEVEC   = %lld\n"_Stz,C_MAX_PREDICATEVEC);
  vTrace("  C_MAX_PTNSPERNODE    = %lld\n"_Stz,C_MAX_PTNSPERNODE);
  vTrace("  C_MAX_RECORD         = %lld\n"_Stz,C_MAX_RECORD);
  vTrace("  C_MAX_SEEKOFFSETS    = %lld\n"_Stz,C_MAX_SEEKOFFSETS);
  vTrace("  C_MAX_SESSIONS       = %lld\n"_Stz,C_MAX_SESSIONS);
  vTrace("  C_MAX_SHM_PERCENT    = %lld\n"_Stz,C_MAX_SHM_PERCENT);
  vTrace("  C_MAX_SHORTCUTS      = %lld\n"_Stz,C_MAX_SHORTCUTS);
  vTrace("  C_MAX_STAGES         = %lld\n"_Stz,C_MAX_STAGES);
  vTrace("  C_MAX_STATEMENT      = %lld\n"_Stz,C_MAX_STATEMENT);
  vTrace("  C_MAX_STREAMS        = %lld\n"_Stz,C_MAX_STREAMS);
  vTrace("  C_MAX_STREAMDEFS     = %lld\n"_Stz,C_MAX_STREAMDEFS);
  vTrace("  C_MAX_THREADS        = %lld\n"_Stz,C_MAX_THREADS);
  vTrace("  C_MAX_TOKENS         = %lld\n"_Stz,C_MAX_TOKENS);
  vTrace("  C_MAX_VCH            = %lld\n"_Stz,C_MAX_VCH);
  vTrace("  C_MAX_VCH_PRECISION  = %lld\n"_Stz,C_MAX_VCH_PRECISION);
  
  vTrace("  C_MIN_LVL_LEVELS     = %lld\n"_Stz,C_MIN_LVL_LEVELS);
  vTrace("  C_MIN_LVL_RATIO      = %lld\n"_Stz,C_MIN_LVL_RATIO);
  vTrace("  C_MIN_SHM_REGION     = %lld\n"_Stz,C_MIN_SHM_REGION);
  
  vTrace("  C_WSP_DATACHUNK      = %lld\n"_Stz,C_WSP_DATACHUNK);
  vTrace("  C_WSP_STACK          = %lld\n"_Stz,C_WSP_STACK);
  
  vTrace("  C_SIZE_TINY_OBJECT   = %lld\n"_Stz,C_SIZE_TINY_OBJECT);
  vTrace("  C_SIZE_SMALL_OBJECT  = %lld\n"_Stz,C_SIZE_SMALL_OBJECT);
  vTrace("  C_SIZE_MEDIUM_OBJECT = %lld\n"_Stz,C_SIZE_MEDIUM_OBJECT);
  vTrace("  C_SIZE_LARGE_OBJECT  = %lld\n"_Stz,C_SIZE_LARGE_OBJECT);
  vTrace("  C_SIZE_POOL_BUFFER   = %lld\n"_Stz,C_SIZE_POOL_BUFFER);
  vTrace("  C_SIZE_TINY_TEXT     = %lld\n"_Stz,C_SIZE_TINY_TEXT);
  vTrace("  C_SIZE_SMALL_TEXT    = %lld\n"_Stz,C_SIZE_SMALL_TEXT);
  vTrace("  C_SIZE_MEDIUM_TEXT   = %lld\n"_Stz,C_SIZE_MEDIUM_TEXT);
  vTrace("  C_SIZE_LARGE_TEXT    = %lld\n"_Stz,C_SIZE_LARGE_TEXT);
}

/*----------------------------------------------------------------------------*/
/* print exit                                                                 */
/*----------------------------------------------------------------------------*/
void Engine::vPrintExit(void)
{
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("Engine exits\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print important constants                                                  */
/*----------------------------------------------------------------------------*/
void Engine::vPrintFormats(void)
{
  if (! AM::AM::TRACE::bVerbose(C_VB_BOOT))
    return;
    
  vInfo("\n"_Stz);
  vInfo("Formats:\n"_Stz);
  
  vTrace("  F_BCD      = %s\n"_Stz,F_BCD);
  vTrace("  F_BCD_frac = %s\n"_Stz,F_BCD_frac);
  vTrace("  F_DTE      = %s\n"_Stz,F_DTE);
  vTrace("  F_FLT      = %s\n"_Stz,F_FLT);
  vTrace("  F_INT      = %s\n"_Stz,F_INT);
  vTrace("  F_IVL      = %s\n"_Stz,F_IVL);
  vTrace("  F_IVL_frac = %s\n"_Stz,F_IVL_frac);
  vTrace("  F_TME      = %s\n"_Stz,F_TME);
  vTrace("  F_TME_frac = %s\n"_Stz,F_TME_frac);
  vTrace("  F_TSP      = %s\n"_Stz,F_TSP);
  vTrace("  F_TSP_frac = %s\n"_Stz,F_TSP_frac);
}

/*----------------------------------------------------------------------------*/
/* print status of the system                                                 */
/*----------------------------------------------------------------------------*/
void Engine::vPrintStatus(void)
{
  if (Region::bTest(gxCmdLineArgs.m_xSysArgs.m_sRegionName)) {
    
    // attach to the region
    bool bResult = Region::bAttach(gxCmdLineArgs.m_xSysArgs.m_sRegionName,
                                   gxCmdLineArgs.m_xSysArgs.m_bVerbose,
                                   gxCmdLineArgs.m_xSysArgs.m_aRegionAddr);
    ASSERT(bResult);
    UNUSED(bResult);

    // get globals
    vAttachRegion();
    
    // check if the Pid in the shm is still alive
    i8 iPid = AM::iPid();
    if ( Process::bTestPid(iPid) ) {
      vTrace("Engine instance %s running\n"_Stz,
             gxCmdLineArgs.m_xSysArgs.m_sRegionName.azChar());
      return;
    }
  }
  
  vTrace("Engine instance %s not running\n"_Stz,
         gxCmdLineArgs.m_xSysArgs.m_sRegionName.azChar());
  
}

/*----------------------------------------------------------------------------*/
/* print machine information                                                  */
/*----------------------------------------------------------------------------*/
void Engine::vPrintMachine(void)
{
  if (! AM::AM::TRACE::bVerbose(C_VB_BOOT))
    return;
  
  s32 sHostName;
  Hardware::vHostName(sHostName);

  i8 iByteOrder = Hardware::iByteOrder();

  i8 iCoreCount = Hardware::iCoreCount();
 
  i8 iCacheLine = Hardware::iCacheLine();

  s32 sFreq_magn;
  i8 iCpuFrequency = Hardware::iCpuFrequency();
  MagLib::vHzMag(iCpuFrequency,sFreq_magn);

  s32 sClock_magn;
  i8 iClockResNsec = Hardware::iClockResNsec();
  MagLib::vHzMag(iClockResNsec,sClock_magn);

  s32 sL1Cache_magn;
  i8 iL1Cache= Hardware::iL1Cache();
  MagLib::vByteMag(iL1Cache,sL1Cache_magn);
  
  s32 sL2Cache_magn;
  i8 iL2Cache= Hardware::iL2Cache();
  MagLib::vByteMag(iL2Cache,sL2Cache_magn);
  
  s32 sL3Cache_magn;
  i8 iL3Cache= Hardware::iL3Cache();
  MagLib::vByteMag(iL3Cache,sL3Cache_magn);
  
  s32 sMemory_magn;
  i8 iPhysicalMemory = Hardware::iPhysicalMemory();
  MagLib::vByteMag(iPhysicalMemory,sMemory_magn);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vInfo("\n"_Stz);
  vInfo("Machine:\n"_Stz);
  
  vInfo("  Host Name : %s\n"_Stz,   sHostName.szStz());
  vInfo("  Core Count: %lld\n"_Stz, iCoreCount);
  vInfo("  Cache Line: %lldB\n"_Stz,iCacheLine);
  vInfo("  Byte Order: %lld\n"_Stz, iByteOrder);
  vInfo("  L1 Cache  : %s (per core)\n"_Stz,   sL1Cache_magn.szStz());
  vInfo("  L2 Cache  : %s (per core)\n"_Stz,   sL2Cache_magn.szStz());
  vInfo("  L3 Cache  : %s\n"_Stz,   sL3Cache_magn.szStz());
  vInfo("  Memory    : %s\n"_Stz,   sMemory_magn.szStz());
  vInfo("  Cpu Freq  : %s\n"_Stz,   sFreq_magn.szStz());
  vInfo("  Clock Res : %s\n"_Stz,   sClock_magn.szStz());
  vInfo("  Tick/sec  : %lld\n"_Stz, TickingDevice::iTicksPerSec());
  vInfo("  Tick Now  : %lld\n"_Stz, TickingDevice::iGetTick());
}

/*----------------------------------------------------------------------------*/
/* print os resource limits                                                   */
/*----------------------------------------------------------------------------*/
void Engine::vPrintRLimits(void)
{
  if (! AM::AM::TRACE::bVerbose(C_VB_BOOT))
    return;
  
  vInfo("\n"_Stz);
  vInfo("Rlimits:\n"_Stz);

  i8 nSoft;
  i8 nHard;
  
  s32 sSoft_magn;
  s32 sHard_magn;

  OpSys::vRLimitCore(nSoft,nHard);
  MagLib::vByteMag(nSoft,sSoft_magn);
  MagLib::vByteMag(nHard,sHard_magn);
  vInfo("  RLIMIT_CORE    : %4s - %4s\n"_Stz,sSoft_magn.szStz(),sHard_magn.szStz());
  
  OpSys::vRLimitCpu(nSoft,nHard);
  MagLib::vUnitMag(nSoft,sSoft_magn);
  MagLib::vUnitMag(nHard,sHard_magn);
  vInfo("  RLIMIT_CPU     : %4s - %4s\n"_Stz,sSoft_magn.szStz(),sHard_magn.szStz());
  
  OpSys::vRLimitData(nSoft,nHard);
  MagLib::vByteMag(nSoft,sSoft_magn);
  MagLib::vByteMag(nHard,sHard_magn);
  vInfo("  RLIMIT_DATA    : %4s - %4s\n"_Stz,sSoft_magn.szStz(),sHard_magn.szStz());
  
  OpSys::vRLimitFileSize(nSoft,nHard);
  MagLib::vByteMag(nSoft,sSoft_magn);
  MagLib::vByteMag(nHard,sHard_magn);
  vInfo("  RLIMIT_FSIZE   : %4s - %4s\n"_Stz,sSoft_magn.szStz(),sHard_magn.szStz());
  
  OpSys::vRLimitMemoryLocked(nSoft,nHard);
  MagLib::vByteMag(nSoft,sSoft_magn);
  MagLib::vByteMag(nHard,sHard_magn);
  vInfo("  RLIMIT_MEMLOCK : %4s - %4s\n"_Stz,sSoft_magn.szStz(),sHard_magn.szStz());
  
  OpSys::vRLimitOpenFiles(nSoft,nHard);
  MagLib::vUnitMag(nSoft,sSoft_magn);
  MagLib::vUnitMag(nHard,sHard_magn);
  vInfo("  RLIMIT_NOFILE  : %4s - %4s\n"_Stz,sSoft_magn.szStz(),sHard_magn.szStz());
  
  OpSys::vRLimitProcesses(nSoft,nHard);
  MagLib::vUnitMag(nSoft,sSoft_magn);
  MagLib::vUnitMag(nHard,sHard_magn);
  vInfo("  RLIMIT_NPROC   : %4s - %4s\n"_Stz,sSoft_magn.szStz(),sHard_magn.szStz());
  
  OpSys::vRLimitRss(nSoft,nHard);
  MagLib::vByteMag(nSoft,sSoft_magn);
  MagLib::vByteMag(nHard,sHard_magn);
  vInfo("  RLIMIT_RSS     : %4s - %4s\n"_Stz,sSoft_magn.szStz(),sHard_magn.szStz());
  
  OpSys::vRLimitStack(nSoft,nHard);
  MagLib::vByteMag(nSoft,sSoft_magn);
  MagLib::vByteMag(nHard,sHard_magn);
  vInfo("  RLIMIT_STACK   : %4s - %4s\n"_Stz,sSoft_magn.szStz(),sHard_magn.szStz());
}

/*----------------------------------------------------------------------------*/
/* print time                                                                 */
/*----------------------------------------------------------------------------*/
void Engine::vPrintTime(void)
{
  if (! AM::AM::TRACE::bVerbose(C_VB_BOOT))
    return;
  
  Now spNow;
  Now spNowUTC;
  spNowUTC.vSetNowUTC();
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {
    vInfo("\n"_Stz);
    vInfo("Time:\n"_Stz);
    vInfo("  %s (local)\n"_Stz,spNow.sz());
    vInfo("  %s (utc)\n"_Stz,spNowUTC.sz());
  }

}

/*----------------------------------------------------------------------------*/
/* print welcome                                                              */
/*----------------------------------------------------------------------------*/
void Engine::vPrintWelcome(void)
{
  s256 sWelcome;
  s256 sCopyright;

  vGetWelcome(sWelcome);
  vGetCopyright(sCopyright);

  if (gxCmdLineArgs.m_xSysArgs.m_bWelcome && ! gxCmdLineArgs.m_xSysArgs.m_bStatus) {
    vInfo("%s\n"_Stz,sWelcome.szStz());
    vInfo("%s\n"_Stz,sCopyright.szStz());
  }
}

/*----------------------------------------------------------------------------*/
/* run utilities                                                              */
/*----------------------------------------------------------------------------*/
void Engine::vRunUtility(int argc, Stz* argv[])
{
  // potentially run the shm utility
  if (gxCmdLineArgs.m_xSysArgs.m_bUteShm) {
    ShmUtility::vRun(argc,(Stz**)argv);
    return;
  }

  // potentially run the api demo
  if (gxCmdLineArgs.m_xSysArgs.m_bUteApiDemo) {
    // ... more
    return;
  }
}

/*----------------------------------------------------------------------------*/
/* startup sanity checking                                                    */
/*----------------------------------------------------------------------------*/
void Engine::vSanityCheck(void)
{
  gxCmdLineArgs.vSanityCheck();
}

/*----------------------------------------------------------------------------*/
/* signal handler                                                             */
/*----------------------------------------------------------------------------*/
void Engine::vSignalHandler(int iSignal)
{
  vTrace("Engine::vSignalHandler:\n"
         "  Node = %s\n"
         "  iSignal = %lld\n"_Stz
         
         ,AM::pGrid()->pThisNode()->szNodeNm()->azChar()
         ,(i8)iSignal
         );
  
  Stz* szWordNm = (Stz*)PsxThd::pTlsGet(giTLS_HandlerKey);
  vTrace("  last known handler = %s\n"_Stz
         ,szWordNm);
  
  Stz* szStepNm = (Stz*)PsxThd::pTlsGet(giTLS_StepKey);
  vTrace("  last known step = %s\n"_Stz
         ,szStepNm);
  

  // print out all the frames to stderr
  vThreadCallStackDump();
  
  
  vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
         "Error: signal %d:\n"_Stz, iSignal);
}

/*----------------------------------------------------------------------------*/
/* start a new system                                                         */
/*----------------------------------------------------------------------------*/
void Engine::vStartSystem(void)
{
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("\n+\n+ %lld) System Start\n+\n"_Stz,giProgressPoint++);
  
  //ASSERT( ! AM::DIC::pRoot()->bHasWords() );

  gpAttachMap->vRecordArguments(&gxCmdLineArgs);
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {
    gpAttachMap->vPrintAttachMap();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move

  gpAttachMap->m_pSchedule = new Schedule;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // job lists

  gpAttachMap->m_pJobs->vCreateJobLists();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // grid and nodes
  gpAttachMap->m_pGrid = new Grid("SEgrid"_Stz);

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Creating Grid and Node's\n"_Stz);


  vCreateGridOfNodes();
  
  Node* pThisNode = AM::pGrid()->pFindNode(gxCmdLineArgs.m_xSysArgs.m_sNodeName);

  if (! pThisNode)
    vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
           "Cannot find Node %s in the cluster"_Stz,
           gxCmdLineArgs.m_xSysArgs.m_sNodeName.azChar());


  AM::pGrid()->vThisNode(pThisNode); // newcode

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Creating Sessions\n"_Stz);


  gpAttachMap->m_pSessions = new Sessions;
  gpAttachMap->m_pSessions->vAllocInternal();

  // create the tx set
  gpAttachMap->m_pTxSet = new TxSet();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Creating Partition Map\n"_Stz);

  gpAttachMap->m_pPtnMap =
      new PtnMap(gpAttachMap->m_pGrid->nKnownNodes(),
                 gpAttachMap->m_xSysArgs.m_nPtnsPerNode);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Adding Steps\n"_Stz);
  Step::vAddSteps();

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Adding PostRecv Functions\n"_Stz);
  vPostRecvFns();
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Adding LowMemory Discard\n"_Stz);
  vLowMemoryDiscard();

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Adding No Memory Logging\n"_Stz);
  vNoMemoryLogging();

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Adding Constants\n"_Stz);
  Constants::vAddConstantsArgs();
  Constants::vAddConstantsCpp();

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Running Boot Job\n"_Stz);
  Api::vCallBoot();
 // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // threading
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Spawning Threads\n"_Stz);
  
  Scheduler::vScheduler_Boot();
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Running Start Job\n"_Stz);
  Api::RunStartJob();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // internal tables
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("  Internal Tables\n"_Stz);
  vInternalTables();

   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {
    vInfo("Engine Instance %s started\n"_Stz,gxCmdLineArgs.m_xSysArgs.m_sRegionName.azChar());
    gpAttachMap->m_pPtnMap->vSee();
  }


  s32 sResident_magn;
  MagLib::vByteMag(Process::iResidentSize(), 3, sResident_magn);
  s32 sVirtual_magn;
  MagLib::vByteMag(Process::iVirtualSize(), 3, sVirtual_magn);

  if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {

    vInfo("\n+\n+ %lld) Net Starting Position\n+\n"_Stz, giProgressPoint++);

    vTrace("Process resident size %s Virtual Size %s\n"_Stz,
           sResident_magn.szStz(),
           sVirtual_magn.szStz()
    );

    vTrace("System time %lldus User Time %lldus\n"_Stz,
           Process::iSystemUsec(),
           Process::iUserUsec()
    );

    BufferPool::vReport();

    vTrace("Open: dirs %lld files %lld mutexes %lld threads %lld sockets %lld\n"_Stz,
           *gixDir,
           *gixFile,
           *gixMutex,
           *gixPsxThd,
           *gixSocket
    );
  }
  
  // mark attachmap to be ready for connections
  gpAttachMap->vAttachMapState(C_READY);
    
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT)) {
    vInfo("\n++\n++ System Started\n++\n"_Stz);
  }
}


/**
 * @brief Initial creation of a Grid of Nodes
 * @details It uses a hash IP/Port of a node for the NodeNo
 *
 * <p>
 * @note We can use Hardware::iReadTscp() for unique NodeNo,
 *       But it seems the value is very large and somewhere int system
 *       doesn't like it.
 *
 *
 * @return none
 */
void Engine::vCreateGridOfNodes(void)
{

  s32 szNodeName;
  i8 iNodeNo =0 ;

  for (i8 i_Node = 0;
       i_Node < gxCmdLineArgs.m_xSysArgs.m_nDefinedNodes;
       i_Node++) {

    if (gxCmdLineArgs.m_xSysArgs.m_nDefinedNodes > 1) {
      iNodeNo = i_Node;
      szNodeName = gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sNodeName;
    } else {
      CheckSum ChecksumValue((unsigned char *) gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sIPAddress.azChar(),
                           gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sIPAddress.nLength());

      iNodeNo = (ChecksumValue.iGetBernstein() + gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_iPortNo);

      //szNodeName.vSprint("%s:%lld"_Stz, gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sIPAddress.azChar(),
      //                   gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_iPortNo);
      szNodeName = gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sNodeName;
   }


    Node* pNode = new Node(iNodeNo, //i_Node +  abs((i2)Hardware::iReadTscp()),
                           szNodeName, //gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sNodeName,
                           AM::SYSARGS::nPtnsPerNode(),
                           gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_sIPAddress,
                           gxCmdLineArgs.m_xSysArgs.m_xNodeDefn[i_Node].m_iPortNo,
                           AM::szRegionName());

    AM::pGrid()->vAddNode(pNode);

  }
}

/*----------------------------------------------------------------------------*/
/* stop the system                                                            */
/*----------------------------------------------------------------------------*/
void Engine::vStopSystem(void)
{
  ASSERT(gpBufferPool);
  
  Region::vUnlink(gxCmdLineArgs.m_xSysArgs.m_sRegionName);
  
  gpAttachMap->m_iStop = 1;
  
  vTrace("Engine instance %s stopped\n"_Stz,
         gxCmdLineArgs.m_xSysArgs.m_sRegionName.azChar());
  
  // the system is now stopped
}

/*----------------------------------------------------------------------------*/
/* check and see if the port has other listeneters                            */
/*----------------------------------------------------------------------------*/
void Engine::vTestPort(i8 iPort)
{
  SysCall oSysCall;
  Socket* pSocket;

  pSocket = new Socket(gpAttachMap);
  bool bResult = pSocket->bCreateServer((int)iPort, oSysCall);
  
  if ( ! bResult ) {
    if (oSysCall.iError() == EADDRINUSE) {
      vTrace("Address in use (port: %lld). "
             "CHECK IF ANOTHER INSTANCE OF RAPIDSSE IS RUNNING!\n"_Stz,
             iPort);
      
    }
    else {
      vTrace("Unable to listen for connections (port: %lld): Errno %d!\n"_Stz,
             iPort, oSysCall.iError());
    }
    //Engine::vStopSystem();
    Process::vExitFailure();
  }
    
  pSocket->bClose(oSysCall);
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("Able to open and close port %lld on this machine\n"_Stz, iPort);

  delete pSocket;
}

/*----------------------------------------------------------------------------*/
/* unlink the system                                                          */
/*----------------------------------------------------------------------------*/
void Engine::vUnlinkSystem(void)
{
  Region::vUnlink(gxCmdLineArgs.m_xSysArgs.m_sRegionName);
}

/*----------------------------------------------------------------------------*/
/* wait for quit or stop                                                      */
/*----------------------------------------------------------------------------*/
void Engine::vWaitStop(void)
{
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("[[mainThread]] Waiting on stop %s\n"_Stz,"..."_Stz);
  
  // this primary thread waits for flag indicationg systems is to stop
  while ( ! gpAttachMap->m_iQuit && ! gpAttachMap->m_iStop )
    AM::vNapMsec(10,CN_NONE);
}

/*----------------------------------------------------------------------------*/
/* write region name                                                          */
/*----------------------------------------------------------------------------*/
void Engine::vWriteRegionName(void)
{
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("\n*\n* Write Region Name\n*\n"_Stz);
 
  s256 sFileName;
  sFileName.vSprint("./SE_%s.region"_Stz,gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz());
  
  // write the regionName into a se.region file
  // used by shell script to start se prompt
  if (gxCmdLineArgs.m_xSysArgs.m_bStart) {
    LocalFile oFile;
    bool b = oFile.bOpenMode(sFileName.szStz(),C_OPEN_READ_WRITE_CREATE);
    ASSERT(b);
    UNUSED(b);
    oFile.vPrint("%s\n"_Stz,gxCmdLineArgs.m_xSysArgs.m_sRegionName.azChar());
    oFile.vClose();
  }
  
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vInfo("REGION NAME: %s\n"_Stz,gxCmdLineArgs.m_xSysArgs.m_sRegionName.azChar());
}











/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
