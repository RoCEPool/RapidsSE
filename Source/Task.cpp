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
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern ServerScheduler*      gpServerScheduler;
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
  
  vInit(""_Stz);
    
  _checkInitialised_(CN_TASK);
  
  Task* pTask = this;
  //  gpTaskList->vAppend(pTask);
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
  
  vInit(""_Stz);
  
  _checkInitialised_(CN_TASK);
  
  Task* pTask = this;
  //  gpTaskList->vAppend(pTask);
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
  
  vInit(""_Stz);
  
  m_pRequest = new ValSet;
  
  // copy the request
  i8 nBytes = ValLib::iSize(pRequest);
  MemLib::vCopy(m_pRequest,pRequest,nBytes,CN_TASK);
  
  m_sName    = "req.task"_Stz;
  m_sComment = "requestLoop"_Stz;
  m_eTaskKind = TK_REQUEST_SERVER;
  m_eActorModel = AC_SERVER;
  
  _checkInitialised_(CN_TASK);
  
  Task* pTask = this;
  //  gpTaskList->vAppend(pTask);
  //  gpTaskList->vDump();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Task::Task(Stz* aLambdaInput)
: Object(CN_TASK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  ASSERT(gpTaskList);

  _class_(CN_TASK);
  
  vInit(aLambdaInput);
  
  _checkInitialised_(CN_TASK);
  
  Task* pTask = this;
  //  gpTaskList->vAppend(pTask);
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
  
  //  Task* pTask = this;
  //  gpTaskList->vRemove(pTask);
//  gpTaskList->vDump();
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
void Task::vInit(Stz* aLambda)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  static i8 iTaskNo = 0;
  
  // basic state
  m_xLock.vInitLock(LK_TASK);
  
  m_iTaskNo     = iTaskNo++;
  m_eTaskKind   = TK_NONE;
  m_eActorModel = AC_NONE;
  m_iPartition  = -1;
  m_sName       = "<no task name>"_Stz;
  m_eState      = TS_NONE;
  m_iTxId       = -1;
  m_npNextInSet = nullptr;
  m_npPrevInSet = nullptr;
  m_iWakeAt     = 0;
  m_pParentTask = nullptr;
  m_zAcks.vCheckInit();
  m_pThread     = nullptr;
  m_pTaskSet    = nullptr;
  
  m_sComment    = ""_Stz;
  
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
    m_sLambda.vConcat(" exit"_Stz);
  
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
void Task::vAddConstant(Stz* aName, i8 iVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);

  Word* pNewWord = new(gpMainDic) Word(aName);
  
  IntVal xIntVal;
  xIntVal = iVal;
  
  pNewWord->vEncode("pushValue"_Stz,&xIntVal);
  pNewWord->vEncode("exit"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add constant to main dictionary                                            */
/*----------------------------------------------------------------------------*/
void Task::vAddConstant(Stz* aName, Stz* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pNewWord = new(gpMainDic) Word(aName);
  
  ValWrk xWspStr;
  xWspStr = aValue;
  
  pNewWord->vEncode("pushValue"_Stz,&xWspStr);
  pNewWord->vEncode("exit"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add primitive word into main dictionary                                    */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitive(Stz* aName, TaskStep pStep, bool bImmediate)
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
    vTrace("[[mainThread]] Adding constants...\n"_Stz);
  
#ifdef _DEBUG
  vAddConstant("_DEBUG"_Stz,1);
#else
  vAddConstant("_DEBUG"_Stz,(i8)0);
#endif
  
  vAddConstant("version"_Stz,          (Stz*)RAPIDSSE_VERSION_STRING);
  
  vAddConstant("jumbobuffersize"_Stz,  C_SIZE_JUMBO_OBJECT);
  vAddConstant("largebuffersize"_Stz,  C_SIZE_POOL_BUFFER);
  vAddConstant("mediumbuffersize"_Stz, C_SIZE_MEDIUM_OBJECT);
  vAddConstant("smallbuffersize"_Stz,  C_SIZE_SMALL_OBJECT);
  vAddConstant("magic"_Stz,            C_MAGIC);
  
  // Arguments
  vAddConstant("args.bgtasks"_Stz,           gxArguments.m_bBgTasks);
  vAddConstant("args.command"_Stz,           gxArguments.m_sCommand);
  vAddConstant("args.commands"_Stz,          gxArguments.m_sCommands);
  vAddConstant("args.config"_Stz,            gxArguments.m_bConfig);
  vAddConstant("args.docol"_Stz,             gxArguments.m_bDocol);
  vAddConstant("args.dontFree"_Stz,          gxArguments.m_bDontFree);
  vAddConstant("args.folder"_Stz,            gxArguments.m_sFolder);
  vAddConstant("args.freeRB"_Stz,            gxArguments.m_bFreeRB);
  vAddConstant("args.frequency"_Stz,         gxArguments.m_iFrequency);
  vAddConstant("args.help"_Stz,              gxArguments.m_bHelp);
  vAddConstant("args.hugePages"_Stz,         gxArguments.m_bHugePages);
  vAddConstant("args.jumboBuffers"_Stz,      gxArguments.m_nJumboBuffers);
  vAddConstant("args.jumboSize"_Stz,         gxArguments.m_iJumboSize);
  vAddConstant("args.jumboTook"_Stz,         gxArguments.m_nJumboTook);
  vAddConstant("args.killPrev"_Stz,          gxArguments.m_bKillPrev);
  vAddConstant("args.levels"_Stz,            gxArguments.m_iLevels);
  vAddConstant("args.lock"_Stz,              gxArguments.m_bLock);
  vAddConstant("args.metadata"_Stz,          gxArguments.m_bMetadata);
  vAddConstant("args.munged"_Stz,            gxArguments.m_bMunged);
  vAddConstant("args.nodeName"_Stz,          gxArguments.m_sNodeName);
  vAddConstant("args.nodes"_Stz,             gxArguments.m_nNodes);
  vAddConstant("args.partitionsPerNode"_Stz, gxArguments.m_nSlots);
  vAddConstant("args.progress"_Stz,          gxArguments.m_bProgress);
  vAddConstant("args.prompt"_Stz,            gxArguments.m_bPrompt);
  vAddConstant("args.regionPrefix"_Stz,      gxArguments.m_sRegionPrefix);
  vAddConstant("args.regionName"_Stz,        gxArguments.m_sRegionName);
  vAddConstant("args.regionSize"_Stz,        gxArguments.m_iRegionSize
                                               - gxArguments.m_iJumboSize);
  vAddConstant("args.restart"_Stz,           gxArguments.m_bRestart);
  vAddConstant("args.sen"_Stz,               gxArguments.m_sSen);
  vAddConstant("args.sleep"_Stz,             gxArguments.m_iSleep);
  vAddConstant("args.slots"_Stz,             gxArguments.m_nSlots);
  vAddConstant("args.start"_Stz,             gxArguments.m_bStart);
  vAddConstant("args.status"_Stz,            gxArguments.m_bStatus);
  vAddConstant("args.stop"_Stz,              gxArguments.m_bStop);
  vAddConstant("args.touch"_Stz,             gxArguments.m_bTouch);
  vAddConstant("args.tracepoint"_Stz,        gxArguments.m_bTracepoint);
  vAddConstant("args.tracing"_Stz,           gxArguments.m_bTracing);
  vAddConstant("args.unlink"_Stz,            gxArguments.m_bUnlink);
  vAddConstant("args.verbose"_Stz,           gxArguments.m_bVerbose);
  vAddConstant("args.welcome"_Stz,           gxArguments.m_bWelcome);
  
  // true / flase
  vAddConstant("true"_Stz,    (i8)1);
  vAddConstant("false"_Stz,   (i8)0);

  // useful numbers
  vAddConstant("C_1KB"_Stz,   C_1KB);
  vAddConstant("C_1MB"_Stz,   C_1MB);
  vAddConstant("C_1GB"_Stz,   C_1GB);
  vAddConstant("C_1TB"_Stz,   C_1TB);
  vAddConstant("C_1PB"_Stz,   C_1PB);
  vAddConstant("C_1EB"_Stz,   C_1EB);
  
  vAddConstant("C_1K"_Stz,    C_1K);
  vAddConstant("C_1M"_Stz,    C_1M);
  vAddConstant("C_1B"_Stz,    C_1B);
  
  vAddConstant("C_4KB"_Stz,   C_4KB);
  vAddConstant("C_16KB"_Stz,  C_16KB);
  vAddConstant("C_64KB"_Stz,  C_64KB);
  vAddConstant("C_256KB"_Stz, C_256KB);
  
  vAddConstant("C_INDEXES_PER_TABLE"_Stz, C_INDEXES_PER_TABLE);
  
  // AC_ : actor models
  /*  0 */ vAddConstant("AC_NONE"_Stz,         AC_NONE);
  /*  1 */ vAddConstant("AC_DRONE"_Stz,        AC_DRONE);
  /*  2 */ vAddConstant("AC_PARTITION"_Stz,    AC_PARTITION);
  /*  3 */ vAddConstant("AC_SERVER"_Stz,       AC_SERVER);
  /*  4 */ vAddConstant("AC_WORKER"_Stz,       AC_WORKER);
  /*  5 */ vAddConstant("AC_ENUMS"_Stz,        AC_ENUMS);

  // AM_ : attach map numbers
  /*  0 */ vAddConstant("AM_NONE"_Stz,         AM_NONE);
  /*  1 */ vAddConstant("AM_INDEXENTRIES"_Stz, AM_INDEXENTRIES);
  /*  2 */ vAddConstant("AM_INDEXLEVELS"_Stz,  AM_INDEXLEVELS);
  /*  3 */ vAddConstant("AM_INDEXSPACE"_Stz,   AM_INDEXSPACE);
  /*  4 */ vAddConstant("AM_ENUMS"_Stz,        AM_ENUMS);
  
  // CN_ : class numbers
  /*  0 */ vAddConstant("CN_NONE"_Stz,         CN_NONE);

  /*    */ vAddConstant("CN_BINVAL"_Stz,       CN_BINVAL);
  /*  4 */ vAddConstant("CN_INTVAL"_Stz,       CN_INTVAL);
  /*  5 */ vAddConstant("CN_IPVAL"_Stz,        CN_IPVAL);
  /*  6 */ vAddConstant("CN_OBJVAL"_Stz,       CN_OBJVAL);
  /*  7 */ vAddConstant("CN_PTRVAL"_Stz,       CN_PTRVAL);
  /*  1 */ vAddConstant("CN_STRVAR"_Stz,       CN_STRVAR);
  /*  8 */ vAddConstant("CN_STRVAL"_Stz,       CN_STRVAL);
  /*  9 */ vAddConstant("CN_VALREG"_Stz,       CN_VALREG);
  /* 10 */ vAddConstant("CN_VALSET"_Stz,       CN_VALSET);
  /* 11 */ vAddConstant("CN_VALWRK"_Stz,       CN_VALWRK);
  
  /* 12 */ vAddConstant("CN_ARGUMENTS"_Stz,    CN_ARGUMENTS);
  /* 13 */ vAddConstant("CN_ATOMIC"_Stz,       CN_ATOMIC);
  /* 14 */ vAddConstant("CN_ATTACHMAP"_Stz,    CN_ATTACHMAP);
  
  /* 16 */ vAddConstant("CN_BUFFERPOOL"_Stz,   CN_BUFFERPOOL);
  /* 17 */ vAddConstant("CN_BPA"_Stz,          CN_BPA);
  
  /* 18 */ vAddConstant("CN_DIC"_Stz,          CN_DIC);
  /* 19 */ vAddConstant("CN_DICPATH"_Stz,      CN_DICPATH);
  /* 20 */ vAddConstant("CN_DIR"_Stz,          CN_DIR);
  
  /* 21 */ vAddConstant("CN_FILE"_Stz,         CN_FILE);
  /* 15 */ vAddConstant("CN_FREEBUFFER"_Stz,   CN_FREEBUFFER);
  /* 22 */ vAddConstant("CN_FREECHUNK"_Stz,    CN_FREECHUNK);
  
  /* 23 */ vAddConstant("CN_GRAPH"_Stz,        CN_GRAPH);
  /* 24 */ vAddConstant("CN_GRAPHCURSOR"_Stz,  CN_GRAPHCURSOR);
  /* 25 */ vAddConstant("CN_GRAPHNODE"_Stz,    CN_GRAPHNODE);
  /* 26 */ vAddConstant("CN_GRID"_Stz,         CN_GRID);
  
  /* 27 */ vAddConstant("CN_HEAPBUFFER"_Stz,   CN_HEAPBUFFER);
  
  /* 28 */ vAddConstant("CN_INDEX"_Stz,        CN_INDEX);
  /*    */ //vAddConstant("CN_INDEXDEFN"_Stz,    CN_INDEXDEFN);

  /* 29 */ vAddConstant("CN_JUMBOHEAP"_Stz,    CN_JUMBOHEAP);

  /* 31 */ vAddConstant("CN_KEYWRK"_Stz,       CN_KEYWRK);
  
  /* 32 */ vAddConstant("CN_LOADCHUNK"_Stz,    CN_LOADCHUNK);
  /* 33 */ vAddConstant("CN_LOADER"_Stz,       CN_LOADER);
  /* 34 */ vAddConstant("CN_LOADERSUB"_Stz,    CN_LOADERSUB);
  /* 35 */ vAddConstant("CN_LOADMAP"_Stz,      CN_LOADMAP);
  /* 36 */ vAddConstant("CN_LOCK"_Stz,         CN_LOCK);
  
  /* 37 */ vAddConstant("CN_MEDIUMHEAP"_Stz,   CN_MEDIUMHEAP);
  
  /* 38 */ vAddConstant("CN_NODE"_Stz,         CN_NODE);
  /* 39 */ vAddConstant("CN_NODECURSOR"_Stz,   CN_NODECURSOR);
  
  /* 40 */ vAddConstant("CN_PARSER"_Stz,       CN_PARSER);
  /* 41 */ vAddConstant("CN_PARTITION"_Stz,    CN_PARTITION);
  /*    */ vAddConstant("CN_PLAN"_Stz,         CN_PLAN);
  /*    */ vAddConstant("CN_PLANLIST"_Stz,     CN_PLANLIST);
  /* 43 */ vAddConstant("CN_PROXY"_Stz,        CN_PROXY);
  /* 44 */ vAddConstant("CN_PROXYSET"_Stz,     CN_PROXYSET);
  
  /* 45 */ vAddConstant("CN_QUEUE"_Stz,        CN_QUEUE);
  /* 46 */ vAddConstant("CN_QUEUEBUFFER"_Stz,  CN_QUEUEBUFFER);
  
  /*    */ vAddConstant("CN_RBCURSOR"_Stz,     CN_RBCURSOR);
  /* 47 */ vAddConstant("CN_REGFRAME"_Stz,     CN_REGFRAME);
  /* 48 */ vAddConstant("CN_RING"_Stz,         CN_RING);
  /* 49 */ vAddConstant("CN_ROW"_Stz,          CN_ROW);
  /* 50 */ vAddConstant("CN_ROWBUFFER"_Stz,    CN_ROWBUFFER);
  /* 51 */ vAddConstant("CN_ROWCURSOR"_Stz,    CN_ROWCURSOR);
  
  /* 54 */ vAddConstant("CN_SCANBUFFER"_Stz,   CN_SCANBUFFER);
  /* 55 */ vAddConstant("CN_SCRATCH"_Stz,      CN_SCRATCH);
  /* 56 */ vAddConstant("CN_SEHISTORY"_Stz,    CN_SEHISTORY);
  /* 57 */ vAddConstant("CN_SEITF"_Stz,        CN_SEITF);
  /* 58 */ vAddConstant("CN_SEITFCURSOR"_Stz,  CN_SEITFCURSOR);
  /* 59 */ vAddConstant("CN_SEITFLIST"_Stz,    CN_SEITFLIST);
  /*    */ vAddConstant("CN_SELCURSOR"_Stz,    CN_SELCURSOR);
  /* 60 */ vAddConstant("CN_SEMAPHORE"_Stz,    CN_SEMAPHORE);
  /* 60 */ vAddConstant("CN_SEPROMPT"_Stz,     CN_SEPROMPT);
  /* 61 */ vAddConstant("CN_SINCE"_Stz,        CN_SINCE);
  /* 62 */ vAddConstant("CN_SKIPCURSOR"_Stz,   CN_SKIPCURSOR);
  /* 63 */ vAddConstant("CN_SKIPLIST"_Stz,     CN_SKIPLIST);
  /* 64 */ vAddConstant("CN_SKIPNODE"_Stz,     CN_SKIPNODE);
  /*    */ vAddConstant("CN_SLOTS"_Stz,        CN_SLOTS);
  /* 65 */ vAddConstant("CN_SMALLHEAP"_Stz,    CN_SMALLHEAP);
  /* 66 */ vAddConstant("CN_SOCKET"_Stz,       CN_SOCKET);
  /* 67 */ vAddConstant("CN_STACK"_Stz,        CN_STACK);
  /* 68 */ vAddConstant("CN_STEPACCUM"_Stz,    CN_STEPACCUM);
  /* 69 */ vAddConstant("CN_STEPARRAY"_Stz,    CN_STEPARRAY);
  /* 70 */ vAddConstant("CN_SYSCALL"_Stz,      CN_SYSCALL);
  
  /* 71 */ vAddConstant("CN_TABLE"_Stz,        CN_TABLE);
  /* 72 */ vAddConstant("CN_TASK"_Stz,         CN_TASK);
  /* 73 */ vAddConstant("CN_TASKCURSOR"_Stz,   CN_TASKCURSOR);
  /* 74 */ vAddConstant("CN_TASKLIST"_Stz,     CN_TASKLIST);
  /*    */ vAddConstant("CN_TASKQUEUE"_Stz,    CN_TASKQUEUE);
  /* 75 */ vAddConstant("CN_SERVERSCHEDULER"_Stz,    CN_SERVERSCHEDULER);
  /* 75 */ vAddConstant("CN_TASKSET"_Stz,      CN_TASKSET);
  /* 76 */ vAddConstant("CN_THREAD"_Stz,       CN_THREAD);
  /* 77 */ vAddConstant("CN_THREADCURSOR"_Stz, CN_THREADCURSOR);
  /* 78 */ vAddConstant("CN_THREADLIST"_Stz,   CN_THREADLIST);
  /*    */ vAddConstant("CN_THREADQUEUE"_Stz,  CN_THREADQUEUE);
  /* 79 */ vAddConstant("CN_TIME"_Stz,         CN_TIME);
  /* 80 */ vAddConstant("CN_TIMER"_Stz,        CN_TIMER);
  /* 81 */ vAddConstant("CN_TOKENISER"_Stz,    CN_TOKENISER);
  /* 82 */ vAddConstant("CN_TX"_Stz,           CN_TX);
  /* 87 */ vAddConstant("CN_TXSET"_Stz,        CN_TXSET);
  
  /* 88 */ vAddConstant("CN_WORD"_Stz,         CN_WORD);

  /* 89 */ vAddConstant("CN_ZSMALL_"_Stz,      CN_ZSMALL_);
  /* 90 */ vAddConstant("CN_ZMEDIUM_"_Stz,     CN_ZMEDIUM_);
  /* 91 */ vAddConstant("CN_ZLARGE_"_Stz,      CN_ZLARGE_);
  /* 92 */ vAddConstant("CN_ZJUMBO_"_Stz,      CN_ZJUMBO_);
  
  /* 93 */ vAddConstant("CN_ENUMS"_Stz,        CN_ENUMS);
  
  // DE_ : directory entry type -- n.b. the DT_FIFO etc are from dirent.h
  /*  1 */ vAddConstant("DE_FIFO"_Stz, DT_FIFO);
  /*  2 */ vAddConstant("DE_CHR"_Stz,  DT_CHR);
  /*  3 */ vAddConstant("DE_DIR"_Stz,  DT_DIR);
  /*  6 */ vAddConstant("DE_BLK"_Stz,  DT_BLK);
  /*  8 */ vAddConstant("DE_REG"_Stz,  DT_REG);
  /* 10 */ vAddConstant("DE_LNK"_Stz,  DT_LNK);
  /* 12 */ vAddConstant("DE_SOCK"_Stz, DT_SOCK);

  // DM_ : datum type
  /*  1 */ vAddConstant("DM_BCD"_Stz, DM_BCD);
  /*  2 */ vAddConstant("DM_BOL"_Stz, DM_BOL);
  /*  3 */ vAddConstant("DM_DTE"_Stz, DM_DTE);
  /*  4 */ vAddConstant("DM_FLT"_Stz, DM_FLT);
  /*  5 */ vAddConstant("DM_INT"_Stz, DM_INT);
  /*  6 */ vAddConstant("DM_IVL"_Stz, DM_IVL);
  /*  7 */ vAddConstant("DM_TME"_Stz, DM_TME);
  /*  8 */ vAddConstant("DM_TSP"_Stz, DM_TSP);
  
  // EC_ : error codes
  /*  0 */ vAddConstant("EC_NONE"_Stz,               EC_NONE);
  /*  1 */ vAddConstant("EC_ASSERT"_Stz,             EC_ASSERT);
  /*  2 */ vAddConstant("EC_BEGIN_TX_FAILED"_Stz,    EC_BEGIN_TX_FAILED);
  /*  3 */ vAddConstant("EC_CATALOG_EXISTS"_Stz,     EC_CATALOG_EXISTS);
  /*  4 */ vAddConstant("EC_CLUSTER_NOT_STARTED"_Stz,EC_CLUSTER_NOT_STARTED);
  /*  5 */ vAddConstant("EC_CLUSTER_PARTITIONS_MISMATCH"_Stz,EC_CLUSTER_PARTITIONS_MISMATCH);
  /*  6 */ vAddConstant("EC_CLUSTER_SIZE_MISMATCH"_Stz,      EC_CLUSTER_SIZE_MISMATCH);
  /*  7 */ vAddConstant("EC_DUPLICATE_COLUMN"_Stz,   EC_DUPLICATE_COLUMN);
  /*  8 */ vAddConstant("EC_DUPLICATE_KEY"_Stz,      EC_DUPLICATE_KEY);
  /*  9 */ vAddConstant("EC_FOLDER_IN_USE"_Stz,      EC_FOLDER_IN_USE);
  /* 10 */ vAddConstant("EC_FOLDER_NOT_EMPTY"_Stz,   EC_FOLDER_NOT_EMPTY);
  /* 11 */ vAddConstant("EC_INDEX_EXISTS"_Stz,       EC_INDEX_EXISTS);
  /* 12 */ vAddConstant("EC_INVALID_CLASS"_Stz,      EC_INVALID_CLASS);
  /* 13 */ vAddConstant("EC_INVALID_COLUMN"_Stz,     EC_INVALID_COLUMN);
  /* 14 */ vAddConstant("EC_INVALID_COLUMNS_UNIQUE_INDEX"_Stz,EC_INVALID_COLUMNS_UNIQUE_INDEX);
  /* 15 */ vAddConstant("EC_INVALID_COMMAND"_Stz,    EC_INVALID_COMMAND);
  
  /* 16 */ vAddConstant("EC_INVALID_FILE"_Stz,       EC_INVALID_FILE);
  /* 17 */ vAddConstant("EC_INVALID_INTERFACE"_Stz,  EC_INVALID_INTERFACE);
  /* 18 */ vAddConstant("EC_INVALID_PARAMETER"_Stz,  EC_INVALID_PARAMETER);
  /* 19 */ vAddConstant("EC_INVALID_PARAMETERS"_Stz, EC_INVALID_PARAMETERS);
  /* 20 */ vAddConstant("EC_INVALID_INSTANCE"_Stz,   EC_INVALID_INSTANCE);
  /* 21 */ vAddConstant("EC_INVALID_SYNTAX"_Stz,     EC_INVALID_SYNTAX);
  /* 22 */ vAddConstant("EC_INVALID_VALUE"_Stz,      EC_INVALID_VALUE);
                        
  /* 23 */ vAddConstant("EC_LOW_BUFFERS"_Stz,        EC_LOW_BUFFERS);
  /* 24 */ vAddConstant("EC_MAX_INDEXES"_Stz,        EC_MAX_INDEXES);
  /* 25 */ vAddConstant("EC_NO_COLUMNS_DIC"_Stz,     EC_NO_COLUMNS_DIC);
  /* 26 */ vAddConstant("EC_NO_SUCH_CATALOG"_Stz,    EC_NO_SUCH_CATALOG);
  /* 27 */ vAddConstant("EC_NO_SUCH_DIRECTORY"_Stz,  EC_NO_SUCH_DIRECTORY);
  /* 28 */ vAddConstant("EC_NO_SUCH_FOLDER"_Stz,     EC_NO_SUCH_FOLDER);
  /* 29 */ vAddConstant("EC_NO_SUCH_INDEX"_Stz,      EC_NO_SUCH_INDEX);
  /* 30 */ vAddConstant("EC_NO_SUCH_SCHEMA"_Stz,     EC_NO_SUCH_SCHEMA);
  /* 31 */ vAddConstant("EC_NO_SUCH_TABLE"_Stz,      EC_NO_SUCH_TABLE);
  /* 32 */ vAddConstant("EC_NO_SUCH_TENANT"_Stz,     EC_NO_SUCH_TENANT);
  /* 33 */ vAddConstant("EC_NO_SUCH_WORD"_Stz,       EC_NO_SUCH_WORD);
  
  /* 34 */ vAddConstant("EC_NOT_A_CATALOG"_Stz,      EC_NOT_A_CATALOG);
  /* 35 */ vAddConstant("EC_NOT_A_FOLDER"_Stz,       EC_NOT_A_FOLDER);
  /* 36 */ vAddConstant("EC_NOT_AN_INDEX"_Stz,       EC_NOT_AN_INDEX);
  /* 37 */ vAddConstant("EC_NOT_A_SCHEMA"_Stz,       EC_NOT_A_SCHEMA);
  /* 38 */ vAddConstant("EC_NOT_A_TABLE"_Stz,        EC_NOT_A_TABLE);
  /* 39 */ vAddConstant("EC_NOT_A_TENANT"_Stz,       EC_NOT_A_TENANT);
  
  /* 40 */ vAddConstant("EC_NOT_IN_CATALOGS"_Stz,    EC_NOT_IN_CATALOGS);
  /* 41 */ vAddConstant("EC_NOT_IN_COLUMNS"_Stz,     EC_NOT_IN_COLUMNS);
  /* 42 */ vAddConstant("EC_NOT_IN_INDEXES"_Stz,     EC_NOT_IN_INDEXES);
  /* 43 */ vAddConstant("EC_NOT_IN_PARTITIONS"_Stz,  EC_NOT_IN_PARTITIONS);
  /* 44 */ vAddConstant("EC_NOT_IN_SCHEMAS"_Stz,     EC_NOT_IN_SCHEMAS);
  /* 45 */ vAddConstant("EC_NOT_IN_TABLE"_Stz,       EC_NOT_IN_TABLE);
  /* 46 */ vAddConstant("EC_NOT_IN_TABLES"_Stz,      EC_NOT_IN_TABLES);
  /* 47 */ vAddConstant("EC_NOT_IN_TENANTS"_Stz,     EC_NOT_IN_TENANTS);
  /* 48 */ vAddConstant("EC_NULL_INTERFACE"_Stz,     EC_NULL_INTERFACE);
  /* 49 */ vAddConstant("EC_NULLPTR"_Stz,            EC_NULLPTR);
  /* 50 */ vAddConstant("EC_RESPONSE_PENDING"_Stz,   EC_RESPONSE_PENDING);
  /* 51 */ vAddConstant("EC_SCHEMA_EXISTS"_Stz,      EC_SCHEMA_EXISTS);
  /* 52 */ vAddConstant("EC_SE_NOT_RUNNING"_Stz,     EC_SE_NOT_RUNNING);
  /* 53 */ vAddConstant("EC_SE_UNKNOWN_ERROR"_Stz,   EC_SE_UNKNOWN_ERROR);
  /* 54 */ vAddConstant("EC_SEND_FAILURE"_Stz,       EC_SEND_FAILURE);
  
  /* 55 */ vAddConstant("EC_TABLE_EXISTS"_Stz,       EC_TABLE_EXISTS);
  /* 56 */ vAddConstant("EC_TABLE_IN_USE"_Stz,       EC_TABLE_IN_USE);
  /* 57 */ vAddConstant("EC_TABLE_NOT_EMPTY"_Stz,    EC_TABLE_NOT_EMPTY);
  /* 58 */ vAddConstant("EC_TENANT_EXISTS"_Stz,      EC_TENANT_EXISTS);
  /* 59 */ vAddConstant("EC_VALIDATION_FAILURE"_Stz, EC_VALIDATION_FAILURE);
  /* 60 */ vAddConstant("EC_VERSION_MISMATCH"_Stz,   EC_VERSION_MISMATCH);
  /* 61 */ vAddConstant("EC_ENUMS"_Stz,              EC_ENUMS);

  // AT_ : fetch techniques
  /*  0 */ vAddConstant("AT_NONE"_Stz,  AT_NONE);
  /*  1 */ vAddConstant("AT_EXACT"_Stz, AT_EXACT);
  /*  2 */ vAddConstant("AT_LOWER"_Stz, AT_LOWER);
  /*  3 */ vAddConstant("AT_UPPER"_Stz, AT_UPPER);
  /*  4 */ vAddConstant("AT_RANGE"_Stz, AT_RANGE);
  /*  5 */ vAddConstant("AT_SCAN"_Stz,  AT_SCAN);
  /*  6 */ vAddConstant("AT_ENUMS"_Stz, AT_ENUMS);
  
  // TN_ : token types
  /*  0 */ vAddConstant("TN_NONE"_Stz,    TN_NONE);
  /*  1 */ vAddConstant("TN_NAME"_Stz,    TN_NAME);
  /*  2 */ vAddConstant("TN_INTEGER"_Stz, TN_INTEGER);
  /*  3 */ vAddConstant("TN_STRING"_Stz,  TN_STRING);
  /*  4 */ vAddConstant("TN_ENUMS"_Stz,   TN_ENUMS);
  
  /*  0 */ vAddConstant("TH_NONE"_Stz,                TH_NONE);
  /*  1 */ vAddConstant("TH_ANSWER"_Stz,              TH_ANSWER);
  /*  2 */ vAddConstant("TH_COORDINATOR"_Stz,         TH_COORDINATOR);
  /*  3 */ vAddConstant("TH_COORDINATOR_PROXY"_Stz,   TH_COORDINATOR_PROXY);
  /*  4 */ vAddConstant("TH_GENERAL"_Stz,             TH_GENERAL);
  /*  5 */ vAddConstant("TH_LISTER"_Stz,              TH_LISTER);
  /*  6 */ vAddConstant("TH_MONITOR"_Stz,             TH_MONITOR);
  /*  7 */ vAddConstant("TH_QUERY"_Stz,               TH_QUERY);
  /*  8 */ vAddConstant("TH_PARTICIPANT"_Stz,         TH_PARTICIPANT);
  /*  9 */ vAddConstant("TH_PARTICIPANT_PROXY"_Stz,   TH_PARTICIPANT_PROXY);
  /* 10 */ vAddConstant("TH_RECLAIM"_Stz,             TH_RECLAIM);
  /* 11 */ vAddConstant("TH_REQUEST"_Stz,             TH_REQUEST);
  /* 12 */ vAddConstant("TH_SCANOP"_Stz,              TH_SCANOP);
  /* 13 */ vAddConstant("TH_SINKOP"_Stz,              TH_SINKOP);
  /* 14 */ vAddConstant("TH_WRITEOP"_Stz,             TH_WRITEOP);
  /* 15 */ vAddConstant("TH_ENUMS"_Stz,               TH_ENUMS); 
  
  // SIZEOF_ : C++ class sizes
  /*  1 */ vAddConstant("SIZEOF_INTVAL"_Stz, sizeof(IntVal));
  /*  2 */ vAddConstant("SIZEOF_IPVAL"_Stz,  sizeof(IpVal));
  /*  3 */ vAddConstant("SIZEOF_OBJVAL"_Stz, sizeof(ObjVal));
  /*  4 */ vAddConstant("SIZEOF_PTRVAL"_Stz, sizeof(PtrVal));
  /*  5 */ vAddConstant("SIZEOF_STRVAL"_Stz, sizeof(StrVal));
  /*  6 */ vAddConstant("SIZEOF_VALSET"_Stz, sizeof(ValWrk)); // correct
  /*  7 */ vAddConstant("SIZEOF_ValWrk"_Stz, sizeof(ValWrk)); // correct
  
  /*  8 */ vAddConstant("SIZEOF_ARGUMENTS"_Stz,    sizeof(Arguments));
  /*  9 */ vAddConstant("SIZEOF_ATOMIC"_Stz,       sizeof(Atomic));
  /* 10 */ vAddConstant("SIZEOF_ATTACHMAP"_Stz,    sizeof(AttachMap));
  /* 11 */ vAddConstant("SIZEOF_BUFFER"_Stz,       sizeof(Buffer));
  /* 12 */ vAddConstant("SIZEOF_BUFFERPOOL"_Stz,   sizeof(BufferPool));
  /* 13 */ vAddConstant("SIZEOF_BPA"_Stz,          sizeof(BPA));
  /* 14 */ vAddConstant("SIZEOF_DIC"_Stz,          sizeof(Dic));
  /* 15 */ vAddConstant("SIZEOF_DICPATH"_Stz,      sizeof(DicPath));
  /* 16 */ vAddConstant("SIZEOF_DIR"_Stz,          sizeof(Dir));
  /* 17 */ vAddConstant("SIZEOF_FILE"_Stz,         sizeof(File));
  /* 18 */ vAddConstant("SIZEOF_FREECHUNK"_Stz,    sizeof(FreeChunk));
  /* 19 */ vAddConstant("SIZEOF_GRID"_Stz,         sizeof(Grid));
  /* 20 */ vAddConstant("SIZEOF_HEAPBUFFER"_Stz,   sizeof(HeapBuffer));
  /*    */ vAddConstant("SIZEOF_INDEX"_Stz,        sizeof(Index));
  /* 21 */ //vAddConstant("SIZEOF_INDEXDEFN"_Stz,    sizeof(IndexDefn));
  /* 22 */ vAddConstant("SIZEOF_LOADER"_Stz,       sizeof(Loader));
  /* 23 */ vAddConstant("SIZEOF_LOADERSUB"_Stz,    sizeof(LoaderSub));
  /* 24 */ vAddConstant("SIZEOF_LOCK"_Stz,         sizeof(Lock));
  /* 25 */ vAddConstant("SIZEOF_MEDIUMHEAP"_Stz,   sizeof(Heap));
  /* 26 */ vAddConstant("SIZEOF_NODE"_Stz,         sizeof(Node));
  /* 27 */ vAddConstant("SIZEOF_NODECURSOR"_Stz,   sizeof(NodeCursor));
  /* 28 */ vAddConstant("SIZEOF_PARSER"_Stz,       sizeof(Parser));
  /* 29 */ vAddConstant("SIZEOF_PARTITION"_Stz,    sizeof(Partition));
  /*    */ vAddConstant("SIZEOF_PROXY"_Stz,        sizeof(Proxy));
  /* 32 */ vAddConstant("SIZEOF_PROXYSET"_Stz,     sizeof(ProxySet));
  /* 33 */ vAddConstant("SIZEOF_QUEUE"_Stz,        sizeof(Queue));
  /* 34 */ vAddConstant("SIZEOF_QUEUEBUFFER"_Stz,  sizeof(QueueBuffer));
  /*    */ vAddConstant("SIZEOF_RBCURSOR"_Stz,     sizeof(RbCursor));
  /* 35 */ vAddConstant("SIZEOF_RING"_Stz,         sizeof(Ring));
  /* 36 */ vAddConstant("SIZEOF_ROW"_Stz,          sizeof(Entry));
  /* 37 */ vAddConstant("SIZEOF_ROWBUFFER"_Stz,    sizeof(EntryBuffer));
  /* 38 */ vAddConstant("SIZEOF_ROWCURSOR"_Stz,    (i8)0);

  /* 41 */ vAddConstant("SIZEOF_SCANBUFFER"_Stz,   sizeof(ScanBuffer));
  /* 42 */ vAddConstant("SIZEOF_SEHISTORY"_Stz,    sizeof(SeHistory));
  /* 43 */ vAddConstant("SIZEOF_SEITF"_Stz,        sizeof(SEitf));
  /* 44 */ vAddConstant("SIZEOF_SEITFCURSOR"_Stz,  sizeof(SEitfCursor));
  /* 45 */ vAddConstant("SIZEOF_SEITFLIST"_Stz,    sizeof(SEitfList));
  /*    */ vAddConstant("SIZEOF_SELCURSOR"_Stz,    sizeof(SelCursor));
  /*    */ vAddConstant("SIZEOF_SEMAPHORE"_Stz,    sizeof(Semaphore));
  /* 46 */ vAddConstant("SIZEOF_SEPROMPT"_Stz,     sizeof(SePrompt));
  /* 47 */ vAddConstant("SIZEOF_SINCE"_Stz,        sizeof(Since));
  /* 48 */ vAddConstant("SIZEOF_SKIPCURSOR"_Stz,   sizeof(SkipCursor));
  /* 49 */ vAddConstant("SIZEOF_SKIPLIST"_Stz,     sizeof(SkipList));
  /* 50 */ vAddConstant("SIZEOF_SKIPNODE"_Stz,     sizeof(SkipNode));
  /*    */ vAddConstant("SIZEOF_SLOTS"_Stz,        sizeof(Slots));
  /* 51 */ vAddConstant("SIZEOF_SMALLHEAP"_Stz,    sizeof(Heap));
  /* 52 */ vAddConstant("SIZEOF_SOCKET"_Stz,       sizeof(Socket));
  /* 53 */ vAddConstant("SIZEOF_STACK"_Stz,        sizeof(Stack));
  /* 54 */ vAddConstant("SIZEOF_STEPACCUM"_Stz,    sizeof(StepAccum));
  /* 55 */ vAddConstant("SIZEOF_STEPARRAY"_Stz,    sizeof(StepArray));
  /* 56 */ vAddConstant("SIZEOF_SYSCALL"_Stz,      sizeof(SysCall));
  /* 57 */ vAddConstant("SIZEOF_TABLE"_Stz,        sizeof(Table));
  /* 58 */ vAddConstant("SIZEOF_TASK"_Stz,         sizeof(Task));
  /* 69 */ vAddConstant("SIZEOF_TASKCURSOR"_Stz,   sizeof(TaskCursor));
  /* 60 */ vAddConstant("SIZEOF_TASKLIST"_Stz,     sizeof(TaskList));
  /*    */ vAddConstant("SIZEOF_TASKQUEUE"_Stz,    sizeof(TaskQueue));
  /* 61 */ vAddConstant("SIZEOF_SCHEDULER"_Stz,    sizeof(ServerScheduler));
  /* 61 */ vAddConstant("SIZEOF_TASKSET"_Stz,      sizeof(TaskSet));
  /* 62 */ vAddConstant("SIZEOF_THREAD"_Stz,       sizeof(Thread));
  /* 63 */ vAddConstant("SIZEOF_THREADCURSOR"_Stz, sizeof(ThreadCursor));
  /* 64 */ vAddConstant("SIZEOF_THREADLIST"_Stz,   sizeof(ThreadList));
  /* 64 */ vAddConstant("SIZEOF_THREADQUEUE"_Stz,  sizeof(ThreadQueue));
  /* 65 */ vAddConstant("SIZEOF_TIME"_Stz,         sizeof(Time));
  /* 66 */ vAddConstant("SIZEOF_TIMER"_Stz,        sizeof(Timer));
  /* 67 */ vAddConstant("SIZEOF_TOKENISER"_Stz,    sizeof(Tokeniser));
  /* 68 */ vAddConstant("SIZEOF_TX"_Stz,           sizeof(Tx));
  /* 74 */ vAddConstant("SIZEOF_WORD"_Stz,         sizeof(Word));

  /*  0 */ vAddConstant("SIZEOF_S32"_Stz,          sizeof(s32));
  /*  1 */ vAddConstant("SIZEOF_S256"_Stz,         sizeof(s256));
  /*  2 */ vAddConstant("SIZEOF_S16K"_Stz,         sizeof(s48K));

  // TK_ : task kind
  /*  0 */ vAddConstant("TK_NONE"_Stz,                    TK_NONE);
  /*  0 */ vAddConstant("TK_ANSWER_DRONE"_Stz,            TK_ANSWER_DRONE);
  /*  0 */ vAddConstant("TK_LISTER_DRONE"_Stz,            TK_LISTER_DRONE);
  /*  0 */ vAddConstant("TK_CONNECTOR_DRONE"_Stz,         TK_CONNECTOR_DRONE);
  /*  0 */ vAddConstant("TK_MONITOR_DRONE"_Stz,           TK_MONITOR_DRONE);
  /*  0 */ vAddConstant("TK_RECLAIM_DRONE"_Stz,           TK_RECLAIM_DRONE);
  /*  0 */ vAddConstant("TK_RECEIVER_DRONE"_Stz,          TK_RECEIVER_DRONE );
  /*  0 */ vAddConstant("TK_REQUEST_SERVER"_Stz,          TK_REQUEST_SERVER);
  /*  0 */ vAddConstant("TK_LOADSUB_SERVER"_Stz,          TK_LOADSUB_SERVER);
  /*  0 */ vAddConstant("TK_COORDINATOR_WORKER"_Stz,      TK_COORDINATOR_WORKER);
  /*  0 */ vAddConstant("TK_COORDINATOR_PROXY_WORKER"_Stz,TK_COORDINATOR_PROXY_WORKER);
  /*  0 */ vAddConstant("TK_PARTICIPANT_WORKER"_Stz,      TK_PARTICIPANT_WORKER);
  /*  0 */ vAddConstant("TK_PARTICIPANT_PROXY_WORKER"_Stz,TK_PARTICIPANT_PROXY_WORKER);
  /*  0 */ vAddConstant("TK_SENDER_WORKER"_Stz,           TK_SENDER_WORKER);
  /*  0 */ vAddConstant("TK_LOADSUB_PART"_Stz,            TK_LOADSUB_PART);
  /*  0 */ vAddConstant("TK_SELECT_PART"_Stz,             TK_SELECT_PART);
  /*  0 */ vAddConstant("TK_ENUMS"_Stz,                   TK_ENUMS );
  
  // TS_ : task state
  /*  0 */ vAddConstant("TS_NONE"_Stz,           TS_NONE);
  /*  1 */ vAddConstant("TS_BLOCKED"_Stz,        TS_BLOCKED);
  /*  2 */ vAddConstant("TS_EXITED"_Stz,         TS_EXITED);
  /*  3 */ vAddConstant("TS_RUNNING"_Stz,        TS_RUNNING);
  /*  4 */ vAddConstant("TS_SLEEPING"_Stz,       TS_SLEEPING);
  /*  5 */ vAddConstant("TS_WAITING_ACK"_Stz,    TS_WAITING_ACK);
  /*  6 */ vAddConstant("TS_WAITING_SIGNAL"_Stz, TS_WAITING_SIGNAL);
  /*  7 */ vAddConstant("TS_YIELDED"_Stz,        TS_YIELDED);
  /*  8 */ vAddConstant("TS_ENUMS"_Stz,          TS_ENUMS);

  // TP_ : transaction phase
  /*  0 */ vAddConstant("TP_NONE"_Stz,        TP_NONE);
  /*  1 */ vAddConstant("TP_EXECUTING"_Stz,   TP_EXECUTING);
  /*  2 */ vAddConstant("TP_COMMITTING"_Stz,  TP_COMMITTING);
  /*  3 */ vAddConstant("TP_COMMITTED"_Stz,   TP_COMMITTED);
  /*  4 */ vAddConstant("TP_PREPARING"_Stz,   TP_PREPARING);
  /*  5 */ vAddConstant("TP_PREPARED"_Stz,    TP_PREPARED);
  /*  6 */ vAddConstant("TP_ROLLINGBACK"_Stz, TP_ROLLINGBACK);
  /*  7 */ vAddConstant("TP_ROLLEDBACK"_Stz,  TP_ROLLEDBACK);
  /*  8 */ vAddConstant("TP_ENUMS"_Stz,       TP_ENUMS);

  // WT_ : word type
  /*  0 */ vAddConstant("WT_NONE"_Stz,      WT_NONE);
  /*  0 */ vAddConstant("WT_OBJECT"_Stz,    WT_OBJECT);
  /*  0 */ vAddConstant("WT_PRIMITIVE"_Stz, WT_PRIMITIVE);
  /*  0 */ vAddConstant("WT_ROUTINE"_Stz,   WT_ROUTINE);
  /*  0 */ vAddConstant("WT_VARIABLE"_Stz,  WT_VARIABLE);
  /*  0 */ vAddConstant("WT_ENUMS"_Stz,     WT_ENUMS);
  
  // OPEN_ : file open argument
  /*  0 */ vAddConstant("OPEN_READ"_Stz,              C_OPEN_READ);
  /*  1 */ vAddConstant("OPEN_WRITE"_Stz,             C_OPEN_WRITE);
  /*  2 */ vAddConstant("OPEN_APPEND"_Stz,            C_OPEN_APPEND);
  /*  3 */ vAddConstant("OPEN_READ_WRITE_EXIST"_Stz,  C_OPEN_READ_WRITE_EXIST);
  /*  4 */ vAddConstant("OPEN_READ_WRITE_CREATE"_Stz, C_OPEN_READ_WRITE_CREATE);
  /*  5 */ vAddConstant("OPEN_READ_APPEND"_Stz,       C_OPEN_READ_APPEND);
  
  // miscellaneous
  /*  0 */ vAddConstant("CLOCK_FREQUENCY"_Stz, Synch::iTickFrequency());
  /*  0 */ vAddConstant("CORE_COUNT"_Stz,      Hardware::iCoreCount());
  /*  0 */ vAddConstant("CPU_COUNT"_Stz,       Hardware::iCpuCount());
  /*  0 */ vAddConstant("PHYSICAL_MEMORY"_Stz, Hardware::iPhysicalMemory());
  
  // debug
  vAddConstant("answerDronePrint"_Stz,    (i8)0);
  vAddConstant("connectorDronePrint"_Stz, (i8)0);
  vAddConstant("listerDronePrint"_Stz,    (i8)0);
  vAddConstant("monitorDronePrint"_Stz,   (i8)0);
  vAddConstant("reclaimDronePrint"_Stz,   (i8)0);
  vAddConstant("receiverDronePrint"_Stz,  (i8)0);
  
  vAddConstant("requestServerPrint"_Stz,  (i8)0);
  
  vAddConstant("senderWorkerPrint"_Stz,   (i8)0);
  
  // c++ objects
  vAddConstant("scheduler"_Stz, (i8)gpServerScheduler);
}

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (gbVerbose)
    vTrace("[[mainThread]] Adding primitives...\n"_Stz);
  
  vAddPrimitive("word"_Stz,            &vWord,             false);
  gpWord = gpMainDic->pFindWord("word"_Stz);

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
  vAddPrimitives_Locks();
  vAddPrimitives_Logic();
  
  vAddPrimitives_Magnitude();
  vAddPrimitives_Memory();
  
  vAddPrimitives_Node();
  vAddPrimitives_NodeCursor();
  
  vAddPrimitives_Object();
  vAddPrimitives_OpFrame();
  
  vAddPrimitives_PartitionScheduler();
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
  vAddPrimitives_Semaphore();
  vAddPrimitives_ServerScheduler();
  vAddPrimitives_Shell();
  vAddPrimitives_Slots();
  vAddPrimitives_Socket();
  vAddPrimitives_Steps();
  vAddPrimitives_Str();
  
  vAddPrimitives_Table();
  vAddPrimitives_Task();
  vAddPrimitives_TaskCursor();
  vAddPrimitives_TaskList();
  vAddPrimitives_TaskQueue();
  vAddPrimitives_TaskSet();
  vAddPrimitives_Thread();
  vAddPrimitives_ThreadCursor();
  vAddPrimitives_ThreadList();
  vAddPrimitives_ThreadQueue();
  vAddPrimitives_Timer();
  vAddPrimitives_To();
  vAddPrimitives_Trace();
  vAddPrimitives_Tx();
  vAddPrimitives_TxSet();
  
  vAddPrimitives_Utility();
  
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
    vTrace("[[mainThread]] Adding routines...\n"_Stz);
  
  s48K sDefinition;

  sDefinition =
  ": __nil ;"_Stz;
  
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
  //"      dic.isCompiled not if process.quit then "
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
  
  " ;"_Stz;
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": ::task_exit"
  "  TS_EXITED task::this task.state!"
  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": ::docmd"
  //  "  input.text"
  "  docol"
  "  ::task_exit"
  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  
  sDefinition =
  ": ::enter"
  "  input.stdin"
  "  docol"
  "  ::task_exit"
  "  ;"_Stz;
  
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
  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  //-----------------------------------------------------
  sDefinition =
  ": include-string"
  "  input.push"
  //"  dup ."
  "  input.text"
  "  docol"
  "  input.pop"
  "  ;"_Stz;
  
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
  "    . debug::trap"
  "  then"
  "  include-file"
  "  ;"_Stz;
  
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
  "  ;"_Stz;
#else
  sDefinition =
  ": ::dicCompile"
  "  \"::dicCompile\" .verbose"
  "  \"compile.sen\" include"
  "  \"::dicCompile exit\" .verbose"
  "  ::task_exit"
  "  ;"_Stz;
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
  //vTrace("CompileColon: %s\n"_Stz,m_sToken.aChar());
  
  // get name of word
  m_pTokeniser->vNextToken();
  //vTrace("CompileColon: %s\n"_Stz,m_sToken.aChar());
  
  Dic* pDic;
  if (m_xPathStack.iCount())
    pDic = m_xPathStack.pTop();
  else
    pDic = gpMainDic;

  pNewWord = new(pDic) Word(m_pTokeniser->sToken());
  
  m_pCurrentWord = pNewWord;

  m_pTokeniser->vNextToken();
  
  while (TN_NONE != m_pTokeniser->eToken()) {
    
    //vTrace("CompileColon: %s\n"_Stz,m_sToken.aChar());
    
//  if (StrLib::bEqualTo(";",m_pTokeniser->sToken().aChar()))
    if (m_pTokeniser->sToken().bEqual(";"_Stz))
      break;
    
    switch (m_pTokeniser->eToken()) {
        
      case TN_INTEGER:
        xValWrk = m_pTokeniser->sToken().iToI8();
        pNewWord->vEncode("pushValue"_Stz,&xValWrk);
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
          THROW("Invalid eWordType %lld"_Stz,pCalledWord->eWordType());
        }
        break;
        
      case TN_STRING: {
        xValWrk = m_pTokeniser->sToken();
        pNewWord->vEncode("pushValue"_Stz,&xValWrk);
        break;
      }
        
      default:
        THROW("Invalid m_eToken %lld"_Stz,m_pTokeniser->eToken());
    }
    
    m_pTokeniser->vNextToken();
  }
  
  pNewWord->vEncode("exit"_Stz);
  

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
  //vTrace("CompileConstant: %s\n"_Stz,m_pTokeniser->sToken().aChar());
  sConstantValue = (Stz*)m_pTokeniser->sToken().aChar();
  
  // get CONSTANT
  m_pTokeniser->vNextToken();
  //vTrace("CompileConstant: %s\n"_Stz,m_pTokeniser->sToken().aChar());
  
  // get name of word
  m_pTokeniser->vNextToken();
  //vTrace("CompileConstant: %s\n"_Stz,m_pTokeniser->sToken().aChar());

  Stz* aName = (Stz*)m_pTokeniser->sToken().aChar();

  //vTrace("CompileConstant: %s %s\n"_Stz,aName,sConstantValue.aChar());
  
  vAddConstant(aName,(Stz*)sConstantValue.aChar());
  
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
void Task::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_TASK);
  ASSERT(pObject->eClass() == CN_TASK);
  
  Task* pTask = reinterpret_cast<Task*>(pObject);
  
  vTrace("%s TaskNo %lld state %s txId %lld\n"_Stz,
         pTask->m_sName.aChar(),
         pTask->m_iTaskNo,
         aTS[pTask->m_eState]->aChar(),
         pTask->m_iTxId);
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Task::vContentDump(void)
{
  _poolAddress_(this);
  
  vTrace("%sTask %02lld %p: %16s state %8s txId %lld\n"_Stz,
         aDoubleTab(),
         m_iTaskNo,
         this,
         m_sName.aChar(),
         aTS[m_eState]->aChar(),
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
void Task::vPrimitive(Stz* aPrimitive)
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
    sRoutine = "<unknown>"_Stz;
  
  i8 iDataValues  = m_pDataStack->iValues();
  i8 iReturnValues = m_pReturnStack->iValues();
  vTrace("%-15s %15s : r%lld ; d%2d="_Stz,
         sRoutine.aChar(),
         aPrimitive,
         iReturnValues,
         iDataValues);
  
  s48K sStackContents;
  vStackContents(m_pDataStack, sStackContents);
  vTrace("%s\n"_Stz,sStackContents.aChar());
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
void Task::vPushStr(Stz* aStr)
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
  
  m_pTokeniser->sToken() = ""_Stz;
  
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
  
  m_pTokeniser->sToken() = ""_Stz;
  
  m_pDataStack->vRetry();
  m_pReturnStack->vRetry();
}

/*----------------------------------------------------------------------------*/
/* seed task to run ::dicCompile                                              */
/*----------------------------------------------------------------------------*/
void Task::vSeedDicCompileSen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (gbVerbose)
    vTrace("[[mainThread]] Seeding Dic Compile...\n"_Stz);
  
  Word* pBootSen = gpMainDic->pFindWord("::dicCompile"_Stz);
  ASSERT(pBootSen);
  
  IP.m_pCell = pBootSen->pFirstCell();
  
  m_eState = TS_RUNNING;
  
  m_pDataStack->vReset();
  m_pReturnStack->vReset();
  
  m_sLambda = ""_Stz;
  
  m_pTokeniser->vUseString(m_sLambda);
}

/*----------------------------------------------------------------------------*/
/* seed task to run given lambda                                              */
/*----------------------------------------------------------------------------*/
void Task::vSeedWord(Stz* aLambda)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pShellEnter = gpMainDic->pFindWord("::docmd"_Stz);
  
  IP.m_pCell = pShellEnter->pFirstCell();
  
  m_eState = TS_YIELDED;
  
  m_pDataStack->vReset();
  m_pReturnStack->vReset();
  
  m_sLambda = aLambda;
  if (m_sLambda.iLength())
    m_sLambda.vConcat(" ::task_exit"_Stz);
  
  m_pTokeniser->vUseString(m_sLambda);
}

/*----------------------------------------------------------------------------*/
/* seed task to run ::enter                                                   */
/*----------------------------------------------------------------------------*/
void Task::vSeedDocol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pShellEnter = gpMainDic->pFindWord("::enter"_Stz);
  
  IP.m_pCell = pShellEnter->pFirstCell();
  
  m_eState = TS_YIELDED;
  
  m_pDataStack->vReset();
  m_pReturnStack->vReset();
  
  m_sLambda = ""_Stz;
  
  s32 sPrompt("docol> "_Stz);
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
  
  sStackContents = ""_Stz;
  
  i8 iParamValues = pStack->iValues();
  
  sStackContents.vCatSprint("("_Stz);
  for (i8 iValue = iParamValues - 1; iValue >= 0; iValue--) {
    
    sStackContents.vCatSprint(" "_Stz);
    
    Value* pValue = pStack->pPeekValue(iValue);
    
    eCN eClass = pValue->eClass();
    
    if (CN_BINVAL == eClass) {
      Object* pObject = _pBinVal(pValue)->pNewObject();
      sStackContents.vCatSprint("(BINVAL)^%s=%p"_Stz,
                                aCN[pObject->eClass()],
                                pObject);
    }
    else if (CN_INTVAL == eClass) {
      sStackContents.vCatSprint("%lld"_Stz,_pIntVal(pValue)->iInt());
    }
    else if (CN_PTRVAL == eClass) {
      if (_pPtrVal(pValue)->pPtr()) {
        
        Object* pPtr = _pPtrVal(pValue)->pPtr();
        
        eCN ePtrClass = _pPtrVal(pValue)->pPtr()->eClass();
        
        if (CN_WORD == ePtrClass)
          sStackContents.vCatSprint("^%s=%p:%s"_Stz,
                                    aCN[ePtrClass],
                                    pPtr,
                                    ((Word*)pPtr)->sName().aChar());
        else if (CN_QUEUE == ePtrClass) {
          Queue* pQueue = _pQueue(pPtr);
          sStackContents.vCatSprint("^%s=%p:%s"_Stz,
                                    aCN[ePtrClass],
                                    pPtr,
                                    "queue"_Stz);
        }
        else if (CN_TASK == ePtrClass) {
          Task* pTask = _pTask(pPtr);
          sStackContents.vCatSprint("^%s=%p:%s"_Stz,
                                    aCN[ePtrClass],
                                    pPtr,
                                    pTask->sName().aChar());
        }
        else {
          if (pPtr->iMagic() == C_MAGIC && pPtr->eClass() < CN_ENUMS)
            sStackContents.vCatSprint("^%s=%p"_Stz,
                                      aCN[ePtrClass],
                                      pPtr);
          //else
          //  sStackContents.vCatSprint("%p"_Stz,
          //                     pValWrk->pPtr());
        }
      }
      else
        sStackContents.vCatSprint("^%s=%p"_Stz,
                                  pValue->aClass(),
                                  _pPtrVal(pValue)->pPtr());
    }
    else if (CN_IPVAL == eClass) {                                      
      sStackContents.vCatSprint("%p"_Stz,
                                _pIpVal(pValue)->pIpPtr());
    }
    else if (CN_STRVAL == eClass) {
      sStackContents.vCatSprint("\"%lls\""_Stz,
                                _pStrVal(pValue)->aChar());
    }
    else if (CN_VALSET == eClass) {
      sStackContents.vCatSprint("<%s>=%p"_Stz,
                                pValue->aClass(),
                                pValue);
    }
    else {
      sStackContents.vCatSprint("<unknown:class=%lld>"_Stz,
                                pValue->eClass());
    }
  }
  
  sStackContents.vCatSprint(" )"_Stz);
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
void Task::vTracePoint(Stz* aMark)
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
    sRoutine = "<unknown>"_Stz;
  
  i8 iAuxValues    = m_pAuxStack->iValues();
  i8 iDataValues   = m_pDataStack->iValues();
  i8 iReturnValues = m_pReturnStack->iValues();
  vTrace("[%s(N%d):%8s\\%lld %-11s] %-16s %-16s : a%2lld="_Stz,
         
         gpGrid->pThisNode()->aNodeName(),
         gpGrid->iThisNode(),
         m_sName.aChar(),
         *m_zAcks,
         m_sComment.aChar(),
         
         sRoutine.aChar(),
         aMark,
         iAuxValues);
         
  s48K sStackContents;
  vStackContents(m_pAuxStack, sStackContents);
  
  vTrace("%s"_Stz,sStackContents.aChar());
  
  vTrace(" r%2lld  d%2lld="_Stz, iReturnValues, iDataValues);

  vStackContents(m_pDataStack, sStackContents);
  
  vTrace("%s\n"_Stz,sStackContents.aChar());
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
void Task::vTaskLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  while (TS_RUNNING == m_eState || TS_WAITING_SIGNAL == m_eState) {
    
    //assert(0 == m_iTaskNo || m_pThread);
    
    TaskStep pTaskStep = *IP.m_pCell->m_ppEntryStep;
    
    m_pEntryCell = IP.m_pCell->m_ppEntryStep + 1;
    
    //vTrace("pTaskStep %p\n"_Stz,pTaskStep);
    
    IP.m_pCell++;
    
    try {
      (pTaskStep)(this);
    }
    
    catch (i8 iException) {
      char wFileName[256];
      
      const char* ps;
      
      ps = StrLib::sLast(m_sFile.aChar());
      StrLib::bBackward(&ps,m_sFile.aChar(),'/');
      bool bBackSlash = StrLib::bBackward(&ps,m_sFile.aChar(),'/');
      if (bBackSlash) {
        StrLib::vCopy(wFileName,ps+1);
      }
      else {
        StrLib::vCopy(wFileName, m_sFile.aChar());
      }
      vError("[%s] %s : line %lld in %s\n"_Stz,
             m_sName.aChar(),
             m_sException.aChar(),
             iException,
             wFileName);
      vRetry();
    }
    
    catch (Error* pError) {
      char wFileName[256];
      
      const char* ps;
      
      ps = StrLib::sLast(pError->aFileName()->aChar());
      StrLib::bBackward(&ps,(char*)pError->aFileName(),'/');
      StrLib::vCopy(wFileName,ps+1);
      vError("[%s] %s: line %lld in %s\n"_Stz,
             m_sName.aChar(),
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
