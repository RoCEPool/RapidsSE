/*============================================================================*/
/* RapidsSE.cpp - rapids storage engine                                       */
/*============================================================================*/

#include "StorageEngine.h"

extern Arguments gxArguments;

BufferPool*         gpBufferPool       = nullptr;
AttachMap *         gpAttachMap        = nullptr;
Heap*               gpMediumHeap       = nullptr;
Heap*               gpSmallHeap        = nullptr;
GlobalDictionary*   gpGlobalDictionary = nullptr;
Dictionary*         gpDictionary       = nullptr;
Word*               gpWord             = nullptr;

TaskList*           gpTaskList         = nullptr;
ThreadList*         gpThreadList       = nullptr;
SessionList*        gpSessionList      = nullptr;
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
  Sessions::vCheckAssumptions();
  Grid::vCheckAssumptions();
  StorageEngine::vCheckAssumptions();
}

/*----------------------------------------------------------------------------*/
/* execute arguments                                                          */
/*----------------------------------------------------------------------------*/
void RapidsSE::vExecute(void)
{
  try {
    // shm seg should exist
    ASSERT(gpBufferPool);
    
    if (gxArguments.m_bCommand) {
      vSystemInit();
      vRunCommand();
      vWaitFlag();
    }
    else if (gxArguments.m_bDocol) {
      vSystemInit();
      vRunDocol();
      vWaitFlag();
    }
    else if (gxArguments.m_bSeitf) {
      vSeitfPrompt();
    }
    else if (gxArguments.m_bStart) {
      vSystemInit();
      vSystemStart();
      vWaitFlag();
    }
    else if (gxArguments.m_bStop) {
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
    vCreateRegionNew();
    vCreateObjects();
    return;
  }
  else {
    vAttachRegionNew();
    vAttachObjects();
    return;
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attach to an existing shm region                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vAttachRegionNew(void)
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
  i8 iPoolSize = gpAttachMap->m_iPoolSize;
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
  mpRegion = Shm::pMap(iFD,C_ADDR_POOL,iPoolSize,true,xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttachRegion failed to map\n");
    Shm::bClose(iFD,xSysCall);
    exit(EXIT_FAILURE);
  }
  
  // overlay buffer pool onto 1st buffer (buffer #0)
  gpBufferPool = reinterpret_cast<BufferPool*>(mpRegion);
  
  vTrace("Shm %p\n",mpRegion);
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCreateRegionNew(void)
{
  int iFD;
  SysCall xSysCall;
  
  vTrace("Creating shm region %s\n",gxArguments.m_sRegionName.a());
  
  // open a new shm region
  bool bResult = Shm::bCreate(gxArguments.m_sRegionName.a(),
                              gxArguments.m_iPoolSize,
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
                       gxArguments.m_iPoolSize,
                       true,
                       xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vCreateRegion failed to map!\n");
    Shm::bClose(iFD,xSysCall);
    exit(EXIT_FAILURE);
  }
  
  gpBufferPool = new(mpRegion) BufferPool;
  
  vTrace("Shm %p\n",mpRegion);
}

/*----------------------------------------------------------------------------*/
/* drop existing region                                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDropRegionNew(void)
{
  SysCall xSysCall;
  
  bool b = Shm::bUnlink(gxArguments.m_sRegionName.a(), xSysCall);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attach to an existing shm region                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vAttachObjects(void)
{
  // get pointers to global objects from attach map
  gpAttachMap        = reinterpret_cast<AttachMap*>(pcAdd(mpRegion,C_SIZE_LARGE_BUFFER));
  gpMediumHeap       = gpAttachMap->m_pMediumHeap;
  gpSmallHeap        = gpAttachMap->m_pSmallHeap;
  gpGlobalDictionary = gpAttachMap->m_pGlobalDictionary;
  gpDictionary       = gpAttachMap->m_pDictionary;
}

/*----------------------------------------------------------------------------*/
/* attach to an existing shm region                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vAttachRegion(void)
{
  // attach to region
  gpBufferPool = new BufferPool(gxArguments.m_sRegionName);
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
  // create new region
  gpBufferPool = new BufferPool(gxArguments.m_sRegionName,
                                gxArguments.m_iPoolSize,
                                C_SIZE_LARGE_BUFFER);
}

/*----------------------------------------------------------------------------*/
/* intialise the main dictionary                                              */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDicInit(NP_Task npBootTask)
{
  npBootTask->vInitialiseDictionary();
  npBootTask->vPrepareBootSen();
  npBootTask->vLoop();
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
  // prepare to run command using the docol interpreter
  gnpRootTask->vPrepareCommand(gxArguments.m_sCommand.a());
  
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
  // prepare to run docol interpreter
  gnpRootTask->vPrepareDocol();
  
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
  s32  sPrompt;
  s256 sRequest;
  s256 sResponse;
  
  SEinterface* pSEitf = new SEinterface;
  pSEitf->vInit();
  
  sPrompt = "seitf> ";
  while (true) {
    sRequest  = "";
    sResponse = "<none>";
    if (Prompt::bGetInput(sPrompt,sRequest)) {
      pSEitf->vAppendRequest(sRequest);
      while (! pSEitf->bPopResponse(sResponse))
        ;
      vTrace("%s\n",sResponse.a());
    }
  }
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
  
  vDicInit(gnpRootTask);
  
  gpAttachMap->vInit();
  
  // get the trace file
  Word* pTraceFileWord = gpDictionary->pFind("trace_file");
  gpTraceFile = reinterpret_cast<File*>(pTraceFileWord->pBodyArea());
  vTrace("SE booted\n");
  
  // the root thread is on pstack of root task
  gnpRootThread = gnpRootTask->pRootThread();
}

/*----------------------------------------------------------------------------*/
/* start the system                                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStart(void)
{
  // prepare to run client using docol interpreter
  gnpRootTask->vPrepareCommand("SystemStart");
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  gnpRootThread->vAppend(*gnpRootTask);
  
  // the system is now running
}

/*----------------------------------------------------------------------------*/
/* stop the system                                                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStop(void)
{
  gpAttachMap->m_iStop = 1;
  
  // the system is now stopped
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
  // this primary thread waits for flag indicationg systems is to stop
  while ( ! gpAttachMap->m_iQuit && ! gpAttachMap->m_iStop )
    PosixThread::vNanoSleep(100_KB);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/