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
TxCollection* gpTxCollection = nullptr;
TxMap*        gpTxMap        = nullptr;
SEitfList*    gpSEitfList    = nullptr;
Scheduler*    gpScheduler    = nullptr;

File*         gpTraceFile    = nullptr;

BPA*          gpBPA          = nullptr;

NP_Task       gnpRootTask    = nullptr;
Thread*       gnpRootThread  = nullptr;

void*         mpRegion       = nullptr;

bool          gbVerbose      = false;
bool          gbTracepoint   = false;
bool          gbProgress     = false;
bool          gbMetadata     = false;

bool          gbIndexSpace   = true;
i8            gnIndexEntries = 100;
i8            gnIndexLevels  = 3;
i8            gnIndexOffsets = gnIndexLevels - 1;

Lock          gxDdlLock;
Lock          gxDicLock;

Atomic        gzPlanId;

/*----------------------------------------------------------------------------*/
/* execute arguments                                                          */
/*----------------------------------------------------------------------------*/
void RapidsSE::vExecute(void)
{
  gxDdlLock.vInit("gxDdlLock"_Str);
  
  gxDicLock.vInit("gxDicLock"_Str);
  
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
      vTrace("RapidsSE instance %s started\n"_Str,gxArguments.m_sRegionName.s());
      gpGrid->vDumpPartitioning();
      
      //vSkipListTest();
      if (gxArguments.m_bDocol) {
        vRunDocol();
        vWaitFlag();
      }
      else if (gxArguments.m_bPrompt || gxArguments.m_bScript) {
        ASSERT(gpBufferPool);
        return;
      }
      else if (gxArguments.m_bUnitTests) {
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
      vError("Unknown command line argument\n"_Str);
      vFatal("RapidsSE exits\n"_Str);
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
  bool bResult;
  
  if (gxArguments.m_bUnlink) {
    //vUnlink();
    return;
  }
  
  if (gxArguments.m_bStart || gxArguments.m_bDocol) {
        
    if (! gxArguments.m_bRestart && Region::bTest(gxArguments.m_sRegionName)) {
      vFatal("Error: RapidsSE instance %s already started\n"_Str,gxArguments.m_sRegionName.s());
      exit(EXIT_FAILURE);
      return;
    }
    
    Region::vCreate(gxArguments.m_sRegionName,
                    gxArguments.m_iRegionSize,
                    gxArguments.m_nJumboTook,
                    gxArguments.m_bTouch,
                    gxArguments.m_bLock,
                    gxArguments.m_bRestart,
                    gxArguments.m_bDontFree,
                    gxArguments.m_bFreeRB);
    vCreateObjects();
    return;
  }
  
  if (gxArguments.m_bStatus) {
    return;
  }
  
  if (gxArguments.m_bStop) {
    if (! Region::bTest(gxArguments.m_sRegionName)) {
      vFatal("Error: RapidsSE instance %s was not started\n"_Str,gxArguments.m_sRegionName.s());
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
      vFatal("Error: RapidsSE instance %s was not started\n"_Str,gxArguments.m_sRegionName.s());
      exit(EXIT_FAILURE);
      return;
    }
    bResult = Region::bAttach(gxArguments.m_sRegionName, gxArguments.m_bWelcome);
    if ( ! bResult ) {
      vFatal("Error: Unable to attach to RapidsSE instance %s\n"_Str,gxArguments.m_sRegionName.s());
      exit(EXIT_FAILURE);
      return;

    }
    vAttachObjects();
    return;
  }
  
  bResult = Region::bAttach(gxArguments.m_sRegionName, gxArguments.m_bWelcome);
  ASSERT(bResult);
  vAttachObjects();
  //vFatal("Error: RapidsSE missing arguments\n"_Str);
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
void RapidsSE::vDicCompile(NP_Task npRootTask)
{
  if (gbVerbose)
    vTrace("[[mainThread]] DicCompile...\n"_Str);
  
  npRootTask->vInitialiseMain();
  
  npRootTask->vSeedDicCompileSen();
  
  if (gbVerbose)
    vTrace("[[mainThread]] RapidsSE::vDicCompile running [%s] %p\n"_Str,
           npRootTask->sName().s(),
           *npRootTask);
  
  npRootTask->vLoop();
}

/*----------------------------------------------------------------------------*/
/* error handler                                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vErrorHandler(Error* pError)
{
  char wFileName[256];
  
  const char* ps;
  
  ps = StrLib::sLast(pError->aFileName()->s());
  StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
  StrLib::vCopy(wFileName,ps+1);
  vError("%s: line %lld in %s\n"_Str,
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
    pJumbo->vClass(CN_FREECHUNK);

    gpJumboHeap->vAddFree((FreeChunk*)pJumbo);
  }
}

/*----------------------------------------------------------------------------*/
/* run command from command line                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vRunCommand(void)
{
  // seed root task to run command using the docol interpreter
  gnpRootTask->vSeedWord(gxArguments.m_sCommand);
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  //gnpRootThread->vAppend(*gnpRootTask);
  gpScheduler->vEnqueueTask_Mutex(*gnpRootTask);
  
  // the docol interpreter is now running the commandf in root task on root thread
}

/*----------------------------------------------------------------------------*/
/* run the docol interpreter                                                  */
/*----------------------------------------------------------------------------*/
void RapidsSE::vRunDocol(void)
{
  // seed root task to run docol interpreter
  gnpRootTask->vSeedDocol();
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  //gnpRootThread->vAppend(*gnpRootTask);
  gpScheduler->vEnqueueTask_Mutex(*gnpRootTask);
  
  // the docol interpreter is now running in root task on root thread
}

/*----------------------------------------------------------------------------*/
/* signal handler                                                             */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSignalHandler(int iSignal)
{
  // print out all the frames to stderr
  vFatal("Error: signal %d:\n"_Str, iSignal);

  Debugging::vStackDump();
  exit(1);
}

/*----------------------------------------------------------------------------*/
/* init a new system                                                          */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemInit(void)
{
  ASSERT( ! gpMainDic->bHasWords() );
  
  // init the tls keys
  TlsKeys::vInit();
  
  new StepArray;
  
  // create the plan list here
  gpPlanList = new PlanList;
  
  // tasklist is new'd here as the exception-to-the-rule
  // so as to be boot the system
  gpTaskList = new TaskList;
  
  gnpRootTask = new Task;
  s32 sName;
  sName = "rootTask"_Str;
  gnpRootTask->vName(sName);
  
  gpScheduler = new Scheduler;
  
  vDicCompile(gnpRootTask);
  
  gpAttachMap->vRecordGlobals(gxArguments.m_sRegionName,
                              gxArguments.m_iRegionSize,
                              gxArguments.m_iJumboSize);
  
  // initialize the plan number
  gzPlanId =  gpGrid->iThisNode() << 48;
  
  // get the trace file
  Word* pTraceFileWord = gpMainDic->pFindWord("trace_file"_Str);
  gpTraceFile = reinterpret_cast<File*>(pTraceFileWord->aBodyArea());
  
  // the root thread is on dstack of root task
  gnpRootThread = gnpRootTask->pRootThread();
}

/*----------------------------------------------------------------------------*/
/* start the actors                                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vStartActors(void)
{
  if (gbVerbose)
    vTrace("Shm region initialised at: %s\n"_Str,gpAttachMap->m_sUTC.s());

  if (gbVerbose)
    vTrace("Starting actors\n"_Str);

  
  // seed root task to start the system
  gnpRootTask->vSeedWord("startActors"_Str);
  
  if (gbVerbose)
    vTrace("[[mainThread]] RapidsSE::vStartActors launching [%s] %p\n"_Str,
         gnpRootTask->sName().s(),
           *gnpRootTask);
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  //gnpRootThread->vAppend(*gnpRootTask);
  gpScheduler->vEnqueueTask_Mutex(*gnpRootTask);
  
  // the primary thread waits here till root task exits
  while (TS_EXITED != gnpRootTask->eState())
    PosixThread::vSleepMilli(1000);
  
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
      vTrace("RapidsSE instance %s running\n"_Str,
             gxArguments.m_sRegionName.s());
      return;
    }
  }
  
  vTrace("RapidsSE instance %s not running\n"_Str,
         gxArguments.m_sRegionName.s());

}

/*----------------------------------------------------------------------------*/
/* stop the system                                                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStop(void)
{
  Region::vUnlink(gxArguments.m_sRegionName);
  
  gpAttachMap->m_iStop = 1;
  
  vTrace("RapidsSE instance %s stopped\n"_Str,
         gxArguments.m_sRegionName.s());
  
  // the system is now stopped
}

/*----------------------------------------------------------------------------*/
/* wait for quit or stop                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vWaitFlag(void)
{
  if (gbVerbose)
    vTrace("[[mainThread]] Waiting on stop %s\n"_Str,"..."_Str);

  // this primary thread waits for flag indicationg systems is to stop
  while ( ! gpAttachMap->m_iQuit && ! gpAttachMap->m_iStop )
    PosixThread::vSleepMilli(10);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
