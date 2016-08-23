/*============================================================================*/
/* Task.cpp - task                                                            */
/*============================================================================*/

#include "RapidsSE.hpp"
#include "Tasking.h"

extern Arguments             gxArguments;

extern AttachMap*            gpAttachMap;
extern BufferPool*           gpBufferPool;
extern Heap*                 gpMediumHeap;
extern BufferPoolAnalyzer*   gpBPA;
extern Dictionary*           gpDictionary;
extern Word*                 gpWord;

extern TaskList*             gpTaskList;
extern NP_Task               gnpRootTask;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Task::operator new(size_t iSize)
{
  ASSERT(gpTaskList);
  
  Task* pTask = (Task*)gpMediumHeap->pAllocObject();
  
  return pTask;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Task::operator new(size_t iSize, Task* pTask)
{
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
 _poolAddress_(this);
  _magic_();

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
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Task::Task(NNP_Task nnpTask)
: Object(CN_TASK)
{
  _poolAddress_(this);
  _magic_();
  
  ASSERT(gpTaskList);
  
  vInit("");
  
  Task* pTask = this;
  gpTaskList->vAppend(pTask);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Task::Task(const char* sCommand)
: Object(CN_TASK)
{
  _poolAddress_(this);
  _magic_();
  
  ASSERT(gpTaskList);

  _class_(CN_TASK);
  
  vInit(sCommand);
  
  Task* pTask = this;
  gpTaskList->vAppend(pTask);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Task::~Task(void)
{
  _poolAddress_(this);
  
  Task* pTask = this;
  gpTaskList->vRemove(pTask);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void Task::vInit(const char* aCommand)
{
  _poolAddress_(this);
  _magic_();
  
  _class_(CN_TASK);
  
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
  
  m_xDictionaryStack.vPush(gpDictionary);
  
  m_pQueue = nullptr;
  
  m_pCell      = nullptr;
  m_pResetIP   = nullptr;
  m_pBeginIP   = nullptr;
  m_bCompiling = false;
  m_bTracing   = false;
  
  m_sCommand = aCommand;
  m_eLiteral = LT_NONE;
  if (m_sCommand.iLength())
    String::vConcat(m_sCommand.a()," exit");
  m_xInputStream.vUseText(m_sCommand);
  
  m_sLiteral = "";
  
  m_pSEitf   = nullptr;
}

/*----------------------------------------------------------------------------*/
/* add constant                                                               */
/*----------------------------------------------------------------------------*/
void Task::vAddConstant(const char* aName, i8 iVal)
{
  Dictionary* pDic = m_xDictionaryStack.pTop();
  
  pDic->vAddConstant(aName,iVal);
}

/*----------------------------------------------------------------------------*/
/* add constant                                                               */
/*----------------------------------------------------------------------------*/
void Task::vAddConstant(const char* aName, const char* aValue)
{
  Dictionary* pDic = m_xDictionaryStack.pTop();
  
  pDic->vAddConstant(aName,aValue);
}

/*----------------------------------------------------------------------------*/
/* add primitive word into dictionary                                         */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitive(const char* pName, pFUN pFunc, bool bImmediate)
{
  Dictionary* pDic = m_xDictionaryStack.pTop();
  
  Word* pWord = new(pDic) Word(WT_PRIMITIVE,pFunc);
  
  pWord->vName(pName);
  pWord->vImmediate(bImmediate);
}

/*----------------------------------------------------------------------------*/
/* adjust next pointer                                                        */
/*----------------------------------------------------------------------------*/
void Task::vAdjustNext(i8 iBytes)
{
  m_pCell = reinterpret_cast<Cell*>(pcAdd(m_pCell,iBytes));
}

/*----------------------------------------------------------------------------*/
/* get Object at IP                                                           */
/*----------------------------------------------------------------------------*/
Object* Task::pArgObject(void)
{
  Object* pObject = reinterpret_cast<Object*>(m_pCell);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* add constants                                                              */
/*----------------------------------------------------------------------------*/
void Task::vAddConstants(void)
{
  vAddConstant("largebuffersize",  C_SIZE_LARGE_BUFFER);
  vAddConstant("mediumbuffersize", C_SIZE_MEDIUM_OBJECT);
  vAddConstant("smallbuffersize",  C_SIZE_SMALL_OBJECT);
  vAddConstant("magic",            MAGIC);
  vAddConstant("regionsize",       gxArguments.m_iRegionSize);
  vAddConstant("bgtasks",          gxArguments.m_bBgTasks);
  
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
  /*  2 */ vAddConstant("CN_IPVAL",                 CN_IPVAL);
  /*  3 */ vAddConstant("CN_OBJVAL",                CN_OBJVAL);
  /*  3 */ vAddConstant("CN_PTRVAL",                CN_PTRVAL);
  /*  4 */ vAddConstant("CN_STRVAL",                CN_STRVAL);
  /*  5 */ vAddConstant("CN_TASKVAL",               CN_TASKVAL);
  /*  6 */ vAddConstant("CN_VALSET",                CN_VALSET);
  /*  7 */ vAddConstant("CN_VALWSP",                CN_VALWSP);
  /*  8 */ vAddConstant("CN_WRDVAL",                CN_WRDVAL);
  
  /*  9 */ vAddConstant("CN_ARGUMENTS",             CN_ARGUMENTS);
  /* 10 */ vAddConstant("CN_ATTACHMAP",             CN_ATTACHMAP);
  /* 11 */ vAddConstant("CN_BUFFER",                CN_BUFFER);
  /* 12 */ vAddConstant("CN_BUFFERPOOL",            CN_BUFFERPOOL);
  /* 13 */ vAddConstant("CN_BUFFERPOOLANALYZER",    CN_BUFFERPOOLANALYZER);
  /* 14 */ vAddConstant("CN_BUFFERPOOLITER",        CN_BUFFERPOOLITER);
  /* 15 */ vAddConstant("CN_DICTIONARY",            CN_DICTIONARY);
  /* 16 */ vAddConstant("CN_DIR",                   CN_DIR);
  /* 17 */ vAddConstant("CN_FILE",                  CN_FILE);
  /* 18 */ vAddConstant("CN_FREECHUNK",             CN_FREECHUNK);
  /* 19 */ vAddConstant("CN_GLOBALDICTIONARY",      CN_GLOBALDICTIONARY);
  /* 20 */ vAddConstant("CN_GRIDMODEL",             CN_GRIDMODEL);
  /* 21 */ vAddConstant("CN_HEAPBUFFER",            CN_HEAPBUFFER);
  /* 22 */ vAddConstant("CN_INDEX",                 CN_INDEX);
  /* 23 */ vAddConstant("CN_LOADER",                CN_LOADER);
  /* 24 */ vAddConstant("CN_LOCK",                  CN_LOCK);
  /* 25 */ vAddConstant("CN_MEDIUMHEAP",            CN_MEDIUMHEAP);
  /* 26 */ vAddConstant("CN_NODE",                  CN_NODE);
  /* 27 */ vAddConstant("CN_PARTITION",             CN_PARTITION);
  /* 28 */ vAddConstant("CN_PROXY",                 CN_PROXY);
  /* 29 */ vAddConstant("CN_PROXYSET",              CN_PROXYSET);
  /* 30 */ vAddConstant("CN_QUEUE",                 CN_QUEUE);
  /* 31 */ vAddConstant("CN_QUEUEBUFFER",           CN_QUEUEBUFFER);
  /* 32 */ vAddConstant("CN_ROW",                   CN_ROW);
  /* 32 */ vAddConstant("CN_ROWBUFFER",             CN_ROWBUFFER);
  /* 33 */ vAddConstant("CN_ROWCURSOR",             CN_ROWCURSOR);
  /* 32 */ vAddConstant("CN_ROWSTRAND",             CN_ROWSTRAND);
  /* 34 */ vAddConstant("CN_SCANBUFFER",            CN_SCANBUFFER);
  /* 35 */ vAddConstant("CN_SEITF",                 CN_SEITF);
  /* 36 */ vAddConstant("CN_SEITFCURSOR",           CN_SEITFCURSOR);
  /* 37 */ vAddConstant("CN_SEITFLIST",             CN_SEITFLIST);
  /* 38 */ vAddConstant("CN_SMALLHEAP",             CN_SMALLHEAP);
  /* 39 */ vAddConstant("CN_SOCKET",                CN_SOCKET);
  /* 40 */ vAddConstant("CN_STACK",                 CN_STACK);
  /* 41 */ vAddConstant("CN_SYSCALL",               CN_SYSCALL);
  /* 42 */ vAddConstant("CN_TABLE",                 CN_TABLE);
  /* 43 */ vAddConstant("CN_TASK",                  CN_TASK);
  /* 44 */ vAddConstant("CN_TASKCURSOR",            CN_TASKCURSOR);
  /* 45 */ vAddConstant("CN_TASKLIST",              CN_TASKLIST);
  /* 46 */ vAddConstant("CN_TASKSET",               CN_TASKSET);
  /* 47 */ vAddConstant("CN_THREAD",                CN_THREAD);
  /* 48 */ vAddConstant("CN_THREADLIST",            CN_THREADLIST);
  /* 49 */ vAddConstant("CN_TIMER",                 CN_TIMER);
  /* 50 */ vAddConstant("CN_TX",                    CN_TX);
  /* 51 */ vAddConstant("CN_TXLIST",                CN_TXLIST);
  /* 52 */ vAddConstant("CN_TXMAP",                 CN_TXMAP);
  /* 53 */ vAddConstant("CN_WORD",                  CN_WORD);
  /* 54 */ vAddConstant("CN_ENUMS",                 CN_ENUMS);

  /*  0 */ vAddConstant("LT_NONE",                  LT_NONE);
  /*  1 */ vAddConstant("LT_NAMELIT",               LT_NAMELIT);
  /*  2 */ vAddConstant("LT_INTLIT",                LT_INTLIT);
  /*  3 */ vAddConstant("LT_STRLIT",                LT_STRLIT);
  /*  4 */ vAddConstant("LT_ENUMS",                 LT_ENUMS);
  
  /*  1 */ vAddConstant("SIZEOF_INTVAL",            sizeof(IntVal));
  /*  2 */ vAddConstant("SIZEOF_IPVAL",             sizeof(IPval));
  /*  3 */ vAddConstant("SIZEOF_OBJVAL",            sizeof(ObjVal));
  /*  3 */ vAddConstant("SIZEOF_PTRVAL",            sizeof(PtrVal));
  /*  4 */ vAddConstant("SIZEOF_STRVAL",            sizeof(StrVal));
  /*  5 */ vAddConstant("SIZEOF_TASKVAL",           sizeof(TaskVal));
  /*  6 */ vAddConstant("SIZEOF_VALSET",            sizeof(ValWsp)); // correct
  /*  7 */ vAddConstant("SIZEOF_VALWSP",            sizeof(ValWsp));
  /*  8 */ vAddConstant("SIZEOF_WRDVAL",            sizeof(WrdVal));
  
  /*  9 */ vAddConstant("SIZEOF_ARGUMENTS",         sizeof(Arguments));
  /* 10 */ vAddConstant("SIZEOF_ATTACHMAP",         sizeof(AttachMap));
  /* 11 */ vAddConstant("SIZEOF_BUFFER",            sizeof(Buffer));
  /* 12 */ vAddConstant("SIZEOF_BUFFERPOOL",        sizeof(BufferPool));
  /* 13 */ vAddConstant("SIZEOF_BUFFERPOOLANALYZER",sizeof(BufferPool));
  /* 14 */ vAddConstant("SIZEOF_BUFFERPOOLITER",    sizeof(BufferPoolAnalyzer));
  /* 15 */ vAddConstant("SIZEOF_DICTIONARY",        sizeof(Dictionary));
  /* 16 */ vAddConstant("SIZEOF_DIR",               sizeof(Dir));
  /* 17 */ vAddConstant("SIZEOF_FILE",              sizeof(File));
  /* 18 */ vAddConstant("SIZEOF_FREECHUNK",         sizeof(FreeChunk));
  /* 19 */ vAddConstant("SIZEOF_GLOBALDICTIONARY",  sizeof(GlobalDictionary));
  /* 20 */ vAddConstant("SIZEOF_GRIDMODEL",         sizeof(GridModel));
  /* 21 */ vAddConstant("SIZEOF_HEAP",              sizeof(Heap));
  /* 22 */ vAddConstant("SIZEOF_INDEX",             sizeof(Index));
  /* 23 */ vAddConstant("SIZEOF_LOADER",            sizeof(Loader));
  /* 24 */ vAddConstant("SIZEOF_LOCK",              sizeof(Lock));
  /* 25 */ vAddConstant("SIZEOF_NODE",              sizeof(Node));
  /* 26 */ vAddConstant("SIZEOF_PARTITION",         sizeof(Partition));
  /* 27 */ vAddConstant("SIZEOF_PROXY",             sizeof(Proxy));
  /* 28 */ vAddConstant("SIZEOF_PROXYSET",          sizeof(ProxySet));
  /* 29 */ vAddConstant("SIZEOF_QBUFFER",           sizeof(QueueBuffer));
  /* 30 */ vAddConstant("SIZEOF_QUEUE",             sizeof(Queue));
  /* 31 */ vAddConstant("SIZEOF_ROW",               sizeof(Row));
  /* 32 */ vAddConstant("SIZEOF_ROWBUFFER",         sizeof(RowBuffer));
  /* 33 */ vAddConstant("SIZEOF_ROWCURSOR",         sizeof(RowCursor));
  /* 34 */ vAddConstant("SIZEOF_ROWSTRAND",         sizeof(RowStrand));
  /* 35 */ vAddConstant("SIZEOF_SCANBUFFER",        sizeof(ScanBuffer));
  /* 36 */ vAddConstant("SIZEOF_SEITF",             sizeof(SEitf));
  /* 37 */ vAddConstant("SIZEOF_SEITFCURSOR",       sizeof(SEitfCursor));
  /* 38 */ vAddConstant("SIZEOF_SEITFLIST",         sizeof(SEitfList));
  /* 39 */ vAddConstant("SIZEOF_SOCKET",            sizeof(Socket));
  /* 40 */ vAddConstant("SIZEOF_STACK",             sizeof(Stack));
  /* 41 */ vAddConstant("SIZEOF_SYSCALL",           sizeof(SysCall));
  /* 42 */ vAddConstant("SIZEOF_TABLE",             sizeof(Table));
  /* 43 */ vAddConstant("SIZEOF_TASK",              sizeof(Task));
  /* 44 */ vAddConstant("SIZEOF_TASKCURSOR",        sizeof(TaskCursor));
  /* 45 */ vAddConstant("SIZEOF_TASKLIST",          sizeof(TaskSet));
  /* 46 */ vAddConstant("SIZEOF_TASKSET",           sizeof(TaskSet));
  /* 47 */ vAddConstant("SIZEOF_THREAD",            sizeof(Thread));
  /* 48 */ vAddConstant("SIZEOF_THREADLIST",        sizeof(ThreadList));
  /* 49 */ vAddConstant("SIZEOF_TIMER",             sizeof(Timer));
  /* 50 */ vAddConstant("SIZEOF_TX",                sizeof(Tx));
  /* 51 */ vAddConstant("SIZEOF_TXLIST",            sizeof(TxList));
  /* 52 */ vAddConstant("SIZEOF_TXMAP",             sizeof(TxMap));
  /* 53 */ vAddConstant("SIZEOF_TXSET",             sizeof(TxList));
  /* 54 */ vAddConstant("SIZEOF_WORD",              sizeof(Word));
  
  /*  0 */ vAddConstant("SIZEOF_S32",               sizeof(s32));
  /*  1 */ vAddConstant("SIZEOF_S256",              sizeof(s256));
  /*  2 */ vAddConstant("SIZEOF_S16K",              sizeof(s16K));

  /*  0 */ vAddConstant("TS_NONE",                  TS_NONE);
  /*  1 */ vAddConstant("TS_RUNNING",               TS_RUNNING);
  /*  2 */ vAddConstant("TS_SWITCH",                TS_SWITCH);
  /*  3 */ vAddConstant("TS_WAITING",               TS_WAITING);
  /*  5 */ vAddConstant("TS_SLEEPING",              TS_SLEEPING);
  /*  6 */ vAddConstant("TS_BLOCKED",               TS_BLOCKED);
  /*  7 */ vAddConstant("TS_EXITED",                TS_EXITED);
  /*  8 */ vAddConstant("TS_ENUMS",                 TS_ENUMS);

  /*  0 */ vAddConstant("TP_FREE",                  TP_FREE);
  /*  1 */ vAddConstant("TP_ALLOCATED",             TP_ALLOCATED);
  /*  2 */ vAddConstant("TP_EXECUTING",             TP_EXECUTING);
  /*  3 */ vAddConstant("TP_COMMITTING",            TP_COMMITTING);
  /*  5 */ vAddConstant("TP_COMMITTED",             TP_COMMITTED);
  /*  6 */ vAddConstant("TP_PREPARING",             TP_PREPARING);
  /*  7 */ vAddConstant("TP_PREPARED",              TP_PREPARED);
  /*  8 */ vAddConstant("TP_ROLLINGBACK",           TP_ROLLINGBACK);
  /*  9 */ vAddConstant("TP_ROLLEDBACK",            TP_ROLLEDBACK);
  /* 10 */ vAddConstant("TP_ENUMS",                 TP_ENUMS);

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
  
  vAddConstant("listerTaskPrint",false);
  vAddConstant("monitorTaskPrint",false);
  vAddConstant("reclaimTaskPrint",false);
}

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives(void)
{
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
  vAddPrimitive("class@",          &vClassFetch,       false);
  vAddPrimitive("class!",          &vClassStore,       false);
  vAddPrimitive("clip",            &vClip,             false);
  vAddPrimitive("code=",           &vCodeAssign,       false);
  vAddPrimitive(":",               &vColon,            false);
  vAddPrimitive(",",               &vComma,            false);
  vAddPrimitive("(",               &vComment,          true );
  vAddPrimitive("compiling",       &vCompiling,        false);
  vAddPrimitive("concat",          &vConcat,           false);
  vAddPrimitive("create",          &vCreate,           false);
  vAddPrimitive(">create",         &vCreateFrom,       false);
  vAddPrimitive("cursor@",         &vCursorFetch,      false);
  vAddPrimitive("cursor!",         &vCursorStore,      false);
  vAddPrimitive("cwd",             &vCwd,              false);
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
  vAddPrimitive("gwd",             &vGwd,              false);
  vAddPrimitive("i",               &vI,                false);
  vAddPrimitive("if",              &vIf,               true );
  vAddPrimitive("immediate",       &vImmediate,        true );
  vAddPrimitive("input.close",     &vInputClose,       false);
  vAddPrimitive("input.depth",     &vInputDepth,       false);
  vAddPrimitive("input.open",      &vInputOpen,        false);
  vAddPrimitive("input.pop",       &vInputPop,         false);
  vAddPrimitive("input.push",      &vInputPush,        false);
  vAddPrimitive("input.stdin",     &vInputStdin,       false);
  vAddPrimitive("input.text",      &vInputText,        false);
  vAddPrimitive("interpreting",    &vInterpreting,     false);
  vAddPrimitive("isBooted",        &vIsBooted,         false);
  vAddPrimitive("isCompiling",     &vIsCompiling,      false);
  vAddPrimitive("isIntLit",        &vIsIntLit,         false);
  vAddPrimitive("isImmediate",     &vIsImmediate,      false);
  vAddPrimitive("isInterpreting",  &vIsInterpreting,   false);
  vAddPrimitive("isNameLit",       &vIsNameLit,        false);
  vAddPrimitive("isPrimitive",     &vIsPrimitive,      false);
  vAddPrimitive("isStrLit",        &vIsStrLit,         false);
  vAddPrimitive("iterate",         &vIterate,          true );
  vAddPrimitive("iterate_rt_A",    &vIterate_RT_A,     false);
  vAddPrimitive("iterate_rt_B",    &vIterate_RT_B,     false);
  vAddPrimitive("j",               &vJ,                false);
  vAddPrimitive("jump",            &vJump,             false);
  vAddPrimitive("jumpFalse",       &vJumpFalse,        false);
  vAddPrimitive("jumpTrue",        &vJumpTrue,         false);
  vAddPrimitive("<",               &vLess,             false);
  vAddPrimitive("<=",              &vLessEqual,        false);
  vAddPrimitive("literal",         &vLiteral,          false);
  vAddPrimitive("literal>",        &vLiteralGet,       false);
  vAddPrimitive("litNext",         &vLitNext,          false);
  vAddPrimitive("litType",         &vLitType,          false);
  vAddPrimitive("locate",          &vLocate,           false);
  vAddPrimitive("loop",            &vLoop,             true );
  vAddPrimitive("loop_rt",         &vLoop_RT,          false);
  vAddPrimitive("+loop",           &vLoopPlus,         true );
  vAddPrimitive("+loop_rt",        &vLoopPlus_RT,      false);
  vAddPrimitive("magic@",          &vMagicFetch,       false);
  vAddPrimitive("magic!!",         &vMagicMark,        false);
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
  vAddPrimitive("processExit",     &vProcessExit,      false);
  vAddPrimitive("push",            &vPush,             false);
  vAddPrimitive(".put",            &vPut,              false);
  vAddPrimitive(".\\n",            &vPutNewline,       false);
  vAddPrimitive("quit",            &vQuit,             false);
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
  vAddPrimitive("streql",          &vStrEql,           false);
  vAddPrimitive("strlen",          &vStrLen,           false);
  vAddPrimitive("subStr",          &vSubStr,           false);
  vAddPrimitive("-",               &vSubtract,         false);
  vAddPrimitive("swap",            &vSwap,             false);
  vAddPrimitive("switch",          &vSwitch,           false);
  vAddPrimitive("test",            &vTest,             false);
  vAddPrimitive("then",            &vThen,             true );
  vAddPrimitive("throw",           &vThrow,            false);
  vAddPrimitive("'",               &vTick,             true );
  vAddPrimitive(">body",           &vToBody,           false);
  vAddPrimitive(">cmd",            &vToCmd,            false);
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
  
  vAddPrimitive("args.print",      &vArgsPrint,        false);
  
  vAddPrimitive("csv.firstValue",  &vCsvFirstValue,    false);
  
  vAddPrimitive("dic.count",       &vDicCount,         false);
  vAddPrimitive("dic.drop",        &vDicDrop,          false);
  vAddPrimitive("}}",              &vDicDrop,          false);
  vAddPrimitive("dic.init",        &vDicInit,          false);
  vAddPrimitive("dic.last",        &vDicLast,          false);
  vAddPrimitive("dic.main",        &vDicMain,          false);
  vAddPrimitive("dic.name",        &vDicName,          false);
  vAddPrimitive("dic.pick",        &vDicPick,          false);
  vAddPrimitive("dic.push",        &vDicPush,          false);
  vAddPrimitive("{{",              &vDicPush,          false);
  vAddPrimitive("dic.stack",       &vDicStack,         false);
  
  vAddPrimitive("file.close",      &vFileClose,        false);
  vAddPrimitive("file.delete",     &vFileDelete,       false);
  vAddPrimitive("file.init",       &vFileInit,         false);
  vAddPrimitive("file.new",        &vFileNew,          false);
  vAddPrimitive("file.open",       &vFileOpen,         false);
  vAddPrimitive("file.path@",      &vFilePathGet,      false);
  vAddPrimitive("file.path!",      &vFilePathSet,      false);
  vAddPrimitive("file.read",       &vFileRead,         false);
  vAddPrimitive("file.write",      &vFileWrite,        false);
  
  vAddPrimitive("loader.new",         &vLoaderNew,         false);
  vAddPrimitive("loader.delete",      &vLoaderDelete,      false);
  vAddPrimitive("loader.init",        &vLoaderInit,        false);
  vAddPrimitive("loader.table!",      &vLoaderTableStore,  false);
  vAddPrimitive("loader.path!",       &vLoaderPathStore,   false);
  vAddPrimitive("loader.openFile",    &vLoaderOpenFile,    false);
  vAddPrimitive("loader.begin",       &vLoaderBegin,       false);
  vAddPrimitive("loader.fetchRecord", &vLoaderFetchRecord, false);
  vAddPrimitive("loader.insertRow",   &vLoaderInsertRow,   false);
  vAddPrimitive("loader.end",         &vLoaderEnd,         false);
  vAddPrimitive("loader.closeFile",   &vLoaderCloseFile,   false);

  vAddPrimitive("lock.dropread",   &vLockDropRead,     false);
  vAddPrimitive("lock.dropwrite",  &vLockDropWrite,    false);
  vAddPrimitive("lock.grabread",   &vLockGrabRead,     false);
  vAddPrimitive("lock.grabwrite",  &vLockGrabWrite,    false);
  vAddPrimitive("lock.init",       &vLockInit,         false);
  
  vAddPrimitive("process.getpid",  &vProcessGetPid,    false);
  vAddPrimitive("process.testpid", &vProcessTestPid,   false);
  
  vAddPrimitive("pstack.class",    &vPstackClass,      false);
  vAddPrimitive("p.depth",         &vPstackDepth,      false);
  
  vAddPrimitive("queue.append",    &vQueueAppend,      false);
  vAddPrimitive("queue.dump",      &vQueueDump,        false);
  vAddPrimitive("queue.init",      &vQueueInit,        false);
  vAddPrimitive("queue.pop",       &vQueuePop,         false);
  
  vAddPrimitive("rowcursor.begin",  &vRowCursorBegin,  false);
  vAddPrimitive("rowcursor.delete", &vRowCursorDelete, false);
  vAddPrimitive("rowcursor.end",    &vRowCursorEnd,    false);
  vAddPrimitive("rowcursor.first",  &vRowCursorFirst,  false);
  vAddPrimitive("rowcursor.get",    &vRowCursorGet,    false);
  vAddPrimitive("rowcursor.init",   &vRowCursorInit,   false);
  vAddPrimitive("rowcursor.new",    &vRowCursorNew,    false);
  vAddPrimitive("rowcursor.next",   &vRowCursorNext,   false);
  vAddPrimitive("rowcursor.table@", &vRowCursorTableFetch, false);
  vAddPrimitive("rowcursor.table!", &vRowCursorTableStore, false);
  
  vAddPrimitive("rstack.depth",    &vRstackDepth,      false);
  vAddPrimitive("rdrop",           &vRstackDrop,       false);
  vAddPrimitive("r@",              &vRstackFetch,      false);
  vAddPrimitive("rpick",           &vRstackPick,       false);
  
  vAddPrimitive("seitf.delete",    &vSEitfDelete,      false);
  vAddPrimitive("seitf.dump",      &vSEitfDump,        false);
  vAddPrimitive("seitf.new",       &vSEitfNew,         false);
  vAddPrimitive("seitf.req@",      &vSEitfReqFetch,    false);
  vAddPrimitive("seitf.req!",      &vSEitfReqStore,    false);
  vAddPrimitive("seitf.rsp@",      &vSEitfRspFetch,    false);
  vAddPrimitive("seitf.rsp!",      &vSEitfRspStore,    false);
  
  vAddPrimitive("seitfcursor.begin",  &vSEitfCursorBegin,  false);
  vAddPrimitive("seitfcursor.delete", &vSEitfCursorDelete, false);
  vAddPrimitive("seitfcursor.end",    &vSEitfCursorEnd,    false);
  vAddPrimitive("seitfcursor.first",  &vSEitfCursorFirst,  false);
  vAddPrimitive("seitfcursor.get",    &vSEitfCursorGet,    false);
  vAddPrimitive("seitfcursor.init",   &vSEitfCursorInit,   false);
  vAddPrimitive("seitfcursor.new",    &vSEitfCursorNew,    false);
  vAddPrimitive("seitfcursor.next",   &vSEitfCursorNext,   false);
  
  vAddPrimitive("seitflist.dump",  &vSEitfListDump,    false);
  vAddPrimitive("seitflist.init",  &vSEitfListInit,    false);
  
  vAddPrimitive("table.copy",        &vTableCopy,        false);
  vAddPrimitive("table.dic",         &vTableDic,         false);
  vAddPrimitive("table.drop",        &vTableDrop,        false);
  vAddPrimitive("table.export",      &vTableExport,      false);
  vAddPrimitive("table.init",        &vTableInit,        false);
  vAddPrimitive("table.insert",      &vTableInsert,      false);
  vAddPrimitive("table.load",        &vTableLoad,        false);
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
  
  vAddPrimitive("taskcursor.begin",  &vTaskCursorBegin,  false);
  vAddPrimitive("taskcursor.delete", &vTaskCursorDelete, false);
  vAddPrimitive("taskcursor.end",    &vTaskCursorEnd,    false);
  vAddPrimitive("taskcursor.first",  &vTaskCursorFirst,  false);
  vAddPrimitive("taskcursor.get",    &vTaskCursorGet,    false);
  vAddPrimitive("taskcursor.init",   &vTaskCursorInit,   false);
  vAddPrimitive("taskcursor.new",    &vTaskCursorNew,    false);
  vAddPrimitive("taskcursor.next",   &vTaskCursorNext,   false);
  
  vAddPrimitive("tasklist",        &vTaskList,         false);
  vAddPrimitive("tasklist.append", &vTaskListAppend,   false);
  vAddPrimitive("tasklist.dump",   &vTaskListDump,     false);
  vAddPrimitive("tasklist.init",   &vTaskListInit,     false);
  
  vAddPrimitive("taskset.append",  &vTaskSetAppend,    false);
  vAddPrimitive("taskset.dump",    &vTaskSetDump,      false);
  vAddPrimitive("taskset.init",    &vTaskSetInit,      false);
  
  vAddPrimitive("thread.append",   &vThreadAppend,     false);
  vAddPrimitive("thread.bind",     &vThreadBind,       false);
  vAddPrimitive("thread.cpu",      &vThreadCpu,        false);
  vAddPrimitive("thread.dump",     &vThreadDump,       false);
  vAddPrimitive("thread.init",     &vThreadInit,       false);
  vAddPrimitive("thread.name@",    &vThreadNameFetch,  false);
  vAddPrimitive("thread.name!",    &vThreadNameStore,  false);
  vAddPrimitive("thread.taskset@", &vThreadSfetch,     false);
  vAddPrimitive("thread.taskset!", &vThreadSstore,     false);
  vAddPrimitive("thread.sleep_s",  &vThreadSleep_s,    false);
  vAddPrimitive("thread.sleep_ms", &vThreadSleep_ms,   false);
  vAddPrimitive("thread.sleep_us", &vThreadSleep_us,   false);
  vAddPrimitive("thread.sleep_ns", &vThreadSleep_ns,   false);
  vAddPrimitive("thread.spawn",    &vThreadSpawn,      false);
  
  vAddPrimitive("threadlist.dump", &vThreadListDump,   false);
  vAddPrimitive("threadlist.init", &vThreadListInit,   false);
  
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
  
  vAddPrimitive("tx.delete",       &vTxDelete,         false);
  vAddPrimitive("tx.new",          &vTxNew,            false);
  vAddPrimitive("tx.begin",        &vTxBegin,          false);
  vAddPrimitive("tx.commit",       &vTxCommit,         false);
  vAddPrimitive("tx.rollback",     &vTxRollback,       false);
  
  vAddPrimitive("txList.dump",     &vTxListDump,       false);
  vAddPrimitive("txList.init",     &vTxListInit,       false);
  
  vAddPrimitive("txMap.init",      &vTxMapInit,        false);
  
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
}

/*----------------------------------------------------------------------------*/
/* add initial set of routines into dictionary (min set to boot)              */
/*----------------------------------------------------------------------------*/
void Task::vAddRoutines(void)
{
  s16K sDefinition;

  sDefinition =
  ": docol "
  
  "  retryPoint"
  
  "  begin_"
  "    litNext"
  
  // end of input
  "    litType LT_NONE == if"
  "      exit"
  "    then"
  
  // integer
  "    litType LT_INTLIT == if"
  
  "      literal >int"            // convert str to int, leave on stack
  
  "      isinterpreting if"       // interpret leaves int on stack
  "        again_"
  "      then"
  
  "      iscompiling if"          // compile encodes push intval
  "        encodeVal"
  "        again_"
  "      then"
  
  "    then"
  
  // string
  "    litType LT_STRLIT == if"
  
  "      literal"                 // put str on stack
  
  "      isinterpreting if"       // interpret leaves str on stack
  "        again_"
  "      then"
  
  "      iscompiling if"          // compile encodes push strval
  "        encodeVal"
  "        again_"
  "      then"
  
  "    then"
  
  // what?
  "    litType LT_NAMELIT != if"           // double check we have a name
  "      \"strange literal\" . litType ."
  "      again_"
  "    then"
  
  // known name?
  "    literal find"              // attempt to find
  "    dup not if"
  "      drop"
  "      \"unknown\" ."
  "      literal ."
  "      isBooted not if processExit then "
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
  sDefinition =
  ": ::dicInitialise"
  "  \"::dicInitialise\" ."
  "  gwd ."
  "  \"../../../commands\" cwd"
  "  gwd ."
  "  \"dicInitialise.sen\" >>:"
  "  \"::dicInitialise exit\" ."
  "  ::exit"
  "  ;";
  
  pCompileColon(sDefinition);
}

/*----------------------------------------------------------------------------*/
/* checked this tx locked                                                     */
/*----------------------------------------------------------------------------*/
void Task::vCheckLocked(void)
{
  m_xLock.vCheckWrite();
}

/*----------------------------------------------------------------------------*/
/* compile given : routine                                                    */
/*----------------------------------------------------------------------------*/
Word* Task::pCompileColon(s& sDefinition)
{
  Word* pNewWord;
  pFUN  pFunc;
  Word* pFoundWord;
  ValWsp xValWsp;
  
  m_xInputStream.vUseText(sDefinition);
  
  // get :
  vLiteralNext();
  
  // get name of word
  vLiteralNext();
  
  pNewWord = new(m_xDictionaryStack.pTop()) Word(m_sLiteral.a());
  
  vLiteralNext();
  
  while (LT_NONE != m_eLiteral) {
    
    if (String::bEqualTo(";",m_sLiteral.a()))
      break;
    
    switch (m_eLiteral) {
        
      case LT_INTLIT:
        xValWsp = String::iToI8(m_sLiteral.a());
        pNewWord->vEncode("push",&xValWsp);
        break;
        
      case LT_NAMELIT:
        pFoundWord = pFindWord(m_sLiteral);
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
          ASSERT(0);
        }
        break;
        
      case LT_STRLIT: {
        xValWsp = m_sLiteral.a();
        pNewWord->vEncode("push",&xValWsp);
        break;
      }
        
      default:
        ASSERT(0);
    }
    
    vLiteralNext();
  }
  
  pNewWord->vEncode("exit");
  

  // reset scanning
  m_xInputStream.vReset();
  
  return pNewWord;
}

/*----------------------------------------------------------------------------*/
/* current word                                                               */
/*----------------------------------------------------------------------------*/
Word* Task::pCurrentWord(void)
{
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
void Task::vDump(void)
{
  _poolAddress_(this);
  
  vTrace("Tx %p: %16s TaskNo %02lld State %d %s TxId %lld \n",
         this,
         m_sName.a(),
         m_iTaskNo,
         m_eState,
         pTS[m_eState],
         m_iTxId
         );
}

/*----------------------------------------------------------------------------*/
/* find word given name                                                       */
/*----------------------------------------------------------------------------*/
Word* Task::pFindWord(s& sName)
{
  _poolAddress_(this);
  
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
  _poolAddress_(this);
  
  Word* pWord = m_xDictionaryStack.pFind(aName);
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* forget word given name                                                     */
/*----------------------------------------------------------------------------*/
bool Task::bForgetWord(s& sName)
{
  _poolAddress_(this);
  
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
bool Task::bGuardParam(i8 iCount) const
{
  _poolAddress_(this);
  
  if (m_xParamStack.iValues() < iCount)
    return false;
  
  return true;
}



/*----------------------------------------------------------------------------*/
/* guard parameter stack 1                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bGuardParam(eCN eClass0) const
{
  _poolAddress_(this);
  
  if (m_xParamStack.iValues() < 1)
    return false;
  
  OnStack* pValue0 = m_xParamStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard parameter stack 2                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bGuardParam(eCN eClass0, eCN eClass1) const
{
  _poolAddress_(this);
  
  if (m_xParamStack.iValues() < 2)
    return false;
  
  OnStack* pValue0 = m_xParamStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  OnStack* pValue1 = m_xParamStack.pPeek(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard parameter stack 3                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bGuardParam(eCN eClass0, eCN eClass1, eCN eClass2) const
{
  _poolAddress_(this);
  
  if (m_xParamStack.iValues() < 3)
    return false;
  
  OnStack* pValue0 = m_xParamStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  OnStack* pValue1 = m_xParamStack.pPeek(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  OnStack* pValue2 = m_xParamStack.pPeek(2);
  if (eClass2 != pValue2->eClass())
    return false;
  
  return true;
}



/*----------------------------------------------------------------------------*/
/* guard return stack                                                         */
/*----------------------------------------------------------------------------*/
bool Task::bGuardReturn(i8 iCount) const
{
  _poolAddress_(this);
  
  if (m_xReturnStack.iValues() < iCount)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard return stack 1                                                       */
/*----------------------------------------------------------------------------*/
bool Task::bGuardReturn(eCN eClass0) const
{
  _poolAddress_(this);
  
  if (m_xReturnStack.iValues() < 1)
    return false;
  
  OnStack* pValue0 = m_xReturnStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard return stack 2                                                       */
/*----------------------------------------------------------------------------*/
bool Task::bGuardReturn(eCN eClass0, eCN eClass1) const
{
  _poolAddress_(this);
  
  if (m_xReturnStack.iValues() < 2)
    return false;
  
  OnStack* pValue0 = m_xReturnStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  OnStack* pValue1 = m_xReturnStack.pPeek(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* guard return stack 3                                                       */
/*----------------------------------------------------------------------------*/
bool Task::bGuardReturn(eCN eClass0, eCN eClass1, eCN eClass2) const
{
  _poolAddress_(this);
  
  if (m_xReturnStack.iValues() < 3)
    return false;
  
  OnStack* pValue0 = m_xReturnStack.pPeek(0);
  if (eClass0 != pValue0->eClass())
    return false;
  
  OnStack* pValue1 = m_xReturnStack.pPeek(1);
  if (eClass1 != pValue1->eClass())
    return false;
  
  OnStack* pValue2 = m_xReturnStack.pPeek(2);
  if (eClass2 != pValue2->eClass())
    return false;
  
  return true;
}







/*----------------------------------------------------------------------------*/
/* add initial set of words into dictionary                                   */
/*----------------------------------------------------------------------------*/
void Task::vInitialiseDictionary(void)
{
  vAddPrimitives();

  vAddConstants();
  
  vAddRoutines();
}

/*----------------------------------------------------------------------------*/
/* is given name?                                                             */
/*----------------------------------------------------------------------------*/
bool Task::bIsName(s& sName)
{
  _poolAddress_(this);
  
  bool b = m_sName.bIsEqual(sName,C_NO_CASE);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get next input literal                                                     */
/*----------------------------------------------------------------------------*/
void Task::vLiteralNext(void)
{
  m_eLiteral = m_xInputStream.eNextLiteral(m_sLiteral.a());
}

/*----------------------------------------------------------------------------*/
/* locate word based on cell                                                  */
/*----------------------------------------------------------------------------*/
Word* Task::pLocateWord(Cell* pCell)
{
  Word* pWord = m_xDictionaryStack.pLocate(pCell);
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* get name of this task                                                      */
/*----------------------------------------------------------------------------*/
s& Task::sName(void)
{
  _poolAddress_(this);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name of this task                                                      */
/*----------------------------------------------------------------------------*/
void Task::vName(s& sName)
{
  _poolAddress_(this);
  
  m_sName = sName;
}

/*----------------------------------------------------------------------------*/
/* get next task in set                                                       */
/*----------------------------------------------------------------------------*/
NP_Task Task::npNextInSet(void)
{
  return m_npNextInSet;
}

/*----------------------------------------------------------------------------*/
/* set next task in set                                                       */
/*----------------------------------------------------------------------------*/
void Task::vNextInSet(NP_Task npNextInSet)
{
  m_npNextInSet = npNextInSet;
}


/*----------------------------------------------------------------------------*/
/* seed task to run ::dicInitialise                                           */
/*----------------------------------------------------------------------------*/
void Task::vSeedDicInitialiseSen(void)
{
  Word* pBootSen = pFindWord("::dicInitialise");
  ASSERT(pBootSen);
  
  m_pCell = pBootSen->pFirstCell();
  
  m_eState = TS_RUNNING;
  
  m_xParamStack.vReset();
  m_xReturnStack.vReset();
  
  m_sCommand = "";
  m_xInputStream.vUseText(m_sCommand);
}

/*----------------------------------------------------------------------------*/
/* seed task to run given command                                             */
/*----------------------------------------------------------------------------*/
void Task::vSeedWord(const char* aCommand)
{
  Word* pShellEnter = pFindWord("::docmd");
  
  m_pCell = pShellEnter->pFirstCell();
  
  m_eState = TS_SWITCH;
  
  m_xParamStack.vReset();
  m_xReturnStack.vReset();
  
  m_sCommand = aCommand;
  if (m_sCommand.iLength())
    String::vConcat(m_sCommand.a()," ::exit");
  m_xInputStream.vUseText(m_sCommand);
  
}

/*----------------------------------------------------------------------------*/
/* seed task to run ::enter                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSeedDocol(void)
{
  Word* pShellEnter = pFindWord("::enter");
  
  m_pCell = pShellEnter->pFirstCell();
  
  m_eState = TS_SWITCH;
  
  m_xParamStack.vReset();
  m_xReturnStack.vReset();
  
  m_sCommand = "";
  m_xInputStream.vUseText(m_sCommand);
}

/*----------------------------------------------------------------------------*/
/* get prev task in set                                                       */
/*----------------------------------------------------------------------------*/
NP_Task Task::npPrevInSet(void)
{
  return m_npPrevInSet;
}

/*----------------------------------------------------------------------------*/
/* set prev task in set                                                       */
/*----------------------------------------------------------------------------*/
void Task::vPrevInSet(NP_Task npPrevInSet)
{
  m_npPrevInSet = npPrevInSet;
}

/*----------------------------------------------------------------------------*/
/* primitive entry                                                            */
/*----------------------------------------------------------------------------*/
void Task::vPrimitive(const char* aPrimitive)
{
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
    else if (CN_WRDVAL == eClass) {
      WrdVal* pWrdVal = reinterpret_cast<WrdVal*>(pValWsp);
      vTrace("%s (word)",pWrdVal->pWord() ? pWrdVal->pWord()->sName().a() : "<nilwrd>");
    }
    else if (CN_TASKVAL == eClass) {
      vTrace("alka??(task)");
      
//      if (pValWsp->nnpTask() == this)
//        vTrace("%p (task.this)",pValWsp->nnpTask());
//      else
//        vTrace("%p (task)",pValWsp->nnpTask());
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
  m_pCell    = m_pResetIP;
  m_pBeginIP = nullptr;
  
  m_sLiteral = "";
  
  m_xParamStack.vReset();
  m_xReturnStack.vReset();
}

/*----------------------------------------------------------------------------*/
/* rertry after exception                                                     */
/*----------------------------------------------------------------------------*/
void Task::vRetry(void)
{
  m_pCell    = m_pResetIP;
  m_pBeginIP = nullptr;
  
  m_sLiteral = "";
  
  m_xParamStack.vRetry();
  m_xReturnStack.vRetry();
}

/*----------------------------------------------------------------------------*/
/* get root thread from root tasks param stack                                */
/*----------------------------------------------------------------------------*/
Thread* Task::pRootThread(void)
{
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
  ASSERT(WT_ROUTINE == pWord->eWordType());
    
  vPushReturn(m_pCell);
    
  BodyField* pBody = pWord->pBodyField();
    
  m_pCell = pBody->m_pCell;
}

/*----------------------------------------------------------------------------*/
/* get transaction state                                                      */
/*----------------------------------------------------------------------------*/
eTS Task::eState(void)
{
  _poolAddress_(this);
  
  return m_eState;
}

/*----------------------------------------------------------------------------*/
/* set transaction state                                                      */
/*----------------------------------------------------------------------------*/
void Task::vState(eTS eState)
{
  _poolAddress_(this);
  
  m_xLock.vGrabWrite();
  m_eState = eState;
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get transaction id                                                         */
/*----------------------------------------------------------------------------*/
i8 Task::iTxId(void) const
{
  _poolAddress_(this);
  
  return m_iTxId;
}

/*----------------------------------------------------------------------------*/
/* set the tx id                                                              */
/*----------------------------------------------------------------------------*/
void Task::vTxId(i8 iTxId)
{
  _poolAddress_(this);
  
  m_iTxId = iTxId;
}

/*----------------------------------------------------------------------------*/
/* pop input direction                                                        */
/*----------------------------------------------------------------------------*/
void Task::vUndirect(void)
{
  m_xInputStream.vCloseFile();
  
  //vTrace("undirect return stack: %lld\n",nnpTask->m_xReturnStack.iValues());
  
  bool bPopped = bPopReturn(m_xInputStream);
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
      vError("[%s] %s : line %lld in %s\n",
             m_sName.a(),
             pError->pError(),
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
  if ( ! bGuardParam(CN_IPVAL) )
    return false;
  
  IPval xpIPval;
  
  m_xParamStack.vPop(&xpIPval);
  
  pCell = xpIPval.pCell();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8 from param stack                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(i8& iArg1)
{
  if ( ! bGuardParam(CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopParam(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8 from param stack                                                 */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(i8& iArg1, i8& iArg2)
{
  if ( ! bGuardParam(CN_INTVAL,CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopParam(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  IntVal xIntVal2;
  vPopParam(&xIntVal2);
  
  iArg2 = xIntVal2.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,val from param stack                                                */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(i8& iArg1, OnStack& xArg2)
{
  if ( ! bGuardParam(CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopParam(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  m_xParamStack.vPop(&xArg2);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8,val from param stack                                             */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(i8& iArg1, i8& iArg2, OnStack& xArg3)
{
  if ( ! bGuardParam(CN_INTVAL,CN_INTVAL,CN_STRVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopParam(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  IntVal xIntVal2;
  vPopParam(&xIntVal2);
  
  iArg2 = xIntVal2.iInt();
  
  m_xParamStack.vPop(&xArg3);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object* from param stack                                             */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Object*& pObject)
{
  if ( ! bGuardParam(CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  vPopParam(&xPtrVal1);
  
  pObject = xPtrVal1.pObject();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object*, Object* from param stack                                      */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Object*& pObject1, Object*& pObject2)
{
  if ( ! bGuardParam(CN_PTRVAL,CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  vPopParam(&xPtrVal1);
  
  pObject1 = xPtrVal1.pObject();
  
  PtrVal xPtrVal2;
  vPopParam(&xPtrVal2);
  
  pObject2 = xPtrVal2.pObject();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop Object* from param stack                                               */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Object*& pObject, OnStack& xValue)
{
  if ( ! bGuardParam(CN_PTRVAL) )
    return false;
  
  PtrVal xPtrVal1;
  vPopParam(&xPtrVal1);
  
  pObject = xPtrVal1.pObject();
  
  return bPopParam(xValue);
}

/*----------------------------------------------------------------------------*/
/* pop s from param stack                                                     */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(s32& sArg1)
{
  if ( ! bGuardParam(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  vPopParam(&xValWsp1);
  
  sArg1 = xValWsp1.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from param stack                                                     */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(s256& sArg1)
{
  if ( ! bGuardParam(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  vPopParam(&xValWsp1);
  
  sArg1 = xValWsp1.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from param stack                                                     */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(s16K& sArg1)
{
  if ( ! bGuardParam(CN_STRVAL) )
    return false;
  
  StrVal xStrVal1;
  vPopParam(&xStrVal1);
  
  sArg1 = xStrVal1.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s,s from param stack                                                   */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(s16K& sArg1, s16K& sArg2)
{
  if ( ! bGuardParam(CN_STRVAL) )
    return false;
  
  StrVal xStrVal1;
  vPopParam(&xStrVal1);
  
  sArg1 = xStrVal1.sStr();
  
  StrVal xStrVal2;
  vPopParam(&xStrVal2);
  
  sArg2 = xStrVal2.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop tx* from param stack                                                   */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(Task*& pTask1)
{
  TaskVal xTaskVal1;
  
  if ( ! bGuardParam(CN_TASKVAL) )
    return false;
  
  vPopParam(&xTaskVal1);
  
  pTask1 = xTaskVal1.pTask();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from param stack                                                 */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(OnStack& xArg1)
{
  if ( ! bGuardParam(1) )
    return false;
  
  m_xParamStack.vPop(&xArg1);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value, value from param stack                                          */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(OnStack& pValue1, OnStack& pValue2)
{
  if ( ! bGuardParam(2) )
    return false;
  
  m_xParamStack.vPop(&pValue1);
  
  m_xParamStack.vPop(&pValue2);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value, value, value from param stack                                   */
/*----------------------------------------------------------------------------*/
bool Task::bPopParam(OnStack& pValue1, OnStack& pValue2, OnStack& pValue3)
{
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
  if ( ! bGuardParam(CN_WRDVAL) )
    return false;
  
  WrdVal xWrdVal1;
  vPopParam(&xWrdVal1);
  
  pWord1 = xWrdVal1.pWord();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from param stack as integer                                      */
/*----------------------------------------------------------------------------*/
bool Task::bPopParamInt(i8& iArg1)
{
  if ( ! bGuardParam(1) )
    return false;
  
  ValWsp xWspVal1;
  vPopParam(&xWspVal1);
  
  iArg1 = xWspVal1.iAsInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from param stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vPopParam(OnStack* pValue)
{
  m_xParamStack.vPop(pValue);
}

/*----------------------------------------------------------------------------*/
/* push cell* onto param stack                                                */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(Cell* pCell)
{
  IPval xIP;
  
  xIP = pCell;
  
  vPushParam(xIP);
}

/*----------------------------------------------------------------------------*/
/* push char* onto param stack                                                */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(const char* aString)
{
  ValWsp xStrVal1;
  xStrVal1 = aString;
  
  vPushParam(&xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push Object* onto param stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(Object* pObject1)
{
  PtrVal xEmbVal1;
  xEmbVal1 = pObject1;
  
  vPushParam(&xEmbVal1);
}

/*----------------------------------------------------------------------------*/
/* push i8 onto param stack                                                   */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(i8 iArg1)
{
  IntVal xIntVal1;
  xIntVal1 = iArg1;
  
  vPushParam(&xIntVal1);
}

/*----------------------------------------------------------------------------*/
/* push s32 onto param stack                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(s32& sArg1)
{
  vPushParam(sArg1.a());
}

/*----------------------------------------------------------------------------*/
/* push s256 onto param stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(s256& sArg1)
{
  vPushParam(sArg1.a());
}

/*----------------------------------------------------------------------------*/
/* push s16K onto param stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(s16K& sArg1)
{
  vPushParam(sArg1.a());
}

/*----------------------------------------------------------------------------*/
/* push object onto parameter stack                                           */
/*----------------------------------------------------------------------------*/
//void Task::vPushParam(Object* pObject)
//{
//  m_xParamStack.vPush(pObject);
//}

/*----------------------------------------------------------------------------*/
/* push value onto parameter stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(OnStack& xVal1)
{
  m_xParamStack.vPush(&xVal1);
}

/*----------------------------------------------------------------------------*/
/* push value onto parameter stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(OnStack* pValue)
{
  m_xParamStack.vPush(pValue);
}

/*----------------------------------------------------------------------------*/
/* push tx* onto param stack                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(NNP_Task pnnArg1)
{
  TaskVal xTaskVal1;
  xTaskVal1 = *pnnArg1;
  
  vPushParam(&xTaskVal1);
}

/*----------------------------------------------------------------------------*/
/* push word* onto param stack                                                */
/*----------------------------------------------------------------------------*/
void Task::vPushParam(Word* pArg1)
{
  WrdVal xWrdVal1;
  xWrdVal1 = pArg1;
  
  vPushParam(&xWrdVal1);
}

/*----------------------------------------------------------------------------*/
/* push char* onto return stack                                               */
/*----------------------------------------------------------------------------*/
void Task::vPushReturn(const char* aString)
{
  ValWsp xStrVal1;
  xStrVal1 = aString;
  
  vPushReturn(&xStrVal1);
}

/*----------------------------------------------------------------------------*/
/* push i8 onto return stack                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPushReturn(i8 iArg1)
{
  IntVal xIntVal1;
  xIntVal1 = iArg1;
  
  vPushReturn(&xIntVal1);
}

/*----------------------------------------------------------------------------*/
/* push s32 onto param stack                                                  */
/*----------------------------------------------------------------------------*/
void Task::vPushReturn(s32& sArg1)
{
  vPushReturn(sArg1.a());
}

/*----------------------------------------------------------------------------*/
/* push s256 onto param stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vPushReturn(s256& sArg1)
{
  vPushReturn(sArg1.a());
}

/*----------------------------------------------------------------------------*/
/* push s16K onto param stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vPushReturn(s16K& sArg1)
{
  vPushReturn(sArg1.a());
}

/*----------------------------------------------------------------------------*/
/* push function address onto return stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vPushReturn(Cell* pCell)
{
  IPval xpIPval;
  xpIPval = pCell;
  
  m_xReturnStack.vPush(&xpIPval);
}

/*----------------------------------------------------------------------------*/
/* push value onto return stack                                               */
/*----------------------------------------------------------------------------*/
void Task::vPushReturn(OnStack* pValue)
{
  m_xReturnStack.vPush(pValue);
}

/*----------------------------------------------------------------------------*/
/* push value onto return stack                                               */
/*----------------------------------------------------------------------------*/
void Task::vPushReturn(OnStack& xValue)
{
  m_xReturnStack.vPush(&xValue);
}

/*----------------------------------------------------------------------------*/
/* pop cell from return stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vPopReturn(Cell*& pCell)
{
  IPval xpIPval;
  
  m_xReturnStack.vPop(&xpIPval);
  
  pCell = xpIPval.pCell();
}

/*----------------------------------------------------------------------------*/
/* pop i8 from return stack                                                   */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(i8& iArg1)
{
  if ( ! bGuardReturn(CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopReturn(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop i8,i8 from return stack                                                */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(i8& iArg1, i8& iArg2)
{
  if ( ! bGuardReturn(CN_INTVAL,CN_INTVAL) )
    return false;
  
  IntVal xIntVal1;
  vPopReturn(&xIntVal1);
  
  iArg1 = xIntVal1.iInt();
  
  IntVal xIntVal2;
  vPopReturn(&xIntVal2);
  
  iArg2 = xIntVal2.iInt();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop val from return stack                                                  */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(OnStack& xValue)
{
  // need to check the parameter stack
  
  vPopReturn(&xValue);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop val from return stack                                                  */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(OnStack& xValue1, OnStack& xValue2)
{
  if ( ! bGuardReturn(2) )
    return false;
  
  vPopReturn(&xValue1);
  vPopReturn(&xValue2);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value from return stack                                                */
/*----------------------------------------------------------------------------*/
void Task::vPopReturn(OnStack* pValue)
{
  m_xReturnStack.vPop(pValue);
}

/*----------------------------------------------------------------------------*/
/* pop s from return stack                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(s32& sArg1)
{
  if ( ! bGuardReturn(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  vPopReturn(&xValWsp1);
  
  sArg1 = xValWsp1.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from return stack                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(s256& sArg1)
{
  if ( ! bGuardReturn(CN_STRVAL) )
    return false;
  
  ValWsp xValWsp1;
  vPopReturn(&xValWsp1);
  
  sArg1 = xValWsp1.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop s from return stack                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopReturn(s16K& sArg1)
{
  if ( ! bGuardReturn(CN_STRVAL) )
    return false;
  
  StrVal xStrVal1;
  vPopReturn(&xStrVal1);
  
  sArg1 = xStrVal1.sStr();
  
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
  nnpTask->vPrimitive("vWord");
}

/*----------------------------------------------------------------------------*/
/* accept -- accept input string and push on stack                            */
/*----------------------------------------------------------------------------*/
void Task::vAccept(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAccept");
  
  ValWsp xWsp;
  xWsp.vClass(CN_STRVAL);
  
  char* y = xWsp.sStr();
  
  y = xWsp.sStr();
  while ( '\n' != (*y++ = getc(stdin)) ) ;
  
  *--y = '\0';
  
  xWsp.pAsStrVal()->vSize();
  
  nnpTask->vPushParam(xWsp);
}

/*----------------------------------------------------------------------------*/
/* + -- add top two integers on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vAdd(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAdd");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("+");
  
  r = i + j;
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* again -- unconditionally repeat loop (use only in : docol)                 */
/*----------------------------------------------------------------------------*/
void Task::vAgain(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAgain");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pBeginLoc;
  nnpTask->vPopReturn(pBeginLoc);
  
  pCurrentWord->vEncode("jump",pBeginLoc);
}

/*----------------------------------------------------------------------------*/
/* again_ -- unconditionally repeat begin_ loop (use only in : docol)         */
/*----------------------------------------------------------------------------*/
void Task::vAgain_(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAgain_");
  
  nnpTask->m_pCell = nnpTask->m_pBeginIP;
}

/*----------------------------------------------------------------------------*/
/* allot -- reserve n bytes in current word                                   */
/*----------------------------------------------------------------------------*/
void Task::vAllot(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAllot");
  
  i8 iBytes;
  
  if ( ! nnpTask->bPopParam(iBytes) )
    THROW_INVALID_PARAM("allot");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();

  char* pAddress = pCurrentWord->pAllot(iBytes);
  ASSERT(pAddress);
}

/*----------------------------------------------------------------------------*/
/* and -- logical and of top two values                                       */
/*----------------------------------------------------------------------------*/
void Task::vAnd(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAnd");
  
  i8 iArgB;
  if ( ! nnpTask->bPopParamInt(iArgB) )
    THROW_INVALID_PARAM("and");
  
  i8 iArgA;
  if ( ! nnpTask->bPopParamInt(iArgA) )
    THROW_INVALID_PARAM("and");

  i8 iArgR = (iArgA && iArgB) ? 1 : 0;
  
  nnpTask->vPushParam(iArgR);
}

/*----------------------------------------------------------------------------*/
/* begin -- begin a loop (use only in : docol)                                */
/*----------------------------------------------------------------------------*/
void Task::vBegin(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vBegin");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  IPval xPtrBeginLoc;

  xPtrBeginLoc = pCurrentWord->pLastCell();
  
  nnpTask->vPushReturn(&xPtrBeginLoc);
}

/*----------------------------------------------------------------------------*/
/* begin_ -- begin a loop (use only in : docol)                               */
/*----------------------------------------------------------------------------*/
void Task::vBegin_(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vBegin_");
  
  nnpTask->m_pBeginIP = nnpTask->m_pCell;
}

/*----------------------------------------------------------------------------*/
/* boot -- boot                                                               */
/*----------------------------------------------------------------------------*/
void Task::vBoot(NNP_Task nnpTask)
{
  static bool bHasRun = false;
  
  nnpTask->vPrimitive("vBoot");
  ASSERT( ! bHasRun );
  bHasRun = true;

  nnpTask->vAddConstants();
}

/*----------------------------------------------------------------------------*/
/* bpa -- buffer pool analyze                                                 */
/*----------------------------------------------------------------------------*/
void Task::vBpa(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vBpa");
  
  BufferPoolAnalyzer* pBPA = new BufferPoolAnalyzer;
  
  pBPA->vAnalyze();
  
  delete pBPA;
}

/*----------------------------------------------------------------------------*/
/* classFetch -- fetch class number from Object object                        */
/*----------------------------------------------------------------------------*/
void Task::vClassFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vClassFetch");
  
  PtrVal xPtrObject;
  nnpTask->vPopParam(&xPtrObject);
  
  IntVal xClassNoVal;
  xClassNoVal = xPtrObject.pObject()->eClass();
  
  nnpTask->vPushParam(&xClassNoVal);
}

/*----------------------------------------------------------------------------*/
/* classStore -- store class number into Object object                        */
/*----------------------------------------------------------------------------*/
void Task::vClassStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vClassStore");

  IntVal xClassNoVal;
  nnpTask->vPopParam(&xClassNoVal);
  
  PtrVal xPtrObject;
  nnpTask->vPopParam(&xPtrObject);
  
  eCN eClassNo = static_cast<eCN>(xClassNoVal.iInt());

  xPtrObject.pObject()->vClass(eClassNo);
}

/*----------------------------------------------------------------------------*/
/* clip -- clip string ( length str -- str )                                  */
/*----------------------------------------------------------------------------*/
void Task::vClip(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vClip");
  
  i8 iLength;
  ValWsp xString;
  ValWsp xSubStr;
  
  StrVal* pStrVal = xString.pAsStrVal();
  
  // get the arguments
  if ( ! nnpTask->bPopParam(iLength,xString) )
    THROW_INVALID_PARAM("clip");
  
  // validate length
  i8 iStrLength = pStrVal->iChars();
  
  if ( iLength == 0 || llabs(iLength) >= iStrLength)
    THROW("Invalid length for: clip");
  
  char* pStart;
  
  if (iLength > 0)
    pStart = xString.sStr();
  else
    pStart = xString.sStr() + iStrLength - 1 - llabs(iLength);

  xSubStr.pAsStrVal()->vCopyString(pStart,llabs(iLength));
  
  nnpTask->vPushParam(&xSubStr);
}

/*----------------------------------------------------------------------------*/
/* codeAssign -- fetch ptr from word.code on stack into this word             */
/*----------------------------------------------------------------------------*/
void Task::vCodeAssign(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vCodeAssign");
  
  WrdVal xThatWord;
  nnpTask->vPopParam(&xThatWord);
  Word* pThatWord = xThatWord.pWord();
  
  Word* pThisWord = nnpTask->pCurrentWord();
  
  ASSERT(pThatWord->pFunc() == pThatWord->pFunc());
  pThisWord->vFunc(pThatWord->pFunc());
}

/*----------------------------------------------------------------------------*/
/* : -- colon                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vColon(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vColon");
  
  nnpTask->m_bCompiling = true;

  nnpTask->vLiteralNext();
  
  new(nnpTask->m_xDictionaryStack.pTop())  Word(nnpTask->m_sLiteral.a());
}

/*----------------------------------------------------------------------------*/
/* comma -- encode push value                                                 */
/*----------------------------------------------------------------------------*/
void Task::vComma(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vComma");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  ValWsp xValWsp;
  if ( ! nnpTask->bPopParam(xValWsp) )
    THROW_INVALID_PARAM("comma");
    
  pCurrentWord->vEncode("push",&xValWsp);
  pCurrentWord->vEncode("exit");
}

/*----------------------------------------------------------------------------*/
/* comment -- comment                                                         */
/*----------------------------------------------------------------------------*/
void Task::vComment(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vComment");
  
  for (;;) {
    nnpTask->vLiteralNext();
    if (String::bEqualTo(")",nnpTask->m_sLiteral.a()))
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* compiling -- enter compiling mode                                          */
/*----------------------------------------------------------------------------*/
void Task::vCompiling(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vCompiling");
  
  nnpTask->m_bCompiling = true;
}

/*----------------------------------------------------------------------------*/
/* concat -- concat two strings                                               */
/*----------------------------------------------------------------------------*/
void Task::vConcat(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vConcat");
  
  ValWsp xStrR;
  ValWsp xStrB;
  ValWsp xStrA;

  if ( ! nnpTask->bPopParam(xStrB,xStrA) )
    THROW_INVALID_PARAM("concat");
  
  xStrR = xStrA;
  
  String::vConcat(xStrR.sStr(),xStrB.sStr());
  xStrR.pAsStrVal()->vSize();
  
  nnpTask->vPushParam(xStrR);
}

/*----------------------------------------------------------------------------*/
/* cursor@ -- fetch cursor from return stack onto param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCursorFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vCursorFetch");
  
  i8 iPick = 1;
  
  ValWsp xCursorWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xCursorWsp);
  
  nnpTask->vPushParam(xCursorWsp);
}

/*----------------------------------------------------------------------------*/
/* cursor! -- replace cursor on return stack from param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCursorStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vCursorStore");
  
  i8 iPeek = 1;
  
  ValWsp xCursorWsp;
  
  if ( ! nnpTask->bPopParam(xCursorWsp) )
    THROW_INVALID_PARAM("cursor!");
  
  nnpTask->m_xReturnStack.vPoke(iPeek,&xCursorWsp);
}

/*----------------------------------------------------------------------------*/
/* cwd -- change working directory                                            */
/*----------------------------------------------------------------------------*/
void Task::vCwd(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vCwd");
  
  s256 sDir;
  if ( ! nnpTask->bPopParam(sDir) )
    THROW_INVALID_PARAM("cwd");
  
  Dir xDir;
  if ( ! xDir.bSet(sDir) )
    THROW("Unable to set current directory");
}

/*----------------------------------------------------------------------------*/
/* depth -- push depth of parameters stack onto stack                         */
/*----------------------------------------------------------------------------*/
void Task::vDepth(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDepth");
  
  i8 iDepth = nnpTask->m_xParamStack.iValues();
  
  nnpTask->vPushParam(iDepth);
}

/*----------------------------------------------------------------------------*/
/* / -- divide top integer by second integer on stack                         */
/*----------------------------------------------------------------------------*/
void Task::vDivide(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDivide");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("/");
    
  r = DIV(i,j);
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* do -- compile time of do ... loop                                immediate */
/*----------------------------------------------------------------------------*/
void Task::vDo(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDo");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("do_rt");
  
  Cell* pDoLoc = pCurrentWord->pLastCell();
  
  nnpTask->vPushReturn(pDoLoc);
}

/*----------------------------------------------------------------------------*/
/* do_rt -- runtime of do ... loop                                            */
/*----------------------------------------------------------------------------*/
void Task::vDo_RT(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDo_RT");
  
  i8 iIndex;
  i8 iLimit;
  if ( ! nnpTask->bPopParam(iIndex,iLimit) )
    THROW_INVALID_PARAM("do_rt");
  
  nnpTask->vPushReturn(iLimit);
  nnpTask->vPushReturn(iIndex); // index is on top
}

/*----------------------------------------------------------------------------*/
/* drop -- discard top value off stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vDrop(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  nnpTask->vPopParam(&xValA);
}

/*----------------------------------------------------------------------------*/
/* dup -- dup top of param stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vDup(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDup");
  
  // could just copy from stack back into stack
  
  ValWsp xValA;
  if ( ! nnpTask->bPopParam(xValA) )
    THROW_INVALID_PARAM("dup");
  
  nnpTask->vPushParam(xValA);
  nnpTask->vPushParam(xValA);
}

/*----------------------------------------------------------------------------*/
/* dump -- dump top of param stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDump");
  
  s16K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->bPopParam(xValWsp) )
    THROW_INVALID_PARAM("dump");
  
  xValWsp.vToString(sWorkspace.a());
  char* aClass = xValWsp.aClass();
  
  vTrace("[%s] %s (%s)\n",nnpTask->m_sName.a(),sWorkspace.a(),aClass);
}

/*----------------------------------------------------------------------------*/
/* else -- else clause                                                        */
/*----------------------------------------------------------------------------*/
void Task::vElse(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vElse");

  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pIfJumpArg;
  nnpTask->vPopReturn(pIfJumpArg);
  
  //-------------------------------------------------------------------------
  
  Cell* pElseJumpArg;
  
  pCurrentWord->vEncode("jump");
  pElseJumpArg = (Cell*)pCurrentWord->pAppend((i8)0);
  
  nnpTask->vPushReturn(pElseJumpArg);
  
  //-------------------------------------------------------------------------
  
  pIfJumpArg->m_pJumpLocn = pCurrentWord->pLastCell();
}

/*----------------------------------------------------------------------------*/
/* encodeCall -- encode call                                                  */
/*----------------------------------------------------------------------------*/
void Task::vEncodeCall(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vEncodeCall");
  
  Word* pCalledWord;
  if ( ! nnpTask->bPopParam(pCalledWord) )
    THROW_INVALID_PARAM("encodeCall");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  CodeField* pCode = pCalledWord->pCodeField();
  
  pCurrentWord->vEncode(pCode);
}

/*----------------------------------------------------------------------------*/
/* encodeDoes -- encode does part                                             */
/*----------------------------------------------------------------------------*/
void Task::vEncodeDoes(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vEncodeDoes");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vFunc(&Task::vEnter);
  pCurrentWord->vAddr(nnpTask->m_pCell+1);
}

/*----------------------------------------------------------------------------*/
/* encodeEmb -- encode push ptr to Object object                              */
/*----------------------------------------------------------------------------*/
void Task::vEncodeEmb(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vEncodeEmb");
  
  PtrVal xPtrObject;
  xPtrObject = nnpTask->m_pWord->pFirstCell();
  
  if (nnpTask->m_bCompiling) {
    //ASSERT(0);
    Word* pCurrentWord = nnpTask->pCurrentWord();
    pCurrentWord->vEncode("push",&xPtrObject);
  }
  else {
    nnpTask->vPushParam(&xPtrObject);
  }
}

/*----------------------------------------------------------------------------*/
/* encodeFun -- encode primitive fun                                          */
/*----------------------------------------------------------------------------*/
void Task::vEncodeFun(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vEncodeFun");
  
  Word* pCalledWord;
  if ( ! nnpTask->bPopParam(pCalledWord) )
    THROW_INVALID_PARAM("encodeFun");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  CodeField* pCode = pCalledWord->pCodeField();
  
  pCurrentWord->vAppend(pCode);
}

/*----------------------------------------------------------------------------*/
/* encodeVal -- encode push value                                             */
/*----------------------------------------------------------------------------*/
void Task::vEncodeVal(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vEncodeVal");
  
  ValWsp xValA;
  if ( ! nnpTask->bPopParam(xValA) )
    THROW_INVALID_PARAM("encodeVal");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("push",&xValA);
}

/*----------------------------------------------------------------------------*/
/* enter -- enter this routine                                                */
/*----------------------------------------------------------------------------*/
void Task::vEnter(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vEnter");
  
  if (nnpTask->m_bTracing)
    vDebug();
  
  nnpTask->vPushReturn(nnpTask->m_pCell);
  
  Cell* pPrevCell = nnpTask->m_pCell - 1;
  CodeField* pCode = pPrevCell->m_pCodeField;
  
  //Cell* pCell = reinterpret_cast<Cell*>(pcAdd(pCode,2*sizeof(Cell*)));

  BodyField* pBody = reinterpret_cast<BodyField*>(pcAdd(pCode,1*sizeof(Cell*)));

  Word* pWord = reinterpret_cast<Word*>(pcAdd(pBody, -(sizeof(Word)-sizeof(BodyField)) ));
  if (WT_OBJECT == pWord->eWordType()) {
    PtrVal xPtrVal;
    xPtrVal = pWord->pFirstCell();
    nnpTask->vPushParam(xPtrVal);

    //nnpTask->vPushParam(pWord->pFirstCell());
  }

  //ASSERT(pBody->m_pCell == pCell);
  //nnpTask->m_pCell = pCell;
  
  nnpTask->m_pCell = pBody->m_pCell;
}

/*----------------------------------------------------------------------------*/
/* ] -- enter compiler state                                                  */
/*----------------------------------------------------------------------------*/
void Task::vEnterCompiler(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vEnterCompiler");
  
  nnpTask->m_bCompiling = true;
}

/*----------------------------------------------------------------------------*/
/* [ -- enter interpreter state                                               */
/*----------------------------------------------------------------------------*/
void Task::vEnterInterpreter(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vEnterInterpreter");

  nnpTask->m_bCompiling = false;
}

/*----------------------------------------------------------------------------*/
/* env@ -- get environment variable                                           */
/*----------------------------------------------------------------------------*/
void Task::vEnvGet(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vEnvGet");
  
  s256 sEnvName;
  
  if ( ! nnpTask->bPopParam(sEnvName) )
    THROW_INVALID_PARAM("env@");
  
  s16K sEnvVal;
  
  if (! Env::bGetEnv(sEnvName.a(),sEnvVal.a()))
    sEnvVal = "";
  
  nnpTask->vPushParam(sEnvVal);
}

/*----------------------------------------------------------------------------*/
/* == -- test top two integers on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Task::vEqual(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("==");
  
  r = i == j;
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* eql -- test top two address on data stack for equality                     */
/*----------------------------------------------------------------------------*/
void Task::vEqualAddress(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vEql");
  
  ValWsp xAddressA;
  ValWsp xAddressB;
  
  if ( ! nnpTask->bPopParam(xAddressA,xAddressB) )
    THROW_INVALID_PARAM("eql");
  
  i8 iA = xAddressA.iAsInt();
  i8 iB = xAddressB.iAsInt();
  
  i8 r = iA == iB;
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* execute -- execute word on param stack                                     */
/*----------------------------------------------------------------------------*/
void Task::vExecute(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vExecute");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("execute");
  
  if (WT_PRIMITIVE == pWord->eWordType()) {
    pFUN pFunc = pWord->pFunc();
    (pFunc)(nnpTask);
  }
  else if (WT_OBJECT == pWord->eWordType()) {
    nnpTask->vPushReturn(nnpTask->m_pCell);

    PtrVal xPtrVal;
    xPtrVal = pWord->pFirstCell();
    nnpTask->vPushParam(xPtrVal);
    
    pFUN pFunc = pWord->pFunc();
    if (!pFunc || Task::vNop == pFunc) {
      nnpTask->vPopReturn(nnpTask->m_pCell);
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
}

/*----------------------------------------------------------------------------*/
/* exit -- exit from routine                                                  */
/*----------------------------------------------------------------------------*/
void Task::vExit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vExit");
  
  //vTrace("Exit: return stack %lld\n", nnpTask->m_xReturnStack.iValues());
  
  nnpTask->vPopReturn(nnpTask->m_pCell);
}

/*----------------------------------------------------------------------------*/
/* @ -- fetch value using pointer on stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFetch");
  
  PtrVal xPtr;
  nnpTask->vPopParam(&xPtr);
  
  ValWsp xVal;
  
  xPtr.vFetch(xVal);
  
  nnpTask->vPushParam(xVal);
}


/*----------------------------------------------------------------------------*/
/* freePercent -- push free percent of buffer pool onto stack                 */
/*----------------------------------------------------------------------------*/
void Task::vFreePercent(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFreePercent");
  
  i8 iFreePercent = gpBufferPool->iFreePercent();
  
  nnpTask->vPushParam(iFreePercent);
}

/*----------------------------------------------------------------------------*/
/* freeSpace -- push free space in buffer pool onto stack                     */
/*----------------------------------------------------------------------------*/
void Task::vFreeSpace(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFreeSpace");
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  
  nnpTask->vPushParam(iFreeSpace);
}

/*----------------------------------------------------------------------------*/
/* > -- test top two integers on data stack for greater than                  */
/*----------------------------------------------------------------------------*/
void Task::vGreater(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vGreater");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM(">");
  
  r = i > j;
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* >= -- test top two integers on data stack for greater than equal to        */
/*----------------------------------------------------------------------------*/
void Task::vGreaterEqual(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vGreaterEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM(">=");
  
  r = i >= j;
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* gwd -- get working directory                                               */
/*----------------------------------------------------------------------------*/
void Task::vGwd(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vGwd");
  
  ValWsp xStrVal;
  
  Dir xDir;
  s256 sDir;
  
  if (xDir.bGet(sDir)) {
    xStrVal = sDir.a();
    nnpTask->vPushParam(&xStrVal);
  }
  else {
    THROW("Unable to get current directory");
  }
}

/*----------------------------------------------------------------------------*/
/* hazard -- should never be called                                           */
/*----------------------------------------------------------------------------*/
void Task::vHazard(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vHazard");
  
  THROW("Entered hazard");
}

/*----------------------------------------------------------------------------*/
/* i -- copy loop index from return stack onto param stack                    */
/*----------------------------------------------------------------------------*/
void Task::vI(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vI");
  
  OnStack* pI = nnpTask->m_xReturnStack.pPeek();
  
  nnpTask->vPushParam(pI);
}

/*----------------------------------------------------------------------------*/
/* if -- if clause                                                            */
/*----------------------------------------------------------------------------*/
void Task::vIf(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIf");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pIfJumpArg;

  pCurrentWord->vEncode("jumpFalse");
  
  pIfJumpArg = (Cell*)pCurrentWord->pAppend((i8)0);
  
  nnpTask->vPushReturn(pIfJumpArg);
}

/*----------------------------------------------------------------------------*/
/* immediate -- make the most recent definition an immediate word             */
/*----------------------------------------------------------------------------*/
void Task::vImmediate(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vImmediate");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();

  pCurrentWord->vImmediate(true);
}

/*----------------------------------------------------------------------------*/
/* input.close -- close current input                                         */
/*----------------------------------------------------------------------------*/
void Task::vInputClose(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vInputClose");
  
  nnpTask->m_xInputStream.vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* input.depth -- push current depth of input onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vInputDepth(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vInputDepth");

  i8 iDepth = nnpTask->m_xInputStream.iDepth();
  
  nnpTask->vPushParam(iDepth);
}

/*----------------------------------------------------------------------------*/
/* input.open -- open new input using filepath on param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vInputOpen(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vInputOpen");
  
  s256 sFilePath;
  
  if ( ! nnpTask->bPopParam(sFilePath) )
    THROW_INVALID_PARAM("input.open");
  
  bool bOpened = nnpTask->m_xInputStream.bOpenFile(sFilePath.a());
  if ( ! bOpened )
    THROW("No such file: %s",sFilePath.a());
}

/*----------------------------------------------------------------------------*/
/* input.pop -- pop previous input from return stack                          */
/*----------------------------------------------------------------------------*/
void Task::vInputPop(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vInputPop");
  
  i8 iInputDepth = nnpTask->m_xInputStream.iDepth();
  
  bool bPopped = nnpTask->bPopReturn(nnpTask->m_xInputStream);
  ASSERT(bPopped);
  
  ASSERT(iInputDepth = nnpTask->m_xInputStream.iDepth() + 1);

  bPopped = nnpTask->bPopReturn(nnpTask->m_sCommand);
  ASSERT(bPopped);
}

/*----------------------------------------------------------------------------*/
/* input.push -- push current input onto return stack (resets input)          */
/*----------------------------------------------------------------------------*/
void Task::vInputPush(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vInputPush");
  
  i8 iInputDepth = nnpTask->m_xInputStream.iDepth();
  
  nnpTask->vPushReturn(nnpTask->m_sCommand);

  nnpTask->vPushReturn(nnpTask->m_xInputStream);
  
  nnpTask->m_xInputStream.vReset();
  
  nnpTask->m_xInputStream.vDepth(iInputDepth + 1);
}

/*----------------------------------------------------------------------------*/
/* input.stdin -- set input to stdin                                          */
/*----------------------------------------------------------------------------*/
void Task::vInputStdin(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vInputStdin");
  
  //nnpTask->vPushReturn(nnpTask->m_xInputStream);
  
  nnpTask->m_xInputStream.vReset();
  
  nnpTask->m_xInputStream.vUseStdin();
}

/*----------------------------------------------------------------------------*/
/* input.text -- redirect input to command string on param stack              */
/*----------------------------------------------------------------------------*/
void Task::vInputText(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vInputText");
  
  s256 sCommand;
  
  if ( ! nnpTask->bPopParam(sCommand) )
    THROW_INVALID_PARAM("input.text");
    
  nnpTask->m_sCommand = sCommand;
  
  nnpTask->m_xInputStream.vUseText(nnpTask->m_sCommand);
}

/*----------------------------------------------------------------------------*/
/* interpreting -- enter interpreting mode                                    */
/*----------------------------------------------------------------------------*/
void Task::vInterpreting(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vInterpreting");
  
  nnpTask->m_bCompiling = false;
}

/*----------------------------------------------------------------------------*/
/* isBooted -- has system booted?                                             */
/*----------------------------------------------------------------------------*/
void Task::vIsBooted(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIsBooted");
  
  Word* pWord = nnpTask->pFindWord("dicInitialised");
  
  i8 bIsBooted = pWord ? true : false;
  
  nnpTask->vPushParam(bIsBooted);
}

/*----------------------------------------------------------------------------*/
/* isCompiling -- currently in compiling mode?                                */
/*----------------------------------------------------------------------------*/
void Task::vIsCompiling(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIsCompiling");
  
  i8 iIsCompiling = nnpTask->m_bCompiling ? 1 : 0;
  
  nnpTask->vPushParam(iIsCompiling);
}

/*----------------------------------------------------------------------------*/
/* isImmediate -- is word immediate?                                          */
/*----------------------------------------------------------------------------*/
void Task::vIsImmediate(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIsImmediate");
  
  // we should just modify the pointers rather than actually pop
  
  Word* pWord;
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("isImmediate");
  
  i8 iIsImmediate = pWord->bImmediate() ? 1 : 0;
  
  nnpTask->vPushParam(iIsImmediate);
}

/*----------------------------------------------------------------------------*/
/* isInterpreting -- currently in interpreting mode?                          */
/*----------------------------------------------------------------------------*/
void Task::vIsInterpreting(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIsInterpreting");
  
  i8 iIsInterpreting = nnpTask->m_bCompiling ? 0 : 1;
  
  nnpTask->vPushParam(iIsInterpreting);
}

/*----------------------------------------------------------------------------*/
/* isIntLit -- is integer literal?                                            */
/*----------------------------------------------------------------------------*/
void Task::vIsIntLit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIsIntLit");
  
  i8 iIsIntLit = LT_INTLIT == nnpTask->m_eLiteral ? 1 : 0;
  
  nnpTask->vPushParam(iIsIntLit);
}

/*----------------------------------------------------------------------------*/
/* isNameLit -- is name literal?                                              */
/*----------------------------------------------------------------------------*/
void Task::vIsNameLit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIsNameLit");
  
  i8 iIsNameLit = LT_NAMELIT == nnpTask->m_eLiteral ? 1 : 0;
  
  nnpTask->vPushParam(iIsNameLit);
}

/*----------------------------------------------------------------------------*/
/* isPrimitive -- is word a primitive?                                        */
/*----------------------------------------------------------------------------*/
void Task::vIsPrimitive(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIsPrimitive");
  
  Word* pWord;

  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("isPrimitive");
  
  i8 iIsPrimitive = WT_PRIMITIVE == pWord->eWordType() ? 1 : 0;
  
  nnpTask->vPushParam(iIsPrimitive);
}

/*----------------------------------------------------------------------------*/
/* isStrLit -- is string literal?                                             */
/*----------------------------------------------------------------------------*/
void Task::vIsStrLit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIsStrLit");
  
  i8 iIsStrLit = LT_STRLIT == nnpTask->m_eLiteral ? 1 : 0;
  
  nnpTask->vPushParam(iIsStrLit);
}

/*----------------------------------------------------------------------------*/
/* iterate -- compile time of iterate ... next                      immediate */
/*----------------------------------------------------------------------------*/
void Task::vIterate(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIterate");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("iterate_rt_a");
  
  Cell* pIterateLoc = pCurrentWord->pLastCell();
  nnpTask->vPushReturn(pIterateLoc);
  pCurrentWord->vEncode("iterate_rt_b");

  pCurrentWord->vEncode("jumpFalse");
  Cell* pJumpFalseArg = (Cell*)pCurrentWord->pAppend((i8)0);
  nnpTask->vPushReturn(pJumpFalseArg);
}

/*----------------------------------------------------------------------------*/
/* iterate_rt_a -- runtime of iterate ... next                                */
/*----------------------------------------------------------------------------*/
void Task::vIterate_RT_A(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIterate_RT_A");
  
  PtrVal xPtrObject;
  PtrVal xPtrCursor;
  
  if ( ! nnpTask->bPopParam(xPtrObject,xPtrCursor) )
    THROW_INVALID_PARAM("iterate_rt_a");
  
  nnpTask->vPushReturn(xPtrCursor);
  nnpTask->vPushReturn(xPtrObject);
}

/*----------------------------------------------------------------------------*/
/* iterate_rt_b -- runtime of iterate ... next                                */
/*----------------------------------------------------------------------------*/
void Task::vIterate_RT_B(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vIterate_RT_B");
  
  i8 iPick = 0;
  
  ValWsp xObjectWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xObjectWsp);
  nnpTask->vPushParam(xObjectWsp);
  
  ValWsp xWsp;
  
  if ( ! xObjectWsp.iAsInt() ) {
    if ( !nnpTask->bPopReturn(xWsp,xWsp) )
      THROW_INVALID_PARAM("iterate_rt_b");
  }
}

/*----------------------------------------------------------------------------*/
/* j -- copy 2nd loop index from return stack onto param stack                */
/*----------------------------------------------------------------------------*/
void Task::vJ(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vJ");
  
  OnStack* pI = nnpTask->m_xReturnStack.pPeek(3);
  
  nnpTask->vPushParam(pI);
}

/*----------------------------------------------------------------------------*/
/* jump -- jump to location                                                   */
/*----------------------------------------------------------------------------*/
void Task::vJump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vJump");
  
  nnpTask->m_pCell = nnpTask->m_pCell->m_pJumpLocn;
}

/*----------------------------------------------------------------------------*/
/* jumpFalse -- jump on false to location                                     */
/*----------------------------------------------------------------------------*/
void Task::vJumpFalse(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vJumpFalse");
  
  i8 iVal;
  if ( ! nnpTask->bPopParamInt(iVal) )
    THROW_INVALID_PARAM("jumpFalse");
  
  // cell points to the jump arg
  if (iVal)
    nnpTask->m_pCell++;
  else
    nnpTask->m_pCell = nnpTask->m_pCell->m_pJumpLocn;
}

/*----------------------------------------------------------------------------*/
/* jumpTrue -- jump on true to location                                       */
/*----------------------------------------------------------------------------*/
void Task::vJumpTrue(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vJumpTrue");
  
  i8 iVal;
  if ( ! nnpTask->bPopParamInt(iVal) )
    THROW_INVALID_PARAM("jumpTrue");
  
  // cell points to the jump arg
  if (iVal)
    nnpTask->m_pCell = nnpTask->m_pCell->m_pJumpLocn;
  else
    nnpTask->m_pCell++;
}

/*----------------------------------------------------------------------------*/
/* < -- test top two integers on data stack for less than                     */
/*----------------------------------------------------------------------------*/
void Task::vLess(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLess");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("<");
  
  r = i < j;
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* <= -- test top two integers on data stack for less than equal to           */
/*----------------------------------------------------------------------------*/
void Task::vLessEqual(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLessEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("<=");
  
  r = i <= j;
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* literal -- push contents of literal buffer as string onto param stack      */
/*----------------------------------------------------------------------------*/
void Task::vLiteral(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLiteral");
  
  nnpTask->vPushParam(nnpTask->m_sLiteral.a());
}

/*----------------------------------------------------------------------------*/
/* literalGet -- get next literal and push onto stack                         */
/*----------------------------------------------------------------------------*/
void Task::vLiteralGet(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLiteralGet");
  
  char* aLiteral = nnpTask->m_sLiteral.a();
  
  nnpTask->m_eLiteral = nnpTask->m_xInputStream.eNextLiteral(aLiteral);
  
  nnpTask->vPushParam(nnpTask->m_sLiteral.a());
}

/*----------------------------------------------------------------------------*/
/* litNext -- get next literal                                                */
/*----------------------------------------------------------------------------*/
void Task::vLitNext(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLitNext");
  
  if (nnpTask->m_bTracing)
    ASSERT(nnpTask->m_bTracing);
  
  // litNext is only used in docol
  
  char* aLiteral = nnpTask->m_sLiteral.a();
  
  nnpTask->m_eLiteral = nnpTask->m_xInputStream.eNextLiteral(aLiteral);
}

/*----------------------------------------------------------------------------*/
/* litType -- push literal type                                               */
/*----------------------------------------------------------------------------*/
void Task::vLitType(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLitType");
  
  //eLT eLiteralType = nnpTask->m_xInputStream.eLiteralType();
  eLT eLiteralType = nnpTask->m_eLiteral;
  
  nnpTask->vPushParam(eLiteralType);
}

/*----------------------------------------------------------------------------*/
/* locate -- locate word based on ip                                          */
/*----------------------------------------------------------------------------*/
void Task::vLocate(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLocate");
  
  Cell* pCell;
  
  if ( ! nnpTask->bPopParam(pCell) )
    THROW_INVALID_PARAM("locate");
  
  Word* pLocatedWord = nnpTask->pLocateWord(pCell);
  ASSERT(pLocatedWord);
  
  nnpTask->vPushParam(pLocatedWord->sName().a());
}

/*----------------------------------------------------------------------------*/
/* loop -- compile time for do ... loop                            immediate  */
/*----------------------------------------------------------------------------*/
void Task::vLoop(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoop");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  IPval xPtrDoLoc;
  nnpTask->vPopReturn(&xPtrDoLoc);
  
  Cell* pDoLoc = xPtrDoLoc.pCell();
  
  pCurrentWord->vEncode("Loop_RT");
  
  pCurrentWord->vEncode("jump",pDoLoc);
}

/*----------------------------------------------------------------------------*/
/* loop_rt -- runtime for do loop                                             */
/*----------------------------------------------------------------------------*/
void Task::vLoop_RT(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoop_RT");
  
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
}

/*----------------------------------------------------------------------------*/
/* loopPlus -- do +loop                                                       */
/*----------------------------------------------------------------------------*/
void Task::vLoopPlus(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoopPlus");

  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pDoLoc;
  nnpTask->vPopReturn(pDoLoc);
    
  pCurrentWord->vEncode("+Loop_RT");
  
  pCurrentWord->vEncode("jump",pDoLoc);
}

/*----------------------------------------------------------------------------*/
/* loopPlus_rt -- runtime for do +loop                                        */
/*----------------------------------------------------------------------------*/
void Task::vLoopPlus_RT(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoopPlus_RT");
  
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
}

/*----------------------------------------------------------------------------*/
/* magicFetch -- fetch magic number from Object object                      */
/*----------------------------------------------------------------------------*/
void Task::vMagicFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vMagicFetch");

  PtrVal xPtrObject;
  nnpTask->vPopParam(&xPtrObject);
  
  i8 iMagic = xPtrObject.pObject()->iMagic();

  nnpTask->vPushParam(iMagic);
}

/*----------------------------------------------------------------------------*/
/* magicMark -- put magic number into Object object                           */
/*----------------------------------------------------------------------------*/
void Task::vMagicMark(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vMagicMark");
  
  PtrVal xPtrObject;
  nnpTask->vPopParam(&xPtrObject);
  
  xPtrObject.pObject()->vSetMagic();
}

/*----------------------------------------------------------------------------*/
/* * multiply second integer by top integer on stack                          */
/*----------------------------------------------------------------------------*/
void Task::vMultiply(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vMultiply");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("*");
  
  r = i * j;
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* nameWord -- push name of word                                              */
/*----------------------------------------------------------------------------*/
void Task::vNameWord(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vNameWord");
  
  Word* pWord;

  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("nameWord");
  
  nnpTask->vPushParam(pWord->sName().a());
}

/*----------------------------------------------------------------------------*/
/* neg -- gegate top integer on stack                                         */
/*----------------------------------------------------------------------------*/
void Task::vNeg(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vNeg");
  
  i8 r, i;
  
  if ( ! nnpTask->bPopParam(i) )
    THROW_INVALID_PARAM("neg");
  
  r = -1 * i;
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* next -- compile time for iterate ... next                        immediate */
/*----------------------------------------------------------------------------*/
void Task::vNext(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vNext");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pJumpFalseArg;
  nnpTask->vPopReturn(pJumpFalseArg);
  
  Cell* pIterateLoc;
  nnpTask->vPopReturn(pIterateLoc);
  
  //----------------------------------------------------------------------------
  
  pCurrentWord->vEncode("jump",pIterateLoc);
  
  pJumpFalseArg->m_pJumpLocn = pCurrentWord->pLastCell();
}

/*----------------------------------------------------------------------------*/
/* nop -- no op                                                               */
/*----------------------------------------------------------------------------*/
void Task::vNop(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vNop");
}

/*----------------------------------------------------------------------------*/
/* not -- not of top value                                                    */
/*----------------------------------------------------------------------------*/
void Task::vNot(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vNot");
  
  i8 iValA;
  if ( ! nnpTask->bPopParamInt(iValA) )
    THROW_INVALID_PARAM("not");
  
  i8 iNotA = iValA ? 0 : 1;
  
  nnpTask->vPushParam(iNotA);
}

/*----------------------------------------------------------------------------*/
/* != -- test top two integers on data stack for not equal                    */
/*----------------------------------------------------------------------------*/
void Task::vNotEqual(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vNotEqual");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("!=");
  
  r = i != j;
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* object@ -- fetch object from return stack onto param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vObjectFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vObjectFetch");
  
  i8 iPick = 0;
  
  ValWsp xObjectWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xObjectWsp);
  
  nnpTask->vPushParam(xObjectWsp);
}

/*----------------------------------------------------------------------------*/
/* object! -- replace object on return stack from param stack                 */
/*----------------------------------------------------------------------------*/
void Task::vObjectStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vObjectStore");
  
  i8 iPeek = 0;
  
  ValWsp xObjectWsp;
  
  if ( ! nnpTask->bPopParam(xObjectWsp) )
    THROW_INVALID_PARAM("object!");
  
  nnpTask->m_xReturnStack.vPoke(iPeek,&xObjectWsp);
}

/*----------------------------------------------------------------------------*/
/* object!! -- mark current word as an object                                 */
/*----------------------------------------------------------------------------*/
void Task::vObjectMark(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vObjectMark");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  //pCurrentWord->vImmediate(true);
  pCurrentWord->vWordType(WT_OBJECT);
  pCurrentWord->vFunc(&vEncodeEmb);
}

/*----------------------------------------------------------------------------*/
/* or -- logical or of top two values                                         */
/*----------------------------------------------------------------------------*/
void Task::vOr(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vOr");
  
  i8 iArgB;
  if ( ! nnpTask->bPopParamInt(iArgB) )
    THROW_INVALID_PARAM("or");
  
  i8 iArgA;
  if ( ! nnpTask->bPopParamInt(iArgA) )
    THROW_INVALID_PARAM("or");
  
  i8 iArgR = (iArgA || iArgB) ? 1 : 0;
  
  nnpTask->vPushParam(iArgR);
}

/*----------------------------------------------------------------------------*/
/* switch -- task switch                                                      */
/*----------------------------------------------------------------------------*/
void Task::vSwitch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSwitch");
  
  nnpTask->vState(TS_SWITCH);
}

/*----------------------------------------------------------------------------*/
/* pick -- place copy of nth value in stack onto stack                        */
/*----------------------------------------------------------------------------*/
void Task::vPick(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vPick");

  i8 iPick;
  if ( ! nnpTask->bPopParamInt(iPick) )
    THROW_INVALID_PARAM("pick");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xParamStack.vPeek(iPick,&xValWsp);

  nnpTask->vPushParam(xValWsp);
}

/*----------------------------------------------------------------------------*/
/* prevWord -- push prev word                                                 */
/*----------------------------------------------------------------------------*/
void Task::vPrevWord(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vPrevWord");
  
  Word* pWord;
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("prevWord");
    
  Word* pPrevWord = pWord->pPrevWord();
  
  nnpTask->vPushParam(pPrevWord);
}

/*----------------------------------------------------------------------------*/
/* . -- print top value on stack                                              */
/*----------------------------------------------------------------------------*/
void Task::vPrint(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vPrint");
  
  s16K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->bPopParam(xValWsp) )
    THROW_INVALID_PARAM(".");
  
  xValWsp.vToString(sWorkspace.a());
  vTrace("[%p %s] %s\n",*nnpTask,nnpTask->m_sName.a(),sWorkspace.a());
}

/*----------------------------------------------------------------------------*/
/* processExit -- process exits early                                         */
/*----------------------------------------------------------------------------*/
void Task::vProcessExit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vProcessExit");
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* push -- push value arg onto stack                                          */
/*----------------------------------------------------------------------------*/
void Task::vPush(NNP_Task nnpTask)
{ 
  nnpTask->vPrimitive("vPush");
  
  OnStack* pValue = nnpTask->m_pValue;
  
  nnpTask->vPushParam(pValue);
  
  nnpTask->vAdjustNext(pValue->iSize());
}

/*----------------------------------------------------------------------------*/
/* pushEmbedded -- push ptr to embedded object                                */
/*----------------------------------------------------------------------------*/
void Task::vPushEmbedded(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vPushEmbedded");
  
  PtrVal xPtrObject;
  xPtrObject = nnpTask->m_pValue;
  
  nnpTask->vPushParam(&xPtrObject);
}

/*----------------------------------------------------------------------------*/
/* .put -- print top value on stack (no new line)                             */
/*----------------------------------------------------------------------------*/
void Task::vPut(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vPut");
  
  s16K sWorkspace;
  
  ValWsp xValWsp;
  if ( ! nnpTask->bPopParam(xValWsp) )
    THROW_INVALID_PARAM(".put");
  
  xValWsp.vToString(sWorkspace.a());
  vTrace("%s",sWorkspace.a());
}

/*----------------------------------------------------------------------------*/
/* .putNewline -- print new line                                              */
/*----------------------------------------------------------------------------*/
void Task::vPutNewline(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vPut");
  
  vTrace("\n");
}

/*----------------------------------------------------------------------------*/
/* quit -- system quit (aka process exits)                                    */
/*----------------------------------------------------------------------------*/
void Task::vQuit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vQuit");

  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* rdtscp -- get the time                                                     */
/*----------------------------------------------------------------------------*/
void Task::vRdtscp(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRdtscp");
  
  IntVal xValInt;
  xValInt = iRdtscp();

  nnpTask->vPushParam(&xValInt);
}

/*----------------------------------------------------------------------------*/
/* repeat -- repeat a begin-while-repeat loop                       immediate */
/*----------------------------------------------------------------------------*/
void Task::vRepeat(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRepeat");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();

  Cell* pWhileJumpArg;
  nnpTask->vPopReturn(pWhileJumpArg);
  
  //-------------------------------------------------------------------------

  Cell* pBeginLoc;
  nnpTask->vPopReturn(pBeginLoc);

  pCurrentWord->vEncode("jump",pBeginLoc);
  
  //-------------------------------------------------------------------------
  
  pWhileJumpArg->m_pJumpLocn = pCurrentWord->pLastCell();
}

/*----------------------------------------------------------------------------*/
/* retryPoint -- mark retry point                                             */
/*----------------------------------------------------------------------------*/
void Task::vRetryPoint(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRetryMark");
  
  nnpTask->m_pResetIP = nnpTask->m_pCell;
  
  nnpTask->m_xParamStack.vRetryPoint();
  nnpTask->m_xReturnStack.vRetryPoint();
}

/*----------------------------------------------------------------------------*/
/* rot -- rotate top three values on param stack                              */
/*----------------------------------------------------------------------------*/
void Task::vRot(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRot");
  
  ValWsp xValA;
  ValWsp xValB;
  ValWsp xValC;
  
  if ( ! nnpTask->bPopParam(xValA,xValB,xValC) )
    THROW_INVALID_PARAM("rot");
  
  nnpTask->vPushParam(&xValA);
  nnpTask->vPushParam(&xValC);
  nnpTask->vPushParam(&xValB);
}

/*----------------------------------------------------------------------------*/
/* rstack.depth -- put number of values in return stack onto param stack      */
/*----------------------------------------------------------------------------*/
void Task::vRstackDepth(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRstackDepth");
  
  i8 iDepth = nnpTask->m_xReturnStack.iValues();
  
  nnpTask->vPushParam(iDepth);
}

/*----------------------------------------------------------------------------*/
/* rdrop -- discard top value off rstack                                      */
/*----------------------------------------------------------------------------*/
void Task::vRstackDrop(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRstackDrop");
  
  // should just discard on stack
  
  ValWsp xValA;
  nnpTask->vPopReturn(&xValA);
}

/*----------------------------------------------------------------------------*/
/* r@ -- place copy of nth value in return stack onto param stack             */
/*----------------------------------------------------------------------------*/
void Task::vRstackFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRstackFetch");
  
  i8 iPick;
  if ( ! nnpTask->bPopParamInt(iPick) )
    THROW_INVALID_PARAM("r@");
  
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xValWsp);
  
  nnpTask->vPushParam(xValWsp);
}

/*----------------------------------------------------------------------------*/
/* rpick -- place copy of nth value in return stack onto param stack          */
/*----------------------------------------------------------------------------*/
void Task::vRstackPick(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRstackPick");
  
  i8 iPick;
  if ( ! nnpTask->bPopParamInt(iPick) )
    THROW_INVALID_PARAM("rpick");
    
  // could do this directly on stack
  
  ValWsp xValWsp;
  
  nnpTask->m_xReturnStack.vPeek(iPick,&xValWsp);
  
  nnpTask->vPushParam(xValWsp);
}

/*----------------------------------------------------------------------------*/
/* see -- dump routine                                                        */
/*----------------------------------------------------------------------------*/
void Task::vSee(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSee");
  
  nnpTask->vLiteralNext();
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_sLiteral);

  if ( ! pWord )
    THROW("See: %s unknown",nnpTask->m_sLiteral.a());
  
  vTrace("See: %s\n",pWord->sName().a());
  
  
  BodyField* pBody = pWord->pBodyField();
  Cell* pCell = pBody->m_pCell;
  
  s16K sWorkspace;
  
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
      //OnStack* pValue = reinterpret_cast<OnStack*>(pCell);
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
}

/*----------------------------------------------------------------------------*/
/* ; -- end compile routine                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSemicolon(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSemicolon");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();

  pCurrentWord->vEncode("exit");
  
  nnpTask->m_bCompiling = false;
}

/*----------------------------------------------------------------------------*/
/* signal -- signal tx on stack                                               */
/*----------------------------------------------------------------------------*/
void Task::vSignal(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSignal");
  
  // ..tbd..
}

/*----------------------------------------------------------------------------*/
/* sleep -- sleep for n seconds                                               */
/*----------------------------------------------------------------------------*/
void Task::vSleep(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSleep");
  
  i8 nSeconds;
  
  if ( ! nnpTask->bPopParam(nSeconds) )
    THROW_INVALID_PARAM("sleep");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepSec(nSeconds);
}

/*----------------------------------------------------------------------------*/
/* snooze -- snooze briefly                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSnooze(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSnooze");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepNano(100_MB);
}

/*----------------------------------------------------------------------------*/
/* ! -- store value                                                           */
/*----------------------------------------------------------------------------*/
void Task::vStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vStore");
  
  PtrVal xPtr;
  nnpTask->vPopParam(&xPtr);

  ValWsp xVal;
  
  if ( ! nnpTask->bPopParam(xVal) )
    THROW_INVALID_PARAM("!");
    
  xPtr.vStore(xVal);
}

/*----------------------------------------------------------------------------*/
/* streql -- test string equality                                             */
/*----------------------------------------------------------------------------*/
void Task::vStrEql(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vStrEql");
  
  ValWsp xStringA;
  ValWsp xStringB;
  
  // get the arguments
  if ( ! nnpTask->bPopParam(xStringB,xStringA) )
    THROW_INVALID_PARAM("streql");
  
  i8 iEqual = String::bEqualTo(xStringA.sStr(),xStringB.sStr());
  
  nnpTask->vPushParam(iEqual);
}

/*----------------------------------------------------------------------------*/
/* strlen -- string length                                                    */
/*----------------------------------------------------------------------------*/
void Task::vStrLen(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vStrLen");
  
  i8 iLength;
  ValWsp xString;
  
  // get the arguments
  if ( ! nnpTask->bPopParam(xString) )
    THROW_INVALID_PARAM("strlen");
  
  StrVal* pStrVal = xString.pAsStrVal();
  
  iLength = String::iLength(pStrVal->sStr());
  
  nnpTask->vPushParam(iLength);
}

/*----------------------------------------------------------------------------*/
/* substr -- substring ( start length str == substr )                         */
/*----------------------------------------------------------------------------*/
void Task::vSubStr(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSubStr");
  
  i8 iLength;
  i8 iStart;
  ValWsp xString;
  ValWsp xSubStr;
  
  StrVal* pStrVal = xString.pAsStrVal();
  
  // get the arguments
  if ( ! nnpTask->bPopParam(iStart,iLength,xString) )
    THROW_INVALID_PARAM("substr");
  
  // validate start index
  if ( iStart < 0 || iStart >= pStrVal->iChars())
    THROW("Invalid start index for: subStr");
  
  // validate length
  if ( iLength <= 0 || iStart + iLength >= pStrVal->iChars())
    THROW("Invalid length for: substr");

  // compute substring
  //xSubStr.vCopyString(xString.sStr() + iStartIndex,iLength);
  
  char* pStart = xString.sStr() + iStart;
  
  xSubStr.pAsStrVal()->vCopyString(pStart,iLength);
  
  nnpTask->vPushParam(&xSubStr);
}

/*----------------------------------------------------------------------------*/
/* - -- subtract second integer from top integer on stack                     */
/*----------------------------------------------------------------------------*/
void Task::vSubtract(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSubtract");
  
  i8 r, j, i;
  
  if ( ! nnpTask->bPopParam(j,i) )
    THROW_INVALID_PARAM("-");
  
  r = i - j;
  
  nnpTask->vPushParam(r);
}

/*----------------------------------------------------------------------------*/
/* swap -- swap top two values on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vSwap(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSwap");
  
  ValWsp xValA;
  ValWsp xValB;
  
  if ( ! nnpTask->bPopParam(xValA,xValB) )
    THROW_INVALID_PARAM("swap");
  
  nnpTask->vPushParam(&xValA);
  nnpTask->vPushParam(&xValB);
}

/*----------------------------------------------------------------------------*/
/* test -- print test                                                         */
/*----------------------------------------------------------------------------*/
void Task::vTest(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTest");
  vTrace("[%s] Test\n",nnpTask->m_sName.a());
}

/*----------------------------------------------------------------------------*/
/* then -- then clause                                                        */
/*----------------------------------------------------------------------------*/
void Task::vThen(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThen");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pIfJumpArg;
  nnpTask->vPopReturn(pIfJumpArg);
  
  pIfJumpArg->m_pJumpLocn = pCurrentWord->pLastCell();
}

/*----------------------------------------------------------------------------*/
/* throw -- throw c++ exception (for testing purposes)                        */
/*----------------------------------------------------------------------------*/
void Task::vThrow(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThrow");
  
  char wBuffer[256];
  
  String::vSprint(wBuffer,"THROW TEST");
  
  throw wBuffer;
}

/*----------------------------------------------------------------------------*/
/* tick -- find next word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTick(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTick");
  
  nnpTask->vLiteralNext();
  
//  if ( ! bExtracted ) {
//    String::vSprint(nnpTask->m_sException,"No name: ' ");
//    nnpTask->m_sFile = __FILE__;
//    throw (i8)__LINE__;
//  }
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_sLiteral);

  if ( ! pWord )
    THROW("No word: %s ",nnpTask->m_sLiteral.a());

  if (nnpTask->m_bCompiling) {
    Word* pCurrentWord = nnpTask->pCurrentWord();
    pCurrentWord->vEncode("push",pWord);
  }
  else {
    nnpTask->vPushParam(pWord);
  }
}

/*----------------------------------------------------------------------------*/
/* >body -- convert word pointer to body pointer                              */
/*----------------------------------------------------------------------------*/
void Task::vToBody(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vToBody");
  
  Word* pWord;
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM(">body");

  PtrVal xPtrBody;
  xPtrBody = pcAdd(pWord,sizeof(Word));

  nnpTask->vPushParam(&xPtrBody);
}

/*----------------------------------------------------------------------------*/
/* >cmd -- convert stack values to cmd string                                 */
/*----------------------------------------------------------------------------*/
void Task::vToCmd(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vToCmd");

  if ( ! nnpTask->bGuardParam(CN_INTVAL) )
    THROW_INVALID_PARAM(">cmd");
  
  IntVal xNumValues;
  nnpTask->vPopParam(&xNumValues);

  s16K sCmd;

  ValWsp xStr;
  xStr = "<cmd>";
  
  for (i8 i = 0; i < xNumValues.iInt(); i++) {
    ValWsp xValWsp;
    nnpTask->m_xParamStack.vPeek(i,&xValWsp);

    if (i)
      String::vConcat(sCmd.a()," ");
    
    xValWsp.vToCommand(sCmd.z());
  }
  
  xStr = sCmd.a();
  
  nnpTask->vPushParam(&xStr);
}

/*----------------------------------------------------------------------------*/
/* >int -- convert value to int                                               */
/*----------------------------------------------------------------------------*/
void Task::vToInt(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vToInt");
  
  ValWsp xStr;
  if ( ! nnpTask->bPopParam(xStr) )
    THROW_INVALID_PARAM(">int");
  
  s256 sBuff;
  xStr.vToString(sBuff.a());
  
  i8 i = String::iToI8(sBuff.a());

  nnpTask->vPushParam(i);
}

/*----------------------------------------------------------------------------*/
/* r> -- move value from rstack to pstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vToPstack(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vToPstack");
  
  ValWsp xVal;
  if ( ! nnpTask->bPopReturn(xVal) )
    THROW_INVALID_PARAM("r>");
  
  nnpTask->vPushParam(xVal);
}

/*----------------------------------------------------------------------------*/
/* >r -- move value from pstack to rstack                                     */
/*----------------------------------------------------------------------------*/
void Task::vToRstack(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vToRstack");
  
  ValWsp xVal;
  if ( ! nnpTask->bPopParam(xVal) )
    THROW_INVALID_PARAM(">r");

  nnpTask->vPushReturn(xVal);
}

/*----------------------------------------------------------------------------*/
/* >seitf -- convert int to seitf ptr                                         */
/*----------------------------------------------------------------------------*/
void Task::vToSEitf(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vToSEitf");
  
  i8 iVal;
  
  if ( ! nnpTask->bPopParam(iVal) )
    THROW_INVALID_PARAM(">seitf");
  
  PtrVal xPtrSEitf;
  xPtrSEitf = reinterpret_cast<SEitf*>(iVal);
  
  nnpTask->vPushParam(&xPtrSEitf);
}

/*----------------------------------------------------------------------------*/
/* >str -- convert value to str                                               */
/*----------------------------------------------------------------------------*/
void Task::vToStr(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vToStr");
  
  ValWsp xVal;
  if ( ! nnpTask->bPopParam(xVal) )
    THROW_INVALID_PARAM(">str");

  char wBuffer[64];
  xVal.vToString(wBuffer);

  nnpTask->vPushParam(wBuffer);
}

/*----------------------------------------------------------------------------*/
/* >task -- convert int to task ptr                                           */
/*----------------------------------------------------------------------------*/
void Task::vToTask(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vToTask");
  
  i8 iVal;

  if ( ! nnpTask->bPopParam(iVal) )
    THROW_INVALID_PARAM(">task");
  
  TaskVal xTask;
  xTask = reinterpret_cast<Task*>(iVal);
  
  nnpTask->vPushParam(&xTask);
}

/*----------------------------------------------------------------------------*/
/* >valset -- collapse values on pstack to a valset                           */
/*----------------------------------------------------------------------------*/
void Task::vToValSet(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vToValSet");

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
    
    //vTrace("%s\n",xVal.sStr());
    
    xValSet.vAppend(pCursor,xVal);
  }
  
  nnpTask->vPushParam(xValSet);
}

/*----------------------------------------------------------------------------*/
/* >values -- expand valset to values on pstack                               */
/*----------------------------------------------------------------------------*/
void Task::vToValues(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vToValues");
 
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
}

/*----------------------------------------------------------------------------*/
/* turn tracing off                                                           */
/*----------------------------------------------------------------------------*/
void Task::vTraceOff(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTraceOff");
  
  nnpTask->m_bTracing = false;
}

/*----------------------------------------------------------------------------*/
/* turn tracing on                                                            */
/*----------------------------------------------------------------------------*/
void Task::vTraceOn(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTraceOn");
  
  nnpTask->m_bTracing = true;
}

/*----------------------------------------------------------------------------*/
/* unloop -- pop the return stack                                             */
/*----------------------------------------------------------------------------*/
void Task::vUnloop(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vUnloop");
  
  // we should just modify the pointers rather than actually pop
  ValWsp xVal;
  nnpTask->vPopReturn(&xVal);
}

/*----------------------------------------------------------------------------*/
/* until -- repeat begin loop until true                                      */
/*----------------------------------------------------------------------------*/
void Task::vUntil(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vUntil");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  Cell* pBeginLoc;
  nnpTask->vPopReturn(pBeginLoc);
  
  //-------------------------------------------------------------------------
  
  pCurrentWord->vEncode("jumpFalse",pBeginLoc);
}

/*----------------------------------------------------------------------------*/
/* version -- push version string                                             */
/*----------------------------------------------------------------------------*/
void Task::vVersion(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSersion");
  
  s256 sVersion;
  sVersion = VERSION_STRING;
  
  nnpTask->vPushParam(sVersion);
}

/*----------------------------------------------------------------------------*/
/* wait -- this task waits for signal                                         */
/*----------------------------------------------------------------------------*/
void Task::vWait(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vWait");
  
  // ..tbd..
}

/*----------------------------------------------------------------------------*/
/* wake -- wake task on stack                                                 */
/*----------------------------------------------------------------------------*/
void Task::vWake(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vWake");
  
  // ..tbd..
}

/*----------------------------------------------------------------------------*/
/* while -- check for continue with loop                            immediate */
/*----------------------------------------------------------------------------*/
void Task::vWhile(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vWhile");

  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("jumpFalse");
  Cell* pWhileJumpArg = (Cell*)pCurrentWord->pAppend((i8)0);
  
  nnpTask->vPushReturn(pWhileJumpArg);
}

/*----------------------------------------------------------------------------*/
/* 0< -- test if top value is less than zero                                  */
/*----------------------------------------------------------------------------*/
void Task::vZeroLess(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vZeroLess");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0<");
  
  if (iZero < 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
}

/*----------------------------------------------------------------------------*/
/* 0<= -- test if top value is less than or equal to zero                     */
/*----------------------------------------------------------------------------*/
void Task::vZeroLessEqual(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vZeroLessEqual");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0<=");
  
  if (iZero <= 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
}

/*----------------------------------------------------------------------------*/
/* 0== -- test if top value is equal to zero / nullptr                        */
/*----------------------------------------------------------------------------*/
void Task::vZeroEqual(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vZeroEqual");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0==");
  
  if (iZero == 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
}

/*----------------------------------------------------------------------------*/
/* 0!= -- test if top value is not equal to zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroNotEqual(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vZeroNotEqual");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0!=");
  
  if (iZero != 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
}

/*----------------------------------------------------------------------------*/
/* zeroGreaterEqual -- test if top value is greater than or equal to zero     */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreaterEqual(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vZeroGreaterEqual");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0>=");
  
  if (iZero >= 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
}

/*----------------------------------------------------------------------------*/
/* 0> -- test if top value is greater than zero                              */
/*----------------------------------------------------------------------------*/
void Task::vZeroGreater(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vZeroGreater");
  
  i8 iZero;
  if ( ! nnpTask->bPopParamInt(iZero) )
    THROW_INVALID_PARAM("0>");
  
  if (iZero > 0)
    nnpTask->vPushParam((i8)1);
  else
    nnpTask->vPushParam((i8)0);
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
  nnpTask->vPrimitive("vAssertStack_0");

  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (0 == iStackValues)  )
    THROW("assert.stack_0");
}

/*----------------------------------------------------------------------------*/
/* assert.stack_0+ -- asssert 0 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_0more(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAssertStack_0more");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack0+");
}

/*----------------------------------------------------------------------------*/
/* assert.stack_1 -- asssert 1 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_1(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAssertStack_1");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (1 == iStackValues)  )
    THROW("assert.stack_1");
}

/*----------------------------------------------------------------------------*/
/* assert.stack_1+ -- asssert 0 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_1more(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAssertStack_1more");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (1 <= iStackValues) )
    THROW("assert.stack1+");
}

/*----------------------------------------------------------------------------*/
/* assert.stack_2 -- asssert 2 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_2(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAssertStack_2");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (2 == iStackValues)  )
    THROW("assert.stack_2");
}

/*----------------------------------------------------------------------------*/
/* assert.stack_2+ -- asssert 2 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_2more(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAssertStack_2more");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (2 <= iStackValues) )
    THROW("assert.stack0+");
}

/*----------------------------------------------------------------------------*/
/* assert.stack_3 -- asssert 3 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_3(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAssertStack_3");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (3 == iStackValues)  )
    THROW("assert.stack_3");
}

/*----------------------------------------------------------------------------*/
/* assert.stack_3+ -- asssert 3 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_3more(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAssertStack_3more");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack3+");
}

/*----------------------------------------------------------------------------*/
/* assert.stack_4 -- asssert 4 values on stack                                */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_4(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAssertStack_4");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (4 == iStackValues)  )
    THROW("assert.stack_4");
}

/*----------------------------------------------------------------------------*/
/* assert.stack_4+ -- asssert 4 or more values on stack                       */
/*----------------------------------------------------------------------------*/
void Task::vAssertStack_4more(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vAssertStack_4more");
  
  i8 iStackValues = nnpTask->m_xParamStack.iValues();
  
  if ( ! (0 <= iStackValues) )
    THROW("assert.stack4+");
}

// args: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* args.print -- print arguments                                              */
/*----------------------------------------------------------------------------*/
void Task::vArgsPrint(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vArgsPrint");
  
  gxArguments.vPrint();
}

// csv: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* csv.firstValue -- get first value from csv string                          */
/*----------------------------------------------------------------------------*/
void Task::vCsvFirstValue(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vCsvFirstValue");
  
  ValWsp xStr;
  if ( ! nnpTask->bPopParam(xStr) )
    THROW_INVALID_PARAM("csv.firstValue");
  
  s256 sFirstValue;
  
  char* z = xStr.sStr();
  
  if (! String::bExtractItem(&z,sFirstValue.a()))
    sFirstValue = "";
  
  nnpTask->vPushParam(sFirstValue);
}

// dic: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* dic.count -- get number of dictionaries on dictionary stack                */
/*----------------------------------------------------------------------------*/
void Task::vDicCount(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDicCount");
  
  i8 iCount;
  
  if (nnpTask->m_pSEitf) {
    iCount = nnpTask->m_pSEitf->pDicStack()->iCount();
  }
  else {
    iCount = nnpTask->m_xDictionaryStack.iCount();
  }
  
  nnpTask->vPushParam(iCount);
}

/*----------------------------------------------------------------------------*/
/* dic.drop -- discard top of dictionary stack                                */
/*----------------------------------------------------------------------------*/
void Task::vDicDrop(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDicDrop");
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->vPopDic();
  }
  else {
    nnpTask->m_xDictionaryStack.vDiscard();
  }
}

/*----------------------------------------------------------------------------*/
/* dic.stack -- print dictionary stack                                        */
/*----------------------------------------------------------------------------*/
void Task::vDicStack(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDicDump");
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->pDicStack()->vDump();
  }
  else {
    nnpTask->m_xDictionaryStack.vDump();
  }
}

/*----------------------------------------------------------------------------*/
/* dic.init -- initialise a dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vDicInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDicInit");
  
  PtrVal xDicPtr;
  
  if ( ! nnpTask->bPopParam(xDicPtr) )
    THROW_INVALID_PARAM("dic.init");
  
  Dictionary* pDictionary = reinterpret_cast<Dictionary*>(xDicPtr.pPtr());

  pDictionary->vInit();
}

/*----------------------------------------------------------------------------*/
/* dic.last -- get last word of dictionary                                    */
/*----------------------------------------------------------------------------*/
void Task::vDicLast(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDicLast");
  
  PtrVal xDicPtr;
  
  if ( ! nnpTask->bPopParam(xDicPtr) )
    THROW_INVALID_PARAM("dic.last");
  
  Dictionary* pDictionary = reinterpret_cast<Dictionary*>(xDicPtr.pPtr());
  
  Word* pLastWord = pDictionary->pCurrentWord();
  
  nnpTask->vPushParam(pLastWord);
}

/*----------------------------------------------------------------------------*/
/* dic.main -- push main dictionary onto stack                                */
/*----------------------------------------------------------------------------*/
void Task::vDicMain(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDicMain");
  
  PtrVal xPtrDic;
  xPtrDic = gpDictionary;
  
  nnpTask->vPushParam(xPtrDic);
}

/*----------------------------------------------------------------------------*/
/* dic.name -- push name of dictionary onto stack                             */
/*----------------------------------------------------------------------------*/
void Task::vDicName(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDicNain");
 
  PtrVal xDicPtr;
  if ( ! nnpTask->bPopParam(xDicPtr) )
    THROW_INVALID_PARAM("dic.push");
  
  Dictionary* pDictionary = reinterpret_cast<Dictionary*>(xDicPtr.pPtr());

  ValWsp sName;
  sName = pDictionary->sName().a();
  
  nnpTask->vPushParam(sName);
}

/*----------------------------------------------------------------------------*/
/* dic.pick -- put a dictionary from dictionary stack onto param stack        */
/*----------------------------------------------------------------------------*/
void Task::vDicPick(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDicPick");
  
  i8 iDic;
  if ( ! nnpTask->bPopParam(iDic) )
    THROW_INVALID_PARAM("dic.pick");
  
  PtrVal xPtrDic;

  if (nnpTask->m_pSEitf) {
    xPtrDic = nnpTask->m_pSEitf->pDicStack()->pPick(iDic);
  }
  else {
    xPtrDic = nnpTask->m_xDictionaryStack.pPick(iDic);
  }
  
  nnpTask->vPushParam(xPtrDic);
}

/*----------------------------------------------------------------------------*/
/* dic.push -- push a dictionary from param stack onto dictionary stack       */
/*----------------------------------------------------------------------------*/
void Task::vDicPush(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDicPush");
  
  PtrVal xDicPtr;
  
  if ( ! nnpTask->bPopParam(xDicPtr) )
    THROW_INVALID_PARAM("dic.push");

  Dictionary* pDictionary = reinterpret_cast<Dictionary*>(xDicPtr.pPtr());
  
  if (nnpTask->m_pSEitf) {
    nnpTask->m_pSEitf->vPushDic(pDictionary);
  }
  else {
    nnpTask->m_xDictionaryStack.vPush(pDictionary);
  }
}

// file: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* file.close -- close a file                                                 */
/*----------------------------------------------------------------------------*/
void Task::vFileClose(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFileInit");
  
  PtrVal xFilePtr;
  
  if ( ! nnpTask->bPopParam(xFilePtr) )
    THROW_INVALID_PARAM("file.close");
  
  File* pFile = reinterpret_cast<File*>(xFilePtr.pPtr());
  
  pFile->vClose();
}

/*----------------------------------------------------------------------------*/
/* file.delete -- delete a file                                               */
/*----------------------------------------------------------------------------*/
void Task::vFileDelete(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFileDelete");
  
  PtrVal xFilePtr;
  
  if ( ! nnpTask->bPopParam(xFilePtr) )
    THROW_INVALID_PARAM("file.delete");
  
  File* pFile = reinterpret_cast<File*>(xFilePtr.pPtr());

  Object* pFileObject = reinterpret_cast<Object*>(pFile);
  delete pFile;
  
  gpSmallHeap->vFreeObject(pFileObject);
}

/*----------------------------------------------------------------------------*/
/* file.init -- initialise a file                                             */
/*----------------------------------------------------------------------------*/
void Task::vFileInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFileInit");
  
  PtrVal xFilePtr;
  
  if ( ! nnpTask->bPopParam(xFilePtr) )
    THROW_INVALID_PARAM("file.init");
  
  File* pFile = reinterpret_cast<File*>(xFilePtr.pPtr());
  
  pFile->vInit();
}

/*----------------------------------------------------------------------------*/
/* file.new -- new a file                                                     */
/*----------------------------------------------------------------------------*/
void Task::vFileNew(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFileNew");
  
  void* pFileAddress = gpSmallHeap->pAllocObject();
  File* pFile = new(pFileAddress) File;
  
  PtrVal xFilePtr;
  xFilePtr = pFile;
  
  nnpTask->vPushParam(xFilePtr);
}

/*----------------------------------------------------------------------------*/
/* file.open -- open a file                                                   */
/*----------------------------------------------------------------------------*/
void Task::vFileOpen(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFileOpen");
  
  PtrVal xFilePtr;
  
  if ( ! nnpTask->bPopParam(xFilePtr) )
    THROW_INVALID_PARAM("file.open");
  
  File* pFile = reinterpret_cast<File*>(xFilePtr.pPtr());
  
  s32 sMode;
  
  if ( ! nnpTask->bPopParam(sMode) )
    THROW("file.open");
  
  bool bOpened = pFile->bOpen(sMode.a());
  
  nnpTask->vPushParam(bOpened);
}

/*----------------------------------------------------------------------------*/
/* file.path@ -- get file path                                                */
/*----------------------------------------------------------------------------*/
void Task::vFilePathGet(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFilePathGet");
  
  PtrVal xFilePtr;
  
  if ( ! nnpTask->bPopParam(xFilePtr) )
    THROW_INVALID_PARAM("file.path@");
  
  File* pFile = reinterpret_cast<File*>(xFilePtr.pPtr());

  s256 sPath = pFile->sPath();
  
  nnpTask->vPushParam(sPath);
}

/*----------------------------------------------------------------------------*/
/* file.path! -- set file path                                                */
/*----------------------------------------------------------------------------*/
void Task::vFilePathSet(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFilePathSet");
  
  PtrVal xFilePtr;
  
  if ( ! nnpTask->bPopParam(xFilePtr) )
    THROW_INVALID_PARAM("file.path!");
  
  File* pFile = reinterpret_cast<File*>(xFilePtr.pPtr());
  
  s256 sPath;
  
  if ( ! nnpTask->bPopParam(sPath) )
    THROW_INVALID_PARAM("file.path!");
  
  pFile->vPath(sPath.a());
}

/*----------------------------------------------------------------------------*/
/* file.read -- read from a file                                              */
/*----------------------------------------------------------------------------*/
void Task::vFileRead(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFileRead");
  
  PtrVal xFilePtr;
  
  if ( ! nnpTask->bPopParam(xFilePtr) )
    THROW_INVALID_PARAM("file.read");
  
  File* pFile = reinterpret_cast<File*>(xFilePtr.pPtr());
  ASSERT(pFile);
  
  s16K sLine;

  if (pFile->bReadLine(sLine)) {
    nnpTask->vPushParam(sLine);
    nnpTask->vPushParam((i8)1);
  }
  else {
    nnpTask->vPushParam((i8)0);
  }
}

/*----------------------------------------------------------------------------*/
/* file.write -- write to a file                                              */
/*----------------------------------------------------------------------------*/
void Task::vFileWrite(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFileWrite");
  
  PtrVal xFilePtr;
  
  if ( ! nnpTask->bPopParam(xFilePtr) )
    THROW_INVALID_PARAM("file.write");
  
  File* pFile = reinterpret_cast<File*>(xFilePtr.pPtr());
  ASSERT(pFile);
  
  ValWsp xValue;
  
  if ( ! nnpTask->bPopParam(xValue) )
    THROW_INVALID_PARAM("file.write");
  
  //pFile->bWrite(xValue);
}

// loader: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* loader.delete -- delete a table loader                                     */
/*----------------------------------------------------------------------------*/
void Task::vLoaderDelete(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoaderDelete");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("loader.delete");
  
  Loader* pLoader = reinterpret_cast<Loader*>(xPtrVal.pPtr());
  ASSERT(CN_LOADER == pLoader->eClass());
  
  delete pLoader;
}

/*----------------------------------------------------------------------------*/
/* loader.new -- new a table loader                                           */
/*----------------------------------------------------------------------------*/
void Task::vLoaderNew(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoaderNew");
  
  Loader* pLoader = new Loader;
  ASSERT(CN_LOADER == pLoader->eClass());
  
  nnpTask->vPushParam(pLoader);
}

/*----------------------------------------------------------------------------*/
/* loader.init -- init a table loader                                         */
/*----------------------------------------------------------------------------*/
void Task::vLoaderInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoaderInit");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("loader.init");
  
  Loader* pLoader = reinterpret_cast<Loader*>(xPtrVal.pPtr());
  ASSERT(CN_LOADER == pLoader->eClass());
  
  new(pLoader) Loader;
}

/*----------------------------------------------------------------------------*/
/* loader.table! -- store table into loader                                   */
/*----------------------------------------------------------------------------*/
void Task::vLoaderTableStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoaderTableStore");
  
  PtrVal xPtrTable;
  PtrVal xPtrLoader;
  if ( ! nnpTask->bPopParam(xPtrLoader,xPtrTable) )
    THROW_INVALID_PARAM("loader.tableStore");
  
  Loader* pLoader = reinterpret_cast<Loader*>(xPtrLoader.pPtr());
  ASSERT(CN_LOADER == pLoader->eClass());
  
  Table* pTable = reinterpret_cast<Table*>(xPtrTable.pPtr());
  ASSERT(CN_TABLE == pTable->eClass());

  pLoader->vTable(pTable);
}

/*----------------------------------------------------------------------------*/
/* loader.path! -- store file path into loader                                */
/*----------------------------------------------------------------------------*/
void Task::vLoaderPathStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoaderPathStore");
  
  ValWsp xWspPath;
  PtrVal xPtrLoader;
  if ( ! nnpTask->bPopParam(xPtrLoader,xWspPath) )
    THROW_INVALID_PARAM("loader.pathStore");
  
  Loader* pLoader = reinterpret_cast<Loader*>(xPtrLoader.pPtr());
  ASSERT(CN_LOADER == pLoader->eClass());
  
  s256 sPath;
  sPath = xWspPath.sStr();
  
  pLoader->vPath(sPath);
}

/*----------------------------------------------------------------------------*/
/* loader.openfile -- open file to be loaded                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderOpenFile(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoaderOpenFile");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("loader.openfile");
  
  Loader* pLoader = reinterpret_cast<Loader*>(xPtrVal.pPtr());
  ASSERT(CN_LOADER == pLoader->eClass());
  
  bool bOpened = pLoader->bOpenFile();
  
  nnpTask->vPushParam(bOpened);
}

/*----------------------------------------------------------------------------*/
/* loader.closefile -- close file being loaded                                */
/*----------------------------------------------------------------------------*/
void Task::vLoaderCloseFile(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoaderCloseFile");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("loader.closefile");
  
  Loader* pLoader = reinterpret_cast<Loader*>(xPtrVal.pPtr());
  ASSERT(CN_LOADER == pLoader->eClass());
  
  pLoader->vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* loader.fetchRecord -- fetch next record from file being loaded             */
/*----------------------------------------------------------------------------*/
void Task::vLoaderFetchRecord(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoaderFetchReord");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("loader.fetchRecord");
  
  Loader* pLoader = reinterpret_cast<Loader*>(xPtrVal.pPtr());
  ASSERT(CN_LOADER == pLoader->eClass());
  
  bool bFetched = pLoader->bFetchRecord();
  
  nnpTask->vPushParam(bFetched);
}

/*----------------------------------------------------------------------------*/
/* loader.insertRow -- insert row into table being loaded                     */
/*----------------------------------------------------------------------------*/
void Task::vLoaderInsertRow(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoaderFetchReord");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("loader.fetchRecord");
  
  Loader* pLoader = reinterpret_cast<Loader*>(xPtrVal.pPtr());
  ASSERT(CN_LOADER == pLoader->eClass());
  
  bool bInserted = pLoader->bInsertRow();
  
  nnpTask->vPushParam(bInserted);
}

/*----------------------------------------------------------------------------*/
/* loader.begin -- begin loading                                              */
/*----------------------------------------------------------------------------*/
void Task::vLoaderBegin(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoaderBegin");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("loader.begin");
  
  Loader* pLoader = reinterpret_cast<Loader*>(xPtrVal.pPtr());
  ASSERT(CN_LOADER == pLoader->eClass());
  
  pLoader->vBegin();
}

/*----------------------------------------------------------------------------*/
/* loader.end -- end loading                                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoaderEnd(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLoaderEnd");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("loader.end");
  
  Loader* pLoader = reinterpret_cast<Loader*>(xPtrVal.pPtr());
  ASSERT(CN_LOADER == pLoader->eClass());
  
  pLoader->vEnd();
}

// lock: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* lock.dropread -- drop read lock                                            */
/*----------------------------------------------------------------------------*/
void Task::vLockDropRead(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLockDropRead");
  
  PtrVal xLockPtr;
  if ( ! nnpTask->bPopParam(xLockPtr) )
    THROW_INVALID_PARAM("lock.dropread");
  
  Lock* pLock = reinterpret_cast<Lock*>(xLockPtr.pPtr());
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vDropRead();
}

/*----------------------------------------------------------------------------*/
/* lock.dropwrite -- drop write lock                                          */
/*----------------------------------------------------------------------------*/
void Task::vLockDropWrite(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLockDropWrite");

  PtrVal xLockPtr;
  if ( ! nnpTask->bPopParam(xLockPtr) )
    THROW_INVALID_PARAM("lock.dropwrite");
  
  Lock* pLock = reinterpret_cast<Lock*>(xLockPtr.pPtr());
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* lock.grabread -- grab read lock                                            */
/*----------------------------------------------------------------------------*/
void Task::vLockGrabRead(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLockGrabRead");

  PtrVal xLockPtr;
  if ( ! nnpTask->bPopParam(xLockPtr) )
    THROW_INVALID_PARAM("lock.grabread");
  
  Lock* pLock = reinterpret_cast<Lock*>(xLockPtr.pPtr());
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());

  pLock->vGrabRead();
}

/*----------------------------------------------------------------------------*/
/* lock.grabwrite -- grab write lock                                          */
/*----------------------------------------------------------------------------*/
void Task::vLockGrabWrite(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLockGrabWrite");

  PtrVal xLockPtr;
  if ( ! nnpTask->bPopParam(xLockPtr) )
    THROW_INVALID_PARAM("lock.grabwrite");
  
  Lock* pLock = reinterpret_cast<Lock*>(xLockPtr.pPtr());
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* lock.init -- initialise lock                                               */
/*----------------------------------------------------------------------------*/
void Task::vLockInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vLockInit");
  
  PtrVal xLockPtr;
  if ( ! nnpTask->bPopParam(xLockPtr) )
    THROW_INVALID_PARAM("lock.init");
  
  Lock* pLock = reinterpret_cast<Lock*>(xLockPtr.pPtr());
  ASSERT(pLock);
  ASSERT(CN_LOCK == pLock->eClass());
  
  pLock->vInit();
}

// process: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* process.getpid -- get pid of this process                                  */
/*----------------------------------------------------------------------------*/
void Task::vProcessGetPid(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vProcessGetPid");
  
  i8 iPid = Process::iGetPid();
  
  nnpTask->vPushParam(iPid);
}

/*----------------------------------------------------------------------------*/
/* process.testpid -- test pid on stack                                       */
/*----------------------------------------------------------------------------*/
void Task::vProcessTestPid(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vProcessTestPid");

  i8 iPid;
  if ( ! nnpTask->bPopParam(iPid) )
    THROW_INVALID_PARAM("process.testpid");
  
  i8 iExists = Process::bTestPid(iPid);
  
  nnpTask->vPushParam(iExists);
}

// pstack: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* p.depth -- put number of values in param stack onto param stack            */
/*----------------------------------------------------------------------------*/
void Task::vPstackDepth(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vPstackDepth");
  
  i8 iDepth = nnpTask->m_xParamStack.iValues();
  
  nnpTask->vPushParam(iDepth);
}

/*----------------------------------------------------------------------------*/
/* pstack.class -- push class of top value on param stack                     */
/*----------------------------------------------------------------------------*/
void Task::vPstackClass(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vPstackClass");
  
  if ( ! nnpTask->bGuardParam(1) )
    THROW("pstack.class");

  ValWsp* pValWsp = static_cast<ValWsp*>(nnpTask->m_xParamStack.pPeek(0));
  
  eCN eClass = pValWsp->eClass();
  
  nnpTask->vPushParam(eClass);
}

// queue: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* queue.append -- append onto queue                                          */
/*----------------------------------------------------------------------------*/
void Task::vQueueAppend(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vQueueAppend");
  
  Object* pObject;
  ValWsp xValWsp;
  
  if ( ! nnpTask->bPopParam(pObject,xValWsp) )
    THROW_INVALID_PARAM("queue.append");
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  ASSERT(CN_QUEUE == pObject->eClass());
  
  pQueue->bAppend(&xValWsp);
}

/*----------------------------------------------------------------------------*/
/* q.dump -- queue dump                                                       */
/*----------------------------------------------------------------------------*/
void Task::vQueueDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vQueueDump");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("q.dump");
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  pQueue->vDump();
}

/*----------------------------------------------------------------------------*/
/* queue.init -- queue init                                                   */
/*----------------------------------------------------------------------------*/
void Task::vQueueInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vQueueInit");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("queue.init");
    
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  pQueue->vInitialize();
}

/*----------------------------------------------------------------------------*/
/* queue.pop -- queue pop                                                     */
/*----------------------------------------------------------------------------*/
void Task::vQueuePop(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vQueuePop");
  
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
  
}

// rowcursor: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* rowcursor.begin -- begin using row cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorBegin(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRowCursorBegin");
  
  PtrVal xPtrRowCursor;
  if ( ! nnpTask->bPopParam(xPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.begin");
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(xPtrRowCursor.pPtr());
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vBegin();
  pRowCursor->vFirst();
  
  PtrVal xPtrRow;
  xPtrRow = pRowCursor->pRowAsStr();
  
  nnpTask->vPushParam(xPtrRow);
}

/*----------------------------------------------------------------------------*/
/* rowcursor.delete -- delete a row cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorDelete(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRowCursorDelete");
  
  PtrVal xPtrRowCursor;
  if ( ! nnpTask->bPopParam(xPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.delete");
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(xPtrRowCursor.pPtr());
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pRowCursor);
  delete pRowCursor;
  
  gpSmallHeap->vFreeObject(pObject);
}

/*----------------------------------------------------------------------------*/
/* rowcursor.end -- end using row cursor                                      */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorEnd(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRowCursorEnd");
  
  PtrVal xPtrRowCursor;
  if ( ! nnpTask->bPopParam(xPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.end");
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(xPtrRowCursor.pPtr());
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vEnd();
}

/*----------------------------------------------------------------------------*/
/* rowcursor.first -- set to first and get row                                */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorFirst(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRowCursorFirst");
  
  PtrVal xPtrRowCursor;
  if ( ! nnpTask->bPopParam(xPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.first");
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(xPtrRowCursor.pPtr());
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vFirst();
  
  PtrVal xPtrRow;
  xPtrRow = pRowCursor->pRowAsStr();
  
  nnpTask->vPushParam(xPtrRow);
}

/*----------------------------------------------------------------------------*/
/* rowcursor.get -- get current row                                           */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorGet(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRowCursorGet");
  
  PtrVal xPtrRowCursor;
  if ( ! nnpTask->bPopParam(xPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.get");
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(xPtrRowCursor.pPtr());
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  PtrVal xPtrRow;
  xPtrRow = pRowCursor->pRowAsStr();
  
  nnpTask->vPushParam(xPtrRow);
}

/*----------------------------------------------------------------------------*/
/* rowcursor.init -- init a row cursor                                        */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRowCursorInit");
  
  PtrVal xPtrRowCursor;
  if ( ! nnpTask->bPopParam(xPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.init");
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(xPtrRowCursor.pPtr());
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  new(pRowCursor) RowCursor;
}

/*----------------------------------------------------------------------------*/
/* rowcursor.new -- new an row cursor                                         */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorNew(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRowCursorNew");
  
  RowCursor* pRowCursor = new RowCursor;
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  PtrVal xPtrRowCursor;
  xPtrRowCursor = pRowCursor;
  
  nnpTask->vPushParam(xPtrRowCursor);
}

/*----------------------------------------------------------------------------*/
/* rowcursor.next -- advance to next and get row                              */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorNext(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRowCursorNext");
  
  PtrVal xPtrRowCursor;
  if ( ! nnpTask->bPopParam(xPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.next");
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(xPtrRowCursor.pPtr());
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vNext();
  
  PtrVal xPtrRow;
  xPtrRow = pRowCursor->pRowAsStr();
  
  nnpTask->vPushParam(xPtrRow);
}

/*----------------------------------------------------------------------------*/
/* rowcursor.table@ -- fetch table from row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRowCursorTableFetch");
  
  PtrVal xPtrRowCursor;
  if ( ! nnpTask->bPopParam(xPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.table@");
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(xPtrRowCursor.pPtr());
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  PtrVal xPtrTable;
  xPtrTable = pRowCursor->pTable();
  
  nnpTask->vPushParam(xPtrTable);
}

/*----------------------------------------------------------------------------*/
/* rowcursor.table! -- store table into row cursor                            */
/*----------------------------------------------------------------------------*/
void Task::vRowCursorTableStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vRowCursorTableStore");
  
  PtrVal xPtrRowCursor;
  if ( ! nnpTask->bPopParam(xPtrRowCursor) )
    THROW_INVALID_PARAM("rowcursor.table!");
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(xPtrRowCursor.pPtr());
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  PtrVal xPtrTable;
  if ( ! nnpTask->bPopParam(xPtrTable) )
    THROW_INVALID_PARAM("rowcursor.table!");
  
  Table* pTable = reinterpret_cast<Table*>(xPtrTable.pPtr());
  ASSERT(CN_TABLE == pTable->eClass());
  
  pRowCursor->vTable(pTable);
}

// seitf: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* seitf.delete -- delete SE interface                                        */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDelete(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfDelete");
  
  PtrVal xPtrSEitf;
  
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("seitf.delete");

  SEitf* pSEitf = static_cast<SEitf*>(xPtrSEitf.pPtr());
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  delete pSEitf;
}

/*----------------------------------------------------------------------------*/
/* seitf.dump -- dump of SE interface                                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfDump");
  
  PtrVal xPtrSEitf;
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("seitf.dump");
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(xPtrSEitf.pPtr());
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  pSEitf->vDump();
}

/*----------------------------------------------------------------------------*/
/* seitf.new -- new SE interface                                              */
/*----------------------------------------------------------------------------*/
void Task::vSEitfNew(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfNew");
  
  SEitf* pSEitf = new SEitf;
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  PtrVal xPtrSEitf;
  xPtrSEitf = pSEitf;
  
  nnpTask->vPushParam(xPtrSEitf);
}

/*----------------------------------------------------------------------------*/
/* seitf.req@ -- fetch SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfReqFetch");
  
  PtrVal xPtrSEitf;
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("seitf.req@");
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(xPtrSEitf.pPtr());
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  PtrVal xPtrQueue;
  xPtrQueue = pSEitf->pRequestQueue();
  
  nnpTask->vPushParam(xPtrQueue);
}

/*----------------------------------------------------------------------------*/
/* seitf.req! -- store SEitf request queue                                    */
/*----------------------------------------------------------------------------*/
void Task::vSEitfReqStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfReqStore");
  
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
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp@ -- fetch SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfRspFetch");
  
  PtrVal xPtrSEitf;
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("seitf.rsp@");
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(xPtrSEitf.pPtr());
  ASSERT(CN_SEITF == pSEitf->eClass());
  
  PtrVal xPtrQueue;
  xPtrQueue = pSEitf->pResponseQueue();
  
  nnpTask->vPushParam(xPtrQueue);
}

/*----------------------------------------------------------------------------*/
/* seitf.rsp! -- store SEitf response queue                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfRspStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfRspStore");
  
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
}

// seitfcursor: ////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* seitfcursor.begin -- begin using seitf currsor                             */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorBegin(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfCursorBegin");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.begin");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());

  pSEitfCursor->vBegin();
  pSEitfCursor->vFirst();
  
  PtrVal xPtrVal;
  xPtrVal = pSEitfCursor->pObject();
  
  nnpTask->vPushParam(xPtrVal);
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.delete -- delete an seitf cursor                               */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorDelete(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfCursorDelete");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.delete");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pSEitfCursor);
  delete pSEitfCursor;
  
  gpSmallHeap->vFreeObject(pObject);
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.end -- end using seitf cursor                                  */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorEnd(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfCursorEnd");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.end");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vEnd();
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.first -- set to first and get seitf                            */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorFirst(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfCursorFirst");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.first");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vFirst();
  
  PtrVal xPtrVal;
  xPtrVal = pSEitfCursor->pObject();
  
  nnpTask->vPushParam(xPtrVal);
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.get -- get current seitf                                       */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorGet(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfCursorGet");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.get");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  PtrVal xPtrVal;
  xPtrVal = pSEitfCursor->pObject();
  
  nnpTask->vPushParam(xPtrVal);
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.init -- init an seitf cursor                                   */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfCursorInit");
  
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
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.new -- new an seitf cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNew(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfCursorNew");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("seitfcursor.new");
  
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(xPtrVal.pPtr());
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  void* pSEitfCursorAddress = gpSmallHeap->pAllocObject();
  SEitfCursor* pSEitfCursor = new(pSEitfCursorAddress) SEitfCursor(pSEitfList);
  
  PtrVal xSEitfCursorPtr;
  xSEitfCursorPtr = pSEitfCursor;
  
  nnpTask->vPushParam(xSEitfCursorPtr);
}

/*----------------------------------------------------------------------------*/
/* seitfcursor.next -- advance to next and get seitf                          */
/*----------------------------------------------------------------------------*/
void Task::vSEitfCursorNext(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfCursorNext");
  
  PtrVal xPtrSEitfCursor;
  if ( ! nnpTask->bPopParam(xPtrSEitfCursor) )
    THROW_INVALID_PARAM("seitfcursor.next");
  
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(xPtrSEitfCursor.pPtr());
  ASSERT(CN_SEITFCURSOR == pSEitfCursor->eClass());
  
  pSEitfCursor->vNext();
  
  PtrVal xPtrVal;
  xPtrVal = pSEitfCursor->pObject();
  
  nnpTask->vPushParam(xPtrVal);
}

// seitflist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* seitflist.dump -- dump of seitfList on param stack                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfListDump");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("seitflist.dump");
  
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(xPtrVal.pPtr());
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  pSEitfList->vDump("SEitfList:");
}

/*----------------------------------------------------------------------------*/
/* seitflist.init -- init of seitfList on param stack                         */
/*----------------------------------------------------------------------------*/
void Task::vSEitfListInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vSEitfListInit");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("seitflist.init");
  
  SEitfList* pSEitfList = reinterpret_cast<SEitfList*>(xPtrVal.pPtr());
  ASSERT(CN_SEITFLIST == pSEitfList->eClass());
  
  new(pSEitfList) SEitfList;
}

// table: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* table.copy -- copy from table to table on param stack                      */
/*----------------------------------------------------------------------------*/
void Task::vTableCopy(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTableCopy");
  
  PtrVal xPtrFromTable;
  if ( ! nnpTask->bPopParam(xPtrFromTable) )
    THROW_INVALID_PARAM("table.copy");
  
  PtrVal xPtrToTable;
  if ( ! nnpTask->bPopParam(xPtrToTable) )
    THROW_INVALID_PARAM("table.copy");
  
  Table* pFromTable = reinterpret_cast<Table*>(xPtrFromTable.pPtr());
  ASSERT(CN_TABLE == pFromTable->eClass());
  
  Table* pToTable = reinterpret_cast<Table*>(xPtrToTable.pPtr());
  ASSERT(CN_TABLE == pToTable->eClass());
  
  pToTable->vCopy(pFromTable);
}

/*----------------------------------------------------------------------------*/
/* table.dic -- get table dictionary                                          */
/*----------------------------------------------------------------------------*/
void Task::vTableDic(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTableDic");
  
  PtrVal xPtrTable;
  if ( ! nnpTask->bPopParam(xPtrTable) )
    THROW_INVALID_PARAM("table.dic");
  
  Table* pTable = reinterpret_cast<Table*>(xPtrTable.pPtr());
  ASSERT(CN_TABLE == pTable->eClass());
  
  Dictionary* pDictionary = pTable->pDic();
  
  nnpTask->vPushParam(pDictionary);
}

/*----------------------------------------------------------------------------*/
/* table.drop -- drop of table on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableDrop(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTableDrop");
  
  PtrVal xPtrTable;
  if ( ! nnpTask->bPopParam(xPtrTable) )
    THROW_INVALID_PARAM("table.drop");
  
  Table* pTable = reinterpret_cast<Table*>(xPtrTable.pPtr());
  ASSERT(CN_TABLE == pTable->eClass());
}

/*----------------------------------------------------------------------------*/
/* table dump                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTableDump");
  
  vTrace("TableDump\n");
  
  PtrVal xPtrTable;
  if ( ! nnpTask->bPopParam(xPtrTable) )
    THROW_INVALID_PARAM("table.dump");
  
  Table* pTable = reinterpret_cast<Table*>(xPtrTable.pPtr());
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vDump();
}

/*----------------------------------------------------------------------------*/
/* table.export -- export table into file                                     */
/*----------------------------------------------------------------------------*/
void Task::vTableExport(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTableExport");
  
  PtrVal xPtrTable;
  PtrVal xPtrFile;
  
  if ( ! nnpTask->bPopParam(xPtrTable,xPtrFile) )
    THROW_INVALID_PARAM("table.export");
  
  Table* pTable = reinterpret_cast<Table*>(xPtrTable.pPtr());
  ASSERT(CN_TABLE == pTable->eClass());
  
  File* pFile = reinterpret_cast<File*>(xPtrFile.pPtr());
  ASSERT(CN_FILE == pFile->eClass());
  
  pTable->vExport(pFile);
}

/*----------------------------------------------------------------------------*/
/* table.init -- init of table on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTableInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTableInit");
  
  PtrVal xPtrTable;
  if ( ! nnpTask->bPopParam(xPtrTable) )
    THROW_INVALID_PARAM("table.init");
  
  Table* pTable = reinterpret_cast<Table*>(xPtrTable.pPtr());
  ASSERT(CN_TABLE == pTable->eClass());
  
  new(pTable) Table;
}

/*----------------------------------------------------------------------------*/
/* table.insert -- insert row into table on param stack                       */
/*----------------------------------------------------------------------------*/
void Task::vTableInsert(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTableInsert");
  
  ValWsp xRow;
  PtrVal xPtrTable;
  if ( ! nnpTask->bPopParam(xPtrTable,xRow) )
    THROW_INVALID_PARAM("table.insert");
  
  Table* pTable = reinterpret_cast<Table*>(xPtrTable.pPtr());
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vInsertRow(0,xRow.pAsStrVal());
}

/*----------------------------------------------------------------------------*/
/* table.load -- load file into table on param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTableLoad(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTableLoad");
  
  PtrVal xPtrTable;
  PtrVal xPtrFile;
  
  if ( ! nnpTask->bPopParam(xPtrTable,xPtrFile) )
    THROW_INVALID_PARAM("table.load");
  
  Table* pTable = reinterpret_cast<Table*>(xPtrTable.pPtr());
  ASSERT(CN_TABLE == pTable->eClass());
  
  File* pFile = reinterpret_cast<File*>(xPtrFile.pPtr());
  ASSERT(CN_FILE == pFile->eClass());
  
  pTable->vLoad(pFile);
}

/*----------------------------------------------------------------------------*/
/* table.truncate -- release all rows/buffers in table on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTableTruncate(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTableTruncate");
  
  PtrVal xPtrTable;
  if ( ! nnpTask->bPopParam(xPtrTable) )
    THROW_INVALID_PARAM("table.truncate");
  
  Table* pTable = reinterpret_cast<Table*>(xPtrTable.pPtr());
  ASSERT(CN_TABLE == pTable->eClass());
  
  pTable->vTruncate();
}

/*----------------------------------------------------------------------------*/
/* table.space -- push the buffer space used bytable on param stack           */
/*----------------------------------------------------------------------------*/
void Task::vTableSpace(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTableSpace");
  
  PtrVal xPtrTable;
  if ( ! nnpTask->bPopParam(xPtrTable) )
    THROW_INVALID_PARAM("table.spave");
  
  Table* pTable = reinterpret_cast<Table*>(xPtrTable.pPtr());
  ASSERT(CN_TABLE == pTable->eClass());
  
  i8 iBuffers = pTable->iRowBuffers();
  i8 iSpace   = iBuffers * C_SIZE_LARGE_BUFFER;
  
  nnpTask->vPushParam(iSpace);
}

// task: ///////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* task.dic -- push task dictionary onto param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskDic(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskDic");
  
  PtrVal xDicPtr;
  xDicPtr = (void*)&nnpTask->m_xTaskDictionary;
  
  nnpTask->vPushParam(xDicPtr);
}

/*----------------------------------------------------------------------------*/
/* task.dump -- dump of task on param stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vTaskDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskDump");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.dump");
  
  pThatTask->vDump();
}

/*----------------------------------------------------------------------------*/
/* task.init -- init of task on param stack                                   */
/*----------------------------------------------------------------------------*/
void Task::vTaskInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskInit");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("task.init");
  
  Task* pThatTask = reinterpret_cast<Task*>(xPtrVal.pPtr());
  ASSERT(CN_TASK == pThatTask->eClass());
  
  //  pThatTask->vInit("");
  
  new(pThatTask) Task;
}

/*----------------------------------------------------------------------------*/
/* task.seitf@ -- fetch SEitf of task on param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskSeitfFetch");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.seitf@");
  
  SEitf* pSEitf = pThatTask->m_pSEitf;
  
  PtrVal xPtrSEitf;
  xPtrSEitf = pSEitf;
  
  nnpTask->vPushParam(xPtrSEitf);
}

/*----------------------------------------------------------------------------*/
/* task.seitf! -- store SEitf on param stack into task on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeitfStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskSeitfStore");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.seitf!");
  
  PtrVal xPtrSEitf;
  
  if ( ! nnpTask->bPopParam(xPtrSEitf) )
    THROW_INVALID_PARAM("task.seitf!");
  
  SEitf* pSEitf = static_cast<SEitf*>(xPtrSEitf.pPtr());
  ASSERT(! pSEitf || CN_SEITF == pSEitf->eClass());
  
  pThatTask->m_pSEitf = pSEitf;
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskNameFetch");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.name@");
  
  nnpTask->vPushParam(pThatTask->m_sName);
}

/*----------------------------------------------------------------------------*/
/* task.name! -- store task name                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNameStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskNameStore");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.name!");
  
  ValWsp xWspName;
  nnpTask->vPopParam(&xWspName);
  
  pThatTask->m_sName = xWspName.pAsStrVal()->sStr();
}

/*----------------------------------------------------------------------------*/
/* task.new -- create a new task                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskNew(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskNew");
  
  if ( ! nnpTask->bGuardParam(CN_STRVAL) )
    THROW_INVALID_PARAM("task.new");
  
  ValWsp xValWsp;
  nnpTask->vPopParam(&xValWsp);
  
  char* aSeedWord = xValWsp.sStr();
  
  NNP_Task nnpNewTask = new Task;
  
  nnpNewTask->vSeedWord(aSeedWord);
    
  Task* pNewTask = *nnpNewTask;
  gpTaskList->vAppend(pNewTask);
  
  nnpTask->vPushParam(nnpNewTask);
}

/*----------------------------------------------------------------------------*/
/* task.queue@ -- fetch queue of task on param stack                          */
/*----------------------------------------------------------------------------*/
void Task::vTaskQfetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskQfetch");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.queue@");
  
  Queue* pQueue = pThatTask->m_pQueue;
  
  Object* pQueueEmb = static_cast<Object*>(pQueue);
  
  nnpTask->vPushParam(pQueueEmb);
}

/*----------------------------------------------------------------------------*/
/* task.queue! -- store queue on param stack into task on param stack         */
/*----------------------------------------------------------------------------*/
void Task::vTaskQstore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskQstore");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.queue!");
  
  Object* pQueueEmb;
  
  if ( ! nnpTask->bPopParam(pQueueEmb) )
    THROW_INVALID_PARAM("task.queue!");
  
  Queue* pQueue = static_cast<Queue*>(pQueueEmb);
  
  pThatTask->m_pQueue = pQueue;
}

/*----------------------------------------------------------------------------*/
/* task.seed -- set the seed word of task                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskSeed(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskSeed");
  
  ValWsp xStrWsp;
  
  if ( ! nnpTask->bPopParam(xStrWsp) )
    THROW_INVALID_PARAM("task.seed");
  
  char* pSeed = xStrWsp.pAsStrVal()->sStr();
  
  nnpTask->vSeedWord(pSeed);
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskStateFetch");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.state@");
  
  eTS eState = pThatTask->m_eState;
  
  nnpTask->vPushParam((i8)eState);
}

/*----------------------------------------------------------------------------*/
/* task.state! -- store state of task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskStateStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskStateStore");
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("task.state!");
  
  i8 iState;
  
  if ( ! nnpTask->bPopParam(iState) )
    THROW_INVALID_PARAM("task.state!");
  
  pThatTask->m_eState = (eTS)iState;
}

/*----------------------------------------------------------------------------*/
/* task.this -- push this tx                                                  */
/*----------------------------------------------------------------------------*/
void Task::vTaskThis(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskThis");
  
  nnpTask->vPushParam(nnpTask);
}

// taskcursor: /////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* taskcursor.begin -- begin using task currsor                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorBegin(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskCursorBegin");
  
  PtrVal xPtrTaskCursor;
  if ( ! nnpTask->bPopParam(xPtrTaskCursor) )
    THROW_INVALID_PARAM("taskcursor.begin");
  
  TaskCursor* pTaskCursor = reinterpret_cast<TaskCursor*>(xPtrTaskCursor.pPtr());
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vBegin();
  pTaskCursor->vFirst();
  
  TaskVal xTaskPtr;
  xTaskPtr = pTaskCursor->pObject();
  nnpTask->vPushParam(xTaskPtr);
}

/*----------------------------------------------------------------------------*/
/* taskcursor.delete -- delete an task cursor                                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorDelete(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskCursorDelete");
  
  PtrVal xPtrTaskCursor;
  if ( ! nnpTask->bPopParam(xPtrTaskCursor) )
    THROW_INVALID_PARAM("taskcursor.delete");
  
  TaskCursor* pTaskCursor = reinterpret_cast<TaskCursor*>(xPtrTaskCursor.pPtr());
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  Object* pObject = reinterpret_cast<Object*>(pTaskCursor);
  delete pTaskCursor;
  
  gpSmallHeap->vFreeObject(pObject);
}

/*----------------------------------------------------------------------------*/
/* taskcursor.end -- end using task cursor                                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorEnd(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskCursorEnd");
  
  PtrVal xPtrTaskCursor;
  if ( ! nnpTask->bPopParam(xPtrTaskCursor) )
    THROW_INVALID_PARAM("taskcursor.end");
  
  TaskCursor* pTaskCursor = reinterpret_cast<TaskCursor*>(xPtrTaskCursor.pPtr());
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vEnd();
}

/*----------------------------------------------------------------------------*/
/* taskcursor.first -- set to first and get task                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorFirst(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskCursorFirst");
  
  PtrVal xPtrTaskCursor;
  if ( ! nnpTask->bPopParam(xPtrTaskCursor) )
    THROW_INVALID_PARAM("taskcursor.first");
  
  TaskCursor* pTaskCursor = reinterpret_cast<TaskCursor*>(xPtrTaskCursor.pPtr());
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vFirst();
  
  TaskVal xTaskPtr;
  xTaskPtr = pTaskCursor->pObject();
  nnpTask->vPushParam(xTaskPtr);
}

/*----------------------------------------------------------------------------*/
/* taskcursor.get -- get current task                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorGet(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskCursorGet");
  
  PtrVal xPtrTaskCursor;
  if ( ! nnpTask->bPopParam(xPtrTaskCursor) )
    THROW_INVALID_PARAM("taskcursor.get");
  
  TaskCursor* pTaskCursor = reinterpret_cast<TaskCursor*>(xPtrTaskCursor.pPtr());
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  Task* pThatTask = pTaskCursor->pObject();
  nnpTask->vPushParam(pThatTask);
}

/*----------------------------------------------------------------------------*/
/* taskcursor.init -- init an task cursor                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskCursorInit");
  
  PtrVal xPtrTaskCursor;
  if ( ! nnpTask->bPopParam(xPtrTaskCursor) )
    THROW_INVALID_PARAM("taskcursor.init");
  
//  s32 sName;
//  sName = "tasklist";
//  Word* pWord = nnpTask->pFindWord(sName);
//  ASSERT(pWord);
//  TaskList* pTaskList = reinterpret_cast<TaskList*>(pWord->pBodyArea());
  
  TaskCursor* pTaskCursor = reinterpret_cast<TaskCursor*>(xPtrTaskCursor.pPtr());
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  new(pTaskCursor) TaskCursor(gpTaskList);
}

/*----------------------------------------------------------------------------*/
/* taskcursor.new -- new an task cursor                                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorNew(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskCursorNew");
  
  PtrVal xPtrTaskCursor;
  if ( ! nnpTask->bPopParam(xPtrTaskCursor) )
    THROW_INVALID_PARAM("taskcursor.new");
  
  TaskList* pTaskList = reinterpret_cast<TaskList*>(xPtrTaskCursor.pPtr());
  ASSERT(CN_TASKCURSOR == pTaskList->eClass());
  
  void* pTaskCursorAddress = gpSmallHeap->pAllocObject();
  TaskCursor* pTaskCursor = new(pTaskCursorAddress) TaskCursor(pTaskList);
  
  PtrVal xSEitfCursorPtr;
  xSEitfCursorPtr = pTaskCursor;
  
  nnpTask->vPushParam(xSEitfCursorPtr);
}

/*----------------------------------------------------------------------------*/
/* taskcursor.next -- advance to next and get task                            */
/*----------------------------------------------------------------------------*/
void Task::vTaskCursorNext(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskCursorNext");
  
  PtrVal xPtrTaskCursor;
  if ( ! nnpTask->bPopParam(xPtrTaskCursor) )
    THROW_INVALID_PARAM("taskcursor.next");
  
  TaskCursor* pTaskCursor = reinterpret_cast<TaskCursor*>(xPtrTaskCursor.pPtr());
  ASSERT(CN_TASKCURSOR == pTaskCursor->eClass());
  
  pTaskCursor->vNext();
  
  TaskVal xTaskPtr;
  xTaskPtr = pTaskCursor->pObject();
  nnpTask->vPushParam(xTaskPtr);
}

// tasklist: ///////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* tasklist -- tasklist                                                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskList(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskList");
  
  nnpTask->vPushParam(gpTaskList);
}

/*----------------------------------------------------------------------------*/
/* tasklist.append -- tasklist append                                         */
/*----------------------------------------------------------------------------*/
void Task::vTaskListAppend(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskListAppend");
  
  PtrVal xPtrTaskList;
  if ( ! nnpTask->bPopParam(xPtrTaskList) )
    THROW_INVALID_PARAM("tasklist.append");
  
  TaskList* pTaskList = reinterpret_cast<TaskList*>(xPtrTaskList.pPtr());
  ASSERT(CN_TASKLIST == pTaskList->eClass());
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("tasklist.append");
  
  pTaskList->vAppend(pThatTask);
}

/*----------------------------------------------------------------------------*/
/* tasklist.dump -- tasklist dump                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskListDump");
  
  PtrVal xPtrTaskList;
  if ( ! nnpTask->bPopParam(xPtrTaskList) )
    THROW_INVALID_PARAM("tasklist.dump");
  
  TaskList* pTaskList = reinterpret_cast<TaskList*>(xPtrTaskList.pPtr());
  ASSERT(CN_TASKLIST == pTaskList->eClass());
 
  pTaskList->vDump("TaskList:");
}

/*----------------------------------------------------------------------------*/
/* tasklist.init -- tasklist init                                             */
/*----------------------------------------------------------------------------*/
void Task::vTaskListInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskListInit");
}

// taskset: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* taskset.append -- taskset append                                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetAppend(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskSetAppend");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("taskset.append");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  Task* pThatTask;
  
  if ( ! nnpTask->bPopParam(pThatTask) )
    THROW_INVALID_PARAM("taskset.append");
  
  pTaskSet->vAppend(pThatTask);
}

/*----------------------------------------------------------------------------*/
/* taskset.dump -- taskset dump                                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskSetDump");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("taskset.dump");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  pTaskSet->vDump();
}

/*----------------------------------------------------------------------------*/
/* taskset.init -- taskset init                                               */
/*----------------------------------------------------------------------------*/
void Task::vTaskSetInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTaskSetInit");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("taskset.init");
  
  ASSERT(CN_TASKSET == pObject->eClass());
  
  TaskSet* pTaskSet = static_cast<TaskSet*>(pObject);
  
  new(pTaskSet) TaskSet;
}

// thread: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* thread.append -- append task to threads taskset                            */
/*----------------------------------------------------------------------------*/
void Task::vThreadAppend(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadAppend");
  
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
}

/*----------------------------------------------------------------------------*/
/* thread.bind -- bind this thread to a cpu                                   */
/*----------------------------------------------------------------------------*/
void Task::vThreadBind(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadBind");
  
  i8 iCpu;
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(iCpu) )
    THROW_INVALID_PARAM("thread.bind");
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.bind");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vBind(iCpu);
}

/*----------------------------------------------------------------------------*/
/* thread.cpu -- get the cpu this thread is running on                        */
/*----------------------------------------------------------------------------*/
void Task::vThreadCpu(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadCpu");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.cpu");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  nnpTask->vPushParam(pThread->iCpu());
}

/*----------------------------------------------------------------------------*/
/* thread.dump -- thread dump                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadDump");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.dump");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  pThread->vDump();
}

/*----------------------------------------------------------------------------*/
/* thread.init -- thread init                                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadInit");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.init");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  new(pThread) Thread;
}

/*----------------------------------------------------------------------------*/
/* thread.name@ -- fetch thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameFetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadNameFetch");
  
  Object* pObject;
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("thread.name@");
  
  ASSERT(CN_THREAD == pObject->eClass());
  
  Thread* pThread = static_cast<Thread*>(pObject);
  
  s32 sName;
  sName = pThread->sName();
  
  nnpTask->vPushParam(sName);
}

/*----------------------------------------------------------------------------*/
/* thread.name! -- store thread name                                          */
/*----------------------------------------------------------------------------*/
void Task::vThreadNameStore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadNameStore");
  
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
}

/*----------------------------------------------------------------------------*/
/* thread.taskset@ -- fetch taskset from the thread which is on param stack   */
/*----------------------------------------------------------------------------*/
void Task::vThreadSfetch(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadSfetch");
 
  Object* pThreadEmb;
  
  if ( ! nnpTask->bPopParam(pThreadEmb) )
    THROW_INVALID_PARAM("thread.taskset@");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  TaskSet* pTaskSet = pThread->m_pTaskSet;

  Object* pTaskSetEmb = static_cast<Object*>(pTaskSet);;

  nnpTask->vPushParam(pTaskSetEmb);
}

/*----------------------------------------------------------------------------*/
/* thread.taskset! -- store taskset into the thread which is on param stack   */
/*----------------------------------------------------------------------------*/
void Task::vThreadSstore(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadSstore");
  
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
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_s -- sleep for n seconds                                      */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_s(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadSleep_s");
  
  i8 nSeconds;
  
  if ( ! nnpTask->bPopParam(nSeconds) )
    THROW_INVALID_PARAM("sleep_s");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepSec(nSeconds);
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ms -- sleep for n milliseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ms(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadSleep_ms");
  
  i8 nMilliseconds;
  
  if ( ! nnpTask->bPopParam(nMilliseconds) )
    THROW_INVALID_PARAM("sleep_ms");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepMilli(nMilliseconds);
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_us -- sleep for n microseconds                                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_us(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadSleep_us");
  
  i8 nMicroseconds;
  
  if ( ! nnpTask->bPopParam(nMicroseconds) )
    THROW_INVALID_PARAM("sleep_us");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepMicro(nMicroseconds);
}

/*----------------------------------------------------------------------------*/
/* thread.sleep_ns -- sleep for n nanoseconds                                 */
/*----------------------------------------------------------------------------*/
void Task::vThreadSleep_ns(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadSleep_ns");
  
  i8 nNanoseconds;
  
  if ( ! nnpTask->bPopParam(nNanoseconds) )
    THROW_INVALID_PARAM("sleep_ns");
  
  Thread* pThread = Thread::pThread();
  
  pThread->m_xPosixThread.vSleepNano(nNanoseconds);
}

/*----------------------------------------------------------------------------*/
/* thread.spawn -- spwan os thread associated with this thread                */
/*----------------------------------------------------------------------------*/
void Task::vThreadSpawn(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadSpawn");
  
  Object* pThreadEmb;
  
  if ( ! nnpTask->bPopParam(pThreadEmb) )
    THROW_INVALID_PARAM("thread.spawn");
  
  ASSERT(CN_THREAD == pThreadEmb->eClass());
  
  Thread* pThread = static_cast<Thread*>(pThreadEmb);
  
  pThread->vSpawn();
}

// threadlist: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* threadlist.dump -- dump the threadList on param stack                      */
/*----------------------------------------------------------------------------*/
void Task::vThreadListDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadListDump");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("threadlist.dump");
  
  ThreadList* pThreadList = reinterpret_cast<ThreadList*>(xPtrVal.pPtr());
  ASSERT(CN_THREADLIST == pThreadList->eClass());

  pThreadList->vDump("ThreadList:");
}

/*----------------------------------------------------------------------------*/
/* threadlist.init -- init of threadList on param stack                       */
/*----------------------------------------------------------------------------*/
void Task::vThreadListInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vThreadListInit");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("threadlist.init");
  
  ThreadList* pThreadList = reinterpret_cast<ThreadList*>(xPtrVal.pPtr());
  ASSERT(CN_THREADLIST == pThreadList->eClass());
  
  new(pThreadList) ThreadList;
}

// timer: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* timer.delete -- delete timer                                               */
/*----------------------------------------------------------------------------*/
void Task::vTimerDelete(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTimeDelete");
  
  PtrVal xPtrSession;
  
  if ( ! nnpTask->bPopParam(xPtrSession) )
    THROW_INVALID_PARAM("timer.delete");
  
  Timer* pTimer = static_cast<Timer*>(xPtrSession.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  Object* pTimerObject = reinterpret_cast<Object*>(pTimer);
  
  delete pTimer;
  
  gpSmallHeap->vFreeObject(pTimerObject);
}

/*----------------------------------------------------------------------------*/
/* timer.init -- init timer                                                   */
/*----------------------------------------------------------------------------*/
void Task::vTimerInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTimerInit");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.init");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  new(pTimer) Timer;
}

/*----------------------------------------------------------------------------*/
/* timer.microsecs -- get elapsed duration in microseconds                    */
/*----------------------------------------------------------------------------*/
void Task::vTimerMicrosecs(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTimerMicrosecs");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.microsecs");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMicroseconds = pTimer->iMicroseconds();
  
  nnpTask->vPushParam(iMicroseconds);
}

/*----------------------------------------------------------------------------*/
/* timer.millisecs -- get elapsed duration in milliseconds                    */
/*----------------------------------------------------------------------------*/
void Task::vTimerMillisecs(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTimerMillisecs");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.millisecs");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iMilliseconds = pTimer->iMilliseconds();
  
  nnpTask->vPushParam(iMilliseconds);
}

/*----------------------------------------------------------------------------*/
/* timer.nanosecs -- get elapsed duration in nanoseconds                      */
/*----------------------------------------------------------------------------*/
void Task::vTimerNanosecs(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTimerNanosecs");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.nanosecs");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iNanoseconds = pTimer->iNanoseconds();
  
  nnpTask->vPushParam(iNanoseconds);
}

/*----------------------------------------------------------------------------*/
/* timer.new -- new timer                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTimerNew(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTimerNew");
  
  void* pTimerAddress = gpSmallHeap->pAllocObject();

  Timer* pTimer = new(pTimerAddress) Timer;
  ASSERT(CN_TIMER == pTimer->eClass());
  
  PtrVal xPtrTimer;
  xPtrTimer = pTimer;
  
  nnpTask->vPushParam(xPtrTimer);
}

/*----------------------------------------------------------------------------*/
/* timer.reset -- reset timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerReset(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTimerReset");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.reset");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vReset();
}

/*----------------------------------------------------------------------------*/
/* timer.secs -- get elapsed duration in seconds                              */
/*----------------------------------------------------------------------------*/
void Task::vTimerSecs(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTimerSecs");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.secs");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iSeconds = pTimer->iSeconds();
  
  nnpTask->vPushParam(iSeconds);
}

/*----------------------------------------------------------------------------*/
/* timer.start -- start timer                                                 */
/*----------------------------------------------------------------------------*/
void Task::vTimerStart(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTimerStart");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.Start");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vStart();
}

/*----------------------------------------------------------------------------*/
/* timer.ticks -- get elapsed ticks                                           */
/*----------------------------------------------------------------------------*/
void Task::vTimerTicks(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTimerTicks");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.ticks");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  i8 iTicks = pTimer->iTicks();
  
  nnpTask->vPushParam(iTicks);
}

/*----------------------------------------------------------------------------*/
/* timer.update -- update timer                                               */
/*----------------------------------------------------------------------------*/
void Task::vTimerUpdate(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTimerUpdate");
  
  PtrVal xPtrTimer;
  if ( ! nnpTask->bPopParam(xPtrTimer) )
    THROW_INVALID_PARAM("timer.update");
  
  Timer* pTimer = reinterpret_cast<Timer*>(xPtrTimer.pPtr());
  ASSERT(CN_TIMER == pTimer->eClass());
  
  pTimer->vUpdate();
}

// tx: /////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* tx.delete -- delete tx                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTxDelete(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTxDelete");
  
  PtrVal xPtrSession;
  
  if ( ! nnpTask->bPopParam(xPtrSession) )
    THROW_INVALID_PARAM("tx.delete");
  
  Tx* pTx = static_cast<Tx*>(xPtrSession.pPtr());
  ASSERT(CN_TX == pTx->eClass());
  
  delete pTx;
}

/*----------------------------------------------------------------------------*/
/* tx.new -- new tx                                                           */
/*----------------------------------------------------------------------------*/
void Task::vTxNew(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTxNew");
  
  Tx* pTx = new Tx;
  ASSERT(CN_TX == pTx->eClass());
  
  PtrVal xPtrTx;
  xPtrTx = pTx;
  
  nnpTask->vPushParam(xPtrTx);
}

/*----------------------------------------------------------------------------*/
/* tx.begin -- begin a tx                                                     */
/*----------------------------------------------------------------------------*/
void Task::vTxBegin(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTxDelete");
  
  PtrVal xPtrSession;
  
  if ( ! nnpTask->bPopParam(xPtrSession) )
    THROW_INVALID_PARAM("tx.delete");
  
  Tx* pTx = static_cast<Tx*>(xPtrSession.pPtr());
  ASSERT(CN_TX == pTx->eClass());
}

/*----------------------------------------------------------------------------*/
/* tx.commit -- commit a tx                                                   */
/*----------------------------------------------------------------------------*/
void Task::vTxCommit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTxCommit");
  
  PtrVal xPtrSession;
  
  if ( ! nnpTask->bPopParam(xPtrSession) )
    THROW_INVALID_PARAM("tx.commit");
  
  Tx* pTx = static_cast<Tx*>(xPtrSession.pPtr());
  ASSERT(CN_TX == pTx->eClass());
}

/*----------------------------------------------------------------------------*/
/* tx.rollback -- rollback a tx                                               */
/*----------------------------------------------------------------------------*/
void Task::vTxRollback(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTxRollback");
  
  PtrVal xPtrSession;
  
  if ( ! nnpTask->bPopParam(xPtrSession) )
    THROW_INVALID_PARAM("tx.rollback");
  
  Tx* pTx = static_cast<Tx*>(xPtrSession.pPtr());
  ASSERT(CN_TX == pTx->eClass());
}

// txlist: /////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* txlist.dump -- dump of txlist on param stack                               */
/*----------------------------------------------------------------------------*/
void Task::vTxListDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTxListDump");
  
  PtrVal xPtrTxList;
  if ( ! nnpTask->bPopParam(xPtrTxList) )
    THROW_INVALID_PARAM("txlist.dump");
    
  TxList* pTxList = reinterpret_cast<TxList*>(xPtrTxList.pPtr());
  ASSERT(CN_TXLIST == pTxList->eClass());
  
  pTxList->vDump("TxList:");
}

/*----------------------------------------------------------------------------*/
/* txlist.init -- init of txlist on param stack                               */
/*----------------------------------------------------------------------------*/
void Task::vTxListInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTxListInit");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("txlist.init");
  
  TxList* pTxList = reinterpret_cast<TxList*>(xPtrVal.pPtr());
  ASSERT(CN_TXLIST == pTxList->eClass());
  
  new(pTxList) TxList;
}

// txmap: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* txmap.init -- init of txmap on param stack                                 */
/*----------------------------------------------------------------------------*/
void Task::vTxMapInit(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vTxMapInit");
  
  PtrVal xPtrVal;
  if ( ! nnpTask->bPopParam(xPtrVal) )
    THROW_INVALID_PARAM("txmap.init");
  
  TxMap* pTxMap = reinterpret_cast<TxMap*>(xPtrVal.pPtr());
  ASSERT(CN_TXMAP == pTxMap->eClass());
  
  new(pTxMap) TxMap;
}

// words: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* create -- create word in dictionary                                        */
/*----------------------------------------------------------------------------*/
void Task::vCreate(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vCreate");
  
  nnpTask->vLiteralNext();
  
  Dictionary* pDictionary;
  if (nnpTask->m_pSEitf)
    pDictionary = nnpTask->m_pSEitf->pDicStack()->pTop();
  else
    pDictionary = nnpTask->m_xDictionaryStack.pTop();
  
  new(pDictionary) Word(nnpTask->m_sLiteral.a());
}

/*----------------------------------------------------------------------------*/
/* >create -- create word in dictionary using string on stack                 */
/*----------------------------------------------------------------------------*/
void Task::vCreateFrom(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vCreateFrom");

  s32 sName;
  if ( ! nnpTask->bPopParam(sName) )
    THROW_INVALID_PARAM(">create");
  
  Dictionary* pDictionary;
  if (nnpTask->m_pSEitf)
    pDictionary = nnpTask->m_pSEitf->pDicStack()->pTop();
  else
    pDictionary = nnpTask->m_xDictionaryStack.pTop();
  
  new(pDictionary) Word(sName.a());
}

/*----------------------------------------------------------------------------*/
/* does> -- routine does                                                      */
/*----------------------------------------------------------------------------*/
void Task::vDoes(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vDoes");
  
  Word* pCurrentWord = nnpTask->pCurrentWord();
  
  pCurrentWord->vEncode("encodeDoes");
  pCurrentWord->vEncode("exit");
}

/*----------------------------------------------------------------------------*/
/* find -- find word                                                          */
/*----------------------------------------------------------------------------*/
void Task::vFind(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vFind");
  
  s256 sName;
  if ( ! nnpTask->bPopParam(sName) )
    THROW_INVALID_PARAM("find");
 
  if (nnpTask->m_bTracing) {
    i8 i = 0; i++;
  }
  
  Word* pWord = nnpTask->pFindWord(sName);
  
  nnpTask->vPushParam(pWord);
}

/*----------------------------------------------------------------------------*/
/* forget -- forget word                                                      */
/*----------------------------------------------------------------------------*/
void Task::vForget(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vForget");
  
  nnpTask->vLiteralNext();
  
  Word* pWord = nnpTask->pFindWord(nnpTask->m_sLiteral);
  
  if ( ! pWord )
    THROW("No such word: %s",nnpTask->m_sLiteral.a());
  
  bool bResult = nnpTask->bForgetWord(nnpTask->m_sLiteral);
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* >forget -- forget word on stack                                            */
/*----------------------------------------------------------------------------*/
void Task::vWordForget(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vWordForget");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM(">forget");
  
  bool bResult = nnpTask->bForgetWord(pWord->sName());
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* word.dump -- dump word                                                     */
/*----------------------------------------------------------------------------*/
void Task::vWordDump(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vWordDump");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("word.dump");
  
  pWord->vDump();
}

/*----------------------------------------------------------------------------*/
/* word.dic -- push word's dictionary                                         */
/*----------------------------------------------------------------------------*/
void Task::vWordDic(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vWordDic");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("word.dic");
  
  Dictionary* pDictionary = pWord->pDictionary();
  
  nnpTask->vPushParam(pDictionary->sName().a());
}

/*----------------------------------------------------------------------------*/
/* word.kind@ -- word kind get                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindGet(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vWordKindGet");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("word.kind@");
    
  Word* pWordKind = pWord->pKind();
  
  nnpTask->vPushParam(pWordKind);
}

/*----------------------------------------------------------------------------*/
/* word.kind! -- word kind set                                                */
/*----------------------------------------------------------------------------*/
void Task::vWordKindSet(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vWordKindSet");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("word.kind!");
  
  Word* pWordKind;

  if ( ! nnpTask->bPopParam(pWordKind) )
    THROW_INVALID_PARAM("word.kind!");
  
  pWord->vKind(pWordKind);
}

/*----------------------------------------------------------------------------*/
/* word.name -- push name of word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordName(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vWordName");
  
  Word* pWord;
  
  if ( ! nnpTask->bPopParam(pWord) )
    THROW_INVALID_PARAM("word.name");
  
  char* pWordName = pWord->sName().a();
  
  nnpTask->vPushParam(pWordName);
}

/*----------------------------------------------------------------------------*/
/* word.this -- push current word                                             */
/*----------------------------------------------------------------------------*/
void Task::vWordThis(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vWordThis");
  
  Word* pWord = nnpTask->pCurrentWord();
  
  nnpTask->vPushParam(pWord);
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
  
  nnpTask->vPrimitive("vSocketAccept");
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("socketaccept");
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  bool bResult = pSocket->bAcceptClient(iSocketId, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to accept connection: Errno %d!", xSysCall.iError());
  
  IntVal xIntVal;
  xIntVal = iSocketId;
  nnpTask->vPushParam(xIntVal);
}

/*----------------------------------------------------------------------------*/
/* open a socket                                                              */
/*----------------------------------------------------------------------------*/
void Task::vSocketClose(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  
  nnpTask->vPrimitive("vSocketClose");
  
  if ( ! nnpTask->bPopParam(pObject) )
    THROW_INVALID_PARAM("socketclose");
  
  pSocket = reinterpret_cast<Socket*>(pObject);
  bool bResult = pSocket->bClose(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to close socket: Errno %d!", xSysCall.iError());
}

/*----------------------------------------------------------------------------*/
/* connect to a server                                                        */
/*----------------------------------------------------------------------------*/
void Task::vSocketConnect(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  Object* pObject;
  IntVal xPort;
  
  nnpTask->vPrimitive("vSocketConnect");
  
  if ( ! nnpTask->bPopParam(pObject, xPort) )
    THROW_INVALID_PARAM("socketconnect");
    
  //const char* sAddr = reinterpret_cast<char*>(pObject);
  //pSocket = new Socket;
  const char* sAddr = "127.0.0.1";
  pSocket = reinterpret_cast<Socket*>(pObject);
  bool bResult = pSocket->bConnectServer(sAddr, (int)xPort.iInt(), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to connect: Errno %d!", xSysCall.iError());
  
  PtrVal xPtrVal;
  xPtrVal = (void*)pSocket;
  nnpTask->vPushParam(xPtrVal);
}

/*----------------------------------------------------------------------------*/
/* listen in for socket connections                                           */
/*----------------------------------------------------------------------------*/
void Task::vSocketListen(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  i8      iPort;
  
  nnpTask->vPrimitive("vSocketListen");
  
  if ( ! nnpTask->bPopParam(iPort) )
    THROW_INVALID_PARAM("socketlisten");
  
  pSocket = new Socket;
  bool bResult = pSocket->bCreateServer((int)iPort, xSysCall);
  
  if ( ! bResult )
    THROW("Unable to listen for connections: Errno %d!", xSysCall.iError());
  
  PtrVal xPtrVal;
  xPtrVal = (void*)pSocket;
  nnpTask->vPushParam(xPtrVal);
}

/*----------------------------------------------------------------------------*/
/* open a socket                                                              */
/*----------------------------------------------------------------------------*/
void Task::vSocketOpen(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  
  nnpTask->vPrimitive("vSocketOpen");
  
  pSocket = new Socket;
  bool bResult = pSocket->bSocket(xSysCall);
  
  if ( ! bResult )
    THROW("Unable to create socker: Errno %d!", xSysCall.iError());
  
  PtrVal xPtrVal;
  xPtrVal = (void*)pSocket;
  nnpTask->vPushParam(xPtrVal);
}

/*----------------------------------------------------------------------------*/
/* send to a socket                                                           */
/*----------------------------------------------------------------------------*/
void Task::vSocketSend(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  char* pData;
  Object* pObject;
  s16K  sWorkspace;
  ValWsp xValWsp;
  
  nnpTask->vPrimitive("vSocketSend");
  
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
}

/*----------------------------------------------------------------------------*/
/* recv from a socket                                                         */
/*----------------------------------------------------------------------------*/
void Task::vSocketRecv(NNP_Task nnpTask)
{
  SysCall xSysCall;
  Socket* pSocket;
  ValWsp xValWsp;
  s16K   sWorkspace;
  i8 iSocketId;
  
  nnpTask->vPrimitive("vSocketRecv");
  
  if ( ! nnpTask->bPopParam(iSocketId) )
    THROW_INVALID_PARAM("socketrecv");
  
  char* pData = reinterpret_cast<char*>(sWorkspace.a());
  pSocket = new Socket((int)iSocketId);
  bool bResult = pSocket->bReadVch(reinterpret_cast<char**>(&pData), xSysCall);
  
  if ( ! bResult )
    THROW("Unable to recvFrom socket: Errno %d!", xSysCall.iError());
  
  xValWsp = pData;
  nnpTask->vPushParam(xValWsp);
}

/*----------------------------------------------------------------------------*/
/* start a socket client                                                      */
/*----------------------------------------------------------------------------*/
void Task::vClient(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vClient");
  
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
}

/*----------------------------------------------------------------------------*/
/* start a socket server                                                      */
/*----------------------------------------------------------------------------*/
void Task::vServer(NNP_Task nnpTask)
{
  nnpTask->vPrimitive("vServer");
  
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
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/