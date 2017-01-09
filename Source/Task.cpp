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
  
  RASSERT( !pTask || CN_TASK == pTask->eClass() );

  return pTask;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Task* Tasks::_pTask(i8 iAddr)
{
  Task* pTask = reinterpret_cast<Task*>(iAddr);
  
  RASSERT( !pTask || CN_TASK == pTask->eClass() );
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Task::operator new(size_t iSize)
{
  RASSERT(iSize);
  ASSERT(gpTaskList);
  
  Task* pTask = reinterpret_cast<Task*>(Heap::pAllocate(CN_TASK));
  
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
  
  //m_xDataStack
  //m_xReturnStack
  //m_xAuxStack
  m_pRegFrame    = nullptr;
  m_pLocalsFrame = nullptr;
  
  IP.m_pCell   = nullptr;
  m_pEntryCell   = nullptr;
  
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
  pNewWord->vEntryStep(pStep);
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
  
  // CN_ : class numbers
  /*  0 */ vAddConstant("CN_NONE",         CN_NONE);

  /*  1 */ vAddConstant("CN_S32",          CN_S32);
  /*  2 */ vAddConstant("CN_S256",         CN_S256);
  /*  3 */ vAddConstant("CN_S8K",          CN_S8K);

  /*  4 */ vAddConstant("CN_INTVAL",       CN_INTVAL);
  /*  5 */ vAddConstant("CN_OBJVAL",       CN_OBJVAL);
  /*  6 */ vAddConstant("CN_PTRVAL",       CN_PTRVAL);
  /*  7 */ vAddConstant("CN_STRVAL",       CN_STRVAL);
  /*  8 */ vAddConstant("CN_VALREG",       CN_VALREG);
  /*  9 */ vAddConstant("CN_VALSET",       CN_VALSET);
  /* 10 */ vAddConstant("CN_VALWSP",       CN_VALWSP);
  
  /* 11 */ vAddConstant("CN_ARGUMENTS",    CN_ARGUMENTS);
  /* 12 */ vAddConstant("CN_ATOMIC",       CN_ATOMIC);
  /* 13 */ vAddConstant("CN_ATTACHMAP",    CN_ATTACHMAP);
  
  /* 14 */ vAddConstant("CN_BUFFER",       CN_BUFFER);
  /* 15 */ vAddConstant("CN_BUFFERPOOL",   CN_BUFFERPOOL);
  /* 16 */ vAddConstant("CN_BPA",          CN_BPA);
  
  /* 17 */ vAddConstant("CN_DIC",          CN_DIC);
  /* 18 */ vAddConstant("CN_DICSTACK",     CN_DICSTACK);
  /* 19 */ vAddConstant("CN_DIR",          CN_DIR);
  
  /* 20 */ vAddConstant("CN_FILE",         CN_FILE);
  /* 21 */ vAddConstant("CN_FREECHUNK",    CN_FREECHUNK);
  
  /* 22 */ vAddConstant("CN_GRAPH",        CN_GRAPH);
  /* 23 */ vAddConstant("CN_GRAPHCURSOR",  CN_GRAPHCURSOR);
  /* 24 */ vAddConstant("CN_GRAPHNODE",    CN_GRAPHNODE);
  /* 25 */ vAddConstant("CN_GRID",         CN_GRID);
  
  /* 26 */ vAddConstant("CN_HEAPBUFFER",   CN_HEAPBUFFER);
  
  /* 27 */ vAddConstant("CN_INDEX",        CN_INDEX);

  /* 28 */ vAddConstant("CN_JUMBOHEAP",    CN_JUMBOHEAP);

  /* 29 */ vAddConstant("CN_KEYCPP",       CN_KEYCPP);
  /* 30 */ vAddConstant("CN_KEYVAL",       CN_KEYVAL);
  
  /* 31 */ vAddConstant("CN_LOADCHUNK",    CN_LOADCHUNK);
  /* 32 */ vAddConstant("CN_LOADER",       CN_LOADER);
  /* 33 */ vAddConstant("CN_LOADERSUB",    CN_LOADERSUB);
  /* 34 */ vAddConstant("CN_LOADMAP",      CN_LOADMAP);
  /* 35 */ vAddConstant("CN_LOCK",         CN_LOCK);
  
  /* 36 */ vAddConstant("CN_MEDIUMHEAP",   CN_MEDIUMHEAP);
  
  /* 37 */ vAddConstant("CN_NODE",         CN_NODE);
  /* 38 */ vAddConstant("CN_NODECURSOR",   CN_NODECURSOR);
  
  /* 39 */ vAddConstant("CN_PARSER",       CN_PARSER);
  /* 40 */ vAddConstant("CN_PARTITION",    CN_PARTITION);
  /* 41 */ vAddConstant("CN_PARTITIONS",   CN_PARTITIONS);
  /* 42 */ vAddConstant("CN_PROXY",        CN_PROXY);
  /* 43 */ vAddConstant("CN_PROXYSET",     CN_PROXYSET);
  
  /* 44 */ vAddConstant("CN_QUEUE",        CN_QUEUE);
  /* 45 */ vAddConstant("CN_QUEUEBUFFER",  CN_QUEUEBUFFER);
  
  /* 46 */ vAddConstant("CN_REGFRAME",     CN_REGFRAME);
  /* 47 */ vAddConstant("CN_RING",         CN_RING);
  /* 48 */ vAddConstant("CN_ROW",          CN_ROW);
  /* 49 */ vAddConstant("CN_ROWBUFFER",    CN_ROWBUFFER);
  /* 50 */ vAddConstant("CN_ROWCURSOR",    CN_ROWCURSOR);
  /* 51 */ vAddConstant("CN_ROWSTRAND",    CN_ROWSTRAND);
  /* 52 */ vAddConstant("CN_ROWWSP",       CN_ROWWSP);
  
  /* 53 */ vAddConstant("CN_SCANBUFFER",   CN_SCANBUFFER);
  /* 54 */ vAddConstant("CN_SCRATCH",      CN_SCRATCH);
  /* 55 */ vAddConstant("CN_SEHISTORY",    CN_SEHISTORY);
  /* 56 */ vAddConstant("CN_SEITF",        CN_SEITF);
  /* 57 */ vAddConstant("CN_SEITFCURSOR",  CN_SEITFCURSOR);
  /* 58 */ vAddConstant("CN_SEITFLIST",    CN_SEITFLIST);
  /* 59 */ vAddConstant("CN_SEPROMPT",     CN_SEPROMPT);
  /* 60 */ vAddConstant("CN_SINCE",        CN_SINCE);
  /* 61 */ vAddConstant("CN_SKIPCURSOR",   CN_SKIPCURSOR);
  /* 62 */ vAddConstant("CN_SKIPLIST",     CN_SKIPLIST);
  /* 63 */ vAddConstant("CN_SKIPNODE",     CN_SKIPNODE);
  /* 64 */ vAddConstant("CN_SMALLHEAP",    CN_SMALLHEAP);
  /* 65 */ vAddConstant("CN_SOCKET",       CN_SOCKET);
  /* 66 */ vAddConstant("CN_STACK",        CN_STACK);
  /* 67 */ vAddConstant("CN_STEPACCUM",    CN_STEPACCUM);
  /* 68 */ vAddConstant("CN_STEPARRAY",    CN_STEPARRAY);
  /* 69 */ vAddConstant("CN_SYSCALL",      CN_SYSCALL);
  
  /* 70 */ vAddConstant("CN_TABLE",        CN_TABLE);
  /* 71 */ vAddConstant("CN_TASK",         CN_TASK);
  /* 72 */ vAddConstant("CN_TASKCURSOR",   CN_TASKCURSOR);
  /* 73 */ vAddConstant("CN_TASKLIST",     CN_TASKLIST);
  /* 74 */ vAddConstant("CN_TASKSET",      CN_TASKSET);
  /* 75 */ vAddConstant("CN_THREAD",       CN_THREAD);
  /* 76 */ vAddConstant("CN_THREADCURSOR", CN_THREADCURSOR);
  /* 77 */ vAddConstant("CN_THREADLIST",   CN_THREADLIST);
  /* 78 */ vAddConstant("CN_TIME",         CN_TIME);
  /* 79 */ vAddConstant("CN_TIMER",        CN_TIMER);
  /* 80 */ vAddConstant("CN_TOKENISER",    CN_TOKENISER);
  /* 81 */ vAddConstant("CN_TX",           CN_TX);
  /* 82 */ vAddConstant("CN_TXARRAY",      CN_TXARRAY);
  /* 83 */ vAddConstant("CN_TXCOLLECTION", CN_TXCOLLECTION);
  /* 84 */ vAddConstant("CN_TXCURSOR",     CN_TXCURSOR);
  /* 85 */ vAddConstant("CN_TXMAP",        CN_TXMAP);
  /* 86 */ vAddConstant("CN_TXREF",        CN_TXREF);
  
  /* 87 */ vAddConstant("CN_WORD",         CN_WORD);

  /* 88 */ vAddConstant("CN_ZSMALL_",      CN_ZSMALL_);
  /* 89 */ vAddConstant("CN_ZMEDIUM_",     CN_ZMEDIUM_);
  /* 90 */ vAddConstant("CN_ZLARGE_",      CN_ZLARGE_);
  /* 91 */ vAddConstant("CN_ZJUMBO_",      CN_ZJUMBO_);
  
  /* 92 */ vAddConstant("CN_ENUMS",        CN_ENUMS);
  
  // DE_ : directory entry type -- n.b. the DT_FIFO etc are from dirent.h
  /*  1 */ vAddConstant("DE_FIFO", DT_FIFO);
  /*  2 */ vAddConstant("DE_CHR",  DT_CHR);
  /*  3 */ vAddConstant("DE_DIR",  DT_DIR);
  /*  6 */ vAddConstant("DE_BLK",  DT_BLK);
  /*  8 */ vAddConstant("DE_REG",  DT_REG);
  /* 10 */ vAddConstant("DE_LNK",  DT_LNK);
  /* 12 */ vAddConstant("DE_SOCK", DT_SOCK);
  
  // EC_ : error codes
  /*  0 */ vAddConstant("EC_NONE",               EC_NONE);
  /*  1 */ vAddConstant("EC_ASSERT",             EC_ASSERT);
  /*  2 */ vAddConstant("EC_BEGIN_TX_FAILED",    EC_BEGIN_TX_FAILED);
  /*  3 */ vAddConstant("EC_CATALOG_EXISTS",     EC_CATALOG_EXISTS);
  /*  4 */ vAddConstant("EC_FOLDER_IN_USE",      EC_FOLDER_IN_USE);
  /*  5 */ vAddConstant("EC_GRID_NOT_STARTED",   EC_GRID_NOT_STARTED);
  /*  6 */ vAddConstant("EC_INVALID_CLASS",      EC_INVALID_CLASS);
  /*  7 */ vAddConstant("EC_INVALID_COMMAND",    EC_INVALID_COMMAND);
  /*  8 */ vAddConstant("EC_INVALID_FILE",       EC_INVALID_FILE);
  /*  9 */ vAddConstant("EC_INVALID_INTERFACE",  EC_INVALID_INTERFACE);
  /* 10 */ vAddConstant("EC_INVALID_PARAMETER",  EC_INVALID_PARAMETER);
  /* 11 */ vAddConstant("EC_INVALID_PARAMETERS", EC_INVALID_PARAMETERS);
  /* 12 */ vAddConstant("EC_INVALID_INTERFACE",  EC_INVALID_INTERFACE);
  /* 13 */ vAddConstant("EC_INVALID_SYNTAX",     EC_INVALID_SYNTAX);
  /* 14 */ vAddConstant("EC_LOW_BUFFERS",        EC_LOW_BUFFERS);
  /* 15 */ vAddConstant("EC_NO_COLUMNS_DIC",     EC_NO_COLUMNS_DIC);
  /* 16 */ vAddConstant("EC_NO_SUCH_CATALOG",    EC_NO_SUCH_CATALOG);
  /* 17 */ vAddConstant("EC_NO_SUCH_DIRECTORY",  EC_NO_SUCH_DIRECTORY);
  /* 18 */ vAddConstant("EC_NO_SUCH_FOLDER",     EC_NO_SUCH_FOLDER);
  /* 19 */ vAddConstant("EC_NO_SUCH_SCHEMA",     EC_NO_SUCH_SCHEMA);
  /* 20 */ vAddConstant("EC_NO_SUCH_TABLE",      EC_NO_SUCH_TABLE);
  /* 21 */ vAddConstant("EC_NO_SUCH_TENANT",     EC_NO_SUCH_TENANT);
  /* 22 */ vAddConstant("EC_NO_SUCH_WORD",       EC_NO_SUCH_WORD);
  /* 23 */ vAddConstant("EC_NOT_A_CATALOG",      EC_NOT_A_CATALOG);
  /* 24 */ vAddConstant("EC_NOT_A_FOLDER",       EC_NOT_A_FOLDER);
  /* 25 */ vAddConstant("EC_NOT_A_SCHEMA",       EC_NOT_A_SCHEMA);
  /* 26 */ vAddConstant("EC_NOT_A_TABLE",        EC_NOT_A_TABLE);
  /* 27 */ vAddConstant("EC_NOT_A_TENANT",       EC_NOT_A_TENANT);
  /* 28 */ vAddConstant("EC_NOT_IN_CATALOGS",    EC_NOT_IN_CATALOGS);
  /* 29 */ vAddConstant("EC_NOT_IN_COLUMNS",     EC_NOT_IN_COLUMNS);
  /* 31 */ vAddConstant("EC_NOT_IN_SCHEMAS",     EC_NOT_IN_SCHEMAS);
  /* 31 */ vAddConstant("EC_NOT_IN_TABLES",      EC_NOT_IN_TABLES);
  /* 32 */ vAddConstant("EC_NOT_IN_TENANTS",     EC_NOT_IN_TENANTS);
  /* 33 */ vAddConstant("EC_NULL_INTERFACE",     EC_NULL_INTERFACE);
  /* 34 */ vAddConstant("EC_NULLPTR",            EC_NULLPTR);
  /* 35 */ vAddConstant("EC_RESPONSE_PENDING",   EC_RESPONSE_PENDING);
  /* 36 */ vAddConstant("EC_SCHEMA_EXISTS",      EC_SCHEMA_EXISTS);
  /* 37 */ vAddConstant("EC_SEND_FAILURE",       EC_SEND_FAILURE);
  /* 38 */ vAddConstant("EC_SE_NOT_RUNNING",     EC_SE_NOT_RUNNING);
  /* 39 */ vAddConstant("EC_TABLE_EXISTS",       EC_TABLE_EXISTS);
  /* 40 */ vAddConstant("EC_TABLE_IN_USE",       EC_TABLE_IN_USE);
  /* 41 */ vAddConstant("EC_TENANT_EXISTS",      EC_TENANT_EXISTS);
  /* 42 */ vAddConstant("EC_VERSION_MISMATCH",   EC_VERSION_MISMATCH);
  /* 43 */ vAddConstant("EC_ENUMS",              EC_ENUMS);

  // TK_ : token types
  /*  0 */ vAddConstant("TK_NONE",    TK_NONE);
  /*  1 */ vAddConstant("TK_NAME",    TK_NAME);
  /*  2 */ vAddConstant("TK_INTEGER", TK_INTEGER);
  /*  3 */ vAddConstant("TK_STRING",  TK_STRING);
  /*  4 */ vAddConstant("TK_ENUMS",   TK_ENUMS);
  
  // SIZEOF_ : C++ class sizes
  /*  1 */ vAddConstant("SIZEOF_INTVAL", sizeof(IntVal));
  /*  2 */ vAddConstant("SIZEOF_OBJVAL", sizeof(ObjVal));
  /*  3 */ vAddConstant("SIZEOF_PTRVAL", sizeof(PtrVal));
  /*  4 */ vAddConstant("SIZEOF_STRVAL", sizeof(StrVal));
  /*  5 */ vAddConstant("SIZEOF_VALSET", sizeof(ValWsp)); // correct
  /*  6 */ vAddConstant("SIZEOF_VALWSP", sizeof(ValWsp)); // correct
  
  /*  7 */ vAddConstant("SIZEOF_ARGUMENTS",    sizeof(Arguments));
  /*  8 */ vAddConstant("SIZEOF_ATOMIC",       sizeof(Atomic));
  /*  9 */ vAddConstant("SIZEOF_ATTACHMAP",    sizeof(AttachMap));
  /* 10 */ vAddConstant("SIZEOF_BUFFER",       sizeof(Buffer));
  /* 11 */ vAddConstant("SIZEOF_BUFFERPOOL",   sizeof(BufferPool));
  /* 12 */ vAddConstant("SIZEOF_BPA",          sizeof(BPA));
  /* 13 */ vAddConstant("SIZEOF_DIC",          sizeof(Dic));
  /* 14 */ vAddConstant("SIZEOF_DICSTACK",     sizeof(DicStack));
  /* 15 */ vAddConstant("SIZEOF_DIR",          sizeof(Dir));
  /* 16 */ vAddConstant("SIZEOF_FILE",         sizeof(File));
  /* 17 */ vAddConstant("SIZEOF_FREECHUNK",    sizeof(FreeChunk));
  /* 18 */ vAddConstant("SIZEOF_GRID",         sizeof(Grid));
  /* 19 */ vAddConstant("SIZEOF_HEAPBUFFER",   sizeof(HeapBuffer));
  /* 20 */ vAddConstant("SIZEOF_INDEX",        sizeof(Index));
  /* 21 */ vAddConstant("SIZEOF_LOADER",       sizeof(Loader));
  /* 22 */ vAddConstant("SIZEOF_LOADERSUB",    sizeof(LoaderSub));
  /* 23 */ vAddConstant("SIZEOF_LOCK",         sizeof(Lock));
  /* 24 */ vAddConstant("SIZEOF_MEDIUMHEAP",   sizeof(Heap));
  /* 25 */ vAddConstant("SIZEOF_NODE",         sizeof(Node));
  /* 26 */ vAddConstant("SIZEOF_NODECURSOR",   sizeof(NodeCursor));
  /* 27 */ vAddConstant("SIZEOF_PARSER",       sizeof(Parser));
  /* 28 */ vAddConstant("SIZEOF_PARTITION",    sizeof(Partition));
  /* 29 */ vAddConstant("SIZEOF_PARTITIONS",   sizeof(Partitions));
  /* 30 */ vAddConstant("SIZEOF_PROXY",        sizeof(Proxy));
  /* 31 */ vAddConstant("SIZEOF_PROXYSET",     sizeof(ProxySet));
  /* 32 */ vAddConstant("SIZEOF_QUEUE",        sizeof(Queue));
  /* 33 */ vAddConstant("SIZEOF_QUEUEBUFFER",  sizeof(QueueBuffer));
  /* 34 */ vAddConstant("SIZEOF_RING",         sizeof(Ring));
  /* 35 */ vAddConstant("SIZEOF_ROW",          sizeof(Row));
  /* 36 */ vAddConstant("SIZEOF_ROWBUFFER",    sizeof(RowBuffer));
  /* 37 */ vAddConstant("SIZEOF_ROWCURSOR",    sizeof(RowCursor));
  /* 38 */ vAddConstant("SIZEOF_ROWSTRAND",    sizeof(RowStrand));
  /* 39 */ vAddConstant("SIZEOF_ROWWSP",       sizeof(RowWsp));
  /* 40 */ vAddConstant("SIZEOF_SCANBUFFER",   sizeof(ScanBuffer));
  /* 41 */ vAddConstant("SIZEOF_SEHISTORY",    sizeof(SeHistory));
  /* 42 */ vAddConstant("SIZEOF_SEITF",        sizeof(SEitf));
  /* 43 */ vAddConstant("SIZEOF_SEITFCURSOR",  sizeof(SEitfCursor));
  /* 44 */ vAddConstant("SIZEOF_SEITFLIST",    sizeof(SEitfList));
  /* 45 */ vAddConstant("SIZEOF_SEPROMPT",     sizeof(SePrompt));
  /* 46 */ vAddConstant("SIZEOF_SINCE",        sizeof(Since));
  /* 47 */ vAddConstant("SIZEOF_SKIPCURSOR",   sizeof(SkipCursor));
  /* 48 */ vAddConstant("SIZEOF_SKIPLIST",     sizeof(SkipList));
  /* 49 */ vAddConstant("SIZEOF_SKIPNODE",     sizeof(SkipNode));
  /* 50 */ vAddConstant("SIZEOF_SMALLHEAP",    sizeof(Heap));
  /* 51 */ vAddConstant("SIZEOF_SOCKET",       sizeof(Socket));
  /* 52 */ vAddConstant("SIZEOF_STACK",        sizeof(Stack));
  /* 53 */ vAddConstant("SIZEOF_STEPACCUM",    sizeof(StepAccum));
  /* 54 */ vAddConstant("SIZEOF_STEPARRAY",    sizeof(StepArray));
  /* 55 */ vAddConstant("SIZEOF_SYSCALL",      sizeof(SysCall));
  /* 56 */ vAddConstant("SIZEOF_TABLE",        sizeof(Table));
  /* 57 */ vAddConstant("SIZEOF_TASK",         sizeof(Task));
  /* 58 */ vAddConstant("SIZEOF_TASKCURSOR",   sizeof(TaskCursor));
  /* 59 */ vAddConstant("SIZEOF_TASKLIST",     sizeof(TaskList));
  /* 60 */ vAddConstant("SIZEOF_TASKSET",      sizeof(TaskSet));
  /* 61 */ vAddConstant("SIZEOF_THREAD",       sizeof(Thread));
  /* 62 */ vAddConstant("SIZEOF_THREADCURSOR", sizeof(ThreadCursor));
  /* 63 */ vAddConstant("SIZEOF_THREADLIST",   sizeof(ThreadList));
  /* 64 */ vAddConstant("SIZEOF_TIME",         sizeof(Time));
  /* 65 */ vAddConstant("SIZEOF_TIMER",        sizeof(Timer));
  /* 66 */ vAddConstant("SIZEOF_TOKENISER",    sizeof(Tokeniser));
  /* 67 */ vAddConstant("SIZEOF_TX",           sizeof(Tx));
  /* 68 */ vAddConstant("SIZEOF_TXARRAY",      sizeof(TxArray));
  /* 69 */ vAddConstant("SIZEOF_TXCOLLECTION", sizeof(TxCollection));
  /* 70 */ vAddConstant("SIZEOF_TXCURSOR",     sizeof(TxCursor));
  /* 71 */ vAddConstant("SIZEOF_TXMAP",        sizeof(TxMap));
  /* 72 */ vAddConstant("SIZEOF_TXREF",        sizeof(TxRef));
  /* 73 */ vAddConstant("SIZEOF_WORD",         sizeof(Word));

  /*  0 */ vAddConstant("SIZEOF_S32",          sizeof(s32));
  /*  1 */ vAddConstant("SIZEOF_S256",         sizeof(s256));
  /*  2 */ vAddConstant("SIZEOF_S16K",         sizeof(s8K));

  // TS_ : task state
  /*  0 */ vAddConstant("TS_NONE",           TS_NONE);
  /*  1 */ vAddConstant("TS_BLOCKED",        TS_BLOCKED);
  /*  2 */ vAddConstant("TS_EXITED",         TS_EXITED);
  /*  3 */ vAddConstant("TS_RUNNING",        TS_RUNNING);
  /*  4 */ vAddConstant("TS_SLEEPING",       TS_SLEEPING);
  /*  5 */ vAddConstant("TS_WAITING_ACK",    TS_WAITING_ACK);
  /*  6 */ vAddConstant("TS_WAITING_SIGNAL", TS_WAITING_SIGNAL);
  /*  7 */ vAddConstant("TS_YIELDED",        TS_YIELDED);
  /*  8 */ vAddConstant("TS_ENUMS",          TS_ENUMS);

  // TP_ : transaction phase
  /*  0 */ vAddConstant("TP_NONE",        TP_NONE);
  /*  1 */ vAddConstant("TP_EXECUTING",   TP_EXECUTING);
  /*  2 */ vAddConstant("TP_COMMITTING",  TP_COMMITTING);
  /*  3 */ vAddConstant("TP_COMMITTED",   TP_COMMITTED);
  /*  4 */ vAddConstant("TP_PREPARING",   TP_PREPARING);
  /*  5 */ vAddConstant("TP_PREPARED",    TP_PREPARED);
  /*  6 */ vAddConstant("TP_ROLLINGBACK", TP_ROLLINGBACK);
  /*  7 */ vAddConstant("TP_ROLLEDBACK",  TP_ROLLEDBACK);
  /*  8 */ vAddConstant("TP_FREE",        TP_FREE);
  /*  9 */ vAddConstant("TP_ENUMS",       TP_ENUMS);

  // WT_ : word type
  /*  0 */ vAddConstant("WT_NONE",      WT_NONE);
  /*  0 */ vAddConstant("WT_OBJECT",    WT_OBJECT);
  /*  0 */ vAddConstant("WT_PRIMITIVE", WT_PRIMITIVE);
  /*  0 */ vAddConstant("WT_ROUTINE",   WT_ROUTINE);
  /*  0 */ vAddConstant("WT_LOCAL",     WT_LOCAL);
  /*  0 */ vAddConstant("WT_ENUMS",     WT_ENUMS);
  
  // OPEN_ : file open argument
  /*  0 */ vAddConstant("OPEN_READ",              C_OPEN_READ);
  /*  1 */ vAddConstant("OPEN_WRITE",             C_OPEN_WRITE);
  /*  2 */ vAddConstant("OPEN_APPEND",            C_OPEN_APPEND);
  /*  3 */ vAddConstant("OPEN_READ_WRITE_EXIST",  C_OPEN_READ_WRITE_EXIST);
  /*  4 */ vAddConstant("OPEN_READ_WRITE_CREATE", C_OPEN_READ_WRITE_CREATE);
  /*  5 */ vAddConstant("OPEN_READ_APPEND",       C_OPEN_READ_APPEND);
  
  // miscellaneous
  /*  0 */ vAddConstant("CLOCK_FREQUENCY", Synch::iTickFrequency());
  /*  0 */ vAddConstant("CORE_COUNT",      System::iCoreCount());
  /*  0 */ vAddConstant("CPU_COUNT",       System::iCpuCount());
  /*  0 */ vAddConstant("PHYSICAL_MEMORY", System::iPhysicalMemory());
  
  vAddConstant("listerTaskPrint",    (i8)0);
  vAddConstant("monitorTaskPrint",   (i8)0);
  vAddConstant("reclaimTaskPrint",   (i8)0);
  vAddConstant("requestTaskPrint",   (i8)0);
  vAddConstant("answerTaskPrint",    (i8)0);
  vAddConstant("connectorTaskPrint", (i8)0);
  vAddConstant("senderTaskPrint",    (i8)0);
  vAddConstant("receiverTaskPrint",  (i8)0);
}

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitive("word",            &vWord,             false);
  gpWord = pFindWord("word");

  vAddPrimitives_Address();
  vAddPrimitives_Args();
  vAddPrimitives_Assert();
  vAddPrimitives_Astack();
  vAddPrimitives_Bp();
  vAddPrimitives_Cast();
  vAddPrimitives_Class();
  vAddPrimitives_Comment();
  vAddPrimitives_Cpu();
  vAddPrimitives_Cursor();
  vAddPrimitives_DdlLock();
  vAddPrimitives_Debug();
  vAddPrimitives_Dic();
  vAddPrimitives_DicLock();
  vAddPrimitives_DicStack();
  vAddPrimitives_Dir();
  vAddPrimitives_Docol();
  vAddPrimitives_Dot();
  vAddPrimitives_Dstack();
  vAddPrimitives_Encode();
  vAddPrimitives_Enter();
  vAddPrimitives_Env();
  vAddPrimitives_Error();
  vAddPrimitives_Exit();
  vAddPrimitives_File();
  vAddPrimitives_Flow();
  vAddPrimitives_Grid();
  vAddPrimitives_Input();
  vAddPrimitives_Int();
  vAddPrimitives_Loader();
  vAddPrimitives_LoaderSub();
  vAddPrimitives_Local();
  vAddPrimitives_Lock();
  vAddPrimitives_Logic();
  vAddPrimitives_Magnitude();
  vAddPrimitives_Memory();
  vAddPrimitives_Node();
  vAddPrimitives_NodeCursor();
  vAddPrimitives_Object();
  vAddPrimitives_Process();
  vAddPrimitives_Push();
  vAddPrimitives_Queue();
  vAddPrimitives_RowBuffer();
  vAddPrimitives_RowCursor();
  vAddPrimitives_Reg();
  vAddPrimitives_Rstack();
  vAddPrimitives_SEitf();
  vAddPrimitives_SEitfCursor();
  vAddPrimitives_SEitfList();
  vAddPrimitives_Shell();
  vAddPrimitives_Socket();
  vAddPrimitives_Steps();
  vAddPrimitives_Str();
  vAddPrimitives_Table();
  vAddPrimitives_Task();
  vAddPrimitives_TaskCursor();
  vAddPrimitives_TaskList();
  vAddPrimitives_TaskSet();
  vAddPrimitives_Thread();
  vAddPrimitives_ThreadCursor();
  vAddPrimitives_ThreadList();
  vAddPrimitives_Timer();
  vAddPrimitives_To();
  vAddPrimitives_Trace();
  vAddPrimitives_Tx();
  vAddPrimitives_TxCursor();
  vAddPrimitives_TxCollection();
  vAddPrimitives_TxMap();
  vAddPrimitives_TxRef();
  vAddPrimitives_Word();
  vAddPrimitives_Zero();
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
  ": docol"
  
  "  retryPoint"
  "  begin_"
  
  // get next input token
  "    input.next"
  
  // test for end of input?
  "    input.type TK_NONE == if"
  "      exit"
  "    then"
  
  // is token an integer?
  "    input.type TK_INTEGER == if"
  
  "      input.token >int"        // convert str to int, leave on stack
  
  "      docol.isInterpreting if" // interpret leaves int on stack
  "        jumpBegin_"
  "      then"
  
  "      docol.isCompiling if"    // compile encodes push intval
  "        encodePushValue"
  "        jumpBegin_"
  "      then"
  
  "    then"
  
  // is token a string?
  "    input.type TK_STRING == if"
  
  "      input.token"             // put str on stack
  
  "      docol.isInterpreting if" // interpret leaves str on stack
  "        jumpBegin_"
  "      then"
  
  "      docol.isCompiling if"    // compile encodes push strval
  "        encodePushValue"
  "        jumpBegin_"
  "      then"
  
  "    then"
  
  // should be a name
  "    input.type TK_NAME != if"  // double check we have a name
  "      \"strange token\" . input.type ."
  "      jumpBegin_"
  "    then"
  
  // is token a known word?
  "    input.token find"
  "    dup not if"                // we have an unknown word
  "      drop"
  "      \"unknown\" ."
  "      input.token ."
  "      dic.isCompiled not if process.quit then "
  "      jumpBegin_"
  "    then"
  
  // known word on stack
  
  // is an immediate word?
  "    dup word.isImmediate if"   // execute an immediate word
  "      execute"
  "      jumpBegin_"
  "    then"
  
  // are we compiling?
  "    docol.isCompiling if "     // compile encodes call to word
  "      dup word.isPrimitive if"
  "        encodeCallPrimitive"   // primitive
  "        jumpBegin_"
  "      then"
  "      encodeCallRoutine"       // routine
  "      jumpBegin_"
  "    then"
  
  // we are interpreting
  "    execute"                   // execute word
  
  "  jumpBegin_"
  
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
  "  \"::dicCompile\" .verbose"
//  "  gwd .verbose"
//  "  \"../../../../Commands\" cwd"
//  "  gwd .verbose"
//  "  traceon"
  "  \"compile.sen\" >>:"
  "  \"::dicCompile exit\" .verbose"
  "  ::exit"
  "  ;";
#else
  sDefinition =
  ": ::dicCompile"
  "  \"::dicCompile\" .verbose"
//  "  gwd .verbose"
//  "  \"../../Commands\" cwd"
//  "  gwd .verbose"
  "  \"compile.sen\" >>:"
  "  \"::dicCompile exit\" .verbose"
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
  Word* pCalledWord;
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
        pNewWord->vEncode("pushValue",&xValWsp);
        break;
        
      case TK_NAME:
        pCalledWord = pFindWord(m_sToken);
        if (WT_PRIMITIVE == pCalledWord->eWordType()) {
          pStep = pCalledWord->pEntryStep();
          if (pCalledWord->bImmediate())
            (*pStep)(this);
          else {
            pNewWord->vAppend(pCalledWord->ppEntryStep());
          }
        }
        else if (WT_ROUTINE == pCalledWord->eWordType()) {
          TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
          pNewWord->vEncode(ppCalledEntryStep);
        }
        else {
          THROW("Invalid eWordType %lld",pCalledWord->eWordType());
        }
        break;
        
      case TK_STRING: {
        xValWsp = m_sToken.a();
        pNewWord->vEncode("pushValue",&xValWsp);
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
  
  i8 iDataValues  = m_xDataStack.iValues();
  i8 iReturnValues = m_xReturnStack.iValues();
  vTrace("%-15s %15s : r%lld ; d%2d=",
         aRoutine,aPrimitive,iReturnValues,iDataValues);
  
//  for (i8 iValue = iParamValues - 1; iValue >= 0; iValue--) {
//    
//    if (iValue != iParamValues-1)
//      vTrace(",");
//    
//    ValWsp* pValWsp = static_cast<ValWsp*>(m_xDataStack.pPeek(iValue));
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
  vStackContents(&m_xDataStack, sStackContents);
  
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
  
  m_xDataStack.vReset();
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
  
  m_xDataStack.vRetry();
  m_xReturnStack.vRetry();
}

/*----------------------------------------------------------------------------*/
/* get root thread from root tasks param stack                                */
/*----------------------------------------------------------------------------*/
Thread* Task::pRootThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Object* pThreadEmb;
  
  if ( ! m_xDataStack.bPop(pThreadEmb) ) {
    String::vSprint(m_sException.a(),"Invalid param stack for: h.spawn");
    m_sFile = __FILE__;
    throw (i8)__LINE__;
  }
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  return pThread;
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
  
  m_xDataStack.vReset();
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
  
  m_xDataStack.vReset();
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
  
  m_xDataStack.vReset();
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
      m_xDataStack.bPop(sValue);
      
      for (char* pv = sValue.a(); *pv; )
        *ps++ = *pv++;
      
      continue;
    }

    // test for %i
    if ('i' == *pf) {
      
      pf++;
      
      i8 iValue;
      m_xDataStack.bPop(iValue);
      
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
    
    Value* pValue0 = m_xDataStack.pPeek(0);
    
    // test for strval
    if (CN_STRVAL == pValue0->eClass()) {

      *pf = 's';
      pf++;
      
      s8K sValue;
      m_xDataStack.bPop(sValue);
      
      for (char* pv = sValue.a(); *pv; )
        *ps++ = *pv++;
      
      continue;
    }
    
    // test for intval
    if (CN_INTVAL == pValue0->eClass()) {
    
      *pf = 'i';
      pf++;
      
      i8 iValue;
      m_xDataStack.bPop(iValue);
      
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
        else {
          Object* pObject = (Object*)pValWsp->pPtr();
          
          if (pObject->iMagic() == MAGIC && pObject->eClass() < CN_ENUMS)
            String::vSprintCat(a,"^%s=%p",
                               pCN[pObject->eClass()],
                               pValWsp->pPtr());
          else
            String::vSprintCat(a,"%p",
                               pValWsp->pPtr());
        }
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
  i8 iDataValues   = m_xDataStack.iValues();
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
  
  vTrace(" r%2lld  d%2lld=", iReturnValues, iDataValues);

  vStackContents(&m_xDataStack, sStackContents);
  
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
  
  while (TS_RUNNING == m_eState || TS_WAITING_SIGNAL == m_eState) {
    
    assert(0 == m_iTaskNo || m_pThread);
    
    TaskStep pTaskStep = *IP.m_pCell->m_ppEntryStep;
    
    m_pEntryCell = IP.m_pCell->m_ppEntryStep + 1;
    
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
