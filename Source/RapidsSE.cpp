/*============================================================================*/
/* RapidsSE.cpp -- Rapids Storage Engine                                      */
/*============================================================================*/

#include "StorageEngine.h"

Arguments     gxArguments;

BufferPool*   gpBufferPool   = nullptr;
AttachMap *   gpAttachMap    = nullptr;
Heap*         gpJumboHeap    = nullptr;
Heap*         gpMediumHeap   = nullptr;
Heap*         gpSmallHeap    = nullptr;
Dic*          gpMainDic      = nullptr;
Word*         gpWord         = nullptr;

Grid*         gpGrid         = nullptr;
StepArray*    gpStepArray    = nullptr;
TaskList*     gpTaskList     = nullptr;
PlanList*     gpPlanList     = nullptr;
ThreadList*   gpThreadList   = nullptr;
TxSet*        gpTxSet        = nullptr;
SEitfList*    gpSEitfList    = nullptr;
PartitionScheduler*    gpPartitionScheduler = nullptr;
ServerScheduler*    gpServerScheduler    = nullptr;

File*         gpTraceFile    = nullptr;

NP_Task       gnpBootTask    = nullptr;

void*         mpRegion       = nullptr;

bool          gbVerbose      = false;
bool          gbTracepoint   = false;
bool          gbProgress     = false;
bool          gbMetadata     = false;
bool          gbSHM          = false;

bool          gbIndexSpace   = true;
i8            gnIndexEntries = 100;
i8            gnIndexLevels  = 3;
i8            gnIndexOffsets = gnIndexLevels - 1;

Lock          gxDdlLock;
Lock          gxDicLock;
Locks         gxLocks;

i8            giSchEnqueueTask = 0;
i8            giSchEnqueueTaskWait = 0;
i8            giSchGetTask = 0;
i8            giSchGetTaskWait = 0;

i8            giMemCopyBytes = 0;
i8            giMemMoveBytes = 0;
i8            giMemScribbleBytes = 0;
i8            giMemSetBytes = 0;
i8            giMemZeroBytes = 0;

i8            gxMemCopy[CN_ENUMS];
i8            gxMemMove[CN_ENUMS];
i8            gxMemScribble[CN_ENUMS];
i8            gxMemSet[CN_ENUMS];
i8            gxMemZero[CN_ENUMS];

Atomic        gzPlanId;

BitMap*       gpBitMap;

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
  
  gxDdlLock.vInitLock(LK_DDL);
  gxDicLock.vInitLock(LK_DIC);
  
  //gpBitMap = new BitMap;
  
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
      vStartActors();
      if (gxArguments.m_bVerbose) {
        vInfo("RapidsSE instance %s started\n"_Stz,gxArguments.m_sRegionName.aChar());
        gpGrid->vDumpPartitioning();
      }
      
      //vSkipListTest();
      if (gxArguments.m_bDocol) {
        vRunDocol();
        vWaitFlag();
      }
      else if (gxArguments.m_bPrompt || gxArguments.m_bScript) {
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
  
  if (gbVerbose)
    AttachMap::vDump(CN_ATTACHMAP, gpAttachMap, aSingleTab());
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCreateObjects(void)
{
  // create global objects
  gpAttachMap  = new AttachMap;
  gpJumboHeap  = new Heap(CN_JUMBOHEAP,  C_SIZE_JUMBO_OBJECT);
  gpMediumHeap = new Heap(CN_MEDIUMHEAP, C_SIZE_MEDIUM_OBJECT);
  gpSmallHeap  = new Heap(CN_SMALLHEAP,  C_SIZE_SMALL_OBJECT);
  gpMainDic    = &gpAttachMap->m_xMainDic;
  
  vInitJumboHeap();
}

/*----------------------------------------------------------------------------*/
/* compile the main dictionary                                                */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDicCompile(NP_Task npBootTask)
{
  if (gbVerbose)
    vTrace("[[mainThread]] Initialise Main Dictionary...\n"_Stz);
  
  npBootTask->vInitialiseMain();
  
  if (gbVerbose)
    vTrace("[[mainThread]] DicCompile...\n"_Stz);
  
  npBootTask->vSeedDicCompileSen();
  
  if (gbVerbose)
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
  gpJumboHeap->vFixedHeap(true);
  
  Buffer* pJumbo = nullptr;
  while ( (pJumbo = gpBufferPool->pNextJumbo(pJumbo)) ) {

    pJumbo->vSetMagic();
    pJumbo->vSetClass(CN_FREECHUNK);

    gpJumboHeap->vAddFree((FreeChunk*)pJumbo);
  }
}

/*----------------------------------------------------------------------------*/
/* run command from command line                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vRunCommand(void)
{
  // seed root task to run command using the docol interpreter
  gnpBootTask->vSeedWord(gxArguments.m_sCommand);
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  gpServerScheduler->vEnqueueTask_Mutex(*gnpBootTask);
  
  // the docol interpreter is now running the commandf in root task on root thread
}

/*----------------------------------------------------------------------------*/
/* run the docol interpreter                                                  */
/*----------------------------------------------------------------------------*/
void RapidsSE::vRunDocol(void)
{
  vInfo("\n*\n* Run docol\n*\n"_Stz);
  
  // seed root task to run docol interpreter
  gnpBootTask->vSeedDocol();
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  gpServerScheduler->vEnqueueTask_Mutex(*gnpBootTask);
  
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

  ASSERT( ! gpMainDic->bHasWords() );
  
  // init the tls keys
  TlsKeys::vInit();
  
  new StepArray;
  
  // create the plan list here
  gpPlanList = new PlanList;
  
  // tasklist is new'd here as the exception-to-the-rule
  // so as to be boot the system
  gpTaskList = new TaskList;
  
  gnpBootTask = new Task;
  s32 sName;
  sName = "bootTask"_Stz;
  gnpBootTask->vName(sName);
  gnpBootTask->vActorModel(AC_SERVER);
  
  gpPartitionScheduler = new PartitionScheduler;
  gpServerScheduler = new ServerScheduler;
  
  vDicCompile(gnpBootTask);
  
  // create the tx sets
  gpTxSet = new TxSet(gpGrid->iThisNode());
  
  gpAttachMap->vRecordGlobals(gxArguments.m_sRegionName,
                              gxArguments.m_iRegionSize,
                              gxArguments.m_iJumboSize);
  
  // initialize the plan number
  gzPlanId =  gpGrid->iThisNode() << 48;
  
  // get the trace file
  Word* pTraceFileWord = gpMainDic->pFindWord("trace_file"_Stz);
  gpTraceFile = reinterpret_cast<File*>(pTraceFileWord->aBodyArea());
    
  if (gbVerbose)
    vTrace("Shm region initialised at: %s\n"_Stz,gpAttachMap->m_sUTC.aChar());
}

/*----------------------------------------------------------------------------*/
/* start the actors                                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vStartActors(void)
{
  if (gxArguments.m_bVerbose)
    vInfo("\n*\n* Start Actors\n*\n"_Stz);
  
  // seed root task to start the system
  gnpBootTask->vSeedWord("startActors"_Stz);
  
  if (gbVerbose)
    vTrace("[[mainThread]] RapidsSE::vStartActors launching: task %s %p\n"_Stz,
           gnpBootTask->sName().aChar(),
           *gnpBootTask);
  
  //gpServerScheduler->vEnqueueTask_Mutex(*gnpBootTask);
  
  // the primary thread waits here till root task exits
  //while (TS_EXITED != gnpBootTask->eState())
  //  PosixThread::vSleepMilli(1000);
  
  gnpBootTask->vState(TS_RUNNING);
  gnpBootTask->vTaskLoop();
  // the system is now running
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
  if (gbVerbose)
    vTrace("[[mainThread]] Waiting on stop %s\n"_Stz,"..."_Stz);

  // this primary thread waits for flag indicationg systems is to stop
  while ( ! gpAttachMap->m_iQuit && ! gpAttachMap->m_iStop )
    PosixThread::vSleepMilli(10);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
