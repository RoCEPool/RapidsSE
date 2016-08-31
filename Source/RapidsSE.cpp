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

Grid*               gpGrid             = nullptr;
TaskList*           gpTaskList         = nullptr;
ThreadList*         gpThreadList       = nullptr;
TxCollection*       gpTxCollection     = nullptr;
TxMap*              gpTxMap            = nullptr;
SEitfList*          gpSEitfList        = nullptr;

File*               gpTraceFile        = nullptr;

pthread_key_t       giTLS_ThreadKey    = 0;
pthread_key_t       giTLS_TaskKey      = 0;

BufferPoolAnalyzer* gpBPA              = nullptr;

NP_Task             gnpRootTask        = nullptr;
Thread*             gnpRootThread      = nullptr;

void*               mpRegion           = nullptr;
void* C_ADDR_POOL = (void*)0x204800000;

pDUMPFUN *gpDump[]
{
  /*  0 */ &RapidsSE::vDumpInvalid,    // CN_NONE
  
  /*  1 */ &RapidsSE::vDumpInvalid,    // CN_INTVAL
  /*  2 */ &RapidsSE::vDumpInvalid,    // CN_OBJVAL
  /*  3 */ &RapidsSE::vDumpInvalid,    // CN_PTRVAL
  /*  4 */ &RapidsSE::vDumpInvalid,    // CN_STRVAL
  /*  5 */ &RapidsSE::vDumpInvalid,    // CN_VALSET
  /*  6 */ &RapidsSE::vDumpInvalid,    // CN_VALWSP
  
  /*  7 */ &RapidsSE::vDumpInvalid,    // CN_ARGUMENTS
  /*  8 */ &AttachMap::vDump,
  /*  9 */ &RapidsSE::vDumpDummy,
  /* 10 */ &RapidsSE::vDumpInvalid,    // CN_BUFFERPOOL
  /* 11 */ &RapidsSE::vDumpInvalid,    // CN_BUFFERPOOLANALYZER
  /* 12 */ &RapidsSE::vDumpInvalid,    // CN_BUFFERPOOLITER
  /* 13 */ &Dictionary::vDump,
  /* 14 */ &RapidsSE::vDumpInvalid,    // CN_DIR
  /* 15 */ &RapidsSE::vDumpInvalid,    // CN_FILE
  /* 16 */ &RapidsSE::vDumpDummy,
  /* 17 */ &RapidsSE::vDumpDummy,
  /* 18 */ &RapidsSE::vDumpUndefined,
  /* 19 */ &HeapBuffer::vDump,
  /* 20 */ &RapidsSE::vDumpUndefined,
  /* 21 */ &Loader::vDump,
  /* 22 */ &RapidsSE::vDumpInvalid,    // CN_LOCK
  /* 23 */ &Heap::vDump,
  /* 24 */ &Node::vDump,
  /* 25 */ &RapidsSE::vDumpUndefined,
  /* 26 */ &Partition::vDump,
  /* 27 */ &RapidsSE::vDumpUndefined,
  /* 28 */ &RapidsSE::vDumpUndefined,
  /* 29 */ &RapidsSE::vDumpUndefined,
  /* 30 */ &RapidsSE::vDumpUndefined,
  /* 31 */ &RapidsSE::vDumpUndefined,
  /* 32 */ &RapidsSE::vDumpUndefined,
  /* 33 */ &RapidsSE::vDumpUndefined,
  /* 34 */ &RapidsSE::vDumpUndefined,
  /* 35 */ &RapidsSE::vDumpUndefined,
  /* 36 */ &RapidsSE::vDumpUndefined,
  /* 37 */ &RapidsSE::vDumpUndefined,
  /* 38 */ &RapidsSE::vDumpUndefined,
  /* 39 */ &Heap::vDump,
  /* 40 */ &RapidsSE::vDumpUndefined,
  /* 41 */ &RapidsSE::vDumpUndefined,
  /* 42 */ &RapidsSE::vDumpInvalid,     // CN_SYSCALL
  /* 43 */ &Table::vDump,
  /* 44 */ &Task::vDump,
  /* 45 */ &RapidsSE::vDumpUndefined,
  /* 46 */ &TaskList::vDump,
  /* 47 */ &RapidsSE::vDumpUndefined,
  /* 48 */ &RapidsSE::vDumpUndefined,
  /* 49 */ &RapidsSE::vDumpUndefined,
  /* 50 */ &RapidsSE::vDumpUndefined,
  /* 51 */ &RapidsSE::vDumpInvalid,    // CN_TIMER
  /* 52 */ &RapidsSE::vDumpUndefined,
  /* 53 */ &TxArray::vDump,
  /* 54 */ &RapidsSE::vDumpUndefined,
  /* 55 */ &RapidsSE::vDumpUndefined,
  /* 56 */ &RapidsSE::vDumpUndefined,
  /* 57 */ &RapidsSE::vDumpUndefined,
  /* 58 */ &RapidsSE::vDumpDummy,
  /* 59 */ &RapidsSE::vDumpUndefined
};

/*----------------------------------------------------------------------------*/
/* check assumptions                                                          */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCheckAssumptions(void)
{
  // this is in reverse order of the spaces in the project
  Platform::vCheckAssumptions();
  Magic::vCheckAssumptions();
  Common::vCheckAssumptions();
  Pool::vCheckAssumptions();
  Values::vCheckAssumptions();
  Structures::vCheckAssumptions();
  Dictionaries::vCheckAssumptions();
  DBobjects::vCheckAssumptions();
  Tasks::vCheckAssumptions();
  Threads::vCheckAssumptions();
  Interface::vCheckAssumptions();
  GridModel::vCheckAssumptions();
  StorageEngine::vCheckAssumptions();
}

/*----------------------------------------------------------------------------*/
/* dummy dump                                                                 */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDumpDummy(eCN eClass, Object* pObject, const char* pTab)
{
  // do nothing
}

/*----------------------------------------------------------------------------*/
/* dump this attachmap                                                        */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDumpInvalid(eCN eClass, Object* pObject, const char* pTab)
{
  vTrace("%sInvalid Class %s in Buffer Pool\n", pTab, pCN[eClass]);
}

/*----------------------------------------------------------------------------*/
/* dump not defined                                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDumpUndefined(eCN eClass, Object* pObject, const char* pTab)
{
  vTrace("%sDump function not defined for class %s\n", pTab, pCN[eClass]);
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
      vStartActors();
      if (gxArguments.m_bDocol) {
        vRunDocol();
        vWaitFlag();
      }
      else if (gxArguments.m_bSePrompt) {
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
    else if (gxArguments.m_bSePrompt) {
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
    else {
      vTrace("Unknown command line argument\n");
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
  
  AttachMap::vDump(CN_ATTACHMAP, gpAttachMap, pSingleTab());
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
  vError("%s: line %lld in %s\n",
         pError->pErrorMsg(),
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
/* start the actors                                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vStartActors(void)
{
  vTrace("Shm region initialised at: %s\n",gpAttachMap->m_sUTC.a());

  // seed root task to start the system
  gnpRootTask->vSeedWord("startActors");
  
  vTrace("[[mainThread]] RapidsSE::vStartActors launching [%s] %p\n",
         gnpRootTask->sName().a(),*gnpRootTask);
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  gnpRootThread->vAppend(*gnpRootTask);
  
  // the system is now running
  
  while (! gpDictionary->pFind("requestQueue"))
    ;

  PosixThread::vSleepNano(250_MB);
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