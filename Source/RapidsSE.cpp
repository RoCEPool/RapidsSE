/*============================================================================*/
/* RapidsSE.cpp -- Rapids Storage Engine                                      */
/*============================================================================*/

#include "StorageEngine.h"

Arguments gxArguments;

BufferPool*         gpBufferPool       = nullptr;
AttachMap *         gpAttachMap        = nullptr;
Heap*               gpJumboHeap        = nullptr;
Heap*               gpMediumHeap       = nullptr;
Heap*               gpSmallHeap        = nullptr;
Dic*                gpMainDic          = nullptr;
Word*               gpWord             = nullptr;

Grid*               gpGrid             = nullptr;
StepArray*          gpStepArray        = nullptr;
TaskList*           gpTaskList         = nullptr;
PlanList*           gpPlanList         = nullptr;
ThreadList*         gpThreadList       = nullptr;
TxCollection*       gpTxCollection     = nullptr;
TxMap*              gpTxMap            = nullptr;
SEitfList*          gpSEitfList        = nullptr;

File*               gpTraceFile        = nullptr;

BPA*                gpBPA              = nullptr;

NP_Task             gnpRootTask        = nullptr;
Thread*             gnpRootThread      = nullptr;

StepBoard*          gpStepBoard        = nullptr;

void*               mpRegion           = nullptr;

Lock                gxDdlLock;
Lock                gxDicLock;

Atomic              gzPlanId;


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
      vTrace("RapidsSE instance %s started\n"_Str,gxArguments.m_sRegionName.aStr());
      gpGrid->vDumpPartitioning();
      
      //vSkipListTest();
      if (gxArguments.m_bDocol) {
        vRunDocol();
        vWaitFlag();
      }
      else if (gxArguments.m_bPrompt) {
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
    else if (gxArguments.m_bPrompt) {
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
      vTrace("Unknown command line argument\n"_Str);
      printf("RapidsSE exits\n");
      exit(EXIT_FAILURE);

    }
  }
  catch (Error* pError) {
    vHandler(pError);
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
    
#ifdef STEP_BOARD
    gpStepBoard = new StepBoard(16);
#endif
    
    if (! gxArguments.m_bRestart && Region::bTest(gxArguments.m_sRegionName)) {
      vTrace("Error: RapidsSE instance %s already started\n"_Str,gxArguments.m_sRegionName.aStr());
      exit(EXIT_FAILURE);
      return;
    }
    
    Region::vCreate(gxArguments.m_sRegionName );
    vCreateObjects();
    return;
  }
  
  if (gxArguments.m_bStatus) {
    return;
  }
  
  if (gxArguments.m_bStop) {
    if (! Region::bTest(gxArguments.m_sRegionName)) {
      vTrace("Error: RapidsSE instance was not started\n"_Str,gxArguments.m_sRegionName.aStr());
      exit(EXIT_FAILURE);
      return;
    }
    bResult = Region::bAttach(gxArguments.m_sRegionName);
    ASSERT(bResult);
    vAttachObjects();
    return;
  }
  
  if (gxArguments.m_bPrompt) {
    if (! Region::bTest(gxArguments.m_sRegionName)) {
      vTrace("Error: RapidsSE instance was not started\n"_Str,gxArguments.m_sRegionName.aStr());
      exit(EXIT_FAILURE);
      return;
    }
    bResult = Region::bAttach(gxArguments.m_sRegionName);
    ASSERT(bResult);
    vAttachObjects();
    return;
  }
  
  bResult = Region::bAttach(gxArguments.m_sRegionName);
  ASSERT(bResult);
  vAttachObjects();
  //vTrace("Error: RapidsSE missing arguments\n"_Str);
  //exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* attach or create the shm region                                            */
/*----------------------------------------------------------------------------*/
bool RapidsSE::bPoolOpen(StrVal& sPoolName)
{
  bool bResult = Region::bAttach(sPoolName);
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
  
  if (gxArguments.m_bVerbose)
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
  if (gxArguments.m_bVerbose)
    vTrace("[[mainThread]] DicCompile...\n"_Str);
  
  npRootTask->vInitialiseMain();
  
  npRootTask->vSeedDicCompileSen();
  
  if (gxArguments.m_bVerbose)
    vTrace("[[mainThread]] RapidsSE::vDicCompile running [%s] %p\n"_Str,
           npRootTask->sName().aStr(),
           *npRootTask);
  npRootTask->vLoop();
}

/*----------------------------------------------------------------------------*/
/* error handler                                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vHandler(Error* pError)
{
  char wFileName[256];
  
  const char* ps;
  
  ps = StrLib::sLast(pError->aFileName()->a());
  StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
  StrLib::vCopy(wFileName,ps+1);
  vError("%s: line %lld in %s\n"_Str,
         pError->aErrorMsg(),
         pError->iLineNo(),
         wFileName);
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
  gnpRootTask->vSeedWord(gxArguments.m_sCommand.aStr());
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  gnpRootThread->vAppend(*gnpRootTask);
  
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
  gnpRootThread->vAppend(*gnpRootTask);
  
  // the docol interpreter is now running in root task on root thread
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
  
  vDicCompile(gnpRootTask);
  
  gpAttachMap->vRecordGlobals();
  
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
  if (gxArguments.m_bVerbose)
    vTrace("Shm region initialised at: %s\n"_Str,gpAttachMap->m_sUTC.aStr());

  if (gxArguments.m_bVerbose)
    vTrace("Starting actors\n"_Str);

  
  // seed root task to start the system
  gnpRootTask->vSeedWord("startActors");
  
  if (gxArguments.m_bVerbose)
    vTrace("[[mainThread]] RapidsSE::vStartActors launching [%s] %p\n"_Str,
         gnpRootTask->sName().aStr(),
           *gnpRootTask);
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  gnpRootThread->vAppend(*gnpRootTask);
  
  // the primary thread waits here till root task exits
  while (TS_EXITED != gnpRootTask->eState())
    PosixThread::vSleepMilli(1);
  
  // the system is now running
}

/*----------------------------------------------------------------------------*/
/* status of the system                                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStatus(void)
{
  if (Region::bTest(gxArguments.m_sRegionName)) {
    
    // attach to teh region
    bool bResult = Region::bAttach(gxArguments.m_sRegionName);
    ASSERT(bResult);
    
    // get globals
    vAttachObjects();
    
    // check if the Pid in the shm is still alive
    if ( Process::bTestPid(gpAttachMap->m_iPid) ) {
      vTrace("RapidsSE instance %s running\n"_Str,
             gxArguments.m_sRegionName.aStr());
      return;
    }
  }
  
  vTrace("RapidsSE instance %s not running\n"_Str,
         gxArguments.m_sRegionName.aStr());

}

/*----------------------------------------------------------------------------*/
/* stop the system                                                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStop(void)
{
  Region::vUnlink(gxArguments.m_sRegionName);
  
  gpAttachMap->m_iStop = 1;
  
  vTrace("RapidsSE instance %s stopped\n"_Str,
         gxArguments.m_sRegionName.aStr());
  
  // the system is now stopped
}

/*----------------------------------------------------------------------------*/
/* wait for quit or stop                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vWaitFlag(void)
{
  if (gxArguments.m_bVerbose)
    vTrace("[[mainThread]] Waiting on stop %s\n"_Str,"..."_Str);

  // this primary thread waits for flag indicationg systems is to stop
  while ( ! gpAttachMap->m_iQuit && ! gpAttachMap->m_iStop )
    PosixThread::vSleepMilli(10);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
