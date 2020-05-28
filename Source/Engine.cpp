/*============================================================================*/
/* Engine.cpp -- Rapids Storage Engine                                        */
/*============================================================================*/

#include "StorageEngine.h"

extern Args         gxArgs;

extern AttachMap*   gpAttachMap;
extern BufferPool*  gpBufferPool;
extern SizeHeap     gxaSizeHeap[];
extern Header*      gaSeShmRegion;
extern File*        gpTraceFile;

extern bool         gbSHM;

extern Stz* gsBuildGitHash;
extern Stz* gsBuildDate;
extern Stz* gsBuildHostName;
extern Stz* gsBuildUserName;
extern Stz* gsBuildBranch;

extern Atomic gixDir;
extern Atomic gixFile;
extern Atomic gixMutex;
extern Atomic gixPsxThd;
extern Atomic gixSocket;

extern i8     giProgressPoint;
extern i8     giProgressSubPoint;

extern int    gi_argc;
extern char** gi_argv;


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
  
  vOpenTrace();

  vPrintWelcome();
  
  
  if (AM::bVerboseProgress()) {
    vTrace("\n"_Stz);
    vTrace("argv[]:\n"_Stz);
    for (i8 i = 0; i < gi_argc; i++)
      vTrace("  argv[%lld] %s\n"_Stz,i,gi_argv[i]);
  }
  
  vPrintTime();

  vPrintBuild();
  
  vPrintMachine();
  
  vPrintConstants();
  vPrintFormats();

  gxArgs.vPrintStartupInfo();

  if (AM::bVerboseProgress())
     vInfo("\n*\n* %lld) Internal Initialisation\n*\n"_Stz,giProgressPoint++);

  if (AM::bVerboseProgress())
     vInfo("Setting signal handlers...\n"_Stz);
  vSetSignalHandlers();
  
  if (AM::bVerboseProgress())
     vInfo("Init TLS keys...\n"_Stz);
  TlsKeys::vInit();
  
  if (AM::bVerboseProgress())
     vInfo("Init H/W counters...\n"_Stz);
  HwCounters::bInit();
  
  if (AM::bVerboseProgress())
     vInfo("Checking assumptions...\n"_Stz);
  StorageEngine::vCheckAllAssumptions();

  if (AM::bVerboseProgress())
     vInfo("Boot check...\n"_Stz);
  AM::vBootCheck();
  
  if (AM::bVerboseProgress())
     vInfo("Sanity check...\n"_Stz);
  vSanityCheck();
  
  //vWriteRegionName();
  
  vChangeDirectory(); //! what happens if this fails?
  
  EnvCheck::vCheckManifest(); //! what happens if this fails
 
  vKillPrev();
  
  vOpenPool();
  
//  gpAttachMap->m_bMarkResource[RK_SOCKET] = true;
//  gpAttachMap->m_bMarkResource[RK_FILE] = true;
//  gpAttachMap->m_bMarkResource[RK_DIR] = true;
  
  if (AM::bVerboseProgress())
     vInfo("\n*\n* %lld) Synthesise Names\n*\n"_Stz,giProgressPoint++);
  vCreateFileNames();
  
  //vOpenTrace();
  
  vCheckNodes();
  
  vCheckPort();

  vCheckGrid();

//  if (AM::bVerboseProgress()) {
//    vInfo("\n*\n* %lld) Attach Map Contents\n*\n"_Stz,giProgressPoint++);
//    gpAttachMap->vPrintAttachMap();
//  }
  
  if (AM::bVerboseProgress()) {
     vInfo("\n**\n** Preliminaries Complete \n**\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* run command line                                                           */
/*----------------------------------------------------------------------------*/
void Engine::vRunCommandLine(void)
{
  
  eBO eBootOption = eDecodeCommandLine();

  if (AM::bVerboseProgress()) {
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
        vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
               "Unknown command line argument\n"_Stz);
    }

  }
  
  try {
    switch (eBootOption) {
        
      case BO_ADD_NODE:
        vStartSystem();
        vWaitStop();
        break;
        
      case BO_PROMPT:
        // wait for the attachmap to be ready
        if (gpAttachMap->uAttachMapState() != C_READY)
          vTrace("Waiting for SE to start...\n"_Stz);
        while (gpAttachMap->uAttachMapState() != C_READY) {
          AM::vNapMsec(1,CN_NONE);
        }
        vOpenLog();
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
        vWaitStop();
        break;
        
      case BO_START_PROMPT:
        vStartSystem();
#ifdef WITH_APPLE
        if (! gxArgs.m_bNoMunge )
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
        vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
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
  
  if (AM::bVerboseProgress())
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
/* call the boot task to compile the main dictionary                          */
/*----------------------------------------------------------------------------*/
void Engine::vCallBoot(void)
{
  JobClass* pJobClass_Boot = JobClass::pFind(CN_JOB_BOOT);
  
  Job_Boot* pJob_Boot
    = static_cast<Job_Boot*>(pJobClass_Boot->pNewJob(nullptr));
  
  pJob_Boot->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  
  pJob_Boot->vSeed(SS_SEED_RUN);
  
  Task* pBootTask = new Task(pJob_Boot);
  pBootTask->vInitExeContext();

  pBootTask->vAddAxioms();

  pBootTask->vAddMunged();
  
  if (AM::bVerboseProgress())
    vTrace("[[mainThread]] Compiling dictionaries...\n"_Stz);
  
  
  pJob_Boot->vTimerStart();
  
  vCallImmmediate(pBootTask);
  
  vDeleteImmmediate(&pBootTask);
}

/*----------------------------------------------------------------------------*/
/* run task immediately                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vCallImmmediate(Task* pImmediateTask)
{
  pImmediateTask->vSeedTask();
  
  pImmediateTask->vState(TS_EXECUTING);
  
  pImmediateTask->vTaskLoop();
}

/*----------------------------------------------------------------------------*/
/* call the prompt loop                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vCallPrompt(void)
{
//  SePrompt* pSePrompt = new SePrompt(gxArgs.m_bCommand);
//
//  pSePrompt->vPromptLoop();
//
//  delete pSePrompt;
  
  if (AM::bVerboseProgress())
    vInfo("\n>\n> %lld) Calling Prompt\n>\n"_Stz,giProgressPoint++);

  AM::vPromptCall();
}

/*----------------------------------------------------------------------------*/
/* change current directory                                                   */
/*----------------------------------------------------------------------------*/
void Engine::vChangeDirectory(void)
{
  s256 sOriginal;
  s256 sCurrent;

  if (AM::bVerboseProgress())
    vInfo("\n*\n* %lld) Change Working Directory\n*\n"_Stz,giProgressPoint++);
  
  if (Dir::bGetCurrent(sOriginal) && AM::bVerboseProgress())
    vInfo("Original working directory: %s\n"_Stz,sOriginal.szStz());
  
  // change working directory to commands dir
  bool bResult = Dir::bChangeCurrent(gxArgs.m_sCommandsDir);
  
  if (! bResult)
    Process::vUnrecoverableError("Unable to change to command dir"_Stz,
                                 gxArgs.m_sCommandsDir);

  if (Dir::bGetCurrent(sCurrent) && AM::bVerboseProgress())
    vInfo("Current working directory: %s\n"_Stz,sCurrent.szStz());
}

/*----------------------------------------------------------------------------*/
/* check grid configuration                                                   */
/*----------------------------------------------------------------------------*/
void Engine::vCheckGrid(void)
{
  // nothing to check if not running a grid test
  if ( ! gxArgs.m_bStart)
    return;
  
  if (AM::bVerboseProgress())
    vInfo("\n*\n* %lld) Checking GridTest Configuration\n*\n"_Stz,giProgressPoint++);
  
   for (i8 i_Node = 0; i_Node < gxArgs.m_nNodes; i_Node++) {
     
     if (AM::bVerboseProgress())
       vInfo("Checking %s %s %lld\n"_Stz,
             gxArgs.m_xNodes[i_Node].m_sNodeName.szStz(),
             gxArgs.m_xNodes[i_Node].m_sIPAddress.szStz(),
             gxArgs.m_xNodes[i_Node].m_iPortNo
             );
     
     if (! gxArgs.m_xNodes[i_Node].m_sIPAddress.bSame(C_LOOPBACK_IP))
       Process::vUnrecoverableError("Invalid loopback IP for grid test"_Stz,
                                    gxArgs.m_xNodes[i_Node].m_sIPAddress);
     
     if (AM::bVerboseProgress())
       vInfo("Loopback %s for %s is valid\n"_Stz,
             gxArgs.m_xNodes[i_Node].m_sIPAddress.szStz(),
             gxArgs.m_xNodes[i_Node].m_sNodeName.szStz());

     if (gxArgs.m_bGridTest)
       vTestPort(gxArgs.m_xNodes[i_Node].m_iPortNo);
   }

}

/*----------------------------------------------------------------------------*/
/* check and see if the port has other listeneters                            */
/*----------------------------------------------------------------------------*/
void Engine::vCheckNodes(void)
{
  if (! gxArgs.m_bStart)
    return;
  
  if (AM::bVerboseProgress())
    vInfo("\n*\n* %lld) Checking Node Configurations\n*\n"_Stz,giProgressPoint++);

  if (gxArgs.m_nNodes < 1)
    Process::vUnrecoverableError("No nodelist was supplied (via --node)"_Stz,
                                 gxArgs.m_sNodeName.szStz());

  if (C_UNKNOWN == gxArgs.m_iThisNode)
    Process::vUnrecoverableError("This node is not in node list"_Stz,
                                 gxArgs.m_sNodeName.szStz());

  vInfo("This is node %lld of %lld nodes\n"_Stz,
        gxArgs.m_iThisNode,
        gxArgs.m_nNodes);
}

/*----------------------------------------------------------------------------*/
/* check and see if the port has other listeneters                            */
/*----------------------------------------------------------------------------*/
void Engine::vCheckPort(void)
{
  SysCall oSysCall;
  Socket* pSocket;
  i8      iPort;
  
  // nothing to check if not starting SE
  if ( ! gxArgs.m_bStart)
    return;
  
  if (AM::bVerboseProgress())
    vInfo("\n*\n* %lld) Testing Listening Port\n*\n"_Stz,giProgressPoint++);
  
  iPort = gxArgs.m_xNodes[gxArgs.m_iThisNode].m_iPortNo;
  
  vTestPort(iPort);
  
//  pSocket = new Socket(gpAttachMap);
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
//  if (AM::bVerboseProgress())
//    vInfo("Able to open port %lld\n"_Stz, iPort);
//
//  pSocket->bClose(oSysCall);
//
//  if (AM::bVerboseProgress())
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
  if (AM::bVerboseProgress())
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
  if (AM::bVerboseProgress())
    vInfo("\n*\n* %lld) Create Attach Map\n*\n"_Stz,giProgressPoint++);
  
  // create the attach map (which records global pointers)
  new AttachMap;


}

/*----------------------------------------------------------------------------*/
/* create log and trace file name                                             */
/*----------------------------------------------------------------------------*/
void Engine::vCreateFileNames(void)
{
  
  // "SEgrid" to be gxArgs.m_sGridName.szStz()
#ifdef WITH_DEBUG_VERSION
  gpAttachMap->m_sTraceFile.vSprint("/tmp/SE_Trace_%s_%s_Debug.txt"_Stz,
                                    "SEgrid"_Stz, gxArgs.m_sNodeName.szStz());
  gpAttachMap->m_sLogRegion.vSprint("SE_Log_%s_Debug"_Stz,"SEgrid"_Stz);
  gpAttachMap->m_sLogCsvFile.vSprint("/tmp/SE_Log_%s_Debug.csv"_Stz,"SEgrid"_Stz);
  gpAttachMap->m_sLogTxtFile.vSprint("/tmp/SE_Log_%s_Debug.txt"_Stz,"SEgrid"_Stz);
#else
  gpAttachMap->m_sTraceFile.vSprint("/tmp/SE_Trace_%s_%s.txt"_Stz,
                                    "SEgrid"_Stz, gxArgs.m_sNodeName.szStz());
  gpAttachMap->m_sLogRegion.vSprint("SE_Log_%s"_Stz, "SEgrid"_Stz);
  gpAttachMap->m_sLogCsvFile.vSprint("/tmp/SE_Log_%s.csv"_Stz, "SEgrid"_Stz);
  gpAttachMap->m_sLogTxtFile.vSprint("/tmp/SE_Log_%s.txt"_Stz, "SEgrid"_Stz);
#endif
  
  if (AM::bVerboseProgress()) {
    vInfo("Trace file path: %s\n"_Stz,gpAttachMap->m_sTraceFile.szStz());
    vInfo("Log file path (csv): %s\n"_Stz,gpAttachMap->m_sLogCsvFile.szStz());
    vInfo("Log file path (text): %s\n"_Stz,gpAttachMap->m_sLogTxtFile.szStz());

    vInfo("Log region name: %s\n"_Stz,gpAttachMap->m_sLogRegion.szStz());
  }
}

/*----------------------------------------------------------------------------*/
/* decode command line from arguments                                         */
/*----------------------------------------------------------------------------*/
eBO Engine::eDecodeCommandLine(void)
{
  if (gxArgs.m_bStart && gxArgs.m_bPrompt)   return BO_START_PROMPT;
  if (gxArgs.m_bStart)                            return BO_START;
  if (gxArgs.m_bPrompt)                           return BO_PROMPT;
  if (gxArgs.m_bStatus)                           return BO_STATUS;
  if (gxArgs.m_bStop)                             return BO_STOP;
  if (gxArgs.m_bUnlink)                           return BO_UNLINK;
  if (gxArgs.m_bSethScript)                       return BO_SETH_PROMPT;
  
  if (gxArgs.m_bAddNode)                          return BO_ADD_NODE;

  return BO_NONE;
}

/*----------------------------------------------------------------------------*/
/* delete task immediate                                                      */
/*----------------------------------------------------------------------------*/
void Engine::vDeleteImmmediate(Task** ppTask)
{
  Task* pTask = *ppTask;
  *ppTask = nullptr;

  pTask->vState(TS_COMPLETED);
  Job* pJob = pTask->pJob();
  pTask->vJob(nullptr);
  delete pJob;
  
  //delete pTask;
}

/*----------------------------------------------------------------------------*/
/* delete an se session                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vDeleteSession(Session* pSession)
{
  //delete pSession;
  AM::pSessionArray()->vFreeSession(pSession);
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
           
  Correctness::vThreadCallStackDump();
}

/*----------------------------------------------------------------------------*/
/* get copyright                                                              */
/*----------------------------------------------------------------------------*/
void Engine::vGetCopyright(s256& sCopyright)
{
  sCopyright = "Copyright (c) Borrui Data 2020"_Stz;
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
  
  sWelcome.vSprint("RapidsSE %s (%s version, pid %lld)"_Stz,
                   
                   RAPIDSSE_VERSION_STRING,
                   szRelDbg,
                   Process::iGetPid()
                  );
}

/*----------------------------------------------------------------------------*/
/* kill previous node instances                                               */
/*----------------------------------------------------------------------------*/
void Engine::vKillPrev(void)
{
  if (! gxArgs.m_bKillPrev)
    return;
  
  if (AM::bVerboseProgress())
    vInfo("\n*\n* %lld) Kill Previous Occurences\n*\n"_Stz,giProgressPoint++);
  
  // an assumption is that regions will be /SE_<node>
  
  void* aRegion;
  s32 sRegionNm;
  int iFD;
  
  for (i8 i_Node = 0; i_Node < gxArgs.m_nNodes; i_Node++) {
    
    sRegionNm.vSprint("%s_%s"_Stz,
                        gxArgs.m_sRegionPrefix.azChar(),
                        gxArgs.m_xNodes[i_Node].m_sNodeName.azChar());
    
    if (! Region::bTest(sRegionNm) )
      continue;

    if (gxArgs.m_bDeveloper)
      vTrace("vKillPrev: region %s exists\n"_Stz,sRegionNm.szStz());
    
    if (! (aRegion = Region::aOpen(sRegionNm,gxArgs.m_aRegionAddr,iFD)))
      continue;
    
    AttachMap* pAttachMap = (AttachMap*)aAdd<AttachMap*>((Object*)aRegion,C_SIZE_POOL_BUFFER);
    
    i8 iPid = pAttachMap->iPid();
    if (gxArgs.m_bDeveloper)
      vTrace("vKillPrev: prev pid %lld\n"_Stz,iPid);

    if (iPid &&
        iPid != Process::iGetPid() &&
        Process::bTestPid(iPid)) {
      
      bool bResult = Process::bKillPid(iPid);
      if (gxArgs.m_bDeveloper) {
        if (bResult)
          vTrace("vKillPrev: process %lld killed\n"_Stz,iPid);
        else
          vTrace("vKillPrev: process %lld not killed\n"_Stz,iPid);
      }
    }
    
    Region::vClose(aRegion,iFD);
    
    Region::vUnlink(sRegionNm);
  }
}

/*----------------------------------------------------------------------------*/
/* get monitor portrait of system                                             */
/*----------------------------------------------------------------------------*/
void Engine::vMonitorPortrait(s48K& sRapidsPortrait)
{
  static BPA* pBPA = nullptr;
  
  s48K sPortrait;
  
  Now oNow;
  
  sRapidsPortrait.vSprint("\nSystem Monitor #%lld %s\n"_Stz,
                          gpAttachMap->m_oStatistics.m_nMonitorTake++,
                          oNow.sz());
  sRapidsPortrait.vConcat("------------------------------------------------------\n"_Stz);


  sPortrait = ""_Stz;
  vMonitorProfile(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  AM::pQueues()->vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  AM::pLockTracker()->vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  AM::pBufferPool()->vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  AM::pHeapMem()->vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  if (AM::bAttached())
    AM::pResourceTracker()->vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  AM::pQueues()->vMonitorThread(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  File::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  Socket::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  Portrait::vMemoryPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
//  sPortrait = ""_Stz;
//  Log::vMonitorPortrait(sPortrait);
//  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  Mutex::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  Task::vMonitorProfile_stats(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  PsxThd::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  Api::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  Table::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  Index::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  Index::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  AM::pSessionArray()->vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  if (1 == ++ gpAttachMap->m_inNextMonitor) {
    pBPA = new BPA;
    pBPA->vEpochTake();
  }
  else {
    pBPA->vEpochDump(sRapidsPortrait);
    pBPA->vEpochTake();
  }
}

/*----------------------------------------------------------------------------*/
/* get monitor portrait profile of system                                     */
/*----------------------------------------------------------------------------*/
void Engine::vMonitorProfile(s48K& sPortrait)
{
  
  i8 iUpTime = AM::iElapsedSsec();
  
  s32 sRegionMag;
  MagLib::vByteMag(AM::iRegionSize(),sRegionMag);

  i8 iPercentFree = AM::pBufferPool()->iFreePercent();
  Stz* aNodeName  = AM::pGrid()->pGetNode(gxArgs.m_iThisNode)->szNodeNm();
  bool bStarted   = AM::pGrid()->bHasStarted();
  bool bDdlLocked = AM::pDdlLock()->bIsWriteLocked();
  
  sPortrait.vSprint("Node %s region %s free %lld%% uptime %llds (%s)\n"
                    "     cluster %s ddl %s \n"_Stz,

                    aNodeName,
                    sRegionMag.szStz(),
                    iPercentFree,
                    iUpTime,
#ifdef WITH_DEBUG_VERSION
                    "debug"_Stz,
#else
                    "release"_Stz,
#endif

                    bStarted   ? "started" : "not-started",
                    bDdlLocked ? "locked"  : "not-locked"
                    );
}

/*----------------------------------------------------------------------------*/
/* munge check                                                                */
/*----------------------------------------------------------------------------*/
void Engine::vMungeCheck(void)
{
  // get the time of the Sen files in the Sen folder
  Dir* pDir = new Dir(nullptr);
  bool bResult = pDir->bOpen("../Sen"_Stz);
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
    sFilePath = "../Sen/"_Stz;
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
  bResult = File::bModTime("../Munge/Task_AddMunged.cpp"_Stz, tMungedTime);
  ASSERT(bResult);
  
  // is the last modified sen file time after the last munge ?
  if (tSenTime > tMungedTime) {
    
    vTrace("\n******************************************************************\n"_Stz);
    vTrace("\n MUNGING!!! (Last modified Sen file : %s)        \n\n"_Stz, sLastModified.azChar());
    vTrace("******************************************************************\n"_Stz);
    
    gxArgs.m_sCommand = "docol munge"_Stz;
    gxArgs.m_bCommand = true;
  }
}

/*----------------------------------------------------------------------------*/
/* create or open the Log                                                     */
/*----------------------------------------------------------------------------*/
void Engine::vOpenLog(void)
{
  bool bStarting = false;
  if (0 == AM::pGrid()->m_iThisNode && gxArgs.m_bStart)
    bStarting = true;
  
  AM::vLogOpenBySE(bStarting);
}

/*----------------------------------------------------------------------------*/
/* attach or create the shm region                                            */
/*----------------------------------------------------------------------------*/
void Engine::vOpenPool(void)
{
  bool bResult;
  
  if (gxArgs.m_bUnlink) {
    //vUnlink();
    return;
  }
  
  if (gxArgs.m_bStart) {
    
    if (! gxArgs.m_bRestart && Region::bTest(gxArgs.m_sRegionName)) {
      vTrace("Error: Instance %s already started\n"_Stz,gxArgs.m_sRegionName.azChar());
      Process::vExitFailure();
      return;
    }
    
    if (AM::bVerboseProgress())
      vInfo("\n*\n* %lld) Create Shared Memory Region\n*\n"_Stz,giProgressPoint++);
    
    Region::vCreate(gxArgs.m_sRegionName,
                    gxArgs.m_bVerbose,
                    gxArgs.m_aRegionAddr,
                    gxArgs.m_iRegionSize,
                    gxArgs.m_bTouch,
                    gxArgs.m_bProgress,
                    gxArgs.m_bLock,
                    gxArgs.m_bRestart,
                    gxArgs.m_bDontFree,
                    gxArgs.m_bFreeCB);
    
    if (AM::bVerboseProgress()) {
      
      s32 sRegionSize_magn;
      MagLib::vByteMag(gxArgs.m_iRegionSize,3,sRegionSize_magn);
      
       vInfo("Created new buffer pool region %p: %s size %s\n"_Stz,
            gxArgs.m_aRegionAddr,
            gxArgs.m_sRegionName.szStz(),
            sRegionSize_magn.szStz());
    }
    
    vCreateAttachMap();
    
    return;
  }
  
  if (gxArgs.m_bStatus) {
    return;
  }
  
  if (gxArgs.m_bStop) {
    if (! Region::bTest(gxArgs.m_sRegionName)) {
      vTrace("Error: Instance %s was not started\n"_Stz,
             gxArgs.m_sRegionName.azChar());
      Process::vExitFailure();
      return;
    }
    if (AM::bVerboseProgress())
      vInfo("\n*\n* Attach Region\n*\n"_Stz);
    bResult = Region::bAttach(gxArgs.m_sRegionName,
                              gxArgs.m_bVerbose,
                              gxArgs.m_aRegionAddr);
    ASSERT(bResult);
    vAttachRegion();
    return;
  }
  
  if (gxArgs.m_bPrompt || gxArgs.m_bSethScript) {
    if (! Region::bTest(gxArgs.m_sRegionName)) {
      vTrace("Error: Instance %s was not started\n"_Stz,gxArgs.m_sRegionName.azChar());
      Process::vExitFailure();
      return;
    }
    if (AM::bVerboseProgress())
      vInfo("\n*\n* Attach Region\n*\n"_Stz);
    bResult = Region::bAttach(gxArgs.m_sRegionName,
                              gxArgs.m_bVerbose,
                              gxArgs.m_aRegionAddr);
    if ( ! bResult ) {
      vTrace("Error: Unable to attach to instance %s\n"_Stz,gxArgs.m_sRegionName.azChar());
      Process::vExitFailure();
      return;
      
    }
    vAttachRegion();
    return;
  }
  
  if (AM::bVerboseProgress())
    vInfo("\n*\n* Attach Region\n*\n"_Stz);
  bResult = Region::bAttach(gxArgs.m_sRegionName,
                            gxArgs.m_bVerbose,
                            gxArgs.m_aRegionAddr);
  ASSERT(bResult);
  vAttachRegion();
}

/*----------------------------------------------------------------------------*/
/* attach or create the shm region                                            */
/*----------------------------------------------------------------------------*/
bool Engine::bOpenPool(s32& sPoolName)
{
  bool bResult = Region::bAttach(sPoolName,
                                 gxArgs.m_bVerbose,
                                 gxArgs.m_aRegionAddr);
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
    sTracePath.vSprint("/tmp/SE_Trace_%s_%s_Debug.txt"_Stz,
                       "SEgrid"_Stz,
                       gxArgs.m_sNodeName.szStz());
  #else
    sTracePath.vSprint("/tmp/SE_Trace_%s_%s.txt"_Stz,
                       "SEgrid"_Stz,
                       gxArgs.m_sNodeName.szStz());
  #endif
  //if (AM::bVerboseProgress())
  //  vInfo("\n*\n* Opening Trace File\n*\n"_Stz);
  
  gpTraceFile = new(C_VIA_MALLOC) File(nullptr);
  
  bool bOpened;

  if (gxArgs.m_bAppendTrace)
    bOpened = gpTraceFile->bOpenAppend(sTracePath);
  else
    bOpened = gpTraceFile->bOpenWrite(sTracePath);
  RASSERT(bOpened);
  
  if (AM::bVerboseProgress())
    vInfo("Opened trace file for %s: %s\n"_Stz,
          (gxArgs.m_bAppendTrace) ? "append" : "write",
          sTracePath.szStz());
}

/*----------------------------------------------------------------------------*/
/* print build                                                                */
/*----------------------------------------------------------------------------*/
void Engine::vPrintBuild(void)
{
  if (! AM::bVerboseProgress())
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
  if (! AM::bVerboseProgress())
    return;
    
  vInfo("\n"_Stz);
  vInfo("Constants:\n"_Stz);
  
  vTrace("  C_LOW_BUFFERS        = %lld\n"_Stz,C_LOW_BUFFERS);
  
  vTrace("  C_MAX_BCD_PRECISION  = %lld\n"_Stz,C_MAX_BCD_PRECISION);
  vTrace("  C_MAX_BUCKETS        = %lld\n"_Stz,C_MAX_BUCKETS);
  vTrace("  C_MAX_COLUMNS        = %lld\n"_Stz,C_MAX_COLUMNS);
  vTrace("  C_MAX_COLUMNVECS     = %lld\n"_Stz,C_MAX_COLUMNVECS);
  vTrace("  C_MAX_DEFCOLUMNS     = %lld\n"_Stz,C_MAX_DEFCOLUMNS);
  vTrace("  C_MAX_FLT_PRECISION  = %lld\n"_Stz,C_MAX_FLT_PRECISION);
  vTrace("  C_MAX_INDEXES        = %lld\n"_Stz,C_MAX_INDEXES);
  vTrace("  C_MAX_INDEX_COLUMNS  = %lld\n"_Stz,C_MAX_INDEX_COLUMNS);
  vTrace("  C_MAX_INT_PRECISION  = %lld\n"_Stz,C_MAX_INT_PRECISION);
  vTrace("  C_MAX_KEY            = %lld\n"_Stz,C_MAX_KEY);
  vTrace("  C_MAX_NESTED_WORDS   = %lld\n"_Stz,C_MAX_NESTED_WORDS);
  vTrace("  C_MAX_NODES          = %lld\n"_Stz,C_MAX_NODES);
  vTrace("  C_MAX_LOG_ENTRIES    = %lld\n"_Stz,C_MAX_LOG_ENTRIES);
  vTrace("  C_MAX_LOG_NOTE       = %lld\n"_Stz,C_MAX_LOG_NOTE);
  vTrace("  C_MAX_OPS            = %lld\n"_Stz,C_MAX_OPS);
  vTrace("  C_MAX_PARTITIONS     = %lld\n"_Stz,C_MAX_PARTITIONS);
  vTrace("  C_MAX_PATH           = %lld\n"_Stz,C_MAX_PATH);
  vTrace("  C_MAX_RECORD         = %lld\n"_Stz,C_MAX_RECORD);
  vTrace("  C_MAX_SESSIONS       = %lld\n"_Stz,C_MAX_SESSIONS);
  vTrace("  C_MAX_SHM_PERCENT    = %lld\n"_Stz,C_MAX_SHM_PERCENT);
  vTrace("  C_MAX_SHORTCUTS      = %lld\n"_Stz,C_MAX_SHORTCUTS);
  vTrace("  C_MAX_SLOTS          = %lld\n"_Stz,C_MAX_SLOTS);
  vTrace("  C_MAX_STAGES         = %lld\n"_Stz,C_MAX_STAGES);
  vTrace("  C_MAX_STRANDS        = %lld\n"_Stz,C_MAX_STRANDS);
  vTrace("  C_MAX_STREAMDEFS     = %lld\n"_Stz,C_MAX_STREAMDEFS);
  vTrace("  C_MAX_THREADS        = %lld\n"_Stz,C_MAX_THREADS);
  vTrace("  C_MAX_TOKENS         = %lld\n"_Stz,C_MAX_TOKENS);
  vTrace("  C_MAX_VCH            = %lld\n"_Stz,C_MAX_VCH);
  vTrace("  C_MAX_VCH_PRECISION  = %lld\n"_Stz,C_MAX_FLT_PRECISION);
  
  vTrace("  C_MIN_SHM_REGION     = %lld\n"_Stz,C_MIN_SHM_REGION);
  
  vTrace("  C_WSP_DATACHUNK      = %lld\n"_Stz,C_WSP_DATACHUNK);
  vTrace("  C_WSP_STACK          = %lld\n"_Stz,C_WSP_STACK);
  
  vTrace("  C_SIZE_SMALL_OBJECT  = %lld\n"_Stz,C_SIZE_SMALL_OBJECT);
  vTrace("  C_SIZE_MEDIUM_OBJECT = %lld\n"_Stz,C_SIZE_MEDIUM_OBJECT);
  vTrace("  C_SIZE_LARGE_OBJECT  = %lld\n"_Stz,C_SIZE_LARGE_OBJECT);
  vTrace("  C_SIZE_POOL_BUFFER   = %lld\n"_Stz,C_SIZE_POOL_BUFFER);
}

/*----------------------------------------------------------------------------*/
/* print exit                                                                 */
/*----------------------------------------------------------------------------*/
void Engine::vPrintExit(void)
{
  if (AM::bVerboseProgress())
    vInfo("Engine exits\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print important constants                                                  */
/*----------------------------------------------------------------------------*/
void Engine::vPrintFormats(void)
{
  if (! AM::bVerboseProgress())
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
  if (Region::bTest(gxArgs.m_sRegionName)) {
    
    // attach to the region
    bool bResult = Region::bAttach(gxArgs.m_sRegionName,
                                   gxArgs.m_bVerbose,
                                   gxArgs.m_aRegionAddr);
    ASSERT(bResult);
    
    // get globals
    vAttachRegion();
    
    // check if the Pid in the shm is still alive
    i8 iPid = AM::iPid();
    if ( Process::bTestPid(iPid) ) {
      vTrace("Engine instance %s running\n"_Stz,
             gxArgs.m_sRegionName.azChar());
      return;
    }
  }
  
  vTrace("Engine instance %s not running\n"_Stz,
         gxArgs.m_sRegionName.azChar());
  
}

/*----------------------------------------------------------------------------*/
/* print welcome                                                              */
/*----------------------------------------------------------------------------*/
void Engine::vPrintMachine(void)
{
  if (! AM::bVerboseProgress())
    return;
  
  s32 sHostName;
  Hardware::vHostName(sHostName);

  i8 iCoreCount = Hardware::iCoreCount();
 
  i8 iCacheLine = Hardware::iCacheLine();

  s32 sFreq_magn;
  i8 iCpuFrequency = Hardware::iCpuFrequency();
  MagLib::vHzMag(iCpuFrequency,sFreq_magn);

  s32 sClock_magn;
  i8 iClockResNsec = Hardware::iClockResNsec();
  MagLib::vHzMag(iClockResNsec,sClock_magn);

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
  vInfo("  L2 Cache  : %s (per core)\n"_Stz,   sL2Cache_magn.szStz());
  vInfo("  L3 Cache  : %s\n"_Stz,   sL3Cache_magn.szStz());
  vInfo("  Memory    : %s\n"_Stz,   sMemory_magn.szStz());
  vInfo("  Cpu Freq  : %s\n"_Stz,   sFreq_magn.szStz());
  vInfo("  Clock Res : %s\n"_Stz,   sClock_magn.szStz());
  vInfo("  Tick/sec  : %lld\n"_Stz, TickingDevice::iTicksPerSec());
  vInfo("  Tick Now  : %lld\n"_Stz, TickingDevice::iGetTick());
}

/*----------------------------------------------------------------------------*/
/* print time                                                                 */
/*----------------------------------------------------------------------------*/
void Engine::vPrintTime(void)
{
  if (! AM::bVerboseProgress())
    return;
  
  Now spNow;
  Now spNowUTC;
  spNowUTC.vSetNowUTC();
  
  if (AM::bVerboseProgress()) {
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

  if (gxArgs.m_bWelcome && ! gxArgs.m_bStatus) {
    vInfo("%s\n"_Stz,sWelcome.szStz());
    vInfo("%s\n"_Stz,sCopyright.szStz());
  }
}

/*----------------------------------------------------------------------------*/
/* run utilities                                                              */
/*----------------------------------------------------------------------------*/
bool Engine::bRunUtilities(int argc, Stz* argv[])
{
  // potentially run the shm utility
  if (gbSHM) {
    ShmUtility::vRun(argc,(Stz**)argv);
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* startup sanity checking                                                    */
/*----------------------------------------------------------------------------*/
void Engine::vSanityCheck(void)
{
  gxArgs.vSanityCheck();
}

/*----------------------------------------------------------------------------*/
/* signal handler                                                             */
/*----------------------------------------------------------------------------*/
void Engine::vSignalHandler(int iSignal)
{
  fprintf(stderr,
          "Engine::vSignalHandler:\n"
          "Node = %s\n"
          "iSignal = %lld\n",
          AM::pGrid()->pThisNode()->szNodeNm()->azChar(),
          (i8)iSignal);
  
  vThreadCallStackDump();
  
  // print out all the frames to stderr
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "Error: signal %d:\n"_Stz, iSignal);
}

/*----------------------------------------------------------------------------*/
/* start the system tasks                                                     */
/*----------------------------------------------------------------------------*/
void Engine::vSpawnJobStart(void)
{
  if (AM::bVerboseProgress())
    vInfo("\n+\n+ %lld) Start System Jobs\n+\n"_Stz,giProgressPoint++);
  
  PsxThd::vMarkNonSeThread();
  
  JobClass* pJobClass_Start = JobClass::pFind(CN_JOB_START);
  
  Job_Start* pJob_Start
    = static_cast<Job_Start*>(pJobClass_Start->pNewJob(nullptr));
  
  pJob_Start->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  
  pJob_Start->vCopyTags(pJobClass_Start);

  //pJob_Start->pTagSet()->vContentDump();
  
  pJob_Start->vSeed(SS_SEED_RUN);
  
  Api::vSpawnJob_system(pJob_Start);
}

/*----------------------------------------------------------------------------*/
/* start a new system                                                         */
/*----------------------------------------------------------------------------*/
void Engine::vStartSystem(void)
{
  if (AM::bVerboseProgress())
    vInfo("\n+\n+ %lld) System Start\n+\n"_Stz,giProgressPoint++);
  
  //ASSERT( ! AM::pRootDic()->bHasWords() );
    
  gpAttachMap->vRecordArguments(&gxArgs);
  
  if (AM::bVerboseProgress()) {
    gpAttachMap->vPrintAttachMap();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move
  
  gpAttachMap->m_pFocalObjects = new FocalObjects;
  gpAttachMap->m_pQueues = new Queues;
    
  gpAttachMap->m_pGrid = new Grid(gxArgs.m_nNodes, gxArgs.m_nSlotsPerNode);
  
  Article* pGridArticle = gpAttachMap->m_pGrid->pArticle();
  
  AM::pLexiconArticle()->vAppend(pGridArticle);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // grid and nodes
  
  if (AM::bVerboseProgress())
    vInfo("  Creating Grid and Node's\n"_Stz);

  for (i8 i_Node = 0; i_Node < AM::pGrid()->nDefinedNodes(); i_Node++) {
    Node* pNode = new Node(i_Node,
                           gxArgs.m_xNodes[i_Node].m_sNodeName,
                           gxArgs.m_xNodes[i_Node].m_sIPAddress,
                           gxArgs.m_xNodes[i_Node].m_iPortNo);
    ASSERT(CN_NODE == pNode->eClass());
    AM::pGrid()->vAddNode(pNode);
    
    Article* pNodeArticle = pNode->pArticle();
    
    pGridArticle->vAppend(pNodeArticle);
  }
  
  Node* pThisNode = AM::pGrid()->pFindNode(gxArgs.m_sNodeName);
  if (! pThisNode)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Cannot find Node %s in the cluster"_Stz,
           gxArgs.m_sNodeName.azChar());
  
  i8 iThisNodeNo = pThisNode->iNodeNo();
  AM::pGrid()->vThisNode(iThisNodeNo);
  
  // initialize the next id generators
  gpAttachMap->m_inNextJobId.vNodeNo(iThisNodeNo);
  gpAttachMap->m_inNextOpId.vNodeNo(iThisNodeNo);
  gpAttachMap->m_inNextPlanId.vNodeNo(iThisNodeNo);
  gpAttachMap->m_inNextResourceId.vNodeNo(iThisNodeNo);
  gpAttachMap->m_inNextStreamId.vNodeNo(iThisNodeNo);
  gpAttachMap->m_inNextTaskId.vNodeNo(iThisNodeNo);
  gpAttachMap->m_inNextThreadId.vNodeNo(iThisNodeNo);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::bVerboseProgress())
    vInfo("  Creating SessionArray\n"_Stz);


  gpAttachMap->m_pSessionArray = new SessionArray;
  
  // create the tx set
  gpAttachMap->m_pTxSet = new TxSet();
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (AM::bVerboseProgress())
    vInfo("  Opening Shared Log\n"_Stz);
  vOpenLog();
  
  
  if (AM::bVerboseProgress())
    vInfo("  Adding Steps\n"_Stz);
  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "adding steps"_Stz); //!format-LogSys
  Step::vAddSteps();
  
  
  if (AM::bVerboseProgress())
    vInfo("  Adding Constants\n"_Stz);
  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "adding constants"_Stz); //!format-LogSys
  Constants::vAddConstantsArgs();
  Constants::vAddConstantsCpp();

  
  if (AM::bVerboseProgress())
    vInfo("  Running Boot Job\n"_Stz);
  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "running boot job"_Stz); //!format-LogSys
  vCallBoot();
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  if (AM::bVerboseProgress())
    vInfo("  Verify Job Seeds\n"_Stz);
  JobClass::vVerifySeeds();
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // threading
  
  if (AM::bVerboseProgress())
    vInfo("  Spawning Threads\n"_Stz);
  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "spawning threads"_Stz); //!format-LogSys
  // spawn general threads
  for (i8 iThreads = 0; iThreads < gxArgs.m_nThreads; iThreads++) {
    Thread* pThread = new Thread;
    pThread->vSpawnPsxThd_general();
  }
  
  // spawn dummy threads
  for (i8 iThreads = 0; iThreads < gxArgs.m_nDummyThreads; iThreads++) {
    Thread* pThread = new Thread;
    pThread->vSpawnPsxThd_dummy();
  }
  
  // pause till all general threads are operational 
  Scheduler::vThread_NapTillGeneralThreads();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (AM::bVerboseProgress())
    vInfo("  Running Start\n"_Stz);
  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "running Start job"_Stz); //!format-LogSys
  vSpawnJobStart();
  
  // wait for all jobs to complete before proceeding
  Api::vWaitForAllJobsToComplete();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "instance %s started"_Stz,
              gxArgs.m_sRegionName.azChar()); //!format-LogSys
  
  if (AM::bVerboseProgress()) {
    vInfo("Engine Instance %s started\n"_Stz,gxArgs.m_sRegionName.azChar());
    AM::pGrid()->vDumpPartitioning();
  }
  
  s32 sResident_magn;
  MagLib::vByteMag(Process::iResidentSize(),3,sResident_magn);
  s32 sVirtual_magn;
  MagLib::vByteMag(Process::iVirtualSize(),3,sVirtual_magn);

  if (AM::bVerboseProgress()) {

    vInfo("\n+\n+ %lld) Net Starting Position\n+\n"_Stz,giProgressPoint++);

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

    // mark attachmap to be ready for connections
    gpAttachMap->vAttachMapState(C_READY);
    
    vInfo("\n++\n++ System Started\n++\n"_Stz);
  }

}

/*----------------------------------------------------------------------------*/
/* stop the system                                                            */
/*----------------------------------------------------------------------------*/
void Engine::vStopSystem(void)
{
  ASSERT(gpBufferPool);
  
  Region::vUnlink(gxArgs.m_sRegionName);
  
  gpAttachMap->m_iStop = 1;
  
  vTrace("Engine instance %s stopped\n"_Stz,
         gxArgs.m_sRegionName.azChar());
  
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
  
  if (AM::bVerboseProgress())
    vInfo("Able to open and close port %lld on this machine\n"_Stz, iPort);

  delete pSocket;
}

/*----------------------------------------------------------------------------*/
/* unlink the system                                                          */
/*----------------------------------------------------------------------------*/
void Engine::vUnlinkSystem(void)
{
  Region::vUnlink(gxArgs.m_sRegionName);
}

/*----------------------------------------------------------------------------*/
/* wait for quit or stop                                                      */
/*----------------------------------------------------------------------------*/
void Engine::vWaitStop(void)
{
  if (AM::bVerboseProgress())
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
  if (AM::bVerboseProgress())
    vInfo("\n*\n* Write Region Name\n*\n"_Stz);
 
  s256 sFileName;
  sFileName.vSprint("./SE_%s.region"_Stz,gxArgs.m_sNodeName.szStz());
  
  // write the regionName into a se.region file
  // used by shell script to start se prompt
  if (gxArgs.m_bStart) {
    File oFile(nullptr);
    bool b = oFile.bOpenMode(sFileName.szStz(),C_OPEN_READ_WRITE_CREATE);
    ASSERT(b);
    oFile.vPrint("%s\n"_Stz,gxArgs.m_sRegionName.azChar());
    oFile.vClose();
  }
  
  if (AM::bVerboseProgress())
    vInfo("REGION NAME: %s\n"_Stz,gxArgs.m_sRegionName.azChar());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
