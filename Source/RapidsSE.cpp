/*============================================================================*/
/* RapidsSE.cpp -- Rapids Storage Engine                                      */
/*============================================================================*/

#include "StorageEngine.h"
#include "ShmUtility.hpp"

Arguments     gxArguments;

BufferPool*   gpBufferPool   = nullptr;
AttachMap *   gpAttachMap    = nullptr;
File*         gpTraceFile    = nullptr;
extern pthread_key_t giTLS_ThreadKey;

void*         mpRegion       = nullptr;
bool          gbSHM          = false;


/*----------------------------------------------------------------------------*/
/* change current directory                                                   */
/*----------------------------------------------------------------------------*/
void RapidsSE::vChangeDirectory(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Change Working Directory\n*\n"_Stz);
  
  // change working directory to commands dir
  Dir xDir;
  s256 sCurrent;
  bool bResult = xDir.bChangeCurrent(gxArguments.m_sCommands);
  if (xDir.bGetCurrent(sCurrent) && gxArguments.m_bVerbose)
    vInfo("CURRENT WORKING DIRECTORY: %s\n"_Stz,sCurrent.aStr());
}

/*----------------------------------------------------------------------------*/
/* execute system                                                             */
/*----------------------------------------------------------------------------*/
void RapidsSE::vExecuteSystem(void)
{
  //OpSys::vGetInfo();
  // OpSys::vTraceInfo();
  
  if (gxArguments.m_bVerbose)
    vPrintSizes();
  
  vSetSignalHandlers();
  
  StorageEngine::vCheckAllAssumptions();
  
  vWriteRegionName();
  vChangeDirectory();
  vKillPrev();
  
  RapidsSE::vPoolOpen();
  
  if (gxArguments.m_bStart)
    vSetGlobals();
  
  RapidsSE::vCheckPort();
  
  if (gxArguments.m_bPrompt && !gxArguments.m_bStart)
    gpAttachMap->vPrintStartupInfo();
  
  RapidsSE::vExecuteCmdLine();
  
  //gpAttachMap->vPrintAttachMap();
  
  //vSkipListTest();
  
  //  ResultBuffer* pRB = new ResultBuffer;
  //  delete pRB;
  
  vRunPrompt();
  
  RapidsSE::vPoolClose();
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
/* print size of things                                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vPrintSizes(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Sizes\n*\n"_Stz);
  
  /*
   vInfo(
   "sizeof(eCN) %lu; sizeof(bool) %lu; \n"_Stz
   "sizeof(Object) %lu; sizeof(StrVal) %lu; \n"_Stz
   "sizeof(s32) %lu; sizeof(s256) %lu; \n"_Stz
   "sizeof(Value) %lu; sizeof(IntVal) %lu; sizeof(ValWrk) %lu \n"_Stz
   "\n"_Stz,
   sizeof(eCN),sizeof(bool),
   sizeof(Object),sizeof(StrVal),
   sizeof(s32),sizeof(s256),
   sizeof(Value),sizeof(IntVal),sizeof(ValWrk)
   );
   */
  /*
   vTrace("sizeof(short) %lld\n"_Stz,(i8)sizeof(short));
   vTrace("sizeof(int) %lld\n"_Stz,(i8)sizeof(int));
   vTrace("sizeof(long) %lld\n"_Stz,(i8)sizeof(long));
   vTrace("sizeof(long long) %lld\n"_Stz,(i8)sizeof(long long));
   
   vTrace("sizeof(i1) %lld\n"_Stz,(i8)sizeof(i1));
   vTrace("sizeof(i2) %lld\n"_Stz,(i8)sizeof(i2));
   vTrace("sizeof(i3) %lld\n"_Stz,(i8)sizeof(i3));
   vTrace("sizeof(i4) %lld\n"_Stz,(i8)sizeof(i4));
   vTrace("sizeof(i8) %lld\n"_Stz,(i8)sizeof(i8));
   
   vTrace("sizeof(Object) %lld\n"_Stz,(i8)sizeof(Object));
   vTrace("sizeof(Object2) %lld\n"_Stz,(i8)sizeof(Object2));
   vTrace("sizeof(eCN) %lld\n"_Stz,(i8)sizeof(eCN));
   
   vTrace("sizeof(Item) %lld\n"_Stz,(i8)sizeof(Item));
   vTrace("sizeof(Entry) %lld\n"_Stz,(i8)sizeof(Entry));
   
   vTrace("alignof(i3) %lld\n"_Stz,(i8)alignof(i3));
   vTrace("alignof(Entry) %lld\n"_Stz,(i8)alignof(Entry));
   */
  
  vInfo("sizeof(Object) %lu; \n"_Stz, sizeof(Object));
  assert(sizeof(Object) == 8);
  
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
  
  if (gxArguments.m_bWelcome && ! gxArguments.m_bStatus)
    vInfo("RapidsSE v%s (%s %s -- %s) pid %lld\n"_Stz,
          RAPIDSSE_VERSION_STRING,
          __DATE__,
          __TIME__,
          aRelDbg,
          Process::iGetPid()
          );
  
  // print startup information
  if (gxArguments.m_bWelcome && gxArguments.m_bStart)
    gxArguments.vPrintStartupInfo();
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
/* run se prompt                                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vRunPrompt(void)
{
  // optionally execute se> prompt
  if (gxArguments.m_bPrompt || gxArguments.m_bScript) {
    
    if (gxArguments.m_bVerbose)
      vInfo("\n*\n* Run Prompt\n*\n"_Stz);
    
    // if only prompt then cwd to Commands
    // (with --start sen changes cwd)
    //    if (!gxArguments.m_bStart) {
    //      // change working directory to commands dir
    //      bool bResult = xDir.bSetCurrent(gxArguments.m_sCommands);
    //      // ignore error
    
    //     if (xDir.bGetCurrent(sCurrent))
    //       vInfo("dir: %s\n"_Stz,sCurrent.aStr());
    //    }
    
    // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    // start prompt R35
    
    if (gxArguments.m_bPromptR35) {
      PosixThread::vTlsSet(giTLS_ThreadKey,(void*)-1);
      
      i8 iTraceLevel = 0;
      eEC eError = RapidsSE_Prompt(iTraceLevel);
      ASSERT(EC_NONE == eError);
      vTrace("RapidsSE_Prompt() called\n"_Stz);
      
      while (true)
        PosixThread::vSleepSec(1000000000);
    }
    
    // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
    
    return;
  }
  
  if (gxArguments.m_bPromptR35) {
    PosixThread::vTlsSet(giTLS_ThreadKey,(void*)-1);
    
    i8 iTraceLevel = 0;
    eEC eError = RapidsSE_Prompt(iTraceLevel);
    ASSERT(EC_NONE == eError);
    vTrace("RapidsSE_Prompt() called\n"_Stz);
    
    while (true)
      PosixThread::vSleepSec(10000000);
  }
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
  gpAttachMap->m_nSplitRatio   = gxArguments.m_nSplitRatio;
  gpAttachMap->m_nMaxQueue     = gxArguments.m_nMaxQueue;
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
    File xFile;
    xFile.vPath("./se.region"_Stz);
    bool b = xFile.bOpen(C_OPEN_READ_WRITE_CREATE);
    ASSERT(b);
    xFile.vPrint("%s\n"_Stz,gxArguments.m_sRegionName.aChar());
    xFile.vClose();
  }
  
  if (gxArguments.m_bVerbose)
    vInfo("REGION NAME: %s\n"_Stz,gxArguments.m_sRegionName.aChar());
}









/*----------------------------------------------------------------------------*/
/* check and see if the port has other listeneters                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCheckPort(void)
{
  SysCall xSysCall;
  Socket* pSocket;
  i8      iPort;
  
  // nothing to check if not starting SE
  if ( ! gxArguments.m_bStart)
    return;
  
  iPort = gxArguments.m_xNodes[gxArguments.m_iThisNode].m_iPortNo;
  if (gxArguments.m_bVerbose)
    vInfo("Looking for listeners on port %lld\n"_Stz, iPort);
  
  pSocket = new Socket;
  bool bResult = pSocket->bCreateServer((int)iPort, xSysCall);
  
  if ( ! bResult ) {
    if (xSysCall.iError() == EADDRINUSE) {
      vError("Address in use (port: %lld). CHECK IF ANOTHER INSTANCE OF RAPIDSSE IS RUNNING!\n"_Stz,
            iPort);
      
    }
    else {
      vError("Unable to listen for connections (port: %lld): Errno %d!\n"_Stz,
             iPort, xSysCall.iError());
    }
    //RapidsSE::vSystemStop();
    exit(EXIT_FAILURE);
  }
  
  pSocket->bClose(xSysCall);
}

/*----------------------------------------------------------------------------*/
/* execute command line                                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vExecuteCmdLine(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Execute Command Line\n*\n"_Stz);
  
//  gpAttachMap->m_xDdlLock.vInitLock(LK_DDL);
//  gpAttachMap->m_xDicLock.vInitLock(LK_DIC);
  
  try {
    if (gxArguments.m_bCommand) {
      ASSERT(gpBufferPool);
      //vSystemInit();
      //vRunCommand();
      //vWaitFlag();
    }
    else if (gxArguments.m_bStart) {
      ASSERT(gpBufferPool);
      vSystemInit();
      if (gxArguments.m_bVerbose) {
        vInfo("RapidsSE instance %s started\n"_Stz,gxArguments.m_sRegionName.aChar());
        gpAttachMap->m_pGrid->vDumpPartitioning();
      }
      
      //vSkipListTest();
      if (gxArguments.m_bDocol) {
        vRunDocol();
        vWaitFlag();
      }
      else if (gxArguments.m_bPrompt
               || gxArguments.m_bPromptR35
               || gxArguments.m_bScript) {
        ASSERT(gpBufferPool);
        return;
      }
      else
        vWaitFlag();
    }
    else if (gxArguments.m_bDocol) {
      ASSERT(gpBufferPool);
      vSystemInit();
      vRunDocol();
      vWaitFlag();
    }
    else if (gxArguments.m_bPrompt || gxArguments.m_bScript) {
      ASSERT(gpBufferPool);
      return;
    }
    else if (gxArguments.m_bScript) {
      ASSERT(gpBufferPool);
      return;
    }
    else if (gxArguments.m_bStatus) {
      vSystemStatus();
    }
    else if (gxArguments.m_bStop) {
      ASSERT(gpBufferPool);
      vSystemStop();
    }
    else if (gxArguments.m_bUnlink) {
      Region::vUnlink(gxArguments.m_sRegionName);
    }
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
/* close the shm region                                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vPoolClose(void)
{
  //delete gpBufferPool;
}

/*----------------------------------------------------------------------------*/
/* attach or create the shm region                                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vPoolOpen(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Pool Open\n*\n"_Stz);

  bool bResult;
  
  if (gxArguments.m_bUnlink) {
    //vUnlink();
    return;
  }
  
  if (gxArguments.m_bStart || gxArguments.m_bDocol) {
        
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
      vInfo("\n*\n* Create Global Objects\n*\n"_Stz);
    vCreateObjects();
    
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
    vAttachObjects();
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
    vAttachObjects();
    return;
  }
  
  bResult = Region::bAttach(gxArguments.m_sRegionName, gxArguments.m_bWelcome);
  ASSERT(bResult);
  vAttachObjects();
  //vFatal("Error: RapidsSE missing arguments\n"_Stz);
  //exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* attach or create the shm region                                            */
/*----------------------------------------------------------------------------*/
bool RapidsSE::bPoolOpen(StrVal& sPoolName)
{
  bool bResult = Region::bAttach(sPoolName, gxArguments.m_bWelcome);
  if (!bResult)
    return bResult;
  
  vAttachObjects();
  return true;
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSEitfDelete(SEitf* pSEitf)
{
  delete pSEitf;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attach to an existing shm region                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vAttachObjects(void)
{
  gpAttachMap = pAdd<AttachMap*>(mpRegion,C_SIZE_POOL_BUFFER);
  
  // get pointers to global objects from attach map
  gpAttachMap->vGetGlobals();
  
  if (gpAttachMap->m_bVerbose)
    AttachMap::vDump(CN_ATTACHMAP, gpAttachMap, aSingleTab());
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCreateObjects(void)
{
  // create the attach map (which records global pointers)
  gpAttachMap = new AttachMap;
  
  // create the carry over (which records global pointers restored on restart)
  gpAttachMap->m_pCarryOver = new CarryOver;
  
  // create the transient heaps
  gpAttachMap->m_pTransientJumboHeap  =
    new Heap(CN_JUMBOTRANSIENT,  C_SIZE_JUMBO_OBJECT);
  gpAttachMap->m_pTransientLargeHeap  =
    new Heap(CN_LARGETRANSIENT,  C_SIZE_LARGE_OBJECT);
  gpAttachMap->m_pTransientMediumHeap =
    new Heap(CN_MEDIUMTRANSIENT, C_SIZE_MEDIUM_OBJECT);
  gpAttachMap->m_pTransientSmallHeap  =
    new Heap(CN_SMALLTRANSIENT,  C_SIZE_SMALL_OBJECT);
  
  // craete the persistent heaps
  gpAttachMap->m_pCarryOver->m_pPersistentJumboHeap  =
    new Heap(CN_JUMBOPERSISTENT,  C_SIZE_JUMBO_OBJECT);
  gpAttachMap->m_pCarryOver->m_pPersistentLargeHeap  =
    new Heap(CN_LARGEPERSISTENT,  C_SIZE_LARGE_OBJECT);
  gpAttachMap->m_pCarryOver->m_pPersistentMediumHeap =
    new Heap(CN_MEDIUMPERSISTENT, C_SIZE_MEDIUM_OBJECT);
  gpAttachMap->m_pCarryOver->m_pPersistentSmallHeap  =
    new Heap(CN_SMALLPERSISTENT,  C_SIZE_SMALL_OBJECT);
  
  vInitJumboHeap();
}

/*----------------------------------------------------------------------------*/
/* compile the main dictionary                                                */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDicCompile(NP_Task npBootTask)
{
  if (gpAttachMap->m_bVerbose)
    vTrace("[[mainThread]] Initialise Main Dictionary...\n"_Stz);
  
  npBootTask->vInitialiseMain();
  
  if (gpAttachMap->m_bVerbose)
    vTrace("[[mainThread]] DicCompile...\n"_Stz);
  
  npBootTask->vSeedDicCompileSen();
  
  if (gpAttachMap->m_bVerbose)
    vTrace("[[mainThread]] RapidsSE::vDicCompile running [%s] %p\n"_Stz,
           npBootTask->sName().aChar(),
           *npBootTask);
  
  npBootTask->vTaskLoop();
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
/* init jumbo heap                                                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vInitJumboHeap(void)
{
  gpAttachMap->m_pTransientJumboHeap->vFixedHeap(true);
  
  Buffer* pJumbo = nullptr;
  while ( (pJumbo = gpBufferPool->pNextJumbo(pJumbo)) ) {

    pJumbo->vSetMagic();
    pJumbo->vSetClass(CN_FREECHUNK);

    gpAttachMap->m_pTransientJumboHeap->vAddFree((FreeChunk*)pJumbo);
  }
  
}

/*----------------------------------------------------------------------------*/
/* run command from command line                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vRunCommand(void)
{
  // seed root task to run command using the docol interpreter
  gpAttachMap->m_npBootTask->vSeedWord(gxArguments.m_sCommand);
 
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  gpAttachMap->m_pServerScheduler->vEnqueueTask_Mutex(*gpAttachMap->m_npBootTask);
  
  // the docol interpreter is now running the commandf in root task on root thread
}

/*----------------------------------------------------------------------------*/
/* run the docol interpreter                                                  */
/*----------------------------------------------------------------------------*/
void RapidsSE::vRunDocol(void)
{
  vInfo("\n*\n* Run docol\n*\n"_Stz);
  
  // seed root task to run docol interpreter
  gpAttachMap->m_npBootTask->vSeedDocol();
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  gpAttachMap->m_pServerScheduler->vEnqueueTask_Mutex(*gpAttachMap->m_npBootTask);
  
  // the docol interpreter is now running in root task on root thread
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
/* init a new system                                                          */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemInit(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* System Init\n*\n"_Stz);

  ASSERT( ! gpAttachMap->m_xMainDic.bHasWords() );
  
  // init the tls keys
  TlsKeys::vInit();
  
  new StepArray;
  
  gpAttachMap->m_pQueues = new Queues;
  
  // create the plan list here
  gpAttachMap->m_pPlanList = new PlanList;
  
  // tasklist is new'd here as the exception-to-the-rule
  // so as to be boot the system
  gpAttachMap->m_pTaskList = new TaskList;
  
  gpAttachMap->m_npBootTask = new Task;
  
  s32 sName;
  sName = "bootTask"_Stz;
  gpAttachMap->m_npBootTask->vName(sName);
  gpAttachMap->m_npBootTask->vActorModel(AC_SERVER);
  
  gpAttachMap->m_xDdlLock.vInitLock(LK_DDL);
  gpAttachMap->m_xDicLock.vInitLock(LK_DIC);
  
  // create grid
  //gpAttachMap->m_pGrid = new Grid(gxArguments.m_nNodes, gxArguments.m_nSlots);
  
  vDicCompile(gpAttachMap->m_npBootTask);
  
  gpAttachMap->m_pOriginatingJobs   = new Jobs(bJobCompare);
  gpAttachMap->m_pParticipatingJobs = new Jobs(bJobCompare);
  
  gpAttachMap->vRecordGlobals(gxArguments.m_sRegionName,
                              gxArguments.m_iRegionSize,
                              gxArguments.m_iJumboSize);
  
  // get the trace file
  Word* pTraceFileWord = gpAttachMap->m_xMainDic.pFindWord("trace_file"_Stz);
  gpTraceFile = reinterpret_cast<File*>(pTraceFileWord->aBodyArea());
    
  if (gpAttachMap->m_bVerbose)
    vTrace("Shm region initialised at: %s\n"_Stz,gpAttachMap->m_sUTC.aChar());
  
  // spawn v3.5 general threads
  for (i8 iThreads = 0; iThreads < gxArguments.m_nThreads; iThreads++) {
    Thread* pThread = new Thread;
    gpAttachMap->m_pThreadList->vAppend(pThread);
    s32 s;
    s = "Thread3.5"_Stz;
    pThread->vName(s);
    pThread->vGeneralThread(true);
    pThread->vSpawn_Thread();
  }
}

/*----------------------------------------------------------------------------*/
/* start the R35 tasks                                                        */
/*----------------------------------------------------------------------------*/
void RapidsSE::vStartR35Tasks(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Start R35 Tasks\n*\n"_Stz);
  
  s32 sName = "startR35Task"_Stz;
  
  Task* pStartTask = new Task;
  pStartTask->vSeedWord("startR35Tasks"_Stz);
  
  pStartTask->vState(TS_READY);
  pStartTask->vR35model(true);
  pStartTask->vActorModel(AC_R35);
  pStartTask->vName(sName);

  Handlers::vSpawnedTaskHandler(pStartTask);
}

/*----------------------------------------------------------------------------*/
/* status of the system                                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStatus(void)
{
  if (Region::bTest(gxArguments.m_sRegionName)) {
    
    // attach to the region
    bool bResult = Region::bAttach(gxArguments.m_sRegionName, gxArguments.m_bWelcome);
    ASSERT(bResult);
    
    // get globals
    vAttachObjects();
    
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
/* stop the system                                                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStop(void)
{
  Region::vUnlink(gxArguments.m_sRegionName);
  
  gpAttachMap->m_iStop = 1;
  
  vTrace("RapidsSE instance %s stopped\n"_Stz,
         gxArguments.m_sRegionName.aChar());
  
  // the system is now stopped
}

/*----------------------------------------------------------------------------*/
/* wait for quit or stop                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vWaitFlag(void)
{
  if (gpAttachMap->m_bVerbose)
    vTrace("[[mainThread]] Waiting on stop %s\n"_Stz,"..."_Stz);

  // this primary thread waits for flag indicationg systems is to stop
  while ( ! gpAttachMap->m_iQuit && ! gpAttachMap->m_iStop )
    PosixThread::vSleepMilli(10);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
