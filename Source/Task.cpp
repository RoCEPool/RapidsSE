/*============================================================================*/
/* Task.cpp - task                                                            */
/*============================================================================*/

//#define MUNGED

#include "StorageEngine.h"
#include "SePrompt.hpp"
#include "Schedulers.h"

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
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern Scheduler*      gpScheduler;
extern bool            gbVerbose;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Task* Tasks::_pTask(void* aAddr)
{
  Task* pTask = reinterpret_cast<Task*>(aAddr);
  
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
  
  vInit(""_Str);
    
  _checkInitialised_(CN_TASK);
  
  Task* pTask = this;
  gpTaskList->vAppend(pTask);
//  gpTaskList->vDump();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Task::Task(MP_Task mpTask)
: Object(CN_TASK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  ASSERT(*mpTask);
  ASSERT(gpTaskList);
  
  vInit(""_Str);
  
  _checkInitialised_(CN_TASK);
  
  Task* pTask = this;
  gpTaskList->vAppend(pTask);
//  gpTaskList->vDump();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor  (for the request task)                                        */
/*----------------------------------------------------------------------------*/
Task::Task(Value* pRequest)
: Object(CN_TASK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  ASSERT(gpTaskList);
  
  vInit(""_Str);
  
  m_pRequest = new ValSet;
  
  // copy the request
  i8 nBytes = ValLib::iSize(pRequest);
  MemLib::vCopy(m_pRequest,pRequest,nBytes);
  
  m_sName    = "req.task"_Str;
  m_sComment = "requestLoop"_Str;
  m_eActorModel = AC_SERVER;
  
  _checkInitialised_(CN_TASK);
  
  Task* pTask = this;
  gpTaskList->vAppend(pTask);
  //  gpTaskList->vDump();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Task::Task(Str* aLambdaInput)
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
  
  // delete the stacks
  delete m_pDataStack;
  delete m_pReturnStack;
  delete m_pAuxStack;
  
  delete m_pTokeniser;
  
  if (m_pRequest)
    delete m_pRequest;
  
  Task* pTask = this;
  gpTaskList->vRemove(pTask);
//  gpTaskList->vDump();
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void Task::vInit(Str* aLambda)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  static i8 iTaskNo = 0;
  
  // basic state
  m_xLock.vInitLock(LK_TASK);
  
  m_iTaskNo     = iTaskNo++;
  m_eTaskKind   = TK_NONE;
  m_eActorModel = AC_NONE;
  m_sName       = "<no task name>"_Str;
  m_eState      = TS_NONE;
  m_iTxId       = -1;
  m_npNextInSet = nullptr;
  m_npPrevInSet = nullptr;
  m_iWakeAt     = 0;
  m_pParentTask = nullptr;
  m_zAcks.vCheckInit();
  m_pThread     = nullptr;
  m_pTaskSet    = nullptr;
  
  m_sComment    = ""_Str;
  
  m_pJobQueue   = nullptr;
  
  m_pDataStack   = new Stack;
  m_pReturnStack = new Stack;
  m_pAuxStack    = new Stack;
  
  m_pRegFrame    = nullptr;
  m_pLocalsFrame = nullptr;
  m_pOpFrame = nullptr;
  
  IP.m_pCell     = nullptr;
  m_pEntryCell   = nullptr;
  
  m_pResetIP     = nullptr;
  m_pBeginIP     = nullptr;
  m_pWord        = nullptr;
  m_bCompiling   = false;
  m_iTracing     = 0;
  m_nVariables   = 0;
  m_pCurrentWord = nullptr;
  m_nSubDic      = 0;
  
  m_pRequest     = nullptr;
  
  m_sLambda = aLambda;
  if (m_sLambda.iLength())
    m_sLambda.vConcat(" exit"_Str);
  
  m_pTokeniser = new Tokeniser;
  m_pTokeniser->vPush();
  m_pTokeniser->vUseString(m_sLambda);
  
  m_pSEitf   = nullptr;
  m_pSocket  = nullptr;
  m_pPlan    = nullptr;
  
  m_iDdlLocked = 0;
}

/*----------------------------------------------------------------------------*/
/* add initial set of words in main dictionary                                */
/*----------------------------------------------------------------------------*/
void Task::vInitialiseMain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddPrimitives();
  
  vAddConstants();
  
  vAddRoutines();

  MP_Task mpTask(this);
  
#ifdef MUNGED
  vAddMunged(mpTask);
#endif
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get actor model                                                            */
/*----------------------------------------------------------------------------*/
eAC Task::eActorModel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_eActorModel;
}

/*----------------------------------------------------------------------------*/
/* set actor model                                                            */
/*----------------------------------------------------------------------------*/
void Task::vActorModel(eAC eActorModel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_eActorModel = eActorModel;
}

/*----------------------------------------------------------------------------*/
/* add constant to main dictionary                                            */
/*----------------------------------------------------------------------------*/
void Task::vAddConstant(Str* aName, i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  Word* pNewWord = new(gpMainDic) Word(aName);
  
  IntVal xIntVal;
  xIntVal = iVal;
  
  pNewWord->vEncode("pushValue"_Str,&xIntVal);
  pNewWord->vEncode("exit"_Str);
}

/*----------------------------------------------------------------------------*/
/* add constant to main dictionary                                            */
/*----------------------------------------------------------------------------*/
void Task::vAddConstant(Str* aName, Str* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pNewWord = new(gpMainDic) Word(aName);
  
  ValWrk xWspStr;
  xWspStr = aValue;
  
  pNewWord->vEncode("pushValue"_Str,&xWspStr);
  pNewWord->vEncode("exit"_Str);
}

/*----------------------------------------------------------------------------*/
/* add primitive word into main dictionary                                    */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitive(Str* aName, TaskStep pStep, bool bImmediate)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
    
  Word* pNewWord = new(gpMainDic) Word(aName);
  
  m_pCurrentWord = pNewWord;
  
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
  
  IP.m_pCell = pAdd<Cell*>(IP.m_pCell,iBytes);
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
  
  if (gbVerbose)
    vTrace("[[mainThread]] Adding constants...\n"_Str);
  
#ifdef _DEBUG
  vAddConstant("_DEBUG"_Str,1);
#else
  vAddConstant("_DEBUG"_Str,(i8)0);
#endif
  
  vAddConstant("version"_Str,          (Str*)RAPIDSSE_VERSION_STRING);
  
  vAddConstant("jumbobuffersize"_Str,  C_SIZE_JUMBO_OBJECT);
  vAddConstant("largebuffersize"_Str,  C_SIZE_POOL_BUFFER);
  vAddConstant("mediumbuffersize"_Str, C_SIZE_MEDIUM_OBJECT);
  vAddConstant("smallbuffersize"_Str,  C_SIZE_SMALL_OBJECT);
  vAddConstant("magic"_Str,            MAGIC);
  
  // Arguments
  vAddConstant("args.bgtasks"_Str,           gxArguments.m_bBgTasks);
  vAddConstant("args.command"_Str,           gxArguments.m_sCommand);
  vAddConstant("args.commands"_Str,          gxArguments.m_sCommands);
  vAddConstant("args.config"_Str,            gxArguments.m_bConfig);
  vAddConstant("args.docol"_Str,             gxArguments.m_bDocol);
  vAddConstant("args.dontFree"_Str,          gxArguments.m_bDontFree);
  vAddConstant("args.folder"_Str,            gxArguments.m_sFolder);
  vAddConstant("args.freeRB"_Str,            gxArguments.m_bFreeRB);
  vAddConstant("args.frequency"_Str,         gxArguments.m_iFrequency);
  vAddConstant("args.help"_Str,              gxArguments.m_bHelp);
  vAddConstant("args.hugePages"_Str,         gxArguments.m_bHugePages);
  vAddConstant("args.jumboBuffers"_Str,      gxArguments.m_nJumboBuffers);
  vAddConstant("args.jumboSize"_Str,         gxArguments.m_iJumboSize);
  vAddConstant("args.jumboTook"_Str,         gxArguments.m_nJumboTook);
  vAddConstant("args.killPrev"_Str,          gxArguments.m_bKillPrev);
  vAddConstant("args.levels"_Str,            gxArguments.m_iLevels);
  vAddConstant("args.lock"_Str,              gxArguments.m_bLock);
  vAddConstant("args.metadata"_Str,          gxArguments.m_bMetadata);
  vAddConstant("args.munged"_Str,            gxArguments.m_bMunged);
  vAddConstant("args.nodeName"_Str,          gxArguments.m_sNodeName);
  vAddConstant("args.nodes"_Str,             gxArguments.m_nNodes);
  vAddConstant("args.partitionsPerNode"_Str, gxArguments.m_nSlots);
  vAddConstant("args.progress"_Str,          gxArguments.m_bProgress);
  vAddConstant("args.prompt"_Str,            gxArguments.m_bPrompt);
  vAddConstant("args.regionPrefix"_Str,      gxArguments.m_sRegionPrefix);
  vAddConstant("args.regionName"_Str,        gxArguments.m_sRegionName);
  vAddConstant("args.regionSize"_Str,        gxArguments.m_iRegionSize
                                               - gxArguments.m_iJumboSize);
  vAddConstant("args.restart"_Str,           gxArguments.m_bRestart);
  vAddConstant("args.sen"_Str,               gxArguments.m_sSen);
  vAddConstant("args.sleep"_Str,             gxArguments.m_iSleep);
  vAddConstant("args.slots"_Str,             gxArguments.m_nSlots);
  vAddConstant("args.start"_Str,             gxArguments.m_bStart);
  vAddConstant("args.status"_Str,            gxArguments.m_bStatus);
  vAddConstant("args.stop"_Str,              gxArguments.m_bStop);
  vAddConstant("args.touch"_Str,             gxArguments.m_bTouch);
  vAddConstant("args.tracepoint"_Str,        gxArguments.m_bTracepoint);
  vAddConstant("args.tracing"_Str,           gxArguments.m_bTracing);
  vAddConstant("args.unitTests"_Str,         gxArguments.m_bUnitTests);
  vAddConstant("args.unlink"_Str,            gxArguments.m_bUnlink);
  vAddConstant("args.verbose"_Str,           gxArguments.m_bVerbose);
  vAddConstant("args.welcome"_Str,           gxArguments.m_bWelcome);
  
  // true / flase
  vAddConstant("true"_Str,    (i8)1);
  vAddConstant("false"_Str,   (i8)0);

  // useful numbers
  vAddConstant("C_1KB"_Str,   C_1KB);
  vAddConstant("C_1MB"_Str,   C_1MB);
  vAddConstant("C_1GB"_Str,   C_1GB);
  vAddConstant("C_1TB"_Str,   C_1TB);
  vAddConstant("C_1PB"_Str,   C_1PB);
  vAddConstant("C_1EB"_Str,   C_1EB);
  
  vAddConstant("C_1K"_Str,    C_1K);
  vAddConstant("C_1M"_Str,    C_1M);
  vAddConstant("C_1B"_Str,    C_1B);
  
  vAddConstant("C_4KB"_Str,   C_4KB);
  vAddConstant("C_16KB"_Str,  C_16KB);
  vAddConstant("C_64KB"_Str,  C_64KB);
  vAddConstant("C_256KB"_Str, C_256KB);
  
  // AC_ : actor models
  /*  0 */ vAddConstant("AC_NONE"_Str,         AC_NONE);
  /*  1 */ vAddConstant("AC_DRONE"_Str,        AC_DRONE);
  /*  2 */ vAddConstant("AC_PARTITION"_Str,    AC_PARTITION);
  /*  3 */ vAddConstant("AC_SERVER"_Str,       AC_SERVER);
  /*  4 */ vAddConstant("AC_WORKER"_Str,       AC_WORKER);
  /*  5 */ vAddConstant("AC_ENUMS"_Str,        AC_ENUMS);

  // AM_ : attach map numbers
  /*  0 */ vAddConstant("AM_NONE"_Str,         AM_NONE);
  /*  1 */ vAddConstant("AM_INDEXENTRIES"_Str, AM_INDEXENTRIES);
  /*  2 */ vAddConstant("AM_INDEXLEVELS"_Str,  AM_INDEXLEVELS);
  /*  3 */ vAddConstant("AM_INDEXSPACE"_Str,   AM_INDEXSPACE);
  /*  4 */ vAddConstant("AM_ENUMS"_Str,        AM_ENUMS);
  
  // CN_ : class numbers
  /*  0 */ vAddConstant("CN_NONE"_Str,         CN_NONE);

  /*    */ vAddConstant("CN_BINVAL"_Str,       CN_BINVAL);
  /*  4 */ vAddConstant("CN_INTVAL"_Str,       CN_INTVAL);
  /*  5 */ vAddConstant("CN_IPVAL"_Str,        CN_IPVAL);
  /*  6 */ vAddConstant("CN_OBJVAL"_Str,       CN_OBJVAL);
  /*  7 */ vAddConstant("CN_PTRVAL"_Str,       CN_PTRVAL);
  /*  1 */ vAddConstant("CN_STRVAR"_Str,       CN_STRVAR);
  /*  8 */ vAddConstant("CN_STRVAL"_Str,       CN_STRVAL);
  /*  9 */ vAddConstant("CN_VALREG"_Str,       CN_VALREG);
  /* 10 */ vAddConstant("CN_VALSET"_Str,       CN_VALSET);
  /* 11 */ vAddConstant("CN_VALWRK"_Str,       CN_VALWRK);
  
  /* 12 */ vAddConstant("CN_ARGUMENTS"_Str,    CN_ARGUMENTS);
  /* 13 */ vAddConstant("CN_ATOMIC"_Str,       CN_ATOMIC);
  /* 14 */ vAddConstant("CN_ATTACHMAP"_Str,    CN_ATTACHMAP);
  
  /* 16 */ vAddConstant("CN_BUFFERPOOL"_Str,   CN_BUFFERPOOL);
  /* 17 */ vAddConstant("CN_BPA"_Str,          CN_BPA);
  
  /* 18 */ vAddConstant("CN_DIC"_Str,          CN_DIC);
  /* 19 */ vAddConstant("CN_DICPATH"_Str,      CN_DICPATH);
  /* 20 */ vAddConstant("CN_DIR"_Str,          CN_DIR);
  
  /* 21 */ vAddConstant("CN_FILE"_Str,         CN_FILE);
  /* 15 */ vAddConstant("CN_FREEBUFFER"_Str,   CN_FREEBUFFER);
  /* 22 */ vAddConstant("CN_FREECHUNK"_Str,    CN_FREECHUNK);
  
  /* 23 */ vAddConstant("CN_GRAPH"_Str,        CN_GRAPH);
  /* 24 */ vAddConstant("CN_GRAPHCURSOR"_Str,  CN_GRAPHCURSOR);
  /* 25 */ vAddConstant("CN_GRAPHNODE"_Str,    CN_GRAPHNODE);
  /* 26 */ vAddConstant("CN_GRID"_Str,         CN_GRID);
  
  /* 27 */ vAddConstant("CN_HEAPBUFFER"_Str,   CN_HEAPBUFFER);
  
  /* 28 */ vAddConstant("CN_INDEX"_Str,        CN_INDEX);
  /*    */ //vAddConstant("CN_INDEXDEFN"_Str,    CN_INDEXDEFN);

  /* 29 */ vAddConstant("CN_JUMBOHEAP"_Str,    CN_JUMBOHEAP);

  /* 31 */ vAddConstant("CN_KEYWRK"_Str,       CN_KEYWRK);
  
  /* 32 */ vAddConstant("CN_LOADCHUNK"_Str,    CN_LOADCHUNK);
  /* 33 */ vAddConstant("CN_LOADER"_Str,       CN_LOADER);
  /* 34 */ vAddConstant("CN_LOADERSUB"_Str,    CN_LOADERSUB);
  /* 35 */ vAddConstant("CN_LOADMAP"_Str,      CN_LOADMAP);
  /* 36 */ vAddConstant("CN_LOCK"_Str,         CN_LOCK);
  
  /* 37 */ vAddConstant("CN_MEDIUMHEAP"_Str,   CN_MEDIUMHEAP);
  
  /* 38 */ vAddConstant("CN_NODE"_Str,         CN_NODE);
  /* 39 */ vAddConstant("CN_NODECURSOR"_Str,   CN_NODECURSOR);
  
  /* 40 */ vAddConstant("CN_PARSER"_Str,       CN_PARSER);
  /* 41 */ vAddConstant("CN_PARTITION"_Str,    CN_PARTITION);
  /*    */ vAddConstant("CN_PLAN"_Str,         CN_PLAN);
  /*    */ vAddConstant("CN_PLANLIST"_Str,     CN_PLANLIST);
  /* 43 */ vAddConstant("CN_PROXY"_Str,        CN_PROXY);
  /* 44 */ vAddConstant("CN_PROXYSET"_Str,     CN_PROXYSET);
  
  /* 45 */ vAddConstant("CN_QUEUE"_Str,        CN_QUEUE);
  /* 46 */ vAddConstant("CN_QUEUEBUFFER"_Str,  CN_QUEUEBUFFER);
  
  /*    */ vAddConstant("CN_RBCURSOR"_Str,     CN_RBCURSOR);
  /* 47 */ vAddConstant("CN_REGFRAME"_Str,     CN_REGFRAME);
  /* 48 */ vAddConstant("CN_RING"_Str,         CN_RING);
  /* 49 */ vAddConstant("CN_ROW"_Str,          CN_ROW);
  /* 50 */ vAddConstant("CN_ROWBUFFER"_Str,    CN_ROWBUFFER);
  /* 51 */ vAddConstant("CN_ROWCURSOR"_Str,    CN_ROWCURSOR);
  /* 52 */ vAddConstant("CN_EBSTRAND"_Str,     CN_EBSTRAND);
  
  /* 54 */ vAddConstant("CN_SCANBUFFER"_Str,   CN_SCANBUFFER);
  /* 55 */ vAddConstant("CN_SCRATCH"_Str,      CN_SCRATCH);
  /* 56 */ vAddConstant("CN_SEHISTORY"_Str,    CN_SEHISTORY);
  /* 57 */ vAddConstant("CN_SEITF"_Str,        CN_SEITF);
  /* 58 */ vAddConstant("CN_SEITFCURSOR"_Str,  CN_SEITFCURSOR);
  /* 59 */ vAddConstant("CN_SEITFLIST"_Str,    CN_SEITFLIST);
  /*    */ vAddConstant("CN_SELCURSOR"_Str,    CN_SELCURSOR);
  /* 60 */ vAddConstant("CN_SEPROMPT"_Str,     CN_SEPROMPT);
  /* 61 */ vAddConstant("CN_SINCE"_Str,        CN_SINCE);
  /* 62 */ vAddConstant("CN_SKIPCURSOR"_Str,   CN_SKIPCURSOR);
  /* 63 */ vAddConstant("CN_SKIPLIST"_Str,     CN_SKIPLIST);
  /* 64 */ vAddConstant("CN_SKIPNODE"_Str,     CN_SKIPNODE);
  /*    */ vAddConstant("CN_SLOTS"_Str,        CN_SLOTS);
  /* 65 */ vAddConstant("CN_SMALLHEAP"_Str,    CN_SMALLHEAP);
  /* 66 */ vAddConstant("CN_SOCKET"_Str,       CN_SOCKET);
  /* 67 */ vAddConstant("CN_STACK"_Str,        CN_STACK);
  /* 68 */ vAddConstant("CN_STEPACCUM"_Str,    CN_STEPACCUM);
  /* 69 */ vAddConstant("CN_STEPARRAY"_Str,    CN_STEPARRAY);
  /* 70 */ vAddConstant("CN_SYSCALL"_Str,      CN_SYSCALL);
  
  /* 71 */ vAddConstant("CN_TABLE"_Str,        CN_TABLE);
  /* 72 */ vAddConstant("CN_TASK"_Str,         CN_TASK);
  /* 73 */ vAddConstant("CN_TASKCURSOR"_Str,   CN_TASKCURSOR);
  /* 74 */ vAddConstant("CN_TASKLIST"_Str,     CN_TASKLIST);
  /* 75 */ vAddConstant("CN_SCHEDULER"_Str,    CN_SCHEDULER);
  /* 75 */ vAddConstant("CN_TASKSET"_Str,      CN_TASKSET);
  /* 76 */ vAddConstant("CN_THREAD"_Str,       CN_THREAD);
  /* 77 */ vAddConstant("CN_THREADCURSOR"_Str, CN_THREADCURSOR);
  /* 78 */ vAddConstant("CN_THREADLIST"_Str,   CN_THREADLIST);
  /* 79 */ vAddConstant("CN_TIME"_Str,         CN_TIME);
  /* 80 */ vAddConstant("CN_TIMER"_Str,        CN_TIMER);
  /* 81 */ vAddConstant("CN_TOKENISER"_Str,    CN_TOKENISER);
  /* 82 */ vAddConstant("CN_TX"_Str,           CN_TX);
  /* 83 */ vAddConstant("CN_TXARRAY"_Str,      CN_TXARRAY);
  /* 84 */ vAddConstant("CN_TXCOLLECTION"_Str, CN_TXCOLLECTION);
  /* 85 */ vAddConstant("CN_TXCURSOR"_Str,     CN_TXCURSOR);
  /* 86 */ vAddConstant("CN_TXMAP"_Str,        CN_TXMAP);
  /* 87 */ vAddConstant("CN_TXREF"_Str,        CN_TXREF);
  
  /* 88 */ vAddConstant("CN_WORD"_Str,         CN_WORD);

  /* 89 */ vAddConstant("CN_ZSMALL_"_Str,      CN_ZSMALL_);
  /* 90 */ vAddConstant("CN_ZMEDIUM_"_Str,     CN_ZMEDIUM_);
  /* 91 */ vAddConstant("CN_ZLARGE_"_Str,      CN_ZLARGE_);
  /* 92 */ vAddConstant("CN_ZJUMBO_"_Str,      CN_ZJUMBO_);
  
  /* 93 */ vAddConstant("CN_ENUMS"_Str,        CN_ENUMS);
  
  // DE_ : directory entry type -- n.b. the DT_FIFO etc are from dirent.h
  /*  1 */ vAddConstant("DE_FIFO"_Str, DT_FIFO);
  /*  2 */ vAddConstant("DE_CHR"_Str,  DT_CHR);
  /*  3 */ vAddConstant("DE_DIR"_Str,  DT_DIR);
  /*  6 */ vAddConstant("DE_BLK"_Str,  DT_BLK);
  /*  8 */ vAddConstant("DE_REG"_Str,  DT_REG);
  /* 10 */ vAddConstant("DE_LNK"_Str,  DT_LNK);
  /* 12 */ vAddConstant("DE_SOCK"_Str, DT_SOCK);

  // DM_ : datum type
  /*  1 */ vAddConstant("DM_BCD"_Str, DM_BCD);
  /*  2 */ vAddConstant("DM_BOL"_Str, DM_BOL);
  /*  3 */ vAddConstant("DM_DTE"_Str, DM_DTE);
  /*  4 */ vAddConstant("DM_FLT"_Str, DM_FLT);
  /*  5 */ vAddConstant("DM_INT"_Str, DM_INT);
  /*  6 */ vAddConstant("DM_IVL"_Str, DM_IVL);
  /*  7 */ vAddConstant("DM_TME"_Str, DM_TME);
  /*  8 */ vAddConstant("DM_TSP"_Str, DM_TSP);
  
  // EC_ : error codes
  /*  0 */ vAddConstant("EC_NONE"_Str,               EC_NONE);
  /*  1 */ vAddConstant("EC_ASSERT"_Str,             EC_ASSERT);
  /*  2 */ vAddConstant("EC_BEGIN_TX_FAILED"_Str,    EC_BEGIN_TX_FAILED);
  /*  3 */ vAddConstant("EC_CATALOG_EXISTS"_Str,     EC_CATALOG_EXISTS);
  /*  4 */ vAddConstant("EC_CLUSTER_NOT_STARTED"_Str,EC_CLUSTER_NOT_STARTED);
  /*  5 */ vAddConstant("EC_DUPLICATE_COLUMN"_Str,   EC_DUPLICATE_COLUMN);
  /*  6 */ vAddConstant("EC_FOLDER_IN_USE"_Str,      EC_FOLDER_IN_USE);
  /*  7 */ vAddConstant("EC_FOLDER_NOT_EMPTY"_Str,   EC_FOLDER_NOT_EMPTY);
  /*  8 */ vAddConstant("EC_INDEX_EXISTS"_Str,       EC_INDEX_EXISTS);
  /*  9 */ vAddConstant("EC_INVALID_CLASS"_Str,      EC_INVALID_CLASS);
  /*  9 */ vAddConstant("EC_INVALID_COLUMN"_Str,     EC_INVALID_COLUMN);
  /* 10 */ vAddConstant("EC_INVALID_COMMAND"_Str,    EC_INVALID_COMMAND);
  /* 11 */ vAddConstant("EC_INVALID_FILE"_Str,       EC_INVALID_FILE);
  /* 12 */ vAddConstant("EC_INVALID_INTERFACE"_Str,  EC_INVALID_INTERFACE);
  /* 13 */ vAddConstant("EC_INVALID_PARAMETER"_Str,  EC_INVALID_PARAMETER);
  /* 14 */ vAddConstant("EC_INVALID_PARAMETERS"_Str, EC_INVALID_PARAMETERS);
  /* 15 */ vAddConstant("EC_INVALID_INSTANCE"_Str,   EC_INVALID_INSTANCE);
  /* 16 */ vAddConstant("EC_INVALID_SYNTAX"_Str,     EC_INVALID_SYNTAX);
  /* 17 */ vAddConstant("EC_LOW_BUFFERS"_Str,        EC_LOW_BUFFERS);
  /* 18 */ vAddConstant("EC_NO_COLUMNS_DIC"_Str,     EC_NO_COLUMNS_DIC);
  /* 19 */ vAddConstant("EC_NO_SUCH_CATALOG"_Str,    EC_NO_SUCH_CATALOG);
  /* 20 */ vAddConstant("EC_NO_SUCH_DIRECTORY"_Str,  EC_NO_SUCH_DIRECTORY);
  /* 21 */ vAddConstant("EC_NO_SUCH_FOLDER"_Str,     EC_NO_SUCH_FOLDER);
  /* 22 */ vAddConstant("EC_NO_SUCH_INDEX"_Str,      EC_NO_SUCH_INDEX);
  /* 23 */ vAddConstant("EC_NO_SUCH_SCHEMA"_Str,     EC_NO_SUCH_SCHEMA);
  /* 24 */ vAddConstant("EC_NO_SUCH_TABLE"_Str,      EC_NO_SUCH_TABLE);
  /* 25 */ vAddConstant("EC_NO_SUCH_TENANT"_Str,     EC_NO_SUCH_TENANT);
  /* 26 */ vAddConstant("EC_NO_SUCH_WORD"_Str,       EC_NO_SUCH_WORD);
  
  /* 27 */ vAddConstant("EC_NOT_A_CATALOG"_Str,      EC_NOT_A_CATALOG);
  /* 28 */ vAddConstant("EC_NOT_A_FOLDER"_Str,       EC_NOT_A_FOLDER);
  /* 29 */ vAddConstant("EC_NOT_AN_INDEX"_Str,       EC_NOT_AN_INDEX);
  /* 30 */ vAddConstant("EC_NOT_A_SCHEMA"_Str,       EC_NOT_A_SCHEMA);
  /* 31 */ vAddConstant("EC_NOT_A_TABLE"_Str,        EC_NOT_A_TABLE);
  /* 32 */ vAddConstant("EC_NOT_A_TENANT"_Str,       EC_NOT_A_TENANT);
  
  /* 33 */ vAddConstant("EC_NOT_IN_CATALOGS"_Str,    EC_NOT_IN_CATALOGS);
  /* 34 */ vAddConstant("EC_NOT_IN_COLUMNS"_Str,     EC_NOT_IN_COLUMNS);
  /* 35 */ vAddConstant("EC_NOT_IN_INDEXES"_Str,     EC_NOT_IN_INDEXES);
  /*    */ vAddConstant("EC_NOT_IN_PARTITIONS"_Str,  EC_NOT_IN_PARTITIONS);
  /* 36 */ vAddConstant("EC_NOT_IN_SCHEMAS"_Str,     EC_NOT_IN_SCHEMAS);
  /* 37 */ vAddConstant("EC_NOT_IN_TABLE"_Str,       EC_NOT_IN_TABLE);
  /* 38 */ vAddConstant("EC_NOT_IN_TABLES"_Str,      EC_NOT_IN_TABLES);
  /* 39 */ vAddConstant("EC_NOT_IN_TENANTS"_Str,     EC_NOT_IN_TENANTS);
  /* 40 */ vAddConstant("EC_NULL_INTERFACE"_Str,     EC_NULL_INTERFACE);
  /* 41 */ vAddConstant("EC_NULLPTR"_Str,            EC_NULLPTR);
  /* 42 */ vAddConstant("EC_RESPONSE_PENDING"_Str,   EC_RESPONSE_PENDING);
  /* 43 */ vAddConstant("EC_SCHEMA_EXISTS"_Str,      EC_SCHEMA_EXISTS);
  /* 44 */ vAddConstant("EC_SE_NOT_RUNNING"_Str,     EC_SE_NOT_RUNNING);
  /* 45 */ vAddConstant("EC_SE_UNKNOWN_ERROR"_Str,   EC_SE_UNKNOWN_ERROR);
  /* 46 */ vAddConstant("EC_SEND_FAILURE"_Str,       EC_SEND_FAILURE);
  
  /* 47 */ vAddConstant("EC_TABLE_EXISTS"_Str,       EC_TABLE_EXISTS);
  /* 48 */ vAddConstant("EC_TABLE_IN_USE"_Str,       EC_TABLE_IN_USE);
  /* 48 */ vAddConstant("EC_TABLE_NOT_EMPTY"_Str,    EC_TABLE_NOT_EMPTY);
  /* 49 */ vAddConstant("EC_TENANT_EXISTS"_Str,      EC_TENANT_EXISTS);
  /* 50 */ vAddConstant("EC_VALIDATION_FAILURE"_Str, EC_VALIDATION_FAILURE);
  /* 51 */ vAddConstant("EC_VERSION_MISMATCH"_Str,   EC_VERSION_MISMATCH); 
  /* 52 */ vAddConstant("EC_ENUMS"_Str,              EC_ENUMS);

  // AT_ : fetch techniques
  /*  0 */ vAddConstant("AT_NONE"_Str,  AT_NONE);
  /*  1 */ vAddConstant("AT_EXACT"_Str, AT_EXACT);
  /*  2 */ vAddConstant("AT_LOWER"_Str, AT_LOWER);
  /*  3 */ vAddConstant("AT_UPPER"_Str, AT_UPPER);
  /*  4 */ vAddConstant("AT_RANGE"_Str, AT_RANGE);
  /*  5 */ vAddConstant("AT_SCAN"_Str,  AT_SCAN);
  /*  6 */ vAddConstant("AT_ENUMS"_Str, AT_ENUMS);
  
  // TN_ : token types
  /*  0 */ vAddConstant("TN_NONE"_Str,    TN_NONE);
  /*  1 */ vAddConstant("TN_NAME"_Str,    TN_NAME);
  /*  2 */ vAddConstant("TN_INTEGER"_Str, TN_INTEGER);
  /*  3 */ vAddConstant("TN_STRING"_Str,  TN_STRING);
  /*  4 */ vAddConstant("TN_ENUMS"_Str,   TN_ENUMS);
  
  /*  0 */ vAddConstant("TH_NONE"_Str,                TH_NONE);
  /*  1 */ vAddConstant("TH_ANSWER"_Str,              TH_ANSWER);
  /*  2 */ vAddConstant("TH_COORDINATOR"_Str,         TH_COORDINATOR);
  /*  3 */ vAddConstant("TH_COORDINATOR_PROXY"_Str,   TH_COORDINATOR_PROXY);
  /*  4 */ vAddConstant("TH_GENERAL"_Str,             TH_GENERAL);
  /*  5 */ vAddConstant("TH_LISTER"_Str,              TH_LISTER);
  /*  6 */ vAddConstant("TH_MONITOR"_Str,             TH_MONITOR);
  /*  7 */ vAddConstant("TH_QUERY"_Str,               TH_QUERY);
  /*  8 */ vAddConstant("TH_PARTICIPANT"_Str,         TH_PARTICIPANT);
  /*  9 */ vAddConstant("TH_PARTICIPANT_PROXY"_Str,   TH_PARTICIPANT_PROXY);
  /* 10 */ vAddConstant("TH_RECLAIM"_Str,             TH_RECLAIM);
  /* 11 */ vAddConstant("TH_REQUEST"_Str,             TH_REQUEST);
  /* 12 */ vAddConstant("TH_SCANOP"_Str,              TH_SCANOP);
  /* 13 */ vAddConstant("TH_SINKOP"_Str,              TH_SINKOP);
  /* 14 */ vAddConstant("TH_WRITEOP"_Str,             TH_WRITEOP);
  /* 15 */ vAddConstant("TH_ENUMS"_Str,               TH_ENUMS); 
  
  // SIZEOF_ : C++ class sizes
  /*  1 */ vAddConstant("SIZEOF_INTVAL"_Str, sizeof(IntVal));
  /*  2 */ vAddConstant("SIZEOF_IPVAL"_Str,  sizeof(IpVal));
  /*  3 */ vAddConstant("SIZEOF_OBJVAL"_Str, sizeof(ObjVal));
  /*  4 */ vAddConstant("SIZEOF_PTRVAL"_Str, sizeof(PtrVal));
  /*  5 */ vAddConstant("SIZEOF_STRVAL"_Str, sizeof(StrVal));
  /*  6 */ vAddConstant("SIZEOF_VALSET"_Str, sizeof(ValWrk)); // correct
  /*  7 */ vAddConstant("SIZEOF_ValWrk"_Str, sizeof(ValWrk)); // correct
  
  /*  8 */ vAddConstant("SIZEOF_ARGUMENTS"_Str,    sizeof(Arguments));
  /*  9 */ vAddConstant("SIZEOF_ATOMIC"_Str,       sizeof(Atomic));
  /* 10 */ vAddConstant("SIZEOF_ATTACHMAP"_Str,    sizeof(AttachMap));
  /* 11 */ vAddConstant("SIZEOF_BUFFER"_Str,       sizeof(Buffer));
  /* 12 */ vAddConstant("SIZEOF_BUFFERPOOL"_Str,   sizeof(BufferPool));
  /* 13 */ vAddConstant("SIZEOF_BPA"_Str,          sizeof(BPA));
  /* 14 */ vAddConstant("SIZEOF_DIC"_Str,          sizeof(Dic));
  /* 15 */ vAddConstant("SIZEOF_DICPATH"_Str,      sizeof(DicPath));
  /* 16 */ vAddConstant("SIZEOF_DIR"_Str,          sizeof(Dir));
  /* 17 */ vAddConstant("SIZEOF_FILE"_Str,         sizeof(File));
  /* 18 */ vAddConstant("SIZEOF_FREECHUNK"_Str,    sizeof(FreeChunk));
  /* 19 */ vAddConstant("SIZEOF_GRID"_Str,         sizeof(Grid));
  /* 20 */ vAddConstant("SIZEOF_HEAPBUFFER"_Str,   sizeof(HeapBuffer));
  /*    */ vAddConstant("SIZEOF_INDEX"_Str,        sizeof(Index));
  /* 21 */ //vAddConstant("SIZEOF_INDEXDEFN"_Str,    sizeof(IndexDefn));
  /* 22 */ vAddConstant("SIZEOF_LOADER"_Str,       sizeof(Loader));
  /* 23 */ vAddConstant("SIZEOF_LOADERSUB"_Str,    sizeof(LoaderSub));
  /* 24 */ vAddConstant("SIZEOF_LOCK"_Str,         sizeof(Lock));
  /* 25 */ vAddConstant("SIZEOF_MEDIUMHEAP"_Str,   sizeof(Heap));
  /* 26 */ vAddConstant("SIZEOF_NODE"_Str,         sizeof(Node));
  /* 27 */ vAddConstant("SIZEOF_NODECURSOR"_Str,   sizeof(NodeCursor));
  /* 28 */ vAddConstant("SIZEOF_PARSER"_Str,       sizeof(Parser));
  /* 29 */ vAddConstant("SIZEOF_PARTITION"_Str,    sizeof(Partition));
  /*    */ vAddConstant("SIZEOF_PROXY"_Str,        sizeof(Proxy));
  /* 32 */ vAddConstant("SIZEOF_PROXYSET"_Str,     sizeof(ProxySet));
  /* 33 */ vAddConstant("SIZEOF_QUEUE"_Str,        sizeof(Queue));
  /* 34 */ vAddConstant("SIZEOF_QUEUEBUFFER"_Str,  sizeof(QueueBuffer));
  /*    */ vAddConstant("SIZEOF_RBCURSOR"_Str,     sizeof(RbCursor));
  /* 35 */ vAddConstant("SIZEOF_RING"_Str,         sizeof(Ring));
  /* 36 */ vAddConstant("SIZEOF_ROW"_Str,          sizeof(Entry));
  /* 37 */ vAddConstant("SIZEOF_ROWBUFFER"_Str,    sizeof(EntryBuffer));
  /* 38 */ vAddConstant("SIZEOF_ROWCURSOR"_Str,    (i8)0);
  /* 39 */ vAddConstant("SIZEOF_EbStrand"_Str,    sizeof(EbStrand));

  /* 41 */ vAddConstant("SIZEOF_SCANBUFFER"_Str,   sizeof(ScanBuffer));
  /* 42 */ vAddConstant("SIZEOF_SEHISTORY"_Str,    sizeof(SeHistory));
  /* 43 */ vAddConstant("SIZEOF_SEITF"_Str,        sizeof(SEitf));
  /* 44 */ vAddConstant("SIZEOF_SEITFCURSOR"_Str,  sizeof(SEitfCursor));
  /* 45 */ vAddConstant("SIZEOF_SEITFLIST"_Str,    sizeof(SEitfList));
  /*    */ vAddConstant("SIZEOF_SELCURSOR"_Str,    sizeof(SelCursor));
  /* 46 */ vAddConstant("SIZEOF_SEPROMPT"_Str,     sizeof(SePrompt));
  /* 47 */ vAddConstant("SIZEOF_SINCE"_Str,        sizeof(Since));
  /* 48 */ vAddConstant("SIZEOF_SKIPCURSOR"_Str,   sizeof(SkipCursor));
  /* 49 */ vAddConstant("SIZEOF_SKIPLIST"_Str,     sizeof(SkipList));
  /* 50 */ vAddConstant("SIZEOF_SKIPNODE"_Str,     sizeof(SkipNode));
  /*    */ vAddConstant("SIZEOF_SLOTS"_Str,        sizeof(Slots));
  /* 51 */ vAddConstant("SIZEOF_SMALLHEAP"_Str,    sizeof(Heap));
  /* 52 */ vAddConstant("SIZEOF_SOCKET"_Str,       sizeof(Socket));
  /* 53 */ vAddConstant("SIZEOF_STACK"_Str,        sizeof(Stack));
  /* 54 */ vAddConstant("SIZEOF_STEPACCUM"_Str,    sizeof(StepAccum));
  /* 55 */ vAddConstant("SIZEOF_STEPARRAY"_Str,    sizeof(StepArray));
  /* 56 */ vAddConstant("SIZEOF_SYSCALL"_Str,      sizeof(SysCall));
  /* 57 */ vAddConstant("SIZEOF_TABLE"_Str,        sizeof(Table));
  /* 58 */ vAddConstant("SIZEOF_TASK"_Str,         sizeof(Task));
  /* 69 */ vAddConstant("SIZEOF_TASKCURSOR"_Str,   sizeof(TaskCursor));
  /* 60 */ vAddConstant("SIZEOF_TASKLIST"_Str,     sizeof(TaskList));
  /* 61 */ vAddConstant("SIZEOF_SCHEDULER"_Str,    sizeof(Scheduler));
  /* 61 */ vAddConstant("SIZEOF_TASKSET"_Str,      sizeof(TaskSet));
  /* 62 */ vAddConstant("SIZEOF_THREAD"_Str,       sizeof(Thread));
  /* 63 */ vAddConstant("SIZEOF_THREADCURSOR"_Str, sizeof(ThreadCursor));
  /* 64 */ vAddConstant("SIZEOF_THREADLIST"_Str,   sizeof(ThreadList));
  /* 65 */ vAddConstant("SIZEOF_TIME"_Str,         sizeof(Time));
  /* 66 */ vAddConstant("SIZEOF_TIMER"_Str,        sizeof(Timer));
  /* 67 */ vAddConstant("SIZEOF_TOKENISER"_Str,    sizeof(Tokeniser));
  /* 68 */ vAddConstant("SIZEOF_TX"_Str,           sizeof(Tx));
  /* 69 */ vAddConstant("SIZEOF_TXARRAY"_Str,      sizeof(TxArray));
  /* 70 */ vAddConstant("SIZEOF_TXCOLLECTION"_Str, sizeof(TxCollection));
  /* 71 */ vAddConstant("SIZEOF_TXCURSOR"_Str,     sizeof(TxCursor));
  /* 72 */ vAddConstant("SIZEOF_TXMAP"_Str,        sizeof(TxMap));
  /* 73 */ vAddConstant("SIZEOF_TXREF"_Str,        sizeof(TxRef));
  /* 74 */ vAddConstant("C_SIZEOF_WORD"_Str,         sizeof(Word));

  /*  0 */ vAddConstant("SIZEOF_S32"_Str,          sizeof(s32));
  /*  1 */ vAddConstant("SIZEOF_S256"_Str,         sizeof(s256));
  /*  2 */ vAddConstant("SIZEOF_S16K"_Str,         sizeof(s48K));

  // TK_ : task kind
  /*  0 */ vAddConstant("TK_NONE"_Str,                    TK_NONE);
  /*  0 */ vAddConstant("TK_ANSWER_DRONE"_Str,            TK_ANSWER_DRONE);
  /*  0 */ vAddConstant("TK_LISTER_DRONE"_Str,            TK_LISTER_DRONE);
  /*  0 */ vAddConstant("TK_CONNECTOR_DRONE"_Str,         TK_CONNECTOR_DRONE);
  /*  0 */ vAddConstant("TK_MONITOR_DRONE"_Str,           TK_MONITOR_DRONE);
  /*  0 */ vAddConstant("TK_RECLAIM_DRONE"_Str,           TK_RECLAIM_DRONE);
  /*  0 */ vAddConstant("TK_RECEIVER_DRONE"_Str,          TK_RECEIVER_DRONE );
  /*  0 */ vAddConstant("TK_REQUEST_SERVER"_Str,          TK_REQUEST_SERVER);
  /*  0 */ vAddConstant("TK_LOADSUB_SERVER"_Str,          TK_LOADSUB_SERVER);
  /*  0 */ vAddConstant("TK_COORDINATOR_WORKER"_Str,      TK_COORDINATOR_WORKER);
  /*  0 */ vAddConstant("TK_COORDINATOR_PROXY_WORKER"_Str,TK_COORDINATOR_PROXY_WORKER);
  /*  0 */ vAddConstant("TK_PARTICIPANT_WORKER"_Str,      TK_PARTICIPANT_WORKER);
  /*  0 */ vAddConstant("TK_PARTICIPANT_PROXY_WORKER"_Str,TK_PARTICIPANT_PROXY_WORKER);
  /*  0 */ vAddConstant("TK_SENDER_WORKER"_Str,           TK_SENDER_WORKER);
  /*  0 */ vAddConstant("TK_LOADSUB_PART"_Str,            TK_LOADSUB_PART);
  /*  0 */ vAddConstant("TK_SELECT_PART"_Str,             TK_SELECT_PART);
  /*  0 */ vAddConstant("TK_ENUMS"_Str,                   TK_ENUMS );
  
  // TS_ : task state
  /*  0 */ vAddConstant("TS_NONE"_Str,           TS_NONE);
  /*  1 */ vAddConstant("TS_BLOCKED"_Str,        TS_BLOCKED);
  /*  2 */ vAddConstant("TS_EXITED"_Str,         TS_EXITED);
  /*  3 */ vAddConstant("TS_RUNNING"_Str,        TS_RUNNING);
  /*  4 */ vAddConstant("TS_SLEEPING"_Str,       TS_SLEEPING);
  /*  5 */ vAddConstant("TS_WAITING_ACK"_Str,    TS_WAITING_ACK);
  /*  6 */ vAddConstant("TS_WAITING_SIGNAL"_Str, TS_WAITING_SIGNAL);
  /*  7 */ vAddConstant("TS_YIELDED"_Str,        TS_YIELDED);
  /*  8 */ vAddConstant("TS_ENUMS"_Str,          TS_ENUMS);

  // TP_ : transaction phase
  /*  0 */ vAddConstant("TP_NONE"_Str,        TP_NONE);
  /*  1 */ vAddConstant("TP_EXECUTING"_Str,   TP_EXECUTING);
  /*  2 */ vAddConstant("TP_COMMITTING"_Str,  TP_COMMITTING);
  /*  3 */ vAddConstant("TP_COMMITTED"_Str,   TP_COMMITTED);
  /*  4 */ vAddConstant("TP_PREPARING"_Str,   TP_PREPARING);
  /*  5 */ vAddConstant("TP_PREPARED"_Str,    TP_PREPARED);
  /*  6 */ vAddConstant("TP_ROLLINGBACK"_Str, TP_ROLLINGBACK);
  /*  7 */ vAddConstant("TP_ROLLEDBACK"_Str,  TP_ROLLEDBACK);
  /*  8 */ vAddConstant("TP_FREE"_Str,        TP_FREE);
  /*  9 */ vAddConstant("TP_ENUMS"_Str,       TP_ENUMS);

  // WT_ : word type
  /*  0 */ vAddConstant("WT_NONE"_Str,      WT_NONE);
  /*  0 */ vAddConstant("WT_OBJECT"_Str,    WT_OBJECT);
  /*  0 */ vAddConstant("WT_PRIMITIVE"_Str, WT_PRIMITIVE);
  /*  0 */ vAddConstant("WT_ROUTINE"_Str,   WT_ROUTINE);
  /*  0 */ vAddConstant("WT_VARIABLE"_Str,  WT_VARIABLE);
  /*  0 */ vAddConstant("WT_ENUMS"_Str,     WT_ENUMS);
  
  // OPEN_ : file open argument
  /*  0 */ vAddConstant("OPEN_READ"_Str,              C_OPEN_READ);
  /*  1 */ vAddConstant("OPEN_WRITE"_Str,             C_OPEN_WRITE);
  /*  2 */ vAddConstant("OPEN_APPEND"_Str,            C_OPEN_APPEND);
  /*  3 */ vAddConstant("OPEN_READ_WRITE_EXIST"_Str,  C_OPEN_READ_WRITE_EXIST);
  /*  4 */ vAddConstant("OPEN_READ_WRITE_CREATE"_Str, C_OPEN_READ_WRITE_CREATE);
  /*  5 */ vAddConstant("OPEN_READ_APPEND"_Str,       C_OPEN_READ_APPEND);
  
  // miscellaneous
  /*  0 */ vAddConstant("CLOCK_FREQUENCY"_Str, Synch::iTickFrequency());
  /*  0 */ vAddConstant("CORE_COUNT"_Str,      Hardware::iCoreCount());
  /*  0 */ vAddConstant("CPU_COUNT"_Str,       Hardware::iCpuCount());
  /*  0 */ vAddConstant("PHYSICAL_MEMORY"_Str, Hardware::iPhysicalMemory());
  
  // debug
  vAddConstant("answerDronePrint"_Str,    (i8)0);
  vAddConstant("connectorDronePrint"_Str, (i8)0);
  vAddConstant("listerDronePrint"_Str,    (i8)0);
  vAddConstant("monitorDronePrint"_Str,   (i8)0);
  vAddConstant("reclaimDronePrint"_Str,   (i8)0);
  vAddConstant("receiverDronePrint"_Str,  (i8)0);
  
  vAddConstant("requestServerPrint"_Str,  (i8)0);
  
  vAddConstant("senderWorkerPrint"_Str,   (i8)0);
  
  // c++ objects
  vAddConstant("scheduler"_Str, (i8)gpScheduler);
}

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (gbVerbose)
    vTrace("[[mainThread]] Adding primitives...\n"_Str);
  
  vAddPrimitive("word"_Str,            &vWord,             false);
  gpWord = gpMainDic->pFindWord("word"_Str);

  vAddPrimitives_AccessPath();
  vAddPrimitives_Accum();
  vAddPrimitives_Address();
  vAddPrimitives_Args();
  vAddPrimitives_Assert();
  vAddPrimitives_Astack();
  vAddPrimitives_AttachMap();
  
  vAddPrimitives_Bp();
  
  vAddPrimitives_Cast();
  vAddPrimitives_Class();
  vAddPrimitives_Comment();
  vAddPrimitives_Common();
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
  vAddPrimitives_Entry();
  vAddPrimitives_EntryBuffer();
  vAddPrimitives_Env();
  vAddPrimitives_Error();
  vAddPrimitives_Exit();
  
  vAddPrimitives_File();
  vAddPrimitives_Flow();
  
  vAddPrimitives_Grid();
  vAddPrimitives_GroupBy();
  
  vAddPrimitives_Index();
  vAddPrimitives_Input();
  vAddPrimitives_Int();
  
  vAddPrimitives_Loader();
  vAddPrimitives_LoaderSub();
  vAddPrimitives_Locals();
  vAddPrimitives_Lock();
  vAddPrimitives_Logic();
  
  vAddPrimitives_Magnitude();
  vAddPrimitives_Memory();
  
  vAddPrimitives_Node();
  vAddPrimitives_NodeCursor();
  
  vAddPrimitives_Object();
  vAddPrimitives_OpFrame();
  
  vAddPrimitives_Plan();
  vAddPrimitives_PlanList();
  vAddPrimitives_Planner();
  vAddPrimitives_Process();
  vAddPrimitives_Push();
  
  vAddPrimitives_Queue();
  
  vAddPrimitives_RbCursor();
  vAddPrimitives_Row();
  vAddPrimitives_RowBuffer();
  vAddPrimitives_RowCursor();
  vAddPrimitives_Reg();
  vAddPrimitives_Rstack();
  
  vAddPrimitives_Scheduler();
  vAddPrimitives_SEitf();
  vAddPrimitives_SEitfCursor();
  vAddPrimitives_SEitfList();
  vAddPrimitives_SelCursor();
  vAddPrimitives_Shell();
  vAddPrimitives_Slots();
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
  
  vAddPrimitives_Vector();
  
  vAddPrimitives_Word();
  
  vAddPrimitives_Zero();
}

/*----------------------------------------------------------------------------*/
/* add initial set of axiomatic routines into dictionary (min set to boot)    */
/*----------------------------------------------------------------------------*/
void Task::vAddRoutines(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (gbVerbose)
    vTrace("[[mainThread]] Adding routines...\n"_Str);
  
  s48K sDefinition;

  sDefinition =
  ": __nil ;"_Str;
  
  pCompileColon(sDefinition);
  
  //--------------------------------------------------------------------------
  sDefinition =
  ": docol"
  
  "  retryPoint"
  "  begin_"
  
  // get next input token
  "    input.next"
  
  // test for end of input?
  "    input.type TN_NONE == if"
  "      exit"
  "    then"
  
  // is token an integer?
  "    input.type TN_INTEGER == if"
  
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
  "    input.type TN_STRING == if"
  
  "      input.token"             // put str on stack
  
  "      docol.isInterpreting if" // interpret leaves str on stack
  "        jumpBegin_"
  "      then"
  
  "      docol.isCompiling if"    // compile encodes push StrVal
  "        encodePushValue"
  "        jumpBegin_"
  "      then"
  
  "    then"
  
  // should be a name
  "    input.type TN_NAME != if"  // double check we have a name
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
  
  " ;"_Str;
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": ::task_exit"
  "  TS_EXITED task::this task.state!"
  "  ;"_Str;
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": ::docmd"
  //  "  input.text"
  "  docol"
  "  ::task_exit"
  "  ;"_Str;
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  
  sDefinition =
  ": ::enter"
  "  input.stdin"
  "  docol"
  "  ::task_exit"
  "  ;"_Str;
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": include-file"
  "  input.push"
  "  args.sen str.prepend"
  "  input.open"
  "  docol"
  "  input.close"
  "  input.pop"
  "  ;"_Str;
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": include-string"
  "  input.push"
  //"  dup ."
  "  input.text"
  "  docol"
  "  input.pop"
  "  ;"_Str;
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": include"
//  "  traceon"
  "  args.munged if"
  "    dup find if"
  "      find execute include-string"
  "      exit"
  "    then"
  "    . debug.trap"
  "  then"
  "  include-file"
  "  ;"_Str;
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  // Temporary ifdef till we get sen files embedded in the executable
#ifdef __APPLE__
  sDefinition =
  ": ::dicCompile"
  "  \"::dicCompile\" .verbose"
  "  args.munged if \"compiling munged dictionary...\" . then"
  "  \"compile.sen\" include"
  "  \"::dicCompile exit\" .verbose"
  "  ::task_exit"
  "  ;"_Str;
#else
  sDefinition =
  ": ::dicCompile"
  "  \"::dicCompile\" .verbose"
  "  \"compile.sen\" include"
  "  \"::dicCompile exit\" .verbose"
  "  ::task_exit"
  "  ;"_Str;
#endif
  pCompileColon(sDefinition);
}

/*----------------------------------------------------------------------------*/
/* compile given : routine                                                    */
/*----------------------------------------------------------------------------*/
Word* Task::pCompileColon(StrVal& sDefinition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pNewWord;
  TaskStep  pStep;
  Word* pCalledWord;
  ValWrk xValWrk;
  
  m_pTokeniser->vUseString(sDefinition);
  
  // get :
  m_pTokeniser->vNextToken();
  //vTrace("CompileColon: %s\n"_Str,m_sToken.s());
  
  // get name of word
  m_pTokeniser->vNextToken();
  //vTrace("CompileColon: %s\n"_Str,m_sToken.s());
  
  Dic* pDic;
  if (m_xPathStack.iCount())
    pDic = m_xPathStack.pTop();
  else
    pDic = gpMainDic;

  pNewWord = new(pDic) Word(m_pTokeniser->sToken());
  
  m_pCurrentWord = pNewWord;

  m_pTokeniser->vNextToken();
  
  while (TN_NONE != m_pTokeniser->eToken()) {
    
    //vTrace("CompileColon: %s\n"_Str,m_sToken.s());
    
//  if (StrLib::bEqualTo(";",m_pTokeniser->sToken().s()))
    if (m_pTokeniser->sToken().bEqual(";"_Str))
      break;
    
    switch (m_pTokeniser->eToken()) {
        
      case TN_INTEGER:
        xValWrk = m_pTokeniser->sToken().iToI8();
        pNewWord->vEncode("pushValue"_Str,&xValWrk);
        break;
        
      case TN_NAME:
        pCalledWord = m_xPathStack.pFindWord(m_pTokeniser->sToken());
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
          THROW("Invalid eWordType %lld"_Str,pCalledWord->eWordType());
        }
        break;
        
      case TN_STRING: {
        xValWrk = m_pTokeniser->sToken();
        pNewWord->vEncode("pushValue"_Str,&xValWrk);
        break;
      }
        
      default:
        THROW("Invalid m_eToken %lld"_Str,m_pTokeniser->eToken());
    }
    
    m_pTokeniser->vNextToken();
  }
  
  pNewWord->vEncode("exit"_Str);
  

  // reset scanning
  m_pTokeniser->vUseEnd();
  
  return pNewWord;
}

/*----------------------------------------------------------------------------*/
/* compile a constant definition                                              */
/*----------------------------------------------------------------------------*/
void Task::vCompileConstant(StrVal& sDefinition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word*    pNewWord;
  TaskStep pStep;
  Word*    pCalledWord;
  s48K     sConstantValue;
  
  m_pTokeniser->vUseString(sDefinition);
  
  // get string
  m_pTokeniser->vNextToken();
  //vTrace("CompileConstant: %s\n"_Str,m_pTokeniser->sToken().s());
  sConstantValue = (Str*)m_pTokeniser->sToken().s();
  
  // get CONSTANT
  m_pTokeniser->vNextToken();
  //vTrace("CompileConstant: %s\n"_Str,m_pTokeniser->sToken().s());
  
  // get name of word
  m_pTokeniser->vNextToken();
  //vTrace("CompileConstant: %s\n"_Str,m_pTokeniser->sToken().s());

  Str* aName = (Str*)m_pTokeniser->sToken().s();

  //vTrace("CompileConstant: %s %s\n"_Str,aName,sConstantValue.s());
  
  vAddConstant(aName,(Str*)sConstantValue.s());
  
  m_pTokeniser->vNextToken();
  
  m_pTokeniser->vUseEnd();
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
void Task::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_TASK);
  ASSERT(pObject->eClass() == CN_TASK);
  
  Task* pTask = reinterpret_cast<Task*>(pObject);
  
  vTrace("%s TaskNo %lld state %s txId %lld\n"_Str,
         pTask->m_sName.s(),
         pTask->m_iTaskNo,
         aTS[pTask->m_eState]->s(),
         pTask->m_iTxId);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vContentDump(void)
{
  _poolAddress_(this);
  
  vTrace("%sTask %02lld %p: %16s state %8s txId %lld\n"_Str,
         aDoubleTab(),
         m_iTaskNo,
         this,
         m_sName.s(),
         aTS[m_eState]->s(),
         m_iTxId);
}

/*----------------------------------------------------------------------------*/
/* set exception                                                              */
/*----------------------------------------------------------------------------*/
void Task::vException(StrVal& sFile, StrVal& eException)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_sFile = sFile;
  m_sException = eException;
}

/*----------------------------------------------------------------------------*/
/* is given name?                                                             */
/*----------------------------------------------------------------------------*/
bool Task::bIsName(StrVal& sName)
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
/* get name of this task                                                      */
/*----------------------------------------------------------------------------*/
StrVal& Task::sName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* set name of this task                                                      */
/*----------------------------------------------------------------------------*/
void Task::vName(StrVal& sName)
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
/* pop int                                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopInt(i8& iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! m_pDataStack->bPop(iInt) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop ptr to object                                                          */
/*----------------------------------------------------------------------------*/
bool Task::bPopObject(Object*& pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! m_pDataStack->bPop(pObject) )
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop str                                                                    */
/*----------------------------------------------------------------------------*/
bool Task::bPopStr(s48K& sStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! m_pDataStack->bPop(sStr) )
    return false;
  
  return true;
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
void Task::vPrimitive(Str* aPrimitive)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if ( ! (m_iTracing & C_TRACE_STEPS) )
    return;
  
  s256 sRoutine;
  
  Cell* pCell = IP.m_pCell;
  Word* pLocatedWord = m_xPathStack.pLocate(pCell);
  if (pLocatedWord)
    sRoutine = pLocatedWord->sName();
  else
    sRoutine = "<unknown>"_Str;
  
  i8 iDataValues  = m_pDataStack->iValues();
  i8 iReturnValues = m_pReturnStack->iValues();
  vTrace("%-15s %15s : r%lld ; d%2d="_Str,
         sRoutine.s(),
         aPrimitive,
         iReturnValues,
         iDataValues);
  
  s48K sStackContents;
  vStackContents(m_pDataStack, sStackContents);
  vTrace("%s\n"_Str,sStackContents.s());
}

/*----------------------------------------------------------------------------*/
/* push int                                                                   */
/*----------------------------------------------------------------------------*/
void Task::vPushInt(i8 iInt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pDataStack->vPushInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* push object                                                                */
/*----------------------------------------------------------------------------*/
void Task::vPushObject(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pDataStack->vPushObject(pObject);
}

/*----------------------------------------------------------------------------*/
/* push str                                                                   */
/*----------------------------------------------------------------------------*/
void Task::vPushStr(Str* aStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  s48K sStr = aStr;
  
  m_pDataStack->vPushS48K(sStr);
}

/*----------------------------------------------------------------------------*/
/* push str                                                                   */
/*----------------------------------------------------------------------------*/
void Task::vPushStr(s48K& sStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_pDataStack->vPushS48K(sStr);
}

/*----------------------------------------------------------------------------*/
/* get the request of the task                                                */
/*----------------------------------------------------------------------------*/
Value* Task::pRequest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_pRequest;
}

/*----------------------------------------------------------------------------*/
/* reset task                                                                 */
/*----------------------------------------------------------------------------*/
void Task::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  IP.m_pCell    = m_pResetIP;
  m_pBeginIP = nullptr;
  
  m_pTokeniser->sToken() = ""_Str;
  
  m_pDataStack->vReset();
  m_pReturnStack->vReset();
}

/*----------------------------------------------------------------------------*/
/* rertry after exception                                                     */
/*----------------------------------------------------------------------------*/
void Task::vRetry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  IP.m_pCell    = m_pResetIP;
  m_pBeginIP = nullptr;
  
  m_pTokeniser->sToken() = ""_Str;
  
  m_pDataStack->vRetry();
  m_pReturnStack->vRetry();
}

/*----------------------------------------------------------------------------*/
/* get root thread from root tasks data stack                                 */
/*----------------------------------------------------------------------------*/
Thread* Task::pRootThread(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Object* pThreadEmb;
  
  if ( ! m_pDataStack->bPop(pThreadEmb) ) {
    m_sException.vSprint("Task::pRootThread: invalid data stack"_Str);
    m_sFile = (Str*)__FILE__;
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
  
  if (gbVerbose)
    vTrace("[[mainThread]] Seeding Dic Compile...\n"_Str);
  
  Word* pBootSen = gpMainDic->pFindWord("::dicCompile"_Str);
  ASSERT(pBootSen);
  
  IP.m_pCell = pBootSen->pFirstCell();
  
  m_eState = TS_RUNNING;
  
  m_pDataStack->vReset();
  m_pReturnStack->vReset();
  
  m_sLambda = ""_Str;
  
  m_pTokeniser->vUseString(m_sLambda);
}

/*----------------------------------------------------------------------------*/
/* seed task to run given lambda                                              */
/*----------------------------------------------------------------------------*/
void Task::vSeedWord(Str* aLambda)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pShellEnter = gpMainDic->pFindWord("::docmd"_Str);
  
  IP.m_pCell = pShellEnter->pFirstCell();
  
  m_eState = TS_YIELDED;
  
  m_pDataStack->vReset();
  m_pReturnStack->vReset();
  
  m_sLambda = aLambda;
  if (m_sLambda.iLength())
    m_sLambda.vConcat(" ::task_exit"_Str);
  
  m_pTokeniser->vUseString(m_sLambda);
}

/*----------------------------------------------------------------------------*/
/* seed task to run ::enter                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSeedDocol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pShellEnter = gpMainDic->pFindWord("::enter"_Str);
  
  IP.m_pCell = pShellEnter->pFirstCell();
  
  m_eState = TS_YIELDED;
  
  m_pDataStack->vReset();
  m_pReturnStack->vReset();
  
  m_sLambda = ""_Str;
  
  s32 sPrompt("docol> "_Str);
  m_pTokeniser->vPrompt(sPrompt);
  
  m_pTokeniser->vUseString(m_sLambda);
  m_pTokeniser->vLineMode(false);
}

/*----------------------------------------------------------------------------*/
/* stack contents as string                                                   */
/*----------------------------------------------------------------------------*/
void Task::vStackContents(Stack* pStack, StrVal& sStackContents)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  sStackContents = ""_Str;
  
  i8 iParamValues = pStack->iValues();
  
  sStackContents.vCatSprint("("_Str);
  for (i8 iValue = iParamValues - 1; iValue >= 0; iValue--) {
    
    sStackContents.vCatSprint(" "_Str);
    
    Value* pValue = pStack->pPeekValue(iValue);
    
    eCN eClass = pValue->eClass();
    
    if (CN_BINVAL == eClass) {
      Object* pObject = _pBinVal(pValue)->pNewObject();
      sStackContents.vCatSprint("(BINVAL)^%s=%p"_Str,
                                aCN[pObject->eClass()],
                                pObject);
    }
    else if (CN_INTVAL == eClass) {
      sStackContents.vCatSprint("%lld"_Str,_pIntVal(pValue)->iInt());
    }
    else if (CN_PTRVAL == eClass) {
      if (_pPtrVal(pValue)->pPtr()) {
        
        Object* pPtr = _pPtrVal(pValue)->pPtr();
        
        eCN ePtrClass = _pPtrVal(pValue)->pPtr()->eClass();
        
        if (CN_WORD == ePtrClass)
          sStackContents.vCatSprint("^%s=%p:%s"_Str,
                                    aCN[ePtrClass],
                                    pPtr,
                                    ((Word*)pPtr)->sName().s());
        else if (CN_QUEUE == ePtrClass) {
          Word* pWord = Word::pToWord(pPtr);
          sStackContents.vCatSprint("^%s=%p:%s"_Str,
                                    aCN[ePtrClass],
                                    pPtr,
                                    pWord->sName().s());
        }
        else if (CN_TASK == ePtrClass) {
          Task* pTask = _pTask(pPtr);
          sStackContents.vCatSprint("^%s=%p:%s"_Str,
                                    aCN[ePtrClass],
                                    pPtr,
                                    pTask->sName().s());
        }
        else {
          if (pPtr->iMagic() == MAGIC && pPtr->eClass() < CN_ENUMS)
            sStackContents.vCatSprint("^%s=%p"_Str,
                                      aCN[ePtrClass],
                                      pPtr);
          //else
          //  sStackContents.vCatSprint("%p"_Str,
          //                     pValWrk->pPtr());
        }
      }
      else
        sStackContents.vCatSprint("^%s=%p"_Str,
                                  pValue->aClass(),
                                  _pPtrVal(pValue)->pPtr());
    }
    else if (CN_IPVAL == eClass) {                                      
      sStackContents.vCatSprint("%p"_Str,
                                _pIpVal(pValue)->pIpPtr());
    }
    else if (CN_STRVAL == eClass) {
      sStackContents.vCatSprint("\"%lls\""_Str,
                                _pStrVal(pValue)->s());
    }
    else if (CN_VALSET == eClass) {
      sStackContents.vCatSprint("<%s>=%p"_Str,
                                pValue->aClass(),
                                pValue);
    }
    else {
      sStackContents.vCatSprint("<unknown:class=%lld>"_Str,
                                pValue->eClass());
    }
  }
  
  sStackContents.vCatSprint(" )"_Str);
}

/*----------------------------------------------------------------------------*/
/* get task state                                                             */
/*----------------------------------------------------------------------------*/
eTS Task::eState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_eState;
}

/*----------------------------------------------------------------------------*/
/* set task state                                                             */
/*----------------------------------------------------------------------------*/
void Task::vState(eTS eState)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_xLock.vGrabWrite();
  m_eState = eState;
  m_xLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* get task kind                                                              */
/*----------------------------------------------------------------------------*/
eTK Task::eTaskKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  return m_eTaskKind;
}

/*----------------------------------------------------------------------------*/
/* set task kind                                                              */
/*----------------------------------------------------------------------------*/
void Task::vTaskKind(eTK eTaskKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  m_eTaskKind = eTaskKind;
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
/* trace current point                                                        */
/*----------------------------------------------------------------------------*/
void Task::vTracePoint(Str* aMark)
{
  if (gxArguments.m_nNodes > 1) {
    i8 iPause = Random::iRandom(999,99999);
    PosixThread::vSleepMicro(iPause);
  }
  
  s256 sRoutine;
  
  Cell* pCell = IP.m_pCell;
  
  Word* pLocatedWord = m_xPathStack.pLocate(pCell);
  if (pLocatedWord)
    sRoutine = pLocatedWord->sName();
  else
    sRoutine = "<unknown>"_Str;
  
  i8 iAuxValues    = m_pAuxStack->iValues();
  i8 iDataValues   = m_pDataStack->iValues();
  i8 iReturnValues = m_pReturnStack->iValues();
  vTrace("[%s(N%d):%8s\\%lld %-11s] %-16s %-16s : a%2lld="_Str,
         
         gpGrid->pThisNode()->aNodeName(),
         gpGrid->iThisNode(),
         m_sName.s(),
         *m_zAcks,
         m_sComment.s(),
         
         sRoutine.s(),
         aMark,
         iAuxValues);
         
  s48K sStackContents;
  vStackContents(m_pAuxStack, sStackContents);
  
  vTrace("%s"_Str,sStackContents.s());
  
  vTrace(" r%2lld  d%2lld="_Str, iReturnValues, iDataValues);

  vStackContents(m_pDataStack, sStackContents);
  
  vTrace("%s\n"_Str,sStackContents.s());
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* task loop                                                                  */
/*----------------------------------------------------------------------------*/
void Task::vLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  while (TS_RUNNING == m_eState || TS_WAITING_SIGNAL == m_eState) {
    
    //assert(0 == m_iTaskNo || m_pThread);
    
    TaskStep pTaskStep = *IP.m_pCell->m_ppEntryStep;
    
    m_pEntryCell = IP.m_pCell->m_ppEntryStep + 1;
    
    //vTrace("pTaskStep %p\n"_Str,pTaskStep);
    
    IP.m_pCell++;
    
    try {
      (pTaskStep)(this);
    }
    
    catch (i8 iException) {
      char wFileName[256];
      
      const char* ps;
      
      ps = StrLib::sLast(m_sFile.s());
      StrLib::bBackward(&ps,m_sFile.s(),'/');
      bool bBackSlash = StrLib::bBackward(&ps,m_sFile.s(),'/');
      if (bBackSlash) {
        StrLib::vCopy(wFileName,ps+1);
      }
      else {
        StrLib::vCopy(wFileName, m_sFile.s());
      }
      vError("[%s] %s : line %lld in %s\n"_Str,
             m_sName.s(),
             m_sException.s(),
             iException,
             wFileName);
      vRetry();
    }
    
    catch (Error* pError) {
      char wFileName[256];
      
      const char* ps;
      
      ps = StrLib::sLast(pError->aFileName()->s());
      StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
      StrLib::vCopy(wFileName,ps+1);
      vError("[%s] %s: line %lld in %s\n"_Str,
             m_sName.s(),
             pError->aErrorMsg(),
             pError->iLineNo(),
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
Footer* Task::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
