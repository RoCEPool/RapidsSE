/*============================================================================*/
/* Engine.cpp -- Rapids Storage Engine                                        */
/*============================================================================*/

#include "StorageEngine.h"

extern Arguments    gxArguments;

extern AttachMap*   gpAttachMap;
extern BufferPool*  gpBufferPool;
extern i8           giClassSize[];
extern Header*      gaSeShmRegion;
extern File*        gpTraceFile;

extern bool         gbSHM;


/*----------------------------------------------------------------------------*/
/* main entry                                                                 */
/*----------------------------------------------------------------------------*/
void Engine::vMainEntry(void)
{
  //OpSys::vGetInfo();
  // OpSys::vTraceInfo();
  
  vPrintWelcome();
  
  Correctness::vTraceWith();
  
  vSetSignalHandlers();
  
  // init the tls keys
  TlsKeys::vInit();
  
  HwCounters::bInit();
  
  StorageEngine::vCheckAllAssumptions();

  AM::vBootCheck();
  
  vSanityCheck();
  
  vWriteRegionName();
  
  vChangeDirectory();
  
  vKillPrev();
  
  vOpenPool();
  
//  gpAttachMap->m_bMarkResource[RK_SOCKET] = true;
//  gpAttachMap->m_bMarkResource[RK_FILE] = true;
//  gpAttachMap->m_bMarkResource[RK_DIR] = true;

  vOpenTrace();
  
  vCheckPort();
  
  //gpAttachMap->vPrintAttachMap();
  
  vRunCommandLine();
  
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
  
  if (AM::bVerbose())
    AttachMap::vDump(CN_ATTACHMAP, pAttachMap, Stz::szTab(1));
}

/*----------------------------------------------------------------------------*/
/* things to check right at the beginning                                     */
/*----------------------------------------------------------------------------*/
void Engine::vBootCheck(void)
{
  ASSERT( 1 == giClassSize[CN__A__]);
  ASSERT( 2 == giClassSize[CN__B__]);
  ASSERT( 3 == giClassSize[CN__C__]);
  ASSERT( 4 == giClassSize[CN__D__]);
  ASSERT( 5 == giClassSize[CN__E__]);
  ASSERT( 6 == giClassSize[CN__F__]);
  ASSERT( 7 == giClassSize[CN__G__]);
  ASSERT( 8 == giClassSize[CN__H__]);
  ASSERT( 9 == giClassSize[CN__I__]);
  ASSERT(10 == giClassSize[CN__J__]);
  ASSERT(11 == giClassSize[CN__K__]);
  ASSERT(12 == giClassSize[CN__L__]);
  ASSERT(13 == giClassSize[CN__M__]);
  ASSERT(14 == giClassSize[CN__N__]);
  ASSERT(15 == giClassSize[CN__O__]);
  ASSERT(16 == giClassSize[CN__P__]);
  ASSERT(17 == giClassSize[CN__Q__]);
  ASSERT(18 == giClassSize[CN__R__]);
  ASSERT(19 == giClassSize[CN__S__]);
  ASSERT(20 == giClassSize[CN__T__]);
  ASSERT(21 == giClassSize[CN__U__]);
  ASSERT(22 == giClassSize[CN__V__]);
  ASSERT(23 == giClassSize[CN__W__]);
  ASSERT(24 == giClassSize[CN__X__]);
  ASSERT(25 == giClassSize[CN__Y__]);
  ASSERT(26 == giClassSize[CN__Z__]);
}

/*----------------------------------------------------------------------------*/
/* call the boot task to compile the main dictionary                          */
/*----------------------------------------------------------------------------*/
void Engine::vCallBoot(void)
{
  JobClass* pJobClass_Boot = JobClass::pFind(CN_JOB_BOOT);
  
  Job_Boot* pJob_Boot
    = static_cast<Job_Boot*>(pJobClass_Boot->pNewJob(nullptr));
  
  pJob_Boot->pEnameSet()->vAdd("<path>"_Stz,"/"_Stz);
  
  pJob_Boot->vSeed(SS_SEED_RUN);
  
  Task* pBootTask = new Task(pJob_Boot);
  pBootTask->vInitExeContext();

  pBootTask->vAddBaseWords();
  
  if (AM::bVerbose())
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
    vInfo("CURRENT WORKING DIRECTORY: %s\n"_Stz,sCurrent.szStz());
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
/* decode command line from arguments                                         */
/*----------------------------------------------------------------------------*/
eBO Engine::eDecodeCommandLine(void)
{
  if (gxArguments.m_bStart && gxArguments.m_bPrompt)   return BO_START_PROMPT;
  if (gxArguments.m_bStart)                            return BO_START;
  if (gxArguments.m_bPrompt)                           return BO_PROMPT;
  if (gxArguments.m_bStatus)                           return BO_STATUS;
  if (gxArguments.m_bStop)                             return BO_STOP;
  if (gxArguments.m_bUnlink)                           return BO_UNLINK;
  if (gxArguments.m_bSethScript)                       return BO_SETH_PROMPT;
  
  if (gxArguments.m_bAddNode)                          return BO_ADD_NODE;

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
  
  char* ps;
  
  if (pError) {
    ps = StrLib::sLast(pError->aFileName()->azChar());
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
           
  Correctness::vStackDump();
}

/*----------------------------------------------------------------------------*/
/* get copyright                                                              */
/*----------------------------------------------------------------------------*/
void Engine::vGetCopyright(s256& sCopyright)
{
  sCopyright = "Copyright (c) Borrui Data 2019"_Stz;
}

/*----------------------------------------------------------------------------*/
/* get welcome                                                                */
/*----------------------------------------------------------------------------*/
void Engine::vGetWelcome(s256& sWelcome)
{
  Stz* aRelDbg;
#ifdef WITH_DEBUG_VERSION
  aRelDbg = "debug"_Stz;
#else
  aRelDbg = "release"_Stz;
#endif
  
  sWelcome.vSprint("RapidsSE v%s (Compiled %s %s -- %s) pid %lld"_Stz,
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
                        gxArguments.m_sRegionPrefix.azChar(),
                        gxArguments.m_xNodes[i_Node].m_sNodeName.azChar());
    
    if (! Region::bTest(sRegionNm) )
      continue;

    if (gxArguments.m_bDeveloper)
      vTrace("vKillPrev: region %s exists\n"_Stz,sRegionNm.szStz());
    
    if (! (aRegion = Region::aOpen(sRegionNm,gxArguments.m_aRegionAddr,iFD)))
      continue;
    
    AttachMap* pAttachMap = (AttachMap*)aAdd<AttachMap*>((Object*)aRegion,C_SIZE_POOL_BUFFER);
    
    i8 iPid = pAttachMap->iPid();
    if (gxArguments.m_bDeveloper)
      vTrace("vKillPrev: prev pid %lld\n"_Stz,iPid);

    if (iPid &&
        iPid != Process::iGetPid() &&
        Process::bTestPid(iPid)) {
      
      bool bResult = Process::bKillPid(iPid);
      if (gxArguments.m_bDeveloper) {
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
  IndexTree::vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  sPortrait = ""_Stz;
  AM::pSessionArray()->vMonitorPortrait(sPortrait);
  sRapidsPortrait.vConcat(sPortrait.szStz());
  
  if (1 == ++ gpAttachMap->m_inNextMonitor)
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
  
  i8 iUpTime = AM::iElapsedSsec();
  
  s32 sRegionMag;
  MagLib::vByteMag(AM::iRegionSize(),sRegionMag);

  i8 iPercentFree = AM::pBufferPool()->iFreePercent();
  Stz* aNodeName  = AM::pGrid()->pGetNode(gxArguments.m_iThisNode)->szNodeNm();
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
/* create or open the Log                                                     */
/*----------------------------------------------------------------------------*/
void Engine::vOpenLog(void)
{
  bool bStarting = false;
  if (0 == AM::pGrid()->m_iThisNode && gxArguments.m_bStart)
    bStarting = true;
  
  AM::vLogOpenBySE(bStarting);
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
      vTrace("Error: Instance %s already started\n"_Stz,gxArguments.m_sRegionName.azChar());
      Process::vExitFailure();
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
                    gxArguments.m_bFreeCB);
    
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
             gxArguments.m_sRegionName.azChar());
      Process::vExitFailure();
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
      vTrace("Error: Instance %s was not started\n"_Stz,gxArguments.m_sRegionName.azChar());
      Process::vExitFailure();
      return;
    }
    if (gxArguments.m_bVerbose)
      vInfo("\n*\n* Attach Region\n*\n"_Stz);
    bResult = Region::bAttach(gxArguments.m_sRegionName,
                              gxArguments.m_bVerbose,
                              gxArguments.m_aRegionAddr);
    if ( ! bResult ) {
      vTrace("Error: Unable to attach to instance %s\n"_Stz,gxArguments.m_sRegionName.azChar());
      Process::vExitFailure();
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
void Engine::vOpenTrace(void)
{
  gpTraceFile = new File(nullptr);
  
  bool bOpened = gpTraceFile->bOpenWrite(C_SE_TRACE);
  RASSERT(bOpened);
  
  if (gxArguments.m_bVerbose)
    vInfo("** trace file opened: %s\n"_Stz,C_SE_TRACE);
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
    i8 iPid = AM::iPid();
    if ( Process::bTestPid(iPid) ) {
      vTrace("Engine instance %s running\n"_Stz,
             gxArguments.m_sRegionName.azChar());
      return;
    }
  }
  
  vTrace("Engine instance %s not running\n"_Stz,
         gxArguments.m_sRegionName.azChar());
  
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

  if (gxArguments.m_bWelcome && ! gxArguments.m_bStatus) {
    vInfo("%s\n"_Stz,sWelcome.szStz());
    vInfo("%s\n"_Stz,sCopyright.szStz());
  }
  
  if (gxArguments.m_bVerbose) {
#ifdef WITH_MUNGED_SEN
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
void Engine::vRunCommandLine(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Run Command Line\n*\n"_Stz);
  
  eBO eBootOption = eDecodeCommandLine();
  
  try {
    switch (eBootOption) {
        
      case BO_ADD_NODE:     vStartSystem();   vWaitStop();                break;
      case BO_START:        vStartSystem();   vWaitStop();                break;
        
      case BO_PROMPT:       vOpenLog();       vCallPrompt();              break;
      case BO_SETH_PROMPT:  vCallPrompt();                                break;
      case BO_START_PROMPT: vStartSystem();   vCallPrompt();              break;
        
      case BO_STATUS:       vPrintStatus();                               break;
      case BO_STOP:         vStopSystem();                                break;
      case BO_UNLINK:       vStopSystem();                                break;
        
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
//void Engine::vRunCommandLine(void)
//{
//  if (gxArguments.m_bVerbose)
//    vInfo("\n*\n* Run Command Line\n*\n"_Stz);
//
//  eBO eBootOption = eDecodeCommandLine();
//
//  try {
//
//    if (gxArguments.m_bStart) {
//
//      vStartSystem();
//
//      if (gxArguments.m_bPrompt)
//        vCallPrompt();
//      else
//        vWaitStop();
//    }
//
//    else if (gxArguments.m_bPrompt) {
//      vOpenLog();
//      vCallPrompt();
//    }
//    else if (gxArguments.m_bStatus)  vPrintStatus();
//    else if (gxArguments.m_bStop)    vStopSystem();
//    else if (gxArguments.m_bUnlink)  vUnlinkSystem();
//    else if (gxArguments.m_bSethScript)  { ASSERT(gpBufferPool); vCallPrompt(); }
//    else {
//      THROW_FATAL("Unknown command line argument\n"_Stz);
//    }
//
//  }
//
//  catch (Error* pError) {
//    vErrorHandler(pError);
//  }
//}

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
/* signal handler                                                             */
/*----------------------------------------------------------------------------*/
void Engine::vSignalHandler(int iSignal)
{
  // print out all the frames to stderr
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "Error: signal %d:\n"_Stz, iSignal);
}

/*----------------------------------------------------------------------------*/
/* start the system tasks                                                     */
/*----------------------------------------------------------------------------*/
void Engine::vSpawnJobStart(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Start Library Tasks\n*\n"_Stz);
  
  PsxThd::vMarkNonSeThread();
  
  JobClass* pJobClass_Start = JobClass::pFind(CN_JOB_START);
  
  Job_Start* pJob_Start
    = static_cast<Job_Start*>(pJobClass_Start->pNewJob(nullptr));
  
  pJob_Start->pEnameSet()->vAdd("<path>"_Stz,"/"_Stz);

  pJob_Start->vSeed(SS_SEED_RUN);
  
  Api::bSpawnJob(pJob_Start);
}

/*----------------------------------------------------------------------------*/
/* start a new system                                                         */
/*----------------------------------------------------------------------------*/
void Engine::vStartSystem(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Library Init\n*\n"_Stz);
  
  //ASSERT( ! AM::pRootDic()->bHasWords() );
    
  gpAttachMap->vRecordArguments(
                                gxArguments.m_bDeveloper,
                                gxArguments.m_nDummyLoop,
                                gxArguments.m_nDummySleep,
                                gxArguments.m_nDummyThreads,
                                gxArguments.m_nEntries,
                                gxArguments.m_iJumboSize,
                                gxArguments.m_bLog,
                                gxArguments.m_nMaxLoadErrors,
                                gxArguments.m_nMaxReadyLP,
                                gxArguments.m_bMetadata,
                                gxArguments.m_bMunged,
                                gxArguments.m_nNodes,
                                gxArguments.m_bProgress,
                                gxArguments.m_sRegionName,
                                gxArguments.m_iRegionSize,
                                gxArguments.m_nSlotsPerNode,
                                gxArguments.m_nSubIngesters,
                                gxArguments.m_nSubLoaders,
                                gxArguments.m_nThreads,
                                gxArguments.m_bTracepoint,
                                gxArguments.m_bVerbose
                                );
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move
  
  gpAttachMap->m_pFocalObjects = new FocalObjects;
  gpAttachMap->m_pQueues = new Queues;
    
  gpAttachMap->m_pGrid = new Grid(gxArguments.m_nNodes, gxArguments.m_nSlotsPerNode);
  
  Article* pGridArticle = gpAttachMap->m_pGrid->pArticle();
  
  AM::pLexiconArticle()->vAppend(pGridArticle);
  
  for (i8 i_Node = 0; i_Node < AM::pGrid()->nDefinedNodes(); i_Node++) {
    Node* pNode = new Node(i_Node,
                           gxArguments.m_xNodes[i_Node].m_sNodeName,
                           gxArguments.m_xNodes[i_Node].m_sIPAddress,
                           gxArguments.m_xNodes[i_Node].m_iPortNo);
    ASSERT(CN_NODE == pNode->eClass());
    AM::pGrid()->vAddNode(pNode);
    
    Article* pNodeArticle = pNode->pArticle();
    
    pGridArticle->vAppend(pNodeArticle);
  }
  
  Node* pThisNode = AM::pGrid()->pFindNode(gxArguments.m_sNodeName);
  if (! pThisNode)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Cannot fine Node %s in the cluster"_Stz,
           gxArguments.m_sNodeName.azChar());
  
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
  
  gpAttachMap->m_pSessionArray = new SessionArray;
  
  // create the tx sets
  gpAttachMap->m_pTxSet = new TxSet(AM::pGrid()->iThisNode());
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vOpenLog();
  
  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "adding steps"_Stz); //!format-LogSys

  Step::vAddSteps();
  
  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "adding constants"_Stz); //!format-LogSys
  
  gxArguments.vAddConstants();
  
  Constants::vAddConstants();

  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "running boot job"_Stz); //!format-LogSys
  vCallBoot();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // threading
  
  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "spawning threads"_Stz); //!format-LogSys
  // spawn general threads
  for (i8 iThreads = 0; iThreads < gxArguments.m_nThreads; iThreads++) {
    Thread* pThread = new Thread;
    pThread->vSpawnPsxThd_general();
  }
  
  // spawn dummy threads
  for (i8 iThreads = 0; iThreads < gxArguments.m_nDummyThreads; iThreads++) {
    Thread* pThread = new Thread;
    pThread->vSpawnPsxThd_dummy();
  }
  
  // pause till all general threads are operational 
  Scheduler::vPauseTillGeneralThreads();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "running boot job"_Stz); //!format-LogSys
  
  vSpawnJobStart();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  AM::vLogSys(SL_INF,gpAttachMap,YA_SYS_BOOT,
              "instance %s started"_Stz,
              gxArguments.m_sRegionName.azChar()); //!format-LogSys
  
  if (gxArguments.m_bVerbose) {
    vInfo("Engine instance %s started\n"_Stz,gxArguments.m_sRegionName.azChar());
    AM::pGrid()->vDumpPartitioning();
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
         gxArguments.m_sRegionName.azChar());
  
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
  if (AM::bVerbose())
    vTrace("[[mainThread]] Waiting on stop %s\n"_Stz,"..."_Stz);
  
  // this primary thread waits for flag indicationg systems is to stop
  while ( ! gpAttachMap->m_iQuit && ! gpAttachMap->m_iStop )
    AM::vNap(10,CN_NONE);
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
    File oFile(nullptr);
    bool b = oFile.bOpenMode("./se.region"_Stz,C_OPEN_READ_WRITE_CREATE);
    ASSERT(b);
    oFile.vPrint("%s\n"_Stz,gxArguments.m_sRegionName.azChar());
    oFile.vClose();
  }
  
  if (gxArguments.m_bVerbose)
    vInfo("REGION NAME: %s\n"_Stz,gxArguments.m_sRegionName.azChar());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
