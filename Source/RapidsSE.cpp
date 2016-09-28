/*============================================================================*/
/* RapidsSE.cpp - rapids storage engine                                       */
/*============================================================================*/

#include "StorageEngine.h"

Arguments gxArguments;

BufferPool*         gpBufferPool       = nullptr;
AttachMap *         gpAttachMap        = nullptr;
Heap*               gpJumboHeap        = nullptr;
Heap*               gpMediumHeap       = nullptr;
Heap*               gpSmallHeap        = nullptr;
Dictionary*         gpMainDic          = nullptr;
Word*               gpWord             = nullptr;

Grid*               gpGrid             = nullptr;
StepArray*          gpStepArray        = nullptr;
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


// Object's : Class::vDump
// Object's with no vDump defined : RapidsSE::vDumpUndefined
// Header : RapidsSE::vDumpInvalid
// Objects we don't want to dump (eg Word) : RapidsSE::vDumpDummy

pDUMPFUN *gpDump[]
{
  /*  0 */ &RapidsSE::vDumpInvalid,    // CN_NONE
  
  /*  1 */ &RapidsSE::vDumpInvalid,    // CN_S32
  /*  2 */ &RapidsSE::vDumpInvalid,    // CN_S256
  /*  3 */ &RapidsSE::vDumpInvalid,    // CN_S8K
  /*  4 */ &RapidsSE::vDumpInvalid,    // CN_INTVAL
  /*  5 */ &RapidsSE::vDumpInvalid,    // CN_OBJVAL
  /*  6 */ &RapidsSE::vDumpInvalid,    // CN_PTRVAL
  /*  7 */ &RapidsSE::vDumpInvalid,    // CN_STRVAL
  /*  8 */ &RapidsSE::vDumpInvalid,    // CN_VALREG
  /*  9 */ &RapidsSE::vDumpInvalid,    // CN_VALSET
  /* 10 */ &RapidsSE::vDumpInvalid,    // CN_VALWSP
  
  /* 11 */ &RapidsSE::vDumpInvalid,    // CN_ARGUMENTS
  /* 12 */ &RapidsSE::vDumpInvalid,    // CN_ATOMIC
  /* 13 */ &AttachMap::vDump,          // CN_ATTACHMAP
  
  /* 14 */ &RapidsSE::vDumpDummy,      // CN_BUFFER
  /* 15 */ &RapidsSE::vDumpInvalid,    // CN_BUFFERPOOL
  /* 16 */ &RapidsSE::vDumpInvalid,    // CN_BUFFERPOOLANALYZER

  /* 17 */ &Dictionary::vDump,         // CN_DICTIONARY
  /* 18 */ &DictionaryStack::vDump,    // CN_DICTIONARYSTACK
  /* 19 */ &RapidsSE::vDumpInvalid,    // CN_DIR
  
  /* 20 */ &RapidsSE::vDumpInvalid,    // CN_FILE
  /* 21 */ &RapidsSE::vDumpDummy,      // CN_FREECHUNK
  
  /* 22 */ &Grid::vDump,               // CN_GRID

  /* 23 */ &HeapBuffer::vDump,         // CN_HEAPBUFFER
  
  /* 24 */ &Index::vDump,              // CN_INDEX
  
  /* 25 */ &RapidsSE::vDumpInvalid,    // CN_JUMBOHEAP

  /*    */ &KeyCpp::vDump,             // CN_KEYCPP
  /*    */ &KeyVal::vDump,             // CN_KEYVAL
  
  /* 26 */ &Loader::vDump,             // CN_LOADER
  /* 27 */ &LoaderSub::vDump,          // CN_LOADERSUB
  /* 28 */ &RapidsSE::vDumpInvalid,    // CN_LOCK
  
  /* 29 */ &Heap::vDump,               // CN_MEDIUMHEAP
  
  /* 30 */ &Node::vDump,               // CN_NODE
  /* 31 */ &NodeCursor::vDump,         // CN_NODECURSOR
  
  /* 32 */ &RapidsSE::vDumpDummy,      // CN_PARSER
  /* 33 */ &Partition::vDump,          // CN_PARTITION
  /* 34 */ &RapidsSE::vDumpUndefined,  // CN_PARTITIONS
  /* 35 */ &RapidsSE::vDumpUndefined,  // CN_PROXY
  /* 36 */ &RapidsSE::vDumpUndefined,  // CN_PROXYSET

  /* 37 */ &Queue::vDump,              // CN_QUEUE
  /* 38 */ &QueueBuffer::vDump,        // CN_QUEUEBUFFER
  
  /*    */ &RegFrame::vDump,           // CN_REGFRAME
  /* 39 */ &RapidsSE::vDumpUndefined,  // CN_RING
  /* 40 */ &RapidsSE::vDumpUndefined,  // CN_ROW
  /* 41 */ &RowBuffer::vDump,          // CN_ROWBUFFER
  /* 42 */ &RapidsSE::vDumpUndefined,  // CN_ROWCURSOR
  /* 43 */ &RowStrand::vDump,          // CN_ROWSTRAND
  /* 44 */ &RapidsSE::vDumpUndefined,  // CN_ROWWSP
  
  /* 45 */ &RapidsSE::vDumpUndefined,  // CN_SCANBUFFER
  /* 46 */ &RapidsSE::vDumpUndefined,  // CN_SEHISTORY
  /* 47 */ &SEitf::vDump,              // CN_SEITF
  /* 48 */ &RapidsSE::vDumpUndefined,  // CN_SEITFCURSOR
  /* 49 */ &RapidsSE::vDumpUndefined,  // CN_SEITFLIST
  /* 50 */ &RapidsSE::vDumpDummy,      // CN_SEPROMPT
  /* 51 */ &RapidsSE::vDumpUndefined,  // CN_SINCE
  /* 52 */ &RapidsSE::vDumpUndefined,  // CN_SKIPCURSOR
  /* 53 */ &RapidsSE::vDumpUndefined,  // CN_SKIPLIST
  /*    */ &SkipMap::vDump,            // CN_SKIPMAP
  /* 54 */ &RapidsSE::vDumpUndefined,  // CN_SKIPNODE
  /* 55 */ &Heap::vDump,               // CN_SMALLHEAP
  /* 56 */ &RapidsSE::vDumpUndefined,  // CN_SOCKET
  /* 57 */ &RapidsSE::vDumpUndefined,  // CN_STACK
  /* 58 */ &RapidsSE::vDumpInvalid,    // CN_STEPACCUM
  /* 59 */ &StepArray::vDump,          // CN_STEPARRAY
  /* 60 */ &RapidsSE::vDumpInvalid,    // CN_SYSCALL
  
  /* 61 */ &Table::vDump,              // CN_TABLE
  /* 62 */ &Task::vDump,               // CN_TASK
  /* 63 */ &RapidsSE::vDumpUndefined,  // CN_TASKCURSOR
  /* 64 */ &TaskList::vDump,           // CN_TASKLIST
  /* 65 */ &RapidsSE::vDumpUndefined,  // CN_TASKSET
  /* 66 */ &RapidsSE::vDumpUndefined,  // CN_THREAD
  /* 67 */ &RapidsSE::vDumpUndefined,  // CN_THREADCURSOR
  /* 68 */ &RapidsSE::vDumpUndefined,  // CN_THREADLIST
  /* 69 */ &RapidsSE::vDumpUndefined,  // CN_TIME
  /* 70 */ &RapidsSE::vDumpInvalid,    // CN_TIMER
  /* 71 */ &Tokeniser::vDump,          // CN_TOKENISER
  /* 72 */ &Tx::vDump,                 // CN_TX
  /* 73 */ &TxArray::vDump,            // CN_TXARRAY
  /* 74 */ &RapidsSE::vDumpUndefined,  // CN_TXCOLLECTION
  /* 75 */ &RapidsSE::vDumpUndefined,  // CN_TXCURSOR
  /* 76 */ &RapidsSE::vDumpUndefined,  // CN_TXMAP
  /* 77 */ &RapidsSE::vDumpUndefined,  // CN_TXREF
  
  /* 78 */ &RapidsSE::vDumpDummy,      // CN_WORD

  /* 79 */ &RapidsSE::vDumpEnums       // CN_ENUMS
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
  ASSERT(eClass != CN_NONE);
  ASSERT(pObject != nullptr);
  ASSERT(pTab != nullptr);
  // do nothing
}

/*----------------------------------------------------------------------------*/
/* dummy dump                                                                 */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDumpEnums(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_ENUMS);
  ASSERT(pObject != nullptr);
  ASSERT(pTab != nullptr);
  // do nothing
}

/*----------------------------------------------------------------------------*/
/* dump this attachmap                                                        */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDumpInvalid(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass != CN_ENUMS);
  ASSERT(pObject != nullptr);
  ASSERT(pTab != nullptr);
  vTrace("%sInvalid Class %s in Buffer Pool\n", pTab, pCN[eClass]);
}

/*----------------------------------------------------------------------------*/
/* dump not defined                                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDumpUndefined(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass != CN_NONE);
  ASSERT(pObject != nullptr);
  ASSERT(pTab != nullptr);
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
      //vSystemInit();
      //vRunCommand();
      //vWaitFlag();
    }
    else if (gxArguments.m_bStart) {
      ASSERT(gpBufferPool);
      vSystemInit();
      vStartActors();
      vTrace("RapidsSE instance %s started\n",gxArguments.m_sRegionName.a());
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
    else {
      vTrace("Unknown command line argument\n");
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
  if (gxArguments.m_bStart || gxArguments.m_bDocol) {
    
    if (! gxArguments.m_bRestart && bTestRegion()) {
      vTrace("Error: RapidsSE instance %s already started\n",gxArguments.m_sRegionName.a());
      exit(EXIT_FAILURE);
      return;
    }
    
    vCreateRegion();
    vCreateObjects();
    return;
  }
  else if (gxArguments.m_bStatus) {
  }
  else if (gxArguments.m_bStop) {
    if (! bTestRegion()) {
      vTrace("Error: RapidsSE instance was not started\n",gxArguments.m_sRegionName.a());
      exit(EXIT_FAILURE);
      return;
    }
    vAttachRegion();
    vAttachObjects();
    return;

  }
  else if (gxArguments.m_bPrompt) {
    if (! bTestRegion()) {
      vTrace("Error: RapidsSE instance was not started\n",gxArguments.m_sRegionName.a());
      exit(EXIT_FAILURE);
      return;
    }
    vAttachRegion();
    vAttachObjects();
    return;
    
  }
  else {
    vAttachRegion();
    vAttachObjects();
    //vTrace("Error: RapidsSE missing arguments\n");
    //exit(EXIT_FAILURE);
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
  gpAttachMap = reinterpret_cast<AttachMap*>(pcAdd(mpRegion,C_SIZE_POOL_BUFFER));
  
  // get pointers to global objects from attach map
  gpAttachMap->vGetGlobals();
  
  if (gxArguments.m_bVerbose)
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
  
  if (gxArguments.m_bWelcome)
    vTrace("Attaching to SE instance %s\n",gxArguments.m_sRegionName.a());
  
  // open existing shm region
  bResult = Shm::bAttach(gxArguments.m_sRegionName.a(),iFD,xSysCall);
  if ( ! bResult ) {
    vTrace("RapidsSE::vAttachRegion: failed to open region: %s\n",
           gxArguments.m_sRegionName.a());
    ASSERT(0);
  }
  
  // allocate a temporary extent of memory
  mpRegion = Shm::pMap(iFD,C_ADDR_POOL,10_MB,true,xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttachRegion failed to map\n");
    Shm::bClose(iFD,xSysCall);
    ASSERT(0);
  }
  
  // overlay attach map onto 2nd buffer (buffer #1)
  gpAttachMap = reinterpret_cast<AttachMap*>(pcAdd(mpRegion,C_SIZE_POOL_BUFFER));
  
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
    ASSERT(0);
  }
  
  // allocate the full extent of memory required
  mpRegion = Shm::pMap(iFD,C_ADDR_POOL,iRegionSize,true,xSysCall);
  if ( ! mpRegion )  {
    vTrace("RapidsSE::vAttachRegion failed to map\n");
    Shm::bClose(iFD,xSysCall);
    ASSERT(0);
  }
  
  // overlay buffer pool onto 1st buffer (buffer #0)
  gpBufferPool = reinterpret_cast<BufferPool*>(mpRegion);
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm mapped at %p\n",mpRegion);
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
  gpMainDic = &gpAttachMap->m_xMainDic;
  
  vInitJumboHeap();
}

/*----------------------------------------------------------------------------*/
/* create new shm region                                                      */
/*----------------------------------------------------------------------------*/
void RapidsSE::vCreateRegion(void)
{
  int iFD;
  SysCall xSysCall;
  
  vTrace("Creating RapidsSE instance %s ...\n",gxArguments.m_sRegionName.a());
  
  // open a new shm region
  bool bResult = Shm::bCreate(gxArguments.m_sRegionName.a(),
                              gxArguments.m_iRegionSize,
                              iFD,
                              xSysCall,
                              gxArguments.m_bRestart);
  if ( ! bResult ) {
    vTrace("RapidsSE::vCreateRegion: failed to create region %s\n",
           gxArguments.m_sRegionName.a());
    ASSERT(0);
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
    ASSERT(0);
  }
  
  gpBufferPool = new(mpRegion) BufferPool;
  
  if (gxArguments.m_bVerbose)
    vTrace("Shm mapped at %p\n",mpRegion);
}

/*----------------------------------------------------------------------------*/
/* compile the main dictionary                                                */
/*----------------------------------------------------------------------------*/
void RapidsSE::vDicCompile(NP_Task npRootTask)
{
  npRootTask->vInitialiseDictionary();
  
  npRootTask->vSeedDicCompileSen();
  
  if (gxArguments.m_bVerbose)
    vTrace("[[mainThread]] RapidsSE::vDicCompile running [%s] %p\n",npRootTask->sName().a(),*npRootTask);
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
  
  // test
//  Object* pObject = gpJumboHeap->pAllocObject(CN_NONE);
//  pObject->vSetMagic();
//  gpJumboHeap->vFreeObject(pObject);
  
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
  ASSERT( ! gpMainDic->bHasWords() );
  
  // init the tls keys
  vTlsInit();
  
  new StepArray;
  
  // tasklist is new'd here as the exception-to-the-rule
  // so as to be boot the system
  gpTaskList = new TaskList;
  
  gnpRootTask = new Task;
  s32 sName;
  sName = "rootTask";
  gnpRootTask->vName(sName);
  
  vDicCompile(gnpRootTask);
  
  gpAttachMap->vRecordGlobals();
  
  // get the trace file
  Word* pTraceFileWord = gpMainDic->pFind("trace_file");
  gpTraceFile = reinterpret_cast<File*>(pTraceFileWord->pDataArea());
  
  // the root thread is on pstack of root task
  gnpRootThread = gnpRootTask->pRootThread();
}

/*----------------------------------------------------------------------------*/
/* start the actors                                                           */
/*----------------------------------------------------------------------------*/
void RapidsSE::vStartActors(void)
{
  if (gxArguments.m_bVerbose)
    vTrace("Shm region initialised at: %s\n",gpAttachMap->m_sUTC.a());

  // seed root task to start the system
  gnpRootTask->vSeedWord("startActors");
  
  if (gxArguments.m_bVerbose)
    vTrace("[[mainThread]] RapidsSE::vStartActors launching [%s] %p\n",
         gnpRootTask->sName().a(),*gnpRootTask);
  
  // append root task onto root thread's task set
  // root thread will pop the root task and run it
  gnpRootThread->vAppend(*gnpRootTask);
  
  // the system is now running
  
  while (! gpMainDic->pFind("requestQueue"))
    ;

  PosixThread::vSleepNano(250_MB);
}

/*----------------------------------------------------------------------------*/
/* status of the system                                                       */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStatus(void)
{
  if (bTestRegion())
    vTrace("RapidsSE instance %s running\n",gxArguments.m_sRegionName.a());
  else
    vTrace("RapidsSE instance %s not running\n",gxArguments.m_sRegionName.a());
}

/*----------------------------------------------------------------------------*/
/* stop the system                                                            */
/*----------------------------------------------------------------------------*/
void RapidsSE::vSystemStop(void)
{
  vDropRegion();
  
  gpAttachMap->m_iStop = 1;
  
  vTrace("RapidsSE instance %s stopped\n",gxArguments.m_sRegionName.a());
  
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
  if (gxArguments.m_bVerbose)
    vTrace("[[mainThread]] Waiting on stop %s\n","...");

  // this primary thread waits for flag indicationg systems is to stop
  while ( ! gpAttachMap->m_iQuit && ! gpAttachMap->m_iStop )
    PosixThread::vSleepMilli(10);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/