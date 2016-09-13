/*============================================================================*/
/* Task.cpp - task                                                            */
/*============================================================================*/

#define USE_INPUTSTREAM

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"


extern Arguments             gxArguments;

extern AttachMap*            gpAttachMap;
extern BufferPool*           gpBufferPool;
extern Heap*                 gpMediumHeap;
extern BufferPoolAnalyzer*   gpBPA;
extern Dictionary*           gpMainDic;
extern Word*                 gpWord;

extern Grid*                 gpGrid;
extern StepArray*            gpStepArray;
extern ThreadList*           gpThreadList;
extern TaskList*             gpTaskList;
extern TxCollection*         gpTxCollection;

extern NP_Task               gnpRootTask;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Task::operator new(size_t iSize)
{
  ASSERT(iSize);
  ASSERT(gpTaskList);
  
  Task* pTask = (Task*)gpMediumHeap->pAllocObject(CN_TASK);
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Task::operator new(size_t iSize, Task* pTask)
{
  ASSERT(iSize);
  ASSERT(gpTaskList);
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Task::operator delete(void* pTaskAddress)
{
  Task* pTask = reinterpret_cast<Task*>(pTaskAddress);
  
  gpMediumHeap->vFreeObject(pTask);
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
  
  Task* pTask = this;
  gpTaskList->vAppend(pTask);
  
  Memory::vCheckInitialised(this,sizeof(Task));
  
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
  
  Task* pTask = this;
  gpTaskList->vAppend(pTask);
  
  Memory::vCheckInitialised(this,sizeof(Task));
  
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
  
  Task* pTask = this;
  gpTaskList->vAppend(pTask);
  
  Memory::vCheckInitialised(this,sizeof(Task));
  
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
  m_iTaskNo    = iTaskNo++;
  m_sName      = "<no task name>";
  m_eState     = TS_NONE;
  m_iTxId      = -1;
  m_npNextInSet = nullptr;
  m_npPrevInSet = nullptr;
  m_iWakeAt    = 0;
  
  m_xDictionaryStack.vPush(gpMainDic);
  
  m_pQueue = nullptr;
  
  m_pCell      = nullptr;
  m_pResetIP   = nullptr;
  m_pBeginIP   = nullptr;
  m_pWord      = nullptr;
  m_bCompiling = false;
  m_bTracing   = false;
  
  m_sLambda = aLambda;
  if (m_sLambda.iLength())
    String::vConcat(m_sLambda.a()," exit");
  
  m_sToken     = "";
  m_eToken     = TK_NONE;
  m_xTokeniser.vPush();
  m_xTokeniser.vUseString(m_sLambda);
  
  m_pSEitf   = nullptr;
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

  Dictionary* pDic = m_xDictionaryStack.pTop();
  
  pDic->vAddConstant(aName,iVal);
}

/*----------------------------------------------------------------------------*/
/* add constant                                                               */
/*----------------------------------------------------------------------------*/
void Task::vAddConstant(const char* aName, const char* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Dictionary* pDic = m_xDictionaryStack.pTop();
  
  pDic->vAddConstant(aName,aValue);
}

/*----------------------------------------------------------------------------*/
/* add primitive word into dictionary                                         */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitive(const char* pName, pFUN pFunc, bool bImmediate)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Dictionary* pDic = m_xDictionaryStack.pTop();
  
  Word* pWord = new(pDic) Word(pName);
  
  pWord->vWordType(WT_PRIMITIVE);
  pWord->vFunc(pFunc);
  pWord->vImmediate(bImmediate);
}

/*----------------------------------------------------------------------------*/
/* adjust next pointer                                                        */
/*----------------------------------------------------------------------------*/
void Task::vAdjustNext(i8 iBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pCell = reinterpret_cast<Cell*>(pcAdd(m_pCell,iBytes));
}

/*----------------------------------------------------------------------------*/
/* get Object at IP                                                           */
/*----------------------------------------------------------------------------*/
Object* Task::pArgObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Object* pObject = reinterpret_cast<Object*>(m_pCell);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* add constants                                                              */
/*----------------------------------------------------------------------------*/
void Task::vAddConstants(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddConstant("largebuffersize",  C_SIZE_LARGE_BUFFER);
  vAddConstant("mediumbuffersize", C_SIZE_MEDIUM_OBJECT);
  vAddConstant("smallbuffersize",  C_SIZE_SMALL_OBJECT);
  vAddConstant("magic",            MAGIC);
  vAddConstant("regionsize",       gxArguments.m_iRegionSize);
  vAddConstant("bgtasks",          gxArguments.m_bBgTasks);
  vAddConstant("partitions",       gxArguments.m_nPartitions);
  
  vAddConstant("true",  1);
  vAddConstant("false", (i8)0);

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
  /* 11 */ vAddConstant("CN_BUFFERPOOLANALYZER",    CN_BUFFERPOOLANALYZER);
  /* 13 */ vAddConstant("CN_PARSER",                CN_PARSER);
  /* 14 */ vAddConstant("CN_DICTIONARY",            CN_DICTIONARY);
  /* 15 */ vAddConstant("CN_DIR",                   CN_DIR);
  /* 16 */ vAddConstant("CN_FILE",                  CN_FILE);
  /* 17 */ vAddConstant("CN_FREECHUNK",             CN_FREECHUNK);
  /* 18 */ vAddConstant("CN_GRID",                  CN_GRID);
  /* 19 */ vAddConstant("CN_HEAPBUFFER",            CN_HEAPBUFFER);
  /* 20 */ vAddConstant("CN_INDEX",                 CN_INDEX);
  /* 21 */ vAddConstant("CN_LOADER",                CN_LOADER);
  /* 22 */ vAddConstant("CN_LOCK",                  CN_LOCK);
  /* 23 */ vAddConstant("CN_MEDIUMHEAP",            CN_MEDIUMHEAP);
  /* 24 */ vAddConstant("CN_NODE",                  CN_NODE);
  /* 25 */ vAddConstant("CN_NODECURSOR",            CN_NODECURSOR);
  /* 26 */ vAddConstant("CN_PARTITION",             CN_PARTITION);
  /* 27 */ vAddConstant("CN_PROXY",                 CN_PROXY);
  /* 28 */ vAddConstant("CN_PROXYSET",              CN_PROXYSET);
  /* 29 */ vAddConstant("CN_QUEUE",                 CN_QUEUE);
  /* 30 */ vAddConstant("CN_QUEUEBUFFER",           CN_QUEUEBUFFER);
  /* 31 */ vAddConstant("CN_ROW",                   CN_ROW);
  /* 32 */ vAddConstant("CN_ROWBUFFER",             CN_ROWBUFFER);
  /* 33 */ vAddConstant("CN_ROWCURSOR",             CN_ROWCURSOR);
  /* 34 */ vAddConstant("CN_ROWSTRAND",             CN_ROWSTRAND);
  /*    */ vAddConstant("CN_ROWWSP",                CN_ROWWSP);
  /* 35 */ vAddConstant("CN_SCANBUFFER",            CN_SCANBUFFER);
  /* 36 */ vAddConstant("CN_SEITF",                 CN_SEITF);
  /* 37 */ vAddConstant("CN_SEITFCURSOR",           CN_SEITFCURSOR);
  /* 38 */ vAddConstant("CN_SEITFLIST",             CN_SEITFLIST);
  /* 39 */ vAddConstant("CN_SMALLHEAP",             CN_SMALLHEAP);
  /* 40 */ vAddConstant("CN_SOCKET",                CN_SOCKET);
  /* 41 */ vAddConstant("CN_STACK",                 CN_STACK);
  /* 42 */ vAddConstant("CN_SYSCALL",               CN_SYSCALL);
  /* 43 */ vAddConstant("CN_TABLE",                 CN_TABLE);
  /* 44 */ vAddConstant("CN_TASK",                  CN_TASK);
  /* 45 */ vAddConstant("CN_TASKCURSOR",            CN_TASKCURSOR);
  /* 46 */ vAddConstant("CN_TASKLIST",              CN_TASKLIST);
  /* 47 */ vAddConstant("CN_TASKSET",               CN_TASKSET);
  /* 48 */ vAddConstant("CN_THREAD",                CN_THREAD);
  /* 49 */ vAddConstant("CN_THREADCURSOR",          CN_THREADCURSOR);
  /* 50 */ vAddConstant("CN_THREADLIST",            CN_THREADLIST);
  /* 51 */ vAddConstant("CN_TIMER",                 CN_TIMER);
  /* 52 */ vAddConstant("CN_TX",                    CN_TX);
  /* 53 */ vAddConstant("CN_TXARRAY",               CN_TXARRAY);
  /* 54 */ vAddConstant("CN_TXCOLLECTION",          CN_TXCOLLECTION);
  /* 55 */ vAddConstant("CN_TXCURSOR",              CN_TXCURSOR);
  /* 56 */ vAddConstant("CN_TXMAP",                 CN_TXMAP);
  /* 57 */ vAddConstant("CN_TXREF",                 CN_TXREF);
  /* 58 */ vAddConstant("CN_WORD",                  CN_WORD);
  /* 59 */ vAddConstant("CN_ENUMS",                 CN_ENUMS);
  
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
  /* 12 */ vAddConstant("SIZEOF_BUFFERPOOLANALYZER",sizeof(BufferPoolAnalyzer));
  /* 13 */ vAddConstant("SIZEOF_DICTIONARY",        sizeof(Dictionary));
  /* 14 */ vAddConstant("SIZEOF_DICTIONARYSTACK",   sizeof(DictionaryStack));
  /* 15 */ vAddConstant("SIZEOF_DIR",               sizeof(Dir));
  /* 16 */ vAddConstant("SIZEOF_FILE",              sizeof(File));
  /* 17 */ vAddConstant("SIZEOF_FREECHUNK",         sizeof(FreeChunk));
  /* 18 */ vAddConstant("SIZEOF_GRID",              sizeof(Grid));
  /* 19 */ vAddConstant("SIZEOF_HEAPBUFFER",        sizeof(HeapBuffer));
  /* 20 */ vAddConstant("SIZEOF_INDEX",             sizeof(Index));
  /* 21 */ vAddConstant("SIZEOF_LOADER",            sizeof(Loader));
  /* 22 */ vAddConstant("SIZEOF_LOCK",              sizeof(Lock));
  /* 23 */ vAddConstant("SIZEOF_MEDIUMHEAP",        sizeof(Heap));
  /* 24 */ vAddConstant("SIZEOF_NODE",              sizeof(Node));
  /* 25 */ vAddConstant("SIZEOF_NODECURSOR",        sizeof(NodeCursor));
  /* 26 */ vAddConstant("SIZEOF_PARSER",            sizeof(Parser));
  /* 27 */ vAddConstant("SIZEOF_PARTITION",         sizeof(Partition));
  /* 28 */ vAddConstant("SIZEOF_PARTITIONS",        sizeof(Partitions));
  /* 29 */ vAddConstant("SIZEOF_PROXY",             sizeof(Proxy));
  /* 30 */ vAddConstant("SIZEOF_PROXYSET",          sizeof(ProxySet));
  /* 31 */ vAddConstant("SIZEOF_QUEUE",             sizeof(Queue));
  /* 32 */ vAddConstant("SIZEOF_QUEUEBUFFER",       sizeof(QueueBuffer));
  /* 33 */ vAddConstant("SIZEOF_RING",              sizeof(Ring));
  /* 34 */ vAddConstant("SIZEOF_ROW",               sizeof(Row));
  /* 35 */ vAddConstant("SIZEOF_ROWBUFFER",         sizeof(RowBuffer));
  /* 36 */ vAddConstant("SIZEOF_ROWCURSOR",         sizeof(RowCursor));
  /* 37 */ vAddConstant("SIZEOF_ROWSTRAND",         sizeof(RowStrand));
  /* 37 */ vAddConstant("SIZEOF_ROWWSP",         sizeof(RowWsp));
  /* 38 */ vAddConstant("SIZEOF_SCANBUFFER",        sizeof(ScanBuffer));
  /* 39 */ vAddConstant("SIZEOF_SEHISTORY",         sizeof(SeHistory));
  /* 40 */ vAddConstant("SIZEOF_SEITF",             sizeof(SEitf));
  /* 41 */ vAddConstant("SIZEOF_SEITFCURSOR",       sizeof(SEitfCursor));
  /* 42 */ vAddConstant("SIZEOF_SEITFLIST",         sizeof(SEitfList));
  /* 43 */ vAddConstant("SIZEOF_SEPROMPT",          sizeof(SePrompt));
  /* 44 */ vAddConstant("SIZEOF_SINCE",             sizeof(Since));
  /* 45 */ vAddConstant("SIZEOF_SKIPCURSOR",        sizeof(SkipCursor));
  /* 46 */ vAddConstant("SIZEOF_SKIPLIST",          sizeof(SkipList));
  /* 47 */ vAddConstant("SIZEOF_SKIPNODE",          sizeof(SkipNode));
  /* 48 */ vAddConstant("SIZEOF_SMALLHEAP",         sizeof(Heap));
  /* 49 */ vAddConstant("SIZEOF_SOCKET",            sizeof(Socket));
  /* 50 */ vAddConstant("SIZEOF_STACK",             sizeof(Stack));
  /* 51 */ vAddConstant("SIZEOF_STEPACCUM",         sizeof(StepAccum));
  /* 52 */ vAddConstant("SIZEOF_STEPARRAY",         sizeof(StepArray));
  /* 53 */ vAddConstant("SIZEOF_SYSCALL",           sizeof(SysCall));
  /* 54 */ vAddConstant("SIZEOF_TABLE",             sizeof(Table));
  /* 55 */ vAddConstant("SIZEOF_TASK",              sizeof(Task));
  /* 56 */ vAddConstant("SIZEOF_TASKCURSOR",        sizeof(TaskCursor));
  /* 57 */ vAddConstant("SIZEOF_TASKLIST",          sizeof(TaskList));
  /* 58 */ vAddConstant("SIZEOF_TASKSET",           sizeof(TaskSet));
  /* 59 */ vAddConstant("SIZEOF_THREAD",            sizeof(Thread));
  /* 60 */ vAddConstant("SIZEOF_THREADCURSOR",      sizeof(ThreadCursor));
  /* 61 */ vAddConstant("SIZEOF_THREADLIST",        sizeof(ThreadList));
  /* 62 */ vAddConstant("SIZEOF_TIME",              sizeof(Time));
  /* 63 */ vAddConstant("SIZEOF_TIMER",             sizeof(Timer));
  /* 64 */ vAddConstant("SIZEOF_TOKENISER",         sizeof(Tokeniser));
  /* 65 */ vAddConstant("SIZEOF_TX",                sizeof(Tx));
  /* 66 */ vAddConstant("SIZEOF_TXARRAY",           sizeof(TxArray));
  /* 67 */ vAddConstant("SIZEOF_TXCOLLECTION",      sizeof(TxCollection));
  /* 68 */ vAddConstant("SIZEOF_TXCURSOR",          sizeof(TxCursor));
  /* 69 */ vAddConstant("SIZEOF_TXMAP",             sizeof(TxMap));
  /* 70 */ vAddConstant("SIZEOF_TXREF",             sizeof(TxRef));
  /* 71 */ vAddConstant("SIZEOF_WORD",              sizeof(Word));

  /*  0 */ vAddConstant("SIZEOF_S32",               sizeof(s32));
  /*  1 */ vAddConstant("SIZEOF_S256",              sizeof(s256));
  /*  2 */ vAddConstant("SIZEOF_S16K",              sizeof(s8K));

  /*  0 */ vAddConstant("TS_NONE",                  TS_NONE);
  /*  1 */ vAddConstant("TS_RUNNING",               TS_RUNNING);
  /*  2 */ vAddConstant("TS_SWITCH",                TS_SWITCH);
  /*  3 */ vAddConstant("TS_WAITING",               TS_WAITING);
  /*  5 */ vAddConstant("TS_SLEEPING",              TS_SLEEPING);
  /*  6 */ vAddConstant("TS_BLOCKED",               TS_BLOCKED);
  /*  7 */ vAddConstant("TS_EXITED",                TS_EXITED);
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
  
  /*  0 */ vAddConstant("CLOCK_FREQUENCY",          System::iClockFrequency());
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
  vAddPrimitive("+",               &vAdd,              false);
  vAddPrimitive("allot",           &vAllot,            false);
  vAddPrimitive("again",           &vAgain,            true );
  vAddPrimitive("again_",          &vAgain_,           false);
  vAddPrimitive("and",             &vAnd,              false);
  vAddPrimitive("begin",           &vBegin,            true );
  vAddPrimitive("begin_",          &vBegin_,           false);
  vAddPrimitive("boot",            &vBoot,             false);
  vAddPrimitive("bpa",             &vBpa,              false);
  vAddPrimitive("bpdump",          &vBpdump,           false);
  vAddPrimitive("class@",          &vClassFetch,       false);
  vAddPrimitive("class!",          &vClassStore,       false);
  vAddPrimitive("code=",           &vCodeAssign,       false);
  vAddPrimitive(":",               &vColon,            false);
  vAddPrimitive(",",               &vComma,            false);
  vAddPrimitive("(",               &vComment,          true );
  vAddPrimitive("compiling",       &vCompiling,        false);
  vAddPrimitive("create",          &vCreate,           false);
  vAddPrimitive(">create",         &vCreateFrom,       false);
  vAddPrimitive("cursor@",         &vCursorFetch,      false);
  vAddPrimitive("cursor!",         &vCursorStore,      false);
  vAddPrimitive("depth",           &vDepth,            false);
  vAddPrimitive("/",               &vDivide,           false);
  vAddPrimitive("do",              &vDo,               true );
  vAddPrimitive("do_rt",           &vDo_RT,            false);
  vAddPrimitive("does>",           &vDoes,             true );
  vAddPrimitive("drop",            &vDrop,             false);
  vAddPrimitive("dup",             &vDup,              false);
  vAddPrimitive("dump",            &vDump,             false);
  vAddPrimitive("else",            &vElse,             true );
  vAddPrimitive("encodeCall",      &vEncodeCall,       false);
  vAddPrimitive("encodeDoes",      &vEncodeDoes,       false);
  vAddPrimitive("encodeEmb",       &vEncodeEmb,        true );
  vAddPrimitive("encodeFun",       &vEncodeFun,        false);
  vAddPrimitive("encodeVal",       &vEncodeVal,        false);
  vAddPrimitive("env@",            &vEnvGet,           false);
  vAddPrimitive("enter",           &vEnter,            false);
  vAddPrimitive("]",               &vEnterCompiler,    false);
  vAddPrimitive("[",               &vEnterInterpreter, true );
  vAddPrimitive("==",              &vEqual,            false);
  vAddPrimitive("eql",             &vEqualAddress,     false);
  vAddPrimitive("execute",         &vExecute,          false);
  vAddPrimitive("exit",            &vExit,             false);
  vAddPrimitive("@",               &vFetch,            false);
  vAddPrimitive("find",            &vFind,             false);
  vAddPrimitive("freePercent",     &vFreePercent,      false);
  vAddPrimitive("freeSpace",       &vFreeSpace,        false);
  vAddPrimitive(">",               &vGreater,          false);
  vAddPrimitive(">=",              &vGreaterEqual,     false);
  vAddPrimitive("hash",            &vHash,             false);
  vAddPrimitive("hazard",          &vHazard,           false);
  vAddPrimitive("i",               &vI,                false);
  vAddPrimitive("if",              &vIf,               true );
  vAddPrimitive("immediate",       &vImmediate,        true );
  vAddPrimitive("interpreting",    &vInterpreting,     false);
  vAddPrimitive("inUseSpace",      &vInUseSpace,       false);
  vAddPrimitive("isBooted",        &vIsBooted,         false);
  vAddPrimitive("isCompiling",     &vIsCompiling,      false);
  vAddPrimitive("isImmediate",     &vIsImmediate,      false);
  vAddPrimitive("isInterpreting",  &vIsInterpreting,   false);
  vAddPrimitive("isPrimitive",     &vIsPrimitive,      false);
  vAddPrimitive("iterate",         &vIterate,          true );
  vAddPrimitive("iterate_rt_A",    &vIterate_RT_A,     false);
  vAddPrimitive("iterate_rt_B",    &vIterate_RT_B,     false);
  vAddPrimitive("j",               &vJ,                false);
  vAddPrimitive("jump",            &vJump,             false);
  vAddPrimitive("jumpFalse",       &vJumpFalse,        false);
  vAddPrimitive("jumpTrue",        &vJumpTrue,         false);
  vAddPrimitive("<",               &vLess,             false);
  vAddPrimitive("<=",              &vLessEqual,        false);
  vAddPrimitive("locate",          &vLocate,           false);
  vAddPrimitive("loop",            &vLoop,             true );
  vAddPrimitive("loop_rt",         &vLoop_RT,          false);
  vAddPrimitive("+loop",           &vLoopPlus,         true );
  vAddPrimitive("+loop_rt",        &vLoopPlus_RT,      false);
  vAddPrimitive("magic@",          &vMagicFetch,       false);
  vAddPrimitive("magic!!",         &vMagicMark,        false);
  vAddPrimitive("%",               &vModulo,           false);
  vAddPrimitive("*",               &vMultiply,         false);
  vAddPrimitive("nameWord",        &vNameWord,         false);
  vAddPrimitive("neg",             &vNeg,              false);
  vAddPrimitive("next",            &vNext,             true );
  vAddPrimitive("nop",             &vNop,              false);
  vAddPrimitive("not",             &vNot,              false);
  vAddPrimitive("!=",              &vNotEqual,         false);
  vAddPrimitive("object@",         &vObjectFetch,      false);
  vAddPrimitive("object!!",        &vObjectMark,       false);
  vAddPrimitive("object!",         &vObjectStore,      false);
  vAddPrimitive("or",              &vOr,               false);
  vAddPrimitive("pick",            &vPick,             false);
  vAddPrimitive("prevWord",        &vPrevWord,         false);
  vAddPrimitive(".",               &vPrint,            false);
  vAddPrimitive("push",            &vPush,             false);
  vAddPrimitive(".put",            &vPut,              false);
  vAddPrimitive(".\\n",            &vPutNewline,       false);
  vAddPrimitive(".putVerbose",     &vPutVerbose,       false);
  vAddPrimitive("rdtscp",          &vRdtscp,           false);
  vAddPrimitive("repeat",          &vRepeat,           true );
  vAddPrimitive("retryPoint",      &vRetryPoint,       false);
  vAddPrimitive("rot",             &vRot,              false);
  vAddPrimitive("see",             &vSee,              false);
  vAddPrimitive(";",               &vSemicolon,        true );
  vAddPrimitive("signal",          &vSignal,           false);
  vAddPrimitive("sleep",           &vSleep,            false);
  vAddPrimitive("snooze",          &vSnooze,           false);
  vAddPrimitive("!",               &vStore,            false);
  vAddPrimitive("-",               &vSubtract,         false);
  vAddPrimitive("swap",            &vSwap,             false);
  vAddPrimitive("switch",          &vSwitch,           false);
  vAddPrimitive("test",            &vTest,             false);
  vAddPrimitive("then",            &vThen,             true );
  vAddPrimitive("throw",           &vThrow,            false);
  vAddPrimitive("totalSpace",      &vTotalSpace,       false);
  vAddPrimitive("'",               &vTick,             true );
  vAddPrimitive(">body",           &vToBody,           false);
  vAddPrimitive(">cmd",            &vToCmd,            false);
  vAddPrimitive(">digitName",      &vToDigitName,      false);
  vAddPrimitive(">int",            &vToInt,            false);
  vAddPrimitive("r>",              &vToPstack,         false);
  vAddPrimitive(">r",              &vToRstack,         false);
  vAddPrimitive(">seitf",          &vToSEitf,          false);
  vAddPrimitive(">str",            &vToStr,            false);
  vAddPrimitive(">task",           &vToTask,           false);
  vAddPrimitive("valset>",         &vToValues,         false);
  vAddPrimitive(">valset",         &vToValSet,         false);
  vAddPrimitive("traceOff",        &vTraceOff,         false);
  vAddPrimitive("traceOn",         &vTraceOn,          false);
  vAddPrimitive("unloop",          &vUnloop,           false);
  vAddPrimitive("until",           &vUntil,            true );
  vAddPrimitive("version",         &vVersion,          false);
  vAddPrimitive("wait",            &vWait,             false);
  vAddPrimitive("wake",            &vWake,             false);
  vAddPrimitive("while",           &vWhile,            true );
  vAddPrimitive("0<",              &vZeroLess,         false);
  vAddPrimitive("0<=",             &vZeroLessEqual,    false);
  vAddPrimitive("0==",             &vZeroEqual,        false);
  vAddPrimitive("0!=",             &vZeroNotEqual,     false);
  vAddPrimitive("0>=",             &vZeroGreaterEqual, false);
  vAddPrimitive("0>",              &vZeroGreater,      false);
  
  //----------------------------------------------------------
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
  
  vAddPrimitive("args.partition",  &vArgsPartition, false);
  vAddPrimitive("args.print",      &vArgsPrint,     false);
  
  vAddPrimitive("csv.firstValue",  &vCsvFirstValue,    false);
  
  vAddPrimitive("debug.trap",      &vDebugTrap,        false);
  
  vAddPrimitive("dic.count",       &vDicCount,         false);
  vAddPrimitive("dic.drop",        &vDicDrop,          false);
  vAddPrimitive("}}",              &vDicDrop,          false);
  vAddPrimitive("dic.frozen@",     &vDicFrozenGet,     false);
  vAddPrimitive("dic.frozen!",     &vDicFrozenSet,     false);
  vAddPrimitive("dic.init",        &vDicInit,          false);
  vAddPrimitive("dic.last",        &vDicLast,          false);
  vAddPrimitive("dic.main",        &vDicMain,          false);
  vAddPrimitive("dic.name",        &vDicName,          false);
  vAddPrimitive("dic.pick",        &vDicPick,          false);
  vAddPrimitive("dic.push",        &vDicPush,          false);
  vAddPrimitive("{{",              &vDicPush,          false);
  vAddPrimitive("dic.stack",       &vDicStack,         false);
  
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
  vAddPrimitive("dir.new",         &vDirNew,          false);
  vAddPrimitive("dir.open",        &vDirOpen,          false);
  vAddPrimitive("dir.path@",       &vDirPathGet,       false);
  vAddPrimitive("dir.path!",       &vDirPathSet,       false);
  vAddPrimitive("dir.read",        &vDirRead,          false);
  vAddPrimitive("dir.rewind",      &vDirRewind,        false);
  vAddPrimitive("dir.seek",        &vDirSeek,          false);
  vAddPrimitive("dir.tell",        &vDirTell,          false);

  vAddPrimitive("file.close",      &vFileClose,        false);
  vAddPrimitive("file.delete",     &vFileDelete,       false);
  vAddPrimitive("file.init",       &vFileInit,         false);
  vAddPrimitive("file.new",        &vFileNew,          false);
  vAddPrimitive("file.open",       &vFileOpen,         false);
  vAddPrimitive("file.path@",      &vFilePathGet,      false);
  vAddPrimitive("file.path!",      &vFilePathSet,      false);
  vAddPrimitive("file.read",       &vFileRead,         false);
  vAddPrimitive("file.write",      &vFileWrite,        false);
  
  vAddPrimitive("grid.appendNode", &vGridAppendNode,   false);
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
  
  vAddPrimitive("loader.new",         &vLoaderNew,         false);
  vAddPrimitive("loader.delete",      &vLoaderDelete,      false);
  vAddPrimitive("loader.init",        &vLoaderInit,        false);
  vAddPrimitive("loader.table!",      &vLoaderTableStore,  false);
  vAddPrimitive("loader.path!",       &vLoaderPathStore,   false);
  vAddPrimitive("loader.txref!",      &vLoaderTxRefStore,  false);
  vAddPrimitive("loader.openFile",    &vLoaderOpenFile,    false);
  vAddPrimitive("loader.begin",       &vLoaderBegin,       false);
  vAddPrimitive("loader.fetchRecord", &vLoaderFetchRecord, false);
  vAddPrimitive("loader.pushRecord",  &vLoaderPushRecord,  false);
  vAddPrimitive("loader.insertRow",   &vLoaderInsertRow,   false);
  vAddPrimitive("loader.end",         &vLoaderEnd,         false);
  vAddPrimitive("loader.closeFile",   &vLoaderCloseFile,   false);

  vAddPrimitive("lock.dropRead",   &vLockDropRead,     false);
  vAddPrimitive("lock.dropWrite",  &vLockDropWrite,    false);
  vAddPrimitive("lock.grabRead",   &vLockGrabRead,     false);
  vAddPrimitive("lock.grabWrite",  &vLockGrabWrite,    false);
  vAddPrimitive("lock.init",       &vLockInit,         false);
  
  vAddPrimitive("memory.byteDump", &vMemoryByteDump,   false);
  vAddPrimitive("memory.wordDump", &vMemoryWordDump,   false);
  
  vAddPrimitive("node.init",       &vNodeInit,         false);
  vAddPrimitive("node.new",        &vNodeNew,          false);
  vAddPrimitive("node.no",         &vNodeNo,           false);
  
  vAddPrimitive("nodeCursor.begin",  &vNodeCursorBegin,  false);
  vAddPrimitive("nodeCursor.delete", &vNodeCursorDelete, false);
  vAddPrimitive("nodeCursor.first",  &vNodeCursorFirst,  false);
  vAddPrimitive("nodeCursor.get",    &vNodeCursorGet,    false);
  vAddPrimitive("nodeCursor.init",   &vNodeCursorInit,   false);
  vAddPrimitive("nodeCursor.new",    &vNodeCursorNew,    false);
  vAddPrimitive("nodeCursor.next",   &vNodeCursorNext,   false);
  
  vAddPrimitive("process.getPid",  &vProcessGetPid,    false);
  vAddPrimitive("process.testPid", &vProcessTestPid,   false);
  vAddPrimitive("process.quit",    &vProcessQuit,      false);
  
  vAddPrimitive("pstack.class",    &vPstackClass,      false);
  vAddPrimitive("p.depth",         &vPstackDepth,      false);
  
  vAddPrimitive("queue.append",    &vQueueAppend,      false);
  vAddPrimitive("queue.dump",      &vQueueDump,        false);
  vAddPrimitive("queue.init",      &vQueueInit,        false);
  vAddPrimitive("queue.pop",       &vQueuePop,         false);
  
  vAddPrimitive("rowCursor.begin",  &vRowCursorBegin,  false);
  vAddPrimitive("rowCursor.delete", &vRowCursorDelete, false);
  vAddPrimitive("rowCursor.end",    &vRowCursorEnd,    false);
  vAddPrimitive("rowCursor.first",  &vRowCursorFirst,  false);
  vAddPrimitive("rowCursor.get",    &vRowCursorGet,    false);
  vAddPrimitive("rowCursor.init",   &vRowCursorInit,   false);
  vAddPrimitive("rowCursor.new",    &vRowCursorNew,    false);
  vAddPrimitive("rowCursor.next",   &vRowCursorNext,   false);
  vAddPrimitive("rowCursor.table@", &vRowCursorTableFetch, false);
  vAddPrimitive("rowCursor.table!", &vRowCursorTableStore, false);
  
  vAddPrimitive("rstack.depth",    &vRstackDepth,      false);
  vAddPrimitive("rdrop",           &vRstackDrop,       false);
  vAddPrimitive("r@",              &vRstackFetch,      false);
  vAddPrimitive("rpick",           &vRstackPick,       false);
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
  
  vAddPrimitive("seitf.delete",    &vSEitfDelete,      false);
  vAddPrimitive("seitf.dump",      &vSEitfDump,        false);
  vAddPrimitive("seitf.new",       &vSEitfNew,         false);
  vAddPrimitive("seitf.req@",      &vSEitfReqFetch,    false);
  vAddPrimitive("seitf.req!",      &vSEitfReqStore,    false);
  vAddPrimitive("seitf.rsp@",      &vSEitfRspFetch,    false);
  vAddPrimitive("seitf.rsp!",      &vSEitfRspStore,    false);
  
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

  vAddPrimitive("stack.class",  &vStackClass,  false);

  vAddPrimitive("steps.list",  &vStepsList,  false);
  vAddPrimitive("steps.reset", &vStepsReset, false);
  
  vAddPrimitive("strpad", &vStringPad, false);
  vAddPrimitive("streql", &vStrEql,    false);
  vAddPrimitive("strlen", &vStrLen,    false);
  vAddPrimitive("subStr", &vSubStr,    false);
  vAddPrimitive("concat", &vConcat,    false);
  vAddPrimitive("clip",   &vClip,      false);
  vAddPrimitive("toDigitName",&vToDigitName, false);
  
  vAddPrimitive("s256.init", &vS256Init, false);
 
  vAddPrimitive("table.dic",         &vTableDic,         false);
  vAddPrimitive("table.drop",        &vTableDrop,        false);
  vAddPrimitive("table.init",        &vTableInit,        false);
  vAddPrimitive("table.insert",      &vTableInsert,      false);
  vAddPrimitive("table.space",       &vTableSpace,       false);
  vAddPrimitive("table.truncate",    &vTableTruncate,    false);
  
  vAddPrimitive("task.dic",        &vTaskDic,          false);
  vAddPrimitive("task.dump",       &vTaskDump,         false);
  vAddPrimitive("task.init",       &vTaskInit,         false);
  vAddPrimitive("task.seitf@",     &vTaskSeitfFetch,   false);
  vAddPrimitive("task.seitf!",     &vTaskSeitfStore,   false);
  vAddPrimitive("task.name@",      &vTaskNameFetch,    false);
  vAddPrimitive("task.name!",      &vTaskNameStore,    false);
  vAddPrimitive("task.new",        &vTaskNew,          false);
  vAddPrimitive("task.queue@",     &vTaskQfetch,       false);
  vAddPrimitive("task.queue!",     &vTaskQstore,       false);
  vAddPrimitive("task.seed",       &vTaskSeed,         false);
  vAddPrimitive("task.state@",     &vTaskStateFetch,   false);
  vAddPrimitive("task.state!",     &vTaskStateStore,   false);
  vAddPrimitive("task.this",       &vTaskThis,         false);
  
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
  
  vAddPrimitive("thread.append",   &vThreadAppend,     false);
  vAddPrimitive("thread.bind",     &vThreadBind,       false);
  vAddPrimitive("thread.cpu",      &vThreadCpu,        false);
  vAddPrimitive("thread.dump",     &vThreadDump,       false);
  vAddPrimitive("thread.init",     &vThreadInit,       false);
  vAddPrimitive("thread.id",       &vThreadId,         false);
  vAddPrimitive("thread.name@",    &vThreadNameFetch,  false);
  vAddPrimitive("thread.name!",    &vThreadNameStore,  false);
  vAddPrimitive("thread.taskset@", &vThreadSfetch,     false);
  vAddPrimitive("thread.taskset!", &vThreadSstore,     false);
  vAddPrimitive("thread.sleep_s",  &vThreadSleep_s,    false);
  vAddPrimitive("thread.sleep_ms", &vThreadSleep_ms,   false);
  vAddPrimitive("thread.sleep_us", &vThreadSleep_us,   false);
  vAddPrimitive("thread.sleep_ns", &vThreadSleep_ns,   false);
  vAddPrimitive("thread.spawn",    &vThreadSpawn,      false);
  
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
  
  //----------------------------------------------------------
  vAddPrimitive("socketAccept",    &vSocketAccept,     false);
  vAddPrimitive("socketClose",     &vSocketClose,      false);
  vAddPrimitive("socketConnect",   &vSocketConnect,    false);
  vAddPrimitive("socketListen",    &vSocketListen,     false);
  vAddPrimitive("socketOpen",      &vSocketOpen,       false);
  vAddPrimitive("socketRecv",      &vSocketRecv,       false);
  vAddPrimitive("socketSend",      &vSocketSend,       false);
  
  vAddPrimitive("client",          &vClient,           false);
  vAddPrimitive("server",          &vServer,           false);
  
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
  "  gwd .putVerbose"
  "  \"../../../../Commands\" cwd"
  "  gwd .putVerbose"
//  "  traceon"
  "  \"compile.sen\" >>:"
  "  \"::dicCompile exit\" .putVerbose"
  "  ::exit"
  "  ;";
#else
  sDefinition =
  ": ::dicCompile"
  "  \"::dicCompile\" ."
  "  gwd ."
  "  \"../../Commands\" cwd"
  "  gwd ."
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
  pFUN  pFunc;
  Word* pFoundWord;
  ValWsp xValWsp;
  
  m_xTokeniser.vUseString(sDefinition);
  
  // get :
  vTokenNext();
  //vTrace("CompileColon: %s\n",m_sToken.a());
  
  // get name of word
  vTokenNext();
  //vTrace("CompileColon: %s\n",m_sToken.a());
  
  pNewWord = new(m_xDictionaryStack.pTop()) Word(m_sToken.a());
  
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
          pFunc = pFoundWord->pFunc();
          if (pFoundWord->bImmediate())
            (*pFunc)(this);
          else {
            pNewWord->vAppend(pFoundWord->pCodeField());
          }
        }
        else if (WT_ROUTINE == pFoundWord->eWordType()) {
          pNewWord->vEncode(pFoundWord->pCodeField());
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
/* current word                                                               */
/*----------------------------------------------------------------------------*/
Word* Task::pCurrentWord(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Dictionary* pDictionary;
  
  if (m_pSEitf)
    pDictionary = m_pSEitf->pDicStack()->pTop();
  else
    pDictionary = m_xDictionaryStack.pTop();

  Word* pWord = pDictionary->pCurrentWord();
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* debug                                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDebug(void)
{
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_TASK);
  ASSERT(pObject->eClass() == CN_TASK);
  
  Task* pTask = reinterpret_cast<Task*>(pObject);
  
  vTrace("%sTask %p: \n", pTab, pTask);
  vTrace("%s  name: %s \n", pTab, pTask->m_sName.a());
  vTrace("%s  taskNo %lld\n", pTab, pTask->m_iTaskNo);
  vTrace("%s  state %s\n", pTab, pTS[pTask->m_eState]);
  vTrace("%s  txId %lld \n\n", pTab, pTask->m_iTxId);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vDump(void)
{
  _poolAddress_(this);
  
  vTrace("Task %p: \n", this);
  vTrace("   name: %16s \n", m_sName.a());
  vTrace("   taskNo %02lld\n" ,m_iTaskNo);
  vTrace("   state %s\n", pTS[m_eState]);
  vTrace("   txId %lld \n", m_iTxId);
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
  pWord = m_xDictionaryStack.pFind(sName.a());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* find word given name                                                       */
/*----------------------------------------------------------------------------*/
Word* Task::pFindWord(const char* aName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pWord = m_xDictionaryStack.pFind(aName);
  
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
    bResult = m_xDictionaryStack.bForget(sName);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* guard parameter stack                                                      */
/*----------------------------------------------------------------------------*/
bool Task::bGuardParam(i8 iCount)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_xParamStack.iValues() < iCount)
    return false;
  
  return true;
}



/*----------------------------------------------------------------------------*/
/* guard parameter stack 1                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bGuardParam(eCN eClass0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_xParamStack.iValues() < 1)
    return false;
  
  Value* pValue0 = m_xParamStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard parameter stack 2                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bGuardParam(eCN eClass0, eCN eClass1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_xParamStack.iValues() < 2)
    return false;
  
  Value* pValue0 = m_xParamStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  Value* pValue1 = m_xParamStack.pPeek(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard parameter stack 3                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bGuardParam(eCN eClass0, eCN eClass1, eCN eClass2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_xParamStack.iValues() < 3)
    return false;
  
  Value* pValue0 = m_xParamStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  Value* pValue1 = m_xParamStack.pPeek(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  Value* pValue2 = m_xParamStack.pPeek(2);
  if (eClass2 != pValue2->eClass())
    return false;
  
  return true;
}



/*----------------------------------------------------------------------------*/
/* guard return stack                                                         */
/*----------------------------------------------------------------------------*/
bool Task::bGuardReturn(i8 iCount)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_xReturnStack.iValues() < iCount)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard return stack 1                                                       */
/*----------------------------------------------------------------------------*/
bool Task::bGuardReturn(eCN eClass0)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_xReturnStack.iValues() < 1)
    return false;
  
  Value* pValue0 = m_xReturnStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard return stack 2                                                       */
/*----------------------------------------------------------------------------*/
bool Task::bGuardReturn(eCN eClass0, eCN eClass1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_xReturnStack.iValues() < 2)
    return false;
  
  Value* pValue0 = m_xReturnStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  Value* pValue1 = m_xReturnStack.pPeek(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard return stack 3                                                       */
/*----------------------------------------------------------------------------*/
bool Task::bGuardReturn(eCN eClass0, eCN eClass1, eCN eClass2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (m_xReturnStack.iValues() < 3)
    return false;
  
  Value* pValue0 = m_xReturnStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  Value* pValue1 = m_xReturnStack.pPeek(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  Value* pValue2 = m_xReturnStack.pPeek(2);
  if (eClass2 != pValue2->eClass())
    return false;
  
  return true;
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
/* locate word based on cell                                                  */
/*----------------------------------------------------------------------------*/
Word* Task::pLocateWord(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pWord = m_xDictionaryStack.pLocate(pCell);
  
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
/* seed task to run ::dicCompile                                              */
/*----------------------------------------------------------------------------*/
void Task::vSeedDicCompileSen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pBootSen = pFindWord("::dicCompile");
  ASSERT(pBootSen);
  
  m_pCell = pBootSen->pFirstCell();
  
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
  
  m_pCell = pShellEnter->pFirstCell();
  
  m_eState = TS_SWITCH;
  
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
  
  m_pCell = pShellEnter->pFirstCell();
  
  m_eState = TS_SWITCH;
  
  m_xParamStack.vReset();
  m_xReturnStack.vReset();
  
  m_sLambda = "";
  
  s32 sPrompt("docol> ");
  m_xTokeniser.vPrompt(sPrompt);
 
  m_xTokeniser.vUseString(m_sLambda);
  m_xTokeniser.vLineMode(false);
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
  
  if (!m_bTracing)
    return;
  
  i8 iParamValues  = m_xParamStack.iValues();
  i8 iReturnValues = m_xReturnStack.iValues();
  vTrace("%15s%15s : r %lld ; p%2d=[","",aPrimitive,iReturnValues,iParamValues);
  
  for (i8 iValue = iParamValues - 1; iValue >= 0; iValue--) {
    
    if (iValue != iParamValues-1)
      vTrace(",");
    
    ValWsp* pValWsp = static_cast<ValWsp*>(m_xParamStack.pPeek(iValue));
    
    eCN eClass = pValWsp->eClass();
    
    if (CN_INTVAL == eClass) {
      vTrace("%lld",pValWsp->iInt());
    }
    else if (CN_PTRVAL == eClass) {
      vTrace("%p (%s)",pValWsp->pPtr(),pValWsp->aClass());
    }
    else if (CN_STRVAL == eClass) {
      vTrace("\"%lls\"",pValWsp->sStr());
    }
    else {
      vTrace("unknown value on stack: class=%lld",pValWsp->eClass());
    }
  }
  
  vTrace("]\n");
}

/*----------------------------------------------------------------------------*/
/* reset task                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pCell    = m_pResetIP;
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
  
  m_pCell    = m_pResetIP;
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
  
  if ( ! bPopParam(pThreadEmb) ) {
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
    
  vPushReturnVoid(m_pCell);
    
  BodyField* pBody = pWord->pBodyField();
    
  m_pCell = pBody->m_pCell;
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
/* get next input token                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTokenNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_eToken = m_xTokeniser.eNextToken(m_sToken.a());
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
  
  while (TS_RUNNING == m_eState) {
    
    pFUN pFunc = m_pCell->m_pCodeField->m_pFunc;
    
    //vTrace("pFunc %p\n",pFunc);
    
    m_pCell++;
    
    try {
      (pFunc)(this);
    }
    
    catch (i8 iException) {
      char wFileName[256];
      
      const char* ps;
      
      ps = String::sLast(m_sFile.a());
      String::bBackward(&ps,m_sFile.a(),'/');
      String::vCopy(wFileName,ps+1);
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
/* pop cell from param stack                                                  */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Cell*& pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_PTRVAL) )
    return false;
  
  PtrVal xPtrIP;
  
  m_xParamStack.vPop(&xPtrIP);
  
  pCell = reinterpret_cast<Cell*>(xPtrIP.pPtr());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop eCN from param stack                                                   */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(eCN& iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  m_xParamStack.vPop(&xIntVal1);
  
  iArg1 = static_cast<eCN>(xIntVal1.iInt());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8 from param stack                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(i8& iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  m_xParamStack.vPop(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8 from param stack                                                 */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(i8& iArg1, i8& iArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_INTVAL,CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  m_xParamStack.vPop(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  IntVal xIntVal2;
  m_xParamStack.vPop(&xIntVal2);
  
  iArg2 = xIntVal2.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,val from param stack                                                */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(i8& iArg1, Object*& pArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_INTVAL,CN_PTRVAL) )
    return false;
  
  IntVal xIntVal1;
  m_xParamStack.vPop(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  PtrVal xPtrVal1;
  m_xParamStack.vPop(&xPtrVal1);
  
  pArg2 = xPtrVal1.pObject();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,val from param stack                                                */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(i8& iArg1, Value& xArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal1;
  m_xParamStack.vPop(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  m_xParamStack.vPop(&xArg2);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,s from param stack                                                  */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(i8& iArg1, s8K& sArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal1;
  m_xParamStack.vPop(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  ValWsp xValWsp2;
  m_xParamStack.vPop(&xValWsp2);
  
  sArg2 = xValWsp2.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8,val from param stack                                             */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(i8& iArg1, i8& iArg2, Value& xArg3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_INTVAL,CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal1;
  m_xParamStack.vPop(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  IntVal xIntVal2;
  m_xParamStack.vPop(&xIntVal2);
  
  iArg2 = xIntVal2.iInt();
  
  m_xParamStack.vPop(&xArg3);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8,val from param stack                                             */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(i8& iArg1, i8& iArg2, s8K& sArg3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_INTVAL,CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal1;
  m_xParamStack.vPop(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  IntVal xIntVal2;
  m_xParamStack.vPop(&xIntVal2);
  
  iArg2 = xIntVal2.iInt();
  
  
  ValWsp xValWsp3;
  m_xParamStack.vPop(&xValWsp3);
  
  sArg3 = xValWsp3.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object* from param stack                                               */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Object*& pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  m_xParamStack.vPop(&xPtrVal1);
  
  pObject = xPtrVal1.pObject();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object*, Object* from param stack                                      */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Object*& pObject1, Object*& pObject2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_PTRVAL,CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  m_xParamStack.vPop(&xPtrVal1);
  
  pObject1 = xPtrVal1.pObject();
  
  PtrVal xPtrVal2;
  m_xParamStack.vPop(&xPtrVal2);
  
  pObject2 = xPtrVal2.pObject();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object* from param stack                                               */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Object*& pObject, Value& xValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  m_xParamStack.vPop(&xPtrVal1);
  
  pObject = xPtrVal1.pObject();
  
  return bPopParam(xValue);
}

/*----------------------------------------------------------------------------*/
/* pop s from param stack                                                     */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(s32& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  m_xParamStack.vPop(&xValWsp1);
  
  sArg1 = xValWsp1.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from param stack                                                     */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(s256& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  m_xParamStack.vPop(&xValWsp1);
  
  sArg1 = xValWsp1.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from param stack                                                     */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(s8K& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  m_xParamStack.vPop(&xValWsp1);
  
  sArg1 = xValWsp1.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s,s from param stack                                                   */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(s8K& sArg1, s8K& sArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_STRVAL,CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  m_xParamStack.vPop(&xValWsp1);
  
  sArg1 = xValWsp1.sStr();
  
  ValWsp xValWsp2;
  m_xParamStack.vPop(&xValWsp2);
  
  sArg2 = xValWsp2.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop tx* from param stack                                                   */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Task*& pTask1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  PtrVal xPtrTask;
  
  if ( ! bGuardParam(CN_PTRVAL) )
    return false;
  
  m_xParamStack.vPop(&xPtrTask);

  pTask1 = reinterpret_cast<Task*>(xPtrTask.pPtr());
  if (CN_TASK != pTask1->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from param stack                                                 */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Value& xArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(1) )
    return false;
  
  m_xParamStack.vPop(&xArg1);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value, value from param stack                                          */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Value& pValue1, Value& pValue2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(2) )
    return false;
  
  m_xParamStack.vPop(&pValue1);
  
  m_xParamStack.vPop(&pValue2);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value, value, value from param stack                                   */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Value& pValue1, Value& pValue2, Value& pValue3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(3) )
    return false;
  
  m_xParamStack.vPop(&pValue1);
  
  m_xParamStack.vPop(&pValue2);

  m_xParamStack.vPop(&pValue3);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop word* from param stack                                                 */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Word*& pWord1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(CN_PTRVAL) )
    return false;
  
  PtrVal xPtrWord1;
  m_xParamStack.vPop(&xPtrWord1);
  
  pWord1 = reinterpret_cast<Word*>(xPtrWord1.pPtr());
  if (CN_WORD != pWord1->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from param stack as integer                                      */
/*----------------------------------------------------------------------------*/
bool Task::bPopParamInt(i8& iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardParam(1) )
    return false;
  
  ValWsp xWspVal1;
  m_xParamStack.vPop(&xWspVal1);
  
  iArg1 = xWspVal1.iAsInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push cell* onto param stack                                                */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(Cell* pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  PtrVal xPtrIP;
  
  xPtrIP = pCell;
  
  vPushParam(xPtrIP);
}

/*----------------------------------------------------------------------------*/
/* push char* onto param stack                                                */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(const char* aString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  ValWsp xStrVal1;
  xStrVal1 = aString;
  
  vPushParam(&xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push Object* onto param stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(Object* pObject1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  PtrVal xPtrObject;
  xPtrObject = pObject1;
  
  vPushParam(&xPtrObject);
}

/*----------------------------------------------------------------------------*/
/* push i8 onto param stack                                                   */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(i8 iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  IntVal xIntVal1;
  xIntVal1 = iArg1;
  
  vPushParam(&xIntVal1);
}

/*----------------------------------------------------------------------------*/
/* push s32 onto param stack                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(s32& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vPushParam(sArg1.a());
}

/*----------------------------------------------------------------------------*/
/* push s256 onto param stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(s256& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vPushParam(sArg1.a());
}

/*----------------------------------------------------------------------------*/
/* push s8K onto param stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(s8K& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vPushParam(sArg1.a());
}

/*----------------------------------------------------------------------------*/
/* push value onto parameter stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(Value& xVal1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_xParamStack.vPush(&xVal1);
}

/*----------------------------------------------------------------------------*/
/* push value onto parameter stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_xParamStack.vPush(pValue);
}

/*----------------------------------------------------------------------------*/
/* push tx* onto param stack                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(NNP_Task pnnArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  PtrVal xPtrTask;
  xPtrTask = *pnnArg1;
  
  vPushParam(&xPtrTask);
}

/*----------------------------------------------------------------------------*/
/* push word* onto param stack                                                */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(Word* pArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  PtrVal xPtrWord;
  xPtrWord = pArg1;
  
  vPushParam(&xPtrWord);
}

/*----------------------------------------------------------------------------*/
/* push i8 onto return stack                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPushReturn(i8 iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  IntVal xIntVal1;
  xIntVal1 = iArg1;
  
  m_xReturnStack.vPush(&xIntVal1);
}

/*----------------------------------------------------------------------------*/
/* push value onto return stack                                               */
/*----------------------------------------------------------------------------*/
void Task::vPushReturn(ValWsp& xValOpaque)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_xReturnStack.vPush(&xValOpaque);
}

/*----------------------------------------------------------------------------*/
/* push address onto return stack                                             */
/*----------------------------------------------------------------------------*/
void Task::vPushReturnVoid(void* pAddress)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  PtrVal xPtrAddress;
  xPtrAddress = pAddress;
  
  m_xReturnStack.vPush(&xPtrAddress);
}

/*----------------------------------------------------------------------------*/
/* pop cell from return stack                                                 */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(Cell*& pCell)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardReturn(CN_PTRVAL) )
    return false;
  
  PtrVal xPtrIP;
  
  m_xReturnStack.vPop(&xPtrIP);
  
  pCell = reinterpret_cast<Cell*>(xPtrIP.pPtr());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8 from return stack                                                   */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(i8& iArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardReturn(CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  m_xReturnStack.vPop(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8 from return stack                                                */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(i8& iArg1, i8& iArg2)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardReturn(CN_INTVAL,CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  m_xReturnStack.vPop(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  IntVal xIntVal2;
  m_xReturnStack.vPop(&xIntVal2);
  
  iArg2 = xIntVal2.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop val from return stack                                                  */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(ValWsp& xValOpaque)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardReturn(1) )
    return false;
  
  m_xReturnStack.vPop(&xValOpaque);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from return stack                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(s32& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardReturn(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  m_xReturnStack.vPop(&xValWsp1);
  
  sArg1 = xValWsp1.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from return stack                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(s256& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardReturn(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  m_xReturnStack.vPop(&xValWsp1);
  
  sArg1 = xValWsp1.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from return stack                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(s8K& sArg1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! bGuardReturn(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  m_xReturnStack.vPop(&xValWsp1);
  
  sArg1 = xValWsp1.sStr();
  
  return true;
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
  
  nnpTask->vPushParam(xWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* + -- add top two integers on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vAdd(NNP_Task nnpTask)
{
ENTRY("vAdd");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("+");
  
  r = i + j;
  
  nnpTask->vPushParam(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* again -- unconditionally repeat loop (use only in : docol)                 */
/*----------------------------------------------------------------------------*/
void Task::vAgain(NNP_Task nnpTask)
{
ENTRY("vAgain");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pBeginLoc;
  if ( ! nnpTask->bPopReturn(pBeginLoc) )
    THROW_INVALID_RETURN("again");
  
  pCurrentWord->vEncode("jump",pBeginLoc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* again_ -- unconditionally repeat begin_ loop (use only in : docol)         */
/*----------------------------------------------------------------------------*/
void Task::vAgain_(NNP_Task nnpTask)
{
ENTRY("vAgain_");
  
  nnpTask->m_pCell = nnpTask->m_pBeginIP;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* allot -- reserve n bytes in current word                                   */
/*----------------------------------------------------------------------------*/
void Task::vAllot(NNP_Task nnpTask)
{
ENTRY("vAllot");
  
  i8 iBytes;
  
  if ( !  nnpTask->bPopParam(iBytes) )
    THROW_INVALID_PARAM("allot");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();

  char* pAddress = pCurrentWord->pAllot(iBytes);
  ASSERT(pAddress);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* and -- logical and of top two values                                       */
/*----------------------------------------------------------------------------*/
void Task::vAnd(NNP_Task nnpTask)
{
ENTRY("vAnd");
  
  i8 iArgB;
  if ( ! nnpTask->bPopParamInt(iArgB) )
    THROW_INVALID_PARAM("and");
  
  i8 iArgA;
  if ( ! nnpTask->bPopParamInt(iArgA) )
    THROW_INVALID_PARAM("and");

  i8 iArgR = (iArgA && iArgB) ? 1 : 0;
  
  nnpTask->vPushParam(iArgR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* begin -- begin a loop (use only in : docol)                                */
/*----------------------------------------------------------------------------*/
void Task::vBegin(NNP_Task nnpTask)
{
ENTRY("vBegin");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pLastCell = pCurrentWord->pLastCell();
  
  nnpTask->vPushReturnVoid(pLastCell);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* begin_ -- begin a loop (use only in : docol)                               */
/*----------------------------------------------------------------------------*/
void Task::vBegin_(NNP_Task nnpTask)
{
ENTRY("vBegin_");
  
  nnpTask->m_pBeginIP = nnpTask->m_pCell;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* boot -- boot                                                               */
/*----------------------------------------------------------------------------*/
void Task::vBoot(NNP_Task nnpTask)
{
  static bool bHasRun = false;
  
ENTRY("vBoot");
  ASSERT( ! bHasRun );
  bHasRun = true;

  nnpTask->vAddConstants();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* bpa -- buffer pool analyze                                                 */
/*----------------------------------------------------------------------------*/
void Task::vBpa(NNP_Task nnpTask)
{
ENTRY("vBpa");
  
  BufferPoolAnalyzer* pBPA = new BufferPoolAnalyzer;
  
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
  
  BufferPoolAnalyzer* pBPA = new BufferPoolAnalyzer;
  
  pBPA->vDump();
  
  delete pBPA;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* classFetch -- fetch class number from Object object                        */
/*----------------------------------------------------------------------------*/
void Task::vClassFetch(NNP_Task nnpTask)
{
ENTRY("vClassFetch");
  
  Object* pObject;
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("classFetch");
  
  eCN eClassNo;
  eClassNo = pObject->eClass();
  
  nnpTask->vPushParam(eClassNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* classStore -- store class number into Object object                        */
/*----------------------------------------------------------------------------*/
void Task::vClassStore(NNP_Task nnpTask)
{
ENTRY("vClassStore");

  eCN eClassNo;
  if ( ! nnpTask->bPopParam(eClassNo) )
    THROW_INVALID_PARAM("class!");
  
  Object* pObject;
  if ( ! nnpTask->bPopParam(pObject) )
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
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("codeAssign");
  Word* pThatWord = _pWord(pObject);
  
  Word* pThisWord = nnpTask->pCurrentWord();
  
  ASSERT(pThatWord->pFunc() == pThatWord->pFunc());
  pThisWord->vFunc(pThatWord->pFunc());
  
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
  
  new(nnpTask->m_xDictionaryStack.pTop())  Word(nnpTask->m_sToken.a());
  
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
  if ( ! nnpTask->bPopParam(xValWsp) )
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
  
  nnpTask->vPushParam(xCursorWsp);
  
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
  
  if ( ! nnpTask->bPopParam(xCursorWsp) )
    THROW_INVALID_PARAM("cursor!");
  
  nnpTask->m_xReturnStack.vPoke(iPeek,&xCursorWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Task::vDepth(NNP_Task nnpTask)
{
ENTRY("vDepth");
  
  i8 iDepth = nnpTask->m_xParamStack.iValues();
  
  nnpTask->vPushParam(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* / -- divide top integer by second integer on stack                         */
/*----------------------------------------------------------------------------*/
void Task::vDivide(NNP_Task nnpTask)
{
ENTRY("vDivide");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("/");
    
  r = DIV(i,j);
  
  nnpTask->vPushParam(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* do -- compile time of do ... loop                                immediate */
/*----------------------------------------------------------------------------*/
void Task::vDo(NNP_Task nnpTask)
{
ENTRY("vDo");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("do_rt");
  
  Cell* pLastCell = pCurrentWord->pLastCell();
  
  nnpTask->vPushReturnVoid(pLastCell);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* do_rt -- runtime of do ... loop                                            */
/*----------------------------------------------------------------------------*/
void Task::vDo_RT(NNP_Task nnpTask)
{
ENTRY("vDo_RT");
  
  i8 iIndex;
  i8 iLimit;
  if ( ! nnpTask->bPopParam(iIndex,iLimit) )
    THROW_INVALID_PARAM("do_rt");
  
  nnpTask->vPushReturn(iLimit);
  nnpTask->vPushReturn(iIndex); // index is on top
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vDrop(NNP_Task nnpTask)
{
ENTRY("vDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  if ( ! nnpTask->bPopParam(xValA) )
    THROW_INVALID_PARAM("drop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of param stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vDup(NNP_Task nnpTask)
{
ENTRY("vDup");
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->bPopParam(xValA) )
    THROW_INVALID_PARAM("dup");
  
  nnpTask->vPushParam(xValA);
  nnpTask->vPushParam(xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dump -- dump top of param stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vDump(NNP_Task nnpTask)
{
ENTRY("vDump");
  
  s8K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->bPopParam(xValWsp) )
    THROW_INVALID_PARAM("dump");
  
  xValWsp.vToString(sWorkspace.a());
  char* aClass = xValWsp.aClass();
  
  eCN eClass = xValWsp.eClass();
  
  if (CN_PTRVAL == eClass) {
    eCN ePtdClass = xValWsp.pAsPtrVal()->pObject()->eClass();
    vTrace("[%s] %s (%s %s)\n",
          nnpTask->m_sName.a(),sWorkspace.a(),aClass,pCN[ePtdClass]);
  }
  else {
    vTrace("[%s] %s (%s)\n",
           nnpTask->m_sName.a(),sWorkspace.a(),aClass);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* else -- else clause                                                        */
/*----------------------------------------------------------------------------*/
void Task::vElse(NNP_Task nnpTask)
{
ENTRY("vElse");

  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pIfJumpArg;
  if ( ! nnpTask->bPopReturn(pIfJumpArg) )
    THROW_INVALID_RETURN("else");
  
  //-------------------------------------------------------------------------
  
  Cell* pElseJumpArg;
  
  pCurrentWord->vEncode("jump");
  pElseJumpArg = (Cell*)pCurrentWord->pAppend((i8)0);
  
  nnpTask->vPushReturnVoid(pElseJumpArg);
  
  //-------------------------------------------------------------------------
  
  pIfJumpArg->m_pJumpLocn = pCurrentWord->pLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeCall -- encode call                                                  */
/*----------------------------------------------------------------------------*/
void Task::vEncodeCall(NNP_Task nnpTask)
{
ENTRY("vEncodeCall");
  
  Word* pCalledWord;
  if ( ! nnpTask->bPopParam(pCalledWord) )
    THROW_INVALID_PARAM("encodeCall");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  CodeField* pCode = pCalledWord->pCodeField();
  
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
  
  pCurrentWord->vFunc(&Task::vEnter);
  pCurrentWord->vAddr(nnpTask->m_pCell+1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeEmb -- encode push ptr to Object object                              */
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
    nnpTask->vPushParam(&xPtrObject);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* encodeFun -- encode primitive fun                                          */
/*----------------------------------------------------------------------------*/
void Task::vEncodeFun(NNP_Task nnpTask)
{
ENTRY("vEncodeFun");
  
  Word* pCalledWord;
  if ( ! nnpTask->bPopParam(pCalledWord) )
    THROW_INVALID_PARAM("encodeFun");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  CodeField* pCode = pCalledWord->pCodeField();
  
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
  if ( ! nnpTask->bPopParam(xValA) )
    THROW_INVALID_PARAM("encodeVal");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("push",&xValA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* enter -- enter this routine                                                */
/*----------------------------------------------------------------------------*/
void Task::vEnter(NNP_Task nnpTask)
{
ENTRY("vEnter");
  
  if (nnpTask->m_bTracing)
    vDebug();
  
  nnpTask->vPushReturnVoid(nnpTask->m_pCell);
  
  Cell* pPrevCell = nnpTask->m_pCell - 1;
  CodeField* pCode = pPrevCell->m_pCodeField;
  
  //Cell* pCell = reinterpret_cast<Cell*>(pcAdd(pCode,2*sizeof(Cell*)));

  BodyField* pBody = reinterpret_cast<BodyField*>(pcAdd(pCode,1*sizeof(Cell*)));

  Word* pWord = reinterpret_cast<Word*>(pcAdd(pBody, -(sizeof(Word)-sizeof(BodyField)) ));
  if (WT_OBJECT == pWord->eWordType()) {
    Cell* pCell = pWord->pFirstCell();
    nnpTask->vPushParam(pCell);

    //nnpTask->vPushParam(pWord->pFirstCell());
  }

  //ASSERT(pBody->m_pCell == pCell);
  //nnpTask->m_pCell = pCell;
  
  nnpTask->m_pCell = pBody->m_pCell;
  
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
  
  if ( ! nnpTask->bPopParam(sEnvName) )
    THROW_INVALID_PARAM("env@");
  
  s8K sEnvVal;
  
  if (! Env::bGetEnv(sEnvName.a(),sEnvVal.a()))
    sEnvVal = "";
  
  nnpTask->vPushParam(sEnvVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* == -- test top two integers on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Task::vEqual(NNP_Task nnpTask)
{
ENTRY("vEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("==");
  
  r = i == j;
  
  nnpTask->vPushParam(r);
  
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
  
  if ( ! nnpTask->bPopParam(xAddressA,xAddressB) )
    THROW_INVALID_PARAM("eql");
  
  i8 iA = xAddressA.iAsInt();
  i8 iB = xAddressB.iAsInt();
  
  i8 r = iA == iB;
  
  nnpTask->vPushParam(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* execute -- execute word on param stack                                     */
/*----------------------------------------------------------------------------*/
void Task::vExecute(NNP_Task nnpTask)
{
ENTRY("vExecute");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("execute");
  
  if (WT_PRIMITIVE == pWord->eWordType()) {
    pFUN pFunc = pWord->pFunc();
    (pFunc)(nnpTask);
  }
  else if (WT_OBJECT == pWord->eWordType()) {
    nnpTask->vPushReturnVoid(nnpTask->m_pCell);

    Cell* pCell = pWord->pFirstCell();
    nnpTask->vPushParam(pCell);
    
    pFUN pFunc = pWord->pFunc();
    if (!pFunc || Task::vNop == pFunc) {
      if ( ! nnpTask->bPopReturn(nnpTask->m_pCell) )
        THROW_INVALID_RETURN("execute");
    }
    else {
      // does> inherited code
      BodyField* pBody = pWord->pBodyField();
      nnpTask->m_pCell = pBody->m_pCell;
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
  
  if ( ! nnpTask->bPopReturn(nnpTask->m_pCell) )
    THROW_INVALID_RETURN("exit");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* @ -- fetch value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vFetch(NNP_Task nnpTask)
{
  ENTRY("vFetch");
  
  Object* pObject;
  if ( ! nnpTask->bPopParam(pObject) )
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
  
  nnpTask->vPushParam(xValFetched);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* freePercent -- push free percent of buffer pool onto stack                 */
/*----------------------------------------------------------------------------*/
void Task::vFreePercent(NNP_Task nnpTask)
{
ENTRY("vFreePercent");
  
  i8 iFreePercent = gpBufferPool->iFreePercent();
  
  nnpTask->vPushParam(iFreePercent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* freeSpace -- push free space in buffer pool onto stack                     */
/*----------------------------------------------------------------------------*/
void Task::vFreeSpace(NNP_Task nnpTask)
{
ENTRY("vFreeSpace");
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  
  nnpTask->vPushParam(iFreeSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* > -- test top two integers on data stack for greater than                  */
/*----------------------------------------------------------------------------*/
void Task::vGreater(NNP_Task nnpTask)
{
ENTRY("vGreater");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM(">");
  
  r = i > j;
  
  nnpTask->vPushParam(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >= -- test top two integers on data stack for greater than equal to        */
/*----------------------------------------------------------------------------*/
void Task::vGreaterEqual(NNP_Task nnpTask)
{
ENTRY("vGreaterEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM(">=");
  
  r = i >= j;
  
  nnpTask->vPushParam(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* hash -- get hash of string                                                 */
/*----------------------------------------------------------------------------*/
void Task::vHash(NNP_Task nnpTask)
{
ENTRY("vHash");
  
  s8K sString;
  if ( ! nnpTask->bPopParam(sString) )
    THROW_INVALID_PARAM("hash");
  
  i8 iLength = String::iLength(sString.a());
  
  i8 iHash = XXH32(sString.a(),iLength,0);
  
  nnpTask->vPushParam(iHash);
  
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
  
  nnpTask->vPushParam(pI);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* if -- if clause                                                            */
/*----------------------------------------------------------------------------*/
void Task::vIf(NNP_Task nnpTask)
{
ENTRY("vIf");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pIfJumpArg;

  pCurrentWord->vEncode("jumpFalse");
  
  pIfJumpArg = (Cell*)pCurrentWord->pAppend((i8)0);
  
  nnpTask->vPushReturnVoid(pIfJumpArg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the most recent definition an immediate word             */
/*----------------------------------------------------------------------------*/
void Task::vImmediate(NNP_Task nnpTask)
{
ENTRY("vImmediate");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();

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
  
  nnpTask->vPushParam(bIsBooted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isCompiling -- currently in compiling mode?                                */
/*----------------------------------------------------------------------------*/
void Task::vIsCompiling(NNP_Task nnpTask)
{
ENTRY("vIsCompiling");
  
  i8 iIsCompiling = nnpTask->m_bCompiling ? 1 : 0;
  
  nnpTask->vPushParam(iIsCompiling);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Task::vIsImmediate(NNP_Task nnpTask)
{
ENTRY("vIsImmediate");
  
  // we should just modify the pointers rather than actually pop
  
  Word* pWord;
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("isImmediate");
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  nnpTask->vPushParam(iIsImmediate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isInterpreting -- currently in interpreting mode?                          */
/*----------------------------------------------------------------------------*/
void Task::vIsInterpreting(NNP_Task nnpTask)
{
ENTRY("vIsInterpreting");
  
  i8 iIsInterpreting = nnpTask->m_bCompiling ? 0 : 1;
  
  nnpTask->vPushParam(iIsInterpreting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* isPrimitive -- is word a primitive?                                        */
/*----------------------------------------------------------------------------*/
void Task::vIsPrimitive(NNP_Task nnpTask)
{
ENTRY("vIsPrimitive");
  
  Word* pWord;

  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("isPrimitive");
  
  i8 iIsPrimitive = WT_PRIMITIVE == pWord->eWordType() ? 1 : 0;
  
  nnpTask->vPushParam(iIsPrimitive);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* inUseSpace -- push in-use space in buffer pool onto stack                  */
/*----------------------------------------------------------------------------*/
void Task::vInUseSpace(NNP_Task nnpTask)
{
ENTRY("vInUseSpace");
  
  i8 iInUseSpace = gpBufferPool->iRegionSize() - gpBufferPool->iFreeSpace();
  
  nnpTask->vPushParam(iInUseSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* iterate -- compile time of iterate ... next                      immediate */
/*----------------------------------------------------------------------------*/
void Task::vIterate(NNP_Task nnpTask)
{
ENTRY("vIterate");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("iterate_rt_a");
  
  Cell* pIterateLoc = pCurrentWord->pLastCell();
  nnpTask->vPushReturnVoid(pIterateLoc);
  pCurrentWord->vEncode("iterate_rt_b");

  pCurrentWord->vEncode("jumpFalse");
  Cell* pJumpFalseArg = (Cell*)pCurrentWord->pAppend((i8)0);
  nnpTask->vPushReturnVoid(pJumpFalseArg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* iterate_rt_a -- runtime of iterate ... next                                */
/*----------------------------------------------------------------------------*/
void Task::vIterate_RT_A(NNP_Task nnpTask)
{
ENTRY("vIterate_RT_A");
  
  PtrVal xPtrObject;
  PtrVal xPtrCursor;
  
  if ( ! nnpTask->bPopParam(xPtrObject,xPtrCursor) )
    THROW_INVALID_PARAM("iterate_rt_a");
  
  nnpTask->vPushReturnVoid(xPtrCursor.pPtr());
  nnpTask->vPushReturnVoid(xPtrObject.pPtr());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* iterate_rt_b -- runtime of iterate ... next                                */
/*----------------------------------------------------------------------------*/
void Task::vIterate_RT_B(NNP_Task nnpTask)
{
ENTRY("vIterate_RT_B");
  
  i8 iPick = 0;
  
  ValWsp xObjectWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xObjectWsp);
  nnpTask->vPushParam(xObjectWsp);
  
  ValWsp xWsp;
  
  if ( ! xObjectWsp.iAsInt() ) {
    if ( !nnpTask->bPopReturn(xWsp) )
      THROW_INVALID_PARAM("iterate_rt_b");
    if ( !nnpTask->bPopReturn(xWsp) )
      THROW_INVALID_PARAM("iterate_rt_b");
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vJ(NNP_Task nnpTask)
{
ENTRY("vJ");
  
  Value* pI = nnpTask->m_xReturnStack.pPeek(3);
  
  nnpTask->vPushParam(pI);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jump -- jump to location                                                   */
/*----------------------------------------------------------------------------*/
void Task::vJump(NNP_Task nnpTask)
{
ENTRY("vJump");
  
  nnpTask->m_pCell = nnpTask->m_pCell->m_pJumpLocn;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpFalse -- jump on false to location                                     */
/*----------------------------------------------------------------------------*/
void Task::vJumpFalse(NNP_Task nnpTask)
{
ENTRY("vJumpFalse");
  
  i8 iVal;
  if ( ! nnpTask->bPopParamInt(iVal) )
    THROW_INVALID_PARAM("jumpFalse");
  
  // cell points to the jump arg
  if (iVal)
    nnpTask->m_pCell++;
  else
    nnpTask->m_pCell = nnpTask->m_pCell->m_pJumpLocn;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jumpTrue -- jump on true to location                                       */
/*----------------------------------------------------------------------------*/
void Task::vJumpTrue(NNP_Task nnpTask)
{
ENTRY("vJumpTrue");
  
  i8 iVal;
  if ( ! nnpTask->bPopParamInt(iVal) )
    THROW_INVALID_PARAM("jumpTrue");
  
  // cell points to the jump arg
  if (iVal)
    nnpTask->m_pCell = nnpTask->m_pCell->m_pJumpLocn;
  else
    nnpTask->m_pCell++;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* < -- test top two integers on data stack for less than                     */
/*----------------------------------------------------------------------------*/
void Task::vLess(NNP_Task nnpTask)
{
ENTRY("vLess");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("<");
  
  r = i < j;
  
  nnpTask->vPushParam(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* <= -- test top two integers on data stack for less than equal to           */
/*----------------------------------------------------------------------------*/
void Task::vLessEqual(NNP_Task nnpTask)
{
ENTRY("vLessEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("<=");
  
  r = i <= j;
  
  nnpTask->vPushParam(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* locate -- locate word based on ip                                          */
/*----------------------------------------------------------------------------*/
void Task::vLocate(NNP_Task nnpTask)
{
ENTRY("vLocate");
  
  Cell* pCell;
  
  if ( ! nnpTask->bPopParam(pCell) )
    THROW_INVALID_PARAM("locate");
  
  Word* pLocatedWord = nnpTask->pLocateWord(pCell);
  ASSERT(pLocatedWord);
  
  nnpTask->vPushParam(pLocatedWord->sName().a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loop -- compile time for do ... loop                            immediate  */
/*----------------------------------------------------------------------------*/
void Task::vLoop(NNP_Task nnpTask)
{
ENTRY("vLoop");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pDoLoc;
  if ( ! nnpTask->bPopReturn(pDoLoc) )
    THROW_INVALID_RETURN("loop");
  
  pCurrentWord->vEncode("Loop_RT");
  
  pCurrentWord->vEncode("jump",pDoLoc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loop_rt -- runtime for do loop                                             */
/*----------------------------------------------------------------------------*/
void Task::vLoop_RT(NNP_Task nnpTask)
{
ENTRY("vLoop_RT");
  
  i8 iIndex;
  i8 iLimit;
  
  if ( ! nnpTask->bPopReturn(iIndex,iLimit) )
    THROW_INVALID_PARAM("loop_rt");
  
  iIndex++;
  
  if (iIndex < iLimit) {
    nnpTask->vPushReturn(iLimit);
    nnpTask->vPushReturn(iIndex); // index is on top
  }
  else {
    nnpTask->m_pCell += 2;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loopPlus -- do +loop                                                       */
/*----------------------------------------------------------------------------*/
void Task::vLoopPlus(NNP_Task nnpTask)
{
ENTRY("vLoopPlus");

  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pDoLoc;
  if ( ! nnpTask->bPopReturn(pDoLoc) )
    THROW_INVALID_RETURN("+loop");
    
  pCurrentWord->vEncode("+Loop_RT");
  
  pCurrentWord->vEncode("jump",pDoLoc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loopPlus_rt -- runtime for do +loop                                        */
/*----------------------------------------------------------------------------*/
void Task::vLoopPlus_RT(NNP_Task nnpTask)
{
ENTRY("vLoopPlus_RT");
  
  i8 iIndex;
  i8 iLimit;
  i8 iInc;
  
  if ( ! nnpTask->bPopReturn(iIndex,iLimit) )
    THROW_INVALID_PARAM("loopPlus_rt");

  if ( ! nnpTask->bPopParam(iInc) )
    THROW_INVALID_PARAM("loopPlus_rt");
  
  iIndex += iInc;
  
  if (iIndex < iLimit) {
    nnpTask->vPushReturn(iLimit);
    nnpTask->vPushReturn(iIndex);
  }
  else {
    nnpTask->m_pCell += 2;
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
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_RETURN("magicFetch");
  
  i8 iMagic = pObject->iMagic();

  nnpTask->vPushParam(iMagic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* magicMark -- put magic number into Object object                           */
/*----------------------------------------------------------------------------*/
void Task::vMagicMark(NNP_Task nnpTask)
{
ENTRY("vMagicMark");
  
  Object* pObject;
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_RETURN("magicMark");
  
  pObject->vSetMagic();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* % -- modulo of top integer by second integer on stack                      */
/*----------------------------------------------------------------------------*/
void Task::vModulo(NNP_Task nnpTask)
{
ENTRY("vModulo");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("%");
  
  r = MOD(i,j);
  
  nnpTask->vPushParam(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* * multiply second integer by top integer on stack                          */
/*----------------------------------------------------------------------------*/
void Task::vMultiply(NNP_Task nnpTask)
{
ENTRY("vMultiply");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("*");
  
  r = i * j;
  
  nnpTask->vPushParam(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nameWord -- push name of word                                              */
/*----------------------------------------------------------------------------*/
void Task::vNameWord(NNP_Task nnpTask)
{
ENTRY("vNameWord");
  
  Word* pWord;

  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("nameWord");
  
  nnpTask->vPushParam(pWord->sName().a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* neg -- gegate top integer on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vNeg(NNP_Task nnpTask)
{
ENTRY("vNeg");
  
  i8 r, i;
  
  if ( ! nnpTask->bPopParam(i) )
    THROW_INVALID_PARAM("neg");
  
  r = -1 * i;
  
  nnpTask->vPushParam(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* next -- compile time for iterate ... next                        immediate */
/*----------------------------------------------------------------------------*/
void Task::vNext(NNP_Task nnpTask)
{
ENTRY("vNext");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pJumpFalseArg;
  if ( ! nnpTask->bPopReturn(pJumpFalseArg) )
    THROW_INVALID_RETURN("next");
  
  Cell* pIterateLoc;
  if ( ! nnpTask->bPopReturn(pIterateLoc) )
    THROW_INVALID_RETURN("next");
  
  //----------------------------------------------------------------------------
  
  pCurrentWord->vEncode("jump",pIterateLoc);
  
  pJumpFalseArg->m_pJumpLocn = pCurrentWord->pLastCell();
  
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
/* not -- not of top value                                                    */
/*----------------------------------------------------------------------------*/
void Task::vNot(NNP_Task nnpTask)
{
ENTRY("vNot");
  
  i8 iValA;
  if ( ! nnpTask->bPopParamInt(iValA) )
    THROW_INVALID_PARAM("not");
  
  i8 iNotA = iValA ? 0 : 1;
  
  nnpTask->vPushParam(iNotA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* != -- test top two integers on data stack for not equal                    */
/*----------------------------------------------------------------------------*/
void Task::vNotEqual(NNP_Task nnpTask)
{
ENTRY("vNotEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("!=");
  
  r = i != j;
  
  nnpTask->vPushParam(r);
  
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
  
  nnpTask->vPushParam(xObjectWsp);
  
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
  
  if ( ! nnpTask->bPopParam(xObjectWsp) )
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
  pCurrentWord->vFunc(&vEncodeEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* or -- logical or of top two values                                         */
/*----------------------------------------------------------------------------*/
void Task::vOr(NNP_Task nnpTask)
{
ENTRY("vOr");
  
  i8 iArgB;
  if ( ! nnpTask->bPopParamInt(iArgB) )
    THROW_INVALID_PARAM("or");
  
  i8 iArgA;
  if ( ! nnpTask->bPopParamInt(iArgA) )
    THROW_INVALID_PARAM("or");
  
  i8 iArgR = (iArgA || iArgB) ? 1 : 0;
  
  nnpTask->vPushParam(iArgR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* switch -- task switch                                                      */
/*----------------------------------------------------------------------------*/
void Task::vSwitch(NNP_Task nnpTask)
{
ENTRY("vSwitch");
  
  nnpTask->vState(TS_SWITCH);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pick -- place copy of nth value in stack onto stack                        */
/*----------------------------------------------------------------------------*/
void Task::vPick(NNP_Task nnpTask)
{
ENTRY("vPick");

  i8 iPick;
  if ( ! nnpTask->bPopParamInt(iPick) )
    THROW_INVALID_PARAM("pick");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xParamStack.vPeek(iPick,&xValWsp);

  nnpTask->vPushParam(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* prevWord -- push prev word                                                 */
/*----------------------------------------------------------------------------*/
void Task::vPrevWord(NNP_Task nnpTask)
{
ENTRY("vPrevWord");
  
  Word* pWord;
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("prevWord");
    
  Word* pPrevWord = pWord->pPrevWord();
  
  nnpTask->vPushParam(pPrevWord);
  
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
  if ( ! nnpTask->bPopParam(xValWsp) )
    THROW_INVALID_PARAM(".");
  
  xValWsp.vToString(sWorkspace.a());
  vTrace("[%p %s] %s\n",*nnpTask,nnpTask->m_sName.a(),sWorkspace.a());
  
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
  
  Value* pValue = nnpTask->m_pValue;
  
  nnpTask->vPushParam(pValue);
  
  nnpTask->vAdjustNext(pValue->iSize());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pushEmbedded -- push ptr to embedded object                                */
/*----------------------------------------------------------------------------*/
void Task::vPushEmbedded(NNP_Task nnpTask)
{
ENTRY("vPushEmbedded");
  
  PtrVal xPtrObject;
  xPtrObject = nnpTask->m_pValue;
  
  nnpTask->vPushParam(&xPtrObject);
  
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
  if ( ! nnpTask->bPopParam(xValWsp) )
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
  if ( ! nnpTask->bPopParam(xValWsp) )
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
  
  i8 iTscp = iRdtscp();

  nnpTask->vPushParam(iTscp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* repeat -- repeat a begin-while-repeat loop                       immediate */
/*----------------------------------------------------------------------------*/
void Task::vRepeat(NNP_Task nnpTask)
{
ENTRY("vRepeat");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();

  Cell* pWhileJumpArg;
  if ( ! nnpTask->bPopReturn(pWhileJumpArg) )
    THROW_INVALID_RETURN("repeat");
  
  //-------------------------------------------------------------------------

  Cell* pBeginLoc;
  if ( ! nnpTask->bPopReturn(pBeginLoc) )
    THROW_INVALID_RETURN("repeat");

  pCurrentWord->vEncode("jump",pBeginLoc);
  
  //-------------------------------------------------------------------------
  
  pWhileJumpArg->m_pJumpLocn = pCurrentWord->pLastCell();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* retryPoint -- mark retry point                                             */
/*----------------------------------------------------------------------------*/
void Task::vRetryPoint(NNP_Task nnpTask)
{
ENTRY("vRetryMark");
  
  nnpTask->m_pResetIP = nnpTask->m_pCell;
  
  nnpTask->m_xParamStack.vRetryPoint();
  nnpTask->m_xReturnStack.vRetryPoint();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top 3 values ( C B A -- A C B )                              */
/*----------------------------------------------------------------------------*/
void Task::vRot(NNP_Task nnpTask)
{
ENTRY("vRot");
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->bPopParam(xValA,xValB,xValC) )
    THROW_INVALID_PARAM("rot");
  
  nnpTask->vPushParam(&xValA);
  nnpTask->vPushParam(&xValC);
  nnpTask->vPushParam(&xValB);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rstack.depth -- put number of values in return stack onto param stack      */
/*----------------------------------------------------------------------------*/
void Task::vRstackDepth(NNP_Task nnpTask)
{
ENTRY("vRstackDepth");
  
  i8 iDepth = nnpTask->m_xReturnStack.iValues();
  
  nnpTask->vPushParam(iDepth);
  
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
  if ( ! nnpTask->bPopReturn(xValA) )
    THROW_INVALID_RETURN("rdrop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r@ -- place copy of nth value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstackFetch(NNP_Task nnpTask)
{
ENTRY("vRstackFetch");
  
  i8 iPick;
  if ( ! nnpTask->bPopParamInt(iPick) )
    THROW_INVALID_PARAM("r@");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xValWsp);
  
  nnpTask->vPushParam(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rpick -- place copy of nth value in return stack onto param stack          */
/*----------------------------------------------------------------------------*/
void Task::vRstackPick(NNP_Task nnpTask)
{
ENTRY("vRstackPick");
  
  i8 iPick;
  if ( ! nnpTask->bPopParamInt(iPick) )
    THROW_INVALID_PARAM("rpick");
    
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xValWsp);
  
  nnpTask->vPushParam(xValWsp);
  
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
  
  nnpTask->vPushParam(xValWsp);
  
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
  
  nnpTask->vPushParam(xValWsp);
  
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
  
  nnpTask->vPushParam(xValWsp);
  
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
  
  nnpTask->vPushParam(xValWsp);
  
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
  
  nnpTask->vPushParam(xValWsp);
  
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
  
  nnpTask->vPushParam(xValWsp);
  
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
  
  nnpTask->vPushParam(xValWsp);
  
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
  
  nnpTask->vPushParam(xValWsp);
  
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
  
  nnpTask->vPushParam(xValWsp);
  
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
  
  nnpTask->vPushParam(xValWsp);
  
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
  
  
  BodyField* pBody = pWord->pBodyField();
  Cell* pCell = pBody->m_pCell;
  
  s8K sWorkspace;
  
  while ((char*)pCell < pcAdd(pWord->pBodyArea(),pWord->iDataBytes())) {
 
    Word* pFuncWord = nnpTask->m_xDictionaryStack.pFindFunc(pCell->m_pCodeField->m_pFunc);

    vTrace("%p: %s",pCell,pFuncWord->sName().a());
  
    if (pCell->m_pCodeField->m_pFunc == Task::vJump) {
      pCell++;
      vTrace(" %p\n",pCell->m_pJumpLocn);
      pCell++;
    }
    else
    if (pCell->m_pCodeField->m_pFunc == Task::vJumpFalse) {
      pCell++;
      vTrace(" %p\n",pCell->m_pJumpLocn);
      pCell++;
    }
    else
    if (pCell->m_pCodeField->m_pFunc == Task::vJumpTrue) {
      pCell++;
      vTrace(" %p\n",pCell->m_pJumpLocn);
      pCell++;
    }
    else
    if (pCell->m_pCodeField->m_pFunc == Task::vPush) {
      pCell++;
      //Value* pValue = reinterpret_cast<Value*>(pCell);
      //pValue->vToString(sWorkspace.a());
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
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* signal -- signal tx on stack                                               */
/*----------------------------------------------------------------------------*/
void Task::vSignal(NNP_Task nnpTask)
{
ENTRY("vSignal");
  
  // ..tbd..
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* sleep -- sleep for n seconds                                               */
/*----------------------------------------------------------------------------*/
void Task::vSleep(NNP_Task nnpTask)
{
ENTRY("vSleep");
  
  i8 nSeconds;
  
  if ( ! nnpTask->bPopParam(nSeconds) )
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
/* ! -- store value                                                           */
/*----------------------------------------------------------------------------*/
void Task::vStore(NNP_Task nnpTask)
{
  ENTRY("vStore");
  
  Object* pObject;
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("!");
  
  ValWsp xValStored;
  if ( ! nnpTask->bPopParam(xValStored) )
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
/* - -- subtract second integer from top integer on stack                     */
/*----------------------------------------------------------------------------*/
void Task::vSubtract(NNP_Task nnpTask)
{
ENTRY("vSubtract");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("-");
  
  r = i - j;
  
  nnpTask->vPushParam(r);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vSwap(NNP_Task nnpTask)
{
ENTRY("vSwap");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->bPopParam(xValA,xValB) )
    THROW_INVALID_PARAM("swap");
  
  nnpTask->vPushParam(&xValA);
  nnpTask->vPushParam(&xValB);
  
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
/* then -- then clause                                                        */
/*----------------------------------------------------------------------------*/
void Task::vThen(NNP_Task nnpTask)
{
ENTRY("vThen");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pIfJumpArg;
  if ( ! nnpTask->bPopReturn(pIfJumpArg) )
    THROW_INVALID_RETURN("then");
  
  pIfJumpArg->m_pJumpLocn = pCurrentWord->pLastCell();
  
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
    nnpTask->vPushParam(pWord);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >body -- convert word pointer to body pointer                              */
/*----------------------------------------------------------------------------*/
void Task::vToBody(NNP_Task nnpTask)
{
ENTRY("vToBody");
  
  Word* pWord;
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM(">body");

  PtrVal xPtrBody;
  xPtrBody = pcAdd(pWord,sizeof(Word));

  nnpTask->vPushParam(&xPtrBody);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >cmd -- convert stack values to cmd string                                 */
/*----------------------------------------------------------------------------*/
void Task::vToCmd(NNP_Task nnpTask)
{
ENTRY("vToCmd");

  i8 nValues;
  if ( ! nnpTask->bPopParam(nValues) )
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
  
  nnpTask->vPushParam(&xStr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Task::vToInt(NNP_Task nnpTask)
{
ENTRY("vToInt");
  
  ValWsp xStr;
  if ( ! nnpTask->bPopParam(xStr) )
    THROW_INVALID_PARAM(">int");
  
  s256 sBuff;
  xStr.vToString(sBuff.a());
  
  i8 i = String::iToI8(sBuff.a());

  nnpTask->vPushParam(i);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* r> -- move value from rstack to pstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vToPstack(NNP_Task nnpTask)
{
ENTRY("vToPstack");
  
  ValWsp xVal;
  if ( ! nnpTask->bPopReturn(xVal) )
    THROW_INVALID_PARAM("r>");
  
  nnpTask->vPushParam(xVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >r -- move value from pstack to rstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vToRstack(NNP_Task nnpTask)
{
ENTRY("vToRstack");
  
  ValWsp xValOpaque;
  if ( ! nnpTask->bPopParam(xValOpaque) )
    THROW_INVALID_PARAM(">r");

  nnpTask->vPushReturn(xValOpaque);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >seitf -- convert int to seitf ptr                                         */
/*----------------------------------------------------------------------------*/
void Task::vToSEitf(NNP_Task nnpTask)
{
ENTRY("vToSEitf");
  
  i8 iVal;
  
  if ( ! nnpTask->bPopParam(iVal) )
    THROW_INVALID_PARAM(">seitf");
  
  PtrVal xPtrSEitf;
  xPtrSEitf = reinterpret_cast<SEitf*>(iVal);
  
  nnpTask->vPushParam(&xPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >str -- convert value to str                                               */
/*----------------------------------------------------------------------------*/
void Task::vToStr(NNP_Task nnpTask)
{
ENTRY("vToStr");
  
  ValWsp xVal;
  if ( ! nnpTask->bPopParam(xVal) )
    THROW_INVALID_PARAM(">str");

  char wBuffer[64];
  xVal.vToString(wBuffer);

  nnpTask->vPushParam(wBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TotalSpace -- push total space in buffer pool onto stack                   */
/*----------------------------------------------------------------------------*/
void Task::vTotalSpace(NNP_Task nnpTask)
{
ENTRY("vTotalSpace");
  
  i8 iTotalSpace = gpBufferPool->iRegionSize();
  
  nnpTask->vPushParam(iTotalSpace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >task -- convert int to task ptr                                           */
/*----------------------------------------------------------------------------*/
void Task::vToTask(NNP_Task nnpTask)
{
ENTRY("vToTask");
  
  i8 iVal;

  if ( ! nnpTask->bPopParam(iVal) )
    THROW_INVALID_PARAM(">task");
  
  PtrVal xPtrTask;
  xPtrTask = reinterpret_cast<Task*>(iVal);
  
  nnpTask->vPushParam(xPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >valset -- collapse values on pstack to a valset                           */
/*----------------------------------------------------------------------------*/
void Task::vToValSet(NNP_Task nnpTask)
{
ENTRY("vToValSet");

  i8 nVal;

  if ( ! nnpTask->bPopParam(nVal) )
    THROW_INVALID_PARAM(">valset");
    
  if ( ! nnpTask->bGuardParam(nVal) )
    THROW_INVALID_PARAM(">valset");

  char* pCursor = nullptr;
  ValSet xValSet;
  
  ValWsp xVal;
  for (i8 iVal = 0; iVal < nVal; iVal++) {
    
    if ( ! nnpTask->bPopParam(xVal) )
      THROW_INVALID_PARAM(">valset");
    
    
    xValSet.vAppend(pCursor,xVal);
  }

//  vTrace("ValSet %lld bytes\n",xValSet.iSize());
  
  nnpTask->vPushParam(xValSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >values -- expand valset to values on pstack                               */
/*----------------------------------------------------------------------------*/
void Task::vToValues(NNP_Task nnpTask)
{
ENTRY("vToValues");
 
  ValSet xValSet;
  
  if ( ! nnpTask->bPopParam(xValSet) )
    THROW_INVALID_PARAM(">values");
  
  i8 nVal = 0;
  ValWsp* pValWsp[128];
  
  char* pCursor = nullptr;
  while (xValSet.bExtract(pCursor,pValWsp[nVal]))
    nVal++;
  
  for (i8 iVal = nVal - 1; iVal >= 0; iVal--)
    nnpTask->vPushParam(pValWsp[iVal]);

  nnpTask->vPushParam(nVal);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing off                                                           */
/*----------------------------------------------------------------------------*/
void Task::vTraceOff(NNP_Task nnpTask)
{
ENTRY("vTraceOff");
  
  nnpTask->m_bTracing = false;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* turn tracing on                                                            */
/*----------------------------------------------------------------------------*/
void Task::vTraceOn(NNP_Task nnpTask)
{
ENTRY("vTraceOn");
  
  nnpTask->m_bTracing = true;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* unloop -- pop the return stack                                             */
/*----------------------------------------------------------------------------*/
void Task::vUnloop(NNP_Task nnpTask)
{
ENTRY("vUnloop");
  
  // we should just modify the pointers rather than actually pop
  ValWsp xValOpaque;
  if ( ! nnpTask->bPopReturn(xValOpaque) )
    THROW_INVALID_RETURN("unloop");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* until -- repeat begin loop until true                                      */
/*----------------------------------------------------------------------------*/
void Task::vUntil(NNP_Task nnpTask)
{
ENTRY("vUntil");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pBeginLoc;
  if ( ! nnpTask->bPopReturn(pBeginLoc) )
    THROW_INVALID_RETURN("until");
  
  //-------------------------------------------------------------------------
  
  pCurrentWord->vEncode("jumpFalse",pBeginLoc);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* version -- push version string                                             */
/*----------------------------------------------------------------------------*/
void Task::vVersion(NNP_Task nnpTask)
{
ENTRY("vSersion");
  
  s256 sVersion;
  sVersion = RAPIDSSE_VERSION_STRING;
  
  nnpTask->vPushParam(sVersion);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* wait -- this task waits for signal                                         */
/*----------------------------------------------------------------------------*/
void Task::vWait(NNP_Task nnpTask)
{
ENTRY("vWait");
  
  // ..tbd..
  
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
/* while -- check for continue with loop                            immediate */
/*----------------------------------------------------------------------------*/
void Task::vWhile(NNP_Task nnpTask)
{
ENTRY("vWhile");

  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("jumpFalse");
  Cell* pWhileJumpArg = (Cell*)pCurrentWord->pAppend((i8)0);
  
  nnpTask->vPushReturnVoid(pWhileJumpArg);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0< -- test if top value is less than zero                                  */
/*----------------------------------------------------------------------------*/
void Task::vZeroLess(NNP_Task nnpTask)
{
ENTRY("vZeroLess");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0<");
  
  if (iZero < 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0<= -- test if top value is less than or equal to zero                     */
/*----------------------------------------------------------------------------*/
void Task::vZeroLessEqual(NNP_Task nnpTask)
{
ENTRY("vZeroLessEqual");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0<=");
  
  if (iZero <= 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0== -- test if top value is equal to zero / nullptr                        */
/*----------------------------------------------------------------------------*/
void Task::vZeroEqual(NNP_Task nnpTask)
{
ENTRY("vZeroEqual");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0==");
  
  if (iZero == 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0!= -- test if top value is not equal to zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroNotEqual(NNP_Task nnpTask)
{
ENTRY("vZeroNotEqual");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0!=");
  
  if (iZero != 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* zeroGreaterEqual -- test if top value is greater than or equal to zero     */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreaterEqual(NNP_Task nnpTask)
{
ENTRY("vZeroGreaterEqual");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0>=");
  
  if (iZero >= 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* 0> -- test if top value is greater than zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreater(NNP_Task nnpTask)
{
ENTRY("vZeroGreater");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0>");
  
  if (iZero > 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// assert: /////////////////////////////////////////////////////////////////////

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

// args: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* args.partition -- get the speficic partition from arguments                */
/*----------------------------------------------------------------------------*/
void Task::vArgsPartition(NNP_Task nnpTask)
{
ENTRY("vArgsPartition");
  
  nnpTask->vPushParam(gxArguments.m_iPartition);
  
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

// csv: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* csv.firstValue -- get first value from csv string                          */
/*----------------------------------------------------------------------------*/
void Task::vCsvFirstValue(NNP_Task nnpTask)
{
ENTRY("vCsvFirstValue");
  
  ValWsp xStr;
  if ( ! nnpTask->bPopParam(xStr) )
    THROW_INVALID_PARAM("csv.firstValue");
  
  s256 sFirstValue;
  
  char* z = xStr.sStr();
  
  if (! String::bExtractItem(&z,sFirstValue.a()))
    sFirstValue = "";
  
  nnpTask->vPushParam(sFirstValue);
  
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
    iCount = nnpTask->m_xDictionaryStack.iCount();
  }
  
  nnpTask->vPushParam(iCount);
  
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
    nnpTask->m_xDictionaryStack.vDiscard();
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
  
  if ( ! nnpTask->bPopParam(pPtrDic) )
    THROW_INVALID_PARAM("dic.frozen@");
  
  Dictionary* pDic = _pDic(pPtrDic);
  
  bool bFrozen = pDic->bFrozen();
  
  nnpTask->vPushParam(bFrozen);
  
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
  
  if ( ! nnpTask->bPopParam(iFrozen,pPtrDic) )
    THROW_INVALID_PARAM("dic.frozen@");
  
  Dictionary* pDic = _pDic(pPtrDic);
  
  pDic->vFrozen(iFrozen);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.stack -- print dictionary stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vDicStack(NNP_Task nnpTask)
{
ENTRY("vDicDump");
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->pDicStack()->vDump();
  }
  else {
    nnpTask->m_xDictionaryStack.vDump();
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
  
  if ( ! nnpTask->bPopParam(pPtrDic) )
    THROW_INVALID_PARAM("dic.init");
  
  Dictionary* pDic = _pDic(pPtrDic);

  new (pDic) Dictionary;
  
  // this is because of GlobalDictionary (ugh)
  pDic->vInit();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.last -- get last word of dictionary                                    */
/*----------------------------------------------------------------------------*/
void Task::vDicLast(NNP_Task nnpTask)
{
ENTRY("vDicLast");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->bPopParam(pPtrDic) )
    THROW_INVALID_PARAM("dic.last");
  
  Dictionary* pDic = _pDic(pPtrDic);
  
  Word* pLastWord = pDic->pCurrentWord();
  
  nnpTask->vPushParam(pLastWord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.main -- push main dictionary onto stack                                */
/*----------------------------------------------------------------------------*/
void Task::vDicMain(NNP_Task nnpTask)
{
ENTRY("vDicMain");
  
  nnpTask->vPushParam(gpMainDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.name -- push name of dictionary onto stack                             */
/*----------------------------------------------------------------------------*/
void Task::vDicName(NNP_Task nnpTask)
{
ENTRY("vDicName");
 
  Object* pPtrDic;
  if ( ! nnpTask->bPopParam(pPtrDic) )
    THROW_INVALID_PARAM("dic.name");
  
  Dictionary* pDic = _pDic(pPtrDic);

  ValWsp sName;
  sName = pDic->sName().a();
  
  nnpTask->vPushParam(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.pick -- put a dictionary from dictionary stack onto param stack        */
/*----------------------------------------------------------------------------*/
void Task::vDicPick(NNP_Task nnpTask)
{
ENTRY("vDicPick");
  
  i8 iDic;
  if ( ! nnpTask->bPopParam(iDic) )
    THROW_INVALID_PARAM("dic.pick");
  
  Object* pPtrDic;

  if (nnpTask->m_pSEitf) {
    pPtrDic = nnpTask->m_pSEitf->pDicStack()->pPick(iDic);
  }
  else {
    pPtrDic = nnpTask->m_xDictionaryStack.pPick(iDic);
  }
  
  nnpTask->vPushParam(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dic.push -- push a dictionary from param stack onto dictionary stack       */
/*----------------------------------------------------------------------------*/
void Task::vDicPush(NNP_Task nnpTask)
{
ENTRY("vDicPush");
  
  Object* pPtrDic;
  
  if ( ! nnpTask->bPopParam(pPtrDic) )
    THROW_INVALID_PARAM("dic.push");

  Dictionary* pDic = _pDic(pPtrDic);
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->vPushDic(pDic);
  }
  else {
    nnpTask->m_xDictionaryStack.vPush(pDic);
  }
  
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
  
  if ( ! nnpTask->bPopParam(sPath) )
    THROW_INVALID_PARAM("dir.exists");
  
  Dir xDir;
  
  i8 iResult = xDir.bExists(sPath);
  nnpTask->vPushParam(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.make -- make a directory                                               */
/*----------------------------------------------------------------------------*/
void Task::vDirMake(NNP_Task nnpTask)
{
ENTRY("vDirMake");
  
  s256 sPath;
  
  if ( ! nnpTask->bPopParam(sPath) )
    THROW_INVALID_PARAM("dir.make");
  
  Dir xDir;
  
  i8 iResult = xDir.bMake(sPath);
  nnpTask->vPushParam(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.remove -- remove a directory                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirRemove(NNP_Task nnpTask)
{
ENTRY("vDirRemove");
  
  s256 sPath;
  
  if ( ! nnpTask->bPopParam(sPath) )
    THROW_INVALID_PARAM("dir.remove");
  
  Dir xDir;
  
  i8 iResult = xDir.bRemove(sPath);
  nnpTask->vPushParam(iResult);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* cwd -- change working directory                                            */
/*----------------------------------------------------------------------------*/
void Task::vCwd(NNP_Task nnpTask)
{
ENTRY("vCwd");
  
  s256 sDir;
  if ( ! nnpTask->bPopParam(sDir) )
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
    nnpTask->vPushParam(&xStrVal);
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

  if ( ! nnpTask->bPopParam(pPtrDir) )
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
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
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
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
    THROW_INVALID_PARAM("dir.close");
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sEntryName;
  
  sEntryName = pDir->sEntryName();
  
  nnpTask->vPushParam(sEntryName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.entryType -- get dir entry type                                        */
/*----------------------------------------------------------------------------*/
void Task::vDirEntryType(NNP_Task nnpTask)
{
ENTRY("vDirEntryType");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
    THROW_INVALID_PARAM("dir.close");
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iEntryType;
  
  iEntryType = pDir->iEntryType();
  
  nnpTask->vPushParam(iEntryType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.init -- init directory                                                 */
/*----------------------------------------------------------------------------*/
void Task::vDirInit(NNP_Task nnpTask)
{
ENTRY("vDirInit");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
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
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
    THROW_INVALID_PARAM("dir.close");
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsClosed = pDir->bIsClosed();
  
  nnpTask->vPushParam(bIsClosed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.isOpen -- is dir open?                                                 */
/*----------------------------------------------------------------------------*/
void Task::vDirIsOpen(NNP_Task nnpTask)
{
ENTRY("vDirIsOpen");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
    THROW_INVALID_PARAM("dir.close");
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bIsOpen = pDir->bIsOpen();
  
  nnpTask->vPushParam(bIsOpen);
  
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
  
  nnpTask->vPushParam(pPtrDir);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.open -- open dir for reading                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirOpen(NNP_Task nnpTask)
{
ENTRY("vDirOpen");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
    THROW_INVALID_PARAM("dir.open");
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bOpened = pDir->bOpen();
  
  nnpTask->vPushParam(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path@ -- get path                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDirPathGet(NNP_Task nnpTask)
{
ENTRY("vDirPathGet");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
    THROW_INVALID_PARAM("dir.path@");
  
  Dir* pDir = _pDir(pPtrDir);
  
  s256 sPath;
  
  sPath = pDir->sPath();
  
  nnpTask->vPushParam(sPath);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.path! -- set path                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDirPathSet(NNP_Task nnpTask)
{
ENTRY("vDirPathSet");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
    THROW_INVALID_PARAM("dir.path!");
  
  Dir* pDir = _pDir(pPtrDir);
  
  ValWsp xWspPath;

  if ( ! nnpTask->bPopParam(xWspPath) )
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
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
    THROW_INVALID_PARAM("dir.read");
  
  Dir* pDir = _pDir(pPtrDir);
  
  bool bRead  = pDir->bRead();
  
  nnpTask->vPushParam(bRead);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* dir.rewind -- rewind dir pointer                                           */
/*----------------------------------------------------------------------------*/
void Task::vDirRewind(NNP_Task nnpTask)
{
ENTRY("vDirRewind");
  
  Object* pPtrDir;
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
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
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
    THROW_INVALID_PARAM("dir.seek");
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition;
  if ( ! nnpTask->bPopParam(iPosition) )
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
  
  if ( ! nnpTask->bPopParam(pPtrDir) )
    THROW_INVALID_PARAM("dir.tell");
  
  Dir* pDir = _pDir(pPtrDir);
  
  i8 iPosition = pDir->iTell();
  
  nnpTask->vPushParam(iPosition);
  
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
  
  if ( ! nnpTask->bPopParam(pPtrFile) )
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
  
  if ( ! nnpTask->bPopParam(pPtrFile) )
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
  
  if ( ! nnpTask->bPopParam(pPtrFile) )
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
  
  nnpTask->vPushParam(pPtrFile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file                                                   */
/*----------------------------------------------------------------------------*/
void Task::vFileOpen(NNP_Task nnpTask)
{
ENTRY("vFileOpen");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->bPopParam(pPtrFile) )
    THROW_INVALID_PARAM("file.open");
  
  File* pFile = _pFile(pPtrFile);
  
  s32 sMode;
  
  if ( ! nnpTask->bPopParam(sMode) )
    THROW("file.open");
  
  bool bOpened = pFile->bOpen(sMode.a());
  
  nnpTask->vPushParam(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path@ -- get file path                                                */
/*----------------------------------------------------------------------------*/
void Task::vFilePathGet(NNP_Task nnpTask)
{
ENTRY("vFilePathGet");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->bPopParam(pPtrFile) )
    THROW_INVALID_PARAM("file.path@");
  
  File* pFile = _pFile(pPtrFile);

  s256 sPath = pFile->sPath();
  
  nnpTask->vPushParam(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* file.path! -- set file path                                                */
/*----------------------------------------------------------------------------*/
void Task::vFilePathSet(NNP_Task nnpTask)
{
ENTRY("vFilePathSet");
  
  Object* pPtrFile;
  
  if ( ! nnpTask->bPopParam(pPtrFile) )
    THROW_INVALID_PARAM("file.path!");
  
  File* pFile = _pFile(pPtrFile);
  
  s256 sPath;
  
  if ( ! nnpTask->bPopParam(sPath) )
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
  
  if ( ! nnpTask->bPopParam(pPtrFile) )
    THROW_INVALID_PARAM("file.read");
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  s8K sLine;

  if (pFile->bReadLine(sLine)) {
    nnpTask->vPushParam(sLine);
    nnpTask->vPushParam((i8)1);
  }
  else {
    nnpTask->vPushParam((i8)0);
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
  
  if ( ! nnpTask->bPopParam(pPtrFile) )
    THROW_INVALID_PARAM("file.write");
  
  File* pFile = _pFile(pPtrFile);
  ASSERT(pFile);
  
  ValWsp xValue;
  
  if ( ! nnpTask->bPopParam(xValue) )
    THROW_INVALID_PARAM("file.write");
  
  char* wStr = xValue.pAsStrVal()->sStr();
  
  pFile->vPrint("%s",wStr);
  
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
  
  if ( ! nnpTask->bPopParam(pPtrNode) )
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
  
  PtrVal xPtrGrid;
  
  if ( ! nnpTask->bPopParam(xPtrGrid) )
    THROW_INVALID_PARAM("grid.init");
  
  Grid* pGrid = reinterpret_cast<Grid*>(xPtrGrid.pPtr());
  ASSERT(CN_GRID == pGrid->eClass());
  
  new(pGrid) Grid;
  
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
  
  nnpTask->vPushParam(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.next -- get next input token from current input stream               */
/*----------------------------------------------------------------------------*/
void Task::vInputNext(NNP_Task nnpTask)
{
ENTRY("vInputNext");
  
  if (nnpTask->m_bTracing)
    ASSERT(nnpTask->m_bTracing);
  
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
  
  if ( ! nnpTask->bPopParam(sFilePath) )
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
  
  if ( ! nnpTask->bPopParam(sInput) )
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
  
  nnpTask->vPushParam(nnpTask->m_sToken.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* input.type -- push current token type onto param stack                     */
/*----------------------------------------------------------------------------*/
void Task::vInputType(NNP_Task nnpTask)
{
ENTRY("vInputType");
  
  eTK eTokenType = nnpTask->m_eToken;
  
  nnpTask->vPushParam(eTokenType);
  
  EXIT();
}

// loader: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* loader.delete -- delete a table loader                                     */
/*----------------------------------------------------------------------------*/
void Task::vLoaderDelete(NNP_Task nnpTask)
{
ENTRY("vLoaderDelete");
  
  Object* pPtrLoader;
  if ( ! nnpTask->bPopParam(pPtrLoader) )
    THROW_INVALID_PARAM("loader.delete");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  delete pLoader;
  
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
  
  nnpTask->vPushParam(pLoader);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.init -- init a table loader                                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderInit(NNP_Task nnpTask)
{
ENTRY("vLoaderInit");
  
  Object* pPtrLoader;
  if ( ! nnpTask->bPopParam(pPtrLoader) )
    THROW_INVALID_PARAM("loader.init");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  new(pLoader) Loader;
  
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
  if ( ! nnpTask->bPopParam(pPtrLoader,pPtrTable) )
    THROW_INVALID_PARAM("loader.tableStore");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  Table* pTable = _pTable(pPtrTable);
  
  ASSERT(CN_TABLE == pTable->eClass());

  pLoader->vTable(pTable);
  
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
  if ( ! nnpTask->bPopParam(pPtrLoader,xWspPath) )
    THROW_INVALID_PARAM("loader.path!");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  s256 sPath;
  sPath = xWspPath.sStr();
  
  pLoader->vPath(sPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.txref! -- store txref into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTxRefStore(NNP_Task nnpTask)
{
ENTRY("vLoaderTxRefStore");
  
  Object* pPtrLoader;
  PtrVal xPtrTxRef;
  if ( ! nnpTask->bPopParam(pPtrLoader,xPtrTxRef) )
    THROW_INVALID_PARAM("loader.txref!");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  TxRef* pTxRef = reinterpret_cast<TxRef*>(xPtrTxRef.pPtr());
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  pLoader->vTxRef(pTxRef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.openfile -- open file to be loaded                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderOpenFile(NNP_Task nnpTask)
{
ENTRY("vLoaderOpenFile");
  
  Object* pPtrLoader;
  if ( ! nnpTask->bPopParam(pPtrLoader) )
    THROW_INVALID_PARAM("loader.openfile");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  bool bOpened = pLoader->bOpenFile();
  
  nnpTask->vPushParam(bOpened);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.closefile -- close file being loaded                                */
/*----------------------------------------------------------------------------*/
void Task::vLoaderCloseFile(NNP_Task nnpTask)
{
ENTRY("vLoaderCloseFile");
  
  Object* pPtrLoader;
  if ( ! nnpTask->bPopParam(pPtrLoader) )
    THROW_INVALID_PARAM("loader.closefile");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  pLoader->vCloseFile();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.fetchRecord -- fetch next record from file being loaded             */
/*----------------------------------------------------------------------------*/
void Task::vLoaderFetchRecord(NNP_Task nnpTask)
{
ENTRY("vLoaderFetchRecord");
  
  Object* pPtrLoader;
  if ( ! nnpTask->bPopParam(pPtrLoader) )
    THROW_INVALID_PARAM("loader.fetchRecord");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  bool bFetched = pLoader->bFetchRecord();
  
  nnpTask->vPushParam(bFetched);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.pushRecord -- fetch record onto stack                               */
/*----------------------------------------------------------------------------*/
void Task::vLoaderPushRecord(NNP_Task nnpTask)
{
ENTRY("vLoaderPushRecord");
  
  Object* pPtrLoader;
  if ( ! nnpTask->bPopParam(pPtrLoader) )
    THROW_INVALID_PARAM("loader.pushRecord");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  ValWsp xRecord;
  xRecord.vClass(CN_STRVAL);
  StrVal* sRecord = xRecord.pAsStrVal();
  
  pLoader->vGetRecord(sRecord);
  
  nnpTask->vPushParam(xRecord);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.insertRow -- insert row into table being loaded                     */
/*----------------------------------------------------------------------------*/
void Task::vLoaderInsertRow(NNP_Task nnpTask)
{
ENTRY("vLoaderInsertRow");
  
  Object* pPtrLoader;
  if ( ! nnpTask->bPopParam(pPtrLoader) )
    THROW_INVALID_PARAM("loader.insertRecord");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  i8 iPartn;
  if ( ! nnpTask->bPopParam(iPartn) )
    THROW_INVALID_PARAM("loader.insertRow");
  
  ASSERT(iPartn < gxArguments.m_nPartitions);
  bool bInserted = pLoader->bInsertRow(iPartn);
  
  nnpTask->vPushParam(bInserted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.begin -- begin loading                                              */
/*----------------------------------------------------------------------------*/
void Task::vLoaderBegin(NNP_Task nnpTask)
{
ENTRY("vLoaderBegin");
  
  Object* pPtrLoader;
  if ( ! nnpTask->bPopParam(pPtrLoader) )
    THROW_INVALID_PARAM("loader.begin");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  pLoader->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* loader.end -- end loading                                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderEnd(NNP_Task nnpTask)
{
ENTRY("vLoaderEnd");
  
  Object* pPtrLoader;
  if ( ! nnpTask->bPopParam(pPtrLoader) )
    THROW_INVALID_PARAM("loader.end");
  
  Loader* pLoader = _pLoader(pPtrLoader);
  ASSERT(CN_LOADER == pLoader->eClass());
  
  pLoader->vEnd();
  
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
  if ( ! nnpTask->bPopParam(pPtrLock) )
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
  if ( ! nnpTask->bPopParam(pPtrLock) )
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
  if ( ! nnpTask->bPopParam(pPtrLock) )
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
  if ( ! nnpTask->bPopParam(pPtrLock) )
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
  if ( ! nnpTask->bPopParam(pPtrLock) )
    THROW_INVALID_PARAM("lock.init");
  
  Lock* pLock = _pLock(pPtrLock);
  
  new(pLock) Lock;
  
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  EXIT();
}

// memory: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* memory.bytedump -- byte-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryByteDump(NNP_Task nnpTask)
{
ENTRY("vMemoryByteDump");
  
  Word* pWord;
  i8 iSize;
  
  if ( ! nnpTask->bPopParam(iSize) )
    THROW_INVALID_PARAM("memory.bytedump");
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("memory.bytedump");
  
  pWord->vByteDump(iSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* memory.worddump -- word-oriented dump                                      */
/*----------------------------------------------------------------------------*/
void Task::vMemoryWordDump(NNP_Task nnpTask)
{
ENTRY("vMemoryWordDump");
  
  Word* pWord;
  i8 iSize;
  
  if ( ! nnpTask->bPopParam(iSize) )
    THROW_INVALID_PARAM("memory.worddump");
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("memory.worddump");
  
  pWord->vWordDump(iSize);
  
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
  
  if ( ! nnpTask->bPopParam(pPtrNode) )
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
  
  if ( ! nnpTask->bPopParam(iNodeNo) )
    THROW_INVALID_PARAM("node.new");
  
  Node* pNode = new Node;
  ASSERT(CN_NODE == pNode->eClass());

  pNode->vNodeNo(iNodeNo);
  
  nnpTask->vPushParam(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* node.no -- push node number of node (of a grid)                            */
/*----------------------------------------------------------------------------*/
void Task::vNodeNo(NNP_Task nnpTask)
{
ENTRY("vNodeNo");
  
  Object* pPtrNode;
  
  if ( ! nnpTask->bPopParam(pPtrNode) )
    THROW_INVALID_PARAM("node.no");
  
  Node* pNode = _pNode(pPtrNode);
  ASSERT(CN_NODE == pNode->eClass());
  
  nnpTask->vPushParam(pNode->iNodeNo());
  
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
  if ( ! nnpTask->bPopParam(pPtrNodeCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrNodeCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrNodeCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrNodeCursor) )
    THROW_INVALID_PARAM("nodeCursor.first");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vFirst();
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->vPushParam(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.get -- push current node (of grid)                              */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorGet(NNP_Task nnpTask)
{
ENTRY("vNodeCursorGet");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->bPopParam(pPtrNodeCursor) )
    THROW_INVALID_PARAM("nodeCursor.get");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->vPushParam(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.init -- init a node cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorInit(NNP_Task nnpTask)
{
ENTRY("vNodeCursorInit");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->bPopParam(pPtrNodeCursor) )
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
  
  nnpTask->vPushParam(pNodeCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* nodeCursor.next -- advance to next and get node (of grid)                  */
/*----------------------------------------------------------------------------*/
void Task::vNodeCursorNext(NNP_Task nnpTask)
{
ENTRY("vNodeCursorNext");
  
  Object* pPtrNodeCursor;
  if ( ! nnpTask->bPopParam(pPtrNodeCursor) )
    THROW_INVALID_PARAM("nodeCursor.next");
  
  NodeCursor* pNodeCursor = _pNodeCursor(pPtrNodeCursor);
  ASSERT(CN_NODECURSOR == pNodeCursor->eClass());
  
  pNodeCursor->vNext();
  
  Object* pObject = pNodeCursor->pObject();
  
  nnpTask->vPushParam(pObject);
  
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
  
  nnpTask->vPushParam(iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* process.testpid -- test pid on stack                                       */
/*----------------------------------------------------------------------------*/
void Task::vProcessTestPid(NNP_Task nnpTask)
{
ENTRY("vProcessTestPid");

  i8 iPid;
  if ( ! nnpTask->bPopParam(iPid) )
    THROW_INVALID_PARAM("process.testpid");
  
  i8 iExists = Process::bTestPid(iPid);
  
  nnpTask->vPushParam(iExists);
  
  EXIT();
}

// pstack: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* p.depth -- put number of values in param stack onto param stack            */
/*----------------------------------------------------------------------------*/
void Task::vPstackDepth(NNP_Task nnpTask)
{
ENTRY("vPstackDepth");
  
  i8 iDepth = nnpTask->m_xParamStack.iValues();
  
  nnpTask->vPushParam(iDepth);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* pstack.class -- push class of top value on param stack                     */
/*----------------------------------------------------------------------------*/
void Task::vPstackClass(NNP_Task nnpTask)
{
ENTRY("vPstackClass");
  
  if ( ! nnpTask->bGuardParam(1) )
    THROW("pstack.class");

  ValWsp* pValWsp = static_cast<ValWsp*>(nnpTask->m_xParamStack.pPeek(0));
  
  eCN eClass = pValWsp->eClass();
  
  nnpTask->vPushParam(eClass);
  
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
  
  if ( ! nnpTask->bPopParam(pObject,xValWsp) )
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
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("q.dump");
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  pQueue->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.init -- queue init                                                   */
/*----------------------------------------------------------------------------*/
void Task::vQueueInit(NNP_Task nnpTask)
{
ENTRY("vQueueInit");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
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
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("queue.pop");
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  
  if (pQueue->bPop(&xValWsp)) {
    nnpTask->vPushParam(&xValWsp);
    nnpTask->vPushParam((i8)1);
  }
  else
    nnpTask->vPushParam((i8)0);
  
  
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
  if ( ! nnpTask->bPopParam(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.begin");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  i8 iPartn;
  if ( ! nnpTask->bPopParam(iPartn) )
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
  if ( ! nnpTask->bPopParam(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.delete");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pRowCursor);
  delete pRowCursor;
  
  gpSmallHeap->vFreeObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.end -- end using row cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorEnd(NNP_Task nnpTask)
{
ENTRY("vRowCursorEnd");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->bPopParam(pPtrRowCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.first");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vFirst();
  
  PtrVal xPtrRow;
  xPtrRow = pRowCursor->pRowAsStr();
  
  nnpTask->vPushParam(xPtrRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.get -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorGet(NNP_Task nnpTask)
{
ENTRY("vRowCursorGet");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->bPopParam(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.get");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  s8K xRow;
  xRow = pRowCursor->pRowAsStr();
  
  nnpTask->vPushParam(xRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorInit(NNP_Task nnpTask)
{
ENTRY("vRowCursorInit");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->bPopParam(pPtrRowCursor) )
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
  
  nnpTask->vPushParam(pRowCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.next -- advance to next and get row                              */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorNext(NNP_Task nnpTask)
{
ENTRY("vRowCursorNext");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->bPopParam(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.next");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vNext();
  
  PtrVal xPtrRow;
  xPtrRow = pRowCursor->pRowAsStr();
  
  nnpTask->vPushParam(xPtrRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.table@ -- fetch table from row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableFetch(NNP_Task nnpTask)
{
ENTRY("vRowCursorTableFetch");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->bPopParam(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.table@");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable = pRowCursor->pTable();
  
  nnpTask->vPushParam(pPtrTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.table! -- store table into row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableStore(NNP_Task nnpTask)
{
ENTRY("vRowCursorTableStore");
  
  Object* pPtrRowCursor;
  if ( ! nnpTask->bPopParam(pPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.table!");
  
  RowCursor* pRowCursor = _pRowCursor(pPtrRowCursor);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pPtrTable;
  if ( ! nnpTask->bPopParam(pPtrTable) )
    THROW_INVALID_PARAM("rowcursor.table!");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  pRowCursor->vTable(pTable);
  
  EXIT();
}

// seitf: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* seitf.delete -- delete SE interface                                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDelete(NNP_Task nnpTask)
{
ENTRY("vSEitfDelete");
  
  PtrVal xPtrSEitf;
  
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("seitf.delete");

  SEitf* pSEitf = static_cast<SEitf*>(xPtrSEitf.pPtr());
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
  
  PtrVal xPtrSEitf;
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("seitf.dump");
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(xPtrSEitf.pPtr());
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
  
  PtrVal xPtrSEitf;
  xPtrSEitf = pSEitf;
  
  nnpTask->vPushParam(xPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.req@ -- fetch SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqFetch(NNP_Task nnpTask)
{
ENTRY("vSEitfReqFetch");
  
  PtrVal xPtrSEitf;
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("seitf.req@");
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(xPtrSEitf.pPtr());
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  PtrVal xPtrQueue;
  xPtrQueue = pSEitf->pRequestQueue();
  
  nnpTask->vPushParam(xPtrQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.req! -- store SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqStore(NNP_Task nnpTask)
{
ENTRY("vSEitfReqStore");
  
  PtrVal xPtrSEitf;
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("seitf.req!");
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(xPtrSEitf.pPtr());
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  PtrVal xPtrQueue;
  
  if ( ! nnpTask->bPopParam(xPtrQueue) )
    THROW_INVALID_PARAM("seitf.req!");
  
  Queue* pQueue = reinterpret_cast<Queue*>(xPtrQueue.pPtr());
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
  
  PtrVal xPtrSEitf;
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("seitf.rsp@");
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(xPtrSEitf.pPtr());
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  PtrVal xPtrQueue;
  xPtrQueue = pSEitf->pResponseQueue();
  
  nnpTask->vPushParam(xPtrQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp! -- store SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspStore(NNP_Task nnpTask)
{
ENTRY("vSEitfRspStore");
  
  PtrVal xPtrSEitf;
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("seitf.rsp!");
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(xPtrSEitf.pPtr());
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  PtrVal xPtrQueue;
  
  if ( ! nnpTask->bPopParam(xPtrQueue) )
    THROW_INVALID_PARAM("seitf.rsp!");
  
  Queue* pQueue = reinterpret_cast<Queue*>(xPtrQueue.pPtr());
  ASSERT(CN_QUEUE == pQueue->eClass());
  
  pSEitf->vResponseQueue(pQueue);
  
  EXIT();
}

// seitfcursor: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* seitfcursor.begin -- begin using seitf currsor                             */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorBegin(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorBegin");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.begin");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());

  pSEitfCursor->vBegin();
  pSEitfCursor->vFirst();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->vPushParam(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.delete -- delete an seitf cursor                               */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorDelete(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorDelete");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.delete");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pSEitfCursor);
  delete pSEitfCursor;
  
  gpSmallHeap->vFreeObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.end -- end using seitf cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorEnd(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorEnd");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.end");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
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
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.first");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vFirst();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->vPushParam(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.get -- get current seitf                                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorGet(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorGet");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.get");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->vPushParam(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.init -- init an seitf cursor                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorInit(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorInit");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.init");
  
  s32 sName;
  sName = "seitflist";
  Word* pWord = nnpTask->pFindWord(sName);
  ASSERT(pWord);
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(pWord->pBodyArea());
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  new(pSEitfCursor) SEitfCursor(pSEitfList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.new -- new an seitf cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNew(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorNew");
  
  PtrVal xPtrSEitfList;
  if ( ! nnpTask->bPopParam(xPtrSEitfList) )
    THROW_INVALID_PARAM("seitfcursor.new");
  
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(xPtrSEitfList.pPtr());
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  SEitfCursor* pSEitfCursor = static_cast<SEitfCursor*>(gpSmallHeap->pAllocObject(CN_SEITFCURSOR));
  new(pSEitfCursor) SEitfCursor(pSEitfList);
  
  PtrVal xPtrSEitfCursor;
  xPtrSEitfCursor = pSEitfCursor;
  
  nnpTask->vPushParam(xPtrSEitfCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.next -- advance to next and get seitf                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNext(NNP_Task nnpTask)
{
ENTRY("vSEitfCursorNext");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.next");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vNext();
  
  Object* pObject = pSEitfCursor->pObject();
  
  nnpTask->vPushParam(pObject);
  
  EXIT();
}

// seitflist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* seitflist.dump -- dump of seitfList on param stack                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListDump(NNP_Task nnpTask)
{
ENTRY("vSEitfListDump");
  
  PtrVal xPtrSEitfList;
  if ( ! nnpTask->bPopParam(xPtrSEitfList) )
    THROW_INVALID_PARAM("seitflist.dump");
  
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(xPtrSEitfList.pPtr());
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  pSEitfList->vDump("SEitfList:");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* seitflist.init -- init of seitfList on param stack                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListInit(NNP_Task nnpTask)
{
ENTRY("vSEitfListInit");
  
  PtrVal xPtrSEitfList;
  if ( ! nnpTask->bPopParam(xPtrSEitfList) )
    THROW_INVALID_PARAM("seitflist.init");
  
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(xPtrSEitfList.pPtr());
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  new(pSEitfList) SEitfList;
  
  EXIT();
}

// stack: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* stack.class -- get class of nth value on stack                             */
/*----------------------------------------------------------------------------*/
void Task::vStackClass(NNP_Task nnpTask)
{
ENTRY("vStackClass");

  i8 nTh;
  if ( ! nnpTask->bPopParam(nTh) )
    THROW_INVALID_PARAM("stack.class");
  
  ValWsp xValWsp;
  
  nnpTask->m_xParamStack.vPeek(nTh,&xValWsp);
  
  eCN eClass = xValWsp.eClass();
  
  nnpTask->vPushParam(eClass);
  
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
  if ( ! nnpTask->bPopParam(iLength,xStringA) )
    THROW_INVALID_PARAM("strpad");
  
  String::vPad(xStringA.sStr(),iLength,' ');
  xStringA.pAsStrVal()->vSize();
  
  nnpTask->vPushParam(xStringA);
  
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
  if ( ! nnpTask->bPopParam(xStringB,xStringA) )
    THROW_INVALID_PARAM("streql");
  
  i8 iEqual = String::bEqualTo(xStringA.sStr(),xStringB.sStr());
  
  nnpTask->vPushParam(iEqual);
  
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
  if ( ! nnpTask->bPopParam(sString) )
    THROW_INVALID_PARAM("strlen");
  
  iLength = String::iLength(sString.a());
  
  nnpTask->vPushParam(iLength);
  
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
//  if ( ! nnpTask->bPopParam(iStart,iLength,xString) )
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
//  nnpTask->vPushParam(&xSubStr);
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
  if ( ! nnpTask->bPopParam(iStart,iLength,sString) )
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
  
  nnpTask->vPushParam(sSubStr);
  
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
  
  if ( ! nnpTask->bPopParam(xStrB,xStrA) )
    THROW_INVALID_PARAM("concat");
  
  xStrR = xStrA;
  
  String::vConcat(xStrR.sStr(),xStrB.sStr());
  xStrR.pAsStrVal()->vSize();
  
  nnpTask->vPushParam(xStrR);
  
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
//  if ( ! nnpTask->bPopParam(iLength,xString) )
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
//  nnpTask->vPushParam(&xSubStr);
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
  if ( ! nnpTask->bPopParam(iLength,sString) )
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
  
  nnpTask->vPushParam(sSubStr);
  
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
  if ( ! nnpTask->bPopParam(iDigit) )
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

  nnpTask->vPushParam(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* s256.init -- initialise an s256                                            */
/*----------------------------------------------------------------------------*/
void Task::vS256Init(NNP_Task nnpTask)
{
  ENTRY("vS256Init");
  
  PtrVal xPtrStrVal;
  
  if ( ! nnpTask->bPopParam(xPtrStrVal) )
    THROW_INVALID_PARAM("s256.init");
  
  StrVal* pStrVal = reinterpret_cast<StrVal*>(xPtrStrVal.pPtr());
  
  ASSERT(CN_STRVAL == pStrVal->eClass());
  
  new(pStrVal) StrVal(256);
  
  EXIT();
}

// table: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* table.dic -- get table dictionary                                          */
/*----------------------------------------------------------------------------*/
void Task::vTableDic(NNP_Task nnpTask)
{
ENTRY("vTableDic");
  
  Object* pPtrTable;
  if ( ! nnpTask->bPopParam(pPtrTable) )
    THROW_INVALID_PARAM("table.dic");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  Dictionary* pDictionary = pTable->pDic();
  
  nnpTask->vPushParam(pDictionary);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.drop -- drop of table on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableDrop(NNP_Task nnpTask)
{
ENTRY("vTableDrop");
  
  Object* pPtrTable;
  if ( ! nnpTask->bPopParam(pPtrTable) )
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
  if ( ! nnpTask->bPopParam(pPtrTable) )
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
  if ( ! nnpTask->bPopParam(pPtrTable) )
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
  PtrVal xPtrTxRef;
  ValWsp xRow;
  
  if ( ! nnpTask->bPopParam(pPtrTable) )
    THROW_INVALID_PARAM("table.insert");
  if ( ! nnpTask->bPopParam(xPtrTxRef) )
    THROW_INVALID_PARAM("table.insert");
  if ( ! nnpTask->bPopParam(xRow) )
    THROW_INVALID_PARAM("table.insert");

  i8 iPartition;
  if ( ! nnpTask->bPopParam(iPartition) )
    THROW_INVALID_PARAM("table.insert");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());

  TxRef* pTxRef = reinterpret_cast<TxRef*>(xPtrTxRef.pPtr());
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  pTable->vInsertRow(iPartition,pTxRef,xRow.pAsStrVal());       // MISSING PARTN NUMBER??
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* table.truncate -- release all rows/buffers in table on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTableTruncate(NNP_Task nnpTask)
{
ENTRY("vTableTruncate");
  
  Object* pPtrTable;
  if ( ! nnpTask->bPopParam(pPtrTable) )
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
  if ( ! nnpTask->bPopParam(pPtrTable) )
    THROW_INVALID_PARAM("table.spave");
  
  Table* pTable = _pTable(pPtrTable);
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iBuffers = pTable->iRowBuffers();
  i8 iSpace   = iBuffers * C_SIZE_LARGE_BUFFER;
  s256 sMag;
  
  nnpTask->vPushParam(iSpace);
  
  EXIT();
}

// task: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* task.dic -- push task dictionary onto param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskDic(NNP_Task nnpTask)
{
ENTRY("vTaskDic");
  
  Object* pPtrDic = &nnpTask->m_xTaskDictionary;
  
  nnpTask->vPushParam(pPtrDic);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dump -- dump of task on param stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vTaskDump(NNP_Task nnpTask)
{
ENTRY("vTaskDump");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.dump");
  
  pThatTask->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.init -- init of task on param stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vTaskInit(NNP_Task nnpTask)
{
ENTRY("vTaskInit");
  
  PtrVal xPtrTask;
  if ( ! nnpTask->bPopParam(xPtrTask) )
    THROW_INVALID_PARAM("task.init");
  
  Task* pThatTask = reinterpret_cast<Task*>(xPtrTask.pPtr());
  ASSERT(CN_TASK == pThatTask->eClass());
  
  new(pThatTask) Task;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seitf@ -- fetch SEitf of task on param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfFetch(NNP_Task nnpTask)
{
ENTRY("vTaskSeitfFetch");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.seitf@");
  
  SEitf* pSEitf = pThatTask->m_pSEitf;
  
  PtrVal xPtrSEitf;
  xPtrSEitf = pSEitf;
  
  nnpTask->vPushParam(xPtrSEitf);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seitf! -- store SEitf on param stack into task on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfStore(NNP_Task nnpTask)
{
ENTRY("vTaskSeitfStore");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.seitf!");
  
  PtrVal xPtrSEitf;
  
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("task.seitf!");
  
  SEitf* pSEitf = static_cast<SEitf*>(xPtrSEitf.pPtr());
  ASSERT(! pSEitf || CN_SEITF == pSEitf->eClass());
  
  pThatTask->m_pSEitf = pSEitf;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameFetch(NNP_Task nnpTask)
{
ENTRY("vTaskNameFetch");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.name@");
  
  nnpTask->vPushParam(pThatTask->m_sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name! -- store task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameStore(NNP_Task nnpTask)
{
ENTRY("vTaskNameStore");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.name!");
  
  s32 sName;
  if ( ! nnpTask->bPopParam(sName) )
    THROW_INVALID_PARAM("task.name!");
  
  pThatTask->m_sName = sName;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.new -- create a new task                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNew(NNP_Task nnpTask)
{
ENTRY("vTaskNew");
  
  if ( ! nnpTask->bGuardParam(CN_STRVAL) )
    THROW_INVALID_PARAM("task.new");
  
  s32 sSeed;
  if ( ! nnpTask->bPopParam(sSeed) )
    THROW_INVALID_PARAM("task.name!");
  
  char* aSeedWord = sSeed.a();
  
  NNP_Task nnpNewTask = new Task;
  
  nnpNewTask->vSeedWord(aSeedWord);
    
  Task* pNewTask = *nnpNewTask;
  gpTaskList->vAppend(pNewTask);
  
  nnpTask->vPushParam(nnpNewTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.queue@ -- fetch queue of task on param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskQfetch(NNP_Task nnpTask)
{
ENTRY("vTaskQfetch");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.queue@");
  
  Queue* pQueue = pThatTask->m_pQueue;
  
  Object* pQueueEmb = static_cast<Object*>(pQueue);
  
  nnpTask->vPushParam(pQueueEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.queue! -- store queue on param stack into task on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTaskQstore(NNP_Task nnpTask)
{
ENTRY("vTaskQstore");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.queue!");
  
  Object* pQueueEmb;
  
  if ( ! nnpTask->bPopParam(pQueueEmb) )
    THROW_INVALID_PARAM("task.queue!");
  
  Queue* pQueue = static_cast<Queue*>(pQueueEmb);
  
  pThatTask->m_pQueue = pQueue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.seed -- set the seed word of task                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeed(NNP_Task nnpTask)
{
ENTRY("vTaskSeed");
  
  ValWsp xStrWsp;
  
  if ( ! nnpTask->bPopParam(xStrWsp) )
    THROW_INVALID_PARAM("task.seed");
  
  char* pSeed = xStrWsp.pAsStrVal()->sStr();
  
  nnpTask->vSeedWord(pSeed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateFetch(NNP_Task nnpTask)
{
ENTRY("vTaskStateFetch");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.state@");
  
  eTS eState = pThatTask->m_eState;
  
  nnpTask->vPushParam((i8)eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state! -- store state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateStore(NNP_Task nnpTask)
{
ENTRY("vTaskStateStore");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.state!");
  
  i8 iState;
  
  if ( ! nnpTask->bPopParam(iState) )
    THROW_INVALID_PARAM("task.state!");
  
  pThatTask->m_eState = (eTS)iState;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.this -- push this tx                                                  */
/*----------------------------------------------------------------------------*/
void Task::vTaskThis(NNP_Task nnpTask)
{
ENTRY("vTaskThis");
  
  nnpTask->vPushParam(nnpTask);
  
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
  if ( ! nnpTask->bPopParam(pPtrTaskCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrTaskCursor) )
    THROW_INVALID_PARAM("taskCursor.delete");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pTaskCursor);
  delete pTaskCursor;
  
  gpSmallHeap->vFreeObject(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.end -- end using task cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorEnd(NNP_Task nnpTask)
{
ENTRY("vTaskCursorEnd");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->bPopParam(pPtrTaskCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrTaskCursor) )
    THROW_INVALID_PARAM("taskCursor.first");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vFirst();
  
  PtrVal xPtrTask;
  xPtrTask = pTaskCursor->pObject();
  
  nnpTask->vPushParam(xPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.get -- get current task of cursor                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorGet(NNP_Task nnpTask)
{
ENTRY("vTaskCursorGet");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->bPopParam(pPtrTaskCursor) )
    THROW_INVALID_PARAM("taskCursor.get");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  Task* pThatTask = pTaskCursor->pObject();
  
  PtrVal xPtrTask;
  xPtrTask = pThatTask;
  
  nnpTask->vPushParam(xPtrTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* taskCursor.init -- init an task cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorInit(NNP_Task nnpTask)
{
ENTRY("vTaskCursorInit");
  
  Object* pPtrTaskCursor;
  if ( ! nnpTask->bPopParam(pPtrTaskCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrTaskCursor) )
    THROW_INVALID_PARAM("taskCursor.next");
  
  TaskCursor* pTaskCursor = _pTaskCursor(pPtrTaskCursor);
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vNext();
  
  PtrVal xPtrTask;
  xPtrTask = pTaskCursor->pObject();
  
  nnpTask->vPushParam(xPtrTask);
  
  EXIT();
}

// tasklist: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* tasklist -- tasklist                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskList(NNP_Task nnpTask)
{
ENTRY("vTaskList");
  
  nnpTask->vPushParam(gpTaskList);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.append -- tasklist append                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskListAppend(NNP_Task nnpTask)
{
ENTRY("vTaskListAppend");
  
  PtrVal xPtrTaskList;
  if ( ! nnpTask->bPopParam(xPtrTaskList) )
    THROW_INVALID_PARAM("tasklist.append");
  
  TaskList* pTaskList = reinterpret_cast<TaskList*>(xPtrTaskList.pPtr());
  ASSERT(CN_TASKLIST == pTaskList->eClass());
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("tasklist.append");
  
  pTaskList->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* tasklist.dump -- tasklist dump                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListDump(NNP_Task nnpTask)
{
ENTRY("vTaskListDump");
  
  PtrVal xPtrTaskList;
  if ( ! nnpTask->bPopParam(xPtrTaskList) )
    THROW_INVALID_PARAM("tasklist.dump");
  
  TaskList* pTaskList = reinterpret_cast<TaskList*>(xPtrTaskList.pPtr());
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
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("taskset.append");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("taskset.append");
  
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
  
  if ( ! nnpTask->bPopParam(pObject) )
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
  
  if ( ! nnpTask->bPopParam(pObject) )
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
  
  if ( ! nnpTask->bPopParam(pThreadEmb) )
    THROW_INVALID_PARAM("thread.append");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("thread.append");
  
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
  
  if ( ! nnpTask->bPopParam(iCpu) )
    THROW_INVALID_PARAM("thread.bind");
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.bind");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vBind(iCpu);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.cpu -- get the cpu this thread is running on                        */
/*----------------------------------------------------------------------------*/
void Task::vThreadCpu(NNP_Task nnpTask)
{
ENTRY("vThreadCpu");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.cpu");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  nnpTask->vPushParam(pThread->iCpu());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.dump -- thread dump                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadDump(NNP_Task nnpTask)
{
ENTRY("vThreadDump");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.dump");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
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
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.id");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  nnpTask->vPushParam(pThread->m_xPosixThread.m_iOpSysThreadId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.init -- thread init                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadInit(NNP_Task nnpTask)
{
ENTRY("vThreadInit");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.init");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  new(pThread) Thread;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name@ -- fetch thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameFetch(NNP_Task nnpTask)
{
ENTRY("vThreadNameFetch");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.name@");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  sName = pThread->sName();
  
  nnpTask->vPushParam(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.name! -- store thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameStore(NNP_Task nnpTask)
{
ENTRY("vThreadNameStore");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.name!");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  ValWsp xNameWsp;
  if ( ! nnpTask->bPopParam(xNameWsp) )
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
  
  if ( ! nnpTask->bPopParam(pThreadEmb) )
    THROW_INVALID_PARAM("thread.taskset@");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;

  Object* pTaskSetEmb = static_cast<Object*>(pTaskSet);;

  nnpTask->vPushParam(pTaskSetEmb);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* thread.taskset! -- store taskset into the thread which is on param stack   */
/*----------------------------------------------------------------------------*/
void Task::vThreadSstore(NNP_Task nnpTask)
{
ENTRY("vThreadSstore");
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->bPopParam(pThreadEmb) )
    THROW_INVALID_PARAM("thread.taskset!");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  Object* pTaskSetEmb;
  
  if ( ! nnpTask->bPopParam(pTaskSetEmb) )
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
  
  if ( ! nnpTask->bPopParam(nSeconds) )
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
  
  if ( ! nnpTask->bPopParam(nMilliseconds) )
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
  
  if ( ! nnpTask->bPopParam(nMicroseconds) )
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
  
  if ( ! nnpTask->bPopParam(nNanoseconds) )
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
  
  if ( ! nnpTask->bPopParam(pThreadEmb) )
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
  if ( ! nnpTask->bPopParam(pPtrThreadCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrThreadCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrThreadCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrThreadCursor) )
    THROW_INVALID_PARAM("threadCursor.first");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vFirst();
  
  PtrVal xPtrThread;
  xPtrThread = pThreadCursor->pObject();
  
  nnpTask->vPushParam(xPtrThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.get -- push current thread                                    */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorGet(NNP_Task nnpTask)
{
ENTRY("vThreadCursorGet");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->bPopParam(pPtrThreadCursor) )
    THROW_INVALID_PARAM("threadCursor.get");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  Object* pObject = pThreadCursor->pObject();
  
  nnpTask->vPushParam(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.init -- init a thread cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorInit(NNP_Task nnpTask)
{
ENTRY("vThreadCursorInit");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->bPopParam(pPtrThreadCursor) )
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
  
  nnpTask->vPushParam(pThreadCursor);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadCursor.next -- advance to next and push thread                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadCursorNext(NNP_Task nnpTask)
{
ENTRY("vThreadCursorNext");
  
  Object* pPtrThreadCursor;
  if ( ! nnpTask->bPopParam(pPtrThreadCursor) )
    THROW_INVALID_PARAM("threadCursor.next");
  
  ThreadCursor* pThreadCursor = _pThreadCursor(pPtrThreadCursor);
  ASSERT(CN_THREADCURSOR == pThreadCursor->eClass());
  
  pThreadCursor->vNext();
  
  PtrVal xPtrThread;
  xPtrThread = pThreadCursor->pObject();
  
  nnpTask->vPushParam(xPtrThread);
  
  EXIT();
}

// threadlist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* threadlist.dump -- dump the threadList on param stack                      */
/*----------------------------------------------------------------------------*/
void Task::vThreadListDump(NNP_Task nnpTask)
{
ENTRY("vThreadListDump");
  
  PtrVal xPtrThreadList;
  if ( ! nnpTask->bPopParam(xPtrThreadList) )
    THROW_INVALID_PARAM("threadlist.dump");
  
  ThreadList* pThreadList = reinterpret_cast<ThreadList*>(xPtrThreadList.pPtr());
  ASSERT(CN_THREADLIST == pThreadList->eClass());

  pThreadList->vDump("ThreadList:");
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* threadlist.init -- init of threadList on param stack                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadListInit(NNP_Task nnpTask)
{
ENTRY("vThreadListInit");
  
  PtrVal xPtrThreadList;
  if ( ! nnpTask->bPopParam(xPtrThreadList) )
    THROW_INVALID_PARAM("threadlist.init");
  
  ThreadList* pThreadList = reinterpret_cast<ThreadList*>(xPtrThreadList.pPtr());
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
  
  PtrVal xPtrSession;
  
  if ( ! nnpTask->bPopParam(xPtrSession) )
    THROW_INVALID_PARAM("timer.delete");
  
  Timer* pTimer = static_cast<Timer*>(xPtrSession.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pTimerObject = reinterpret_cast<Object*>(pTimer);
  
  delete pTimer;
  
  gpSmallHeap->vFreeObject(pTimerObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.init -- init timer                                                   */
/*----------------------------------------------------------------------------*/
void Task::vTimerInit(NNP_Task nnpTask)
{
ENTRY("vTimerInit");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.init");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
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
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.microsecs");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMicroseconds = pTimer->iMicroseconds();
  
  nnpTask->vPushParam(iMicroseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.millisecs -- get elapsed duration in milliseconds                    */
/*----------------------------------------------------------------------------*/
void Task::vTimerMillisecs(NNP_Task nnpTask)
{
ENTRY("vTimerMillisecs");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.millisecs");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMilliseconds = pTimer->iMilliseconds();
  
  nnpTask->vPushParam(iMilliseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.nanosecs -- get elapsed duration in nanoseconds                      */
/*----------------------------------------------------------------------------*/
void Task::vTimerNanosecs(NNP_Task nnpTask)
{
ENTRY("vTimerNanosecs");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.nanosecs");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iNanoseconds = pTimer->iNanoseconds();
  
  nnpTask->vPushParam(iNanoseconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.new -- new timer                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTimerNew(NNP_Task nnpTask)
{
ENTRY("vTimerNew");
  
  Timer* pTimer = reinterpret_cast<Timer*>(gpSmallHeap->pAllocObject(CN_TIMER));

  new(pTimer) Timer;
 
  ASSERT(CN_TIMER == pTimer->eClass());
  
  PtrVal xPtrTimer;
  xPtrTimer = pTimer;
  
  nnpTask->vPushParam(xPtrTimer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.reset -- reset timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerReset(NNP_Task nnpTask)
{
ENTRY("vTimerReset");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.reset");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
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
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.secs");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iSeconds = pTimer->iSeconds();
  
  nnpTask->vPushParam(iSeconds);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.start -- start timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerStart(NNP_Task nnpTask)
{
ENTRY("vTimerStart");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.Start");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
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
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.ticks");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iTicks = pTimer->iTicks();
  
  nnpTask->vPushParam(iTicks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* timer.update -- update timer                                               */
/*----------------------------------------------------------------------------*/
void Task::vTimerUpdate(NNP_Task nnpTask)
{
ENTRY("vTimerUpdate");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.update");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
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
  
  PtrVal xPtrTx;
  
  if ( ! nnpTask->bPopParam(xPtrTx) )
    THROW_INVALID_PARAM("tx.delete");
  
  Tx* pTx = static_cast<Tx*>(xPtrTx.pPtr());
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
  
  PtrVal xPtrTxCollection;
  if ( ! nnpTask->bPopParam(xPtrTxCollection) )
    THROW_INVALID_PARAM("txCollection.dump");
  
  TxCollection* pTxCollection = reinterpret_cast<TxCollection*>(xPtrTxCollection.pPtr());
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
  
  PtrVal xPtrTxCollection;
  if ( ! nnpTask->bPopParam(xPtrTxCollection) )
    THROW_INVALID_PARAM("txCollection.init");
  
  TxCollection* pTxCollection = reinterpret_cast<TxCollection*>(xPtrTxCollection.pPtr());
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
  if ( ! nnpTask->bPopParam(pPtrTxCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrTxCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrTxCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrTxCursor) )
    THROW_INVALID_PARAM("txCursor.first");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  pTxCursor->vFirst();
  
  PtrVal xPtrThread;
  xPtrThread = pTxCursor->pObject();
  
  nnpTask->vPushParam(xPtrThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.get -- push current tx                                            */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorGet(NNP_Task nnpTask)
{
ENTRY("vTxCursorGet");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->bPopParam(pPtrTxCursor) )
    THROW_INVALID_PARAM("txCursor.get");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_TXCURSOR == pTxCursor->eClass());
  
  Object* pObject = pTxCursor->pObject();
  
  nnpTask->vPushParam(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txCursor.init -- init a tx cursor                                          */
/*----------------------------------------------------------------------------*/
void Task::vTxCursorInit(NNP_Task nnpTask)
{
ENTRY("vTxCursorInit");
  
  Object* pPtrTxCursor;
  if ( ! nnpTask->bPopParam(pPtrTxCursor) )
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
  if ( ! nnpTask->bPopParam(pPtrTxCursor) )
    THROW_INVALID_PARAM("txCursor.next");
  
  TxCursor* pTxCursor = _pTxCursor(pPtrTxCursor);
  ASSERT(CN_THREADCURSOR == pTxCursor->eClass());
  
  pTxCursor->vNext();
  
  PtrVal xPtrTx;
  xPtrTx = pTxCursor->pObject();
  
  nnpTask->vPushParam(xPtrTx);
  
  EXIT();
}

// txMap: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* txMap.init -- init of txmap on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxMapInit(NNP_Task nnpTask)
{
ENTRY("vTxMapInit");
  
  PtrVal xPtrTxMap;
  if ( ! nnpTask->bPopParam(xPtrTxMap) )
    THROW_INVALID_PARAM("txMap.init");
  
  TxMap* pTxMap = reinterpret_cast<TxMap*>(xPtrTxMap.pPtr());
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
  
  PtrVal xPtrTxRef;
  if ( ! nnpTask->bPopParam(xPtrTxRef) )
    THROW_INVALID_PARAM("txRef.init");
  
  TxRef* pTxRef = reinterpret_cast<TxRef*>(xPtrTxRef.pPtr());
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
  
  PtrVal xPtrTxRef;
  
  if ( ! nnpTask->bPopParam(xPtrTxRef) )
    THROW_INVALID_PARAM("tx.begin");
  
  TxRef* pTxRef = static_cast<TxRef*>(xPtrTxRef.pPtr());
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bBegun = pTxRef->bBeginTx();
  
  nnpTask->vPushParam(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.end -- end a tx (via a txRef on stack)                               */
/*----------------------------------------------------------------------------*/
void Task::vTxRefEnd(NNP_Task nnpTask)
{
ENTRY("vTxRefEnd");
  
  PtrVal xPtrTxRef;
  
  if ( ! nnpTask->bPopParam(xPtrTxRef) )
    THROW_INVALID_PARAM("txRef.end");
  
  TxRef* pTxRef = static_cast<TxRef*>(xPtrTxRef.pPtr());
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bEnded = pTxRef->bEndTx();
  
  nnpTask->vPushParam(bEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.commit -- commit a tx (via a txRef on stack)                         */
/*----------------------------------------------------------------------------*/
void Task::vTxRefCommit(NNP_Task nnpTask)
{
ENTRY("vTxRefCommit");
  
  PtrVal xPtrTxRef;
  
  if ( ! nnpTask->bPopParam(xPtrTxRef) )
    THROW_INVALID_PARAM("txRef.commit");
  
  TxRef* pTxRef = static_cast<TxRef*>(xPtrTxRef.pPtr());
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bCommitted = pTxRef->bCommitTx();
  
  nnpTask->vPushParam(bCommitted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* txRef.rollback -- rollback a tx (via a txRef on stack)                     */
/*----------------------------------------------------------------------------*/
void Task::vTxRefRollback(NNP_Task nnpTask)
{
ENTRY("vTxRefRollback");
  
  PtrVal xPtrTxRef;
  
  if ( ! nnpTask->bPopParam(xPtrTxRef) )
    THROW_INVALID_PARAM("txRef.rollback");
  
  TxRef* pTxRef = static_cast<TxRef*>(xPtrTxRef.pPtr());
  ASSERT(CN_TXREF == pTxRef->eClass());
  
  bool bRolledBack = pTxRef->bRollbackTx();
  
  nnpTask->vPushParam(bRolledBack);
  
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
  
  Dictionary* pDictionary;
  if (nnpTask->m_pSEitf)
    pDictionary = nnpTask->m_pSEitf->pDicStack()->pTop();
  else
    pDictionary = nnpTask->m_xDictionaryStack.pTop();
  
  new(pDictionary) Word(nnpTask->m_sToken.a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* >create -- create word in dictionary using string on stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCreateFrom(NNP_Task nnpTask)
{
ENTRY("vCreateFrom");

  s32 sName;
  if ( ! nnpTask->bPopParam(sName) )
    THROW_INVALID_PARAM(">create");
  
  Dictionary* pDictionary;
  if (nnpTask->m_pSEitf)
    pDictionary = nnpTask->m_pSEitf->pDicStack()->pTop();
  else
    pDictionary = nnpTask->m_xDictionaryStack.pTop();
  
  new(pDictionary) Word(sName.a());
  
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
  if ( ! nnpTask->bPopParam(sName) )
    THROW_INVALID_PARAM("find");
 
  if (nnpTask->m_bTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = nnpTask->pFindWord(sName);
  
  nnpTask->vPushParam(pWord);
  
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
/* >forget -- forget word on stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vWordForget(NNP_Task nnpTask)
{
ENTRY("vWordForget");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM(">forget");
  
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
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("word.dump");
  
  pWord->vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.dic -- push word's dictionary                                         */
/*----------------------------------------------------------------------------*/
void Task::vWordDic(NNP_Task nnpTask)
{
ENTRY("vWordDic");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("word.dic");
  
  Dictionary* pDictionary = pWord->pDictionary();
  
  nnpTask->vPushParam(pDictionary->sName().a());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindGet(NNP_Task nnpTask)
{
ENTRY("vWordKindGet");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("word.kind@");
    
  Word* pWordKind = pWord->pKind();
  
  nnpTask->vPushParam(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindSet(NNP_Task nnpTask)
{
ENTRY("vWordKindSet");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("word.kind!");
  
  Word* pWordKind;

  if ( ! nnpTask->bPopParam(pWordKind) )
    THROW_INVALID_PARAM("word.kind!");
  
  pWord->vKind(pWordKind);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.name -- push name of word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordName(NNP_Task nnpTask)
{
ENTRY("vWordName");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("word.name");
  
  char* pWordName = pWord->sName().a();
  
  nnpTask->vPushParam(pWordName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* word.this -- push current word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordThis(NNP_Task nnpTask)
{
ENTRY("vWordThis");
  
  Word* pWord = nnpTask->pCurrentWord();
  
  nnpTask->vPushParam(pWord);
  
  EXIT();
}

// sockets: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* accept a socket connection                                                 */
/*----------------------------------------------------------------------------*/
void Task::vSocketAccept(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  int     iSocketId;
  Object* pObject;
  
ENTRY("vSocketAccept");
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("socketaccept");
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  bool bResult = pSocket->bAcceptClient(iSocketId, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to accept connection: Errno %d!", xSysCall.iError());
  
  nnpTask->vPushParam(iSocketId);
  
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
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("socketclose");
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  bool bResult = pSocket->bClose(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to close socket: Errno %d!", xSysCall.iError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* connect to a server                                                        */
/*----------------------------------------------------------------------------*/
void Task::vSocketConnect(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  i8 iPort;
  
ENTRY("vSocketConnect");
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("socketconnect");

  if ( ! nnpTask->bPopParam(iPort) )
    THROW_INVALID_PARAM("socketconnect");
    
  //const char* sAddr = reinterpret_cast<char*>(pObject);
  //pSocket = new Socket;
  const char* sAddr = "127.0.0.1";
  pSocket = reinterpret_cast<Socket*>(pObject);
  bool bResult = pSocket->bConnectServer(sAddr, (int)iPort, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to connect: Errno %d!", xSysCall.iError());
  
  PtrVal xPtrSocket;
  xPtrSocket = (void*)pSocket;
  nnpTask->vPushParam(xPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* listen in for socket connections                                           */
/*----------------------------------------------------------------------------*/
void Task::vSocketListen(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  i8      iPort;
  
ENTRY("vSocketListen");
  
  if ( ! nnpTask->bPopParam(iPort) )
    THROW_INVALID_PARAM("socketlisten");
  
  pSocket = new Socket;
  bool bResult = pSocket->bCreateServer((int)iPort, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to listen for connections: Errno %d!", xSysCall.iError());
  
  PtrVal xPtrSocket;
  xPtrSocket = (void*)pSocket;
  nnpTask->vPushParam(xPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* open a socket                                                              */
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
  
  PtrVal xPtrSocket;
  xPtrSocket = (void*)pSocket;
  nnpTask->vPushParam(xPtrSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* send to a socket                                                           */
/*----------------------------------------------------------------------------*/
void Task::vSocketSend(NNP_Task nnpTask)
{
ENTRY("vSocketSend");

  SysCall xSysCall;
  Socket* pSocket;
  char* pData;
  Object* pObject;
  s8K  sWorkspace;
  ValWsp xValWsp;
  
  
  if ( ! nnpTask->bPopParam(xValWsp) )
    THROW_INVALID_PARAM("socketsend");
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("socketsend");
  
  xValWsp.vToString(sWorkspace.a());

  pSocket = reinterpret_cast<Socket*>(pObject);
  pData   = reinterpret_cast<char*>(sWorkspace.a());
  
  bool bResult = pSocket->bSendTo((int)String::iLength(pData), pData, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to sendTo socket: Errno %d!", xSysCall.iError());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* recv from a socket                                                         */
/*----------------------------------------------------------------------------*/
void Task::vSocketRecv(NNP_Task nnpTask)
{
  ENTRY("vSocketRecv");
  
  SysCall xSysCall;
  Socket* pSocket;
  ValWsp xValWsp;
  s8K   sWorkspace;
  i8 iSocketId;
  
  
  if ( ! nnpTask->bPopParam(iSocketId) )
    THROW_INVALID_PARAM("socketrecv");
  
  char* pData = reinterpret_cast<char*>(sWorkspace.a());
  pSocket = new Socket((int)iSocketId);
  bool bResult = pSocket->bReadVch(reinterpret_cast<char**>(&pData), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
  
  xValWsp = pData;
  nnpTask->vPushParam(xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* start a socket client                                                      */
/*----------------------------------------------------------------------------*/
void Task::vClient(NNP_Task nnpTask)
{
ENTRY("vClient");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  char* pAddress = pCurrentWord->pAllot(sizeof(Thread));

  Thread* pThread = reinterpret_cast<Thread*>(pAddress);
  pThread->vSetMagic();
  pThread->vClass(CN_THREAD);
  
  ASSERT(CN_THREAD == pThread->eClass());
  
  new(pThread) Thread;
  pThread->vSpawn();
  
  NNP_Task nnpTaskNew = new Task(nnpTask);
  nnpTaskNew->vSeedWord("client_code:");
  pThread->vAppend(nnpTaskNew);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* start a socket server                                                      */
/*----------------------------------------------------------------------------*/
void Task::vServer(NNP_Task nnpTask)
{
ENTRY("vServer");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  char* pAddress = pCurrentWord->pAllot(sizeof(Thread));
  
  Thread* pThread = reinterpret_cast<Thread*>(pAddress);
  pThread->vSetMagic();
  pThread->vClass(CN_THREAD);

  ASSERT(CN_THREAD == pThread->eClass());
  
  new(pThread) Thread;
  pThread->vSpawn();
  
  // FIX!!!
  //NNP_Task nnpTaskNew = new Task();
  //nnpTaskNew->vSeedWord("server_code:");
  //pThread->vAppend(nnpTaskNew);
  
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
  if ( ! nnpTask->bPopParam(iBytes) )
    THROW_INVALID_PARAM("bytes.magnitude");
  
  s256 sMag;
  Magnitudes::vMagnitude(iBytes, sMag.a());

  nnpTask->vPushParam(sMag);
  
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