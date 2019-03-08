/*============================================================================*/
/* Engine.cpp -- Rapids Storage Engine                                        */
/*============================================================================*/

#include "StorageEngine.h"
#include "ShmUtility.hpp"

Arguments  gxArguments;

Header*       gaSeShmRegion  = nullptr;
BufferPool*   gpBufferPool   = nullptr;
AttachMap *   gpAttachMap    = nullptr;
File*         gpTraceFile    = nullptr;

extern pthread_key_t giTLS_ThreadKey;

bool          gbSHM          = false;


/*----------------------------------------------------------------------------*/
/* main entry                                                                 */
/*----------------------------------------------------------------------------*/
void Engine::vMainEntry(void)
{
  //OpSys::vGetInfo();
  // OpSys::vTraceInfo();
  
  vPrintWelcome();
  
  Base::vTraceWith();
  
  vSetSignalHandlers();
  
  // init the tls keys
  TlsKeys::vInit();
  
  HwCounters::bInit();
  
  StorageEngine::vCheckAllAssumptions();

  vSanityCheck();
  
  vWriteRegionName();
  
  vChangeDirectory();
  
  vKillPrev();
  
  vOpenPool();
  
//  gpAttachMap->m_bMarkResource[SR_SOCKET] = true;
//  gpAttachMap->m_bMarkResource[SR_FILE] = true;
//  gpAttachMap->m_bMarkResource[SR_DIR] = true;

  vOpenTraceFile();
  
  if (gxArguments.m_bStart)
    vSetGlobals();

  vCheckPort();
  
  //gpAttachMap->vPrintAttachMap();
  
  vRunCmdLine();
  
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
  gpAttachMap = pAdd<AttachMap*>((Object*)gaSeShmRegion,C_SIZE_POOL_BUFFER);
  
  // get pointers to global objects from attach map
  gpAttachMap->vGetGlobals();
  
  if (gpAttachMap->m_bVerbose)
    AttachMap::vDump(CN_ATTACHMAP, gpAttachMap, Stz::aTab(1));
}

/*----------------------------------------------------------------------------*/
/* call the boot task to compile the main dictionary                          */
/*----------------------------------------------------------------------------*/
void Engine::vCallBoot(void)
{
  JobClass* pJobClass_Boot = JobClass::pFind(CN_JOB_BOOT);
  
  Job_Boot* pJob_Boot
  = static_cast<Job_Boot*>(pJobClass_Boot->pNewJob(nullptr));
  
  pJob_Boot->vSeed(SS_SEED_RUN);
  
  Task* pBootTask = new Task(pJob_Boot);
  pBootTask->vNewStacks();
  
  pBootTask->vAddBaseWords();
  
  if (gpAttachMap->m_bVerbose)
    vTrace("[[mainThread]] Compiling dictionaries...\n"_Stz);
  
  vCallImmmediate(pBootTask);
  
  vDeleteImmmediate(&pBootTask);
}

/*----------------------------------------------------------------------------*/
/* run task immediately                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vCallImmmediate(Task* pImmediateTask)
{
  pImmediateTask->vSeedJob();
  
  pImmediateTask->vState(TK_EXECUTING);
  
  pImmediateTask->vTaskLoop();
}

/*----------------------------------------------------------------------------*/
/* call the prompt task                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vCallPrompt(void)
{
  SePrompt* pSePrompt = new SePrompt(gxArguments.m_bCommand);
  
  pSePrompt->vPromptLoop();
  
  delete pSePrompt;
}

/*----------------------------------------------------------------------------*/
/* change current directory                                                   */
/*----------------------------------------------------------------------------*/
void Engine::vChangeDirectory(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Change Working Directory\n*\n"_Stz);
  
  // change working directory to commands dir
  bool bResult = Dir::bChangeCurrent(gxArguments.m_sCommandsDir);

  s256 sCurrent;
  if (Dir::bGetCurrent(sCurrent) && gxArguments.m_bVerbose)
    vInfo("CURRENT WORKING DIRECTORY: %s\n"_Stz,sCurrent.aStz());
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
  if ( ! gxArguments.m_bStart)
    return;
  
  iPort = gxArguments.m_xNodes[gxArguments.m_iThisNode].m_iPortNo;
  if (gxArguments.m_bVerbose)
    vInfo("Looking for listeners on port %lld\n"_Stz, iPort);
  
  pSocket = new Socket;
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
    exit(EXIT_FAILURE);
  }
  
  pSocket->bClose(oSysCall);
}

/*----------------------------------------------------------------------------*/
/* close the shm region                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vClosePool(void)
{
  //delete gpBufferPool;
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void Engine::vCreateAttachMap(void)
{
  // create the attach map (which records global pointers)
  new AttachMap;

  if (gxArguments.m_bVerbose)
    vInfo("AttachMap created: sizeof(AttachMap)=%lldB\n"_Stz,
          sizeof(AttachMap));

}

/*----------------------------------------------------------------------------*/
/* delete task immediate                                                      */
/*----------------------------------------------------------------------------*/
void Engine::vDeleteImmmediate(Task** ppTask)
{
  Task* pTask = *ppTask;
  *ppTask = nullptr;

  Job* pJob = pTask->pJob();
  pTask->vJob(nullptr);
  delete pJob;
  
  delete pTask;
}

/*----------------------------------------------------------------------------*/
/* delete an se session                                                       */
/*----------------------------------------------------------------------------*/
void Engine::vDeleteSession(Session* pSession)
{
  delete pSession;
}

/*----------------------------------------------------------------------------*/
/* error handler                                                              */
/*----------------------------------------------------------------------------*/
void Engine::vErrorHandler(Error* pError)
{
  char caFileName[256];
  
  const char* ps;
  
  if (pError) {
    ps = StrLib::sLast(pError->aFileName()->aChar());
    StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
    StrLib::vCopy(caFileName,ps+1);
    vTrace("%s: line %lld in %s\n"_Stz,
           pError->aErrorMsg(),
           pError->iLineNo(),
           caFileName);
  }
  else {
    vTrace("Engine::vErrorHandler: Unknown error\n"_Stz);
  }
           
  Debugging::vStackDump();
}

/*----------------------------------------------------------------------------*/
/* print welcome                                                              */
/*----------------------------------------------------------------------------*/
void Engine::vGetWelcome(s256& sWelcome)
{
  Stz* aRelDbg;
#ifdef WITH_DEBUG_VERSION
  aRelDbg = "debug"_Stz;
#else
  aRelDbg = "release"_Stz;
#endif
  
  sWelcome.vSprint("RapidsSE v%s (Compiled %s %s -- %s) pid %lld\n"
                   "Copyright (c) Borrui Data 2019"_Stz,
                     RAPIDSSE_VERSION_STRING,
                     __DATE__,
                     __TIME__,
                     aRelDbg,
                     Process::iGetPid()
                     );
}

/*----------------------------------------------------------------------------*/
/* kill previous node instances                                               */
/*----------------------------------------------------------------------------*/
void Engine::vKillPrev(void)
{
  if (! gxArguments.m_bKillPrev)
    return;
  
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Kill Previous Occurences\n*\n"_Stz);
  
  // an assumption is that regions will be /SE_<node>
  
  void* aRegion;
  s32 sRegionNm;
  int iFD;
  
  for (i8 i_Node = 0; i_Node < gxArguments.m_nNodes; i_Node++) {
    
    sRegionNm.vSprint("%s_%s"_Stz,
                        gxArguments.m_sRegionPrefix.aChar(),
                        gxArguments.m_xNodes[i_Node].m_sNodeName.aChar());
    
    if (! Region::bTest(sRegionNm) )
      continue;

    if (gxArguments.m_bDeveloper)
      vTrace("vKillPrev: region %s exists\n"_Stz,sRegionNm.aStz());
    
    if (! (aRegion = Region::aOpen(sRegionNm,gxArguments.m_aRegionAddr,iFD)))
      continue;
    
    AttachMap* pAttachMap = (AttachMap*)ahAdd<AttachMap*>((Object*)aRegion,C_SIZE_POOL_BUFFER);
    
    if (gxArguments.m_bDeveloper)
      vTrace("vKillPrev: prev pid %lld\n"_Stz,pAttachMap->m_iPid);

    if (pAttachMap->m_iPid &&
        pAttachMap->m_iPid != Process::iGetPid() &&
        Process::bTestPid(pAttachMap->m_iPid)) {
      
      bool bResult = Process::bKillPid(pAttachMap->m_iPid);
      if (gxArguments.m_bDeveloper) {
        if (bResult)
          vTrace("vKillPrev: process %lld killed\n"_Stz,pAttachMap->m_iPid);
        else
          vTrace("vKillPrev: process %lld not killed\n"_Stz,pAttachMap->m_iPid);
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
  s48K sPortrait;
  
  Now oNow;
  
  sRapidsPortrait.vSprint("\nSystem Monitor #%lld %s\n"_Stz,
                          gpAttachMap->m_oStatistics.m_nMonitorTake++,
                          oNow.sFormatted());
  sRapidsPortrait.vConcat("------------------------------------------------------\n"_Stz);


  sPortrait = ""_Stz;
  vMonitorProfile(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  gpAttachMap->m_pQueues->vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  gpAttachMap->m_oLockTracker.vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  gpAttachMap->m_pBufferPool->vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  gpAttachMap->m_xHeapMem.vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  gpAttachMap->m_pQueues->vMonitorThread(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  File::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  Socket::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  Statistics::vMemoryPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  ActivityMap::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  Mutex::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  Task::vMonitorProfile_stats(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  Pthread::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  Api::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  Table::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  Index::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  IndexTree::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  sPortrait = ""_Stz;
  gpAttachMap->m_pApiState->vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.aStz());
  
  if (1 == *gpAttachMap->m_ixMonitorTake)
    BPA::vEpochTake();
  else {
    BPA::vEpochDump(sRapidsPortrait);
    BPA::vEpochTake();
  }
}

/*----------------------------------------------------------------------------*/
/* get monitor portrait profile of system                                     */
/*----------------------------------------------------------------------------*/
void Engine::vMonitorProfile(s48K& sPortrait)
{
  Since oSince;
  oSince.vNow();
  
  i8 iUpTime = oSince.iSeconds() - gpAttachMap->m_oSince.iSeconds();
  
  s32 sRegionMag;
  Magnitudes::vByteMag(gpAttachMap->Arg.m_iRegionSize,sRegionMag);

  i8 iPercentFree = gpAttachMap->m_pBufferPool->iFreePercent();
  Stz* aNodeName  = gpAttachMap->m_pGrid->pGetNode(gxArguments.m_iThisNode)->aNodeName();
  bool bStarted   = gpAttachMap->m_pGrid->bHasStarted();
  bool bDdlLocked = gpAttachMap->m_okDdlLock.bIsWriteLocked();
  
  sPortrait.vSprint("Node %s region %s free %lld%% uptime %llds (%s)\n"
                    "     cluster %s ddl %s \n"_Stz,

                    aNodeName,
                    sRegionMag.aStz(),
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
/* create or open the ActionSequence map                                      */
/*----------------------------------------------------------------------------*/
void Engine::vOpenActionSequence(void)
{
  bool bStarting = false;
  if (0 == gpAttachMap->m_pGrid->m_iThisNode && gxArguments.m_bStart)
    bStarting = true;
  
  ActionSequence::vOpenBySE(bStarting);
}

/*----------------------------------------------------------------------------*/
/* create or open the ActivityMap                                             */
/*----------------------------------------------------------------------------*/
void Engine::vOpenActivityMap(void)
{
  bool bStarting = false;
  if (0 == gpAttachMap->m_pGrid->m_iThisNode && gxArguments.m_bStart)
    bStarting = true;
  
  ActivityMap::vOpenBySE(bStarting);
}

/*----------------------------------------------------------------------------*/
/* attach or create the shm region                                            */
/*----------------------------------------------------------------------------*/
void Engine::vOpenPool(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Pool Open\n*\n"_Stz);
  
  bool bResult;
  
  if (gxArguments.m_bUnlink) {
    //vUnlink();
    return;
  }
  
  if (gxArguments.m_bStart) {
    
    if (! gxArguments.m_bRestart && Region::bTest(gxArguments.m_sRegionName)) {
      vTrace("Error: Instance %s already started\n"_Stz,gxArguments.m_sRegionName.aChar());
      exit(EXIT_FAILURE);
      return;
    }
    
    if (gxArguments.m_bVerbose)
      vInfo("\n*\n* Create Region\n*\n"_Stz);
    
    Region::vCreate(gxArguments.m_sRegionName,
                    gxArguments.m_bVerbose,
                    gxArguments.m_aRegionAddr,
                    gxArguments.m_iRegionSize,
                    gxArguments.m_nJumboTook,
                    gxArguments.m_bTouch,
                    gxArguments.m_bProgress,
                    gxArguments.m_bLock,
                    gxArguments.m_bRestart,
                    gxArguments.m_bDontFree,
                    gxArguments.m_bFreeRB);
    
    if (gxArguments.m_bVerbose)
      vInfo("\n*\n* Create AttachMap\n*\n"_Stz);
    vCreateAttachMap();
    
    return;
  }
  
  if (gxArguments.m_bStatus) {
    return;
  }
  
  if (gxArguments.m_bStop) {
    if (! Region::bTest(gxArguments.m_sRegionName)) {
      vTrace("Error: Instance %s was not started\n"_Stz,
             gxArguments.m_sRegionName.aChar());
      exit(EXIT_FAILURE);
      return;
    }
    if (gxArguments.m_bVerbose)
      vInfo("\n*\n* Attach Region\n*\n"_Stz);
    bResult = Region::bAttach(gxArguments.m_sRegionName,
                              gxArguments.m_bVerbose,
                              gxArguments.m_aRegionAddr);
    ASSERT(bResult);
    vAttachRegion();
    return;
  }
  
  if (gxArguments.m_bPrompt || gxArguments.m_bSethScript) {
    if (! Region::bTest(gxArguments.m_sRegionName)) {
      vTrace("Error: Instance %s was not started\n"_Stz,gxArguments.m_sRegionName.aChar());
      exit(EXIT_FAILURE);
      return;
    }
    if (gxArguments.m_bVerbose)
      vInfo("\n*\n* Attach Region\n*\n"_Stz);
    bResult = Region::bAttach(gxArguments.m_sRegionName,
                              gxArguments.m_bVerbose,
                              gxArguments.m_aRegionAddr);
    if ( ! bResult ) {
      vTrace("Error: Unable to attach to instance %s\n"_Stz,gxArguments.m_sRegionName.aChar());
      exit(EXIT_FAILURE);
      return;
      
    }
    vAttachRegion();
    return;
  }
  
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Attach Region\n*\n"_Stz);
  bResult = Region::bAttach(gxArguments.m_sRegionName,
                            gxArguments.m_bVerbose,
                            gxArguments.m_aRegionAddr);
  ASSERT(bResult);
  vAttachRegion();
  //vFatal("Error: Engine missing arguments\n"_Stz);
  //exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* attach or create the shm region                                            */
/*----------------------------------------------------------------------------*/
bool Engine::bOpenPool(s32& sPoolName)
{
  bool bResult = Region::bAttach(sPoolName,
                                 gxArguments.m_bVerbose,
                                 gxArguments.m_aRegionAddr);
  if (!bResult)
    return bResult;
  
  vAttachRegion();
  return true;
}

/*----------------------------------------------------------------------------*/
/* open trace file                                                            */
/*----------------------------------------------------------------------------*/
void Engine::vOpenTraceFile(void)
{
  gpTraceFile = new File;
  
  bool bOpened = gpTraceFile->bOpenWrite("/tmp/se_trace.txt"_Stz);
  RASSERT(bOpened);
  
  if (gxArguments.m_bVerbose)
    vInfo("** trace file opened: /tmp/se_trace.txt\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print exit                                                                 */
/*----------------------------------------------------------------------------*/
void Engine::vPrintExit(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("Engine exits\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print status of the system                                                 */
/*----------------------------------------------------------------------------*/
void Engine::vPrintStatus(void)
{
  if (Region::bTest(gxArguments.m_sRegionName)) {
    
    // attach to the region
    bool bResult = Region::bAttach(gxArguments.m_sRegionName,
                                   gxArguments.m_bVerbose,
                                   gxArguments.m_aRegionAddr);
    ASSERT(bResult);
    
    // get globals
    vAttachRegion();
    
    // check if the Pid in the shm is still alive
    if ( Process::bTestPid(gpAttachMap->m_iPid) ) {
      vTrace("Engine instance %s running\n"_Stz,
             gxArguments.m_sRegionName.aChar());
      return;
    }
  }
  
  vTrace("Engine instance %s not running\n"_Stz,
         gxArguments.m_sRegionName.aChar());
  
}

/*----------------------------------------------------------------------------*/
/* print welcome                                                              */
/*----------------------------------------------------------------------------*/
void Engine::vPrintWelcome(void)
{
  s256 sWelcome;
  
  vGetWelcome(sWelcome);
  
  if (gxArguments.m_bWelcome && ! gxArguments.m_bStatus)
    vInfo("%s\n"_Stz,sWelcome.aStz());
  
  if (gxArguments.m_bVerbose) {
#ifdef MUNGE
    vTrace("  Compiled with MUNGE\n\n"_Stz);
#else
    vTrace("  Not compiled with MUNGE\n\n"_Stz);
#endif
  }
  
  // print startup information
  if (gxArguments.m_bWelcome && gxArguments.m_bStart)
    gxArguments.vPrintStartupInfo();
}

/*----------------------------------------------------------------------------*/
/* run command line                                                           */
/*----------------------------------------------------------------------------*/
void Engine::vRunCmdLine(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Execute Command Line\n*\n"_Stz);
  
  try {
    
    if (gxArguments.m_bStart) {
      
      vStartSystem();
      
      if (gxArguments.m_bPrompt)
        vCallPrompt();
      else
        vWaitStop();
    }
    
    else if (gxArguments.m_bPrompt) {
      vOpenActivityMap();
      vOpenActionSequence();
      vCallPrompt();
    }
    else if (gxArguments.m_bStatus)  vPrintStatus();
    else if (gxArguments.m_bStop)    vStopSystem();
    else if (gxArguments.m_bUnlink)  vUnlinkSystem();
    else if (gxArguments.m_bSethScript)  { ASSERT(gpBufferPool); vCallPrompt(); }
    else {
      vFatal("Unknown command line argument\n"_Stz);
    }
    
  }
  
  catch (Error* pError) {
    vErrorHandler(pError);
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
  gxArguments.vSanityCheck();
}

/*----------------------------------------------------------------------------*/
/* set globals                                                                */
/*----------------------------------------------------------------------------*/
void Engine::vSetGlobals(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Set Globals From Arguments\n*\n"_Stz);
  
  // set globals from gxArguments
  gpAttachMap->m_bVerbose      = gxArguments.m_bVerbose;
  gpAttachMap->m_bTracepoint   = gxArguments.m_bTracepoint;
  gpAttachMap->m_bProgress     = gxArguments.m_bProgress;
  gpAttachMap->m_bMetadata     = gxArguments.m_bMetadata;
  gpAttachMap->m_nIndexEntries = gxArguments.m_nEntries;
}

/*----------------------------------------------------------------------------*/
/* signal handler                                                             */
/*----------------------------------------------------------------------------*/
void Engine::vSignalHandler(int iSignal)
{
  // print out all the frames to stderr
  vFatal("Error: signal %d:\n"_Stz, iSignal);
  
  Debugging::vStackDump();
  exit(1);
}

/*----------------------------------------------------------------------------*/
/* start the system tasks                                                     */
/*----------------------------------------------------------------------------*/
void Engine::vSpawnJobStart(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Start System Tasks\n*\n"_Stz);
  
  Pthread::vTlsSet(giTLS_ThreadKey,(void*)-1);
  
  JobClass* pJobClass_Start = JobClass::pFind(CN_JOB_START);
  
  Job_Start* pJob_Start
  = static_cast<Job_Start*>(pJobClass_Start->pNewJob(nullptr));
  
  pJob_Start->vSeed(SS_SEED_RUN);
  
  Task* pStartTask = new Task(pJob_Start);
  
  Handlers::vSpawnedTask(pStartTask);
}

/*----------------------------------------------------------------------------*/
/* start a new system                                                         */
/*----------------------------------------------------------------------------*/
void Engine::vStartSystem(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* System Init\n*\n"_Stz);
  
  ASSERT( ! gpAttachMap->m_oRootDic.bHasWords() );
    
  gpAttachMap->vRecordArguments(gxArguments.m_bActionSequence,
                                gxArguments.m_bActivityMap,
                                gxArguments.m_bDeveloper,
                                gxArguments.m_nDummyLoop,
                                gxArguments.m_nDummySleep,
                                gxArguments.m_nDummyThreads,
                                gxArguments.m_iJumboSize,
                                gxArguments.m_nMaxLoadErrors,
                                gxArguments.m_nMaxReadyLP,
                                gxArguments.m_nNodes,
                                gxArguments.m_sRegionName,
                                gxArguments.m_iRegionSize,
                                gxArguments.m_nSlotsPerNode,
                                gxArguments.m_nSubIngesters,
                                gxArguments.m_nSubLoaders,
                                gxArguments.m_nThreads
                                );
  
  //----------------------------------------------------------------------------
  // move
  
  gpAttachMap->m_pQueues = new Queues;
    
  gpAttachMap->m_okDdlLock.vInitLock(LK_DDL);
  gpAttachMap->m_okDicLock.vInitLock(LK_DIC);
  
  gpAttachMap->m_pGrid = new Grid(gxArguments.m_nNodes, gxArguments.m_nSlotsPerNode);
  
  for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
    Node* pNode = new Node(i_Node,
                           gxArguments.m_xNodes[i_Node].m_sNodeName,
                           gxArguments.m_xNodes[i_Node].m_sIPAddress,
                           gxArguments.m_xNodes[i_Node].m_iPortNo);
    ASSERT(CN_NODE == pNode->eClass());
    gpAttachMap->m_pGrid->vAddNode(pNode);
  }
  
  Node* pThisNode = gpAttachMap->m_pGrid->pFindNode(gxArguments.m_sNodeName);
  if (! pThisNode)
    vFatal("Cannot fine Node %s in the cluster"_Stz,
          gxArguments.m_sNodeName.aChar());
  
  i8 iThisNodeNo = pThisNode->iNodeNo();
  gpAttachMap->m_pGrid->vThisNode(iThisNodeNo);

  gpAttachMap->m_ixNextJobId = Job::iAsNodeNo_Bits(iThisNodeNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  gpAttachMap->m_pApiState = new ApiState;
  
  // create the tx sets
  gpAttachMap->m_pTxSet = new TxSet(gpAttachMap->m_pGrid->iThisNode());
  
  
  // initialize the plan number
  gpAttachMap->m_ixPlanId = gpAttachMap->m_pGrid->iThisNode() << 48;

  gpAttachMap->m_pOriginatingJobs   = new JobList(bJobCompare,JL_ORIGINATING);
  gpAttachMap->m_pParticipatingJobs = new JobList(bJobCompare,JL_PARTICIPATING);
  
  //----------------------------------------------------------------------------

  vOpenActivityMap();
  vOpenActionSequence();

  Step::vAddSteps();
  
  gxArguments.vAddConstants();
  
  Axioms::vAddConstants();

  vCallBoot();
  
  //--------------------------------------------------------------------------
  
  
  // get the trace file
  //Word* pTraceFileWord = gpAttachMap->m_oRootDic.pFindWord("trace_file"_Stz);
  //gpTraceFile = reinterpret_cast<File*>(pTraceFileWord->aBodyArea());
  
  //if (gpAttachMap->m_bVerbose)
  //  vTrace("Shm region initialised at: %s\n"_Stz,gpAttachMap->m_sUTC.aChar());
  
  // spawn general threads
  for (i8 iThreads = 0; iThreads < gxArguments.m_nThreads; iThreads++) {
    Thread* pThread = new Thread;
    pThread->vSpawnPthread();
  }
  
  // spawn dummy threads
  for (i8 iThreads = 0; iThreads < gxArguments.m_nDummyThreads; iThreads++) {
    Thread* pThread = new Thread;
    pThread->vSpawnPthread_Dummy();
  }

  while (*gpAttachMap->m_ixThreads != gxArguments.m_nThreads)
    Pthread::vSleepMilli(10,CN_NONE);
  
  vSpawnJobStart();
  
  if (gxArguments.m_bVerbose) {
    vInfo("Engine instance %s started\n"_Stz,gxArguments.m_sRegionName.aChar());
    gpAttachMap->m_pGrid->vDumpPartitioning();
  }
  
}

/*----------------------------------------------------------------------------*/
/* stop the system                                                            */
/*----------------------------------------------------------------------------*/
void Engine::vStopSystem(void)
{
  ASSERT(gpBufferPool);
  
  Region::vUnlink(gxArguments.m_sRegionName);
  
  gpAttachMap->m_iStop = 1;
  
  vTrace("Engine instance %s stopped\n"_Stz,
         gxArguments.m_sRegionName.aChar());
  
  // the system is now stopped
}

/*----------------------------------------------------------------------------*/
/* unlink the system                                                          */
/*----------------------------------------------------------------------------*/
void Engine::vUnlinkSystem(void)
{
  Region::vUnlink(gxArguments.m_sRegionName);
}

/*----------------------------------------------------------------------------*/
/* wait for quit or stop                                                      */
/*----------------------------------------------------------------------------*/
void Engine::vWaitStop(void)
{
  if (gpAttachMap->m_bVerbose)
    vTrace("[[mainThread]] Waiting on stop %s\n"_Stz,"..."_Stz);
  
  // this primary thread waits for flag indicationg systems is to stop
  while ( ! gpAttachMap->m_iQuit && ! gpAttachMap->m_iStop )
    Pthread::vSleepMilli(10,CN_NONE);
}

/*----------------------------------------------------------------------------*/
/* write region name                                                          */
/*----------------------------------------------------------------------------*/
void Engine::vWriteRegionName(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Write Region Name\n*\n"_Stz);
  
  // write the regionName into a se.region file
  // used by shell script to start se prompt
  if (gxArguments.m_bStart) {
    File oFile;
    bool b = oFile.bOpenMode("./se.region"_Stz,C_OPEN_READ_WRITE_CREATE);
    ASSERT(b);
    oFile.vPrint("%s\n"_Stz,gxArguments.m_sRegionName.aChar());
    oFile.vClose();
  }
  
  if (gxArguments.m_bVerbose)
    vInfo("REGION NAME: %s\n"_Stz,gxArguments.m_sRegionName.aChar());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
