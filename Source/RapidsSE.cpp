/*============================================================================*/
/* RapidsSE.cpp - rapids storage engine                                       */
/*============================================================================*/

#include "StorageEngine.h"

Arguments gxArguments;

BufferPool*         gpBufferPool       = nullptr;
AttachMap *         gpAttachMap        = nullptr;
Heap*               gpMediumHeap       = nullptr;
Heap*               gpSmallHeap        = nullptr;
GlobalDictionary*   gpGlobalDictionary = nullptr;
Dictionary*         gpDictionary       = nullptr;
Word*               gpWord             = nullptr;

TaskList*           gpTaskList         = nullptr;
ThreadList*         gpThreadList       = nullptr;
SEitfList*          gpSEitfList        = nullptr;
TxList*             gpTxList           = nullptr;

File*               gpTraceFile        = nullptr;

pthread_key_t       giTLS_ThreadKey    = 0;
pthread_key_t       giTLS_TaskKey      = 0;

BufferPoolAnalyzer* gpBPA              = nullptr;

NP_Task             gnpRootTask        = nullptr;
Thread*             gnpRootThread      = nullptr;

void*               mpRegion           = nullptr;
void* C_ADDR_POOL = (void*)0x204800000;

/*----------------------------------------------------------------------------*/
/* check assumptions                                                          */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCheckAssumptions(void)
{
  // this is in reverse order of the spaces in the project
  Platform::vCheckAssumptions();
  Types::vCheckAssumptions();
  Common::vCheckAssumptions();
  MemoryManagement::vCheckAssumptions();
  StackValues::vCheckAssumptions();
  DataStructures::vCheckAssumptions();
  Dictionaries::vCheckAssumptions();
  DBobjects::vCheckAssumptions();
  Tasking::vCheckAssumptions();
  Threading::vCheckAssumptions();
  Interface::vCheckAssumptions();
  Grid::vCheckAssumptions();
  StorageEngine::vCheckAssumptions();
}

/*----------------------------------------------------------------------------*/
/* execute arguments                                                          */
/*----------------------------------------------------------------------------*/
void RapidsSE::vExecute(void)
{
  try {
    
    if (gxArguments.m_bCommand) {
      ASSERT(gpBufferPool);
      vSystemInit();
      vRunCommand();
      vWaitFlag();
    }
    else if (gxArguments.m_bStart) {
      ASSERT(gpBufferPool);
      vSystemInit();
      vSystemStart();
      if (gxArguments.m_bSeitf) {
        while (! gpDictionary->pFind("requestQueue"))
          ;
        PosixThread::vSleepNano(250_MB);
        vSeitfPrompt();
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
    else if (gxArguments.m_bSeitf) {
      ASSERT(gpBufferPool);
      vSeitfPrompt();
    }
    else if (gxArguments.m_bStatus) {
      vSystemStatus();
    }
    else if (gxArguments.m_bStop) {
      ASSERT(gpBufferPool);
      vSystemStop();
    }
    else {
      vTrace("Unknown command\n");
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
  if (gxArguments.m_bStart || gxArguments.m_bDocol) {
    vCreateRegion();
    vCreateObjects();
    return;
  }
  else if (gxArguments.m_bStatus) {
  }
  else {
    vAttachRegion();
    vAttachObjects();
    return;
  }
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSEitfDelete(SEitf* pSEitf)
{
  delete pSEitf;
}

/*----------------------------------------------------------------------------*/
/* get a new seitf instance                                                   */
/*----------------------------------------------------------------------------*/
SEitf* RapidsSE::pSEitfNew(void)
{
  SEitf* pSEitf = new SEitf(gxArguments.m_iPartition);
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attach to an existing shm region                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vAttachObjects(void)
{
  gpAttachMap = reinterpret_cast<AttachMap*>(pcAdd(mpRegion,C_SIZE_LARGE_BUFFER));
  
  // get pointers to global objects from attach map
  gpAttachMap->vGetGlobals();
}

/*----------------------------------------------------------------------------*/
/* attach to an existing shm region                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vAttachRegion(void)
{
  int iFD;
  SysCall xSysCall;
  bool bResult;
  
  vTrace("Attaching shm region %s\n",gxArguments.m_sRegionName.a());
  
  // open existing shm region
  bResult = Shm::bAttach(gxArguments.m_sRegionName.a(),iFD,xSysCall);
  if ( ! bResult ) {
    vTrace("RapidsSE::vAttachRegion: failed to open region: %s\n",
           gxArguments.m_sRegionName.a());
    exit(EXIT_FAILURE);
  }
  
  // allocate a temporary extent of memory
  mpRegion = Shm::pMap(iFD,C_ADDR_POOL,10_MB,true,xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttachRegion failed to map\n");
    Shm::bClose(iFD,xSysCall);
    exit(EXIT_FAILURE);
  }
  
  // overlay attach map onto 2nd buffer (buffer #1)
  gpAttachMap = reinterpret_cast<AttachMap*>(pcAdd(mpRegion,C_SIZE_LARGE_BUFFER));
  
  // get the actual size
  i8 iRegionSize = gpAttachMap->m_iRegionSize;
  gpAttachMap = nullptr;
  
  // close the temporary mapping
  bResult = Shm::bClose(iFD,xSysCall);
  bResult = Shm::bUnmap(mpRegion,10_MB,xSysCall);
  
  //--------------------------------------------------------------------------
  
  // reopen the existing shm region
  bResult = Shm::bAttach(gxArguments.m_sRegionName.a(),iFD,xSysCall);
  if ( ! bResult ) {
    vTrace("RapidsSE::vAttachRegion: failed to open region: %s\n",
           gxArguments.m_sRegionName.a());
    exit(EXIT_FAILURE);
  }
  
  // allocate the full extent of memory required
  mpRegion = Shm::pMap(iFD,C_ADDR_POOL,iRegionSize,true,xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttachRegion failed to map\n");
    Shm::bClose(iFD,xSysCall);
    exit(EXIT_FAILURE);
  }
  
  // overlay buffer pool onto 1st buffer (buffer #0)
  gpBufferPool = reinterpret_cast<BufferPool*>(mpRegion);
  
  vTrace("Shm mapped at %p\n",mpRegion);
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCreateObjects(void)
{
  // create global objects
  gpAttachMap        = new AttachMap();
  gpMediumHeap       = new Heap(CN_MEDIUMHEAP,C_SIZE_MEDIUM_OBJECT);
  gpSmallHeap        = new Heap(CN_SMALLHEAP,C_SIZE_SMALL_OBJECT);
  gpGlobalDictionary = new GlobalDictionary;
  gpDictionary       = &gpGlobalDictionary->m_xDictionary;
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCreateRegion(void)
{
  int iFD;
  SysCall xSysCall;
  
  vTrace("Creating shm region %s\n",gxArguments.m_sRegionName.a());
  
  // open a new shm region
  bool bResult = Shm::bCreate(gxArguments.m_sRegionName.a(),
                              gxArguments.m_iRegionSize,
                              iFD,
                              xSysCall);
  if ( ! bResult ) {
    vTrace("RapidsSE::vCreateRegion: failed to create region %s\n",
           gxArguments.m_sRegionName.a());
    exit(EXIT_FAILURE);
  }
  
  // allocate memory for the shm region
  mpRegion = Shm::pMap(iFD,
                       C_ADDR_POOL,
                       gxArguments.m_iRegionSize,
                       true,
                       xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vCreateRegion failed to map!\n");
    Shm::bClose(iFD,xSysCall);
    exit(EXIT_FAILURE);
  }
  
  gpBufferPool = new(mpRegion) BufferPool;
  
  vTrace("Shm mapped at %p\n",mpRegion);
}

/*----------------------------------------------------------------------------*/
/* intialise the main dictionary                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDicInitialise(NP_Task npRootTask)
{
  npRootTask->vInitialiseDictionary();
  
  npRootTask->vSeedDicInitialiseSen();
  
  vTrace("[[mainThread]] RapidsSE::vDicInitialise running [%s] %p\n",npRootTask->sName().a(),*npRootTask);
  npRootTask->vLoop();
}

/*----------------------------------------------------------------------------*/
/* drop existing region                                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDropRegion(void)
{
  SysCall xSysCall;
  
  bool b = Shm::bUnlink(gxArguments.m_sRegionName.a(), xSysCall);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* error handler                                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vHandler(Error* pError)
{
  char wFileName[256];
  
  const char* ps;
  
  ps = String::sLast(pError->pFileName());
  String::bBackward(&ps,pError->pFileName(),'/');
  String::vCopy(wFileName,ps+1);
  vError("%s : line %lld in %s\n",
         pError->pError(),
         pError->iLineNumber(),
         wFileName);
}

/*----------------------------------------------------------------------------*/
/* run command from command line                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vRunCommand(void)
{
  // seed root task to run command using the docol interpreter
  gnpRootTask->vSeedWord(gxArguments.m_sCommand.a());
  
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
/* run seitf>                                                                 */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSeitfPrompt(void)
{
  SEitf* pSEitf = new SEitf(gxArguments.m_iPartition);
  pSEitf->vInit();
  pSEitf->vPrompt();
}

/*----------------------------------------------------------------------------*/
/* init a new system                                                          */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemInit(void)
{
  ASSERT( ! gpDictionary->bHasWords() );
  
  // init the tls keys
  vTlsInit();
  
  // tasklist is new'd here as the exception-to-the-rule
  // so as to be boot the system
  gpTaskList = new TaskList;
  
  gnpRootTask = new Task;
  s32 sName;
  sName = "rootTask";
  gnpRootTask->vName(sName);
  
  vDicInitialise(gnpRootTask);
  
  gpAttachMap->vRecordGlobals();
  
  // get the trace file
  Word* pTraceFileWord = gpDictionary->pFind("trace_file");
  gpTraceFile = reinterpret_cast<File*>(pTraceFileWord->pBodyArea());
  
  // the root thread is on pstack of root task
  gnpRootThread = gnpRootTask->pRootThread();
}

/*----------------------------------------------------------------------------*/
/* start the system                                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStart(void)
{
  vTrace("Shm region initialised at: %s\n",gpAttachMap->m_sUTC.a());

  // seed root task to start the system
  gnpRootTask->vSeedWord("SystemStart");
  
  vTrace("[[mainThread]] RapidsSE::vSystemStart launching [%s] %p\n",
         gnpRootTask->sName().a(),*gnpRootTask);
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  gnpRootThread->vAppend(*gnpRootTask);
  
  // the system is now running
}

/*----------------------------------------------------------------------------*/
/* status of the system                                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStatus(void)
{
  if (bTestRegion())
    vTrace("RapidsSE %s running\n",gxArguments.m_sRegionName.a());
  else
    vTrace("RapidsSE %s not running\n",gxArguments.m_sRegionName.a());
}

/*----------------------------------------------------------------------------*/
/* stop the system                                                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStop(void)
{
  vDropRegion();
  
  gpAttachMap->m_iStop = 1;
  
  // the system is now stopped
}

/*----------------------------------------------------------------------------*/
/* test for region                                                            */
/*----------------------------------------------------------------------------*/
bool RapidsSE::bTestRegion(void)
{
  int iFD;
  SysCall xSysCall;
  
  // open existing shm region
  bool bResult = Shm::bAttach(gxArguments.m_sRegionName.a(),iFD,xSysCall);
  if ( ! bResult )
    return false;
  
  bResult = Shm::bClose(iFD,xSysCall);

  return true;
}

/*----------------------------------------------------------------------------*/
/* initialise tls                                                             */
/*----------------------------------------------------------------------------*/
void RapidsSE::vTlsInit(void)
{
  bool b = PosixThread::bTlsCreate(&giTLS_ThreadKey);
  ASSERT(b);
  
  //ASSERT(!b);
  b = PosixThread::bTlsCreate(&giTLS_TaskKey);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* wait for quit or stop                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vWaitFlag(void)
{
  vTrace("[[mainThread]] Waiting on stop %s\n","...");

  // this primary thread waits for flag indicationg systems is to stop
  while ( ! gpAttachMap->m_iQuit && ! gpAttachMap->m_iStop )
    PosixThread::vSleepMilli(10);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/