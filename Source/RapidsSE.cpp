/*============================================================================*/
/* RapidsSE.cpp -- Rapids Storage Engine                                      */
/*============================================================================*/

#include "StorageEngine.h"
#include "ShmUtility.hpp"

Arguments  gxArguments;
ResourceTracker gxResourceTracker;

BufferPool*   gpBufferPool   = nullptr;
AttachMap *   gpAttachMap    = nullptr;
File*         gpTraceFile    = nullptr;
extern pthread_key_t giTLS_ThreadKey;

void*         mpRegion       = nullptr;
bool          gbSHM          = false;


/*----------------------------------------------------------------------------*/
/* main entry                                                                 */
/*----------------------------------------------------------------------------*/
void RapidsSE::vMainEntry(void)
{
  //OpSys::vGetInfo();
  // OpSys::vTraceInfo();
  
  vPrintWelcome();
  
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
void RapidsSE::vAttachRegion(void)
{
  gpAttachMap = pAdd<AttachMap*>(mpRegion,C_SIZE_POOL_BUFFER);
  
  // get pointers to global objects from attach map
  gpAttachMap->vGetGlobals();
  
  if (gpAttachMap->m_bVerbose)
    AttachMap::vDump(CN_ATTACHMAP, gpAttachMap, aSingleTab());
}

/*----------------------------------------------------------------------------*/
/* call the boot task to compile the main dictionary                          */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCallBoot(void)
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
void RapidsSE::vCallImmmediate(Task* pImmediateTask)
{
  pImmediateTask->vSeedJob();
  
  pImmediateTask->vState(TS_EXECUTING);
  
  pImmediateTask->vTaskLoop();
}

/*----------------------------------------------------------------------------*/
/* call the prompt task                                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCallPrompt(void)
{
  SePrompt* pSePrompt = new SePrompt;
  
  pSePrompt->vPromptLoop();
  
  delete pSePrompt;
}

/*----------------------------------------------------------------------------*/
/* change current directory                                                   */
/*----------------------------------------------------------------------------*/
void RapidsSE::vChangeDirectory(void)
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
void RapidsSE::vCheckPort(void)
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
      vError("Address in use (port: %lld). CHECK IF ANOTHER INSTANCE OF RAPIDSSE IS RUNNING!\n"_Stz,
             iPort);
      
    }
    else {
      vError("Unable to listen for connections (port: %lld): Errno %d!\n"_Stz,
             iPort, oSysCall.iError());
    }
    //RapidsSE::vStopSystem();
    exit(EXIT_FAILURE);
  }
  
  pSocket->bClose(oSysCall);
}

/*----------------------------------------------------------------------------*/
/* close the shm region                                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vClosePool(void)
{
  //delete gpBufferPool;
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCreateAttachMap(void)
{
  // create the attach map (which records global pointers)
  new AttachMap;
}

/*----------------------------------------------------------------------------*/
/* delete task immediate                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDeleteImmmediate(Task** ppTask)
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
void RapidsSE::vDeleteSession(Session* pSession)
{
  delete pSession;
}

/*----------------------------------------------------------------------------*/
/* error handler                                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vErrorHandler(Error* pError)
{
  char wFileName[256];
  
  const char* ps;
  
  ps = StrLib::sLast(pError->aFileName()->aChar());
  StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
  StrLib::vCopy(wFileName,ps+1);
  vError("%s: line %lld in %s\n"_Stz,
         pError->aErrorMsg(),
         pError->iLineNo(),
         wFileName);
  
  Debugging::vStackDump();
}

/*----------------------------------------------------------------------------*/
/* kill previous node instances                                               */
/*----------------------------------------------------------------------------*/
void RapidsSE::vKillPrev(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Kill Previous Occurences\n*\n"_Stz);
  
  if ( ! gxArguments.m_bKillPrev)
    return;
  
  // an assumption is that regions will be /SE_<node>
  
  void* pRegion;
  s256 sRegionName;
  int iFD;
  bool b;
  
  for (i8 i_Index=0; i_Index < gxArguments.m_nNodes; i_Index++) {
    
    sRegionName.vSprint("%s_%s"_Stz,
                        gxArguments.m_sRegionPrefix.aChar(),
                        gxArguments.m_xNodes[i_Index].m_sNodeName.aChar());
    
    if ( ! Region::bTest(sRegionName) )
      continue;
    
    if ( ! (pRegion = Region::pOpen(sRegionName,iFD)) )
      continue;
    
    AttachMap* pAttachMap = pAdd<AttachMap*>(pRegion,C_SIZE_POOL_BUFFER);
    
    if (pAttachMap->m_iPid &&
        pAttachMap->m_iPid != Process::iGetPid() &&
        Process::bTestPid(pAttachMap->m_iPid)) {
      
      b = Process::bKillPid(pAttachMap->m_iPid);
      if ( b )
        vTrace("vKillPrev: process %lld killed\n"_Stz,pAttachMap->m_iPid);
      else
        vTrace("vKillPrev: process %lld not killed\n"_Stz,pAttachMap->m_iPid);
    }
    
    Region::vClose(pRegion,iFD);
    
    Region::vUnlink(sRegionName);
  }
}

/*----------------------------------------------------------------------------*/
/* create or open the ActivityMap                                             */
/*----------------------------------------------------------------------------*/
void RapidsSE::vOpenActivityMap(void)
{
  ActivityMap::vOpenBySE();
}

/*----------------------------------------------------------------------------*/
/* attach or create the shm region                                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vOpenPool(void)
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
      vFatal("Error: RapidsSE instance %s already started\n"_Stz,gxArguments.m_sRegionName.aChar());
      exit(EXIT_FAILURE);
      return;
    }
    
    if (gxArguments.m_bVerbose)
      vInfo("\n*\n* Create Region\n*\n"_Stz);
    
    Region::vCreate(gxArguments.m_sRegionName,
                    gxArguments.m_iRegionSize,
                    gxArguments.m_nJumboTook,
                    gxArguments.m_bTouch,
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
      vFatal("Error: RapidsSE instance %s was not started\n"_Stz,gxArguments.m_sRegionName.aChar());
      exit(EXIT_FAILURE);
      return;
    }
    bResult = Region::bAttach(gxArguments.m_sRegionName, gxArguments.m_bWelcome);
    ASSERT(bResult);
    vAttachRegion();
    return;
  }
  
  if (gxArguments.m_bPrompt || gxArguments.m_bScript) {
    if (! Region::bTest(gxArguments.m_sRegionName)) {
      vFatal("Error: RapidsSE instance %s was not started\n"_Stz,gxArguments.m_sRegionName.aChar());
      exit(EXIT_FAILURE);
      return;
    }
    bResult = Region::bAttach(gxArguments.m_sRegionName, gxArguments.m_bWelcome);
    if ( ! bResult ) {
      vFatal("Error: Unable to attach to RapidsSE instance %s\n"_Stz,gxArguments.m_sRegionName.aChar());
      exit(EXIT_FAILURE);
      return;
      
    }
    vAttachRegion();
    return;
  }
  
  bResult = Region::bAttach(gxArguments.m_sRegionName, gxArguments.m_bWelcome);
  ASSERT(bResult);
  vAttachRegion();
  //vFatal("Error: RapidsSE missing arguments\n"_Stz);
  //exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* attach or create the shm region                                            */
/*----------------------------------------------------------------------------*/
bool RapidsSE::bOpenPool(StrVal& sPoolName)
{
  bool bResult = Region::bAttach(sPoolName, gxArguments.m_bWelcome);
  if (!bResult)
    return bResult;
  
  vAttachRegion();
  return true;
}

/*----------------------------------------------------------------------------*/
/* open trace file                                                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vOpenTraceFile(void)
{
  gpTraceFile = new File;
  
  bool bOpened = gpTraceFile->bOpen("/tmp/se_trace.txt"_Stz, C_OPEN_WRITE);
  RASSERT(bOpened);
  
  if (gxArguments.m_bVerbose)
    vInfo("** trace file opened: /tmp/se_trace.txt\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print exit                                                                 */
/*----------------------------------------------------------------------------*/
void RapidsSE::vPrintExit(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("RapidsSE exits\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print status of the system                                                 */
/*----------------------------------------------------------------------------*/
void RapidsSE::vPrintStatus(void)
{
  if (Region::bTest(gxArguments.m_sRegionName)) {
    
    // attach to the region
    bool bResult = Region::bAttach(gxArguments.m_sRegionName, gxArguments.m_bWelcome);
    ASSERT(bResult);
    
    // get globals
    vAttachRegion();
    
    // check if the Pid in the shm is still alive
    if ( Process::bTestPid(gpAttachMap->m_iPid) ) {
      vTrace("RapidsSE instance %s running\n"_Stz,
             gxArguments.m_sRegionName.aChar());
      return;
    }
  }
  
  vTrace("RapidsSE instance %s not running\n"_Stz,
         gxArguments.m_sRegionName.aChar());
  
}

/*----------------------------------------------------------------------------*/
/* print welcome                                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vPrintWelcome(void)
{
  Stz* aRelDbg;
#ifdef _DEBUG
  aRelDbg = "debug"_Stz;
#else
  aRelDbg = "release"_Stz;
#endif

  gxArguments.m_sWelcome.vSprint("RapidsSE v%s (%s %s -- %s) pid %lld"_Stz,
        RAPIDSSE_VERSION_STRING,
        __DATE__,
        __TIME__,
        aRelDbg,
        Process::iGetPid()
        );

  if (gxArguments.m_bWelcome && ! gxArguments.m_bStatus)
    vInfo("%s\n"_Stz,gxArguments.m_sWelcome.aStz());
  
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
void RapidsSE::vRunCmdLine(void)
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
      vCallPrompt();
    }
    else if (gxArguments.m_bStatus)  vPrintStatus();
    else if (gxArguments.m_bStop)    vStopSystem();
    else if (gxArguments.m_bUnlink)  vUnlinkSystem();
    else if (gxArguments.m_bScript)  { ASSERT(gpBufferPool); vCallPrompt(); }
    else {
      vError("Unknown command line argument\n"_Stz);
      vFatal("RapidsSE exits\n"_Stz);
      exit(EXIT_FAILURE);
    }
    
  }
  
  catch (Error* pError) {
    vErrorHandler(pError);
  }
}

/*----------------------------------------------------------------------------*/
/* run utilities                                                              */
/*----------------------------------------------------------------------------*/
bool RapidsSE::bRunUtilities(int argc, Stz* argv[])
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
void RapidsSE::vSanityCheck(void)
{
  gxArguments.vSanityCheck();
}

/*----------------------------------------------------------------------------*/
/* set globals                                                                */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSetGlobals(void)
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
void RapidsSE::vSignalHandler(int iSignal)
{
  // print out all the frames to stderr
  vFatal("Error: signal %d:\n"_Stz, iSignal);
  
  Debugging::vStackDump();
  exit(1);
}

/*----------------------------------------------------------------------------*/
/* start the system tasks                                                     */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSpawnJobStart(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Start System Tasks\n*\n"_Stz);
  
  PosixThread::vTlsSet(giTLS_ThreadKey,(void*)-1);
  
  JobClass* pJobClass_Start = JobClass::pFind(CN_JOB_START);
  
  Job_Start* pJob_Start
  = static_cast<Job_Start*>(pJobClass_Start->pNewJob(nullptr));
  
  pJob_Start->vSeed(SS_SEED_RUN);
  
  Task* pStartTask = new Task(pJob_Start);
  
  
  Handlers::vSpawnedTaskHandler(pStartTask);
}

/*----------------------------------------------------------------------------*/
/* start a new system                                                         */
/*----------------------------------------------------------------------------*/
void RapidsSE::vStartSystem(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* System Init\n*\n"_Stz);
  
  ASSERT( ! gpAttachMap->m_xMainDic.bHasWords() );
    
  gpAttachMap->m_pQueues = new Queues;
  
  // create the plan list here
  gpAttachMap->m_pPlanList = new PlanList;
  
  gpAttachMap->m_xDdlLock.vInitLock(LK_DDL);
  gpAttachMap->m_xDicLock.vInitLock(LK_DIC);
  
  //--------------------------------------------------------------------------
  
  gpAttachMap->m_pGrid = new Grid(gxArguments.m_nNodes, gxArguments.m_nSlots);
  
  for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
    Node* pNode = new Node(i_Node,
                           gxArguments.m_xNodes[i_Node].m_sNodeName,
                           gxArguments.m_xNodes[i_Node].m_sIPAddress,
                           gxArguments.m_xNodes[i_Node].m_iPortNo);
    ASSERT(CN_NODE == pNode->eClass());
    gpAttachMap->m_pGrid->vAddNode(pNode);
  }
  
  Node* pThisNode = gpAttachMap->m_pGrid->pFindNode(gxArguments.m_sNodeName);
  if ( ! pThisNode ) {
    THROW("Cannot fine Node %s in the cluster"_Stz,
          gxArguments.m_sNodeName.aChar());
  }
  
  i8 iNodeNo = pThisNode->iNodeNo();
  
  gpAttachMap->m_pGrid->vThisNode(iNodeNo);
  
  //--------------------------------------------------------------------------
  
  vOpenActivityMap();
  
  Step::vAddSteps();
  
  Axioms::vAddConstants();

  vCallBoot();
  
  //--------------------------------------------------------------------------
  
  gpAttachMap->m_pOriginatingJobs   = new Jobs(bJobCompare,JL_ORIGINATING);
  gpAttachMap->m_pParticipatingJobs = new Jobs(bJobCompare,JL_PARTICIPATING);
  
  gpAttachMap->vRecordGlobals(gxArguments.m_sRegionName,
                              gxArguments.m_iRegionSize,
                              gxArguments.m_iJumboSize,
                              gxArguments.m_nSubLoaders);
  
  // get the trace file
  //Word* pTraceFileWord = gpAttachMap->m_xMainDic.pFindWord("trace_file"_Stz);
  //gpTraceFile = reinterpret_cast<File*>(pTraceFileWord->aBodyArea());
  
  //if (gpAttachMap->m_bVerbose)
  //  vTrace("Shm region initialised at: %s\n"_Stz,gpAttachMap->m_sUTC.aChar());
  
  // spawn general threads
  for (i8 iThreads = 0; iThreads < gxArguments.m_nThreads; iThreads++) {
    Thread* pThread = new Thread;
    pThread->vSpawnThread();
  }
  
  // spawn dummy threads
  for (i8 iThreads = 0; iThreads < gxArguments.m_nDummyThreads; iThreads++) {
    Thread* pThread = new Thread;
    pThread->vSpawnDummyThread();
  }

  while (*gpAttachMap->m_ziThreads != gxArguments.m_nThreads)
    PosixThread::vSleepMilli(10);
  
  vSpawnJobStart();
  
  if (gxArguments.m_bVerbose) {
    vInfo("RapidsSE instance %s started\n"_Stz,gxArguments.m_sRegionName.aChar());
    gpAttachMap->m_pGrid->vDumpPartitioning();
  }
  
}

/*----------------------------------------------------------------------------*/
/* stop the system                                                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vStopSystem(void)
{
  ASSERT(gpBufferPool);
  
  Region::vUnlink(gxArguments.m_sRegionName);
  
  gpAttachMap->m_iStop = 1;
  
  vTrace("RapidsSE instance %s stopped\n"_Stz,
         gxArguments.m_sRegionName.aChar());
  
  // the system is now stopped
}

/*----------------------------------------------------------------------------*/
/* unlink the system                                                          */
/*----------------------------------------------------------------------------*/
void RapidsSE::vUnlinkSystem(void)
{
  Region::vUnlink(gxArguments.m_sRegionName);
}

/*----------------------------------------------------------------------------*/
/* wait for quit or stop                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vWaitStop(void)
{
  if (gpAttachMap->m_bVerbose)
    vTrace("[[mainThread]] Waiting on stop %s\n"_Stz,"..."_Stz);
  
  // this primary thread waits for flag indicationg systems is to stop
  while ( ! gpAttachMap->m_iQuit && ! gpAttachMap->m_iStop )
    PosixThread::vSleepMilli(10);
}

/*----------------------------------------------------------------------------*/
/* write region name                                                          */
/*----------------------------------------------------------------------------*/
void RapidsSE::vWriteRegionName(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Write Region Name\n*\n"_Stz);
  
  // write the regionName into a se.region file
  // used by shell script to start se prompt
  if (gxArguments.m_bStart) {
    File oFile;
    bool b = oFile.bOpen("./se.region"_Stz,C_OPEN_READ_WRITE_CREATE);
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
