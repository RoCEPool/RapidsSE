/*============================================================================*/
/* Task.cpp - task                                                            */
/*============================================================================*/

#define USE_INPUTSTREAM

#define USE_LOADER2

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"


extern Arguments       gxArguments;

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Task* Tasks::_pTask(void* pAddr)
{
  Task* pTask = reinterpret_cast<Task*>(pAddr);
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Task* Tasks::_pTask(i8 iAddr)
{
  Task* pTask = reinterpret_cast<Task*>(iAddr);
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Task::operator new(size_t iSize)
{
  RASSERT(iSize);
  ASSERT(gpTaskList);
  
  Task* pTask = _pTask(Heap::pAllocate(CN_TASK));
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Task::operator new(size_t iSize, Task* pTask)
{
  RASSERT(iSize);
  ASSERT(gpTaskList);
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Task::operator delete(void* pTaskAddress)
{
  Task* pTask = reinterpret_cast<Task*>(pTaskAddress);
  
  Heap::vFree(pTask);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Task::Task(void)
: Object(CN_TASK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  ASSERT(gpTaskList);
  
//  // this contructor is invoked only for root task creation
//  if (gnpRootTask != nullptr) {
//    String::vSprint(m_sException.a(),"Root task already exits!");
//    m_sFile = __FILE__;
//    throw (i8)__LINE__;
//  }
  
  vInit("");
    
  _checkInitialised_(CN_TASK);
  
  Task* pTask = this;
  gpTaskList->vAppend(pTask);
//  gpTaskList->vDump();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Task::Task(NNP_Task nnpTask)
: Object(CN_TASK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  ASSERT(*nnpTask);
  ASSERT(gpTaskList);
  
  vInit("");
  
  _checkInitialised_(CN_TASK);
  
  Task* pTask = this;
  gpTaskList->vAppend(pTask);
//  gpTaskList->vDump();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Task::Task(const char* aLambdaInput)
: Object(CN_TASK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  ASSERT(gpTaskList);

  _class_(CN_TASK);
  
  vInit(aLambdaInput);
  
  _checkInitialised_(CN_TASK);
  
  Task* pTask = this;
  gpTaskList->vAppend(pTask);
//  gpTaskList->vDump();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Task::~Task(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Task* pTask = this;
  gpTaskList->vRemove(pTask);
//  gpTaskList->vDump();
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void Task::vInit(const char* aLambda)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  static i8 iTaskNo = 0;
  
  // basic state
  m_xLock.vCheckInit();
  m_iTaskNo     = iTaskNo++;
  m_sName       = "<no task name>";
  m_eState      = TS_NONE;
  m_iTxId       = -1;
  m_npNextInSet = nullptr;
  m_npPrevInSet = nullptr;
  m_iWakeAt     = 0;
  m_pParentTask = nullptr;
  m_zAcks.vCheckInit();
  m_pThread     = nullptr;
  m_pTaskSet    = nullptr;
  
  m_sComment    = "";
  
  m_xDicStack.vPush(gpMainDic);
  
  m_pQueue  = nullptr;
  m_pSocket = nullptr;
  
  //m_xParamStack
  //m_xReturnStack
  //m_xAuxStack
  m_pRegFrame    = nullptr;
  m_pLocalsFrame = nullptr;
  
  IP.m_pCell      = nullptr;
  m_pResetIP   = nullptr;
  m_pBeginIP   = nullptr;
  m_pWord      = nullptr;
  m_bCompiling = false;
  m_iTracing   = 0;
  m_nLocals    = 0;
  m_pCurrentWord = nullptr;
  
  m_sLambda = aLambda;
  if (m_sLambda.iLength())
    String::vConcat(m_sLambda.a()," exit");
  
  m_sToken     = "";
  m_eToken     = TK_NONE;
  m_xTokeniser.vPush();
  m_xTokeniser.vUseString(m_sLambda);
  
  m_pSEitf   = nullptr;
  
  m_iDdlLocked = 0;
}

/*----------------------------------------------------------------------------*/
/* add initial set of words into dictionary                                   */
/*----------------------------------------------------------------------------*/
void Task::vInitialiseDictionary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitives();
  
  vAddConstants();
  
  vAddRoutines();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add constant                                                               */
/*----------------------------------------------------------------------------*/
void Task::vAddConstant(const char* aName, i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  Dic* pDic = m_xDicStack.pTop();
  
  pDic->vAddConstant(aName,iVal);
}

/*----------------------------------------------------------------------------*/
/* add constant                                                               */
/*----------------------------------------------------------------------------*/
void Task::vAddConstant(const char* aName, const char* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Dic* pDic = m_xDicStack.pTop();
  
  pDic->vAddConstant(aName,aValue);
}

/*----------------------------------------------------------------------------*/
/* add primitive word into dictionary                                         */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitive(const char* pName, TaskStep pStep, bool bImmediate)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Dic* pDic = m_xDicStack.pTop();
  
  Word* pNewWord = new(pDic) Word(pName);
  
  vCurrentWord(pNewWord);
  
  pNewWord->vWordType(WT_PRIMITIVE);
  pNewWord->vDoesStep(pStep);
  pNewWord->vImmediate(bImmediate);
}

/*----------------------------------------------------------------------------*/
/* adjust next pointer                                                        */
/*----------------------------------------------------------------------------*/
void Task::vAdjustNext(i8 iBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  IP.m_pCell = reinterpret_cast<Cell*>(pcAdd(IP.m_pCell,iBytes));
}

/*----------------------------------------------------------------------------*/
/* get Object at IP                                                           */
/*----------------------------------------------------------------------------*/
Object* Task::pArgObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Object* pObject = reinterpret_cast<Object*>(IP.m_pCell);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* add constants                                                              */
/*----------------------------------------------------------------------------*/
void Task::vAddConstants(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
#ifdef _DEBUG
  vAddConstant("_DEBUG",1);
#else
  vAddConstant("_DEBUG",(i8)0);
#endif
  
  vAddConstant("version",          RAPIDSSE_VERSION_STRING);
  
  vAddConstant("largebuffersize",  C_SIZE_POOL_BUFFER);
  vAddConstant("mediumbuffersize", C_SIZE_MEDIUM_OBJECT);
  vAddConstant("smallbuffersize",  C_SIZE_SMALL_OBJECT);
  vAddConstant("magic",            MAGIC);
  vAddConstant("args.bgtasks",     gxArguments.m_bBgTasks);
  vAddConstant("args.config",      gxArguments.m_bConfig);
  vAddConstant("args.node",        gxArguments.m_iNode);
  vAddConstant("args.nodes",       gxArguments.m_nNodes);
  vAddConstant("args.partitions",  gxArguments.m_nPartitions);
  vAddConstant("args.port",        gxArguments.m_iPort);
  vAddConstant("args.regionSize",  gxArguments.m_iRegionSize);
  vAddConstant("args.sleep",       gxArguments.m_iSleep);
  vAddConstant("args.tracepoint",  gxArguments.m_bTracepoint);
  vAddConstant("args.verbose",     gxArguments.m_bVerbose);
  
  vAddConstant("true",    (i8)1);
  vAddConstant("false",   (i8)0);

  vAddConstant("C_1KB",   C_1KB);
  vAddConstant("C_1MB",   C_1MB);
  vAddConstant("C_1GB",   C_1GB);
  vAddConstant("C_1TB",   C_1TB);
  vAddConstant("C_1PB",   C_1PB);
  vAddConstant("C_1EB",   C_1EB);
  
  vAddConstant("C_1K",    C_1K);
  vAddConstant("C_1M",    C_1M);
  vAddConstant("C_1Bn",   C_1Bn);
  
  vAddConstant("C_4KB",   C_4KB);
  vAddConstant("C_16KB",  C_16KB);
  vAddConstant("C_64KB",  C_64KB);
  vAddConstant("C_256KB", C_256KB);
  
  /*  0 */ vAddConstant("CN_NONE",                  CN_NONE);

  /*  1 */ vAddConstant("CN_INTVAL",                CN_INTVAL);
  /*  2 */ vAddConstant("CN_OBJVAL",                CN_OBJVAL);
  /*  3 */ vAddConstant("CN_PTRVAL",                CN_PTRVAL);
  /*  4 */ vAddConstant("CN_STRVAL",                CN_STRVAL);
  /*  5 */ vAddConstant("CN_VALSET",                CN_VALSET);
  /*  6 */ vAddConstant("CN_VALWSP",                CN_VALWSP);
  
  /*  7 */ vAddConstant("CN_ARGUMENTS",             CN_ARGUMENTS);
  /*  8 */ vAddConstant("CN_ATTACHMAP",             CN_ATTACHMAP);
  /*  9 */ vAddConstant("CN_BUFFER",                CN_BUFFER);
  /* 10 */ vAddConstant("CN_BUFFERPOOL",            CN_BUFFERPOOL);
  /* 11 */ vAddConstant("CN_BPA",                   CN_BPA);
  /* 13 */ vAddConstant("CN_PARSER",                CN_PARSER);
  /* 14 */ vAddConstant("CN_DIC",                   CN_DIC);
  /* 15 */ vAddConstant("CN_DIR",                   CN_DIR);
  /* 16 */ vAddConstant("CN_FILE",                  CN_FILE);
  /* 17 */ vAddConstant("CN_FREECHUNK",             CN_FREECHUNK);
  /* 18 */ vAddConstant("CN_GRID",                  CN_GRID);
  /* 19 */ vAddConstant("CN_HEAPBUFFER",            CN_HEAPBUFFER);
  /* 20 */ vAddConstant("CN_INDEX",                 CN_INDEX);
  /* 21 */ vAddConstant("CN_LOADER",                CN_LOADER);
  /* 22 */ vAddConstant("CN_LOADERSUB",             CN_LOADERSUB);
  /* 23 */ vAddConstant("CN_LOCK",                  CN_LOCK);
  /* 24 */ vAddConstant("CN_MEDIUMHEAP",            CN_MEDIUMHEAP);
  /* 25 */ vAddConstant("CN_NODE",                  CN_NODE);
  /* 26 */ vAddConstant("CN_NODECURSOR",            CN_NODECURSOR);
  /* 27 */ vAddConstant("CN_PARTITION",             CN_PARTITION);
  /* 28 */ vAddConstant("CN_PROXY",                 CN_PROXY);
  /* 29 */ vAddConstant("CN_PROXYSET",              CN_PROXYSET);
  /* 30 */ vAddConstant("CN_QUEUE",                 CN_QUEUE);
  /* 31 */ vAddConstant("CN_QUEUEBUFFER",           CN_QUEUEBUFFER);
  /* 32 */ vAddConstant("CN_ROW",                   CN_ROW);
  /* 33 */ vAddConstant("CN_ROWBUFFER",             CN_ROWBUFFER);
  /* 34 */ vAddConstant("CN_ROWCURSOR",             CN_ROWCURSOR);
  /* 35 */ vAddConstant("CN_ROWSTRAND",             CN_ROWSTRAND);
  /* 36 */ vAddConstant("CN_ROWWSP",                CN_ROWWSP);
  /* 37 */ vAddConstant("CN_SCANBUFFER",            CN_SCANBUFFER);
  /* 38 */ vAddConstant("CN_SEITF",                 CN_SEITF);
  /* 39 */ vAddConstant("CN_SEITFCURSOR",           CN_SEITFCURSOR);
  /* 40 */ vAddConstant("CN_SEITFLIST",             CN_SEITFLIST);
  /* 41 */ vAddConstant("CN_SMALLHEAP",             CN_SMALLHEAP);
  /* 42 */ vAddConstant("CN_SOCKET",                CN_SOCKET);
  /* 43 */ vAddConstant("CN_STACK",                 CN_STACK);
  /* 44 */ vAddConstant("CN_SYSCALL",               CN_SYSCALL);
  /* 45 */ vAddConstant("CN_TABLE",                 CN_TABLE);
  /* 46 */ vAddConstant("CN_TASK",                  CN_TASK);
  /* 47 */ vAddConstant("CN_TASKCURSOR",            CN_TASKCURSOR);
  /* 48 */ vAddConstant("CN_TASKLIST",              CN_TASKLIST);
  /* 49 */ vAddConstant("CN_TASKSET",               CN_TASKSET);
  /* 50 */ vAddConstant("CN_THREAD",                CN_THREAD);
  /* 51 */ vAddConstant("CN_THREADCURSOR",          CN_THREADCURSOR);
  /* 52 */ vAddConstant("CN_THREADLIST",            CN_THREADLIST);
  /* 53 */ vAddConstant("CN_TIMER",                 CN_TIMER);
  /* 54 */ vAddConstant("CN_TX",                    CN_TX);
  /* 55 */ vAddConstant("CN_TXARRAY",               CN_TXARRAY);
  /* 56 */ vAddConstant("CN_TXCOLLECTION",          CN_TXCOLLECTION);
  /* 57 */ vAddConstant("CN_TXCURSOR",              CN_TXCURSOR);
  /* 58 */ vAddConstant("CN_TXMAP",                 CN_TXMAP);
  /* 59 */ vAddConstant("CN_TXREF",                 CN_TXREF);
  /* 60 */ vAddConstant("CN_WORD",                  CN_WORD);
  /* 61 */ vAddConstant("CN_ENUMS",                 CN_ENUMS);
  
  /*  0 */ vAddConstant("EC_NONE",                  EC_NONE);
  /*  1 */ vAddConstant("EC_ASSERT",                EC_ASSERT);
  /*  2 */ vAddConstant("EC_BEGIN_TX_FAILED",       EC_BEGIN_TX_FAILED);
  /*  3 */ vAddConstant("EC_CATALOG_EXISTS",        EC_CATALOG_EXISTS);

  /*  4 */ vAddConstant("EC_FOLDER_IN_USE",         EC_FOLDER_IN_USE);
  /*  5 */ vAddConstant("EC_GRID_NOT_STARTED",      EC_GRID_NOT_STARTED);
  
  /*  6 */ vAddConstant("EC_INVALID_CLASS",         EC_INVALID_CLASS);
  /*  7 */ vAddConstant("EC_INVALID_COMMAND",       EC_INVALID_COMMAND);
  /*  8 */ vAddConstant("EC_INVALID_FILE",          EC_INVALID_FILE);
  /*  9 */ vAddConstant("EC_INVALID_INTERFACE",     EC_INVALID_INTERFACE);
  /* 10 */ vAddConstant("EC_INVALID_PARAMETER",     EC_INVALID_PARAMETER);
  /* 11 */ vAddConstant("EC_INVALID_PARAMETERS",    EC_INVALID_PARAMETERS);
  /* 12 */ vAddConstant("EC_INVALID_INTERFACE",     EC_INVALID_INTERFACE);
  /* 13 */ vAddConstant("EC_INVALID_SYNTAX",        EC_INVALID_SYNTAX);
  
  /* 14 */ vAddConstant("EC_LOW_BUFFERS",           EC_LOW_BUFFERS);
  
  /* 15 */ vAddConstant("EC_NO_COLUMNS_DIC",        EC_NO_COLUMNS_DIC);
  
  /* 16 */ vAddConstant("EC_NO_SUCH_CATALOG",       EC_NO_SUCH_CATALOG);
  /* 17 */ vAddConstant("EC_NO_SUCH_DIRECTORY",     EC_NO_SUCH_DIRECTORY);
  /* 18 */ vAddConstant("EC_NO_SUCH_FOLDER",        EC_NO_SUCH_FOLDER);
  /* 19 */ vAddConstant("EC_NO_SUCH_SCHEMA",        EC_NO_SUCH_SCHEMA);
  /* 20 */ vAddConstant("EC_NO_SUCH_TABLE",         EC_NO_SUCH_TABLE);
  /* 21 */ vAddConstant("EC_NO_SUCH_TENANT",        EC_NO_SUCH_TENANT);
  /* 22 */ vAddConstant("EC_NO_SUCH_WORD",          EC_NO_SUCH_WORD);
  
  /* 23 */ vAddConstant("EC_NOT_A_CATALOG",         EC_NOT_A_CATALOG);
  /* 24 */ vAddConstant("EC_NOT_A_FOLDER",          EC_NOT_A_FOLDER);
  /* 25 */ vAddConstant("EC_NOT_A_SCHEMA",          EC_NOT_A_SCHEMA);
  /* 26 */ vAddConstant("EC_NOT_A_TABLE",           EC_NOT_A_TABLE);
  /* 27 */ vAddConstant("EC_NOT_A_TENANT",          EC_NOT_A_TENANT);
  
  /* 28 */ vAddConstant("EC_NOT_IN_CATALOGS",       EC_NOT_IN_CATALOGS);
  /* 29 */ vAddConstant("EC_NOT_IN_COLUMNS",        EC_NOT_IN_COLUMNS);
  /* 31 */ vAddConstant("EC_NOT_IN_SCHEMAS",        EC_NOT_IN_SCHEMAS);
  /* 31 */ vAddConstant("EC_NOT_IN_TABLES",         EC_NOT_IN_TABLES);
  /* 32 */ vAddConstant("EC_NOT_IN_TENANTS",        EC_NOT_IN_TENANTS);
  
  /* 33 */ vAddConstant("EC_NULL_INTERFACE",        EC_NULL_INTERFACE);
  /* 34 */ vAddConstant("EC_NULLPTR",               EC_NULLPTR);
  /* 35 */ vAddConstant("EC_RESPONSE_PENDING",      EC_RESPONSE_PENDING);
  /* 36 */ vAddConstant("EC_SCHEMA_EXISTS",         EC_SCHEMA_EXISTS);
  
  /* 37 */ vAddConstant("EC_SEND_FAILURE",          EC_SEND_FAILURE);
  /* 38 */ vAddConstant("EC_SE_NOT_RUNNING",        EC_SE_NOT_RUNNING);
  
  /* 39 */ vAddConstant("EC_TABLE_EXISTS",          EC_TABLE_EXISTS);
  /* 40 */ vAddConstant("EC_TABLE_IN_USE",          EC_TABLE_IN_USE);
  /* 41 */ vAddConstant("EC_TENANT_EXISTS",         EC_TENANT_EXISTS);
  /* 42 */ vAddConstant("EC_VERSION_MISMATCH",      EC_VERSION_MISMATCH);
  /* 43 */ vAddConstant("EC_ENUMS",                 EC_ENUMS);

  /*  0 */ vAddConstant("TK_NONE",                  TK_NONE);
  /*  1 */ vAddConstant("TK_NAME",                  TK_NAME);
  /*  2 */ vAddConstant("TK_INTEGER",               TK_INTEGER);
  /*  3 */ vAddConstant("TK_STRING",                TK_STRING);
  /*  4 */ vAddConstant("TK_ENUMS",                 TK_ENUMS);
  
  /*  1 */ vAddConstant("SIZEOF_INTVAL",            sizeof(IntVal));
  /*  2 */ vAddConstant("SIZEOF_OBJVAL",            sizeof(ObjVal));
  /*  3 */ vAddConstant("SIZEOF_PTRVAL",            sizeof(PtrVal));
  /*  4 */ vAddConstant("SIZEOF_STRVAL",            sizeof(StrVal));
  /*  5 */ vAddConstant("SIZEOF_VALSET",            sizeof(ValWsp)); // correct
  /*  6 */ vAddConstant("SIZEOF_VALWSP",            sizeof(ValWsp)); // correct
  
  /*  7 */ vAddConstant("SIZEOF_ARGUMENTS",         sizeof(Arguments));
  /*  8 */ vAddConstant("SIZEOF_ATOMIC",            sizeof(Atomic));
  /*  9 */ vAddConstant("SIZEOF_ATTACHMAP",         sizeof(AttachMap));
  /* 10 */ vAddConstant("SIZEOF_BUFFER",            sizeof(Buffer));
  /* 11 */ vAddConstant("SIZEOF_BUFFERPOOL",        sizeof(BufferPool));
  /* 12 */ vAddConstant("SIZEOF_BPA",               sizeof(BPA));
  /* 13 */ vAddConstant("SIZEOF_DIC",               sizeof(Dic));
  /* 14 */ vAddConstant("SIZEOF_DICSTACK",          sizeof(DicStack));
  /* 15 */ vAddConstant("SIZEOF_DIR",               sizeof(Dir));
  /* 16 */ vAddConstant("SIZEOF_FILE",              sizeof(File));
  /* 17 */ vAddConstant("SIZEOF_FREECHUNK",         sizeof(FreeChunk));
  /* 18 */ vAddConstant("SIZEOF_GRID",              sizeof(Grid));
  /* 19 */ vAddConstant("SIZEOF_HEAPBUFFER",        sizeof(HeapBuffer));
  /* 20 */ vAddConstant("SIZEOF_INDEX",             sizeof(Index));
  /* 21 */ vAddConstant("SIZEOF_LOADER",            sizeof(Loader));
  /* 22 */ vAddConstant("SIZEOF_LOADERSUB",         sizeof(LoaderSub));
  /* 23 */ vAddConstant("SIZEOF_LOCK",              sizeof(Lock));
  /* 24 */ vAddConstant("SIZEOF_MEDIUMHEAP",        sizeof(Heap));
  /* 25 */ vAddConstant("SIZEOF_NODE",              sizeof(Node));
  /* 26 */ vAddConstant("SIZEOF_NODECURSOR",        sizeof(NodeCursor));
  /* 27 */ vAddConstant("SIZEOF_PARSER",            sizeof(Parser));
  /* 28 */ vAddConstant("SIZEOF_PARTITION",         sizeof(Partition));
  /* 29 */ vAddConstant("SIZEOF_PARTITIONS",        sizeof(Partitions));
  /* 30 */ vAddConstant("SIZEOF_PROXY",             sizeof(Proxy));
  /* 31 */ vAddConstant("SIZEOF_PROXYSET",          sizeof(ProxySet));
  /* 32 */ vAddConstant("SIZEOF_QUEUE",             sizeof(Queue));
  /* 33 */ vAddConstant("SIZEOF_QUEUEBUFFER",       sizeof(QueueBuffer));
  /* 34 */ vAddConstant("SIZEOF_RING",              sizeof(Ring));
  /* 35 */ vAddConstant("SIZEOF_ROW",               sizeof(Row));
  /* 36 */ vAddConstant("SIZEOF_ROWBUFFER",         sizeof(RowBuffer));
  /* 37 */ vAddConstant("SIZEOF_ROWCURSOR",         sizeof(RowCursor));
  /* 38 */ vAddConstant("SIZEOF_ROWSTRAND",         sizeof(RowStrand));
  /* 39 */ vAddConstant("SIZEOF_ROWWSP",            sizeof(RowWsp));
  /* 40 */ vAddConstant("SIZEOF_SCANBUFFER",        sizeof(ScanBuffer));
  /* 41 */ vAddConstant("SIZEOF_SEHISTORY",         sizeof(SeHistory));
  /* 42 */ vAddConstant("SIZEOF_SEITF",             sizeof(SEitf));
  /* 43 */ vAddConstant("SIZEOF_SEITFCURSOR",       sizeof(SEitfCursor));
  /* 44 */ vAddConstant("SIZEOF_SEITFLIST",         sizeof(SEitfList));
  /* 45 */ vAddConstant("SIZEOF_SEPROMPT",          sizeof(SePrompt));
  /* 46 */ vAddConstant("SIZEOF_SINCE",             sizeof(Since));
  /* 47 */ vAddConstant("SIZEOF_SKIPCURSOR",        sizeof(SkipCursor));
  /* 48 */ vAddConstant("SIZEOF_SKIPLIST",          sizeof(SkipList));
  /* 49 */ vAddConstant("SIZEOF_SKIPNODE",          sizeof(SkipNode));
  /* 50 */ vAddConstant("SIZEOF_SMALLHEAP",         sizeof(Heap));
  /* 51 */ vAddConstant("SIZEOF_SOCKET",            sizeof(Socket));
  /* 52 */ vAddConstant("SIZEOF_STACK",             sizeof(Stack));
  /* 53 */ vAddConstant("SIZEOF_STEPACCUM",         sizeof(StepAccum));
  /* 54 */ vAddConstant("SIZEOF_STEPARRAY",         sizeof(StepArray));
  /* 55 */ vAddConstant("SIZEOF_SYSCALL",           sizeof(SysCall));
  /* 56 */ vAddConstant("SIZEOF_TABLE",             sizeof(Table));
  /* 57 */ vAddConstant("SIZEOF_TASK",              sizeof(Task));
  /* 58 */ vAddConstant("SIZEOF_TASKCURSOR",        sizeof(TaskCursor));
  /* 59 */ vAddConstant("SIZEOF_TASKLIST",          sizeof(TaskList));
  /* 60 */ vAddConstant("SIZEOF_TASKSET",           sizeof(TaskSet));
  /* 61 */ vAddConstant("SIZEOF_THREAD",            sizeof(Thread));
  /* 62 */ vAddConstant("SIZEOF_THREADCURSOR",      sizeof(ThreadCursor));
  /* 63 */ vAddConstant("SIZEOF_THREADLIST",        sizeof(ThreadList));
  /* 64 */ vAddConstant("SIZEOF_TIME",              sizeof(Time));
  /* 65 */ vAddConstant("SIZEOF_TIMER",             sizeof(Timer));
  /* 66 */ vAddConstant("SIZEOF_TOKENISER",         sizeof(Tokeniser));
  /* 67 */ vAddConstant("SIZEOF_TX",                sizeof(Tx));
  /* 68 */ vAddConstant("SIZEOF_TXARRAY",           sizeof(TxArray));
  /* 69 */ vAddConstant("SIZEOF_TXCOLLECTION",      sizeof(TxCollection));
  /* 70 */ vAddConstant("SIZEOF_TXCURSOR",          sizeof(TxCursor));
  /* 71 */ vAddConstant("SIZEOF_TXMAP",             sizeof(TxMap));
  /* 72 */ vAddConstant("SIZEOF_TXREF",             sizeof(TxRef));
  /* 73 */ vAddConstant("SIZEOF_WORD",              sizeof(Word));

  /*  0 */ vAddConstant("SIZEOF_S32",               sizeof(s32));
  /*  1 */ vAddConstant("SIZEOF_S256",              sizeof(s256));
  /*  2 */ vAddConstant("SIZEOF_S16K",              sizeof(s8K));

  /*  0 */ vAddConstant("TS_NONE",                  TS_NONE);
  /*  1 */ vAddConstant("TS_BLOCKED",               TS_BLOCKED);
  /*  2 */ vAddConstant("TS_EXITED",                TS_EXITED);
  /*  3 */ vAddConstant("TS_RUNNING",               TS_RUNNING);
  /*  4 */ vAddConstant("TS_SLEEPING",              TS_SLEEPING);
  /*  5 */ vAddConstant("TS_SUSPENDED",             TS_SUSPENDED);
  /*  6 */ vAddConstant("TS_WAITING",               TS_WAITING);
  /*  7 */ vAddConstant("TS_YIELDED",               TS_YIELDED);
  /*  8 */ vAddConstant("TS_ENUMS",                 TS_ENUMS);

  /*  0 */ vAddConstant("TP_NONE",                  TP_NONE);
  /*  1 */ vAddConstant("TP_EXECUTING",             TP_EXECUTING);
  /*  2 */ vAddConstant("TP_COMMITTING",            TP_COMMITTING);
  /*  3 */ vAddConstant("TP_COMMITTED",             TP_COMMITTED);
  /*  4 */ vAddConstant("TP_PREPARING",             TP_PREPARING);
  /*  5 */ vAddConstant("TP_PREPARED",              TP_PREPARED);
  /*  6 */ vAddConstant("TP_ROLLINGBACK",           TP_ROLLINGBACK);
  /*  7 */ vAddConstant("TP_ROLLEDBACK",            TP_ROLLEDBACK);
  /*  8 */ vAddConstant("TP_FREE",                  TP_FREE);
  /*  9 */ vAddConstant("TP_ENUMS",                 TP_ENUMS);

  /*  0 */ vAddConstant("OPEN_READ",                C_OPEN_READ);
  /*  1 */ vAddConstant("OPEN_WRITE",               C_OPEN_WRITE);
  /*  2 */ vAddConstant("OPEN_APPEND",              C_OPEN_APPEND);
  /*  3 */ vAddConstant("OPEN_READ_WRITE_EXIST",    C_OPEN_READ_WRITE_EXIST);
  /*  4 */ vAddConstant("OPEN_READ_WRITE_CREATE",   C_OPEN_READ_WRITE_CREATE);
  /*  5 */ vAddConstant("OPEN_READ_APPEND",         C_OPEN_READ_APPEND);
  
  /*  0 */ vAddConstant("CLOCK_FREQUENCY",          Synch::iTickFrequency());
  /*  0 */ vAddConstant("CORE_COUNT",               System::iCoreCount());
  /*  0 */ vAddConstant("CPU_COUNT",                System::iCpuCount());
  /*  0 */ vAddConstant("PHYSICAL_MEMORY",          System::iPhysicalMemory());
  
  /*  1 */ vAddConstant("FILE_FIFO",                DT_FIFO);
  /*  2 */ vAddConstant("FILE_CHR",                 DT_CHR);
  /*  3 */ vAddConstant("FILE_DIR",                 DT_DIR);
  /*  6 */ vAddConstant("FILE_BLK",                 DT_BLK);
  /*  8 */ vAddConstant("FILE_REG",                 DT_REG);
  /* 10 */ vAddConstant("FILE_LNK",                 DT_LNK);
  /* 12 */ vAddConstant("FILE_SOCK",                DT_SOCK);

  vAddConstant("listerTaskPrint", (i8)0);
  vAddConstant("monitorTaskPrint",(i8)0);
  vAddConstant("reclaimTaskPrint",(i8)0);
  vAddConstant("requestTaskPrint",(i8)0);
  vAddConstant("answerTaskPrint",(i8)0);
  vAddConstant("connectorTaskPrint",(i8)0);
  vAddConstant("senderTaskPrint", (i8)0);
  vAddConstant("receiverTaskPrint", (i8)0);
}

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("word",            &vWord,             false);
  gpWord = pFindWord("word");
  
  vAddPrimitive("accept",          &vAccept,           false);
  vAddPrimitive("allot",           &vAllot,            false);
  vAddPrimitive("bpa",             &vBpa,              false);
  vAddPrimitive("bpdump",          &vBpdump,           false);
  vAddPrimitive("(int)",           &vCastInt,          false);
  vAddPrimitive("(ptr)",           &vCastPtr,          false);
  vAddPrimitive("cells",           &vCells,            false);
  vAddPrimitive("class@",          &vClassFetch,       false);
  vAddPrimitive("class!",          &vClassStore,       false);
  vAddPrimitive("code=",           &vCodeAssign,       false);
  vAddPrimitive(":",               &vColon,            false);
  vAddPrimitive(",",               &vComma,            false);
  vAddPrimitive("(",               &vComment,          true );
  vAddPrimitive("compile,",        &vCompileComma,     false);
  vAddPrimitive("compiling",       &vCompiling,        false);
  vAddPrimitive("create",          &vCreate,           false);
  vAddPrimitive(">create",         &vCreateFrom,       false);
  vAddPrimitive("cursor@",         &vCursorFetch,      false);
  vAddPrimitive("cursor!",         &vCursorStore,      false);
  vAddPrimitive("does>",           &vDoes,             true );
  vAddPrimitive("encodeCall",      &vEncodeCall,       false);
  vAddPrimitive("encodeDoes",      &vEncodeDoes,       false);
  vAddPrimitive("encodeEmb",       &vEncodeEmb,        true );
  vAddPrimitive("encodeFun",       &vEncodeFun,        false);
  vAddPrimitive("encodeVal",       &vEncodeVal,        false);
  vAddPrimitive("env@",            &vEnvGet,           false);
  vAddPrimitive("enterDoes",       &vEnterDoes,        false);
  vAddPrimitive("enterRoutine",    &vEnterRoutine,     false);
  vAddPrimitive("]",               &vEnterCompiler,    false);
  vAddPrimitive("[",               &vEnterInterpreter, true );
  vAddPrimitive("eql",             &vEqualAddress,     false);
  vAddPrimitive("execute",         &vExecute,          false);
  vAddPrimitive("exit",            &vExit,             false);
  vAddPrimitive("@",               &vFetch,            false);
  vAddPrimitive("find",            &vFind,             false);
  vAddPrimitive("findInCurrent",   &vFindInCurrent,    false);
  vAddPrimitive("freeBuffers",     &vFreeBuffers,      false);
  vAddPrimitive("freePercent",     &vFreePercent,      false);
  vAddPrimitive("freeSpace",       &vFreeSpace,        false);
  vAddPrimitive("hash",            &vHash,             false);
  vAddPrimitive("hazard",          &vHazard,           false);
  vAddPrimitive("i",               &vI,                false);
  vAddPrimitive("immediate",       &vImmediate,        true );
  vAddPrimitive("interpreting",    &vInterpreting,     false);
  vAddPrimitive("inUseSpace",      &vInUseSpace,       false);
  vAddPrimitive("isBooted",        &vIsBooted,         false);
  vAddPrimitive("isCompiling",     &vIsCompiling,      false);
  vAddPrimitive("isImmediate",     &vIsImmediate,      false);
  vAddPrimitive("isInterpreting",  &vIsInterpreting,   false);
  vAddPrimitive("isPrimitive",     &vIsPrimitive,      false);
  vAddPrimitive("j",               &vJ,                false);
  vAddPrimitive("locate",          &vLocate,           false);
  vAddPrimitive("magic@",          &vMagicFetch,       false);
  vAddPrimitive("magic!!",         &vMagicMark,        false);
  vAddPrimitive("nameWord",        &vNameWord,         false);
  vAddPrimitive("nop",             &vNop,              false);
  vAddPrimitive("nullPtr",         &vNullPtr,          false);
  vAddPrimitive("object@",         &vObjectFetch,      false);
  vAddPrimitive("object!!",        &vObjectMark,       false);
  vAddPrimitive("object!",         &vObjectStore,      false);
  vAddPrimitive("prevWord",        &vPrevWord,         false);
  vAddPrimitive(".",               &vPrint,            false);
  vAddPrimitive("push",            &vPush,             false);
  vAddPrimitive(".put",            &vPut,              false);
  vAddPrimitive(".\\n",            &vPutNewline,       false);
  vAddPrimitive(".putVerbose",     &vPutVerbose,       false);
  vAddPrimitive("rdtscp",          &vRdtscp,           false);
  vAddPrimitive("retryPoint",      &vRetryPoint,       false);
  vAddPrimitive("see",             &vSee,              false);
  vAddPrimitive(";",               &vSemicolon,        true );
  vAddPrimitive("sleep",           &vSleep,            false);
  vAddPrimitive("snooze",          &vSnooze,           false);
  vAddPrimitive("sprint",          &vSprint,           false);
  vAddPrimitive("!",               &vStore,            false);
  vAddPrimitive("str:",            &vStringColon,      false);
  vAddPrimitive("test",            &vTest,             false);
  vAddPrimitive("throw",           &vThrow,            false);
  vAddPrimitive("totalSpace",      &vTotalSpace,       false);
  vAddPrimitive("'",               &vTick,             true );
  vAddPrimitive(">body",           &vToBody,           false);
  vAddPrimitive(">cmd",            &vToCmd,            false);
  vAddPrimitive(">digitName",      &vToDigitName,      false);
  vAddPrimitive(">int",            &vToInt,            false);
  vAddPrimitive(">seitf",          &vToSEitf,          false);
  vAddPrimitive(">str",            &vToStr,            false);
  vAddPrimitive(">task",           &vToTask,           false);
  vAddPrimitive("token",           &vToken,            false);
  vAddPrimitive("traceOff",        &vTraceOff,         false);
  vAddPrimitive("traceOn",         &vTraceOn,          false);
  vAddPrimitive("tracePoint",      &vTracePoint,       false);
  vAddPrimitive("trace!",          &vTraceSet,         false);
  vAddPrimitive("wake",            &vWake,             false);
  
  //----------------------------------------------------------
  vAddPrimitive("args.partition",  &vArgsPartition, false);
  vAddPrimitive("args.print",      &vArgsPrint,     false);
  
  vAddPrimitive("assert",          &vAssert,            false);
  vAddPrimitive("assert.stack_0",  &vAssertStack_0,     false);
  vAddPrimitive("assert.stack_0+", &vAssertStack_0more, false);
  vAddPrimitive("assert.stack_1",  &vAssertStack_1,     false);
  vAddPrimitive("assert.stack_1+", &vAssertStack_1more, false);
  vAddPrimitive("assert.stack_2",  &vAssertStack_2,     false);
  vAddPrimitive("assert.stack_2+", &vAssertStack_2more, false);
  vAddPrimitive("assert.stack_3",  &vAssertStack_3,     false);
  vAddPrimitive("assert.stack_3+", &vAssertStack_3more, false);
  vAddPrimitive("assert.stack_4",  &vAssertStack_4,     false);
  vAddPrimitive("assert.stack_5+", &vAssertStack_4more, false);
  
  vAddPrimitive("adepth",          &vAstackDepth,      false);
  vAddPrimitive("adrop",           &vAstackDrop,       false);
  vAddPrimitive("adup",            &vAstackDup,        false);
  vAddPrimitive("a@",              &vAstackFetch,      false);
  vAddPrimitive("anip",            &vAstackNip,        false);
  vAddPrimitive("apick",           &vAstackPick,       false);
  vAddPrimitive("areverse",        &vAstackReverse,    false);
  vAddPrimitive(">a",              &vAstackToA,        false);
  vAddPrimitive("a>",              &vAstackToP,        false);
  vAddPrimitive(">auxStack",       &vAstackTransfer,   false);
  vAddPrimitive("aswap",           &vAstackSwap,       false);
  vAddPrimitive("avalset>",        &vAstackToValues,   false);
  vAddPrimitive(">avalset",        &vAstackToValSet,   false);
  vAddPrimitive("a0",              &vAstack0,          false);
  vAddPrimitive("a1",              &vAstack1,          false);
  vAddPrimitive("a2",              &vAstack2,          false);
  vAddPrimitive("a3",              &vAstack3,          false);
  vAddPrimitive("a4",              &vAstack4,          false);
  vAddPrimitive("a5",              &vAstack5,          false);
  vAddPrimitive("a6",              &vAstack6,          false);
  vAddPrimitive("a7",              &vAstack7,          false);
  vAddPrimitive("a8",              &vAstack8,          false);
  vAddPrimitive("a9",              &vAstack9,          false);
  
  vAddPrimitive("class.name",      &vClassName,    false);
  
  vAddPrimitive("csv.firstValue",  &vCsvFirstValue,    false);
  
  vAddPrimitive("dicLock.dropWrite",&vDicLockDropWrite, false);
  vAddPrimitive("dicLock.grabWrite",&vDicLockGrabWrite, false);
  
  vAddPrimitive("ddlLock.dropRead", &vDdlLockDropRead,  false);
  vAddPrimitive("ddlLock.dropWrite",&vDdlLockDropWrite, false);
  vAddPrimitive("ddlLock.grabRead", &vDdlLockGrabRead,  false);
  vAddPrimitive("ddlLock.grabWrite",&vDdlLockGrabWrite, false);
  vAddPrimitive("ddlLock.testDrop", &vDdlLockTestDrop,  false);
  
  vAddPrimitive("debug.trap",      &vDebugTrap,        false);
  
  vAddPrimitive("dic.count",       &vDicCount,         false);
  vAddPrimitive("dic.drop",        &vDicDrop,          false);
  vAddPrimitive("}}",              &vDicDrop,          false);
  vAddPrimitive("dic.frozen@",     &vDicFrozenGet,     false);
  vAddPrimitive("dic.frozen!",     &vDicFrozenSet,     false);
  vAddPrimitive("dic.init",        &vDicInit,          false);
  vAddPrimitive("dic.kind@",       &vDicKindGet,       false);
  vAddPrimitive("dic.kind!",       &vDicKindSet,       false);
  vAddPrimitive("dic.last",        &vDicLast,          false);
  vAddPrimitive("dic.main",        &vDicMain,          false);
  vAddPrimitive("dic.name",        &vDicName,          false);
  vAddPrimitive("dic.pick",        &vDicPick,          false);
  vAddPrimitive("dic.push",        &vDicPush,          false);
  vAddPrimitive("{{",              &vDicPush,          false);
  vAddPrimitive("dic.ref",         &vDicRef,           false);
  vAddPrimitive("dic.stack",       &vDicStack,         false);

  vAddPrimitive("dicStack.push",   &vDicStackPush,     false);
  
  vAddPrimitive("dir.exists",      &vDirExists,        false);
  vAddPrimitive("dir.make",        &vDirMake,          false);
  vAddPrimitive("dir.remove",      &vDirRemove,        false);
  vAddPrimitive("cwd",             &vCwd,              false);
  vAddPrimitive("gwd",             &vGwd,              false);

  vAddPrimitive("dir.close",       &vDirClose,         false);
  vAddPrimitive("dir.delete",      &vDirDelete,        false);
  vAddPrimitive("dir.entryName",   &vDirEntryName,     false);
  vAddPrimitive("dir.entryType",   &vDirEntryType,     false);
  vAddPrimitive("dir.init",        &vDirInit,          false);
  vAddPrimitive("dir.isClosed",    &vDirIsClosed,      false);
  vAddPrimitive("dir.isOpen",      &vDirIsOpen,        false);
  vAddPrimitive("dir.new",         &vDirNew,           false);
  vAddPrimitive("dir.open",        &vDirOpen,          false);
  vAddPrimitive("dir.path@",       &vDirPathGet,       false);
  vAddPrimitive("dir.path!",       &vDirPathSet,       false);
  vAddPrimitive("dir.read",        &vDirRead,          false);
  vAddPrimitive("dir.rewind",      &vDirRewind,        false);
  vAddPrimitive("dir.seek",        &vDirSeek,          false);
  vAddPrimitive("dir.tell",        &vDirTell,          false);
 
  vAddPrimitive("error.msg",       &vErrorMsg,         false);
  
  vAddPrimitive("file.close",      &vFileClose,        false);
  vAddPrimitive("file.delete",     &vFileDelete,       false);
  vAddPrimitive("file.init",       &vFileInit,         false);
  vAddPrimitive("file.new",        &vFileNew,          false);
  vAddPrimitive("file.open",       &vFileOpen,         false);
  vAddPrimitive("file.path@",      &vFilePathGet,      false);
  vAddPrimitive("file.path!",      &vFilePathSet,      false);
  vAddPrimitive("file.read",       &vFileRead,         false);
  vAddPrimitive("file.readRecord", &vFileReadRecord,   false);
  vAddPrimitive("file.write",      &vFileWrite,        false);
  
  vAddPrimitive("again",           &vFlowAgain,        true );
  vAddPrimitive("again_",          &vFlowAgain_,       false);
  vAddPrimitive("begin",           &vFlowBegin,        true );
  vAddPrimitive("begin_",          &vFlowBegin_,       false);
  vAddPrimitive("do",              &vFlowDo,           true );
  vAddPrimitive("do_rt",           &vFlowDo_RT,        false);
  vAddPrimitive("else",            &vFlowElse,         true );
  vAddPrimitive("if",              &vFlowIf,           true );
  vAddPrimitive("iterate",         &vFlowIterate,      true );
  vAddPrimitive("iterate_rt_A",    &vFlowIterate_RT_A, false);
  vAddPrimitive("iterate_rt_B",    &vFlowIterate_RT_B, false);
  vAddPrimitive("jump",            &vFlowJump,         false);
  vAddPrimitive("jumpFalse",       &vFlowJumpFalse,    false);
  vAddPrimitive("jumpTrue",        &vFlowJumpTrue,     false);
  vAddPrimitive("loop",            &vFlowLoop,         true );
  vAddPrimitive("loop_rt",         &vFlowLoop_RT,      false);
  vAddPrimitive("+loop",           &vFlowLoopPlus,     true );
  vAddPrimitive("+loop_rt",        &vFlowLoopPlus_RT,  false);
  vAddPrimitive("next",            &vFlowNext,         true );
  vAddPrimitive("repeat",          &vFlowRepeat,       true );
  vAddPrimitive("then",            &vFlowThen,         true );
  vAddPrimitive("unloop",          &vFlowUnloop,       false);
  vAddPrimitive("until",           &vFlowUntil,        true );
  vAddPrimitive("while",           &vFlowWhile,        true );
  
  vAddPrimitive("grid.appendNode", &vGridAppendNode,   false);
  vAddPrimitive("grid.started!",   &vGridStartedStore, false);
  vAddPrimitive("grid.started@",   &vGridStartedFetch, false);
  vAddPrimitive("grid.init",       &vGridInit,         false);
  
  vAddPrimitive("input.close",     &vInputClose,       false);
  vAddPrimitive("input.depth",     &vInputDepth,       false);
  vAddPrimitive("input.next",      &vInputNext,        false);
  vAddPrimitive("input.open",      &vInputOpen,        false);
  vAddPrimitive("input.pop",       &vInputPop,         false);
  vAddPrimitive("input.push",      &vInputPush,        false);
  vAddPrimitive("input.stdin",     &vInputStdin,       false);
  vAddPrimitive("input.text",      &vInputText,        false);
  vAddPrimitive("input.token",     &vInputToken,       false);
  vAddPrimitive("input.type",      &vInputType,        false);
  
  vAddPrimitive("+",               &vIntAdd,           false);
  vAddPrimitive("/",               &vIntDivide,        false);
  vAddPrimitive("==",              &vIntEqual,         false);
  vAddPrimitive(">",               &vIntGreater,       false);
  vAddPrimitive(">=",              &vIntGreaterEqual,  false);
  vAddPrimitive("<",               &vIntLess,          false);
  vAddPrimitive("<=",              &vIntLessEqual,     false);
  vAddPrimitive("%",               &vIntModulo,        false);
  vAddPrimitive("*",               &vIntMultiply,      false);
  vAddPrimitive("neg",             &vIntNeg,           false);
  vAddPrimitive("!=",              &vIntNotEqual,      false);
  vAddPrimitive("-",               &vIntSubtract,      false);

  vAddPrimitive("loader.delete",      &vLoaderDelete,      false);
  vAddPrimitive("loader.findChunk",   &vLoaderFindChunk,   false);
  vAddPrimitive("loader.init",        &vLoaderInit,        false);
  vAddPrimitive("loader.new",         &vLoaderNew,         false);
  vAddPrimitive("loader.newSub",      &vLoaderNewSub,      false);
  vAddPrimitive("loader.path!",       &vLoaderPathStore,   false);
  vAddPrimitive("loader.subs!",       &vLoaderSubsStore,   false);
  vAddPrimitive("loader.table!",      &vLoaderTableStore,  false);
  vAddPrimitive("loader.txref!",      &vLoaderTxRefStore,  false);
  
  vAddPrimitive("loaderSub.appendBuffer",&vLoaderSubAppendBuffer,false);
  vAddPrimitive("loaderSub.begin",       &vLoaderSubBegin,       false);
  vAddPrimitive("loaderSub.completed",   &vLoaderSubCompleted,   false);
  vAddPrimitive("loaderSub.delete",      &vLoaderSubDelete,      false);
  vAddPrimitive("loaderSub.end",         &vLoaderSubEnd,         false);
  vAddPrimitive("loaderSub.fetchRecord", &vLoaderSubFetchRecord, false);
  vAddPrimitive("loaderSub.hashRecord",  &vLoaderSubHashRecord,  false);
  vAddPrimitive("loaderSub.init",        &vLoaderSubInit,        false);
  vAddPrimitive("loaderSub.insertRow",   &vLoaderSubInsertRow,   false);
  vAddPrimitive("loaderSub.insertRow?",  &vLoaderSubInsertRow2,  false);
  vAddPrimitive("loaderSub.loader!",     &vLoaderSubLoaderStore, false);
  vAddPrimitive("loaderSub.new",         &vLoaderSubNew,         false);
  vAddPrimitive("loaderSub.partition",   &vLoaderSubPartition,   false);
  vAddPrimitive("loaderSub.partition!",  &vLoaderSubPartitionStore, false);
  vAddPrimitive("loaderSub.rowBuffer",   &vLoaderSubRowBuffer,   false);
  vAddPrimitive("loaderSub.rowBuffer!",  &vLoaderSubRowBufferStore, false);
  vAddPrimitive("loaderSub.table",       &vLoaderSubTable,       false);
  
  vAddPrimitive("local.fetch",     &vLocalFetch,       false);
  vAddPrimitive("local.store",     &vLocalStore,       false);
  vAddPrimitive("locals|",         &vLocalsBar,        true);
  vAddPrimitive("locals.pop",      &vLocalsPop,        false);
  vAddPrimitive("locals.push",     &vLocalsPush,       false);
  vAddPrimitive("to",              &vLocalsTo,         true);
  
  vAddPrimitive("lock.dropRead",   &vLockDropRead,     false);
  vAddPrimitive("lock.dropWrite",  &vLockDropWrite,    false);
  vAddPrimitive("lock.grabRead",   &vLockGrabRead,     false);
  vAddPrimitive("lock.grabWrite",  &vLockGrabWrite,    false);
  vAddPrimitive("lock.init",       &vLockInit,         false);
  
  vAddPrimitive("and",             &vLogicAnd,         false);
  vAddPrimitive("not",             &vLogicNot,         false);
  vAddPrimitive("or",              &vLogicOr,          false);
  
  vAddPrimitive("memory.byteDump", &vMemoryByteDump,   false);
  vAddPrimitive("memory.wordDump", &vMemoryWordDump,   false);
  
  vAddPrimitive("node.init",       &vNodeInit,         false);
  vAddPrimitive("node.new",        &vNodeNew,          false);
  vAddPrimitive("node.no",         &vNodeNo,           false);
  vAddPrimitive("node.status",     &vNodeStatus,       false);
  
  vAddPrimitive("nodeCursor.begin",  &vNodeCursorBegin,  false);
  vAddPrimitive("nodeCursor.delete", &vNodeCursorDelete, false);
  vAddPrimitive("nodeCursor.first",  &vNodeCursorFirst,  false);
  vAddPrimitive("nodeCursor.get",    &vNodeCursorGet,    false);
  vAddPrimitive("nodeCursor.init",   &vNodeCursorInit,   false);
  vAddPrimitive("nodeCursor.new",    &vNodeCursorNew,    false);
  vAddPrimitive("nodeCursor.next",   &vNodeCursorNext,   false);
  
  vAddPrimitive("object.wordbody",   &vObjectWordBody,   false);

  vAddPrimitive("number.magnitude", &vNumberMagnitude, false);
  
  vAddPrimitive("process.getPid",  &vProcessGetPid,    false);
  vAddPrimitive("process.testPid", &vProcessTestPid,   false);
  vAddPrimitive("process.quit",    &vProcessQuit,      false);
  
  vAddPrimitive("depth",           &vPstackDepth,      false);
  vAddPrimitive("drop",            &vPstackDrop,       false);
  vAddPrimitive("2drop",           &vPstackDropTwo,    false);
  vAddPrimitive("dump",            &vPstackDump,       false);
  vAddPrimitive("dup",             &vPstackDup,        false);
  vAddPrimitive("?dup",            &vPstackDupNonZero, false);
  vAddPrimitive("nip",             &vPstackNip,        false);
  vAddPrimitive("pick",            &vPstackPick,       false);
  vAddPrimitive("reverse",         &vPstackReverse,    false);
  vAddPrimitive("roll",            &vPstackRoll,       false);
  vAddPrimitive("-roll",           &vPstackRollInv,    false);
  vAddPrimitive("rot",             &vPstackRot,        false);
  vAddPrimitive("-rot",            &vPstackRotInv,     false);
  vAddPrimitive("swap",            &vPstackSwap,       false);
  vAddPrimitive("tuck",            &vPstackTuck,       false);
  vAddPrimitive("valset>",         &vPstackToValues,   false);
  vAddPrimitive(">valset",         &vPstackToValSet,   false);
  
  vAddPrimitive("queue.append",    &vQueueAppend,      false);
  vAddPrimitive("queue.dump",      &vQueueDump,        false);
  vAddPrimitive("queue.init",      &vQueueInit,        false);
  vAddPrimitive("queue.pop",       &vQueuePop,         false);
  
  vAddPrimitive("rowBuffer.append", &vRowBufferAppend, false);
  vAddPrimitive("rowBuffer.appendRecord", &vRowBufferAppendRecord, false);
  vAddPrimitive("rowBuffer.dump",   &vRowBufferDump,    false);
  vAddPrimitive("rowBuffer.new",    &vRowBufferNew,    false);
  
  vAddPrimitive("rowCursor.begin",  &vRowCursorBegin,  false);
  vAddPrimitive("rowCursor.delete", &vRowCursorDelete, false);
  vAddPrimitive("rowCursor.end",    &vRowCursorEnd,    false);
  vAddPrimitive("rowCursor.first",  &vRowCursorFirst,  false);
  vAddPrimitive("rowCursor.get",    &vRowCursorGet,    false);
  vAddPrimitive("rowCursor.init",   &vRowCursorInit,   false);
  vAddPrimitive("rowCursor.new",    &vRowCursorNew,    false);
  vAddPrimitive("rowCursor.next",   &vRowCursorNext,   false);
  vAddPrimitive("rowCursor.rowBuffer", &vRowCursorRowBuffer,  false);
  vAddPrimitive("rowCursor.table@", &vRowCursorTableFetch, false);
  vAddPrimitive("rowCursor.table!", &vRowCursorTableStore, false);
  
  vAddPrimitive("reg.pop",          &vRegPop,          false);
  vAddPrimitive("reg.push",         &vRegPush,         false);
  vAddPrimitive("reg0",             &vRegReg0,         false);
  vAddPrimitive("reg1",             &vRegReg1,         false);
  vAddPrimitive("reg2",             &vRegReg2,         false);
  vAddPrimitive("reg3",             &vRegReg3,         false);
  vAddPrimitive("reg4",             &vRegReg4,         false);
  vAddPrimitive("reg5",             &vRegReg5,         false);
  vAddPrimitive("reg6",             &vRegReg6,         false);
  vAddPrimitive("reg7",             &vRegReg7,         false);
  vAddPrimitive("reg8",             &vRegReg8,         false);
  vAddPrimitive("reg9",             &vRegReg9,         false);
  vAddPrimitive("regA",             &vRegRegA,         false);
  vAddPrimitive("regB",             &vRegRegB,         false);
  vAddPrimitive("regC",             &vRegRegC,         false);
  vAddPrimitive("regD",             &vRegRegD,         false);
  vAddPrimitive("regE",             &vRegRegE,         false);
  vAddPrimitive("regF",             &vRegRegF,         false);
  vAddPrimitive("regN",             &vRegRegNth,       false);
  vAddPrimitive(">reg0",            &vRegToReg0,       false);
  vAddPrimitive(">reg1",            &vRegToReg1,       false);
  vAddPrimitive(">reg2",            &vRegToReg2,       false);
  vAddPrimitive(">reg3",            &vRegToReg3,       false);
  vAddPrimitive(">reg4",            &vRegToReg4,       false);
  vAddPrimitive(">reg5",            &vRegToReg5,       false);
  vAddPrimitive(">reg6",            &vRegToReg6,       false);
  vAddPrimitive(">reg7",            &vRegToReg7,       false);
  vAddPrimitive(">reg8",            &vRegToReg8,       false);
  vAddPrimitive(">reg9",            &vRegToReg9,       false);
  vAddPrimitive(">regA",            &vRegToRegA,       false);
  vAddPrimitive(">regB",            &vRegToRegB,       false);
  vAddPrimitive(">regC",            &vRegToRegC,       false);
  vAddPrimitive(">regD",            &vRegToRegD,       false);
  vAddPrimitive(">regE",            &vRegToRegE,       false);
  vAddPrimitive(">regF",            &vRegToRegF,       false);
  vAddPrimitive(">regN",            &vRegToRegNth,     false);
  
  vAddPrimitive("rdepth",          &vRstackDepth,      false);
  vAddPrimitive("rdrop",           &vRstackDrop,       false);
  vAddPrimitive("r@",              &vRstackFetch,      false);
  vAddPrimitive("rpick",           &vRstackPick,       false);
  vAddPrimitive("rreverse",        &vRstackReverse,    false);
  vAddPrimitive("r>",              &vRstackToP,        false);
  vAddPrimitive(">r",              &vRstackToR,        false);
  vAddPrimitive("r0",              &vRstack0,          false);
  vAddPrimitive("r1",              &vRstack1,          false);
  vAddPrimitive("r2",              &vRstack2,          false);
  vAddPrimitive("r3",              &vRstack3,          false);
  vAddPrimitive("r4",              &vRstack4,          false);
  vAddPrimitive("r5",              &vRstack5,          false);
  vAddPrimitive("r6",              &vRstack6,          false);
  vAddPrimitive("r7",              &vRstack7,          false);
  vAddPrimitive("r8",              &vRstack8,          false);
  vAddPrimitive("r9",              &vRstack9,          false);
  
  vAddPrimitive("seitf.count",     &vSEitfCount,       false);
  vAddPrimitive("seitf.delete",    &vSEitfDelete,      false);
  vAddPrimitive("seitf.dump",      &vSEitfDump,        false);
  vAddPrimitive("seitf.new",       &vSEitfNew,         false);
  vAddPrimitive("seitf.req@",      &vSEitfReqFetch,    false);
  vAddPrimitive("seitf.req!",      &vSEitfReqStore,    false);
  vAddPrimitive("seitf.rsp@",      &vSEitfRspFetch,    false);
  vAddPrimitive("seitf.rsp!",      &vSEitfRspStore,    false);
  vAddPrimitive("seitf.table@",    &vSEitfTableFetch,  false);
  vAddPrimitive("seitf.tableRef",  &vSEitfTableRef,    false);
  vAddPrimitive("seitf.tableDeref",&vSEitfTableDeref,  false);
  
  vAddPrimitive("seitfCursor.begin",  &vSEitfCursorBegin,  false);
  vAddPrimitive("seitfCursor.delete", &vSEitfCursorDelete, false);
  vAddPrimitive("seitfCursor.end",    &vSEitfCursorEnd,    false);
  vAddPrimitive("seitfCursor.first",  &vSEitfCursorFirst,  false);
  vAddPrimitive("seitfCursor.get",    &vSEitfCursorGet,    false);
  vAddPrimitive("seitfCursor.init",   &vSEitfCursorInit,   false);
  vAddPrimitive("seitfCursor.new",    &vSEitfCursorNew,    false);
  vAddPrimitive("seitfCursor.next",   &vSEitfCursorNext,   false);
  
  vAddPrimitive("seitfList.dump",  &vSEitfListDump,    false);
  vAddPrimitive("seitfList.init",  &vSEitfListInit,    false);

  vAddPrimitive("shell.execute",  &vShellExecute,  false);
 
  vAddPrimitive("astack.class", &vAstackClass,    false);
  vAddPrimitive("stack.class",  &vStackClass,     false);
  
  vAddPrimitive("steps.list",  &vStepsList,  false);
  vAddPrimitive("steps.reset", &vStepsReset, false);
  
  vAddPrimitive("strpad", &vStringPad, false);
  vAddPrimitive("streql", &vStrEql,    false);
  vAddPrimitive("strlen", &vStrLen,    false);
  vAddPrimitive("subStr", &vSubStr,    false);
  vAddPrimitive("concat", &vConcat,    false);
  vAddPrimitive("clip",   &vClip,      false);
  vAddPrimitive("prepend",    &vPrepend,     false);
  vAddPrimitive("toDigitName",&vToDigitName, false);
  vAddPrimitive("rndStr", &vRndStr,    false);
  
  vAddPrimitive("s256.init", &vS256Init, false);
 
  vAddPrimitive("table.append",       &vTableAppend,       false);
  vAddPrimitive("table.appendBuffer", &vTableAppendBuffer, false);
  vAddPrimitive("table.dic",          &vTableDic,          false);
  vAddPrimitive("table.drop",         &vTableDrop,         false);
  vAddPrimitive("table.init",         &vTableInit,         false);
  vAddPrimitive("table.insert",       &vTableInsert,       false);
  vAddPrimitive("table.name",         &vTableName,         false);
  vAddPrimitive("table.ref",          &vTableRef,          false);
  vAddPrimitive("table.space",        &vTableSpace,        false);
  vAddPrimitive("table.truncate",     &vTableTruncate,     false);
  
  vAddPrimitive("task.dec",        &vTaskAcksDec,      false);
  vAddPrimitive("task.acks@",      &vTaskAcksFetch,    false);
  vAddPrimitive("task.acks!",      &vTaskAcksStore,    false);
  vAddPrimitive("task.comment@",   &vTaskCommentFetch, false);
  vAddPrimitive("task.comment!",   &vTaskCommentStore, false);
  vAddPrimitive("task.dic",        &vTaskDic,          false);
  vAddPrimitive("task.dump",       &vTaskDump,         false);
  vAddPrimitive("task.init",       &vTaskInit,         false);
  vAddPrimitive("task.name@",      &vTaskNameFetch,    false);
  vAddPrimitive("task.name!",      &vTaskNameStore,    false);
  vAddPrimitive("task.new",        &vTaskNew,          false);
  vAddPrimitive("task.parent@",    &vTaskParentFetch,  false);
  vAddPrimitive("task.parent!",    &vTaskParentStore,  false);
  vAddPrimitive("task.queue@",     &vTaskQfetch,       false);
  vAddPrimitive("task.queue!",     &vTaskQstore,       false);
  vAddPrimitive("task.seed",       &vTaskSeed,         false);
  vAddPrimitive("task.seitf@",     &vTaskSeitfFetch,   false);
  vAddPrimitive("task.seitf!",     &vTaskSeitfStore,   false);
  vAddPrimitive("task.signal",     &vTaskSignal,       false);
  vAddPrimitive("task.socket@",    &vTaskSocketFetch,  false);
  vAddPrimitive("task.socket!",    &vTaskSocketStore,  false);
  vAddPrimitive("task.state@",     &vTaskStateFetch,   false);
  vAddPrimitive("task.state!",     &vTaskStateStore,   false);
  vAddPrimitive("task.suspend",    &vTaskSuspend,      false);
  vAddPrimitive("task.taskset@",   &vTaskTaskSetFetch, false);
  vAddPrimitive("task.taskset!",   &vTaskTaskSetStore, false);
  vAddPrimitive("task.this",       &vTaskThis,         false);
  vAddPrimitive("task.thread",     &vTaskThread,       false);
  vAddPrimitive("task.wait",       &vTaskWait,         false);
  vAddPrimitive("task.yield",      &vTaskYield,        false);
  
  vAddPrimitive("taskCursor.begin",  &vTaskCursorBegin,  false);
  vAddPrimitive("taskCursor.delete", &vTaskCursorDelete, false);
  vAddPrimitive("taskCursor.end",    &vTaskCursorEnd,    false);
  vAddPrimitive("taskCursor.first",  &vTaskCursorFirst,  false);
  vAddPrimitive("taskcursor.get",    &vTaskCursorGet,    false);
  vAddPrimitive("taskcursor.init",   &vTaskCursorInit,   false);
  vAddPrimitive("taskcursor.new",    &vTaskCursorNew,    false);
  vAddPrimitive("taskcursor.next",   &vTaskCursorNext,   false);
  
  vAddPrimitive("taskList",        &vTaskList,         false);
  vAddPrimitive("taskList.append", &vTaskListAppend,   false);
  vAddPrimitive("taskList.dump",   &vTaskListDump,     false);
  vAddPrimitive("taskList.init",   &vTaskListInit,     false);
  
  vAddPrimitive("taskSet.append",  &vTaskSetAppend,    false);
  vAddPrimitive("taskSet.dump",    &vTaskSetDump,      false);
  vAddPrimitive("taskSet.init",    &vTaskSetInit,      false);
  
  vAddPrimitive("thread.append",      &vThreadAppend,      false);
  vAddPrimitive("thread.bind",        &vThreadBind,        false);
  vAddPrimitive("thread.condSignal",  &vThreadCondSignal,  false);
  vAddPrimitive("thread.condWait",    &vThreadCondWait,    false);
  vAddPrimitive("thread.cpu",         &vThreadCpu,         false);
  vAddPrimitive("thread.dump",        &vThreadDump,        false);
  vAddPrimitive("thread.init",        &vThreadInit,        false);
  vAddPrimitive("thread.id",          &vThreadId,          false);
  vAddPrimitive("thread.mutexLock",   &vThreadMutexLock,   false);
  vAddPrimitive("thread.mutexUnlock", &vThreadMutexUnlock, false);
  vAddPrimitive("thread.name@",       &vThreadNameFetch,   false);
  vAddPrimitive("thread.name!",       &vThreadNameStore,   false);
  vAddPrimitive("thread.taskset@",    &vThreadSfetch,      false);
  vAddPrimitive("thread.taskset!",    &vThreadSstore,      false);
  vAddPrimitive("thread.sleep_s",     &vThreadSleep_s,     false);
  vAddPrimitive("thread.sleep_ms",    &vThreadSleep_ms,    false);
  vAddPrimitive("thread.sleep_us",    &vThreadSleep_us,    false);
  vAddPrimitive("thread.sleep_ns",    &vThreadSleep_ns,    false);
  vAddPrimitive("thread.spawn",       &vThreadSpawn,       false);
  
  vAddPrimitive("threadCursor.begin",  &vThreadCursorBegin,  false);
  vAddPrimitive("threadCursor.delete", &vThreadCursorDelete, false);
  vAddPrimitive("threadCursor.end",    &vThreadCursorEnd,    false);
  vAddPrimitive("threadCursor.first",  &vThreadCursorFirst,  false);
  vAddPrimitive("threadCursor.get",    &vThreadCursorGet,    false);
  vAddPrimitive("threadCursor.init",   &vThreadCursorInit,   false);
  vAddPrimitive("threadCursor.new",    &vThreadCursorNew,    false);
  vAddPrimitive("threadCursor.next",   &vThreadCursorNext,   false);

  vAddPrimitive("threadList.dump", &vThreadListDump,   false);
  vAddPrimitive("threadList.init", &vThreadListInit,   false);
  
  vAddPrimitive("timer.delete",    &vTimerDelete,      false);
  vAddPrimitive("timer.init",      &vTimerInit,        false);
  vAddPrimitive("timer.microsecs", &vTimerMicrosecs,   false);
  vAddPrimitive("timer.millisecs", &vTimerMillisecs,   false);
  vAddPrimitive("timer.nanosecs",  &vTimerNanosecs,    false);
  vAddPrimitive("timer.new",       &vTimerNew,         false);
  vAddPrimitive("timer.reset",     &vTimerReset,       false);
  vAddPrimitive("timer.secs",      &vTimerSecs,        false);
  vAddPrimitive("timer.start",     &vTimerStart,       false);
  vAddPrimitive("timer.ticks",     &vTimerTicks,       false);
  vAddPrimitive("timer.update",    &vTimerUpdate,      false);
  
  vAddPrimitive("tx.dump",         &vTxDump,           false);
  
  vAddPrimitive("txCursor.begin",  &vTxCursorBegin,  false);
  vAddPrimitive("txCursor.delete", &vTxCursorDelete, false);
  vAddPrimitive("txCursor.end",    &vTxCursorEnd,    false);
  vAddPrimitive("txCursor.first",  &vTxCursorFirst,  false);
  vAddPrimitive("txCursor.get",    &vTxCursorGet,    false);
  vAddPrimitive("txCursor.init",   &vTxCursorInit,   false);
  vAddPrimitive("txCursor.new",    &vTxCursorNew,    false);
  vAddPrimitive("txCursor.next",   &vTxCursorNext,   false);
  
  vAddPrimitive("txCollection.dump", &vTxCollectionDump, false);
  vAddPrimitive("txCollection.init", &vTxCollectionInit, false);
  
  vAddPrimitive("txMap.init",      &vTxMapInit,        false);
  
  vAddPrimitive("txRef.init",      &vTxRefInit,        false);
  vAddPrimitive("txRef.begin",     &vTxRefBegin,       false);
  vAddPrimitive("txRef.end",       &vTxRefEnd,         false);
  vAddPrimitive("txRef.commit",    &vTxRefCommit,      false);
  vAddPrimitive("txRef.rollback",  &vTxRefRollback,    false);
  
  vAddPrimitive("word.dump",       &vWordDump,         false);
  vAddPrimitive("word.dic",        &vWordDic,          false);
  vAddPrimitive("forget",          &vForget,           false);
  vAddPrimitive(">forget",         &vWordForget,       false);
  vAddPrimitive("word.kind@",      &vWordKindGet,      false);
  vAddPrimitive("word.kind!",      &vWordKindSet,      false);
  vAddPrimitive("word.name",       &vWordName,         false);
  vAddPrimitive("word.this",       &vWordThis,         false);
  
  vAddPrimitive("0<",              &vZeroLess,         false);
  vAddPrimitive("0<=",             &vZeroLessEqual,    false);
  vAddPrimitive("0==",             &vZeroEqual,        false);
  vAddPrimitive("0!=",             &vZeroNotEqual,     false);
  vAddPrimitive("0>=",             &vZeroGreaterEqual, false);
  vAddPrimitive("0>",              &vZeroGreater,      false);

  //----------------------------------------------------------
  vAddPrimitive("socket.accept",   &vSocketAccept,     false);
  vAddPrimitive("socket.close",    &vSocketClose,      false);
  vAddPrimitive("socket.connect",  &vSocketConnect,    false);
  vAddPrimitive("socket.listen",   &vSocketListen,     false);
  vAddPrimitive("socket.open",     &vSocketOpen,       false);
  vAddPrimitive("socket.recv",     &vSocketRecv,       false);
  vAddPrimitive("socket.send",     &vSocketSend,       false);
  
  //----------------------------------------------------------
  vAddPrimitive("bytes.magnitude", &vBytesMagnitude,   false);
}

/*----------------------------------------------------------------------------*/
/* add initial set of routines into dictionary (min set to boot)              */
/*----------------------------------------------------------------------------*/
void Task::vAddRoutines(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  s8K sDefinition;

  sDefinition =
  ": __nil ;";
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": docol "
  
  "  retryPoint"
  
  "  begin_"
  "    input.next"
  
  // end of input
  "    input.type TK_NONE == if"
  "      exit"
  "    then"
  
  // integer
  "    input.type TK_INTEGER == if"
  
  "      input.token >int"        // convert str to int, leave on stack
  
  "      isinterpreting if"       // interpret leaves int on stack
  "        again_"
  "      then"
  
  "      iscompiling if"          // compile encodes push intval
  "        encodeVal"
  "        again_"
  "      then"
  
  "    then"
  
  // string
  "    input.type TK_STRING == if"
  
  "      input.token"             // put str on stack
  
  "      isinterpreting if"       // interpret leaves str on stack
  "        again_"
  "      then"
  
  "      iscompiling if"          // compile encodes push strval
  "        encodeVal"
  "        again_"
  "      then"
  
  "    then"
  
  // what?
  "    input.type TK_NAME != if"  // double check we have a name
  "      \"strange token\" . input.type ."
  "      again_"
  "    then"
  
  // known name?
  "    input.token find"         // attempt to find
  "    dup not if"
  "      drop"
  "      \"unknown\" ."
  "      input.token ."
  "      isBooted not if process.quit then "
  "      again_"
  "    then"
  
  // known word on stack
  
  // immediate
  "    dup isImmediate if"       // execute an immediate word
  "      execute"
  "      again_"
  "    then"
  
  // compiling
  "    isCompiling if "          // compile encodes call to word
  "      dup isPrimitive if"
  "        encodeFun"
  "        again_"
  "      then"
  "      dup isPrimitive not if"
  "        encodeCall"
  "        again_"
  "      then"
  "    then"
  
  // interpreting
  "    isInterpreting if "       // interpreting calls word
  "      execute"
  "      again_"
  "    then"
  
  "  again_"
  " ;";
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": ::exit"
  "  TS_EXITED task.this task.state!"
  "  ;";
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": ::docmd"
  //  "  input.text"
  "  docol"
  "  ::exit"
  "  ;";
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  
  sDefinition =
  ": ::enter"
  "  input.stdin"
  "  docol"
  "  ::exit"
  "  ;";
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": >>:"
  "  input.push"
  "  input.open"
  "  docol"
  "  input.close"
  "  input.pop"
  "  ;";
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  // Temporary ifdef till we get sen files embedded in the executable
#ifdef __APPLE__
  sDefinition =
  ": ::dicCompile"
  "  \"::dicCompile\" .putVerbose"
//  "  gwd .putVerbose"
//  "  \"../../../../Commands\" cwd"
//  "  gwd .putVerbose"
//  "  traceon"
  "  \"compile.sen\" >>:"
  "  \"::dicCompile exit\" .putVerbose"
  "  ::exit"
  "  ;";
#else
  sDefinition =
  ": ::dicCompile"
  "  \"::dicCompile\" .putVerbose"
//  "  gwd .putVerbose"
//  "  \"../../Commands\" cwd"
//  "  gwd .putVerbose"
  "  \"compile.sen\" >>:"
  "  \"::dicCompile exit\" .putVerbose"
  "  ::exit"
  "  ;";
#endif
  pCompileColon(sDefinition);
}

/*----------------------------------------------------------------------------*/
/* checked this tx locked                                                     */
/*----------------------------------------------------------------------------*/
void Task::vCheckLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_xLock.vCheckWrite();
}

/*----------------------------------------------------------------------------*/
/* compile given : routine                                                    */
/*----------------------------------------------------------------------------*/
Word* Task::pCompileColon(s& sDefinition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pNewWord;
  TaskStep  pStep;
  Word* pFoundWord;
  ValWsp xValWsp;
  
  m_xTokeniser.vUseString(sDefinition);
  
  // get :
  vTokenNext();
  //vTrace("CompileColon: %s\n",m_sToken.a());
  
  // get name of word
  vTokenNext();
  //vTrace("CompileColon: %s\n",m_sToken.a());
  
  pNewWord = new(m_xDicStack.pTop()) Word(m_sToken.a());
  
  vCurrentWord(pNewWord);

  vTokenNext();
  
  while (TK_NONE != m_eToken) {
    
    //vTrace("CompileColon: %s\n",m_sToken.a());
    
    if (String::bEqualTo(";",m_sToken.a()))
      break;
    
    switch (m_eToken) {
        
      case TK_INTEGER:
        xValWsp = String::iToI8(m_sToken.a());
        pNewWord->vEncode("push",&xValWsp);
        break;
        
      case TK_NAME:
        pFoundWord = pFindWord(m_sToken);
        if (WT_PRIMITIVE == pFoundWord->eWordType()) {
          pStep = pFoundWord->pDoesStep();
          if (pFoundWord->bImmediate())
            (*pStep)(this);
          else {
            pNewWord->vAppend(pFoundWord->ppDoesStep());
          }
        }
        else if (WT_ROUTINE == pFoundWord->eWordType()) {
          pNewWord->vEncode(pFoundWord->ppDoesStep());
        }
        else {
          THROW("Invalid eWordType %lld",pFoundWord->eWordType());
        }
        break;
        
      case TK_STRING: {
        xValWsp = m_sToken.a();
        pNewWord->vEncode("push",&xValWsp);
        break;
      }
        
      default:
        THROW("Invalid m_eToken %lld",m_eToken);
    }
    
    vTokenNext();
  }
  
  pNewWord->vEncode("exit");
  

  // reset scanning
  m_xTokeniser.vUseEnd();
  
  return pNewWord;
}

/*----------------------------------------------------------------------------*/
/* get current word                                                           */
/*----------------------------------------------------------------------------*/
Word* Task::pCurrentWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
//  Dic* pDic;
//  
//  if (m_pSEitf)
//    pDic = m_pSEitf->pDicStack()->pTop();
//  else
//    pDic = m_xDicStack.pTop();
//
//  Word* pWord = pDic->pCurrentWord();
//  
//  return pWord;

  return m_pCurrentWord;
}

/*----------------------------------------------------------------------------*/
/* set current word                                                           */
/*----------------------------------------------------------------------------*/
void Task::vCurrentWord(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pCurrentWord = pWord;
}

/*----------------------------------------------------------------------------*/
/* debug                                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDebug(void)
{
}

/*----------------------------------------------------------------------------*/
/* drop dic                                                                   */
/*----------------------------------------------------------------------------*/
void Task::vDicDrop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_pSEitf)
    m_pSEitf->vPopDic();
  else
    m_xDicStack.vDiscard();
}

/*----------------------------------------------------------------------------*/
/* peek dic                                                                   */
/*----------------------------------------------------------------------------*/
Dic* Task::pDicPeek(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Dic* pDic = nullptr;
  
  if (m_pSEitf)
    pDic = m_pSEitf->pDicStack()->pPick(0);
  else
    pDic = m_xDicStack.pPick(0);
  
  return pDic;
}

/*----------------------------------------------------------------------------*/
/* push dic                                                                   */
/*----------------------------------------------------------------------------*/
void Task::vDicPush(Dic* pDic)
{
  if (m_pSEitf)
    m_pSEitf->vPushDic(pDic);
  else
    m_xDicStack.vPush(pDic);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_TASK);
  ASSERT(pObject->eClass() == CN_TASK);
  
  Task* pTask = reinterpret_cast<Task*>(pObject);
  
  vTrace("%sTask %p: %s TaskNo %lld state %s txId %lld\n",
         pTab,
         pTask,
         pTask->m_sName.a(),
         pTask->m_iTaskNo,
         pTS[pTask->m_eState],
         pTask->m_iTxId);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vDump()
{
  _poolAddress_(this);
  
  vTrace("%sTask %02lld %p: %16s state %8s txId %lld\n",
         pDoubleTab(),
         m_iTaskNo,
         this,
         m_sName.a(),
         pTS[m_eState],
         m_iTxId);
}

/*----------------------------------------------------------------------------*/
/* set exception                                                              */
/*----------------------------------------------------------------------------*/
void Task::vException(s& sFile, s& eException)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_sFile = sFile;
  m_sException = eException;
}

/*----------------------------------------------------------------------------*/
/* find word given name                                                       */
/*----------------------------------------------------------------------------*/
Word* Task::pFindWord(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pWord = nullptr;
  
  // attempt to find word in seitf dic stack
  if (m_pSEitf) {
    pWord = m_pSEitf->pFindWord(sName);
    if (pWord)
      return pWord;
  }
  
  // attempt to find word in this tasks dic stack
  pWord = m_xDicStack.pFind(sName.a());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* find word given name in current dictionary                                 */
/*----------------------------------------------------------------------------*/
Word* Task::pFindWordInCurrent(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pWord = nullptr;
  
  // attempt to find word in seitf current dic
  if (m_pSEitf) {
    pWord = m_pSEitf->pFindWordInCurrent(sName);
    if (pWord)
      return pWord;
    else
      return nullptr;
  }
  
  // attempt to find word in this tasks current dic
  pWord = m_xDicStack.pFindInCurrent(sName.a());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* find word given name                                                       */
/*----------------------------------------------------------------------------*/
Word* Task::pFindWord(const char* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pWord = m_xDicStack.pFind(aName);
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* forget word given name                                                     */
/*----------------------------------------------------------------------------*/
bool Task::bForgetWord(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  bool bResult;
  if (m_pSEitf)
    bResult = m_pSEitf->pDicStack()->bForget(sName);
  else
    bResult = m_xDicStack.bForget(sName);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* is given name?                                                             */
/*----------------------------------------------------------------------------*/
bool Task::bIsName(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  bool b = m_sName.bIsEqual(sName,C_NO_CASE);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* pop locals frame                                                           */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  RegFrame* pLocalsframe = m_pLocalsFrame;
  
  m_pLocalsFrame = m_pLocalsFrame->pNext();
  
  delete pLocalsframe;
}

/*----------------------------------------------------------------------------*/
/* push a locals frame                                                        */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPush(void)
{
  RegFrame* pLocalsframe = new RegFrame;
  
  pLocalsframe->vNext(m_pLocalsFrame);
  
  m_pLocalsFrame = pLocalsframe;
}

/*----------------------------------------------------------------------------*/
/* locate word based on cell                                                  */
/*----------------------------------------------------------------------------*/
Word* Task::pLocateWord(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pWord = m_xDicStack.pLocate(pCell);
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* get name of this task                                                      */
/*----------------------------------------------------------------------------*/
s& Task::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name of this task                                                      */
/*----------------------------------------------------------------------------*/
void Task::vName(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_sName = sName;
}

/*----------------------------------------------------------------------------*/
/* get next task in set                                                       */
/*----------------------------------------------------------------------------*/
NP_Task Task::npNextInSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_npNextInSet;
}

/*----------------------------------------------------------------------------*/
/* set next task in set                                                       */
/*----------------------------------------------------------------------------*/
void Task::vNextInSet(NP_Task npNextInSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_npNextInSet = npNextInSet;
}


/*----------------------------------------------------------------------------*/
/* get prev task in set                                                       */
/*----------------------------------------------------------------------------*/
NP_Task Task::npPrevInSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_npPrevInSet;
}

/*----------------------------------------------------------------------------*/
/* set prev task in set                                                       */
/*----------------------------------------------------------------------------*/
void Task::vPrevInSet(NP_Task npPrevInSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_npPrevInSet = npPrevInSet;
}

/*----------------------------------------------------------------------------*/
/* primitive entry                                                            */
/*----------------------------------------------------------------------------*/
void Task::vPrimitive(const char* aPrimitive)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! (m_iTracing & C_TRACE_STEPS) )
    return;
  
  char aRoutine[128];
  
  Cell* pCell = IP.m_pCell;
  Word* pLocatedWord = pLocateWord(pCell);
  if (pLocatedWord)
    String::vCopy(aRoutine,pLocatedWord->sName().a());
  else
    String::vCopy(aRoutine,"<unknown>");
  
  i8 iParamValues  = m_xParamStack.iValues();
  i8 iReturnValues = m_xReturnStack.iValues();
  vTrace("%-15s %15s : r %lld ; p%2d=",
         aRoutine,aPrimitive,iReturnValues,iParamValues);
  
//  for (i8 iValue = iParamValues - 1; iValue >= 0; iValue--) {
//    
//    if (iValue != iParamValues-1)
//      vTrace(",");
//    
//    ValWsp* pValWsp = static_cast<ValWsp*>(m_xParamStack.pPeek(iValue));
//    
//    eCN eClass = pValWsp->eClass();
//    
//    if (CN_INTVAL == eClass) {
//      vTrace("%lld",pValWsp->iInt());
//    }
//    else if (CN_PTRVAL == eClass) {
//      vTrace("%p (%s)",pValWsp->pPtr(),pValWsp->aClass());
//    }
//    else if (CN_STRVAL == eClass) {
//      vTrace("\"%lls\"",pValWsp->sStr());
//    }
//    else if (CN_VALSET == eClass) {
//      vTrace("%p (%s)",pValWsp,pValWsp->aClass());
//    }
//    else {
//      vTrace("unknown value on stack: class=%lld",pValWsp->eClass());
//    }
//  }

  s8K sStackContents;
  vStackContents(&m_xParamStack, sStackContents);
  
  vTrace("%s\n",sStackContents.a());
}

/*----------------------------------------------------------------------------*/
/* reset task                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  IP.m_pCell    = m_pResetIP;
  m_pBeginIP = nullptr;
  
  m_sToken = "";
  
  m_xParamStack.vReset();
  m_xReturnStack.vReset();
}

/*----------------------------------------------------------------------------*/
/* rertry after exception                                                     */
/*----------------------------------------------------------------------------*/
void Task::vRetry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  IP.m_pCell    = m_pResetIP;
  m_pBeginIP = nullptr;
  
  m_sToken = "";
  
  m_xParamStack.vRetry();
  m_xReturnStack.vRetry();
}

/*----------------------------------------------------------------------------*/
/* get root thread from root tasks param stack                                */
/*----------------------------------------------------------------------------*/
Thread* Task::pRootThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Object* pThreadEmb;
  
  if ( ! m_xParamStack.bPop(pThreadEmb) ) {
    String::vSprint(m_sException.a(),"Invalid param stack for: h.spawn");
    m_sFile = __FILE__;
    throw (i8)__LINE__;
  }
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  return pThread;
}

/*----------------------------------------------------------------------------*/
/* call routine                                                               */
/*----------------------------------------------------------------------------*/
void Task::vRoutineCall(Word* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  ASSERT(WT_ROUTINE == pWord->eWordType());
    
  m_xReturnStack.vPush((void*)IP.m_pCell);
    
  IP.m_pCell = pWord->pDoesArg();
}

/*----------------------------------------------------------------------------*/
/* seed task to run ::dicCompile                                              */
/*----------------------------------------------------------------------------*/
void Task::vSeedDicCompileSen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pBootSen = pFindWord("::dicCompile");
  ASSERT(pBootSen);
  
  IP.m_pCell = pBootSen->pFirstCell();
  
  m_eState = TS_RUNNING;
  
  m_xParamStack.vReset();
  m_xReturnStack.vReset();
  
  m_sLambda = "";
  
  m_xTokeniser.vUseString(m_sLambda);
}

/*----------------------------------------------------------------------------*/
/* seed task to run given lamda                                               */
/*----------------------------------------------------------------------------*/
void Task::vSeedWord(const char* aLambda)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pShellEnter = pFindWord("::docmd");
  
  IP.m_pCell = pShellEnter->pFirstCell();
  
  m_eState = TS_YIELDED;
  
  m_xParamStack.vReset();
  m_xReturnStack.vReset();
  
  m_sLambda = aLambda;
  if (m_sLambda.iLength())
    String::vConcat(m_sLambda.a()," ::exit");
  
  m_xTokeniser.vUseString(m_sLambda);
}

/*----------------------------------------------------------------------------*/
/* seed task to run ::enter                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSeedDocol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pShellEnter = pFindWord("::enter");
  
  IP.m_pCell = pShellEnter->pFirstCell();
  
  m_eState = TS_YIELDED;
  
  m_xParamStack.vReset();
  m_xReturnStack.vReset();
  
  m_sLambda = "";
  
  s32 sPrompt("docol> ");
  m_xTokeniser.vPrompt(sPrompt);
  
  m_xTokeniser.vUseString(m_sLambda);
  m_xTokeniser.vLineMode(false);
}

/*----------------------------------------------------------------------------*/
/* sprint of stack using given format string                                  */
/*----------------------------------------------------------------------------*/
void Task::vSprint(s8K& sFormat, s8K& sString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  char* pf = sFormat.a();
  char* ps = sString.a();
  
  /*
  while (*pf) {
    
    // test for ordinary character
    if ('%' != *pf) {
      *ps++ = *pf++;
      continue;
    }
    
    // we have a '%'
    pf++;
    
    // test for %%
    if ('%' == *pf) {
      *ps++ = *pf++;
      continue;
    }
    
    // test for %s
    if ('s' == *pf) {
      
      pf++;
      
      s8K sValue;
      m_xParamStack.bPop(sValue);
      
      for (char* pv = sValue.a(); *pv; )
        *ps++ = *pv++;
      
      continue;
    }

    // test for %i
    if ('i' == *pf) {
      
      pf++;
      
      i8 iValue;
      m_xParamStack.bPop(iValue);
      
      char aValue[32];
      String::wToString(iValue,aValue);
      
      for (char* pv = aValue; *pv; )
        *ps++ = *pv++;
      
      continue;
    }
    
    // just copy %?
    *ps++ = '%';
    *ps++ = *pf++;
  }
  
  *pf = '\0';
   
  */
  
  while (*pf) {
    
    // test for ordinary character
    if ('%' != *pf) {
      *ps++ = *pf++;
      continue;
    }
    
    // we have a '%'
    pf++;
    
    // test for %%
    if ('%' == *pf) {
      *ps++ = *pf++;
      continue;
    }
    
    // should be %v
    ASSERT('v' == *pf);
    
    Value* pValue0 = m_xParamStack.pPeek(0);
    
    // test for strval
    if (CN_STRVAL == pValue0->eClass()) {

      *pf = 's';
      pf++;
      
      s8K sValue;
      m_xParamStack.bPop(sValue);
      
      for (char* pv = sValue.a(); *pv; )
        *ps++ = *pv++;
      
      continue;
    }
    
    // test for intval
    if (CN_INTVAL == pValue0->eClass()) {
    
      *pf = 'i';
      pf++;
      
      i8 iValue;
      m_xParamStack.bPop(iValue);
      
      char aValue[32];
      String::wToString(iValue,aValue);
      
      for (char* pv = aValue; *pv; )
        *ps++ = *pv++;
      
      continue;
    }
    
    // just copy %?
    *ps++ = '%';
    *ps++ = *pf++;
  }
  
  *pf = '\0';

}

/*----------------------------------------------------------------------------*/
/* stack contents as string                                                   */
/*----------------------------------------------------------------------------*/
void Task::vStackContents(Stack* pStack, s& sStackContents)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  char* a = sStackContents.a();
  *a = '\0';
  
  i8 iParamValues = pStack->iValues();
  
  String::vSprintCat(a,"(");
  for (i8 iValue = iParamValues - 1; iValue >= 0; iValue--) {
    
    String::vSprintCat(a," ");
    
    ValWsp* pValWsp = static_cast<ValWsp*>(pStack->pPeek(iValue));
    
    eCN eClass = pValWsp->eClass();
    
    if (CN_INTVAL == eClass) {
      String::vSprintCat(a,"%lld",pValWsp->iInt());
    }
    else if (CN_PTRVAL == eClass) {
      if (pValWsp->pPtr()) {
        if (CN_WORD == ((Object*)pValWsp->pPtr())->eClass())
          String::vSprintCat(a,"^%s=%p:%s",
                             pCN[((Object*)pValWsp->pPtr())->eClass()],
                             pValWsp->pPtr(),
                             ((Word*)pValWsp->pPtr())->sName().a());
        else if (CN_QUEUE == ((Object*)pValWsp->pPtr())->eClass() ) {
          Word* pWord = Word::pToWord(pValWsp->pPtr());
          String::vSprintCat(a,"^%s=%p:%s",
                             pCN[((Object*)pValWsp->pPtr())->eClass()],
                             pValWsp->pPtr(),
                             pWord->sName().a());
        }
        else if (CN_TASK == ((Object*)pValWsp->pPtr())->eClass() ) {
          Task* pTask = _pTask(pValWsp->pPtr());
          String::vSprintCat(a,"^%s=%p:%s",
                             pCN[((Object*)pValWsp->pPtr())->eClass()],
                             pValWsp->pPtr(),
                             pTask->sName().a());
        }
        else
          String::vSprintCat(a,"^%s=%p",
                             pCN[((Object*)pValWsp->pPtr())->eClass()],
                             pValWsp->pPtr());
      }
      else
        String::vSprintCat(a,"^%s=%p",pValWsp->aClass(),pValWsp->pPtr());
    }
    else if (CN_STRVAL == eClass) {
      String::vSprintCat(a,"\"%lls\"",pValWsp->sStr());
    }
    else if (CN_VALSET == eClass) {
      String::vSprintCat(a,"<%s>=%p",pValWsp->aClass(),pValWsp);
    }
    else {
      String::vSprintCat(a,"<unknown:class=%lld>",pValWsp->eClass());
    }
  }
  
  String::vSprintCat(a," )");
}

/*----------------------------------------------------------------------------*/
/* get transaction state                                                      */
/*----------------------------------------------------------------------------*/
eTS Task::eState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_eState;
}

/*----------------------------------------------------------------------------*/
/* set transaction state                                                      */
/*----------------------------------------------------------------------------*/
void Task::vState(eTS eState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_xLock.vGrabWrite();
  m_eState = eState;
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get thread                                                                 */
/*----------------------------------------------------------------------------*/
Threads::Thread* Task::pThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pThread;
}

/*----------------------------------------------------------------------------*/
/* set thread                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThread(Threads::Thread* pThread)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pThread = pThread;
}

/*----------------------------------------------------------------------------*/
/* get next input token                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTokenNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_eToken = m_xTokeniser.eNextToken(m_sToken.a());
}

/*----------------------------------------------------------------------------*/
/*trace current point                                                         */
/*----------------------------------------------------------------------------*/
void Task::vTracePoint(const char* aMark)
{
  if (gxArguments.m_nNodes > 1) {
    i8 iPause = Random::iRandom(999,99999);
    PosixThread::vSleepMicro(iPause);
  }
  
  char aRoutine[128];
  
  Cell* pCell = IP.m_pCell;
  
  Word* pLocatedWord = pLocateWord(pCell);
  if (pLocatedWord)
    String::vCopy(aRoutine,pLocatedWord->sName().a());
  else
    String::vCopy(aRoutine,"<unknown>");
  
  i8 iAuxValues    = m_xAuxStack.iValues();
  i8 iParamValues  = m_xParamStack.iValues();
  i8 iReturnValues = m_xReturnStack.iValues();
  vTrace("[N%lld:%8s\\%lld %-11s] %-16s %-16s : a%2lld=",
         
         gxArguments.m_iNode,
         m_sName.a(),
         *m_zAcks,
         m_sComment.a(),
         
         aRoutine,
         aMark,
         iAuxValues);
         
  s8K sStackContents;
  vStackContents(&m_xAuxStack, sStackContents);
  
  vTrace("%s",sStackContents.a());
  
  vTrace(" r%2lld  p%2lld=", iReturnValues, iParamValues);

  vStackContents(&m_xParamStack, sStackContents);
  
  vTrace("%s\n",sStackContents.a());
}

/*----------------------------------------------------------------------------*/
/* get transaction id                                                         */
/*----------------------------------------------------------------------------*/
i8 Task::iTxId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_iTxId;
}

/*----------------------------------------------------------------------------*/
/* set the tx id                                                              */
/*----------------------------------------------------------------------------*/
void Task::vTxId(i8 iTxId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_iTxId = iTxId;
}

/*----------------------------------------------------------------------------*/
/* pop input direction                                                        */
/*----------------------------------------------------------------------------*/
void Task::vUndirect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  //vTrace("undirect return stack: %lld\n",nnpTask->m_xReturnStack.iValues());
  
  bool bPopped;
  
  bPopped = m_xTokeniser.bPop();
  ASSERT(bPopped);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* task loop                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  while (TS_RUNNING == m_eState || TS_WAITING == m_eState) {
    
    TaskStep pTaskStep = *IP.m_pCell->m_ppDoesStep;
    
    //vTrace("pTaskStep %p\n",pTaskStep);
    
    IP.m_pCell++;
    
    try {
      (pTaskStep)(this);
    }
    
    catch (i8 iException) {
      char wFileName[256];
      
      const char* ps;
      
      ps = String::sLast(m_sFile.a());
      String::bBackward(&ps,m_sFile.a(),'/');
      bool bBackSlash = String::bBackward(&ps,m_sFile.a(),'/');
      if (bBackSlash) {
        String::vCopy(wFileName,ps+1);
      }
      else {
        String::vCopy(wFileName, m_sFile.a());
      }
      vError("[%s] %s : line %lld in %s\n",
             m_sName.a(),
             m_sException.a(),
             iException,
             wFileName);
      vRetry();
    }
    
    catch (Error* pError) {
      char wFileName[256];
      
      const char* ps;
      
      ps = String::sLast(pError->pFileName());
      String::bBackward(&ps,pError->pFileName(),'/');
      String::vCopy(wFileName,ps+1);
      vError("[%s] %s: line %lld in %s\n",
             m_sName.a(),
             pError->pErrorMsg(),
             pError->iLineNumber(),
             wFileName);
      vRetry();
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* word -- root kind                                                          */
/*----------------------------------------------------------------------------*/
void Task::vWord(NNP_Task nnpTask)
{
ENTRY("vWord");
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* accept -- accept input string and push on stack                            */
/*----------------------------------------------------------------------------*/
void Task::vAccept(NNP_Task nnpTask)
{
ENTRY("vAccept");
  
  ValWsp xWsp;
  xWsp.vClass(CN_STRVAL);
  
  char* y = xWsp.sStr();
  
  y = xWsp.sStr();
  while ( '\n' != (*y++ = getc(stdin)) ) ;
  
  *--y = '\0';
  
  xWsp.pAsStrVal()->vSize();
  
  nnpTask->m_xParamStack.vPush(xWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* allot -- reserve n bytes in current word                                   */
/*----------------------------------------------------------------------------*/
void Task::vAllot(NNP_Task nnpTask)
{
ENTRY("vAllot");
  
  i8 iBytes;
  
  if ( !  nnpTask->m_xParamStack.bPop(iBytes) )
    THROW_INVALID_PARAM("allot");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();

  char* pAddress = pCurrentWord->pAllot(iBytes);
  ASSERT(pAddress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bpa -- buffer pool analyze                                                 */
/*----------------------------------------------------------------------------*/
void Task::vBpa(NNP_Task nnpTask)
{
ENTRY("vBpa");
  
  BPA* pBPA = new BPA;
  
  pBPA->vAnalyze();
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bpdump -- dump of objects in the buffer pool                               */
/*----------------------------------------------------------------------------*/
void Task::vBpdump(NNP_Task nnpTask)
{
ENTRY("vBpdump");
  
  BPA* pBPA = new BPA;
  
  pBPA->vDump();
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* (int) -- cast ptr to int                                                   */
/*----------------------------------------------------------------------------*/
void Task::vCastInt(NNP_Task nnpTask)
{
  ENTRY("vCastInt");
  
  Object* pPtr;
  if ( ! nnpTask->m_xParamStack.bPop(pPtr) )
    THROW_INVALID_PARAM("(int)");
  
  nnpTask->m_xParamStack.vPush((i8)pPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* (ptr) -- cast int to ptr                                                   */
/*----------------------------------------------------------------------------*/
void Task::vCastPtr(NNP_Task nnpTask)
{
  ENTRY("vCastPtr");
  
  i8 iInt;
  if ( ! nnpTask->m_xParamStack.bPop(iInt) )
    THROW_INVALID_PARAM("(ptr)");
  
  nnpTask->m_xParamStack.vPush((Object*)iInt);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cells -- convert cells into n bytes                                        */
/*----------------------------------------------------------------------------*/
void Task::vCells(NNP_Task nnpTask)
{
  ENTRY("vCells");
  
  i8 nCells;
  if ( !  nnpTask->m_xParamStack.bPop(nCells) )
    THROW_INVALID_PARAM("cells");
  
  i8 nBytes = nCells * sizeof(void*);
  
  nnpTask->m_xParamStack.vPush(nBytes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* classFetch -- fetch class number from Object object                        */
/*----------------------------------------------------------------------------*/
void Task::vClassFetch(NNP_Task nnpTask)
{
ENTRY("vClassFetch");
  
  Object* pObject;
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("classFetch");
  
  eCN eClassNo;
  eClassNo = pObject->eClass();
  
  nnpTask->m_xParamStack.vPush(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* classStore -- store class number into Object object                        */
/*----------------------------------------------------------------------------*/
void Task::vClassStore(NNP_Task nnpTask)
{
ENTRY("vClassStore");

  eCN eClassNo;
  if ( ! nnpTask->m_xParamStack.bPop(eClassNo) )
    THROW_INVALID_PARAM("class!");
  
  Object* pObject;
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("classStore");

  pObject->vClass(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* codeAssign -- fetch ptr from word.code on stack into this word             */
/*----------------------------------------------------------------------------*/
void Task::vCodeAssign(NNP_Task nnpTask)
{
ENTRY("vCodeAssign");
  
  Object* pObject;
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("codeAssign");
  Word* pThatWord = _pWord(pObject);
  
  Word* pThisWord = nnpTask->pCurrentWord();
  
  ASSERT(pThatWord->pDoesStep() == pThatWord->pDoesStep());
  pThisWord->vDoesStep(pThatWord->pDoesStep());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* : -- colon                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vColon(NNP_Task nnpTask)
{
ENTRY("vColon");
  
  nnpTask->m_bCompiling = true;

  nnpTask->vTokenNext();
  
  Word* pNewWord = new(nnpTask->m_xDicStack.pTop()) Word(nnpTask->m_sToken.a());
  
  nnpTask->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push value                                                 */
/*----------------------------------------------------------------------------*/
void Task::vComma(NNP_Task nnpTask)
{
ENTRY("vComma");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_xParamStack.bPop(xValWsp) )
    THROW_INVALID_PARAM("comma");
    
  pCurrentWord->vEncode("push",&xValWsp);
  pCurrentWord->vEncode("exit");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* comment -- comment                                                         */
/*----------------------------------------------------------------------------*/
void Task::vComment(NNP_Task nnpTask)
{
ENTRY("vComment");
  
  for (;;) {
    nnpTask->vTokenNext();
    if (String::bEqualTo(")",nnpTask->m_sToken.a()))
      break;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compile, -- encode call                                                    */
/*----------------------------------------------------------------------------*/
void Task::vCompileComma(NNP_Task nnpTask)
{
  ENTRY("vCompileComma");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("compile,");
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  TaskStep* pCode = pCalledWord->ppDoesStep();
  
  pCurrentWord->vEncode(pCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* compiling -- enter compiling mode                                          */
/*----------------------------------------------------------------------------*/
void Task::vCompiling(NNP_Task nnpTask)
{
ENTRY("vCompiling");
  
  nnpTask->m_bCompiling = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor@ -- fetch cursor from return stack onto param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCursorFetch(NNP_Task nnpTask)
{
ENTRY("vCursorFetch");
  
  i8 iPick = 1;
  
  ValWsp xCursorWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xCursorWsp);
  
  nnpTask->m_xParamStack.vPush(xCursorWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCursorStore(NNP_Task nnpTask)
{
ENTRY("vCursorStore");
  
  i8 iPeek = 1;
  
  ValWsp xCursorWsp;
  
  if ( ! nnpTask->m_xParamStack.bPop(xCursorWsp) )
    THROW_INVALID_PARAM("cursor!");
  
  nnpTask->m_xReturnStack.vPoke(iPeek,&xCursorWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeCall -- encode call                                                  */
/*----------------------------------------------------------------------------*/
void Task::vEncodeCall(NNP_Task nnpTask)
{
ENTRY("vEncodeCall");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("encodeCall");
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  TaskStep* pCode = pCalledWord->ppDoesStep();
  
  pCurrentWord->vEncode(pCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeDoes -- encode does part                                             */
/*----------------------------------------------------------------------------*/
void Task::vEncodeDoes(NNP_Task nnpTask)
{
ENTRY("vEncodeDoes");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vDoesStep(&Task::vEnterDoes);
  pCurrentWord->vDoesArg(nnpTask->IP.m_pCell+1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeEmb -- encode push ptr to object                                     */
/*----------------------------------------------------------------------------*/
void Task::vEncodeEmb(NNP_Task nnpTask)
{
ENTRY("vEncodeEmb");
  
  PtrVal xPtrObject;
  
  xPtrObject = nnpTask->m_pWord->pFirstCell();
  
  if (nnpTask->m_bCompiling) {
    Word* pCurrentWord = nnpTask->pCurrentWord();
    pCurrentWord->vEncode("push",&xPtrObject);
  }
  else {
    nnpTask->m_xParamStack.vPush(&xPtrObject);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeFun -- encode primitive fun                                          */
/*----------------------------------------------------------------------------*/
void Task::vEncodeFun(NNP_Task nnpTask)
{
ENTRY("vEncodeFun");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("vEncodeFun");
  Word* pCalledWord = _pWord(pWordObject);
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  TaskStep* pCode = pCalledWord->ppDoesStep();
  
  pCurrentWord->vAppend(pCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeVal -- encode push value                                             */
/*----------------------------------------------------------------------------*/
void Task::vEncodeVal(NNP_Task nnpTask)
{
ENTRY("vEncodeVal");
  
  ValWsp xValA;
  if ( ! nnpTask->m_xParamStack.bPop(xValA) )
    THROW_INVALID_PARAM("encodeVal");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("push",&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enterDoes -- enter this routine                                            */
/*----------------------------------------------------------------------------*/
void Task::vEnterDoes(NNP_Task nnpTask)
{
  ENTRY("vEnterDoes");
  
  nnpTask->m_xReturnStack.vPush(nnpTask->IP.m_pCell);
  
  Cell*    pPrevCell = nnpTask->IP.m_pCell - 1;
  
  TaskStep* pDoesStep = pPrevCell->m_ppDoesStep;
  
  Cell**   pDoesArg  = reinterpret_cast<Cell**>(pvAdd((void*)pDoesStep,sizeof(Cell*)));
  
  Word* pWord = reinterpret_cast<Word*>(pcAdd(pDoesArg, -(sizeof(Word)-sizeof(Cell*)) ));
  
  ASSERT(WT_OBJECT == pWord->eWordType());
  
  if (WT_OBJECT == pWord->eWordType()) {
    Cell* pCell = pWord->pFirstCell();
    nnpTask->m_xParamStack.vPush(pCell);
  }
  
  nnpTask->IP.m_pCell = *pDoesArg;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enterRoutine -- enter routine                                              */
/*----------------------------------------------------------------------------*/
void Task::vEnterRoutine(NNP_Task nnpTask)
{
ENTRY("vEnterRoutine");
  
  nnpTask->m_xReturnStack.vPush(nnpTask->IP.m_pCell);
  
  Cell*    pPrevCell = nnpTask->IP.m_pCell - 1;
  
  TaskStep* pDoesStep = pPrevCell->m_ppDoesStep;
  
  Cell**   pDoesArg  = reinterpret_cast<Cell**>(pvAdd((void*)pDoesStep,sizeof(Cell*)));
  
  nnpTask->IP.m_pCell = *pDoesArg;
  
  if ( (nnpTask->m_iTracing & C_TRACE_ROUTINES) )
    nnpTask->vTracePoint(">");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ] -- enter compiler state                                                  */
/*----------------------------------------------------------------------------*/
void Task::vEnterCompiler(NNP_Task nnpTask)
{
ENTRY("vEnterCompiler");
  
  nnpTask->m_bCompiling = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Task::vEnterInterpreter(NNP_Task nnpTask)
{
ENTRY("vEnterInterpreter");

  nnpTask->m_bCompiling = false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* env@ -- get environment variable                                           */
/*----------------------------------------------------------------------------*/
void Task::vEnvGet(NNP_Task nnpTask)
{
ENTRY("vEnvGet");
  
  s256 sEnvName;
  
  if ( ! nnpTask->m_xParamStack.bPop(sEnvName) )
    THROW_INVALID_PARAM("env@");
  
  s256 sEnvVal;
  
  if (! Env::bGet(sEnvName,sEnvVal))
    sEnvVal = "";
  
  nnpTask->m_xParamStack.vPush(sEnvVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* eql -- test top two address on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Task::vEqualAddress(NNP_Task nnpTask)
{
ENTRY("vEql");
  
  ValWsp xAddressA;
  ValWsp xAddressB;
  
  if ( ! nnpTask->m_xParamStack.bPop(xAddressA,xAddressB) )
    THROW_INVALID_PARAM("eql");
  
  i8 iA = xAddressA.iAsInt();
  i8 iB = xAddressB.iAsInt();
  
  i8 r = iA == iB;
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* execute -- execute word on param stack                                     */
/*----------------------------------------------------------------------------*/
void Task::vExecute(NNP_Task nnpTask)
{
ENTRY("vExecute");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("vExecute");
  Word* pWord = _pWord(pWordObject);
  
  if (WT_PRIMITIVE == pWord->eWordType()) {
    TaskStep pTaskStep = pWord->pDoesStep();
    (pTaskStep)(nnpTask);
  }
  else if (WT_OBJECT == pWord->eWordType()) {
    nnpTask->m_xReturnStack.vPush(nnpTask->IP.m_pCell);

    Cell* pCell = pWord->pFirstCell();
    nnpTask->m_xParamStack.vPush(pCell);
    
    TaskStep pTaskStep = pWord->pDoesStep();
    if (!pTaskStep || Task::vNop == pTaskStep) {
      void* pCellVoid;
      if ( ! nnpTask->m_xReturnStack.bPop(pCellVoid) )
        THROW_INVALID_RETURN("execute");
      nnpTask->IP.m_pCell = _pCell(pCellVoid);
    }
    else {
      // does> inherited code
      nnpTask->IP.m_pCell = pWord->pDoesArg();
    }
  }
  else {
    nnpTask->vRoutineCall(pWord);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* exit -- exit from routine                                                  */
/*----------------------------------------------------------------------------*/
void Task::vExit(NNP_Task nnpTask)
{
ENTRY("vExit");
  
  //vTrace("Exit: return stack %lld\n", nnpTask->m_xReturnStack.iValues());
  
  if ( (nnpTask->m_iTracing & C_TRACE_ROUTINES) )
    nnpTask->vTracePoint("<");
  
  if (nnpTask->m_pLocalsFrame &&
        nnpTask->m_pLocalsFrame->pStackPtr() ==
         (void*)nnpTask->m_xReturnStack.pTopValue())
    nnpTask->vLocalsPop();
  
  void* pCellVoid;
  if ( ! nnpTask->m_xReturnStack.bPop(pCellVoid) )
    THROW_INVALID_RETURN("exit");
  
  nnpTask->IP.m_pCell = _pCell(pCellVoid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* @ -- fetch value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vFetch(NNP_Task nnpTask)
{
  ENTRY("vFetch");
  
  Object* pObject;
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("@");
  
  ValWsp xValFetched;
  
  eCN eClass = pObject->eClass();
  
  Value* pValue = reinterpret_cast<Value*>(pObject);
  
  switch (eClass) {
    case CN_INTVAL:
      xValFetched = pValue->pAsIntVal()->iInt();
      break;
      
    case CN_STRVAL:
      xValFetched = pValue->pAsStrVal()->sStr();
      break;
      
    case CN_PTRVAL:
      xValFetched = pValue->pAsPtrVal()->pPtr();
      break;
      
    default:
      THROW("Invalid eClass %lld",eClass);
  }
  
  nnpTask->m_xParamStack.vPush(xValFetched);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* freeBuffers -- push number of free buffers in buffer pool onto stack       */
/*----------------------------------------------------------------------------*/
void Task::vFreeBuffers(NNP_Task nnpTask)
{
  ENTRY("vFreePercent");
  
  i8 iFreeBuffers = gpBufferPool->iFreeCount();
  
  nnpTask->m_xParamStack.vPush(iFreeBuffers);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* freePercent -- push free percent of buffer pool onto stack                 */
/*----------------------------------------------------------------------------*/
void Task::vFreePercent(NNP_Task nnpTask)
{
ENTRY("vFreePercent");
  
  i8 iFreePercent = gpBufferPool->iFreePercent();
  
  nnpTask->m_xParamStack.vPush(iFreePercent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* freeSpace -- push free space in buffer pool onto stack                     */
/*----------------------------------------------------------------------------*/
void Task::vFreeSpace(NNP_Task nnpTask)
{
ENTRY("vFreeSpace");
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  
  nnpTask->m_xParamStack.vPush(iFreeSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hash -- get hash of string                                                 */
/*----------------------------------------------------------------------------*/
void Task::vHash(NNP_Task nnpTask)
{
ENTRY("vHash");
  
  s8K sString;
  if ( ! nnpTask->m_xParamStack.bPop(sString) )
    THROW_INVALID_PARAM("hash");
  
  i8 iLength = String::iLength(sString.a());
  
  i8 iHash = XXH32(sString.a(),iLength,0);
  
  nnpTask->m_xParamStack.vPush(iHash);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hazard -- should never be called                                           */
/*----------------------------------------------------------------------------*/
void Task::vHazard(NNP_Task nnpTask)
{
ENTRY("vHazard");
  
  THROW("Entered hazard");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* i -- copy loop index from return stack onto param stack                    */
/*----------------------------------------------------------------------------*/
void Task::vI(NNP_Task nnpTask)
{
ENTRY("vI");
  
  Value* pI = nnpTask->m_xReturnStack.pPeek();
  
  nnpTask->m_xParamStack.vPush(pI);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the most recent definition an immediate word             */
/*----------------------------------------------------------------------------*/
void Task::vImmediate(NNP_Task nnpTask)
{
ENTRY("vImmediate");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  //vTrace("Task::vImmediate: %s\n",pCurrentWord->sName().a());
  
  pCurrentWord->vImmediate(true);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* interpreting -- enter interpreting mode                                    */
/*----------------------------------------------------------------------------*/
void Task::vInterpreting(NNP_Task nnpTask)
{
ENTRY("vInterpreting");
  
  nnpTask->m_bCompiling = false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isBooted -- has system booted?                                             */
/*----------------------------------------------------------------------------*/
void Task::vIsBooted(NNP_Task nnpTask)
{
ENTRY("vIsBooted");
  
  Word* pWord = nnpTask->pFindWord("__compiled__");
  
  i8 bIsBooted = pWord ? true : false;
  
  nnpTask->m_xParamStack.vPush(bIsBooted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isCompiling -- currently in compiling mode?                                */
/*----------------------------------------------------------------------------*/
void Task::vIsCompiling(NNP_Task nnpTask)
{
ENTRY("vIsCompiling");
  
  i8 iIsCompiling = nnpTask->m_bCompiling ? 1 : 0;
  
  nnpTask->m_xParamStack.vPush(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Task::vIsImmediate(NNP_Task nnpTask)
{
ENTRY("vIsImmediate");
  
  // we should just modify the pointers rather than actually pop
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("isImmediate");
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  nnpTask->m_xParamStack.vPush(iIsImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isInterpreting -- currently in interpreting mode?                          */
/*----------------------------------------------------------------------------*/
void Task::vIsInterpreting(NNP_Task nnpTask)
{
ENTRY("vIsInterpreting");
  
  i8 iIsInterpreting = nnpTask->m_bCompiling ? 0 : 1;
  
  nnpTask->m_xParamStack.vPush(iIsInterpreting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isPrimitive -- is word a primitive?                                        */
/*----------------------------------------------------------------------------*/
void Task::vIsPrimitive(NNP_Task nnpTask)
{
ENTRY("vIsPrimitive");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("isPrimitive");
  Word* pWord = _pWord(pWordObject);
  
  i8 iIsPrimitive = WT_PRIMITIVE == pWord->eWordType() ? 1 : 0;
  
  nnpTask->m_xParamStack.vPush(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* inUseSpace -- push in-use space in buffer pool onto stack                  */
/*----------------------------------------------------------------------------*/
void Task::vInUseSpace(NNP_Task nnpTask)
{
ENTRY("vInUseSpace");
  
  i8 iInUseSpace = gpBufferPool->iRegionSize() - gpBufferPool->iFreeSpace();
  
  nnpTask->m_xParamStack.vPush(iInUseSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vJ(NNP_Task nnpTask)
{
ENTRY("vJ");
  
  Value* pI = nnpTask->m_xReturnStack.pPeek(3);
  
  nnpTask->m_xParamStack.vPush(pI);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locate -- locate word based on ip                                          */
/*----------------------------------------------------------------------------*/
void Task::vLocate(NNP_Task nnpTask)
{
ENTRY("vLocate");
  
  i8 iAsInt;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iAsInt) )
    THROW_INVALID_PARAM("locate");
  
  void* pCellVoid = (void*)iAsInt;
  Cell* pCell = _pCell(pCellVoid);
  
  Word* pLocatedWord = nnpTask->pLocateWord(pCell);
  
  if (pLocatedWord) {
    s32 sName;
    sName = pLocatedWord->sName();
    nnpTask->m_xParamStack.vPush(sName);
  }
  else {
    i8 iZero = 0;
    nnpTask->m_xParamStack.vPush(iZero);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* magicFetch -- fetch magic number from Object object                        */
/*----------------------------------------------------------------------------*/
void Task::vMagicFetch(NNP_Task nnpTask)
{
ENTRY("vMagicFetch");

  Object* pObject;
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_RETURN("magicFetch");
  
  i8 iMagic = pObject->iMagic();

  nnpTask->m_xParamStack.vPush(iMagic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* magicMark -- put magic number into Object object                           */
/*----------------------------------------------------------------------------*/
void Task::vMagicMark(NNP_Task nnpTask)
{
ENTRY("vMagicMark");
  
  Object* pObject;
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_RETURN("magicMark");
  
  pObject->vSetMagic();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nameWord -- push name of word                                              */
/*----------------------------------------------------------------------------*/
void Task::vNameWord(NNP_Task nnpTask)
{
ENTRY("vNameWord");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("nameWord");
  Word* pWord = _pWord(pWordObject);
  
  nnpTask->m_xParamStack.vPush(pWord->sName().a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nop -- no op                                                               */
/*----------------------------------------------------------------------------*/
void Task::vNop(NNP_Task nnpTask)
{
ENTRY("vNop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nullPtr -- push a nullptr                                                  */
/*----------------------------------------------------------------------------*/
void Task::vNullPtr(NNP_Task nnpTask)
{
  ENTRY("vNullPtr");
  
  PtrVal xPtrVal;
  xPtrVal = nullptr;
  
  nnpTask->m_xParamStack.vPush(xPtrVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object@ -- fetch object from return stack onto param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vObjectFetch(NNP_Task nnpTask)
{
ENTRY("vObjectFetch");
  
  i8 iPick = 0;
  
  ValWsp xObjectWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xObjectWsp);
  
  nnpTask->m_xParamStack.vPush(xObjectWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object! -- replace object on return stack from param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vObjectStore(NNP_Task nnpTask)
{
ENTRY("vObjectStore");
  
  i8 iPeek = 0;
  
  ValWsp xObjectWsp;
  
  if ( ! nnpTask->m_xParamStack.bPop(xObjectWsp) )
    THROW_INVALID_PARAM("object!");
  
  nnpTask->m_xReturnStack.vPoke(iPeek,&xObjectWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object!! -- mark current word as an object                                 */
/*----------------------------------------------------------------------------*/
void Task::vObjectMark(NNP_Task nnpTask)
{
ENTRY("vObjectMark");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  //pCurrentWord->vImmediate(true);
  pCurrentWord->vWordType(WT_OBJECT);
  pCurrentWord->vDoesStep(&vEncodeEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prevWord -- push prev word                                                 */
/*----------------------------------------------------------------------------*/
void Task::vPrevWord(NNP_Task nnpTask)
{
ENTRY("vPrevWord");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("prevWord");
  Word* pWord = _pWord(pWordObject);
  
  Word* pPrevWord = pWord->pPrevWord();
  
  nnpTask->m_xParamStack.vPush(pPrevWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* . -- print top value on stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vPrint(NNP_Task nnpTask)
{
ENTRY("vPrint");
  
  s8K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_xParamStack.bPop(xValWsp) )
    THROW_INVALID_PARAM(".");
  
  xValWsp.vToString(sWorkspace.a());
  vTrace("[N%lld:%8s\\%lld %-11s] %s\n",
         
         gxArguments.m_iNode,
         nnpTask->m_sName.a(),
         *nnpTask->m_zAcks,
         nnpTask->m_sComment.a(),
         
         sWorkspace.a()
         );
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* processQuit -- process exits early                                         */
/*----------------------------------------------------------------------------*/
void Task::vProcessQuit(NNP_Task nnpTask)
{
ENTRY("vProcessQuit");
  
  exit(EXIT_FAILURE);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* push -- push value arg onto stack                                          */
/*----------------------------------------------------------------------------*/
void Task::vPush(NNP_Task nnpTask)
{ 
ENTRY("vPush");
  
  Value* pValue = nnpTask->IP.m_pValue;
  
  nnpTask->m_xParamStack.vPush(pValue);
  
  nnpTask->vAdjustNext(pValue->iSize());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushEmbedded -- push ptr to embedded object                                */
/*----------------------------------------------------------------------------*/
void Task::vPushEmbedded(NNP_Task nnpTask)
{
ENTRY("vPushEmbedded");
  
  Object* pPtrObject = nnpTask->IP.m_pObject;
  
  nnpTask->m_xParamStack.vPush(pPtrObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .put -- print top value on stack (no new line)                             */
/*----------------------------------------------------------------------------*/
void Task::vPut(NNP_Task nnpTask)
{
ENTRY("vPut");
  
  s8K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_xParamStack.bPop(xValWsp) )
    THROW_INVALID_PARAM(".put");
  
  xValWsp.vToString(sWorkspace.a());
  vTrace("%s",sWorkspace.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .putVerbose -- print top value on stack if arg.m_bverbose                  */
/*----------------------------------------------------------------------------*/
void Task::vPutVerbose(NNP_Task nnpTask)
{
  ENTRY("vPutVerbose");
  
  s8K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_xParamStack.bPop(xValWsp) )
    THROW_INVALID_PARAM(".put");
  
  if (gxArguments.m_bVerbose) {
    xValWsp.vToString(sWorkspace.a());
    vTrace("[%p %s] %s\n",*nnpTask,nnpTask->m_sName.a(),sWorkspace.a());
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* .putNewline -- print new line                                              */
/*----------------------------------------------------------------------------*/
void Task::vPutNewline(NNP_Task nnpTask)
{
ENTRY("vPut");
  
  vTrace("\n");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdtscp -- get the time                                                     */
/*----------------------------------------------------------------------------*/
void Task::vRdtscp(NNP_Task nnpTask)
{
ENTRY("vRdtscp");
  
  i8 iTscp = Synch::iTicks();

  nnpTask->m_xParamStack.vPush(iTscp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* retryPoint -- mark retry point                                             */
/*----------------------------------------------------------------------------*/
void Task::vRetryPoint(NNP_Task nnpTask)
{
ENTRY("vRetryMark");
  
  nnpTask->m_pResetIP = nnpTask->IP.m_pCell;
  
  nnpTask->m_xParamStack.vRetryPoint();
  nnpTask->m_xReturnStack.vRetryPoint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* see -- dump routine                                                        */
/*----------------------------------------------------------------------------*/
void Task::vSee(NNP_Task nnpTask)
{
ENTRY("vSee");
  
  nnpTask->vTokenNext();
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_sToken);

  if ( ! pWord )
    THROW("See: %s unknown",nnpTask->m_sToken.a());
  
  vTrace("See: %s\n",pWord->sName().a());
  
  Cell* pCell = pWord->pDoesArg();
  
  s8K sWorkspace;
  
//  while ((char*)pCell < pcAdd(pWord->pDataArea(),pWord->iDataBytes())) {
  while (pCell < pWord->pLastCell()) {
 
    Word* pDoesWord = nnpTask->m_xDicStack.pFindStep(*pCell->m_ppDoesStep);

    vTrace("%p: %s",pCell,pDoesWord->sName().a());
  
    if ( *pCell->m_ppDoesStep == Task::vFlowJump ) {
      pCell++;
      vTrace(" %p\n",pCell->m_pJumpTo);
      pCell++;
    }
    else
    if ( *pCell->m_ppDoesStep == Task::vFlowJumpFalse ) {
      pCell++;
      vTrace(" %p\n",pCell->m_pJumpTo);
      pCell++;
    }
    else
    if ( *pCell->m_ppDoesStep == Task::vFlowJumpTrue ) {
      pCell++;
      vTrace(" %p\n",pCell->m_pJumpTo);
      pCell++;
    }
    else
    if ( *pCell->m_ppDoesStep == Task::vPush ) {
      pCell++;
      ValWsp* pValue = reinterpret_cast<ValWsp*>(pCell);
      pValue->vToString(sWorkspace.a());
      vTrace(" %s\n",sWorkspace.a());
      i8 iBytes = pValue->iSize();
      pCell = reinterpret_cast<Cell*>(pcAdd(pCell,iBytes));
    }
    else {
      vTrace("\n");
      pCell++;
    }
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ; -- end compile routine                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSemicolon(NNP_Task nnpTask)
{
ENTRY("vSemicolon");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();

  pCurrentWord->vEncode("exit");
  
  nnpTask->m_bCompiling = false;
  
  if (nnpTask->m_nLocals) {
    
    // reset the number of local variables
    nnpTask->m_nLocals = 0;
    
    // pop top dictionary
    nnpTask->vDicDrop();
  }
  
  nnpTask->vCurrentWord(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sleep -- sleep for n seconds                                               */
/*----------------------------------------------------------------------------*/
void Task::vSleep(NNP_Task nnpTask)
{
ENTRY("vSleep");
  
  i8 nSeconds;
  
  if ( ! nnpTask->m_xParamStack.bPop(nSeconds) )
    THROW_INVALID_PARAM("sleep");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepSec(nSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* snooze -- snooze briefly                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSnooze(NNP_Task nnpTask)
{
ENTRY("vSnooze");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepNano(100_MB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sprint -- pop format string and use it for a sprintf                       */
/*----------------------------------------------------------------------------*/
void Task::vSprint(NNP_Task nnpTask)
{
  ENTRY("vSprint");
  
  s8K sFormat;
  if ( ! nnpTask->m_xParamStack.bPop(sFormat) )
    THROW_INVALID_PARAM("sprint");
  
  s8K sString;
  
  nnpTask->vSprint(sFormat,sString);
  
  nnpTask->m_xParamStack.vPush(sString);
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ! -- store value                                                           */
/*----------------------------------------------------------------------------*/
void Task::vStore(NNP_Task nnpTask)
{
  ENTRY("vStore");
  
  Object* pObject;
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("!");
  
  ValWsp xValStored;
  if ( ! nnpTask->m_xParamStack.bPop(xValStored) )
    THROW_INVALID_PARAM("!");
  
  Value* pValue = reinterpret_cast<Value*>(pObject);
  
  eCN eClass = pObject->eClass();
  
  switch (eClass) {
    case CN_INTVAL:
      *pValue->pAsIntVal() = xValStored.iInt();
      break;
      
    case CN_STRVAL:
      *pValue->pAsStrVal() = xValStored.sStr();
      break;
      
    case CN_PTRVAL:
      *pValue->pAsPtrVal() = xValStored.pPtr();
      break;
      
    default:
      THROW("Invalid eClass %lld",eClass);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* str: -- string colon                                                       */
/*----------------------------------------------------------------------------*/
void Task::vStringColon(NNP_Task nnpTask)
{
ENTRY("vStringColon");
  
  nnpTask->vTokenNext();
  
  nnpTask->m_xParamStack.vPush(nnpTask->m_sToken);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* test -- print test                                                         */
/*----------------------------------------------------------------------------*/
void Task::vTest(NNP_Task nnpTask)
{
ENTRY("vTest");
  vTrace("[%s] Test\n",nnpTask->m_sName.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* throw -- throw c++ exception (for testing purposes)                        */
/*----------------------------------------------------------------------------*/
void Task::vThrow(NNP_Task nnpTask)
{
ENTRY("vThrow");
  
  char wBuffer[256];
  
  String::vSprint(wBuffer,"THROW TEST");
  
  throw wBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tick -- find next word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTick(NNP_Task nnpTask)
{
ENTRY("vTick");
  
  nnpTask->vTokenNext();
  
//  if ( ! bExtracted ) {
//    String::vSprint(nnpTask->m_sException,"No name: ' ");
//    nnpTask->m_sFile = __FILE__;
//    throw (i8)__LINE__;
//  }
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_sToken);

  if ( ! pWord )
    THROW("No word: %s ",nnpTask->m_sToken.a());

  if (nnpTask->m_bCompiling) {
    Word* pCurrentWord = nnpTask->pCurrentWord();
    pCurrentWord->vEncode("push",pWord);
  }
  else {
    nnpTask->m_xParamStack.vPush(pWord);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >body -- convert word pointer to body pointer                              */
/*----------------------------------------------------------------------------*/
void Task::vToBody(NNP_Task nnpTask)
{
ENTRY("vToBody");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM(">body");
  Word* pWord = _pWord(pWordObject);

  char* pBody = pcAdd(pWord,sizeof(Word));
  
  Object* pPtrBody = reinterpret_cast<Object*>(pBody);

  nnpTask->m_xParamStack.vPush(pPtrBody);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >cmd -- convert stack values to cmd string                                 */
/*----------------------------------------------------------------------------*/
void Task::vToCmd(NNP_Task nnpTask)
{
ENTRY("vToCmd");

  i8 nValues;
  if ( ! nnpTask->m_xParamStack.bPop(nValues) )
    THROW_INVALID_PARAM(">cmd");

  s8K sCmd;

  ValWsp xStr;
  xStr = "<cmd>";
  
  for (i8 i = 0; i < nValues; i++) {
    ValWsp xValWsp;
    nnpTask->m_xParamStack.vPeek(i,&xValWsp);

    if (i)
      String::vConcat(sCmd.a()," ");
    
    xValWsp.vToCommand(sCmd.z());
  }
  
  xStr = sCmd.a();
  
  nnpTask->m_xParamStack.vPush(&xStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Task::vToInt(NNP_Task nnpTask)
{
ENTRY("vToInt");
  
  ValWsp xStr;
  if ( ! nnpTask->m_xParamStack.bPop(xStr) )
    THROW_INVALID_PARAM(">int");
  
  s256 sBuff;
  xStr.vToString(sBuff.a());
  
  i8 i = String::iToI8(sBuff.a());

  nnpTask->m_xParamStack.vPush(i);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >seitf -- convert int to seitf ptr                                         */
/*----------------------------------------------------------------------------*/
void Task::vToSEitf(NNP_Task nnpTask)
{
ENTRY("vToSEitf");
  
  i8 iVal;
  
  if ( ! nnpTask->m_xParamStack.bPop(iVal) )
    THROW_INVALID_PARAM(">seitf");
  
  Object* pPtrSEitf = _pSEitf(iVal);
  
  nnpTask->m_xParamStack.vPush(pPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >str -- convert value to str                                               */
/*----------------------------------------------------------------------------*/
void Task::vToStr(NNP_Task nnpTask)
{
ENTRY("vToStr");
  
  ValWsp xVal;
  if ( ! nnpTask->m_xParamStack.bPop(xVal) )
    THROW_INVALID_PARAM(">str");

  s8K sString;
  xVal.vToString(sString.a());
  
  nnpTask->m_xParamStack.vPush(sString);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TotalSpace -- push total space in buffer pool onto stack                   */
/*----------------------------------------------------------------------------*/
void Task::vTotalSpace(NNP_Task nnpTask)
{
ENTRY("vTotalSpace");
  
  i8 iTotalSpace = gpBufferPool->iRegionSize();
  
  nnpTask->m_xParamStack.vPush(iTotalSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >task -- convert int to task ptr                                           */
/*----------------------------------------------------------------------------*/
void Task::vToTask(NNP_Task nnpTask)
{
ENTRY("vToTask");
  
  i8 iVal;
  if ( ! nnpTask->m_xParamStack.bPop(iVal) )
    THROW_INVALID_PARAM(">task");
  
  Object* pPtrTask = _pTask(iVal);
  
  nnpTask->m_xParamStack.vPush(pPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing off                                                           */
/*----------------------------------------------------------------------------*/
void Task::vTraceOff(NNP_Task nnpTask)
{
ENTRY("vTraceOff");
  
  nnpTask->m_iTracing = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing on                                                            */
/*----------------------------------------------------------------------------*/
void Task::vTraceOn(NNP_Task nnpTask)
{
ENTRY("vTraceOn");
  
  nnpTask->m_iTracing = C_TRACE_STEPS;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* trace point                                                                */
/*----------------------------------------------------------------------------*/
void Task::vTracePoint(NNP_Task nnpTask)
{
  ENTRY("vTracePoint");
  
  s256 sPointName;
  
  if ( ! nnpTask->m_xParamStack.bPop(sPointName) )
    THROW_INVALID_PARAM("tracePoint");
  
  if (gxArguments.m_bTracepoint)
    nnpTask->vTracePoint(sPointName.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* set tracing                                                                */
/*----------------------------------------------------------------------------*/
void Task::vTraceSet(NNP_Task nnpTask)
{
  ENTRY("vTraceSet");
  
  i8 iTrace;
  
  if ( ! nnpTask->m_xParamStack.bPop(iTrace) )
    THROW_INVALID_PARAM("trace!");
  
  nnpTask->m_iTracing = iTrace;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* wake -- wake task on stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vWake(NNP_Task nnpTask)
{
ENTRY("vWake");
  
  // ..tbd..
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// assert: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* assert -- assert top value on stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vAssert(NNP_Task nnpTask)
{
  ENTRY("vAssert");
  
  i8 iTrue;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iTrue) )
    THROW_INVALID_PARAM("assert");
  
  if ( ! iTrue )
    THROW("assert");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_0 -- asssert 0 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_0(NNP_Task nnpTask)
{
ENTRY("vAssertStack_0");

  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (0 == iStackValues)  )
    THROW("assert.stack_0");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_0+ -- asssert 0 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_0more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_0more");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack0+");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_1 -- asssert 1 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_1(NNP_Task nnpTask)
{
ENTRY("vAssertStack_1");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (1 == iStackValues)  )
    THROW("assert.stack_1");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_1+ -- asssert 0 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_1more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_1more");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (1 <= iStackValues) )
    THROW("assert.stack1+");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_2 -- asssert 2 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_2(NNP_Task nnpTask)
{
ENTRY("vAssertStack_2");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (2 == iStackValues)  )
    THROW("assert.stack_2");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_2+ -- asssert 2 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_2more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_2more");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (2 <= iStackValues) )
    THROW("assert.stack0+");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_3 -- asssert 3 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_3(NNP_Task nnpTask)
{
ENTRY("vAssertStack_3");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (3 == iStackValues)  )
    THROW("assert.stack_3");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_3+ -- asssert 3 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_3more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_3more");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack3+");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_4 -- asssert 4 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_4(NNP_Task nnpTask)
{
ENTRY("vAssertStack_4");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (4 == iStackValues)  )
    THROW("assert.stack_4");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* assert.stack_4+ -- asssert 4 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_4more(NNP_Task nnpTask)
{
ENTRY("vAssertStack_4more");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack4+");
  
  EXIT();
}

// astack: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* adepth -- put number of values in aux stack onto param stack               */
/*----------------------------------------------------------------------------*/
void Task::vAstackDepth(NNP_Task nnpTask)
{
  ENTRY("vAstackDepth");
  
  i8 iDepth = nnpTask->m_xAuxStack.iValues();
  
  nnpTask->m_xParamStack.vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* adrop -- discard top value off astack                                      */
/*----------------------------------------------------------------------------*/
void Task::vAstackDrop(NNP_Task nnpTask)
{
  ENTRY("vAstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xAuxStack.bPop(xValA) )
    THROW_INVALID_RETURN("adrop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of aux stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vAstackDup(NNP_Task nnpTask)
{
  ENTRY("vAstackDup");
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xParamStack.bPop(xValA) )
  THROW_INVALID_PARAM("adup");
  
  nnpTask->m_xAuxStack.vPush(xValA);
  nnpTask->m_xAuxStack.vPush(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a@ -- place copy of top value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstackFetch(NNP_Task nnpTask)
{
  ENTRY("vAstackFetch");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(0,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the aux stack ( B A -- A )                  */
/*----------------------------------------------------------------------------*/
void Task::vAstackNip(NNP_Task nnpTask)
{
  ENTRY("vAstackNip");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xAuxStack.bPop(xValA,xValB) )
  THROW_INVALID_PARAM("anip");
  
  nnpTask->m_xAuxStack.vPush(&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apick -- place copy of nth value in aux stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vAstackPick(NNP_Task nnpTask)
{
  ENTRY("vAstackPick");
  
  i8 iPick;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iPick) )
    THROW_INVALID_PARAM("apick");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(iPick,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* areverse -- reverse astack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Task::vAstackReverse(NNP_Task nnpTask)
{
  ENTRY("vAstackReverse");
  
  ValWsp xVal0;
  ValWsp xVal1;
  ValWsp xVal2;
  ValWsp xVal3;
  ValWsp xVal4;
  ValWsp xVal5;
  ValWsp xVal6;
  ValWsp xVal7;
  
  ValWsp* apValWsp[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! nnpTask->m_xParamStack.bPop(nValues) )
    THROW_INVALID_PARAM("areverse");
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! nnpTask->m_xAuxStack.bPop(apValWsp[i]) )
      THROW_INVALID_PARAM("areverse");
  }
  
  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_xAuxStack.vPush(apValWsp[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on aux stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vAstackSwap(NNP_Task nnpTask)
{
  ENTRY("vAstackSwap");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xAuxStack.bPop(xValA,xValB) )
  THROW_INVALID_PARAM("aswap");
  
  nnpTask->m_xAuxStack.vPush(&xValA);
  nnpTask->m_xAuxStack.vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >avalset -- collapse values on aux stack to a valset                       */
/*----------------------------------------------------------------------------*/
void Task::vAstackToValSet(NNP_Task nnpTask)
{
  ENTRY("vAstackToValSet");
  
  i8 nVal;
  
  if ( ! nnpTask->m_xAuxStack.bPop(nVal) )
  THROW_INVALID_PARAM(">avalset");
  
  if ( ! nnpTask->m_xAuxStack.bGuard(nVal) )
  THROW_INVALID_PARAM(">avalset");
  
  char* pCursor = nullptr;
  ValSet xValSet;
  
  ValWsp xVal;
  for (i8 iVal = 0; iVal < nVal; iVal++) {
    
    if ( ! nnpTask->m_xAuxStack.bPop(xVal) )
    THROW_INVALID_PARAM(">avalset");
    
    xValSet.vAppend(pCursor,xVal);
  }
  
  //  vTrace("ValSet %lld bytes\n",xValSet.iSize());
  
  nnpTask->m_xAuxStack.vPush(xValSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >avalues -- expand valset to values on aux stack                           */
/*----------------------------------------------------------------------------*/
void Task::vAstackToValues(NNP_Task nnpTask)
{
  ENTRY("vPstackToValues");
  
  ValSet xValSet;
  
  if ( ! nnpTask->m_xAuxStack.bPop(xValSet) )
    THROW_INVALID_PARAM(">avalues");
  
  i8 nVal = 0;
  ValWsp* pValWsp[128];
  
  char* pCursor = nullptr;
  while (xValSet.bExtract(pCursor,pValWsp[nVal]))
    nVal++;
  
  for (i8 iVal = nVal - 1; iVal >= 0; iVal--)
  nnpTask->m_xAuxStack.vPush(pValWsp[iVal]);
  
  nnpTask->m_xAuxStack.vPush(nVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >a -- move value from pstack to astack                                     */
/*----------------------------------------------------------------------------*/
void Task::vAstackToA(NNP_Task nnpTask)
{
  ENTRY("vRstackToR");
  
  ValWsp xValOpaque;
  if ( ! nnpTask->m_xParamStack.bPop(xValOpaque) )
    THROW_INVALID_PARAM(">r");
  
  nnpTask->m_xAuxStack.vPush(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a> -- move value from astack to pstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vAstackToP(NNP_Task nnpTask)
{
  ENTRY("vAstackToP");
  
  ValWsp xVal;
  if ( ! nnpTask->m_xAuxStack.bPop(xVal) )
    THROW_INVALID_PARAM("a>");
  
  nnpTask->m_xParamStack.vPush(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >auxStack -- pop stack of this task and push onto aux stack of that task   */
/*----------------------------------------------------------------------------*/
void Task::vAstackTransfer(NNP_Task nnpTask)
{
  ENTRY("vAstackTransfer");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTask) )
    THROW_INVALID_PARAM(">auxStack");
  Task* pThatTask = _pTask(pPtrTask);
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_xParamStack.bPop(xValWsp) )
    THROW_INVALID_PARAM(">auxStack");
  
  pThatTask->m_xLock.vGrabWrite();
  pThatTask->m_xAuxStack.vPush(xValWsp);
  pThatTask->m_xLock.vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a0 -- place copy of 0th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack0(NNP_Task nnpTask)
{
  ENTRY("vAstack0");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(0,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* a1 -- place copy of 1st value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack1(NNP_Task nnpTask)
{
  ENTRY("vAstack1");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(1,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack2(NNP_Task nnpTask)
{
  ENTRY("vAstack2");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(2,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack3(NNP_Task nnpTask)
{
  ENTRY("vAstack3");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(3,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack4(NNP_Task nnpTask)
{
  ENTRY("vAstack4");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(4,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack5(NNP_Task nnpTask)
{
  ENTRY("vAstack5");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(5,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack6(NNP_Task nnpTask)
{
  ENTRY("vAstack6");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(6,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack7(NNP_Task nnpTask)
{
  ENTRY("vAstack7");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(7,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack8(NNP_Task nnpTask)
{
  ENTRY("vAstack8");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(8,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in aux stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vAstack9(NNP_Task nnpTask)
{
  ENTRY("vAstack9");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(9,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

// args: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* args.partition -- get the speficic partition from arguments                */
/*----------------------------------------------------------------------------*/
void Task::vArgsPartition(NNP_Task nnpTask)
{
ENTRY("vArgsPartition");
  
  nnpTask->m_xParamStack.vPush(gxArguments.m_iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* args.print -- print arguments                                              */
/*----------------------------------------------------------------------------*/
void Task::vArgsPrint(NNP_Task nnpTask)
{
ENTRY("vArgsPrint");
  
  gxArguments.vPrint();
  
  EXIT();
}

// class: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* class.name -- translate class no to name                                   */
/*----------------------------------------------------------------------------*/
void Task::vClassName(NNP_Task nnpTask)
{
  ENTRY("vClassName");
  
  i8 iClassNo;
  if ( ! nnpTask->m_xParamStack.bPop(iClassNo) )
    THROW_INVALID_PARAM("class.name");
  
  s32 sClassName;
  
  sClassName = pCN[iClassNo];
  
  nnpTask->m_xParamStack.vPush(sClassName);
  
  EXIT();
}

// csv: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* csv.firstValue -- get first value from csv string                          */
/*----------------------------------------------------------------------------*/
void Task::vCsvFirstValue(NNP_Task nnpTask)
{
ENTRY("vCsvFirstValue");
  
  ValWsp xStr;
  if ( ! nnpTask->m_xParamStack.bPop(xStr) )
    THROW_INVALID_PARAM("csv.firstValue");
  
  s256 sFirstValue;
  
  char* z = xStr.sStr();
  
  if (! String::bExtractItem(&z,sFirstValue.a()))
    sFirstValue = "";
  
  nnpTask->m_xParamStack.vPush(sFirstValue);
  
  EXIT();
}

// diclock: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* diclLock.dropWrite -- drop dic lock for write                               */
/*----------------------------------------------------------------------------*/
void Task::vDicLockDropWrite(NNP_Task nnpTask)
{
  ENTRY("vDicLockDropWrite");
  
  gxDicLock.vDropWrite();
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicLock.grabWrite -- grab dic lock for write                               */
/*----------------------------------------------------------------------------*/
void Task::vDicLockGrabWrite(NNP_Task nnpTask)
{
  ENTRY("vDicLockGrabWrite");
  
  gxDicLock.vGrabWrite();
    
  EXIT();
}

// ddllock: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* ddlLock.dropRead -- drop ddl lock for read                                 */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockDropRead(NNP_Task nnpTask)
{
  ENTRY("vDdlLockDropRead");
  
  ASSERT(C_HAVE_READ_LOCK == nnpTask->m_iDdlLocked);
  
  gxDdlLock.vDropRead();
  
  nnpTask->m_iDdlLocked = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.dropWrite -- drop ddl lock for write                               */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockDropWrite(NNP_Task nnpTask)
{
  ENTRY("vDdlLockDropWrite");
  
  ASSERT(C_HAVE_WRITE_LOCK == nnpTask->m_iDdlLocked);
  
  gxDdlLock.vDropWrite();
  
  nnpTask->m_iDdlLocked = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.grabRead -- grab ddl lock for read                                 */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockGrabRead(NNP_Task nnpTask)
{
  ENTRY("vDdlLockGrabRead");
  
  ASSERT(! nnpTask->m_iDdlLocked);
  
  gxDdlLock.vGrabRead();
  
  nnpTask->m_iDdlLocked = C_HAVE_READ_LOCK;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.grabWrite -- grab ddl lock for write                               */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockGrabWrite(NNP_Task nnpTask)
{
  ENTRY("vDdlLockGrabWrite");
  
  ASSERT(! nnpTask->m_iDdlLocked);

  gxDdlLock.vGrabWrite();
  
  nnpTask->m_iDdlLocked = C_HAVE_WRITE_LOCK;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ddlLock.testDrop -- test for ddl lock and drop if locked by task           */
/*----------------------------------------------------------------------------*/
void Task::vDdlLockTestDrop(NNP_Task nnpTask)
{
  ENTRY("vDdlLockTestDrop");
  
  if (C_HAVE_READ_LOCK == nnpTask->m_iDdlLocked) {
    gxDdlLock.vDropRead();
    nnpTask->m_iDdlLocked = 0;
  }
  else
  if (C_HAVE_WRITE_LOCK == nnpTask->m_iDdlLocked) {
    gxDdlLock.vDropWrite();
    nnpTask->m_iDdlLocked = 0;
  }
  
  EXIT();
}

// debug: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* debug.trap -- call debug trap                                              */
/*----------------------------------------------------------------------------*/
void Task::vDebugTrap(NNP_Task nnpTask)
{
ENTRY("vDebugTrap");

  Common::vDebugTrap();
  
  EXIT();
}

// dic: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* dic.count -- get number of dictionaries on dictionary stack                */
/*----------------------------------------------------------------------------*/
void Task::vDicCount(NNP_Task nnpTask)
{
ENTRY("vDicCount");
  
  i8 iCount;
  
  if (nnpTask->m_pSEitf) {
    iCount = nnpTask->m_pSEitf->pDicStack()->iCount();
  }
  else {
    iCount = nnpTask->m_xDicStack.iCount();
  }
  
  nnpTask->m_xParamStack.vPush(iCount);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.drop -- discard top of dictionary stack                                */
/*----------------------------------------------------------------------------*/
void Task::vDicDrop(NNP_Task nnpTask)
{
ENTRY("vDicDrop");
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->vPopDic();
  }
  else {
    nnpTask->m_xDicStack.vDiscard();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.frozen@ -- get dictionary frozen?                                      */
/*----------------------------------------------------------------------------*/
void Task::vDicFrozenGet(NNP_Task nnpTask)
{
ENTRY("vDicFrozenGet");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDic) )
    THROW_INVALID_PARAM("dic.frozen@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  bool bFrozen = pDic->bFrozen();
  
  nnpTask->m_xParamStack.vPush(bFrozen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.frozen! -- set dictionary frozen?                                      */
/*----------------------------------------------------------------------------*/
void Task::vDicFrozenSet(NNP_Task nnpTask)
{
ENTRY("vDicFrozenSet");

  i8 iFrozen;
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xParamStack.bPop(iFrozen,pPtrDic) )
    THROW_INVALID_PARAM("dic.frozen@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  pDic->vFrozen(iFrozen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.dump -- dump dictionary stack                                          */
/*----------------------------------------------------------------------------*/
void Task::vDicStack(NNP_Task nnpTask)
{
ENTRY("vDicDump");
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->pDicStack()->vDump();
  }
  else {
    nnpTask->m_xDicStack.vDump();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.init -- initialise a dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vDicInit(NNP_Task nnpTask)
{
ENTRY("vDicInit");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDic) )
    THROW_INVALID_PARAM("dic.init");
  
  Dic* pDic = _pDic(pPtrDic);

  new (pDic) Dic;
  
  // this is because of GlobalDictionary (ugh)
  pDic->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind@ -- get dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Task::vDicKindGet(NNP_Task nnpTask)
{
  ENTRY("vDicFrozenGet");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDic) )
    THROW_INVALID_PARAM("dic.kind@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  Object* pPtrKind;
  pPtrKind = pDic->pDicKind();
  
  nnpTask->m_xParamStack.vPush(pPtrKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.kind! -- set dictionary kind                                           */
/*----------------------------------------------------------------------------*/
void Task::vDicKindSet(NNP_Task nnpTask)
{
  ENTRY("vDicKindSet");
  
  Object* pPtrKind;
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDic,pPtrKind) )
    THROW_INVALID_PARAM("dic.kind@");
  
  Dic* pDic = _pDic(pPtrDic);
  
  Word* pKind = _pWord(pPtrKind);
  
  pDic->vDicKind(pKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.last -- get last word of dictionary                                    */
/*----------------------------------------------------------------------------*/
void Task::vDicLast(NNP_Task nnpTask)
{
ENTRY("vDicLast");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDic) )
    THROW_INVALID_PARAM("dic.last");
  
  Dic* pDic = _pDic(pPtrDic);
  
  Word* pLastWord = pDic->pCurrentWord();
  
  nnpTask->m_xParamStack.vPush(pLastWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.main -- push main dictionary onto stack                                */
/*----------------------------------------------------------------------------*/
void Task::vDicMain(NNP_Task nnpTask)
{
ENTRY("vDicMain");
  
  nnpTask->m_xParamStack.vPush(gpMainDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name -- push name of dictionary onto stack                             */
/*----------------------------------------------------------------------------*/
void Task::vDicName(NNP_Task nnpTask)
{
ENTRY("vDicName");
 
  Object* pPtrDic;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDic) )
    THROW_INVALID_PARAM("dic.name");
  
  Dic* pDic = _pDic(pPtrDic);

  ValWsp sName;
  sName = pDic->sName().a();
  
  nnpTask->m_xParamStack.vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.pick -- put a dictionary from dictionary stack onto param stack        */
/*----------------------------------------------------------------------------*/
void Task::vDicPick(NNP_Task nnpTask)
{
ENTRY("vDicPick");
  
  i8 iDic;
  if ( ! nnpTask->m_xParamStack.bPop(iDic) )
    THROW_INVALID_PARAM("dic.pick");
  
  Object* pPtrDic;

  if (nnpTask->m_pSEitf) {
    pPtrDic = nnpTask->m_pSEitf->pDicStack()->pPick(iDic);
  }
  else {
    pPtrDic = nnpTask->m_xDicStack.pPick(iDic);
  }
  
  nnpTask->m_xParamStack.vPush(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.push -- push a dictionary from param stack onto dictionary stack       */
/*----------------------------------------------------------------------------*/
void Task::vDicPush(NNP_Task nnpTask)
{
ENTRY("vDicPush");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDic) )
    THROW_INVALID_PARAM("dic.push");

  Dic* pDic = _pDic(pPtrDic);
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->vPushDic(pDic);
  }
  else {
    nnpTask->m_xDicStack.vPush(pDic);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.ref -- has dictionary got references                                   */
/*----------------------------------------------------------------------------*/
void Task::vDicRef(NNP_Task nnpTask)
{
  ENTRY("vDicRef");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDic) )
    THROW_INVALID_PARAM("dic.ref");
  
  Dic* pDic = _pDic(pPtrDic);

  bool bRef = pDic->bRefOut();
  
  nnpTask->m_xParamStack.vPush(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dicStack.push -- push a dictionary from stack onto task dictionary stack   */
/*----------------------------------------------------------------------------*/
void Task::vDicStackPush(NNP_Task nnpTask)
{
  ENTRY("vDicStackPush");
  
  Object* pPtrDic;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDic) )
    THROW_INVALID_PARAM("dicStack.push");
  
  Dic* pDic = _pDic(pPtrDic);
  
  nnpTask->m_xDicStack.vPush(pDic);
  
  EXIT();
}

// dir: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* dir.exists -- test if a directory exists                                   */
/*----------------------------------------------------------------------------*/
void Task::vDirExists(NNP_Task nnpTask)
{
ENTRY("vDirExists");
  
  s256 sPath;
  
  if ( ! nnpTask->m_xParamStack.bPop(sPath) )
    THROW_INVALID_PARAM("dir.exists");
  
  Dir xDir;
  
  i8 iResult = xDir.bExists(sPath);
  nnpTask->m_xParamStack.vPush(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.make -- make a directory                                               */
/*----------------------------------------------------------------------------*/
void Task::vDirMake(NNP_Task nnpTask)
{
ENTRY("vDirMake");
  
  s256 sPath;
  
  if ( ! nnpTask->m_xParamStack.bPop(sPath) )
    THROW_INVALID_PARAM("dir.make");
  
  Dir xDir;
  
  i8 iResult = xDir.bMake(sPath);
  nnpTask->m_xParamStack.vPush(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.remove -- remove a directory                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirRemove(NNP_Task nnpTask)
{
ENTRY("vDirRemove");
  
  s256 sPath;
  
  if ( ! nnpTask->m_xParamStack.bPop(sPath) )
    THROW_INVALID_PARAM("dir.remove");
  
  Dir xDir;
  
  i8 iResult = xDir.bRemove(sPath);
  nnpTask->m_xParamStack.vPush(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cwd -- change working directory                                            */
/*----------------------------------------------------------------------------*/
void Task::vCwd(NNP_Task nnpTask)
{
ENTRY("vCwd");
  
  s256 sDir;
  if ( ! nnpTask->m_xParamStack.bPop(sDir) )
    THROW_INVALID_PARAM("cwd");
  
  Dir xDir;
  if ( ! xDir.bSetCurrent(sDir) )
    THROW("Unable to set current directory");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* gwd -- get working directory                                               */
/*----------------------------------------------------------------------------*/
void Task::vGwd(NNP_Task nnpTask)
{
ENTRY("vGwd");
  
  ValWsp xStrVal;
  
  Dir xDir;
  s256 sDir;
  
  if (xDir.bGetCurrent(sDir)) {
    xStrVal = sDir.a();
    nnpTask->m_xParamStack.vPush(&xStrVal);
  }
  else {
    THROW("Unable to get current directory");
  }
  
  EXIT();
}



/*----------------------------------------------------------------------------*/
/* dir.close -- close directory                                               */
/*----------------------------------------------------------------------------*/
void Task::vDirClose(NNP_Task nnpTask)
{
ENTRY("vDirClose");
  
  Object* pPtrDir;

  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.close");
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.delete -- delete instance of dir                                       */
/*----------------------------------------------------------------------------*/
void Task::vDirDelete(NNP_Task nnpTask)
{
ENTRY("vDirDelete");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("file.delete");
  
  Dir* pDir = _pDir(pPtrDir);
  
  delete pDir;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryName -- get dir entry name                                        */
/*----------------------------------------------------------------------------*/
void Task::vDirEntryName(NNP_Task nnpTask)
{
ENTRY("vDirEntryName");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.close");
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sEntryName;
  
  sEntryName = pDir->sEntryName();
  
  nnpTask->m_xParamStack.vPush(sEntryName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryType -- get dir entry type                                        */
/*----------------------------------------------------------------------------*/
void Task::vDirEntryType(NNP_Task nnpTask)
{
ENTRY("vDirEntryType");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.close");
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iEntryType;
  
  iEntryType = pDir->iEntryType();
  
  nnpTask->m_xParamStack.vPush(iEntryType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.init -- init directory                                                 */
/*----------------------------------------------------------------------------*/
void Task::vDirInit(NNP_Task nnpTask)
{
ENTRY("vDirInit");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.init");
  
  Dir* pDir = _pDir(pPtrDir);
  
  new(pDir) Dir;
  
  pDir->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isClosed -- is dir closed?                                             */
/*----------------------------------------------------------------------------*/
void Task::vDirIsClosed(NNP_Task nnpTask)
{
ENTRY("vDirIsClosed");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.close");
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsClosed = pDir->bIsClosed();
  
  nnpTask->m_xParamStack.vPush(bIsClosed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isOpen -- is dir open?                                                 */
/*----------------------------------------------------------------------------*/
void Task::vDirIsOpen(NNP_Task nnpTask)
{
ENTRY("vDirIsOpen");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.close");
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsOpen = pDir->bIsOpen();
  
  nnpTask->m_xParamStack.vPush(bIsOpen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.new -- new instance of dir                                             */
/*----------------------------------------------------------------------------*/
void Task::vDirNew(NNP_Task nnpTask)
{
ENTRY("vDirNew");
  
  Dir* pDir = new Dir;
  
  Object* pPtrDir;
  pPtrDir = (Object*)pDir;
  
  nnpTask->m_xParamStack.vPush(pPtrDir);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.open -- open dir for reading                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirOpen(NNP_Task nnpTask)
{
ENTRY("vDirOpen");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.open");
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bOpened = pDir->bOpen();
  
  nnpTask->m_xParamStack.vPush(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path@ -- get path                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDirPathGet(NNP_Task nnpTask)
{
ENTRY("vDirPathGet");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.path@");
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sPath;
  
  sPath = pDir->sPath();
  
  nnpTask->m_xParamStack.vPush(sPath);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path! -- set path                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDirPathSet(NNP_Task nnpTask)
{
ENTRY("vDirPathSet");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.path!");
  
  Dir* pDir = _pDir(pPtrDir);
  
  ValWsp xWspPath;

  if ( ! nnpTask->m_xParamStack.bPop(xWspPath) )
    THROW_INVALID_PARAM("dir.path!");
  
  s256 sPath;
  sPath = xWspPath.sStr();
  
  pDir->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.read -- read next dir entry                                            */
/*----------------------------------------------------------------------------*/
void Task::vDirRead(NNP_Task nnpTask)
{
ENTRY("vDirRead");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.read");
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bRead  = pDir->bRead();
  
  nnpTask->m_xParamStack.vPush(bRead);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.rewind -- rewind dir pointer                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirRewind(NNP_Task nnpTask)
{
ENTRY("vDirRewind");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.rewind");
  
  Dir* pDir = _pDir(pPtrDir);
  
  pDir->vRewind();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.seek -- seek dir ptr to a given position                               */
/*----------------------------------------------------------------------------*/
void Task::vDirSeek(NNP_Task nnpTask)
{
ENTRY("vDirSeek");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.seek");
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition;
  if ( ! nnpTask->m_xParamStack.bPop(iPosition) )
    THROW_INVALID_PARAM("dir.seek");
  
  pDir->vSeek(iPosition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.tell -- tel current dir position                                       */
/*----------------------------------------------------------------------------*/
void Task::vDirTell(NNP_Task nnpTask)
{
ENTRY("vDirTell");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrDir) )
    THROW_INVALID_PARAM("dir.tell");
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition = pDir->iTell();
  
  nnpTask->m_xParamStack.vPush(iPosition);
  
  EXIT();
}

// errors: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* error.msg -- get error message given the error code                        */
/*----------------------------------------------------------------------------*/
void Task::vErrorMsg(NNP_Task nnpTask)
{
  ENTRY("vErrorMsg");
  
  
  i8 iErrorCode;
  if ( ! nnpTask->m_xParamStack.bPop(iErrorCode) )
    THROW_INVALID_PARAM("error.msg");
  
  if (iErrorCode < EC_NONE || iErrorCode > EC_ENUMS)
    THROW("error.msg");

  s256 sErrorMsg;
  sErrorMsg = pEC[iErrorCode];
  
  nnpTask->m_xParamStack.vPush(sErrorMsg);
  
  EXIT();
}


// file: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* file.close -- close a file                                                 */
/*----------------------------------------------------------------------------*/
void Task::vFileClose(NNP_Task nnpTask)
{
ENTRY("vFileClose");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrFile) )
    THROW_INVALID_PARAM("file.close");
  
  File* pFile = _pFile(pPtrFile);
  
  pFile->vClose();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.delete -- delete a file                                               */
/*----------------------------------------------------------------------------*/
void Task::vFileDelete(NNP_Task nnpTask)
{
ENTRY("vFileDelete");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrFile) )
    THROW_INVALID_PARAM("file.delete");
  
  File* pFile = _pFile(pPtrFile);

  delete pFile;
  
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.init -- initialise a file                                             */
/*----------------------------------------------------------------------------*/
void Task::vFileInit(NNP_Task nnpTask)
{
ENTRY("vFileInit");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrFile) )
    THROW_INVALID_PARAM("file.init");
  
  File* pFile = _pFile(pPtrFile);
  
  new (pFile) File;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.new -- new a file                                                     */
/*----------------------------------------------------------------------------*/
void Task::vFileNew(NNP_Task nnpTask)
{
ENTRY("vFileNew");
  
  File* pFile = new File;
  
  Object* pPtrFile;
  pPtrFile = (Object*)pFile;
  
  nnpTask->m_xParamStack.vPush(pPtrFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file                                                   */
/*----------------------------------------------------------------------------*/
void Task::vFileOpen(NNP_Task nnpTask)
{
ENTRY("vFileOpen");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrFile) )
    THROW_INVALID_PARAM("file.open");
  
  File* pFile = _pFile(pPtrFile);
  
  s32 sMode;
  
  if ( ! nnpTask->m_xParamStack.bPop(sMode) )
    THROW("file.open");
  
  bool bOpened = pFile->bOpen(sMode.a());
  
  nnpTask->m_xParamStack.vPush(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path@ -- get file path                                                */
/*----------------------------------------------------------------------------*/
void Task::vFilePathGet(NNP_Task nnpTask)
{
ENTRY("vFilePathGet");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrFile) )
    THROW_INVALID_PARAM("file.path@");
  
  File* pFile = _pFile(pPtrFile);

  s256 sPath = pFile->sPath();
  
  nnpTask->m_xParamStack.vPush(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path! -- set file path                                                */
/*----------------------------------------------------------------------------*/
void Task::vFilePathSet(NNP_Task nnpTask)
{
ENTRY("vFilePathSet");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrFile) )
    THROW_INVALID_PARAM("file.path!");
  
  File* pFile = _pFile(pPtrFile);
  
  s256 sPath;
  
  if ( ! nnpTask->m_xParamStack.bPop(sPath) )
    THROW_INVALID_PARAM("file.path!");
  
  pFile->vPath(sPath.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.read -- read from a file                                              */
/*----------------------------------------------------------------------------*/
void Task::vFileRead(NNP_Task nnpTask)
{
ENTRY("vFileRead");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrFile) )
    THROW_INVALID_PARAM("file.read");
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s8K sLine;

  if (pFile->bReadLine(sLine)) {
    nnpTask->m_xParamStack.vPush(sLine);
    nnpTask->m_xParamStack.vPush((i8)1);
  }
  else {
    nnpTask->m_xParamStack.vPush((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.readRecord -- read record from file                                   */
/*----------------------------------------------------------------------------*/
void Task::vFileReadRecord(NNP_Task nnpTask)
{
  ENTRY("vFileRead");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrFile) )
    THROW_INVALID_PARAM("file.readRecord");
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  if (pFile->bReadRecord()) {
    nnpTask->m_xParamStack.vPush((i8)1);
  }
  else {
    nnpTask->m_xParamStack.vPush((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.write -- write to a file                                              */
/*----------------------------------------------------------------------------*/
void Task::vFileWrite(NNP_Task nnpTask)
{
ENTRY("vFileWrite");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrFile) )
    THROW_INVALID_PARAM("file.write");
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  ValWsp xValue;
  
  if ( ! nnpTask->m_xParamStack.bPop(xValue) )
    THROW_INVALID_PARAM("file.write");
  
  char* wStr = xValue.pAsStrVal()->sStr();
  
  pFile->vPrint("%s",wStr);
  
  EXIT();
}

// flow: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* again -- unconditionally repeat loop (use only in : docol)                 */
/*----------------------------------------------------------------------------*/
void Task::vFlowAgain(NNP_Task nnpTask)
{
  ENTRY("vAgain");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  void* pBeginLocVoid;
  if ( ! nnpTask->m_xReturnStack.bPop(pBeginLocVoid) )
    THROW_INVALID_RETURN("again");
  Cell* pBeginLoc = _pCell(pBeginLocVoid);
  
  pCurrentWord->vEncode("jump",pBeginLoc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* again_ -- unconditionally repeat begin_ loop (use only in : docol)         */
/*----------------------------------------------------------------------------*/
void Task::vFlowAgain_(NNP_Task nnpTask)
{
  ENTRY("vAgain_");
  
  nnpTask->IP.m_pCell = nnpTask->m_pBeginIP;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* begin -- begin a loop (use only in : docol)                                */
/*----------------------------------------------------------------------------*/
void Task::vFlowBegin(NNP_Task nnpTask)
{
  ENTRY("vBegin");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pLastCell = pCurrentWord->pLastCell();
  
  nnpTask->m_xReturnStack.vPush(pLastCell);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* begin_ -- begin a loop (use only in : docol)                               */
/*----------------------------------------------------------------------------*/
void Task::vFlowBegin_(NNP_Task nnpTask)
{
  ENTRY("vBegin_");
  
  nnpTask->m_pBeginIP = nnpTask->IP.m_pCell;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* do -- compile time of do ... loop                                immediate */
/*----------------------------------------------------------------------------*/
void Task::vFlowDo(NNP_Task nnpTask)
{
  ENTRY("vDo");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("do_rt");
  
  Cell* pLastCell = pCurrentWord->pLastCell();
  
  nnpTask->m_xReturnStack.vPush((void*)pLastCell);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* do_rt -- runtime of do ... loop                                            */
/*----------------------------------------------------------------------------*/
void Task::vFlowDo_RT(NNP_Task nnpTask)
{
  ENTRY("vDo_RT");
  
  i8 iIndex;
  i8 iLimit;
  if ( ! nnpTask->m_xParamStack.bPop(iIndex,iLimit) )
    THROW_INVALID_PARAM("do_rt");
  
  nnpTask->m_xReturnStack.vPush(iLimit);
  nnpTask->m_xReturnStack.vPush(iIndex); // index is on top
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* else -- else clause                                                        */
/*----------------------------------------------------------------------------*/
void Task::vFlowElse(NNP_Task nnpTask)
{
  ENTRY("vElse");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  void* pIfJumpArgVoid;
  if ( ! nnpTask->m_xReturnStack.bPop(pIfJumpArgVoid) )
    THROW_INVALID_RETURN("else");
  Cell* pIfJumpArg = _pCell(pIfJumpArgVoid);
  
  //-------------------------------------------------------------------------
  
  Cell* pElseJumpArg;
  
  pCurrentWord->vEncode("jump");
  pElseJumpArg = (Cell*)pCurrentWord->pAppend((i8)0);
  
  nnpTask->m_xReturnStack.vPush(pElseJumpArg);
  
  //-------------------------------------------------------------------------
  
  pIfJumpArg->m_pJumpTo = pCurrentWord->pLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* if -- if clause                                                            */
/*----------------------------------------------------------------------------*/
void Task::vFlowIf(NNP_Task nnpTask)
{
  ENTRY("vIf");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pIfJumpArg;
  
  pCurrentWord->vEncode("jumpFalse");
  
  pIfJumpArg = (Cell*)pCurrentWord->pAppend((i8)0);
  
  nnpTask->m_xReturnStack.vPush(pIfJumpArg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* repeat -- repeat a begin-while-repeat loop                       immediate */
/*----------------------------------------------------------------------------*/
void Task::vFlowRepeat(NNP_Task nnpTask)
{
  ENTRY("vRepeat");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  void* pWhileJumpArgVoid;
  if ( ! nnpTask->m_xReturnStack.bPop(pWhileJumpArgVoid) )
    THROW_INVALID_RETURN("repeat");
  Cell* pWhileJumpArg = _pCell(pWhileJumpArgVoid);
  
  //-------------------------------------------------------------------------
  
  void* pBeginLocVoid;
  if ( ! nnpTask->m_xReturnStack.bPop(pBeginLocVoid) )
    THROW_INVALID_RETURN("repeat");
  Cell* pBeginLoc = _pCell(pBeginLocVoid);
  
  pCurrentWord->vEncode("jump",pBeginLoc);
  
  //-------------------------------------------------------------------------
  
  pWhileJumpArg->m_pJumpTo = pCurrentWord->pLastCell();
  
  EXIT();
}

// grid: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* grid.appendNode -- append grid node into grid                              */
/*----------------------------------------------------------------------------*/
void Task::vGridAppendNode(NNP_Task nnpTask)
{
ENTRY("vGridAppendNode");
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrNode) )
    THROW_INVALID_PARAM("grid.addNode");

  Node* pNode = _pNode(pPtrNode);
  ASSERT(pNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  gpGrid->vAppend(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.init -- initialise a grid                                             */
/*----------------------------------------------------------------------------*/
void Task::vGridInit(NNP_Task nnpTask)
{
ENTRY("vGridInit");
  
  Object* pPtrGrid;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrGrid) )
    THROW_INVALID_PARAM("grid.init");
  
  Grid* pGrid = _pGrid(pPtrGrid);
  ASSERT(CN_GRID == pGrid->eClass());
  
  new(pGrid) Grid;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.started! -- set grid started                                          */
/*----------------------------------------------------------------------------*/
void Task::vGridStartedStore(NNP_Task nnpTask)
{
  ENTRY("vGridSetStarted");
  
  Object* pPtrGrid;
  i8 iStarted;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrGrid) )
    THROW_INVALID_PARAM("grid.started!");
  
  Grid* pGrid = _pGrid(pPtrGrid);
  ASSERT(CN_GRID == pGrid->eClass());
  
  if ( ! nnpTask->m_xParamStack.bPop(iStarted) )
    THROW_INVALID_PARAM("grid.started!");
  
  pGrid->vHasStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid.started@ -- get grid started                                          */
/*----------------------------------------------------------------------------*/
void Task::vGridStartedFetch(NNP_Task nnpTask)
{
  ENTRY("vGridGetStarted");
  
  Object* pPtrGrid;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrGrid) )
    THROW_INVALID_PARAM("grid.started@");
  
  Grid* pGrid = _pGrid(pPtrGrid);
  ASSERT(CN_GRID == pGrid->eClass());
  
  nnpTask->m_xParamStack.vPush(pGrid->bHasStarted());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* iterate -- compile time of iterate ... next                      immediate */
/*----------------------------------------------------------------------------*/
void Task::vFlowIterate(NNP_Task nnpTask)
{
  ENTRY("vIterate");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("iterate_rt_a");
  
  Cell* pIterateLoc = pCurrentWord->pLastCell();
  nnpTask->m_xReturnStack.vPush(pIterateLoc);
  pCurrentWord->vEncode("iterate_rt_b");
  
  pCurrentWord->vEncode("jumpFalse");
  Cell* pJumpFalseArg = (Cell*)pCurrentWord->pAppend((i8)0);
  nnpTask->m_xReturnStack.vPush(pJumpFalseArg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* iterate_rt_a -- runtime of iterate ... next                                */
/*----------------------------------------------------------------------------*/
void Task::vFlowIterate_RT_A(NNP_Task nnpTask)
{
  ENTRY("vIterate_RT_A");
  
  Object* pPtrObject;
  Object* pPtrCursor;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrObject,pPtrCursor) )
    THROW_INVALID_PARAM("iterate_rt_a");
  
  nnpTask->m_xReturnStack.vPush(pPtrCursor);
  nnpTask->m_xReturnStack.vPush(pPtrObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* iterate_rt_b -- runtime of iterate ... next                                */
/*----------------------------------------------------------------------------*/
void Task::vFlowIterate_RT_B(NNP_Task nnpTask)
{
  ENTRY("vIterate_RT_B");
  
  i8 iPick = 0;
  
  ValWsp xObjectWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xObjectWsp);
  nnpTask->m_xParamStack.vPush(xObjectWsp);
  
  ValWsp xWsp;
  
  if ( ! xObjectWsp.iAsInt() ) {
    if ( !nnpTask->m_xReturnStack.bPop(xWsp) )
      THROW_INVALID_PARAM("iterate_rt_b");
    if ( !nnpTask->m_xReturnStack.bPop(xWsp) )
      THROW_INVALID_PARAM("iterate_rt_b");
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jump -- jump to location                                                   */
/*----------------------------------------------------------------------------*/
void Task::vFlowJump(NNP_Task nnpTask)
{
  ENTRY("vJump");
  
  nnpTask->IP.m_pCell = nnpTask->IP.m_pCell->m_pJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpFalse -- jump on false to location                                     */
/*----------------------------------------------------------------------------*/
void Task::vFlowJumpFalse(NNP_Task nnpTask)
{
  ENTRY("vJumpFalse");
  
  i8 iVal;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iVal) )
    THROW_INVALID_PARAM("jumpFalse");
  
  //  i8 iFlag = nnpTask->m_xParamStack.iPop();
  
  // cell points to the jump arg
  if (iVal)
    nnpTask->IP.m_pCell++;
  else
    nnpTask->IP.m_pCell = nnpTask->IP.m_pCell->m_pJumpTo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpTrue -- jump on true to location                                       */
/*----------------------------------------------------------------------------*/
void Task::vFlowJumpTrue(NNP_Task nnpTask)
{
  ENTRY("vJumpTrue");
  
  i8 iVal;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iVal) )
    THROW_INVALID_PARAM("jumpTrue");
  
  // cell points to the jump arg
  if (iVal)
    nnpTask->IP.m_pCell = nnpTask->IP.m_pCell->m_pJumpTo;
  else
    nnpTask->IP.m_pCell++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loop -- compile time for do ... loop                            immediate  */
/*----------------------------------------------------------------------------*/
void Task::vFlowLoop(NNP_Task nnpTask)
{
  ENTRY("vLoop");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  void* pDoLocVoid;
  if ( ! nnpTask->m_xReturnStack.bPop(pDoLocVoid) )
    THROW_INVALID_RETURN("loop");
  Cell* pDoLoc = _pCell(pDoLocVoid);
  
  pCurrentWord->vEncode("Loop_RT");
  
  pCurrentWord->vEncode("jump",pDoLoc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loop_rt -- runtime for do loop                                             */
/*----------------------------------------------------------------------------*/
void Task::vFlowLoop_RT(NNP_Task nnpTask)
{
  ENTRY("vLoop_RT");
  
  i8 iIndex;
  i8 iLimit;
  
  if ( ! nnpTask->m_xReturnStack.bPop(iIndex,iLimit) )
    THROW_INVALID_PARAM("loop_rt");
  
  iIndex++;
  
  if (iIndex < iLimit) {
    nnpTask->m_xReturnStack.vPush(iLimit);
    nnpTask->m_xReturnStack.vPush(iIndex); // index is on top
  }
  else {
    nnpTask->IP.m_pCell += 2;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loopPlus -- do +loop                                                       */
/*----------------------------------------------------------------------------*/
void Task::vFlowLoopPlus(NNP_Task nnpTask)
{
  ENTRY("vLoopPlus");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  void* pDoLocVoid;
  if ( ! nnpTask->m_xReturnStack.bPop(pDoLocVoid) )
    THROW_INVALID_RETURN("+loop");
  Cell* pDoLoc = _pCell(pDoLocVoid);
  
  pCurrentWord->vEncode("+Loop_RT");
  
  pCurrentWord->vEncode("jump",pDoLoc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loopPlus_rt -- runtime for do +loop                                        */
/*----------------------------------------------------------------------------*/
void Task::vFlowLoopPlus_RT(NNP_Task nnpTask)
{
  ENTRY("vLoopPlus_RT");
  
  i8 iIndex;
  i8 iLimit;
  i8 iInc;
  
  if ( ! nnpTask->m_xReturnStack.bPop(iIndex,iLimit) )
    THROW_INVALID_PARAM("loopPlus_rt");
  
  if ( ! nnpTask->m_xParamStack.bPop(iInc) )
    THROW_INVALID_PARAM("loopPlus_rt");
  
  iIndex += iInc;
  
  if (iIndex < iLimit) {
    nnpTask->m_xReturnStack.vPush(iLimit);
    nnpTask->m_xReturnStack.vPush(iIndex);
  }
  else {
    nnpTask->IP.m_pCell += 2;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* next -- compile time for iterate ... next                        immediate */
/*----------------------------------------------------------------------------*/
void Task::vFlowNext(NNP_Task nnpTask)
{
  ENTRY("vNext");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  void* pJumpFalseArgVoid;
  if ( ! nnpTask->m_xReturnStack.bPop(pJumpFalseArgVoid) )
    THROW_INVALID_RETURN("next");
  Cell* pJumpFalseArg = _pCell(pJumpFalseArgVoid);
  
  void* pIterateLocVoid;
  if ( ! nnpTask->m_xReturnStack.bPop(pIterateLocVoid) )
    THROW_INVALID_RETURN("next");
  Cell* pIterateLoc = _pCell(pIterateLocVoid);
  
  //----------------------------------------------------------------------------
  
  pCurrentWord->vEncode("jump",pIterateLoc);
  
  pJumpFalseArg->m_pJumpTo = pCurrentWord->pLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* then -- then clause                                                        */
/*----------------------------------------------------------------------------*/
void Task::vFlowThen(NNP_Task nnpTask)
{
  ENTRY("vThen");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  void* pIfJumpArgVoid;
  if ( ! nnpTask->m_xReturnStack.bPop(pIfJumpArgVoid) )
    THROW_INVALID_RETURN("then");
  Cell* pIfJumpArg = _pCell(pIfJumpArgVoid);
  
  pIfJumpArg->m_pJumpTo = pCurrentWord->pLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* unloop -- pop the return stack                                             */
/*----------------------------------------------------------------------------*/
void Task::vFlowUnloop(NNP_Task nnpTask)
{
  ENTRY("vUnloop");
  
  // we should just modify the pointers rather than actually pop
  ValWsp xValA;
  ValWsp xValB;
  if ( ! nnpTask->m_xReturnStack.bPop(xValA,xValB) )
    THROW_INVALID_RETURN("unloop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* until -- repeat begin loop until true                                      */
/*----------------------------------------------------------------------------*/
void Task::vFlowUntil(NNP_Task nnpTask)
{
  ENTRY("vUntil");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  void* pBeginLocVoid;
  if ( ! nnpTask->m_xReturnStack.bPop(pBeginLocVoid) )
    THROW_INVALID_RETURN("until");
  Cell* pBeginLoc = _pCell(pBeginLocVoid);
  
  //-------------------------------------------------------------------------
  
  pCurrentWord->vEncode("jumpFalse",pBeginLoc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* while -- check for continue with loop                            immediate */
/*----------------------------------------------------------------------------*/
void Task::vFlowWhile(NNP_Task nnpTask)
{
  ENTRY("vWhile");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("jumpFalse");
  Cell* pWhileJumpArg = (Cell*)pCurrentWord->pAppend((i8)0);
  
  nnpTask->m_xReturnStack.vPush(pWhileJumpArg);
  
  EXIT();
}

// input: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* input.close -- close current input stream                                  */
/*----------------------------------------------------------------------------*/
void Task::vInputClose(NNP_Task nnpTask)
{
ENTRY("vInputClose");
  
  nnpTask->m_xTokeniser.vUseEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.depth -- push current depth of input stream onto param stack        */
/*----------------------------------------------------------------------------*/
void Task::vInputDepth(NNP_Task nnpTask)
{
ENTRY("vInputDepth");
  
  i8 iDepth = nnpTask->m_xTokeniser.iDepth();
  
  nnpTask->m_xParamStack.vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.next -- get next input token from current input stream               */
/*----------------------------------------------------------------------------*/
void Task::vInputNext(NNP_Task nnpTask)
{
ENTRY("vInputNext");
  
  if (nnpTask->m_iTracing) {
    ASSERT(nnpTask->m_iTracing);
  }
  
  char* aToken = nnpTask->m_sToken.a();
  
  nnpTask->m_eToken = nnpTask->m_xTokeniser.eNextToken(aToken);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.open -- open new input using filepath on param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vInputOpen(NNP_Task nnpTask)
{
ENTRY("vInputOpen");
  
  s256 sFilePath;
  
  if ( ! nnpTask->m_xParamStack.bPop(sFilePath) )
    THROW_INVALID_PARAM("input.open");
  
  bool bOpened = nnpTask->m_xTokeniser.bUseFile(sFilePath);
  
  if ( ! bOpened )
    THROW("No such file: %s",sFilePath.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.pop -- pop to previous input stream                                  */
/*----------------------------------------------------------------------------*/
void Task::vInputPop(NNP_Task nnpTask)
{
ENTRY("vInputPop");
  
  bool bPopped = nnpTask->m_xTokeniser.bPop();
  ASSERT(bPopped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.push -- push input stream, allowing for new input stream             */
/*----------------------------------------------------------------------------*/
void Task::vInputPush(NNP_Task nnpTask)
{
ENTRY("vInputPush");
  
  nnpTask->m_xTokeniser.vPush();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.stdin -- use stdin as new input stream                               */
/*----------------------------------------------------------------------------*/
void Task::vInputStdin(NNP_Task nnpTask)
{
ENTRY("vInputStdin");
  
  nnpTask->m_xTokeniser.vUseStdin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.text -- use string as new input stream                               */
/*----------------------------------------------------------------------------*/
void Task::vInputText(NNP_Task nnpTask)
{
ENTRY("vInputText");
  
  s256 sInput;
  
  if ( ! nnpTask->m_xParamStack.bPop(sInput) )
    THROW_INVALID_PARAM("input.text");
    
  nnpTask->m_xTokeniser.vUseString(sInput);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.token -- push current token of input stream onto param stack         */
/*----------------------------------------------------------------------------*/
void Task::vInputToken(NNP_Task nnpTask)
{
ENTRY("vInputToken");
  
  nnpTask->m_xParamStack.vPush(nnpTask->m_sToken);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.type -- push current token type onto param stack                     */
/*----------------------------------------------------------------------------*/
void Task::vInputType(NNP_Task nnpTask)
{
ENTRY("vInputType");
  
  eTK eTokenType = nnpTask->m_eToken;
  
  nnpTask->m_xParamStack.vPush(eTokenType);
  
  EXIT();
}

// int: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* + -- add top two integers on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vIntAdd(NNP_Task nnpTask)
{
  ENTRY("vIntAdd");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(j,i) )
    THROW_INVALID_PARAM("+");
  
  r = i + j;
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* / -- divide top integer by second integer on stack                         */
/*----------------------------------------------------------------------------*/
void Task::vIntDivide(NNP_Task nnpTask)
{
  ENTRY("vIntDivide");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(j,i) )
    THROW_INVALID_PARAM("/");
  
  r = DIV(i,j);
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* == -- test top two integers on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Task::vIntEqual(NNP_Task nnpTask)
{
  ENTRY("vIntEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(j,i) )
    THROW_INVALID_PARAM("==");
  
  r = i == j;
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* > -- test top two integers on data stack for greater than                  */
/*----------------------------------------------------------------------------*/
void Task::vIntGreater(NNP_Task nnpTask)
{
  ENTRY("vIntGreater");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(j,i) )
    THROW_INVALID_PARAM(">");
  
  r = i > j;
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >= -- test top two integers on data stack for greater than equal to        */
/*----------------------------------------------------------------------------*/
void Task::vIntGreaterEqual(NNP_Task nnpTask)
{
  ENTRY("vIntGreaterEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(j,i) )
    THROW_INVALID_PARAM(">=");
  
  r = i >= j;
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* < -- test top two integers on data stack for less than                     */
/*----------------------------------------------------------------------------*/
void Task::vIntLess(NNP_Task nnpTask)
{
  ENTRY("vIntLess");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(j,i) )
    THROW_INVALID_PARAM("<");
  
  r = i < j;
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* <= -- test top two integers on data stack for less than equal to           */
/*----------------------------------------------------------------------------*/
void Task::vIntLessEqual(NNP_Task nnpTask)
{
  ENTRY("vIntLessEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(j,i) )
    THROW_INVALID_PARAM("<=");
  
  r = i <= j;
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* % -- modulo of top integer by second integer on stack                      */
/*----------------------------------------------------------------------------*/
void Task::vIntModulo(NNP_Task nnpTask)
{
  ENTRY("vIntModulo");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(j,i) )
    THROW_INVALID_PARAM("%");
  
  r = MOD(i,j);
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* * multiply second integer by top integer on stack                          */
/*----------------------------------------------------------------------------*/
void Task::vIntMultiply(NNP_Task nnpTask)
{
  ENTRY("vIntMultiply");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(j,i) )
    THROW_INVALID_PARAM("*");
  
  r = i * j;
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* neg -- gegate top integer on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vIntNeg(NNP_Task nnpTask)
{
  ENTRY("vIntNeg");
  
  i8 r, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(i) )
    THROW_INVALID_PARAM("neg");
  
  r = -1 * i;
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* != -- test top two integers on data stack for not equal                    */
/*----------------------------------------------------------------------------*/
void Task::vIntNotEqual(NNP_Task nnpTask)
{
  ENTRY("vIntNotEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(j,i) )
    THROW_INVALID_PARAM("!=");
  
  r = i != j;
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* - -- subtract second integer from top integer on stack                     */
/*----------------------------------------------------------------------------*/
void Task::vIntSubtract(NNP_Task nnpTask)
{
  ENTRY("vIntSubtract");
  
  i8 r, j, i;
  
  if ( ! nnpTask->m_xParamStack.bPop(j,i) )
    THROW_INVALID_PARAM("-");
  
  r = i - j;
  
  nnpTask->m_xParamStack.vPush(r);
  
  EXIT();
}

// loader: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* loader.delete -- delete a table loader                                     */
/*----------------------------------------------------------------------------*/
void Task::vLoaderDelete(NNP_Task nnpTask)
{
  ENTRY("vLoade3Delete");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLoader) )
    THROW_INVALID_PARAM("loader.delete");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  delete pLoader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.findChunk -- find next chunk                                        */
/*----------------------------------------------------------------------------*/
void Task::vLoaderFindChunk(NNP_Task nnpTask)
{
  ENTRY("vLoaderFindChunk");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLoader) )
    THROW_INVALID_PARAM("loader.findChunk");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  bool bFoundChunk = pLoader->bFindChunk();
  
  nnpTask->m_xParamStack.vPush(bFoundChunk);
 
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.init -- init a table loader                                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderInit(NNP_Task nnpTask)
{
  ENTRY("vLoaderInit");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLoader) )
    THROW_INVALID_PARAM("loader.init");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  new(pLoader) Loader;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.new -- new a table loader                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNew(NNP_Task nnpTask)
{
  ENTRY("vLoaderNew");
  
  Loader* pLoader = new Loader;
  ASSERT(CN_LOADER == pLoader->eClass());
  
  nnpTask->m_xParamStack.vPush(pLoader);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.newSub -- new a loader sub                                          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNewSub(NNP_Task nnpTask)
{
  ENTRY("vLoaderNewSub");
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLoader) )
    THROW_INVALID_PARAM("loader.init");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  LoaderSub* pLoaderSub = new LoaderSub;
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vLoader(pLoader);
  
  nnpTask->m_xParamStack.vPush(pLoaderSub);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.path! -- store file path into loader                                */
/*----------------------------------------------------------------------------*/
void Task::vLoaderPathStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderPathStore");
  
  ValWsp xWspPath;
  Object* pPtrLoader;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLoader,xWspPath) )
    THROW_INVALID_PARAM("loader.path!");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  s256 sPath;
  sPath = xWspPath.sStr();
  
  pLoader->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.subs! -- store number of subordinates into loader                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubsStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubsStore");
  
  i8 nSubs;
  Object* pPtrLoader;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLoader,nSubs) )
    THROW_INVALID_PARAM("loader.subs!");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  pLoader->vSubs(nSubs);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.table! -- store table into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTableStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderTableStore");
  
  Object* pPtrTable;
  Object* pPtrLoader;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLoader,pPtrTable) )
    THROW_INVALID_PARAM("loader.tableStore");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  Table* pTable = _pTable(pPtrTable);
  
  ASSERT(CN_TABLE == pTable->eClass());
  
  pLoader->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.txref! -- store txref into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTxRefStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderTxRefStore");
  
  Object* pPtrLoader;
  Object* pPtrTxRef;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLoader,pPtrTxRef) )
    THROW_INVALID_PARAM("loader.txref!");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  pLoader->vTxRef(pTxRef);
  
  EXIT();
}

// loaderSub: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* loaderSub.appendBuffer -- append buffer into table                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubAppendBuffer(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubAppendBuffer");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vAppendBuffer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.begin -- begin loading                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubBegin(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubBegin");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.completed -- this sub-loader has completed                       */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubCompleted(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubCompleted");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  bool bIsDecider = pLoaderSub->bCompleted();
  
  nnpTask->m_xParamStack.vPush(bIsDecider);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.delete -- delete a table loader                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubDelete(NNP_Task nnpTask)
{
  ENTRY("vLoade3SubDelete");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  delete pLoaderSub;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.end -- end loading                                               */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubEnd(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubEnd");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.fetchRecord -- fetch next record from file being loaded          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubFetchRecord(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubFetchRecord");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  bool bFetched = pLoaderSub->bFetchRecord();
  
  nnpTask->m_xParamStack.vPush(bFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.hashRecord -- hash record being loaded                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubHashRecord(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubHashRecord");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vHashRecord();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.init -- init a table loader                                      */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubInit(NNP_Task nnpTask)
{
  ENTRY("vLoaderInit");
  
  Object* pPtrLoaderSub;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLoaderSub) )
    THROW_INVALID_PARAM("loaderSub.init");
  
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoaderSub);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  new(pLoaderSub) LoaderSub;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.insertRow -- insert row into buffer (buffer into table when full)*/
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubInsertRow(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubInsertRow");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  bool bInserted = pLoaderSub->bInsertRow();
  
  nnpTask->m_xParamStack.vPush(bInserted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.insertRow2 -- insert row into buffer                             */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubInsertRow2(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubInsertRow?");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  bool bInserted = pLoaderSub->bInsertRow2();
  
  nnpTask->m_xParamStack.vPush(bInserted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.loader! -- store loader                                          */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubLoaderStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubLoaderStore");
  
  Object* pPtrLoaderSub;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLoaderSub) )
    THROW_INVALID_PARAM("loaderSub.loader!");
  
  LoaderSub* pLoaderSub = _pLoaderSub(pPtrLoaderSub);
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  Object* pPtrLoader;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLoader) )
    THROW_INVALID_PARAM("loader.openfile");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.new -- new a table loader subordinate                            */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubNew(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubNew");
  
  LoaderSub* pLoaderSub = new LoaderSub;
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  nnpTask->m_xParamStack.vPush(pLoaderSub);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.partition -- get partition number derived from hash of record    */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubPartition(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubPartition");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  i8 iPartition = pLoaderSub->iPartition();
  
  nnpTask->m_xParamStack.vPush(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.partition! -- store partition number                             */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubPartitionStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubPartitionStore");
  
  i8 iPartition;
  if ( ! nnpTask->m_xParamStack.bPop(iPartition) )
    THROW_INVALID_PARAM("loaderSub.partition!");
 
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());

  pLoaderSub->vPartition(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.rowBuffer -- get row buffer of given partition                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubRowBuffer(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubRowBuffer");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  RowBuffer* pRowBuffer = pLoaderSub->pRowBuffer();
  
  nnpTask->m_xParamStack.vPush(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.rowBuffer! -- store row buffer                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubRowBufferStore(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubRowBufferStore");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowBuffer) )
    THROW_INVALID_PARAM("loaderSub.rowBuffer!");
  
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT( !pRowBuffer || CN_ROWBUFFER == pRowBuffer->eClass() );
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  pLoaderSub->vRowBuffer(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loaderSub.table -- get table being loaded                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderSubTable(NNP_Task nnpTask)
{
  ENTRY("vLoaderSubTable");
  
  LoaderSub* pLoaderSub = _pLoaderSub(nnpTask->m_pRegFrame->pValReg(0)->pPtr());
  ASSERT(CN_LOADERSUB == pLoaderSub->eClass());
  
  Table* pTable = pLoaderSub->pTable();
  
  nnpTask->m_xParamStack.vPush(pTable);
  
  EXIT();
}

// locals: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* local.fetch -- push locals[n] onto pstack                                  */
/*----------------------------------------------------------------------------*/
void Task::vLocalFetch(NNP_Task nnpTask)
{
  ENTRY("vLocalFetch");
  
  if ( ! nnpTask->m_pLocalsFrame )
    THROW_INVALID_PARAM("local.fetch");
  
  i8 iLocal;
  if ( ! nnpTask->m_xParamStack.bPop(iLocal) )
    THROW_INVALID_PARAM("local.fetch");
  
  ValReg* pValReg = nnpTask->m_pLocalsFrame->pValReg(iLocal);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* localStore -- pop pstack into locals[n]                                    */
/*----------------------------------------------------------------------------*/
void Task::vLocalStore(NNP_Task nnpTask)
{
  ENTRY("vLocalStore");
  
  if ( ! nnpTask->m_pLocalsFrame )
    THROW_INVALID_PARAM("localStore");
  
  i8 iLocal;
  if ( ! nnpTask->m_xParamStack.bPop(iLocal) )
    THROW_INVALID_PARAM("localStore");
  
  ValReg* pValReg = nnpTask->m_pLocalsFrame->pValReg(iLocal);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals| -- declare set of locals                                immediate  */
/*----------------------------------------------------------------------------*/
void Task::vLocalsBar(NNP_Task nnpTask)
{
ENTRY("vLocalsBar");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Dic* pDic = nnpTask->pDicPeek();

  ValWsp xValWsp;
  
  for (;;) {
    
    // get name of local
    nnpTask->vTokenNext();
    
    // end of locals declaration?
    if (String::bEqualTo("|",nnpTask->m_sToken.a()))
      break;

    // test for first use of locals|
    if ( ! nnpTask->m_nLocals ) {
      
      // create a locals dictionary
      pDic = new Dic;
      nnpTask->vDicPush(pDic);
      
      // enocde locals frame push
      pCurrentWord->vEncode("locals.push");
    }
    
    // create new word for local in current dictionary
    Word* pWordLocal = new(pDic) Word(nnpTask->m_sToken.a());
    // ** do not set as current word **
    
    // set word as local variable
    pWordLocal->vWordType(WT_LOCAL);
    pWordLocal->vLocal(nnpTask->m_nLocals);
    
    // encode the word
    xValWsp = nnpTask->m_nLocals;
    pWordLocal->vEncode("push",&xValWsp);
    pWordLocal->vEncode("local.fetch");
    pWordLocal->vEncode("exit");

    // initialise the local from pstack
    pCurrentWord->vEncode("push",&xValWsp);
    pCurrentWord->vEncode("local.store");
    
    nnpTask->m_nLocals++;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.pop -- pop locals frame                                             */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPop(NNP_Task nnpTask)
{
ENTRY("vLocalsPop");
  
  RegFrame* pLocalsFrame = nnpTask->m_pLocalsFrame;
  
  if ( ! pLocalsFrame )
    THROW_INVALID_PARAM("locals.pop");
  
  nnpTask->m_pLocalsFrame = nnpTask->m_pLocalsFrame->pNext();
  
  delete pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locals.push -- push locals frame                                           */
/*----------------------------------------------------------------------------*/
void Task::vLocalsPush(NNP_Task nnpTask)
{
ENTRY("vLocalsPush");
  
  RegFrame* pLocalsFrame = new RegFrame;
  
  pLocalsFrame->vStackPtr(nnpTask->m_xReturnStack.pTopValue());
  
  pLocalsFrame->vNext(nnpTask->m_pLocalsFrame);
  
  nnpTask->m_pLocalsFrame = pLocalsFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* to -- compile store value into local variable                    immediate */
/*----------------------------------------------------------------------------*/
void Task::vLocalsTo(NNP_Task nnpTask)
{
ENTRY("vLocalsTo");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  // get name of local
  nnpTask->vTokenNext();

  Word* pWordLocal = nnpTask->pFindWord(nnpTask->m_sToken.a());
  ASSERT(pWordLocal);
 
  ValWsp xValWsp;
  xValWsp = pWordLocal->iLocal();
  
  pCurrentWord->vEncode("push",&xValWsp);
  pCurrentWord->vEncode("local.store");

  EXIT();
}

// lock: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* lock.dropread -- drop read lock                                            */
/*----------------------------------------------------------------------------*/
void Task::vLockDropRead(NNP_Task nnpTask)
{
ENTRY("vLockDropRead");
  
  Object* pPtrLock;;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLock) )
    THROW_INVALID_PARAM("lock.dropread");
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vDropRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.dropwrite -- drop write lock                                          */
/*----------------------------------------------------------------------------*/
void Task::vLockDropWrite(NNP_Task nnpTask)
{
ENTRY("vLockDropWrite");

  Object* pPtrLock;;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLock) )
    THROW_INVALID_PARAM("lock.dropwrite");
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vDropWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabread -- grab read lock                                            */
/*----------------------------------------------------------------------------*/
void Task::vLockGrabRead(NNP_Task nnpTask)
{
ENTRY("vLockGrabRead");

  Object* pPtrLock;;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLock) )
    THROW_INVALID_PARAM("lock.grabread");
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vGrabRead();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.grabwrite -- grab write lock                                          */
/*----------------------------------------------------------------------------*/
void Task::vLockGrabWrite(NNP_Task nnpTask)
{
ENTRY("vLockGrabWrite");

  Object* pPtrLock;;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLock) )
    THROW_INVALID_PARAM("lock.grabwrite");
  
  Lock* pLock = _pLock(pPtrLock);
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vGrabWrite();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* lock.init -- initialise lock                                               */
/*----------------------------------------------------------------------------*/
void Task::vLockInit(NNP_Task nnpTask)
{
ENTRY("vLockInit");
  
  Object* pPtrLock;;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrLock) )
    THROW_INVALID_PARAM("lock.init");
  
  Lock* pLock = _pLock(pPtrLock);
  
  new(pLock) Lock;
  
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* and -- logical and of top two values                                       */
/*----------------------------------------------------------------------------*/
void Task::vLogicAnd(NNP_Task nnpTask)
{
  ENTRY("vLogicAnd");
  
  i8 iArgB;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iArgB) )
    THROW_INVALID_PARAM("and");
  
  i8 iArgA;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iArgA) )
    THROW_INVALID_PARAM("and");
  
  i8 iArgR = (iArgA && iArgB) ? 1 : 0;
  
  nnpTask->m_xParamStack.vPush(iArgR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* not -- not of top value                                                    */
/*----------------------------------------------------------------------------*/
void Task::vLogicNot(NNP_Task nnpTask)
{
  ENTRY("vLogicNot");
  
  i8 iValA;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iValA) )
    THROW_INVALID_PARAM("not");
  
  i8 iNotA = iValA ? 0 : 1;
  
  nnpTask->m_xParamStack.vPush(iNotA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* or -- logical or of top two values                                         */
/*----------------------------------------------------------------------------*/
void Task::vLogicOr(NNP_Task nnpTask)
{
  ENTRY("vLogicOr");
  
  i8 iArgB;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iArgB) )
    THROW_INVALID_PARAM("or");
  
  i8 iArgA;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iArgA) )
    THROW_INVALID_PARAM("or");
  
  i8 iArgR = (iArgA || iArgB) ? 1 : 0;
  
  nnpTask->m_xParamStack.vPush(iArgR);
  
  EXIT();
}

// memory: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* memory.bytedump -- byte-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryByteDump(NNP_Task nnpTask)
{
ENTRY("vMemoryByteDump");
  
  i8 iSize;
  if ( ! nnpTask->m_xParamStack.bPop(iSize) )
    THROW_INVALID_PARAM("memory.bytedump");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("memory.bytedump");
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vByteDump(iSize);
  Memory::vHexDump(pWord,iSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.worddump -- word-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryWordDump(NNP_Task nnpTask)
{
ENTRY("vMemoryWordDump");
  
  i8 iSize;
  if ( ! nnpTask->m_xParamStack.bPop(iSize) )
    THROW_INVALID_PARAM("memory.worddump");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("memory.worddump");
  Word* pWord = _pWord(pWordObject);
  
  //pWord->vWordDump(iSize);
  Memory::vWordDump(pWord,iSize);
  
  EXIT();
}

// node: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* node.init -- initialise a node (of a grid)                                 */
/*----------------------------------------------------------------------------*/
void Task::vNodeInit(NNP_Task nnpTask)
{
ENTRY("vNodeInit");
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrNode) )
    THROW_INVALID_PARAM("node.init");
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  new(pNode) Node;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.new -- new a node (of a grid)                                         */
/*----------------------------------------------------------------------------*/
void Task::vNodeNew(NNP_Task nnpTask)
{
ENTRY("vNodeNew");
  
  i8 iNodeNo;
  
  if ( ! nnpTask->m_xParamStack.bPop(iNodeNo) )
    THROW_INVALID_PARAM("node.new");
  
  Node* pNode = new Node;
  ASSERT(CN_NODE == pNode->eClass());

  pNode->vNodeNo(iNodeNo);
  
  nnpTask->m_xParamStack.vPush(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.no -- push node number of node (of a grid)                            */
/*----------------------------------------------------------------------------*/
void Task::vNodeNo(NNP_Task nnpTask)
{
ENTRY("vNodeNo");
  
  Object* pPtrNode;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrNode) )
    THROW_INVALID_PARAM("node.no");
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  nnpTask->m_xParamStack.vPush(pNode->iNodeNo());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.status -- get status of this node (of a grid)                         */
/*----------------------------------------------------------------------------*/
void Task::vNodeStatus(NNP_Task nnpTask)
{
  ENTRY("vNodeStatus");

  s256 s;
  
  if (Region::bTest(gxArguments.m_sRegionName))
    String::vSprint(s.a(),"RapidsSE instance %s running", gxArguments.m_sRegionName.a());
  else
    String::vSprint(s.a(),"RapidsSE instance %s not running",gxArguments.m_sRegionName.a());
  
  nnpTask->m_xParamStack.vPush(s);
  
  EXIT();
}

// nodecursor: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* nodecursor.begin -- begin using node (of grid) cursor                      */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorBegin(NNP_Task nnpTask)
{
ENTRY("vNodeCursorBegin");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_PARAM("nodeCursor.begin");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodecursor.delete -- delete a node (of grid) cursor                        */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorDelete(NNP_Task nnpTask)
{
ENTRY("vNodeCursorDelete");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_PARAM("nodeCursor.delete");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  delete pNodeCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodecursor.end -- end using node (of grid) cursor                          */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorEnd(NNP_Task nnpTask)
{
ENTRY("vNodeCursorEnd");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_PARAM("nodecursor.end");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.first -- set to first and push node (of grid)                   */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorFirst(NNP_Task nnpTask)
{
ENTRY("vNodeCursorFirst");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_PARAM("nodeCursor.first");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vFirst();
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.get -- push current node (of grid)                              */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorGet(NNP_Task nnpTask)
{
ENTRY("vNodeCursorGet");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_PARAM("nodeCursor.get");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.init -- init a node cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorInit(NNP_Task nnpTask)
{
ENTRY("vNodeCursorInit");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_PARAM("nodeCursor.init");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  new(pNodeCursor) NodeCursor(gpGrid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.new -- new a node (of grid) cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorNew(NNP_Task nnpTask)
{
ENTRY("vNodeCursorNew");
  
  NodeCursor* pNodeCursor = new NodeCursor(gpGrid);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  nnpTask->m_xParamStack.vPush(pNodeCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.next -- advance to next and get node (of grid)                  */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorNext(NNP_Task nnpTask)
{
ENTRY("vNodeCursorNext");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrNodeCursor) )
    THROW_INVALID_PARAM("nodeCursor.next");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vNext();
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pObject);
  
  EXIT();
}

// object: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* object.dump -- dump the given object                                       */
/*----------------------------------------------------------------------------*/
void Task::vObjectDump(NNP_Task nnpTask)
{
  ENTRY("vObjectDump");
  
  Object* pObject;
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("object.dump");
  
  eCN eClass = pObject->eClass();
  
  pDUMPFUN* pDumpFun =gpDump[eClass];
  
  (pDumpFun)(pObject->eClass(), pObject, pSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* object.wordbody -- mark object as a part of a wordbody                     */
/*----------------------------------------------------------------------------*/
void Task::vObjectWordBody(NNP_Task nnpTask)
{
  ENTRY("vWordBody");
  
  Object* pObject;
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("object.wordbody");
  
  pObject->vWordBody(true);
  
  EXIT();
}

// process: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* process.getpid -- get pid of this process                                  */
/*----------------------------------------------------------------------------*/
void Task::vProcessGetPid(NNP_Task nnpTask)
{
ENTRY("vProcessGetPid");
  
  i8 iPid = Process::iGetPid();
  
  nnpTask->m_xParamStack.vPush(iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process.testpid -- test pid on stack                                       */
/*----------------------------------------------------------------------------*/
void Task::vProcessTestPid(NNP_Task nnpTask)
{
ENTRY("vProcessTestPid");

  i8 iPid;
  if ( ! nnpTask->m_xParamStack.bPop(iPid) )
    THROW_INVALID_PARAM("process.testpid");
  
  i8 iExists = Process::bTestPid(iPid);
  
  nnpTask->m_xParamStack.vPush(iExists);
  
  EXIT();
}

// pstack: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Task::vPstackDepth(NNP_Task nnpTask)
{
  ENTRY("vPstackDepth");
  
  i8 iDepth = nnpTask->m_xParamStack.iValues();
  
  nnpTask->m_xParamStack.vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vPstackDrop(NNP_Task nnpTask)
{
  ENTRY("vPstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xParamStack.bPop(xValA) )
    THROW_INVALID_PARAM("drop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 2drop -- discard top two values off stack                                  */
/*----------------------------------------------------------------------------*/
void Task::vPstackDropTwo(NNP_Task nnpTask)
{
  ENTRY("vPstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  ValWsp xValB;
  if ( ! nnpTask->m_xParamStack.bPop(xValA,xValB) )
    THROW_INVALID_PARAM("2drop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of param stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vPstackDup(NNP_Task nnpTask)
{
  ENTRY("vPstackDup");
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xParamStack.bPop(xValA) )
    THROW_INVALID_PARAM("dup");
  
  nnpTask->m_xParamStack.vPush(xValA);
  nnpTask->m_xParamStack.vPush(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ?dup -- dup top of param stack if non zero                                 */
/*----------------------------------------------------------------------------*/
void Task::vPstackDupNonZero(NNP_Task nnpTask)
{
  ENTRY("vPstackDupNonZero");
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xParamStack.bPop(xValA) )
    THROW_INVALID_PARAM("?dup");
  
  if (xValA.iAsInt()) {
    nnpTask->m_xParamStack.vPush(xValA);
    nnpTask->m_xParamStack.vPush(xValA);
  }
  else
    nnpTask->m_xParamStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dump -- dump top of param stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vPstackDump(NNP_Task nnpTask)
{
  ENTRY("vPstackDump");
  
  s8K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->m_xParamStack.bPop(xValWsp) )
    THROW_INVALID_PARAM("dump");
  
  xValWsp.vToString(sWorkspace.a());
  char* aClass = xValWsp.aClass();
  
  eCN eClass = xValWsp.eClass();
  
  if (CN_PTRVAL == eClass) {
    PtrVal* pPtrVal = xValWsp.pAsPtrVal();
    Object* pObject = pPtrVal->pObject();
    if (pObject) {
      eCN ePtdClass   = pObject->eClass();
      vTrace("[%s] %s (%s %s)\n",
             nnpTask->m_sName.a(),sWorkspace.a(),aClass,pCN[ePtdClass]);
    }
    else {
      vTrace("[%s] nullptr\n",nnpTask->m_sName.a());
    }
  }
  else {
    vTrace("[%s] %s (%s)\n",
           nnpTask->m_sName.a(),sWorkspace.a(),aClass);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nip -- drop the 2nd value from the stack ( B A -- A )                      */
/*----------------------------------------------------------------------------*/
void Task::vPstackNip(NNP_Task nnpTask)
{
  ENTRY("vPstackNip");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xParamStack.bPop(xValA,xValB) )
    THROW_INVALID_PARAM("nip");
  
  nnpTask->m_xParamStack.vPush(&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- place copy of nth value in stack onto stack                        */
/*----------------------------------------------------------------------------*/
void Task::vPstackPick(NNP_Task nnpTask)
{
  ENTRY("vPstackPick");
  
  i8 iPick;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iPick) )
    THROW_INVALID_PARAM("pick");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xParamStack.vPeek(iPick,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reverse -- reverse stack values ( ... D C B A n -- A B C D ... )           */
/*----------------------------------------------------------------------------*/
void Task::vPstackReverse(NNP_Task nnpTask)
{
  ENTRY("vPstackReverse");
  
  ValWsp xVal0;
  ValWsp xVal1;
  ValWsp xVal2;
  ValWsp xVal3;
  ValWsp xVal4;
  ValWsp xVal5;
  ValWsp xVal6;
  ValWsp xVal7;
  
  ValWsp* apValWsp[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! nnpTask->m_xParamStack.bPop(nValues) )
    THROW_INVALID_PARAM("reverse");
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! nnpTask->m_xParamStack.bPop(apValWsp[i]) )
      THROW_INVALID_PARAM("reverse");
  }

  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_xParamStack.vPush(apValWsp[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* roll -- roll top stack values at n ( ... D C B A n -- A ... D C B )        */
/*----------------------------------------------------------------------------*/
void Task::vPstackRoll(NNP_Task nnpTask)
{
  ENTRY("vPstackRoll");
  
  ValWsp xValA;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_xParamStack.bPop(nValues) )
    THROW_INVALID_PARAM("roll");
  
  if ( ! nnpTask->m_xParamStack.bPop(xValA) )
    THROW_INVALID_PARAM("roll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xParamStack.bPop(xValX) )
      THROW_INVALID_PARAM("roll");
    
    nnpTask->m_xReturnStack.vPush(&xValX);
  }
  
  nnpTask->m_xParamStack.vPush(&xValA);
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValX) )
      THROW_INVALID_PARAM("roll");
    
    nnpTask->m_xParamStack.vPush(&xValX);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -roll -- inverse roll top stack values at n ( Z ... B A n -- ... B A Z )   */
/*----------------------------------------------------------------------------*/
void Task::vPstackRollInv(NNP_Task nnpTask)
{
  ENTRY("vPstackRollInv");
  
  ValWsp xValZ;
  ValWsp xValX;
  
  i8 nValues;
  if ( ! nnpTask->m_xParamStack.bPop(nValues) )
    THROW_INVALID_PARAM("-roll");
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xParamStack.bPop(xValX) )
      THROW_INVALID_PARAM("-roll");
    
    nnpTask->m_xReturnStack.vPush(&xValX);
  }
  
  if ( ! nnpTask->m_xParamStack.bPop(xValZ) )
    THROW_INVALID_PARAM("-roll");
  
  
  for (i8 iCount = 0; iCount < nValues; iCount++) {
    if ( ! nnpTask->m_xReturnStack.bPop(xValX) )
      THROW_INVALID_PARAM("-roll");
    
    nnpTask->m_xParamStack.vPush(&xValX);
  }
  
  nnpTask->m_xParamStack.vPush(&xValZ);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values ( C B A -- A C B )                              */
/*----------------------------------------------------------------------------*/
void Task::vPstackRot(NNP_Task nnpTask)
{
  ENTRY("vPstackRot");
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->m_xParamStack.bPop(xValA,xValB,xValC) )
    THROW_INVALID_PARAM("rot");
  
  nnpTask->m_xParamStack.vPush(&xValA);
  nnpTask->m_xParamStack.vPush(&xValC);
  nnpTask->m_xParamStack.vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* -rot -- inverse rotate top 3 values ( C B A -- B A C )                     */
/*----------------------------------------------------------------------------*/
void Task::vPstackRotInv(NNP_Task nnpTask)
{
  ENTRY("vPstackRotInv");
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->m_xParamStack.bPop(xValA,xValB,xValC) )
    THROW_INVALID_PARAM("rot");
  
  nnpTask->m_xParamStack.vPush(&xValB);
  nnpTask->m_xParamStack.vPush(&xValA);
  nnpTask->m_xParamStack.vPush(&xValC);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vPstackSwap(NNP_Task nnpTask)
{
  ENTRY("vPstackSwap");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xParamStack.bPop(xValA,xValB) )
    THROW_INVALID_PARAM("swap");
  
  nnpTask->m_xParamStack.vPush(&xValA);
  nnpTask->m_xParamStack.vPush(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >valset -- collapse values on pstack to a valset                           */
/*----------------------------------------------------------------------------*/
void Task::vPstackToValSet(NNP_Task nnpTask)
{
  ENTRY("vPstackToValSet");
  
  i8 nVal;
  
  if ( ! nnpTask->m_xParamStack.bPop(nVal) )
  THROW_INVALID_PARAM(">valset");
  
  if ( ! nnpTask->m_xParamStack.bGuard(nVal) )
  THROW_INVALID_PARAM(">valset");
  
  char* pCursor = nullptr;
  ValSet xValSet;
  
  ValWsp xVal;
  for (i8 iVal = 0; iVal < nVal; iVal++) {
    
    if ( ! nnpTask->m_xParamStack.bPop(xVal) )
    THROW_INVALID_PARAM(">valset");
    
    xValSet.vAppend(pCursor,xVal);
  }
  
  //  vTrace("ValSet %lld bytes\n",xValSet.iSize());
  
  nnpTask->m_xParamStack.vPush(xValSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >values -- expand valset to values on pstack                               */
/*----------------------------------------------------------------------------*/
void Task::vPstackToValues(NNP_Task nnpTask)
{
  ENTRY("vPstackToValues");
  
  ValSet xValSet;
  
  if ( ! nnpTask->m_xParamStack.bPop(xValSet) )
  THROW_INVALID_PARAM(">values");
  
  i8 nVal = 0;
  ValWsp* pValWsp[128];
  
  char* pCursor = nullptr;
  while (xValSet.bExtract(pCursor,pValWsp[nVal]))
  nVal++;
  
  for (i8 iVal = nVal - 1; iVal >= 0; iVal--)
  nnpTask->m_xParamStack.vPush(pValWsp[iVal]);
  
  nnpTask->m_xParamStack.vPush(nVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tuck -- copy the first stack value below second ( B A -- A B A )           */
/*----------------------------------------------------------------------------*/
void Task::vPstackTuck(NNP_Task nnpTask)
{
  ENTRY("vPstackTuck");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xParamStack.bPop(xValA,xValB) )
    THROW_INVALID_PARAM("tuck");
  
  nnpTask->m_xParamStack.vPush(&xValA);
  nnpTask->m_xParamStack.vPush(&xValB);
  nnpTask->m_xParamStack.vPush(&xValA);
  
  EXIT();
}

// queue: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* queue.append -- append onto queue                                          */
/*----------------------------------------------------------------------------*/
void Task::vQueueAppend(NNP_Task nnpTask)
{
ENTRY("vQueueAppend");
  
  Object* pObject;
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject,xValWsp) )
    THROW_INVALID_PARAM("queue.append");
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  ASSERT(CN_QUEUE == pObject->eClass());
  
  pQueue->bAppend(&xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* q.dump -- queue dump                                                       */
/*----------------------------------------------------------------------------*/
void Task::vQueueDump(NNP_Task nnpTask)
{
ENTRY("vQueueDump");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("q.dump");
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  pQueue->vDump(pSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.init -- queue init                                                   */
/*----------------------------------------------------------------------------*/
void Task::vQueueInit(NNP_Task nnpTask)
{
ENTRY("vQueueInit");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("queue.init");
    
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  new(pQueue) Queue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.pop -- queue pop                                                     */
/*----------------------------------------------------------------------------*/
void Task::vQueuePop(NNP_Task nnpTask)
{
ENTRY("vQueuePop");
  
  Object* pObject;
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("queue.pop");
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  
  if (pQueue->bPop(&xValWsp)) {
    nnpTask->m_xParamStack.vPush(&xValWsp);
    nnpTask->m_xParamStack.vPush((i8)1);
  }
  else
    nnpTask->m_xParamStack.vPush((i8)0);
  
  
  EXIT();
}

// registers: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* reg.pop -- pop register set                                                */
/*----------------------------------------------------------------------------*/
void Task::vRegPop(NNP_Task nnpTask)
{
  ENTRY("vRegPop");
  
  RegFrame* pRegFrame = nnpTask->m_pRegFrame;
  
  if ( ! pRegFrame )
    THROW_INVALID_PARAM("reg.pop");
  
  nnpTask->m_pRegFrame = nnpTask->m_pRegFrame->pNext();
  
  delete pRegFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg.push -- push a register set                                            */
/*----------------------------------------------------------------------------*/
void Task::vRegPush(NNP_Task nnpTask)
{
ENTRY("vRegPush");
  
  RegFrame* pRegFrame = new RegFrame;
  
  pRegFrame->vNext(nnpTask->m_pRegFrame);
  
  nnpTask->m_pRegFrame = pRegFrame;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg0 -- push reg0 onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg0(NNP_Task nnpTask)
{
ENTRY("vRegReg0");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("reg0");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(0);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg1 -- push reg1 onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg1(NNP_Task nnpTask)
{
  ENTRY("vRegReg1");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("reg1");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(1);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg2 -- push reg2 onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg2(NNP_Task nnpTask)
{
  ENTRY("vRegReg2");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("reg2");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(2);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg3 -- push reg3 onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg3(NNP_Task nnpTask)
{
  ENTRY("vRegReg3");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("reg3");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(3);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg4 -- push reg4 onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg4(NNP_Task nnpTask)
{
  ENTRY("vRegReg4");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("reg4");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(4);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg5 -- push reg5 onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg5(NNP_Task nnpTask)
{
  ENTRY("vRegReg5");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("reg5");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(5);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg6 -- push reg6 onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg6(NNP_Task nnpTask)
{
  ENTRY("vRegReg6");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("reg6");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(6);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg7 -- push reg7 onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg7(NNP_Task nnpTask)
{
  ENTRY("vRegReg7");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("reg7");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(7);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

// number: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* number.magnitude -- get magnitude of number                                */
/*----------------------------------------------------------------------------*/
void Task::vNumberMagnitude(NNP_Task nnpTask)
{
  ENTRY("vNumberMagnitude");
  
  i8 iNumber;
  i8 iScale;
  
  if ( ! nnpTask->m_xParamStack.bPop(iNumber) )
    THROW_INVALID_PARAM("number.magnitude");
  
  if ( ! nnpTask->m_xParamStack.bPop(iScale) )
    THROW_INVALID_PARAM("number.magnitude");
  
  s256 s;
  
  Magnitudes::vMagnitude(iNumber, iScale, s.a());
  
  nnpTask->m_xParamStack.vPush(s);
  
  EXIT();
}

// object: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* reg8 -- push reg8 onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg8(NNP_Task nnpTask)
{
  ENTRY("vRegReg8");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("reg8");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(8);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* reg9 -- push reg9 onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegReg9(NNP_Task nnpTask)
{
  ENTRY("vRegReg9");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("reg9");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(9);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regA -- push regA onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegA(NNP_Task nnpTask)
{
  ENTRY("vRegRegA");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("regA");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(10);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regB -- push regB onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegB(NNP_Task nnpTask)
{
  ENTRY("vRegRegB");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("regB");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(11);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regC -- push regC onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegC(NNP_Task nnpTask)
{
  ENTRY("vRegRegC");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("regC");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(12);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regD -- push regD onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegD(NNP_Task nnpTask)
{
  ENTRY("vRegRegD");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("regD");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(13);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regE -- push regE onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegE(NNP_Task nnpTask)
{
  ENTRY("vRegRegE");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("regE");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(14);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regF -- push regF onto pstack                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegRegF(NNP_Task nnpTask)
{
  ENTRY("vRegRegF");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("regF");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(15);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* regN -- push reg[n] onto pstack                                            */
/*----------------------------------------------------------------------------*/
void Task::vRegRegNth(NNP_Task nnpTask)
{
  ENTRY("vRegRegNth");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM("regN");
  
  i8 iReg;
  if ( ! nnpTask->m_xParamStack.bPop(iReg) )
    THROW_INVALID_PARAM("regN");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(iReg);
  
  nnpTask->m_xParamStack.vPush(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg0 -- pop pstack into reg0                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg0(NNP_Task nnpTask)
{
ENTRY("vRegToReg0");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">reg0");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(0);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg1 -- pop pstack into reg1                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg1(NNP_Task nnpTask)
{
  ENTRY("vRegToReg1");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">reg1");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(1);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg2 -- pop pstack into reg2                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg2(NNP_Task nnpTask)
{
  ENTRY("vRegToReg2");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">reg2");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(2);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg3 -- pop pstack into reg3                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg3(NNP_Task nnpTask)
{
  ENTRY("vRegToReg3");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">reg3");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(3);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg4 -- pop pstack into reg4                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg4(NNP_Task nnpTask)
{
  ENTRY("vRegToReg4");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">reg4");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(4);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg5 -- pop pstack into reg5                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg5(NNP_Task nnpTask)
{
  ENTRY("vRegToReg5");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">reg5");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(5);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg6 -- pop pstack into reg6                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg6(NNP_Task nnpTask)
{
  ENTRY("vRegToReg6");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">reg6");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(6);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg7 -- pop pstack into reg7                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg7(NNP_Task nnpTask)
{
  ENTRY("vRegToReg7");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">reg7");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(7);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg8 -- pop pstack into reg8                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg8(NNP_Task nnpTask)
{
  ENTRY("vRegToReg8");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">reg8");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(8);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >reg9 -- pop pstack into reg9                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToReg9(NNP_Task nnpTask)
{
  ENTRY("vRegToReg9");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">reg9");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(9);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regA -- pop pstack into regA                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegA(NNP_Task nnpTask)
{
  ENTRY("vRegToRegA");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">regA");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(10);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regB -- pop pstack into regB                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegB(NNP_Task nnpTask)
{
  ENTRY("vRegToRegB");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">regB");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(11);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regC -- pop pstack into regC                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegC(NNP_Task nnpTask)
{
  ENTRY("vRegToRegC");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">regC");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(12);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regD -- pop pstack into regD                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegD(NNP_Task nnpTask)
{
  ENTRY("vRegToRegD");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">regD");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(13);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regE -- pop pstack into regE                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegE(NNP_Task nnpTask)
{
  ENTRY("vRegToRegE");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">regE");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(14);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regF -- pop pstack into regF                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegF(NNP_Task nnpTask)
{
  ENTRY("vRegToRegF");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">regF");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(15);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >regN -- pop pstack into regN                                              */
/*----------------------------------------------------------------------------*/
void Task::vRegToRegNth(NNP_Task nnpTask)
{
  ENTRY("vRegToRegNth");
  
  if ( ! nnpTask->m_pRegFrame )
    THROW_INVALID_PARAM(">regN");
  
  i8 iReg;
  if ( ! nnpTask->m_xParamStack.bPop(iReg) )
    THROW_INVALID_PARAM(">regNth");
  
  ValReg* pValReg = nnpTask->m_pRegFrame->pValReg(iReg);
  
  nnpTask->m_xParamStack.bPop(pValReg);
  
  EXIT();
}

// rowbuffer: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* rowbuffer.append -- attempt to append row onto row buffer                  */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferAppend(NNP_Task nnpTask)
{
ENTRY("vRowBufferAppend");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowBuffer) )
    THROW_INVALID_PARAM("rowbuffer.append");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());

  Object* pPtrTxRef;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxRef) )
    THROW_INVALID_PARAM("rowbuffer.append");
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  s8K sRecord;
  if ( ! nnpTask->m_xParamStack.bPop(sRecord) )
    THROW_INVALID_PARAM("rowbuffer.append");

  i8 iLength = sRecord.iLength();
  
  bool b = pRowBuffer->bInsertRow(pTxRef,sRecord.a(),iLength);
  
  if (b) {
    nnpTask->m_xParamStack.vPush((i8)1);
  }
  else {
    nnpTask->m_xParamStack.vPush(sRecord);
    nnpTask->m_xParamStack.vPush((i8)0);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.appendRecord -- attempt to append record onto row buffer         */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferAppendRecord(NNP_Task nnpTask)
{
  ENTRY("vRowBufferAppendRecord");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowBuffer) )
    THROW_INVALID_PARAM("rowBuffer.appendRecord");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  Object* pPtrTxRef;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxRef) )
    THROW_INVALID_PARAM("rowBuffer.appendRecord");
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  Object* pPtrFile;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrFile) )
    THROW_INVALID_PARAM("rowBuffer.appendRecord");
  
  File* pFile = _pFile(pPtrFile);
  
  char* aRecord = pFile->pRecord()->pWorkArea();
  
//  i8 iLength = String::iLength(aRecord);
  i8 iLength = pFile->iRecordLength();
  
  bool b = pRowBuffer->bInsertRow(pTxRef,aRecord,iLength);
  
  if (b) {
    nnpTask->m_xParamStack.vPush((i8)1);
  }
  else {
    nnpTask->m_xParamStack.vPush((i8)0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.dump -- dump the row buffer                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferDump(NNP_Task nnpTask)
{
  ENTRY("vRowBufferDump");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowBuffer) )
    THROW_INVALID_PARAM("rowbuffer.dump");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());

  Row* pRow = pRowBuffer->pFirstRow();
  
  while (pRow) {
    pRow->vDump();
    pRow = pRowBuffer->pNextRow(pRow);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowbuffer.new -- new a row buffer                                          */
/*----------------------------------------------------------------------------*/
void Task::vRowBufferNew(NNP_Task nnpTask)
{
  ENTRY("vRowBufferNew");
  
  RowBuffer* pRowBuffer = new RowBuffer;
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  nnpTask->m_xParamStack.vPush(pRowBuffer);
  
  EXIT();
}

// rowcursor: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* rowcursor.begin -- begin using row cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorBegin(NNP_Task nnpTask)
{
ENTRY("vRowCursorBegin");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.begin");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  i8 iPartn;
  if ( ! nnpTask->m_xParamStack.bPop(iPartn) )
    THROW_INVALID_PARAM("rowcursor.begin");
  
  pRowCursor->vBegin(iPartn);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.delete -- delete a row cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorDelete(NNP_Task nnpTask)
{
ENTRY("vRowCursorDelete");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.delete");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pRowCursor);
  delete pRowCursor;
  
  Heap::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.end -- end using row cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorEnd(NNP_Task nnpTask)
{
ENTRY("vRowCursorEnd");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.end");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.first -- set to first and get row                                */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorFirst(NNP_Task nnpTask)
{
ENTRY("vRowCursorFirst");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.first");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vFirst();
  
  i8 bRow = pRowCursor->pRowAsStr() ? 1 : 0;
  
  nnpTask->m_xParamStack.vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.get -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorGet(NNP_Task nnpTask)
{
ENTRY("vRowCursorGet");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.get");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  s8K xRow;
  xRow = pRowCursor->pRowAsStr();
  
  nnpTask->m_xParamStack.vPush(xRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorInit(NNP_Task nnpTask)
{
ENTRY("vRowCursorInit");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.init");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  new(pRowCursor) RowCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.new -- new an row cursor                                         */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorNew(NNP_Task nnpTask)
{
ENTRY("vRowCursorNew");
  
  RowCursor* pRowCursor = new RowCursor;
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  nnpTask->m_xParamStack.vPush(pRowCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.next -- advance to next and get row                              */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorNext(NNP_Task nnpTask)
{
ENTRY("vRowCursorNext");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.next");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vNext();
  
  i8 bRow = pRowCursor->pRowAsStr() ? 1 : 0;
  
  nnpTask->m_xParamStack.vPush(bRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.rowBuffer -- get current rowBuffer                               */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorRowBuffer(NNP_Task nnpTask)
{
  ENTRY("vRowCursorRowBuffer");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowCursor) )
  THROW_INVALID_PARAM("rowcursor.rowBuffer");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  RowBuffer* pRowBuffer;
  pRowBuffer = pRowCursor->pRowBuffer();
  
  nnpTask->m_xParamStack.vPush(pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.table@ -- fetch table from row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableFetch(NNP_Task nnpTask)
{
ENTRY("vRowCursorTableFetch");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.table@");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable = pRowCursor->pTable();
  
  nnpTask->m_xParamStack.vPush(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.table! -- store table into row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableStore(NNP_Task nnpTask)
{
ENTRY("vRowCursorTableStore");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.table!");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("rowcursor.table!");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pRowCursor->vTable(pTable);
  
  EXIT();
}

// rstack: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* rdepth -- put number of values in return stack onto param stack            */
/*----------------------------------------------------------------------------*/
void Task::vRstackDepth(NNP_Task nnpTask)
{
  ENTRY("vRstackDepth");
  
  i8 iDepth = nnpTask->m_xReturnStack.iValues();
  
  nnpTask->m_xParamStack.vPush(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rdrop -- discard top value off rstack                                      */
/*----------------------------------------------------------------------------*/
void Task::vRstackDrop(NNP_Task nnpTask)
{
  ENTRY("vRstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->m_xReturnStack.bPop(xValA) )
    THROW_INVALID_RETURN("rdrop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r@ -- place copy of top value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstackFetch(NNP_Task nnpTask)
{
  ENTRY("vRstackFetch");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(0,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rpick -- place copy of nth value in return stack onto param stack          */
/*----------------------------------------------------------------------------*/
void Task::vRstackPick(NNP_Task nnpTask)
{
  ENTRY("vRstackPick");
  
  i8 iPick;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iPick) )
    THROW_INVALID_PARAM("rpick");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rreverse -- reverse rstack values ( ... D C B A n -- A B C D ... )         */
/*----------------------------------------------------------------------------*/
void Task::vRstackReverse(NNP_Task nnpTask)
{
  ENTRY("vRstackReverse");
  
  ValWsp xVal0;
  ValWsp xVal1;
  ValWsp xVal2;
  ValWsp xVal3;
  ValWsp xVal4;
  ValWsp xVal5;
  ValWsp xVal6;
  ValWsp xVal7;
  
  ValWsp* apValWsp[8] = {
    &xVal0, &xVal1, &xVal2, &xVal3, &xVal4, &xVal5, &xVal6, &xVal7
  };
  
  i8 nValues;
  if ( ! nnpTask->m_xParamStack.bPop(nValues) )
    THROW_INVALID_PARAM("areverse");
  
  for (i8 i = 0; i < nValues; i++) {
    if ( ! nnpTask->m_xReturnStack.bPop(apValWsp[i]) )
      THROW_INVALID_PARAM("areverse");
  }
  
  for (i8 i = 0; i < nValues; i++) {
    nnpTask->m_xReturnStack.vPush(apValWsp[i]);
  }
}

/*----------------------------------------------------------------------------*/
/* r> -- move value from rstack to pstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vRstackToP(NNP_Task nnpTask)
{
  ENTRY("vRstackToP");
  
  ValWsp xVal;
  if ( ! nnpTask->m_xReturnStack.bPop(xVal) )
    THROW_INVALID_PARAM("r>");
  
  nnpTask->m_xParamStack.vPush(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >r -- move value from pstack to rstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vRstackToR(NNP_Task nnpTask)
{
  ENTRY("vRstackToR");
  
  ValWsp xValOpaque;
  if ( ! nnpTask->m_xParamStack.bPop(xValOpaque) )
    THROW_INVALID_PARAM(">r");
  
  nnpTask->m_xReturnStack.vPush(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r0 -- place copy of 0th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack0(NNP_Task nnpTask)
{
  ENTRY("vRstack0");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(0,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r1 -- place copy of 1st value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack1(NNP_Task nnpTask)
{
  ENTRY("vRstack1");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(1,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r2 -- place copy of 2nd value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack2(NNP_Task nnpTask)
{
  ENTRY("vRstack2");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(2,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r3 -- place copy of 3rd value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack3(NNP_Task nnpTask)
{
  ENTRY("vRstack3");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(3,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r4 -- place copy of 4th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack4(NNP_Task nnpTask)
{
  ENTRY("vRstack4");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(4,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r5 -- place copy of 0th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack5(NNP_Task nnpTask)
{
  ENTRY("vRstack5");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(5,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r6 -- place copy of 6th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack6(NNP_Task nnpTask)
{
  ENTRY("vRstack6");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(6,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r7 -- place copy of 7th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack7(NNP_Task nnpTask)
{
  ENTRY("vRstack7");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(7,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r8 -- place copy of 8th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack8(NNP_Task nnpTask)
{
  ENTRY("vRstack8");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(8,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r9 -- place copy of 9th value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstack9(NNP_Task nnpTask)
{
  ENTRY("vRstack9");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(9,&xValWsp);
  
  nnpTask->m_xParamStack.vPush(xValWsp);
  
  EXIT();
}

// seitf: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* seitf.count -- get count of SE interface's                                 */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCount(NNP_Task nnpTask)
{
  ENTRY("vSEitfCount");
  
  Object* pPtrSEitf;
  
  nnpTask->m_xParamStack.vPush(gpSEitfList->iCount());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.delete -- delete SE interface                                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDelete(NNP_Task nnpTask)
{
ENTRY("vSEitfDelete");
  
  Object* pPtrSEitf;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitf) )
    THROW_INVALID_PARAM("seitf.delete");

  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  delete pSEitf;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.dump -- dump of SE interface                                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDump(NNP_Task nnpTask)
{
ENTRY("vSEitfDump");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitf) )
    THROW_INVALID_PARAM("seitf.dump");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  pSEitf->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.new -- new SE interface                                              */
/*----------------------------------------------------------------------------*/
void Task::vSEitfNew(NNP_Task nnpTask)
{
ENTRY("vSEitfNew");
  
  SEitf* pSEitf = new SEitf(0);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrSEitf = pSEitf;
  
  nnpTask->m_xParamStack.vPush(pPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.req@ -- fetch SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqFetch(NNP_Task nnpTask)
{
ENTRY("vSEitfReqFetch");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitf) )
    THROW_INVALID_PARAM("seitf.req@");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue = pSEitf->pRequestQueue();
  
  nnpTask->m_xParamStack.vPush(pPtrQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.req! -- store SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqStore(NNP_Task nnpTask)
{
ENTRY("vSEitfReqStore");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitf) )
    THROW_INVALID_PARAM("seitf.req!");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrQueue) )
    THROW_INVALID_PARAM("seitf.req!");
  
  Queue* pQueue = _pQueue(pPtrQueue);
  ASSERT(CN_QUEUE == pQueue->eClass());
  
  pSEitf->vRequestQueue(pQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp@ -- fetch SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspFetch(NNP_Task nnpTask)
{
ENTRY("vSEitfRspFetch");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitf) )
    THROW_INVALID_PARAM("seitf.rsp@");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue = pSEitf->pResponseQueue();
  
  nnpTask->m_xParamStack.vPush(pPtrQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp! -- store SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspStore(NNP_Task nnpTask)
{
ENTRY("vSEitfRspStore");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitf) )
    THROW_INVALID_PARAM("seitf.rsp!");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrQueue;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrQueue) )
    THROW_INVALID_PARAM("seitf.rsp!");
  
  Queue* pQueue = _pQueue(pPtrQueue);
  ASSERT(CN_QUEUE == pQueue->eClass());
  
  pSEitf->vResponseQueue(pQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.table@ -- fetch table from SEitf                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableFetch(NNP_Task nnpTask)
{
  ENTRY("vSEitfTableFetch");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitf) )
    THROW_INVALID_PARAM("seitf.rsp!");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  nnpTask->m_xParamStack.vPush(pSEitf->pCurrentTable());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tableDeref -- store table in SEitf and inc reference                 */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableDeref(NNP_Task nnpTask)
{
  ENTRY("vSEitfRspStore");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitf) )
    THROW_INVALID_PARAM("seitf.tableDeref");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Table* pTable = pSEitf->pCurrentTable();
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vRefDec();

  pSEitf->vCurrentTable(nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.tableRef -- store table in SEitf and inc reference                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfTableRef(NNP_Task nnpTask)
{
  ENTRY("vSEitfTableRef");
  
  Object* pPtrSEitf;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitf) )
    THROW_INVALID_PARAM("seitf.tableRef");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  Object* pPtrTable;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("seitf.tableRef");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vRefInc();
  
  pSEitf->vCurrentTable(pTable);
  
  EXIT();
}

// seitfcursor: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* seitfcursor.begin -- begin using seitf currsor                             */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorBegin(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorBegin");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.begin");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());

  pSEitfCursor->vBegin();
  pSEitfCursor->vFirst();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.delete -- delete an seitf cursor                               */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorDelete(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorDelete");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.delete");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pSEitfCursor);
  delete pSEitfCursor;
  
  Heap::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.end -- end using seitf cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorEnd(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorEnd");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.end");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.first -- set to first and get seitf                            */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorFirst(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorFirst");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.first");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vFirst();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.get -- get current seitf                                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorGet(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorGet");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.get");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.init -- init an seitf cursor                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorInit(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorInit");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.init");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  s32 sName;
  sName = "seitflist";
  Word* pWord = nnpTask->pFindWord(sName);
  ASSERT(pWord);
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(pWord->pDataArea());
  
  
  new(pSEitfCursor) SEitfCursor(pSEitfList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.new -- new an seitf cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNew(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorNew");
  
  Object* pPtrSEitfList;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitfList) )
    THROW_INVALID_PARAM("seitfcursor.new");
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(Heap::pAllocate(CN_SEITFCURSOR));

  new(pSEitfCursor) SEitfCursor(pSEitfList);
  
  Object* pPtrSEitfCursor = pSEitfCursor;
  
  nnpTask->m_xParamStack.vPush(pPtrSEitfCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.next -- advance to next and get seitf                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNext(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorNext");
  
  Object* pPtrSEitfCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.next");
  
  SEitfCursor* pSEitfCursor = _pSEitfCursor(pPtrSEitfCursor);
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vNext();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pObject);
  
  EXIT();
}

// seitflist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* seitflist.dump -- dump of seitfList on param stack                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListDump(NNP_Task nnpTask)
{
ENTRY("vSEitfListDump");
  
  Object* pPtrSEitfList;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitfList) )
    THROW_INVALID_PARAM("seitflist.dump");
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  pSEitfList->vDump("SEitfList:", pSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitflist.init -- init of seitfList on param stack                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListInit(NNP_Task nnpTask)
{
ENTRY("vSEitfListInit");
  
  Object* pPtrSEitfList;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitfList) )
    THROW_INVALID_PARAM("seitflist.init");
  
  SEitfList* pSEitfList = _pSEitfList(pPtrSEitfList);
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  new(pSEitfList) SEitfList;
  
  EXIT();
}

// shell: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* shell.execute -- execute given shell command                               */
/*----------------------------------------------------------------------------*/
void Task::vShellExecute(NNP_Task nnpTask)
{
  ENTRY("vShellExecute");
  
  ValWsp xCommand;
  
  if ( ! nnpTask->m_xParamStack.bPop(xCommand) )
    THROW_INVALID_PARAM("shell.execute");
  
  SysCall xSysCall;
  
  i8 iExitStatus = Shell::iExecute(xCommand.sStr(),xSysCall);
  
  nnpTask->m_xParamStack.vPush(iExitStatus);
  
  EXIT();
}

// stack: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* astack.class -- get class of nth value on aux stack                        */
/*----------------------------------------------------------------------------*/
void Task::vAstackClass(NNP_Task nnpTask)
{
  ENTRY("vAstackClass");
  
  i8 nTh;
  if ( ! nnpTask->m_xParamStack.bPop(nTh) )
  THROW_INVALID_PARAM("astack.class");
  
  ValWsp xValWsp;
  
  nnpTask->m_xAuxStack.vPeek(nTh,&xValWsp);
  
  eCN eClass = xValWsp.eClass();
  
  nnpTask->m_xAuxStack.vPush(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* stack.class -- get class of nth value on stack                             */
/*----------------------------------------------------------------------------*/
void Task::vStackClass(NNP_Task nnpTask)
{
ENTRY("vStackClass");

  i8 nTh;
  if ( ! nnpTask->m_xParamStack.bPop(nTh) )
    THROW_INVALID_PARAM("stack.class");
  
  ValWsp xValWsp;
  
  nnpTask->m_xParamStack.vPeek(nTh,&xValWsp);
  
  eCN eClass = xValWsp.eClass();
  
  nnpTask->m_xParamStack.vPush(eClass);
  
  EXIT();
}

// steps: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* steps.list -- list steps                                                   */
/*----------------------------------------------------------------------------*/
void Task::vStepsList(NNP_Task nnpTask)
{
ENTRY("vStepsList");

  gpStepArray->vList();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* steps.reset -- reset steps                                                 */
/*----------------------------------------------------------------------------*/
void Task::vStepsReset(NNP_Task nnpTask)
{
  ENTRY("vStepsReset");
  
  gpStepArray->vReset();
  
  EXIT();
}

// string: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* strpad -- pad string to given length                                       */
/*----------------------------------------------------------------------------*/
void Task::vStringPad(NNP_Task nnpTask)
{
ENTRY("vStringPad");
  
  i8 iLength;

  ValWsp xStringA;
  
  // get the arguments
  if ( ! nnpTask->m_xParamStack.bPop(iLength,xStringA) )
    THROW_INVALID_PARAM("strpad");
  
  String::vPad(xStringA.sStr(),iLength,' ');
  xStringA.pAsStrVal()->vSize();
  
  nnpTask->m_xParamStack.vPush(xStringA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* streql -- test string equality                                             */
/*----------------------------------------------------------------------------*/
void Task::vStrEql(NNP_Task nnpTask)
{
ENTRY("vStrEql");
  
  ValWsp xStringA;
  ValWsp xStringB;
  
  // get the arguments
  if ( ! nnpTask->m_xParamStack.bPop(xStringB,xStringA) )
    THROW_INVALID_PARAM("streql");
  
  i8 iEqual = String::bEqualTo(xStringA.sStr(),xStringB.sStr());
  
  nnpTask->m_xParamStack.vPush(iEqual);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* strlen -- string length                                                    */
/*----------------------------------------------------------------------------*/
void Task::vStrLen(NNP_Task nnpTask)
{
ENTRY("vStrLen");
  
  i8 iLength;
  s8K sString;
  
  // get the arguments
  if ( ! nnpTask->m_xParamStack.bPop(sString) )
    THROW_INVALID_PARAM("strlen");
  
  iLength = String::iLength(sString.a());
  
  nnpTask->m_xParamStack.vPush(iLength);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* substr -- substring ( start length str == substr )                         */
/*----------------------------------------------------------------------------*/
//void Task::vSubStr(NNP_Task nnpTask)
//{
//ENTRY("vSubStr");
//  
//  i8 iLength;
//  i8 iStart;
//  ValWsp xString;
//  ValWsp xSubStr;
//  
//  StrVal* pStrVal = xString.pAsStrVal();
//  
//  // get the arguments
//  if ( ! nnpTask->m_xParamStack.bPop(iStart,iLength,xString) )
//    THROW_INVALID_PARAM("substr");
//  
//  // validate start index
//  if ( iStart < 0 || iStart >= pStrVal->iChars())
//    THROW("Invalid start index for: subStr");
//  
//  // handle to end of string case
//  if (-1 == iLength)
//    iLength = pStrVal->iChars() - iStart;
//  
//  if ( iLength <= 0 || iStart + iLength > pStrVal->iChars())
//    THROW("Invalid length for: substr");
//  
//  // compute substring
//  //xSubStr.vCopyString(xString.sStr() + iStartIndex,iLength);
//  
//  char* pStart = xString.sStr() + iStart;
//  
//  xSubStr.pAsStrVal()->vCopyString(pStart,iLength);
//  
//  nnpTask->m_xParamStack.vPush(&xSubStr);
//  
//  EXIT();
//}
void Task::vSubStr(NNP_Task nnpTask)
{
  ENTRY("vSubStr");
  
  i8 iLength;
  i8 iStart;
  s8K sString;
  s8K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_xParamStack.bPop(iStart,iLength,sString) )
    THROW_INVALID_PARAM("substr");
  
  i8 iHasLength = String::iLength(sString.a());
  
  // validate start index
  if ( iStart < 0 || iStart >= iHasLength)
    THROW("Invalid start index for: subStr");
  
  // handle to end of string case
  if (-1 == iLength)
    iLength = iHasLength - iStart;
  
  if ( iLength <= 0 || iStart + iLength > iHasLength)
    THROW("Invalid length for: substr");
  
  char* pStart = sString.a() + iStart;
  
  String::vCopyLen(sSubStr.a(),pStart,iLength);
  
  nnpTask->m_xParamStack.vPush(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* concat -- concat two strings                                               */
/*----------------------------------------------------------------------------*/
void Task::vConcat(NNP_Task nnpTask)
{
ENTRY("vConcat");
  
  ValWsp xStrR;
  ValWsp xStrB;
  ValWsp xStrA;
  
  if ( ! nnpTask->m_xParamStack.bPop(xStrB,xStrA) )
    THROW_INVALID_PARAM("concat");
  
  xStrR = xStrA;
  
  String::vConcat(xStrR.sStr(),xStrB.sStr());
  xStrR.pAsStrVal()->vSize();
  
  nnpTask->m_xParamStack.vPush(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* clip -- clip string ( length str -- str )                                  */
/*----------------------------------------------------------------------------*/
//void Task::vClip(NNP_Task nnpTask)
//{
//ENTRY("vClip");
//  
//  i8 iLength;
//  ValWsp xString;
//  ValWsp xSubStr;
//  
//  StrVal* pStrVal = xString.pAsStrVal();
//  
//  // get the arguments
//  if ( ! nnpTask->m_xParamStack.bPop(iLength,xString) )
//    THROW_INVALID_PARAM("clip");
//  
//  // validate length
//  i8 iStrLength = pStrVal->iChars();
//  
//  if ( iLength == 0 || llabs(iLength) >= iStrLength)
//    THROW("Invalid length for: clip");
//  
//  char* pStart;
//  
//  if (iLength > 0)
//    pStart = xString.sStr();
//  else
//    pStart = xString.sStr() + iStrLength - 1 - llabs(iLength);
//  
//  xSubStr.pAsStrVal()->vCopyString(pStart,llabs(iLength));
//  
//  nnpTask->m_xParamStack.vPush(&xSubStr);
//  
//  EXIT();
//}
void Task::vClip(NNP_Task nnpTask)
{
  ENTRY("vClip");
  
  i8 iLength;
  s8K sString;
  s8K sSubStr;
  
  // get the arguments
  if ( ! nnpTask->m_xParamStack.bPop(iLength,sString) )
    THROW_INVALID_PARAM("clip");
  
  // validate length
  i8 iHasLength = String::iLength(sString.a());
  
  if ( iLength == 0 || llabs(iLength) >= iHasLength)
    THROW("Invalid length for: clip");
  
  char* pStart;
  
  if (iLength > 0)
    pStart = sString.a();
  else
    pStart = sString.a() + iHasLength - 1 - llabs(iLength);
  
  String::vCopyLen(sSubStr.a(),pStart,llabs(iLength));
  
  nnpTask->m_xParamStack.vPush(sSubStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >DigitName -- convert to digit name                                        */
/*----------------------------------------------------------------------------*/
void Task::vToDigitName(NNP_Task nnpTask)
{
ENTRY("vToDigitName");
  
  i8 iDigit;
  
  // get the arguments
  if ( ! nnpTask->m_xParamStack.bPop(iDigit) )
    THROW_INVALID_PARAM(">DigitName");
  
  s32 sName;
  
  switch (iDigit) {
    case 0: sName = "zero";    break;
    case 1: sName = "one";     break;
    case 2: sName = "two";     break;
    case 3: sName = "three";   break;
    case 4: sName = "four";    break;
    case 5: sName = "five";    break;
    case 6: sName = "six";     break;
    case 7: sName = "seven";   break;
    case 8: sName = "eight";   break;
    case 9: sName = "nine";    break;
    default:sName = "invalid"; break;
  }

  nnpTask->m_xParamStack.vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prepend -- prepend a string before the other                               */
/*----------------------------------------------------------------------------*/
void Task::vPrepend(NNP_Task nnpTask)
{
  ENTRY("vPrepend");
  
  ValWsp xStrR;
  ValWsp xStrB;
  ValWsp xStrA;
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->m_xParamStack.bPop(xValB,xValA) )
  THROW_INVALID_PARAM("prepend");
  
  // If received INTEGER convert to STRING
  if (xValA.eClass() == CN_INTVAL) {
    s8K sString;
    xValA.vToString(sString.a());
    xStrA = sString.a();
  }
  else {
   xStrA = xValA;
  }
  
  // If received INTEGER convert to STRING
  if (xValB.eClass() == CN_INTVAL) {
    s8K sString;
    xValB.vToString(sString.a());
    xStrB = sString.a();
  }
  else {
   xStrB = xValB;
  }
  
  xStrR = xStrB;
  
  String::vConcat(xStrR.sStr(),xStrA.sStr());
  xStrR.pAsStrVal()->vSize();
  
  nnpTask->m_xParamStack.vPush(xStrR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rndStr -- generate random string of given length                           */
/*----------------------------------------------------------------------------*/
void Task::vRndStr(NNP_Task nnpTask)
{
  ENTRY("vRndStr");
  
  i8 iLength;
  if ( ! nnpTask->m_xParamStack.bPop(iLength) )
    THROW_INVALID_PARAM("rndStr");
  
  s8K sRnd;
  
  sRnd = Random::sRandomString(iLength).a();
  
  nnpTask->m_xParamStack.vPush(sRnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* s256.init -- initialise an s256                                            */
/*----------------------------------------------------------------------------*/
void Task::vS256Init(NNP_Task nnpTask)
{
  ENTRY("vS256Init");
  
  Object* pPtrStrVal;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrStrVal) )
    THROW_INVALID_PARAM("s256.init");
  
  StrVal* pStrVal = _pStrVal(pPtrStrVal);
  
  ASSERT(CN_STRVAL == pStrVal->eClass());
  
  new(pStrVal) StrVal(256);
  
  EXIT();
}

// table: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* table.append -- append buffer into table (from seitf)                      */
/*----------------------------------------------------------------------------*/
void Task::vTableAppend(NNP_Task nnpTask)
{
  ENTRY("vTableAppend");
  i8 iPartition;
  s8K sString;
  
  if ( ! nnpTask->m_xParamStack.bPop(sString) )
    THROW_INVALID_PARAM("table.append");
  
  Table* pTable = reinterpret_cast<Table*>(String::iToI8(sString.a()));
  ASSERT(CN_TABLE == pTable->eClass());
  
  if ( ! nnpTask->m_xParamStack.bPop(sString) )
    THROW_INVALID_PARAM("table.append");
  
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(String::iToI8(sString.a()));
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
  
  if ( ! nnpTask->m_xParamStack.bPop(sString) )
    THROW_INVALID_PARAM("table.append");
  
  iPartition = String::iToI8(sString.a());
  pTable->vAppendBuffer(iPartition,pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.appendBuffer -- append row buffer into table                         */
/*----------------------------------------------------------------------------*/
void Task::vTableAppendBuffer(NNP_Task nnpTask)
{
  ENTRY("vTableAppendBuffer");

  Object* pPtrTable;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("table.appendBuffer");
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iPartition = 0;
  if ( ! nnpTask->m_xParamStack.bPop(iPartition) )
    THROW_INVALID_PARAM("table.appendBuffer");
  
  Object* pPtrRowBuffer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrRowBuffer) )
    THROW_INVALID_PARAM("table.appendBuffer");
  RowBuffer* pRowBuffer = _pRowBuffer(pPtrRowBuffer);
  ASSERT(CN_ROWBUFFER == pRowBuffer->eClass());
    
  pTable->vAppendBuffer(iPartition,pRowBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.dic -- get table dictionary                                          */
/*----------------------------------------------------------------------------*/
void Task::vTableDic(NNP_Task nnpTask)
{
ENTRY("vTableDic");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("table.dic");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Dic* pDic = pTable->pDic();
  
  nnpTask->m_xParamStack.vPush(pDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.drop -- drop of table on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableDrop(NNP_Task nnpTask)
{
ENTRY("vTableDrop");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("table.drop");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table dump                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableDump(NNP_Task nnpTask)
{
ENTRY("vTableDump");
  
  vTrace("TableDump\n");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("table.dump");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.init -- init of table on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableInit(NNP_Task nnpTask)
{
ENTRY("vTableInit");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("table.init");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  new(pTable) Table;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.insert -- insert row into table on param stack                       */
/*----------------------------------------------------------------------------*/
void Task::vTableInsert(NNP_Task nnpTask)
{
ENTRY("vTableInsert");
  
  Object* pPtrTable;
  Object* pPtrTxRef;
  ValWsp xRow;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("table.insert");
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxRef) )
    THROW_INVALID_PARAM("table.insert");
  if ( ! nnpTask->m_xParamStack.bPop(xRow) )
    THROW_INVALID_PARAM("table.insert");

  i8 iPartition;
  if ( ! nnpTask->m_xParamStack.bPop(iPartition) )
    THROW_INVALID_PARAM("table.insert");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());

  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  pTable->vInsertRow(iPartition,
                     pTxRef,
                     xRow.pAsStrVal()->sStr(),
                     xRow.pAsStrVal()->iChars());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.name -- get table name                                               */
/*----------------------------------------------------------------------------*/
void Task::vTableName(NNP_Task nnpTask)
{
  ENTRY("vTableName");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("table.init");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  s32 sName;
  
  sName = pTable->pTableName();
  
  nnpTask->m_xParamStack.vPush(sName);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.ref -- has table got references?                                     */
/*----------------------------------------------------------------------------*/
void Task::vTableRef(NNP_Task nnpTask)
{
  ENTRY("vTableRef");
  
  Object* pPtrTable;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("table.ref");
  
  Table* pTable = _pTable(pPtrTable);
  
  bool bRef = pTable->bRefOut();
  
  nnpTask->m_xParamStack.vPush(bRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.truncate -- release all rows/buffers in table on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTableTruncate(NNP_Task nnpTask)
{
ENTRY("vTableTruncate");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("table.truncate");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vTruncate();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.space -- push the buffer space used bytable on param stack           */
/*----------------------------------------------------------------------------*/
void Task::vTableSpace(NNP_Task nnpTask)
{
ENTRY("vTableSpace");
  
  Object* pPtrTable;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTable) )
    THROW_INVALID_PARAM("table.spave");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iBuffers = pTable->iRowBuffers();
  i8 iSpace   = iBuffers * C_SIZE_POOL_BUFFER;
  s256 sMag;
  
  nnpTask->m_xParamStack.vPush(iSpace);
  
  EXIT();
}

// task: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* --task.acks -- decrement given tasks m_zAcks and push result               */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksDec(NNP_Task nnpTask)
{
  ENTRY("vTaskAcksFetch");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTask) )
    THROW_INVALID_PARAM("--task.acks");
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 iResult = --pGivenTask->m_zAcks;
  
  nnpTask->m_xParamStack.vPush(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks@ -- push current value of given tasks m_zAcks                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskAcksFetch");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTask) )
    THROW_INVALID_PARAM("task.acks@");
  Task* pGivenTask = _pTask(pPtrTask);
  
  nnpTask->m_xParamStack.vPush(*pGivenTask->m_zAcks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.acks! -- store value into given tasks m_zAcks                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskAcksStore(NNP_Task nnpTask)
{
  ENTRY("vTaskAcksStore");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTask) )
    THROW_INVALID_PARAM("task.acks!");
  Task* pGivenTask = _pTask(pPtrTask);
  
  i8 iInt;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iInt) )
    THROW_INVALID_PARAM("task.acks!");
  
  ASSERT(0 == *pGivenTask->m_zAcks);

  pGivenTask->m_zAcks = iInt;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment@ -- push comment of given task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskCommentFetch");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTask) )
    THROW_INVALID_PARAM("task.comment@");
  Task* pGivenTask = _pTask(pPtrTask);
  
  nnpTask->m_xParamStack.vPush(pGivenTask->m_sComment);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.comment! -- store comment into given task                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskCommentStore(NNP_Task nnpTask)
{
  ENTRY("vTaskCommentStore");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTask) )
    THROW_INVALID_PARAM("task.comment!");
  Task* pGivenTask = _pTask(pPtrTask);
  
  s32 sComment;
  if ( ! nnpTask->m_xParamStack.bPop(sComment) )
    THROW_INVALID_PARAM("task.comment!");
  
  pGivenTask->m_sComment = sComment;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dic -- push task dictionary onto param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskDic(NNP_Task nnpTask)
{
ENTRY("vTaskDic");
  
  Object* pPtrDic = &nnpTask->m_xTaskDic;
  
  nnpTask->m_xParamStack.vPush(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dump -- dump of task on param stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vTaskDump(NNP_Task nnpTask)
{
ENTRY("vTaskDump");
  
  Object* pObject;
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("task.dump");
  Task* pThatTask = _pTask(pObject);
  
  pThatTask->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.init -- init of task on param stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vTaskInit(NNP_Task nnpTask)
{
ENTRY("vTaskInit");
  
  Object* pPtrTask;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTask) )
    THROW_INVALID_PARAM("task.init");
  
  Task* pThatTask = _pTask(pPtrTask);
  ASSERT(CN_TASK == pThatTask->eClass());
  
  new(pThatTask) Task;
  
  Word* pWordThread = Word::pToWord(pThatTask);
  
  s32 sName;
  sName = pWordThread->sName();
  
  pThatTask->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameFetch(NNP_Task nnpTask)
{
ENTRY("vTaskNameFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.name@");
  Task* pThatTask = _pTask(pTaskObject);
  
  nnpTask->m_xParamStack.vPush(pThatTask->m_sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name! -- store task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameStore(NNP_Task nnpTask)
{
ENTRY("vTaskNameStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.name!");
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sName;
  if ( ! nnpTask->m_xParamStack.bPop(sName) )
    THROW_INVALID_PARAM("task.name!");
  
  pThatTask->m_sName = sName;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.new -- create and seed a new task                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskNew(NNP_Task nnpTask)
{
ENTRY("vTaskNew");
  //                                   pParent   sSeed     nTaskArgs
  if ( ! nnpTask->m_xParamStack.bGuard(CN_PTRVAL,CN_STRVAL,CN_INTVAL) )
    THROW_INVALID_PARAM("task.new");
  
  Object* pParent;
  if ( ! nnpTask->m_xParamStack.bPop(pParent) )
    THROW_INVALID_PARAM("task.new");
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  s32 sSeed;
  if ( ! nnpTask->m_xParamStack.bPop(sSeed) )
    THROW_INVALID_PARAM("task.new");
  char* aSeedWord = sSeed.a();
  
  i8 nTaskArgs;
  if ( ! nnpTask->m_xParamStack.bPop(nTaskArgs) )
    THROW_INVALID_PARAM("task.new");
  //ASSERT(! nTaskArgs);

  if ( ! nnpTask->m_xParamStack.bGuard(nTaskArgs) )
    THROW_INVALID_PARAM("task.new");
  
  // create new task (will be automatically appended to task list)
  NNP_Task nnpNewTask = new Task;
  
  nnpNewTask->m_pParentTask = pParentTask;
  nnpNewTask->vSeedWord(aSeedWord);
  
  // copy task arguments to new task (preserving stack order)
  ValWsp xValWsp;
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    nnpTask->m_xParamStack.vPeek(iTaskArg,&xValWsp);
    nnpNewTask->m_xParamStack.vPush(xValWsp);
  }
  
  // discard original task arguments
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    bool bPopped = nnpTask->m_xParamStack.bPop(xValWsp);
    ASSERT(bPopped);
  }
  
  Task* pNewTask = *nnpNewTask;
  
  Object* pTask = pNewTask;
  nnpTask->m_xParamStack.vPush(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.parent@ -- fetch parent of task on param stack                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskParentFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskParentFetch");
  
  nnpTask->m_xParamStack.vPush(nnpTask->m_pParentTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.parent! -- store parent task into this task                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskParentStore(NNP_Task nnpTask)
{
  ENTRY("vTaskParentStore");
  
  Object* pParent;
  if ( ! nnpTask->m_xParamStack.bPop(pParent) )
    THROW_INVALID_PARAM("task.parent!");
  Task* pParentTask = _pTask(pParent);
  ASSERT( ! pParentTask || CN_TASK == pParentTask->eClass());
  
  nnpTask->m_pParentTask = pParentTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.queue@ -- fetch queue of task on param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskQfetch(NNP_Task nnpTask)
{
ENTRY("vTaskQfetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.queue@");
  Task* pThatTask = _pTask(pTaskObject);
  
  Queue* pQueue = pThatTask->m_pQueue;
  
  Object* pQueueEmb = static_cast<Object*>(pQueue);
  
  nnpTask->m_xParamStack.vPush(pQueueEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.queue! -- store queue on param stack into task on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTaskQstore(NNP_Task nnpTask)
{
ENTRY("vTaskQstore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.queue!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pQueueEmb;
  
  if ( ! nnpTask->m_xParamStack.bPop(pQueueEmb) )
    THROW_INVALID_PARAM("task.queue!");
  
  Queue* pQueue = static_cast<Queue*>(pQueueEmb);
  
  pThatTask->m_pQueue = pQueue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seed -- set seed word and args of task                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeed(NNP_Task nnpTask)
{
ENTRY("vTaskSeed");
  
//  //                                   sSeed     nTaskArgs
//  if ( ! nnpTask->m_xParamStack.bGuard(CN_STRVAL,CN_INTVAL) )
//    THROW_INVALID_PARAM("task.seed");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.seed");
  Task* pThatTask = _pTask(pTaskObject);
  
  s32 sSeed;
  if ( ! nnpTask->m_xParamStack.bPop(sSeed) )
    THROW_INVALID_PARAM("task.seed");
  pThatTask->vSeedWord(sSeed.a());

  i8 nTaskArgs;
  if ( ! nnpTask->m_xParamStack.bPop(nTaskArgs) )
    THROW_INVALID_PARAM("task.seed");
  //ASSERT(! nTaskArgs);
  
  if ( ! nnpTask->m_xParamStack.bGuard(nTaskArgs) )
    THROW_INVALID_PARAM("task.seed");
  
  // copy task arguments to new task (preserving stack order)
  ValWsp xValWsp;
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    nnpTask->m_xParamStack.vPeek(iTaskArg,&xValWsp);
    pThatTask->m_xParamStack.vPush(xValWsp);
  }
  
  // discard original task arguments
  for (i8 iTaskArg = nTaskArgs - 1; iTaskArg >= 0; iTaskArg--) {
    bool bPopped = nnpTask->m_xParamStack.bPop(xValWsp);
    ASSERT(bPopped);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seitf@ -- fetch SEitf of task on param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskSeitfFetch");
  
  Object* pObject;
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("task.seitf@");
  Task* pThatTask = _pTask(pObject);
  
  SEitf* pSEitf = pThatTask->m_pSEitf;
  
  Object* pPtrSEitf= pSEitf;
  
  nnpTask->m_xParamStack.vPush(pPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seitf! -- store SEitf on param stack into task on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfStore(NNP_Task nnpTask)
{
  ENTRY("vTaskSeitfStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.seitf!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* pPtrSEitf;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrSEitf) )
    THROW_INVALID_PARAM("task.seitf!");
  
  SEitf* pSEitf = _pSEitf(pPtrSEitf);
  ASSERT(! pSEitf || CN_SEITF == pSEitf->eClass());
  
  pThatTask->m_pSEitf = pSEitf;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* signal -- signal task on stack                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskSignal(NNP_Task nnpTask)
{
  ENTRY("vTaskSignal");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.signal");
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iMore = --pThatTask->m_zAcks;
  
  if ( ! iMore ) {
//    vTrace("Task::vTaskSignal: task %p %s resuming task %p %s\n",
//           *nnpTask,nnpTask->m_sName.a(),
//           pThatTask,pThatTask->m_sName.a());
    if (gxArguments.m_bTracepoint)
      vTrace("[N%lld:%8s\\%lld %-11s] resuming %s\\%lld\n",
             gxArguments.m_iNode,
             nnpTask->m_sName.a(),
             *nnpTask->m_zAcks,
             nnpTask->m_sComment.a(),
             
             pThatTask->m_sName.a(),
             *pThatTask->m_zAcks);
    
    while (TS_SUSPENDED != pThatTask->m_eState)
      PosixThread::vSleepMicro(10);
    
    //Thread* pThread = Thread::pThread();
    //pThread->m_pTaskSet->vAppend(pThatTask);
    pThatTask->m_pTaskSet->vAppend(pThatTask);
    
  }
  else {
//    vTrace("Task::vTaskSignal: task %p %s signalled task %p\\%lld %s\n",
//           *nnpTask,nnpTask->m_sName.a(),
//           pThatTask,iMore,pThatTask->m_sName.a());
    vTrace("[N%lld:%8s\\%lld %-11s] signalled %s\\%lld\n",
           gxArguments.m_iNode,
           nnpTask->m_sName.a(),
           *nnpTask->m_zAcks,
           nnpTask->m_sComment.a(),
           
           pThatTask->m_sName.a(),
           iMore);
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.socket@ -- fetch socket of task on param stack                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskSocketFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskSocketFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.socket@");
  Task* pThatTask = _pTask(pTaskObject);
  
  Socket* pSocket = pThatTask->m_pSocket;
  
  Object* pSocketEmb = static_cast<Object*>(pSocket);
  
  nnpTask->m_xParamStack.vPush(pSocketEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.socket! -- store socket on param stack into task                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskSocketStore(NNP_Task nnpTask)
{
  ENTRY("vTaskSocketStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.socket!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrSocket;
  
  if ( ! nnpTask->m_xParamStack.bPop(ptrSocket) )
    THROW_INVALID_PARAM("task.socket!");
  
  Socket* pSocket = static_cast<Socket*>(ptrSocket);
  
  pThatTask->m_pSocket = pSocket;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateFetch(NNP_Task nnpTask)
{
ENTRY("vTaskStateFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.state@");
  Task* pThatTask = _pTask(pTaskObject);
  
  eTS eState = pThatTask->m_eState;
  
  nnpTask->m_xParamStack.vPush((i8)eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state! -- store state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateStore(NNP_Task nnpTask)
{
ENTRY("vTaskStateStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.state!");
  Task* pThatTask = _pTask(pTaskObject);
  
  i8 iState;
  
  if ( ! nnpTask->m_xParamStack.bPop(iState) )
    THROW_INVALID_PARAM("task.state!");
  
  pThatTask->m_eState = (eTS)iState;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.suspend -- this task suspends                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskSuspend(NNP_Task nnpTask)
{
  ENTRY("vTaskSuspend");
  
//  vTrace("Task::vTaskSuspend: task %p %s suspends\n",
//         *nnpTask,nnpTask->m_sName.a());
  
  if (gxArguments.m_bTracepoint)
    vTrace("[N%lld:%8s\\%lld %-11s] suspends\n",
           gxArguments.m_iNode,
           nnpTask->m_sName.a(),
           *nnpTask->m_zAcks,
           nnpTask->m_sComment.a()
           );
  
  nnpTask->vState(TS_SUSPENDED);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.taskset@ -- fetch taskset of task on param stack                      */
/*----------------------------------------------------------------------------*/
void Task::vTaskTaskSetFetch(NNP_Task nnpTask)
{
  ENTRY("vTaskTaskSetFetch");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.taskset@");
  Task* pThatTask = _pTask(pTaskObject);
  
  TaskSet* pTaskSet = pThatTask->m_pTaskSet;
  
  Object* pTaskSetEmb = static_cast<Object*>(pTaskSet);
  
  nnpTask->m_xParamStack.vPush(pTaskSetEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.taskset! -- store taskset on param stack into task                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskTaskSetStore(NNP_Task nnpTask)
{
  ENTRY("vTaskTaskSetStore");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.taskset!");
  Task* pThatTask = _pTask(pTaskObject);
  
  Object* ptrTaskSet;
  
  if ( ! nnpTask->m_xParamStack.bPop(ptrTaskSet) )
    THROW_INVALID_PARAM("task.taskset!");
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(ptrTaskSet);
  
  pThatTask->m_pTaskSet = pTaskSet;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.this -- push this task                                                */
/*----------------------------------------------------------------------------*/
void Task::vTaskThis(NNP_Task nnpTask)
{
ENTRY("vTaskThis");
  
  Object* pTask = *nnpTask;
  
  nnpTask->m_xParamStack.vPush(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.thread -- push given tasks thread                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskThread(NNP_Task nnpTask)
{
  ENTRY("vTaskThread");
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.thread");
  Task* pThatTask = _pTask(pTaskObject);

  Thread* pThread = pThatTask->m_pThread;
  nnpTask->m_xParamStack.vPush(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.wait -- this task waits for signal                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskWait(NNP_Task nnpTask)
{
  ENTRY("vTaskWait");
  
  // ..tbd..
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.yield -- this task yields                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskYield(NNP_Task nnpTask)
{
  ENTRY("vTaskYield");
  
  nnpTask->vState(TS_YIELDED);
  
  EXIT();
}

// taskCursor: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* taskcursor.begin -- begin using task currsor                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorBegin(NNP_Task nnpTask)
{
ENTRY("vTaskCursorBegin");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_PARAM("taskCursor.begin");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.delete -- delete an task cursor                                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorDelete(NNP_Task nnpTask)
{
ENTRY("vTaskCursorDelete");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_PARAM("taskCursor.delete");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pTaskCursor);
  delete pTaskCursor;
  
  Heap::vFree(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.end -- end using task cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorEnd(NNP_Task nnpTask)
{
ENTRY("vTaskCursorEnd");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_PARAM("taskCursor.end");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.first -- set to first and get task                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorFirst(NNP_Task nnpTask)
{
ENTRY("vTaskCursorFirst");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_PARAM("taskCursor.first");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vFirst();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.get -- get current task of cursor                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorGet(NNP_Task nnpTask)
{
ENTRY("vTaskCursorGet");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_PARAM("taskCursor.get");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  Task* pThatTask = pTaskCursor->pObject();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.init -- init an task cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorInit(NNP_Task nnpTask)
{
ENTRY("vTaskCursorInit");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_PARAM("taskCursor.init");
  
//  s32 sName;
//  sName = "tasklist";
//  Word* pWord = nnpTask->pFindWord(sName);
//  ASSERT(pWord);
//  TaskList* pTaskList = reinterpret_cast<TaskList*>(pWord->pBodyArea());
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  new(pTaskCursor) TaskCursor(gpTaskList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.new -- new a task cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorNew(NNP_Task nnpTask)
{
ENTRY("vTaskCursorNew");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.next -- advance to next and get task                            */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorNext(NNP_Task nnpTask)
{
ENTRY("vTaskCursorNext");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTaskCursor) )
    THROW_INVALID_PARAM("taskCursor.next");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vNext();
  
  Object* pPtrTask = pTaskCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pPtrTask);
  
  EXIT();
}

// tasklist: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* tasklist -- tasklist                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskList(NNP_Task nnpTask)
{
ENTRY("vTaskList");
  
  nnpTask->m_xParamStack.vPush(gpTaskList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.append -- tasklist append                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskListAppend(NNP_Task nnpTask)
{
ENTRY("vTaskListAppend");
  
  Object* pPtrTaskList;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTaskList) )
    THROW_INVALID_PARAM("tasklist.append");
  
  TaskList* pTaskList = _pTaskList(pPtrTaskList);
  ASSERT(CN_TASKLIST == pTaskList->eClass());
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.dump");
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskList->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.dump -- tasklist dump                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListDump(NNP_Task nnpTask)
{
ENTRY("vTaskListDump");
  
  Object* pPtrTaskList;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTaskList) )
    THROW_INVALID_PARAM("tasklist.dump");
  
  TaskList* pTaskList = _pTaskList(pPtrTaskList);
  ASSERT(CN_TASKLIST == pTaskList->eClass());
 
  pTaskList->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.init -- tasklist init                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListInit(NNP_Task nnpTask)
{
ENTRY("vTaskListInit");
  
  EXIT();
}

// taskset: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* taskset.append -- taskset append                                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetAppend(NNP_Task nnpTask)
{
ENTRY("vTaskSetAppend");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("taskset.append");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("taskset.append");
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskSet->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskset.dump -- taskset dump                                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetDump(NNP_Task nnpTask)
{
ENTRY("vTaskSetDump");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("taskset.dump");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  pTaskSet->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskset.init -- taskset init                                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetInit(NNP_Task nnpTask)
{
ENTRY("vTaskSetInit");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("taskset.init");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  new(pTaskSet) TaskSet;
  
  EXIT();
}

// thread: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* thread.append -- append task to threads taskset                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadAppend(NNP_Task nnpTask)
{
ENTRY("vThreadAppend");
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xParamStack.bPop(pThreadEmb) )
    THROW_INVALID_PARAM("thread.append");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;
  
  Object* pTaskObject;
  if ( ! nnpTask->m_xParamStack.bPop(pTaskObject) )
    THROW_INVALID_PARAM("task.dump");
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskSet->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.bind -- bind this thread to a cpu                                   */
/*----------------------------------------------------------------------------*/
void Task::vThreadBind(NNP_Task nnpTask)
{
ENTRY("vThreadBind");
  
  i8 iCpu;
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(iCpu) )
    THROW_INVALID_PARAM("thread.bind");
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("thread.bind");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vBind(iCpu);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.condSignal -- signal condition on threads mutex                     */
/*----------------------------------------------------------------------------*/
void Task::vThreadCondSignal(NNP_Task nnpTask)
{
 ENTRY("vThreadCondSignal");
  
  Object* pThreadObject;
  if ( ! nnpTask->m_xParamStack.bPop(pThreadObject) )
    THROW_INVALID_PARAM("thread.condSignal");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vCondSignal();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.condWait -- wait for cond on threads mutex                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadCondWait(NNP_Task nnpTask)
{
 ENTRY("vThreadCondWait");
  
  Object* pThreadObject;
  if ( ! nnpTask->m_xParamStack.bPop(pThreadObject) )
    THROW_INVALID_PARAM("thread.condWait");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vCondWait();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.cpu -- get the cpu this thread is running on                        */
/*----------------------------------------------------------------------------*/
void Task::vThreadCpu(NNP_Task nnpTask)
{
ENTRY("vThreadCpu");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("thread.cpu");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  nnpTask->m_xParamStack.vPush(pThread->iCpu());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dump -- thread dump                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadDump(NNP_Task nnpTask)
{
ENTRY("vThreadDump");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("thread.dump");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.id -- get the id of thread                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadId(NNP_Task nnpTask)
{
ENTRY("vThreadId");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("thread.id");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  nnpTask->m_xParamStack.vPush(pThread->m_xPosixThread.m_iOpSysThreadId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.init -- thread init                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadInit(NNP_Task nnpTask)
{
ENTRY("vThreadInit");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("thread.init");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  new(pThread) Thread;
  
  Word* pWordThread = Word::pToWord(pThread);
  
  s32 sName;
  sName = pWordThread->sName();
  
  pThread->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.mutexLock -- acquire a threads mutex lock                           */
/*----------------------------------------------------------------------------*/
void Task::vThreadMutexLock(NNP_Task nnpTask)
{
ENTRY("vThreadMutexLock");
  
  Object* pThreadObject;
  if ( ! nnpTask->m_xParamStack.bPop(pThreadObject) )
    THROW_INVALID_PARAM("thread.mutexLock");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vMutexLock();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.mutexUnlock -- release a threads mutex lock                         */
/*----------------------------------------------------------------------------*/
void Task::vThreadMutexUnlock(NNP_Task nnpTask)
{
ENTRY("vThreadMutexUnlock");

  Object* pThreadObject;
  if ( ! nnpTask->m_xParamStack.bPop(pThreadObject) )
    THROW_INVALID_PARAM("thread.mutexUnlock");
  
  Thread* pThread = _pThread(pThreadObject);
  ASSERT(CN_THREAD == pThread->eClass());
  
  pThread->m_xPosixThread.vMutexUnlock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name@ -- fetch thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameFetch(NNP_Task nnpTask)
{
ENTRY("vThreadNameFetch");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("thread.name@");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  sName = pThread->sName();
  
  nnpTask->m_xParamStack.vPush(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name! -- store thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameStore(NNP_Task nnpTask)
{
ENTRY("vThreadNameStore");
  
  Object* pObject;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("thread.name!");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  ValWsp xNameWsp;
  if ( ! nnpTask->m_xParamStack.bPop(xNameWsp) )
    THROW_INVALID_PARAM("thread.name!");
  
  sName = xNameWsp.sStr();
  
  pThread->vName(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskset@ -- fetch taskset from the thread which is on param stack   */
/*----------------------------------------------------------------------------*/
void Task::vThreadSfetch(NNP_Task nnpTask)
{
ENTRY("vThreadSfetch");
 
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xParamStack.bPop(pThreadEmb) )
    THROW_INVALID_PARAM("thread.taskset@");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;

  Object* pTaskSetEmb = static_cast<Object*>(pTaskSet);;

  nnpTask->m_xParamStack.vPush(pTaskSetEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskset! -- store taskset into the thread which is on param stack   */
/*----------------------------------------------------------------------------*/
void Task::vThreadSstore(NNP_Task nnpTask)
{
ENTRY("vThreadSstore");
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xParamStack.bPop(pThreadEmb) )
    THROW_INVALID_PARAM("thread.taskset!");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  Object* pTaskSetEmb;
  
  if ( ! nnpTask->m_xParamStack.bPop(pTaskSetEmb) )
    THROW_INVALID_PARAM("thread.taskset!");
  
  ASSERT(CN_TASKSET == pTaskSetEmb->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pTaskSetEmb);
  
  pThread->m_pTaskSet = pTaskSet;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_s -- sleep for n seconds                                      */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_s(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_s");
  
  i8 nSeconds;
  
  if ( ! nnpTask->m_xParamStack.bPop(nSeconds) )
    THROW_INVALID_PARAM("sleep_s");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepSec(nSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ms -- sleep for n milliseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ms(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_ms");
  
  i8 nMilliseconds;
  
  if ( ! nnpTask->m_xParamStack.bPop(nMilliseconds) )
    THROW_INVALID_PARAM("sleep_ms");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepMilli(nMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_us -- sleep for n microseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_us(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_us");
  
  i8 nMicroseconds;
  
  if ( ! nnpTask->m_xParamStack.bPop(nMicroseconds) )
    THROW_INVALID_PARAM("sleep_us");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepMicro(nMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ns -- sleep for n nanoseconds                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ns(NNP_Task nnpTask)
{
ENTRY("vThreadSleep_ns");
  
  i8 nNanoseconds;
  
  if ( ! nnpTask->m_xParamStack.bPop(nNanoseconds) )
    THROW_INVALID_PARAM("sleep_ns");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepNano(nNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.spawn -- spwan os thread associated with this thread                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn(NNP_Task nnpTask)
{
ENTRY("vThreadSpawn");
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->m_xParamStack.bPop(pThreadEmb) )
    THROW_INVALID_PARAM("thread.spawn");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn();
  
  EXIT();
}

// threadCursor: ///////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* threadCursor.begin -- begin using thread cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorBegin(NNP_Task nnpTask)
{
ENTRY("vThreadCursorBegin");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_PARAM("threadCursor.begin");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.delete -- delete a thread cursor                              */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorDelete(NNP_Task nnpTask)
{
ENTRY("vTheadCursorDelete");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_PARAM("threadCursor.delete");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  delete pThreadCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.end -- end using thread cursor                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorEnd(NNP_Task nnpTask)
{
ENTRY("vThreadCursorEnd");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_PARAM("threadcursor.end");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.first -- set to first and push thread                         */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorFirst(NNP_Task nnpTask)
{
ENTRY("vThreadCursorFirst");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_PARAM("threadCursor.first");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vFirst();
  
  Object* pThread = pThreadCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.get -- push current thread                                    */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorGet(NNP_Task nnpTask)
{
ENTRY("vThreadCursorGet");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_PARAM("threadCursor.get");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  Object* pObject = pThreadCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.init -- init a thread cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorInit(NNP_Task nnpTask)
{
ENTRY("vThreadCursorInit");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_PARAM("threadCursor.init");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  new(pThreadCursor) ThreadCursor(gpThreadList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.new -- new a thread cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorNew(NNP_Task nnpTask)
{
ENTRY("vThreadCursorNew");
  
  ThreadCursor* pThreadCursor = new ThreadCursor(gpThreadList);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  nnpTask->m_xParamStack.vPush(pThreadCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.next -- advance to next and push thread                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorNext(NNP_Task nnpTask)
{
ENTRY("vThreadCursorNext");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrThreadCursor) )
    THROW_INVALID_PARAM("threadCursor.next");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vNext();
  
  Object* pThread = pThreadCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pThread);
  
  EXIT();
}

// threadlist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* threadlist.dump -- dump the threadList on param stack                      */
/*----------------------------------------------------------------------------*/
void Task::vThreadListDump(NNP_Task nnpTask)
{
ENTRY("vThreadListDump");
  
  Object* pPtrThreadList;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrThreadList) )
    THROW_INVALID_PARAM("threadlist.dump");
  
  ThreadList* pThreadList = _pThreadList(pPtrThreadList);
  ASSERT(CN_THREADLIST == pThreadList->eClass());

  pThreadList->vDump("ThreadList:", pSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadlist.init -- init of threadList on param stack                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadListInit(NNP_Task nnpTask)
{
ENTRY("vThreadListInit");
  
  Object* pPtrThreadList;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrThreadList) )
    THROW_INVALID_PARAM("threadlist.init");
  
  ThreadList* pThreadList = _pThreadList(pPtrThreadList);
  ASSERT(CN_THREADLIST == pThreadList->eClass());
  
  new(pThreadList) ThreadList;
  
  EXIT();
}

// timer: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* timer.delete -- delete timer                                               */
/*----------------------------------------------------------------------------*/
void Task::vTimerDelete(NNP_Task nnpTask)
{
ENTRY("vTimeDelete");
  
  Header* pPtrTimer;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTimer) )
    THROW_INVALID_PARAM("timer.delete");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pTimerObject = reinterpret_cast<Object*>(pTimer);
  
  delete pTimer;
  
  Heap::vFree(pTimerObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.init -- init timer                                                   */
/*----------------------------------------------------------------------------*/
void Task::vTimerInit(NNP_Task nnpTask)
{
ENTRY("vTimerInit");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTimer) )
    THROW_INVALID_PARAM("timer.init");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  new(pTimer) Timer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.microsecs -- get elapsed duration in microseconds                    */
/*----------------------------------------------------------------------------*/
void Task::vTimerMicrosecs(NNP_Task nnpTask)
{
ENTRY("vTimerMicrosecs");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTimer) )
    THROW_INVALID_PARAM("timer.microsecs");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMicroseconds = pTimer->iMicroseconds();
  
  nnpTask->m_xParamStack.vPush(iMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.millisecs -- get elapsed duration in milliseconds                    */
/*----------------------------------------------------------------------------*/
void Task::vTimerMillisecs(NNP_Task nnpTask)
{
ENTRY("vTimerMillisecs");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTimer) )
    THROW_INVALID_PARAM("timer.millisecs");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMilliseconds = pTimer->iMilliseconds();
  
  nnpTask->m_xParamStack.vPush(iMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.nanosecs -- get elapsed duration in nanoseconds                      */
/*----------------------------------------------------------------------------*/
void Task::vTimerNanosecs(NNP_Task nnpTask)
{
ENTRY("vTimerNanosecs");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTimer) )
    THROW_INVALID_PARAM("timer.nanosecs");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iNanoseconds = pTimer->iNanoseconds();
  
  nnpTask->m_xParamStack.vPush(iNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.new -- new timer                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTimerNew(NNP_Task nnpTask)
{
ENTRY("vTimerNew");
  
  void* pObject = Heap::pAllocate(CN_ROWSTRAND);

  Timer* pTimer = _pTimer(Heap::pAllocate(CN_TIMER));

  new(pTimer) Timer;
 
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Header* pPtrTimer = pTimer;
  
  nnpTask->m_xParamStack.vPush(pPtrTimer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.reset -- reset timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerReset(NNP_Task nnpTask)
{
ENTRY("vTimerReset");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTimer) )
    THROW_INVALID_PARAM("timer.reset");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vReset();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.secs -- get elapsed duration in seconds                              */
/*----------------------------------------------------------------------------*/
void Task::vTimerSecs(NNP_Task nnpTask)
{
ENTRY("vTimerSecs");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTimer) )
    THROW_INVALID_PARAM("timer.secs");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iSeconds = pTimer->iSeconds();
  
  nnpTask->m_xParamStack.vPush(iSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.start -- start timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerStart(NNP_Task nnpTask)
{
ENTRY("vTimerStart");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTimer) )
    THROW_INVALID_PARAM("timer.Start");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vStart();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.ticks -- get elapsed ticks                                           */
/*----------------------------------------------------------------------------*/
void Task::vTimerTicks(NNP_Task nnpTask)
{
ENTRY("vTimerTicks");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTimer) )
    THROW_INVALID_PARAM("timer.ticks");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iTicks = pTimer->iTicks();
  
  nnpTask->m_xParamStack.vPush(iTicks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.update -- update timer                                               */
/*----------------------------------------------------------------------------*/
void Task::vTimerUpdate(NNP_Task nnpTask)
{
ENTRY("vTimerUpdate");
  
  Header* pPtrTimer;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTimer) )
    THROW_INVALID_PARAM("timer.update");
  
  Timer* pTimer = _pTimer(pPtrTimer);
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vUpdate();
  
  EXIT();
}

// tx: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* tx.dump -- delete tx                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTxDump(NNP_Task nnpTask)
{
ENTRY("vTxDump");
  
  Object* pPtrTx;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTx) )
    THROW_INVALID_PARAM("tx.delete");
  
  Tx* pTx = _pTx(pPtrTx);
  ASSERT(CN_TX == pTx->eClass());

  pTx->vDump();
  
  EXIT();
}

// txCollection: ///////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* txCollection.dump -- dump of txCollection on param stack                  */
/*----------------------------------------------------------------------------*/
void Task::vTxCollectionDump(NNP_Task nnpTask)
{
ENTRY("vTxCollectionDump");
  
  Object* pPtrTxCollection;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxCollection) )
    THROW_INVALID_PARAM("txCollection.dump");
  
  TxCollection* pTxCollection = _pTxCollection(pPtrTxCollection);
  ASSERT(CN_TXCOLLECTION == pTxCollection->eClass());
  
  pTxCollection->vDump("gpTxCollection:");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCollection.init -- init of txCollection on param stack                   */
/*----------------------------------------------------------------------------*/
void Task::vTxCollectionInit(NNP_Task nnpTask)
{
ENTRY("vTxCollectionInit");
  
  Object* pPtrTxCollection;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxCollection) )
    THROW_INVALID_PARAM("txCollection.init");
  
  TxCollection* pTxCollection = _pTxCollection(pPtrTxCollection);
  ASSERT(CN_TXCOLLECTION == pTxCollection->eClass());
  
  new(pTxCollection) TxCollection;
  
  EXIT();
}

// txCursor: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* txCursor.begin -- begin using tx cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorBegin(NNP_Task nnpTask)
{
ENTRY("vTxCursorBegin");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxCursor) )
    THROW_INVALID_PARAM("txCursor.begin");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.delete -- delete a tx cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorDelete(NNP_Task nnpTask)
{
ENTRY("vTxCursorDelete");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxCursor) )
    THROW_INVALID_PARAM("txCursor.delete");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  delete pTxCursor;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.end -- end using tx cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorEnd(NNP_Task nnpTask)
{
ENTRY("vTxCursorEnd");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxCursor) )
    THROW_INVALID_PARAM("txcursor.end");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.first -- set to first and push tx                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorFirst(NNP_Task nnpTask)
{
ENTRY("vTxCursorFirst");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxCursor) )
    THROW_INVALID_PARAM("txCursor.first");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vFirst();
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.get -- push current tx                                            */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorGet(NNP_Task nnpTask)
{
ENTRY("vTxCursorGet");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxCursor) )
    THROW_INVALID_PARAM("txCursor.get");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.init -- init a tx cursor                                          */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorInit(NNP_Task nnpTask)
{
ENTRY("vTxCursorInit");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxCursor) )
    THROW_INVALID_PARAM("txCursor.init");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  new(pTxCursor) TxCursor(gpTxCollection);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.new -- new a tx cursor                                            */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorNew(NNP_Task nnpTask)
{
ENTRY("vTxCursorNew");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.next -- advance to next and push tx                               */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorNext(NNP_Task nnpTask)
{
ENTRY("vTxCursorNext");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxCursor) )
    THROW_INVALID_PARAM("txCursor.next");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_THREADCURSOR == pTxCursor->eClass());
  
  pTxCursor->vNext();
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->m_xParamStack.vPush(pObject);
  
  EXIT();
}

// txMap: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* txMap.init -- init of txmap on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxMapInit(NNP_Task nnpTask)
{
ENTRY("vTxMapInit");
  
  Object* pPtrTxMap;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxMap) )
    THROW_INVALID_PARAM("txMap.init");
  
  TxMap* pTxMap = _pTxMap(pPtrTxMap);
  ASSERT(CN_TXMAP == pTxMap->eClass());
  
  new(pTxMap) TxMap;
  
  EXIT();
}

// txRef: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* txRef.init -- init of txref on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxRefInit(NNP_Task nnpTask)
{
ENTRY("vTxRefInit");
  
  Object* pPtrTxRef;
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxRef) )
    THROW_INVALID_PARAM("txRef.init");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  new(pTxRef) TxRef;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.begin -- begin a tx (via a txRef on stack)                           */
/*----------------------------------------------------------------------------*/
void Task::vTxRefBegin(NNP_Task nnpTask)
{
ENTRY("vTxRefBegin");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxRef) )
    THROW_INVALID_PARAM("tx.begin");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bBegun = pTxRef->bBeginTx();
  
  nnpTask->m_xParamStack.vPush(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.end -- end a tx (via a txRef on stack)                               */
/*----------------------------------------------------------------------------*/
void Task::vTxRefEnd(NNP_Task nnpTask)
{
ENTRY("vTxRefEnd");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxRef) )
    THROW_INVALID_PARAM("txRef.end");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bEnded = pTxRef->bEndTx();
  
  nnpTask->m_xParamStack.vPush(bEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.commit -- commit a tx (via a txRef on stack)                         */
/*----------------------------------------------------------------------------*/
void Task::vTxRefCommit(NNP_Task nnpTask)
{
ENTRY("vTxRefCommit");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxRef) )
    THROW_INVALID_PARAM("txRef.commit");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bCommitted = pTxRef->bCommitTx();
  
  nnpTask->m_xParamStack.vPush(bCommitted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.rollback -- rollback a tx (via a txRef on stack)                     */
/*----------------------------------------------------------------------------*/
void Task::vTxRefRollback(NNP_Task nnpTask)
{
ENTRY("vTxRefRollback");
  
  Object* pPtrTxRef;
  
  if ( ! nnpTask->m_xParamStack.bPop(pPtrTxRef) )
    THROW_INVALID_PARAM("txRef.rollback");
  
  TxRef* pTxRef = _pTxRef(pPtrTxRef);
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bRolledBack = pTxRef->bRollbackTx();
  
  nnpTask->m_xParamStack.vPush(bRolledBack);
  
  EXIT();
}

// words: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* create -- create word in dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vCreate(NNP_Task nnpTask)
{
ENTRY("vCreate");
  
  nnpTask->vTokenNext();
  
  Dic* pDic;
  if (nnpTask->m_pSEitf)
    pDic = nnpTask->m_pSEitf->pDicStack()->pTop();
  else
    pDic = nnpTask->m_xDicStack.pTop();
  
  Word* pNewWord = new(pDic) Word(nnpTask->m_sToken.a());
  
  nnpTask->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >create -- create word in dictionary using string on stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCreateFrom(NNP_Task nnpTask)
{
ENTRY("vCreateFrom");

  s32 sName;
  if ( ! nnpTask->m_xParamStack.bPop(sName) )
    THROW_INVALID_PARAM(">create");
  
  Dic* pDic;
  if (nnpTask->m_pSEitf)
    pDic = nnpTask->m_pSEitf->pDicStack()->pTop();
  else
    pDic = nnpTask->m_xDicStack.pTop();
  
  Word* pNewWord = new(pDic) Word(sName.a());
  
  nnpTask->vCurrentWord(pNewWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* does> -- routine does                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDoes(NNP_Task nnpTask)
{
ENTRY("vDoes");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("encodeDoes");
  pCurrentWord->vEncode("exit");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* find -- find word                                                          */
/*----------------------------------------------------------------------------*/
void Task::vFind(NNP_Task nnpTask)
{
ENTRY("vFind");
  
  s256 sName;
  if ( ! nnpTask->m_xParamStack.bPop(sName) )
    THROW_INVALID_PARAM("find");
 
  if (nnpTask->m_iTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = nnpTask->pFindWord(sName);
  
  nnpTask->m_xParamStack.vPush(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* findInCurrent -- find word in current dictionary                           */
/*----------------------------------------------------------------------------*/
void Task::vFindInCurrent(NNP_Task nnpTask)
{
  ENTRY("vFind");
  
  s256 sName;
  if ( ! nnpTask->m_xParamStack.bPop(sName) )
    THROW_INVALID_PARAM("findInCurrent");
  
  if (nnpTask->m_iTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = nnpTask->pFindWordInCurrent(sName);
  
  nnpTask->m_xParamStack.vPush(pWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* forget -- forget word                                                      */
/*----------------------------------------------------------------------------*/
void Task::vForget(NNP_Task nnpTask)
{
ENTRY("vForget");
  
  nnpTask->vTokenNext();
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_sToken);
  
  if ( ! pWord )
    THROW("No such word: %s",nnpTask->m_sToken.a());
  
  bool bResult = nnpTask->bForgetWord(nnpTask->m_sToken);
  ASSERT(bResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* token -- get next token as string on stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vToken(NNP_Task nnpTask)
{
  ENTRY("vToken");
  
  nnpTask->vTokenNext();
  
  s32 sToken(nnpTask->m_sToken.a());
  
  nnpTask->m_xParamStack.vPush(sToken);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >forget -- forget word on stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vWordForget(NNP_Task nnpTask)
{
ENTRY("vWordForget");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("vWordForget");
  Word* pWord = _pWord(pWordObject);
  
  bool bResult = nnpTask->bForgetWord(pWord->sName());
  ASSERT(bResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vWordDump(NNP_Task nnpTask)
{
ENTRY("vWordDump");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("vWordDump");
  Word* pWord = _pWord(pWordObject);
  
  pWord->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dic -- push word's dictionary                                         */
/*----------------------------------------------------------------------------*/
void Task::vWordDic(NNP_Task nnpTask)
{
ENTRY("vWordDic");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("vWordDic");
  Word* pWord = _pWord(pWordObject);
  
  Dic* pDic = pWord->pDic();
  
  nnpTask->m_xParamStack.vPush(pDic->sName().a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindGet(NNP_Task nnpTask)
{
ENTRY("vWordKindGet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("vWordKindGet");
  Word* pWord = _pWord(pWordObject);
  
  Word* pWordKind = pWord->pKind();
  
  nnpTask->m_xParamStack.vPush(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindSet(NNP_Task nnpTask)
{
ENTRY("vWordKindSet");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("vWordKindSet");
  Word* pWord = _pWord(pWordObject);
  
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("vWordKindSet");
  Word* pWordKind = _pWord(pWordObject);
  
  pWord->vKind(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name -- push name of word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordName(NNP_Task nnpTask)
{
ENTRY("vWordName");
  
  Object* pWordObject;
  if ( ! nnpTask->m_xParamStack.bPop(pWordObject) )
    THROW_INVALID_PARAM("word.name");
  Word* pWord = _pWord(pWordObject);
  
  s32 sWordName(pWord->sName().a());
  
  nnpTask->m_xParamStack.vPush(sWordName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.this -- push current word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordThis(NNP_Task nnpTask)
{
ENTRY("vWordThis");
  
  Word* pWord = nnpTask->pCurrentWord();
  
  nnpTask->m_xParamStack.vPush(pWord);
  
  EXIT();
}

// zero: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* 0< -- test if top value is less than zero                                  */
/*----------------------------------------------------------------------------*/
void Task::vZeroLess(NNP_Task nnpTask)
{
  ENTRY("vZeroLess");
  
  i8 iZero;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0<");
  
  if (iZero < 0)
    nnpTask->m_xParamStack.vPush((i8)1);
  else
    nnpTask->m_xParamStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0<= -- test if top value is less than or equal to zero                     */
/*----------------------------------------------------------------------------*/
void Task::vZeroLessEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroLessEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0<=");
  
  if (iZero <= 0)
    nnpTask->m_xParamStack.vPush((i8)1);
  else
    nnpTask->m_xParamStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0== -- test if top value is equal to zero / nullptr                        */
/*----------------------------------------------------------------------------*/
void Task::vZeroEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0==");
  
  if (iZero == 0)
    nnpTask->m_xParamStack.vPush((i8)1);
  else
    nnpTask->m_xParamStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0!= -- test if top value is not equal to zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroNotEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroNotEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0!=");
  
  if (iZero != 0)
    nnpTask->m_xParamStack.vPush((i8)1);
  else
    nnpTask->m_xParamStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* zeroGreaterEqual -- test if top value is greater than or equal to zero     */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreaterEqual(NNP_Task nnpTask)
{
  ENTRY("vZeroGreaterEqual");
  
  i8 iZero;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0>=");
  
  if (iZero >= 0)
    nnpTask->m_xParamStack.vPush((i8)1);
  else
    nnpTask->m_xParamStack.vPush((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0> -- test if top value is greater than zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreater(NNP_Task nnpTask)
{
  ENTRY("vZeroGreater");
  
  i8 iZero;
  if ( ! nnpTask->m_xParamStack.bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0>");
  
  if (iZero > 0)
    nnpTask->m_xParamStack.vPush((i8)1);
  else
    nnpTask->m_xParamStack.vPush((i8)0);
  
  EXIT();
}

// sockets: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* socket.accept: accept a socket connection                                  */
/*----------------------------------------------------------------------------*/
void Task::vSocketAccept(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  int     iSocketId;
  Object* pObject;
  
ENTRY("vSocketAccept");
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("socket.accept");
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // accept a connection
  bool bResult = pSocket->bAcceptClient(iSocketId, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to accept connection: Errno %d!", xSysCall.iError());
  
  Task* pTask = (Task*)0x208f500c8;
  
  // return the new socket
  nnpTask->m_xParamStack.vPush(new Socket(iSocketId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* open a socket                                                              */
/*----------------------------------------------------------------------------*/
void Task::vSocketClose(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  
ENTRY("vSocketClose");
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("socketclose");
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  // close the socket
  bool bResult = pSocket->bClose(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to close socket: Errno %d!", xSysCall.iError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.connect: connect to a server                                        */
/*----------------------------------------------------------------------------*/
void Task::vSocketConnect(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  ValWsp xNode;
  
  i8 iPort;
  
ENTRY("vSocketConnect");
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("socket.connect");
  
  if ( ! nnpTask->m_xParamStack.bPop(xNode) )
    THROW_INVALID_PARAM("socket.connect");

  if ( ! nnpTask->m_xParamStack.bPop(iPort) )
    THROW_INVALID_PARAM("socket.connect");

  // get address of node
  const char* sAddr = reinterpret_cast<char*>(xNode.sStr());
  
  // get pSocket
  pSocket = reinterpret_cast<Socket*>(pObject);
  
  bool bResult = false;
  
  // try connecting in a loop if timeout occurs
  // possibly other node not back to accepting connections
  
  i8 iTimedOutCount = 0;
  i8 iConnRefusedCount = 0;
  
  while (!bResult) {
    bResult = pSocket->bConnectServer(sAddr, (int)iPort, xSysCall);
    if ( ! bResult ) {
      i8 iError = xSysCall.iError();
      
      if (iError == ETIMEDOUT) {
        if (0 == iTimedOutCount++)
          vTrace("Connect timedout... retrying...\n");
        continue;
      }
      
      if (iError == ECONNREFUSED) {
        if (0 == iConnRefusedCount++)
          vTrace("Connect refused... retrying...\n");
        continue;
      }
      
      THROW("Unable to connect: Errno %d!", iError);
    }
  }
  
  // return the socket object
  Object* pPtrSocket = pSocket;
  nnpTask->m_xParamStack.vPush(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.listen: listen in for socket connections                            */
/*----------------------------------------------------------------------------*/
void Task::vSocketListen(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  i8      iPort;
  
ENTRY("vSocketListen");
  
  if ( ! nnpTask->m_xParamStack.bPop(iPort) )
    THROW_INVALID_PARAM("socketlisten");
  
  pSocket = new Socket;
  bool bResult = pSocket->bCreateServer((int)iPort, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to listen for connections: Errno %d!", xSysCall.iError());
  
  // return new socket
  Object* pPtrSocket = pSocket;
  nnpTask->m_xParamStack.vPush(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.open: open a socket                                                 */
/*----------------------------------------------------------------------------*/
void Task::vSocketOpen(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  
ENTRY("vSocketOpen");
  
  pSocket = new Socket;
  bool bResult = pSocket->bSocket(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to create socker: Errno %d!", xSysCall.iError());
  
  // return pSocket
  Object* pPtrSocket = pSocket;
  nnpTask->m_xParamStack.vPush(pPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.send: send to a socket ( valset socket -- 1 )                       */
/*----------------------------------------------------------------------------*/
void Task::vSocketSend(NNP_Task nnpTask)
{
ENTRY("vSocketSend");

  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  ValSet  xValSet;
  bool    bResult;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("socket.send");
  
  if ( ! nnpTask->m_xParamStack.bPop(xValSet) )
    THROW_INVALID_PARAM("socket.send");
  
  pSocket = reinterpret_cast<Socket*>(pObject);

  //******************************************************************
  // Send the size of valset
  //******************************************************************
  i8 iBytes = xValSet.iSize();
  bResult = pSocket->bSendTo(sizeof(iBytes), &iBytes, xSysCall);
  
  if ( ! bResult ) {
    //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
    nnpTask->m_xParamStack.vPush((i8)0);
    
    //goto exit;
    EXIT();
  }
  
  //******************************************************************
  // now send the actual value set
  //******************************************************************
  bResult = pSocket->bSendTo(iBytes, &xValSet, xSysCall);
  
  if ( ! bResult ) {
    //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
    nnpTask->m_xParamStack.vPush((i8)0);
  }
  
  //******************************************************************
  // get rowBuffer if any in valset
  //******************************************************************
  RowBuffer* pRowBuffer = xValSet.pGetRowBuffer();
  
  if (pRowBuffer) {
    
    //******************************************************************
    // send rowBuffer size
    //******************************************************************
    i8 iBytes = pRowBuffer->iBytesUsed();
    bResult = pSocket->bSendTo(sizeof(iBytes), &iBytes, xSysCall);
    
    if ( ! bResult ) {
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      nnpTask->m_xParamStack.vPush((i8)0);
      
      //goto exit;
      EXIT();
    }
    
    //******************************************************************
    // send rowBuffer itself
    //******************************************************************
    bResult = pSocket->bSendTo(iBytes, pRowBuffer->pDataArea(), xSysCall);
    
    if ( ! bResult ) {
      //THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
      nnpTask->m_xParamStack.vPush((i8)0);
      
      //goto exit;
      EXIT();
    }
    
  }

  nnpTask->m_xParamStack.vPush((i8)1);
  
exit:
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* socket.recv: recv from a socket                                            */
/*----------------------------------------------------------------------------*/
void Task::vSocketRecv(NNP_Task nnpTask)
{
  ENTRY("vSocketRecv");
  
  SysCall xSysCall;
  Socket* pSocket;
  ValSet xValSet;
  //s8K   sWorkspace;
  Object* pObject;
  i8 iBytes;
  
  if ( ! nnpTask->m_xParamStack.bPop(pObject) )
    THROW_INVALID_PARAM("socketrecv");
  
  pSocket = reinterpret_cast<Socket*>(pObject);

  if (gxArguments.m_bTracepoint)
    vTrace("[N%lld:%8s\\%lld %-11s] receiving on %lld\n",
           gxArguments.m_iNode,
           nnpTask->m_sName.a(),
           *nnpTask->m_zAcks,
           nnpTask->m_sComment.a(),
           
           pSocket->m_iSocketId);
  
  //******************************************************************
  // Receive the size of valset
  //******************************************************************
  bool bResult = pSocket->bRecvBytes(sizeof(iBytes), reinterpret_cast<void*>(&iBytes), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
  
  //******************************************************************
  // Receive the valset
  //******************************************************************
  bResult = pSocket->bRecvBytes(iBytes, reinterpret_cast<void*>(&xValSet), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
  
  //******************************************************************
  // Check if valset has a row buffer
  //******************************************************************
  bool bHasRowBuffer = xValSet.bHasRowBuffer();
  
  if (bHasRowBuffer) {
    
    //******************************************************************
    // receive rowBuffer size
    //******************************************************************
    bResult = pSocket->bRecvBytes(sizeof(iBytes), reinterpret_cast<void*>(&iBytes), xSysCall);
    
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
    
    //******************************************************************
    // receive rowBuffer
    //******************************************************************
    RowBuffer* pRowBuffer = new RowBuffer(iBytes);
    bResult = pSocket->bRecvBytes(iBytes, reinterpret_cast<void*>(pRowBuffer->pDataArea()), xSysCall);
  
    if ( ! bResult )
      THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
  

    //******************************************************************
    // set rowBuffer in valset
    //******************************************************************
    bool bSet = xValSet.bSetRowBuffer(pRowBuffer);
    ASSERT(bSet);
  }
  
  nnpTask->m_xParamStack.vPush(xValSet);
  
  EXIT();
}

// sockets: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* bytes.magnitude -- return magnitude of bytes                               */
/*----------------------------------------------------------------------------*/
void Task::vBytesMagnitude(NNP_Task nnpTask)
{
  ENTRY("vBytesMagnitude");
  
  i8 iBytes;
  if ( ! nnpTask->m_xParamStack.bPop(iBytes) )
    THROW_INVALID_PARAM("bytes.magnitude");
  
  s256 sMag;
  Magnitudes::vMagnitude(iBytes, sMag.a());

  nnpTask->m_xParamStack.vPush(sMag);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Task::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
