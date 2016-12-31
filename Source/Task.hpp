/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/

#pragma once

#include "Tasks.h"

extern pthread_key_t  giTLS_TaskKey;

namespace Interface
{
  class SEitf;
}

namespace Threads
{
  class Thread;
}

namespace Tasks
{
  enum eTS // Task State
  {
  /* 0 */ TS_NONE=0,
  /* 1 */ TS_BLOCKED,   // task is blocked by another transaction
  /* 2 */ TS_EXITED,    // task has completed
  /* 3 */ TS_RUNNING,   // task is currenty being executed by Thread::vLoop
  /* 4 */ TS_SLEEPING,  // task is sleeping (not yet used)
  /* 5 */ TS_SUSPENDED, // task is suspended pending inter-task activity
  /* 6 */ TS_WAITING,   // task is waiting on queue arrival
  /* 7 */ TS_YIELDED,   // task is yeilded, not running and is in a TaskSet
  /* 8 */ TS_ENUMS
  };
  
  static const char* const pTS[]
  {
    /* 0 */ "NONE",
    /* 1 */ "BLOCKED",
    /* 2 */ "EXITED",
    /* 3 */ "RUNNING",
    /* 4 */ "SLEEPING",
    /* 5 */ "SUSPENDED",
    /* 6 */ "WAITING",
    /* 7 */ "YIELDED",
    /* 8 */ "ENUMS",
  };
  
  typedef NP<Task,CN_TASK> NP_Task;
  typedef NNP<Task,CN_TASK> NNP_Task;

  class TaskSet;
  
  class Task : public Object,
               public ListNode_t<Task>
  {
    #define GET_THROW(_1,_2,NAME,...) NAME
    
    #define THROW(...) GET_THROW(__VA_ARGS__, THROW2, THROW1)(__VA_ARGS__)
      
    #define THROW1(p1) \
    { \
      s256 sFile(__FILE__); \
      s256 sException; \
      String::vSprint(sException.a(),p1); \
      Task* pTask = reinterpret_cast<Task*>(PosixThread::pTlsGet(giTLS_TaskKey)); \
      pTask->vException(sFile,sException); \
      Common::vThrowTrap(); \
      throw (i8)__LINE__; \
    }
      
    #define THROW2(p1, p2) \
    { \
      s256 sFile(__FILE__); \
      s256 sException; \
      String::vSprint(sException.a(),p1,p2); \
      Task* pTask = reinterpret_cast<Task*>(PosixThread::pTlsGet(giTLS_TaskKey)); \
      pTask->vException(sFile,sException); \
      Common::vThrowTrap(); \
      throw (i8)__LINE__; \
    }
    
    // throw invalid parameter stack
    #define THROW_INVALID_PARAM(primitive) \
    { \
      THROW("Invalid param stack for: %s",primitive); \
    }

    // throw invalid return stack
    #define THROW_INVALID_RETURN(primitive) \
    { \
      THROW("Invalid return stack for: %s",primitive); \
    }
    
#ifdef ENTRY_EXIT
    
  #ifdef STEP_BOARD
    #define ENTRY(p1) \
      gpStepBoard->vName(nnpTask->m_pThread ? nnpTask->m_pThread->iThreadNo() : 0,p1); \
      static StepAccum* pStepAccum = nullptr; \
      if (! pStepAccum) \
        pStepAccum = gpStepArray->pAllocAccum(p1); \
      i8 _iTscEntry = Synch::iTicks(); \
      nnpTask->vPrimitive(p1);
  #else
    #define ENTRY(p1) \
      static StepAccum* pStepAccum = nullptr; \
      if (! pStepAccum) \
      pStepAccum = gpStepArray->pAllocAccum(p1); \
      i8 _iTscEntry = Synch::iTicks(); \
      nnpTask->vPrimitive(p1);
  #endif
    
  #define EXIT() \
    ASSERT(pStepAccum); \
    i8 _iTscExit = Synch::iTicks(); \
    i8 _iTscDiff = _iTscExit - _iTscEntry; \
    pStepAccum->m_nInvoked++; \
    pStepAccum->m_nCycles += _iTscDiff;

#else

  #ifdef STEP_BOARD
    #define ENTRY(p1) \
      gpStepBoard->vName(nnpTask->m_pThread ? nnpTask->m_pThread->iThreadNo() : 0,p1);
  #else
    #define ENTRY(p1)
  #endif

  #define EXIT()
    
#endif
    
  private:
    
    static const i8 C_TRACE_STEPS    = 0x1;
    static const i8 C_TRACE_ROUTINES = 0x2;

    Lock    m_xLock;
    i8      m_iTaskNo;     // task
    s32     m_sName;       // task name
    eTS     m_eState;      // state (running, sleeping, ...)
    i8      m_iTxId;       // transaction id
    i8      m_iWakeAt;     // time at which to wake this tx
    Task*   m_pParentTask; // parent of this task
    Atomic  m_zAcks;       // number of acks still waaiting for
    Threads::Thread* m_pThread; // current thread
    TaskSet* m_pTaskSet;
    s32     m_sComment;    // comment
    
    NP_Task m_npNextInSet; // next task in set
    NP_Task m_npPrevInSet; // prev task in set
    
    DicStack m_xDicStack;
    
    Queue* m_pQueue;         // processing queue
    Socket* m_pSocket;       // socket
    
    Stack  m_xParamStack;     // parameter stack
    Stack  m_xReturnStack;    // return stack
    Stack  m_xAuxStack;       // auxiliary stack
    
    RegFrame* m_pRegFrame;    // register frame chain
    RegFrame* m_pLocalsFrame; // register frame chain

    s256      m_sLambda;    // lambda input
    Tokeniser m_xTokeniser; // input stream tokenizer
    s256      m_sToken;     // current token
    eTK       m_eToken;     // token type
    
    union {
      Cell*   m_pCell;    // points to cell inside a word
      Value*  m_pValue;   // points to an embedded value within a word
      Object* m_pObject;  // points to an embedded object within a word
      Header* m_pHeader;  // points to an embbeded header within a word
    } IP;
    TaskStep* m_pEntryCell; //s point to does arg 
    
    
//  u8    m_uMarker1;
    Cell* m_pResetIP;
    Cell* m_pBeginIP;
    Word* m_pWord;
    bool  m_bCompiling;
    i8    m_iTracing;
    i8    m_nLocals;      // number of locals
    Word* m_pCurrentWord;
    
    // exception
    s256  m_sFile;
    s256  m_sException;

    Interface::SEitf* m_pSEitf;
    
    // test
    Dic m_xTaskDic;
    
    i8  m_iDdlLocked;
    
    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void* operator new(size_t, Task* pTask);
    void  operator delete(void*);
    
    Task(void);
    Task(NNP_Task nnpTask);
    Task(const char* aLambda);
    void vInit(const char* aLambda);
    ~Task(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    void vAdjustNext(i8 iBytes);
    void vAddConstant(const char* aName, i8 iVal);
    void vAddConstant(const char* aName, const char* aValue);
    void vAddConstants(void);
    void vAddPrimitive(const char* aName, TaskStep pStep, bool bImmediate);
    void vAddPrimitives(void);
    void vAddRoutines(void);
    Object* pArgObject(void);
    void vCheckLocked(void);
    Word* pCompileColon(s& sDefinition);
    Word* pCurrentWord(void);
    void  vCurrentWord(Word* pWord);
    static void vDebug(void);
    void vDicDrop(void);
    Dic* pDicPeek(void);
    void vDicPush(Dic* pDic);
    void vDump(void);
    void vException(s& sFile, s& eException);
    Word* pFindWord(s& sName);
    Word* pFindWordInCurrent(s& sName);
    Word* pFindWord(const char* aName);
    bool  bForgetWord(s& sName);
    void vInitialiseDictionary(void);
    bool  bIsName(s& sName);
    void  vLocalsPop(void);
    void  vLocalsPush(void);
    Word* pLocateWord(Cell* pCell);
    void vLoop(void);
    NP_Task npNextInSet(void);
    s&   sName(void);
    void vName(s& sName);
    void vNextInSet(NP_Task npNext);
    NP_Task npPrevInSet(void);
    void vPrevInSet(NP_Task npPrev);
    void vPrimitive(const char *aPrimitive);
    void vReset(void);
    void vRetry(void);
    Threads::Thread* pRootThread(void);
    void vSeedDicCompileSen(void);
    void vSeedDocol(void);
    void vSeedWord(const char *aWord);
    void vSprint(s8K& sFormat, s8K& sString);
    void vStackContents(Stack* pStack, s& sStackContents);
    eTS  eState(void);
    void vState(eTS eState);
    Threads::Thread* pThread(void);
    void vThread(Threads::Thread* pThread);
    void vTokenNext(void);
    void vTracePoint(const char* aMark);
    i8   iTxId(void);
    void vTxId(i8 iTxId);
    void vUndirect(void);
    
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    
   
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    
    static void vAccept(NNP_Task nnpTask);
    static void vAllot(NNP_Task nnpTask);
    static void vAllParticipants(NNP_Task nnpTask);
    static void vBpa(NNP_Task nnpTask);
    static void vBpdump(NNP_Task nnpTask);
    static void vCells(NNP_Task nnpTask);
    static void vColon(NNP_Task nnpTask);
    static void vComma(NNP_Task nnpTask);          
    static void vComment(NNP_Task nnpTask);        
    static void vCompileComma(NNP_Task nnpTask);
    static void vCursorFetch(NNP_Task nnpTask);
    static void vCursorStore(NNP_Task nnpTask);
    static void vEqualAddress(NNP_Task nnpTask);
    static void vExecute(NNP_Task nnpTask);
    static void vExit(NNP_Task nnpTask);           
    static void vFetch(NNP_Task nnpTask);
    static void vHazard(NNP_Task nnpTask);
    static void vHash(NNP_Task nnpTask);
    static void vLocalCalls(NNP_Task nnpTask);
    static void vLocate(NNP_Task nnpTask);
    static void vNop(NNP_Task nnpTask);
    static void vOneParticipant(NNP_Task nnpTask);
    static void vRdtscp(NNP_Task nnpTask);
    static void vRetryPoint(NNP_Task nnpTask);
    static void vSee(NNP_Task nnpTask);
    static void vSemicolon(NNP_Task nnpTask);      
    static void vSnooze(NNP_Task nnpTask);
    static void vSleep(NNP_Task nnpTask);
    static void vStore(NNP_Task nnpTask);
    static void vStringColon(NNP_Task nnpTask);
    static void vTest(NNP_Task nnpTask);
    static void vThrow(NNP_Task nnpTask);
    static void vTick(NNP_Task nnpTask);           
    static void vToken(NNP_Task nnpTask);

    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/

    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/

    // args
    static void vArgsPartition(NNP_Task nnpTask);
    static void vArgsPrint(NNP_Task nnpTask);

    // assert
    static void vAssert(NNP_Task nnpTask);
    static void vAssertStack_0(NNP_Task nnpTask);
    static void vAssertStack_0more(NNP_Task nnpTask);
    static void vAssertStack_1(NNP_Task nnpTask);
    static void vAssertStack_1more(NNP_Task nnpTask);
    static void vAssertStack_2(NNP_Task nnpTask);
    static void vAssertStack_2more(NNP_Task nnpTask);
    static void vAssertStack_3(NNP_Task nnpTask);
    static void vAssertStack_3more(NNP_Task nnpTask);
    static void vAssertStack_4(NNP_Task nnpTask);
    static void vAssertStack_4more(NNP_Task nnpTask);
    
    // astack
    static void vAstackDepth(NNP_Task nnpTask);
    static void vAstackDrop(NNP_Task nnpTask);
    static void vAstackDup(NNP_Task nnpTask);
    static void vAstackFetch(NNP_Task nnpTask);
    static void vAstackNip(NNP_Task nnpTask);
    static void vAstackPick(NNP_Task nnpTask);
    static void vAstackReverse(NNP_Task nnpTask);
    static void vAstackSwap(NNP_Task nnpTask);
    static void vAstackToA(NNP_Task nnpTask);
    static void vAstackToP(NNP_Task nnpTask);
    static void vAstackToValSet(NNP_Task nnpTask);
    static void vAstackToValues(NNP_Task nnpTask);
    static void vAstackTransfer(NNP_Task nnpTask);
    static void vAstack0(NNP_Task nnpTask);
    static void vAstack1(NNP_Task nnpTask);
    static void vAstack2(NNP_Task nnpTask);
    static void vAstack3(NNP_Task nnpTask);
    static void vAstack4(NNP_Task nnpTask);
    static void vAstack5(NNP_Task nnpTask);
    static void vAstack6(NNP_Task nnpTask);
    static void vAstack7(NNP_Task nnpTask);
    static void vAstack8(NNP_Task nnpTask);
    static void vAstack9(NNP_Task nnpTask);
    
    // cast
    static void vCastInt(NNP_Task nnpTask);
    static void vCastPtr(NNP_Task nnpTask);
    
    // class
    static void vClassName(NNP_Task nnpTask);

    // csv
    static void vCsvFirstValue(NNP_Task nnpTask);
    
    // diclock
    static void vDicLockDropWrite(NNP_Task nnpTask);
    static void vDicLockGrabWrite(NNP_Task nnpTask);
    
    // ddllock
    static void vDdlLockDropRead(NNP_Task nnpTask);
    static void vDdlLockDropWrite(NNP_Task nnpTask);
    static void vDdlLockGrabRead(NNP_Task nnpTask);
    static void vDdlLockGrabWrite(NNP_Task nnpTask);
    static void vDdlLockTestDrop(NNP_Task nnpTask);
    
    // debug
    static void vDebugTrap(NNP_Task nnpTask);
    
    // dic
    static void vDicCount(NNP_Task nnpTask);
    static void vDicDrop(NNP_Task nnpTask);       
    static void vDicFrozenGet(NNP_Task nnpTask);
    static void vDicFrozenSet(NNP_Task nnpTask);
    static void vDicInit(NNP_Task nnpTask);
    static void vDicIsCompiled(NNP_Task nnpTask);
    static void vDicKindGet(NNP_Task nnpTask);
    static void vDicKindSet(NNP_Task nnpTask);
    static void vDicLast(NNP_Task nnpTask);
    static void vDicMain(NNP_Task nnpTask);
    static void vDicName(NNP_Task nnpTask);
    static void vDicPick(NNP_Task nnpTask);
    static void vDicPush(NNP_Task nnpTask);       
    static void vDicRef(NNP_Task nnpTask);
    static void vDicStack(NNP_Task nnpTask);
    
    static void vDicStackPush(NNP_Task nnpTask);
    
    // dir
    static void vDirExists(NNP_Task nnpTask);
    static void vDirMake(NNP_Task nnpTask);
    static void vDirRemove(NNP_Task nnpTask);
    static void vGwd(NNP_Task nnpTask);
    static void vCwd(NNP_Task nnpTask);
    
    static void vDirClose(NNP_Task nnpTask);
    static void vDirDelete(NNP_Task nnpTask);
    static void vDirEntryName(NNP_Task nnpTask);
    static void vDirEntryType(NNP_Task nnpTask);
    static void vDirInit(NNP_Task nnpTask);
    static void vDirIsClosed(NNP_Task nnpTask);
    static void vDirIsOpen(NNP_Task nnpTask);
    static void vDirNew(NNP_Task nnpTask);
    static void vDirOpen(NNP_Task nnpTask);
    static void vDirPathGet(NNP_Task nnpTask);
    static void vDirPathSet(NNP_Task nnpTask);
    static void vDirRead(NNP_Task nnpTask);
    static void vDirRewind(NNP_Task nnpTask);
    static void vDirSeek(NNP_Task nnpTask);
    static void vDirTell(NNP_Task nnpTask);
    
    // docol
    static void vDocolCompiling(NNP_Task nnpTask);
    static void vDocolInterpreting(NNP_Task nnpTask);
    static void vDocolIsCompiling(NNP_Task nnpTask);
    static void vDocolIsInterpreting(NNP_Task nnpTask);
    
    // dot
    static void vDot(NNP_Task nnpTask);
    static void vDotPut(NNP_Task nnpTask);
    static void vDotNewline(NNP_Task nnpTask);
    static void vDotVerbose(NNP_Task nnpTask);
    
    // encode
    static void vEncodeCallPrimitive(NNP_Task nnpTask);
    static void vEncodeCallWord(NNP_Task nnpTask);
    static void vEncodePushValue(NNP_Task nnpTask);
    
    // enter
    static void vEnterObject(NNP_Task nnpTask);
    static void vEnterRoutine(NNP_Task nnpTask);
    
    // env
    static void vEnvGet(NNP_Task nnpTask);
    
    // errors
    static void vErrorMsg(NNP_Task nnpTask);
    
    // file
    static void vFileClose(NNP_Task nnpTask);     
    static void vFileDelete(NNP_Task nnpTask);
    static void vFileInit(NNP_Task nnpTask);
    static void vFileNew(NNP_Task nnpTask);
    static void vFileOpen(NNP_Task nnpTask);
    static void vFilePathGet(NNP_Task nnpTask);
    static void vFilePathSet(NNP_Task nnpTask);   
    static void vFileRead(NNP_Task nnpTask);
    static void vFileReadRecord(NNP_Task nnpTask);
    static void vFileWrite(NNP_Task nnpTask);
    
    // flow
    static void vFlowAgain(NNP_Task nnpTask);
    static void vFlowAgain_(NNP_Task nnpTask);
    static void vFlowBegin(NNP_Task nnpTask);
    static void vFlowBegin_(NNP_Task nnpTask);
    static void vFlowDo(NNP_Task nnpTask);
    static void vFlowDo_RT(NNP_Task nnpTask);
    static void vFlowElse(NNP_Task nnpTask);
    static void vFlowIf(NNP_Task nnpTask);
    static void vFlowIterate(NNP_Task nnpTask);
    static void vFlowIterate_RT_A(NNP_Task nnpTask);
    static void vFlowIterate_RT_B(NNP_Task nnpTask);
    static void vFlowJump(NNP_Task nnpTask);
    static void vFlowJumpFalse(NNP_Task nnpTask);
    static void vFlowJumpTrue(NNP_Task nnpTask);
    static void vFlowLoop(NNP_Task nnpTask);
    static void vFlowLoop_RT(NNP_Task nnpTask);
    static void vFlowLoopPlus(NNP_Task nnpTask);
    static void vFlowLoopPlus_RT(NNP_Task nnpTask);
    static void vFlowNext(NNP_Task nnpTask);
    static void vFlowRepeat(NNP_Task nnpTask);
    static void vFlowThen(NNP_Task nnpTask);
    static void vFlowUnloop(NNP_Task nnpTask);
    static void vFlowUntil(NNP_Task nnpTask);
    static void vFlowWhile(NNP_Task nnpTask);
    
    // grid
    static void vGridAppendNode(NNP_Task nnpTask);
    static void vGridInit(NNP_Task nnpTask);
    static void vGridStartedStore(NNP_Task nnpTask);
    static void vGridStartedFetch(NNP_Task nnpTask);
    
    // input
    static void vInputClose(NNP_Task nnpTask);
    static void vInputDepth(NNP_Task nnpTask);
    static void vInputNext(NNP_Task nnpTask);
    static void vInputOpen(NNP_Task nnpTask);
    static void vInputPop(NNP_Task nnpTask);
    static void vInputPush(NNP_Task nnpTask);
    static void vInputStdin(NNP_Task nnpTask);
    static void vInputText(NNP_Task nnpTask);
    static void vInputToken(NNP_Task nnpTask);
    static void vInputType(NNP_Task nnpTask);
    
    // int
    static void vIntAdd(NNP_Task nnpTask);
    static void vIntDivide(NNP_Task nnpTask);
    static void vIntEqual(NNP_Task nnpTask);
    static void vIntGreater(NNP_Task nnpTask);
    static void vIntGreaterEqual(NNP_Task nnpTask);
    static void vIntLess(NNP_Task nnpTask);
    static void vIntLessEqual(NNP_Task nnpTask);
    static void vIntModulo(NNP_Task nnpTask);
    static void vIntMultiply(NNP_Task nnpTask);
    static void vIntNeg(NNP_Task nnpTask);
    static void vIntNotEqual(NNP_Task nnpTask);
    static void vIntSubtract(NNP_Task nnpTask);
    
    // loader
    static void vLoaderDelete(NNP_Task nnpTask);
    static void vLoaderFindChunk(NNP_Task nnpTask);
    static void vLoaderInit(NNP_Task nnpTask);
    static void vLoaderNew(NNP_Task nnpTask);
    static void vLoaderNewSub(NNP_Task nnpTask);
    static void vLoaderPathStore(NNP_Task nnpTask);
    static void vLoaderSubsStore(NNP_Task nnpTask);
    static void vLoaderTableStore(NNP_Task nnpTask);
    static void vLoaderTxRefStore(NNP_Task nnpTask);
    
    static void vLoaderSubAppendBuffer(NNP_Task nnpTask);
    static void vLoaderSubBegin(NNP_Task nnpTask);
    static void vLoaderSubCompleted(NNP_Task nnpTask);
    static void vLoaderSubDelete(NNP_Task nnpTask);
    static void vLoaderSubEnd(NNP_Task nnpTask);
    static void vLoaderSubFetchRecord(NNP_Task nnpTask);
    static void vLoaderSubHashRecord(NNP_Task nnpTask);
    static void vLoaderSubInit(NNP_Task nnpTask);
    static void vLoaderSubInsertRow(NNP_Task nnpTask);
    static void vLoaderSubInsertRow2(NNP_Task nnpTask);
    static void vLoaderSubLoaderStore(NNP_Task nnpTask);
    static void vLoaderSubNew(NNP_Task nnpTask);
    static void vLoaderSubPartition(NNP_Task nnpTask);
    static void vLoaderSubPartitionStore(NNP_Task nnpTask);
    static void vLoaderSubRowBuffer(NNP_Task nnpTask);
    static void vLoaderSubRowBufferStore(NNP_Task nnpTask);
    static void vLoaderSubTable(NNP_Task nnpTask);
 
    // locals
    static void vLocalFetch(NNP_Task nnpTask);
    static void vLocalStore(NNP_Task nnpTask);
    static void vLocalsBar(NNP_Task nnpTask);
    static void vLocalsPop(NNP_Task nnpTask);
    static void vLocalsPush(NNP_Task nnpTask);
    static void vLocalsTo(NNP_Task nnpTask);
   
    // lock
    static void vLockDropRead(NNP_Task nnpTask);
    static void vLockDropWrite(NNP_Task nnpTask);
    static void vLockGrabRead(NNP_Task nnpTask);
    static void vLockGrabWrite(NNP_Task nnpTask);
    static void vLockInit(NNP_Task nnpTask);
    
    // logic
    static void vLogicAnd(NNP_Task nnpTask);
    static void vLogicNot(NNP_Task nnpTask);
    static void vLogicOr(NNP_Task nnpTask);
    
    // memory
    static void vMemoryByteDump(NNP_Task nnpTask);
    static void vMemoryWordDump(NNP_Task nnpTask);
    
    // node (of grid)
    static void vNodeInit(NNP_Task nnpTask);
    static void vNodeNew(NNP_Task nnpTask);
    static void vNodeNo(NNP_Task nnpTask);
    static void vNodeStatus(NNP_Task nnpTask);
    
    // nodeCursor
    static void vNodeCursorBegin(NNP_Task nnpTask);
    static void vNodeCursorDelete(NNP_Task nnpTask);
    static void vNodeCursorEnd(NNP_Task nnpTask);
    static void vNodeCursorFirst(NNP_Task nnpTask);
    static void vNodeCursorGet(NNP_Task nnpTask);
    static void vNodeCursorInit(NNP_Task nnpTask);
    static void vNodeCursorNew(NNP_Task nnpTask);
    static void vNodeCursorNext(NNP_Task nnpTask);
    
    // number
    static void vNumberMagnitude(NNP_Task nnpTask);
    
    // object
    static void vObjectClassFetch(NNP_Task nnpTask);
    static void vObjectClassStore(NNP_Task nnpTask);
    static void vObjectDump(NNP_Task nnpTask);
    static void vObjectFetch(NNP_Task nnpTask);
    static void vObjectMagicMark(NNP_Task nnpTask);
    static void vObjectStore(NNP_Task nnpTask);
    static void vObjectWordBodySet(NNP_Task nnpTask);
    
    // process
    static void vProcessGetPid(NNP_Task nnpTask);
    static void vProcessQuit(NNP_Task nnpTask);
    static void vProcessTestPid(NNP_Task nnpTask);
    
    // pstack
    static void vPstackDepth(NNP_Task nnpTask);
    static void vPstackDrop(NNP_Task nnpTask);
    static void vPstackDropTwo(NNP_Task nnpTask);
    static void vPstackDump(NNP_Task nnpTask);
    static void vPstackDup(NNP_Task nnpTask);
    static void vPstackDupNonZero(NNP_Task nnpTask);
    static void vPstackNip(NNP_Task nnpTask);
    static void vPstackPick(NNP_Task nnpTask);
    static void vPstackReverse(NNP_Task nnpTask);
    static void vPstackRoll(NNP_Task nnpTask);
    static void vPstackRollInv(NNP_Task nnpTask);
    static void vPstackRot(NNP_Task nnpTask);
    static void vPstackRotInv(NNP_Task nnpTask);
    static void vPstackSwap(NNP_Task nnpTask);
    static void vPstackToValSet(NNP_Task nnpTask);
    static void vPstackToValues(NNP_Task nnpTask);
    static void vPstackTuck(NNP_Task nnpTask);
    
    // push
    static void vPushI(NNP_Task nnpTask);
    static void vPushJ(NNP_Task nnpTask);
    static void vPushNullPtr(NNP_Task nnpTask);
    static void vPushValue(NNP_Task nnpTask);
    
    // queue
    static void vQueueAppend(NNP_Task nnpTask);
    static void vQueueDump(NNP_Task nnpTask);
    static void vQueueInit(NNP_Task nnpTask);     
    static void vQueuePop(NNP_Task nnpTask);      

    // registers
    static void vRegPop(NNP_Task nnpTask);
    static void vRegPush(NNP_Task nnpTask);
    static void vRegReg0(NNP_Task nnpTask);
    static void vRegReg1(NNP_Task nnpTask);
    static void vRegReg2(NNP_Task nnpTask);
    static void vRegReg3(NNP_Task nnpTask);
    static void vRegReg4(NNP_Task nnpTask);
    static void vRegReg5(NNP_Task nnpTask);
    static void vRegReg6(NNP_Task nnpTask);
    static void vRegReg7(NNP_Task nnpTask);
    static void vRegReg8(NNP_Task nnpTask);
    static void vRegReg9(NNP_Task nnpTask);
    static void vRegRegA(NNP_Task nnpTask);
    static void vRegRegB(NNP_Task nnpTask);
    static void vRegRegC(NNP_Task nnpTask);
    static void vRegRegD(NNP_Task nnpTask);
    static void vRegRegE(NNP_Task nnpTask);
    static void vRegRegF(NNP_Task nnpTask);
    static void vRegRegNth(NNP_Task nnpTask);
    static void vRegToReg0(NNP_Task nnpTask);
    static void vRegToReg1(NNP_Task nnpTask);
    static void vRegToReg2(NNP_Task nnpTask);
    static void vRegToReg3(NNP_Task nnpTask);
    static void vRegToReg4(NNP_Task nnpTask);
    static void vRegToReg5(NNP_Task nnpTask);
    static void vRegToReg6(NNP_Task nnpTask);
    static void vRegToReg7(NNP_Task nnpTask);
    static void vRegToReg8(NNP_Task nnpTask);
    static void vRegToReg9(NNP_Task nnpTask);
    static void vRegToRegA(NNP_Task nnpTask);
    static void vRegToRegB(NNP_Task nnpTask);
    static void vRegToRegC(NNP_Task nnpTask);
    static void vRegToRegD(NNP_Task nnpTask);
    static void vRegToRegE(NNP_Task nnpTask);
    static void vRegToRegF(NNP_Task nnpTask);
    static void vRegToRegNth(NNP_Task nnpTask);
    
    // rowBuffer
    static void vRowBufferAppend(NNP_Task nnpTask);
    static void vRowBufferAppendRecord(NNP_Task nnpTask);
    static void vRowBufferDump(NNP_Task nnpTask);
    static void vRowBufferNew(NNP_Task nnpTask);
    
    // rowCursor
    static void vRowCursorBegin(NNP_Task nnpTask);
    static void vRowCursorDelete(NNP_Task nnpTask);
    static void vRowCursorEnd(NNP_Task nnpTask);
    static void vRowCursorFirst(NNP_Task nnpTask);
    static void vRowCursorGet(NNP_Task nnpTask);
    static void vRowCursorInit(NNP_Task nnpTask);
    static void vRowCursorNew(NNP_Task nnpTask);
    static void vRowCursorNext(NNP_Task nnpTask);
    static void vRowCursorRowBuffer(NNP_Task nnpTask);
    static void vRowCursorTableFetch(NNP_Task nnpTask);
    static void vRowCursorTableStore(NNP_Task nnpTask);
    
    // rstack
    static void vRstackDepth(NNP_Task nnpTask);
    static void vRstackDrop(NNP_Task nnpTask);
    static void vRstackFetch(NNP_Task nnpTask);
    static void vRstackPick(NNP_Task nnpTask);
    static void vRstackReverse(NNP_Task nnpTask);
    static void vRstackToP(NNP_Task nnpTask);
    static void vRstackToR(NNP_Task nnpTask);
    static void vRstack0(NNP_Task nnpTask);
    static void vRstack1(NNP_Task nnpTask);
    static void vRstack2(NNP_Task nnpTask);
    static void vRstack3(NNP_Task nnpTask);
    static void vRstack4(NNP_Task nnpTask);
    static void vRstack5(NNP_Task nnpTask);
    static void vRstack6(NNP_Task nnpTask);
    static void vRstack7(NNP_Task nnpTask);
    static void vRstack8(NNP_Task nnpTask);
    static void vRstack9(NNP_Task nnpTask);
    
    // seitf
    static void vSEitfCount(NNP_Task nnpTask);
    static void vSEitfDelete(NNP_Task nnpTask);
    static void vSEitfDump(NNP_Task nnpTask);
    static void vSEitfNew(NNP_Task nnpTask);
    static void vSEitfReqFetch(NNP_Task nnpTask);
    static void vSEitfReqStore(NNP_Task nnpTask);
    static void vSEitfRspFetch(NNP_Task nnpTask);
    static void vSEitfRspStore(NNP_Task nnpTask);
    static void vSEitfTableFetch(NNP_Task nnpTask);
    static void vSEitfTableRef(NNP_Task nnpTask);
    static void vSEitfTableDeref(NNP_Task nnpTask);

    // seitfCursor
    static void vSEitfCursorBegin(NNP_Task nnpTask);
    static void vSEitfCursorDelete(NNP_Task nnpTask);
    static void vSEitfCursorEnd(NNP_Task nnpTask);
    static void vSEitfCursorFirst(NNP_Task nnpTask);
    static void vSEitfCursorGet(NNP_Task nnpTask);
    static void vSEitfCursorInit(NNP_Task nnpTask);
    static void vSEitfCursorNew(NNP_Task nnpTask);
    static void vSEitfCursorNext(NNP_Task nnpTask);
    
    // seitfList
    static void vSEitfListDump(NNP_Task nnpTask);
    static void vSEitfListInit(NNP_Task nnpTask);
    
    // shell
    static void vShellExecute(NNP_Task nnpTask);
    
    // space
    static void vFreeBuffers(NNP_Task nnpTask);
    static void vFreePercent(NNP_Task nnpTask);
    static void vFreeSpace(NNP_Task nnpTask);
    static void vInUseSpace(NNP_Task nnpTask);
    static void vTotalSpace(NNP_Task nnpTask);
    
    // stack
    static void vAstackClass(NNP_Task nnpTask);
    static void vStackClass(NNP_Task nnpTask);                 //! DUP OF class@
    
    // steps
    static void vStepsList(NNP_Task nnpTask);
    static void vStepsReset(NNP_Task nnpTask);
    
    // string
    static void vStringPad(NNP_Task nnpTask);
    static void vStrEql(NNP_Task nnpTask);
    static void vStrLen(NNP_Task nnpTask);
    static void vSubStr(NNP_Task nnpTask);
    static void vConcat(NNP_Task nnpTask);
    static void vClip(NNP_Task nnpTask);
    static void vPrepend(NNP_Task nnpTask);
    static void vRndStr(NNP_Task nnpTask);
    static void vSprint(NNP_Task nnpTask);
    
    static void vS256Init(NNP_Task nnpTask);
    
    // table
    static void vTableAppend(NNP_Task nnpTask);
    static void vTableAppendBuffer(NNP_Task nnpTask);
    static void vTableDic(NNP_Task nnpTask);
    static void vTableDrop(NNP_Task nnpTask);
    static void vTableDump(NNP_Task nnpTask);
    static void vTableFirstBuffer(NNP_Task nnpTask);
    static void vTableInit(NNP_Task nnpTask);
    static void vTableInsert(NNP_Task nnpTask);
    static void vTableList(NNP_Task nnpTask);
    static void vTableName(NNP_Task nnpTask);
    static void vTableRef(NNP_Task nnpTask);
    static void vTableSpace(NNP_Task nnpTask);
    static void vTableTruncate(NNP_Task nnpTask);
    
    // task
    static void vTaskAcksDec(NNP_Task nnpTask);
    static void vTaskAcksFetch(NNP_Task nnpTask);
    static void vTaskAcksStore(NNP_Task nnpTask);
    static void vTaskCommentFetch(NNP_Task nnpTask);
    static void vTaskCommentStore(NNP_Task nnpTask);
    static void vTaskDic(NNP_Task nnpTask);
    static void vTaskDump(NNP_Task nnpTask);
    static void vTaskInit(NNP_Task nnpTask);
    static void vTaskNameFetch(NNP_Task nnpTask);
    static void vTaskNameStore(NNP_Task nnpTask);
    static void vTaskNew(NNP_Task nnpTask);
    static void vTaskThis(NNP_Task nnpTask);
    static void vTaskParentFetch(NNP_Task nnpTask);
    static void vTaskParentStore(NNP_Task nnpTask);
    static void vTaskQfetch(NNP_Task nnpTask);
    static void vTaskQstore(NNP_Task nnpTask);
    static void vTaskSeed(NNP_Task nnpTask);
    static void vTaskSeitfFetch(NNP_Task nnpTask);
    static void vTaskSeitfStore(NNP_Task nnpTask);
    static void vTaskSignal(NNP_Task nnpTask);
    static void vTaskSocketFetch(NNP_Task nnpTask);
    static void vTaskSocketStore(NNP_Task nnpTask);
    static void vTaskStateFetch(NNP_Task nnpTask);
    static void vTaskStateStore(NNP_Task nnpTask);
    static void vTaskSuspend(NNP_Task nnpTask);
    static void vTaskTaskSetFetch(NNP_Task nnpTask);
    static void vTaskTaskSetStore(NNP_Task nnpTask);
    static void vTaskThread(NNP_Task nnpTask);
    static void vTaskWait(NNP_Task nnpTask);
    static void vTaskYield(NNP_Task nnpTask);
    
    // taskCursor
    static void vTaskCursorBegin(NNP_Task nnpTask);
    static void vTaskCursorDelete(NNP_Task nnpTask);
    static void vTaskCursorEnd(NNP_Task nnpTask);
    static void vTaskCursorFirst(NNP_Task nnpTask);
    static void vTaskCursorGet(NNP_Task nnpTask);
    static void vTaskCursorInit(NNP_Task nnpTask);
    static void vTaskCursorNew(NNP_Task nnpTask);
    static void vTaskCursorNext(NNP_Task nnpTask);
    
    // taskList
    static void vTaskList(NNP_Task nnpTask);
    static void vTaskListAppend(NNP_Task nnpTask);
    static void vTaskListDump(NNP_Task nnpTask);
    static void vTaskListInit(NNP_Task nnpTask);
    
    // taskSet
    static void vTaskSetAppend(NNP_Task nnpTask); 
    static void vTaskSetDump(NNP_Task nnpTask);
    static void vTaskSetInit(NNP_Task nnpTask);   
    
    // thread
    static void vThreadAppend(NNP_Task nnpTask);  
    static void vThreadBind(NNP_Task nnpTask);
    static void vThreadCondSignal(NNP_Task nnpTask);
    static void vThreadCondWait(NNP_Task nnpTask);
    static void vThreadCpu(NNP_Task nnpTask);
    static void vThreadDump(NNP_Task nnpTask);
    static void vThreadId(NNP_Task nnpTask);
    static void vThreadInit(NNP_Task nnpTask);
    static void vThreadMutexLock(NNP_Task nnpTask);
    static void vThreadMutexUnlock(NNP_Task nnpTask);
    static void vThreadNameFetch(NNP_Task nnpTask);
    static void vThreadNameStore(NNP_Task nnpTask);
    static void vThreadSfetch(NNP_Task nnpTask);
    static void vThreadSstore(NNP_Task nnpTask);
    static void vThreadSleep_s(NNP_Task nnpTask);
    static void vThreadSleep_ms(NNP_Task nnpTask);
    static void vThreadSleep_us(NNP_Task nnpTask);
    static void vThreadSleep_ns(NNP_Task nnpTask);
    static void vThreadSpawn(NNP_Task nnpTask);   

    // threadCursor
    static void vThreadCursorBegin(NNP_Task nnpTask);
    static void vThreadCursorDelete(NNP_Task nnpTask);
    static void vThreadCursorEnd(NNP_Task nnpTask);
    static void vThreadCursorFirst(NNP_Task nnpTask);
    static void vThreadCursorGet(NNP_Task nnpTask);
    static void vThreadCursorInit(NNP_Task nnpTask);
    static void vThreadCursorNew(NNP_Task nnpTask);
    static void vThreadCursorNext(NNP_Task nnpTask);
    
    // threadList
    static void vThreadListDump(NNP_Task nnpTask);
    static void vThreadListInit(NNP_Task nnpTask);
    
    // timer
    static void vTimerDelete(NNP_Task nnpTask);
    static void vTimerInit(NNP_Task nnpTask);
    static void vTimerMicrosecs(NNP_Task nnpTask);
    static void vTimerMillisecs(NNP_Task nnpTask);
    static void vTimerNanosecs(NNP_Task nnpTask);
    static void vTimerNew(NNP_Task nnpTask);
    static void vTimerReset(NNP_Task nnpTask);
    static void vTimerSecs(NNP_Task nnpTask);
    static void vTimerStart(NNP_Task nnpTask);
    static void vTimerTicks(NNP_Task nnpTask);
    static void vTimerUpdate(NNP_Task nnpTask);
    
    // to
    static void vToBody(NNP_Task nnpTask);
    static void vToCmd(NNP_Task nnpTask);
    static void vToDigitName(NNP_Task nnpTask);
    static void vToInt(NNP_Task nnpTask);
    static void vToSEitf(NNP_Task nnpTask);
    static void vToStr(NNP_Task nnpTask);
    static void vToTask(NNP_Task nnpTask);
    
    // trace
    static void vTraceOff(NNP_Task nnpTask);
    static void vTraceOn(NNP_Task nnpTask);
    static void vTracePoint(NNP_Task nnpTask);
    static void vTraceSet(NNP_Task nnpTask);
    // tx
    static void vTxDump(NNP_Task nnpTask);
    
    // txCollection
    static void vTxCollectionDump(NNP_Task nnpTask);
    static void vTxCollectionInit(NNP_Task nnpTask);
    
    // txCursor
    static void vTxCursorBegin(NNP_Task nnpTask);
    static void vTxCursorDelete(NNP_Task nnpTask);
    static void vTxCursorEnd(NNP_Task nnpTask);
    static void vTxCursorFirst(NNP_Task nnpTask);
    static void vTxCursorGet(NNP_Task nnpTask);
    static void vTxCursorInit(NNP_Task nnpTask);
    static void vTxCursorNew(NNP_Task nnpTask);
    static void vTxCursorNext(NNP_Task nnpTask);
    
    // txMap
    static void vTxMapInit(NNP_Task nnpTask);
    static void vTxRefBegin(NNP_Task nnpTask);
    static void vTxRefCommit(NNP_Task nnpTask);
    static void vTxRefRollback(NNP_Task nnpTask);
    static void vTxRefEnd(NNP_Task nnpTask);

    // txRef
    static void vTxRefInit(NNP_Task nnpTask);
    
    // word
    static void vCreate(NNP_Task nnpTask);
    static void vCreateFrom(NNP_Task nnpTask);
    static void vDoes(NNP_Task nnpTask);
    static void vDoes_RT(NNP_Task nnpTask);
    static void vFind(NNP_Task nnpTask);
    static void vFindInCurrent(NNP_Task nnpTask);
    static void vForget(NNP_Task nnpTask);
    static void vImmediate(NNP_Task nnpTask);
    static void vWord(NNP_Task nnpTask);
    static void vWordDic(NNP_Task nnpTask);
    static void vWordDump(NNP_Task nnpTask);
    static void vWordForget(NNP_Task nnpTask);
    static void vWordIsImmediate(NNP_Task nnpTask);
    static void vWordIsPrimitive(NNP_Task nnpTask);
    static void vWordKindGet(NNP_Task nnpTask);
    static void vWordKindSet(NNP_Task nnpTask);
    static void vWordName(NNP_Task nnpTask);
    static void vWordPrev(NNP_Task nnpTask);
    static void vWordThis(NNP_Task nnpTask);
    static void vWordTypeGet(NNP_Task nnpTask);
    static void vWordTypeSet(NNP_Task nnpTask);

    // zero
    static void vZeroLess(NNP_Task nnpTask);
    static void vZeroLessEqual(NNP_Task nnpTask);
    static void vZeroEqual(NNP_Task nnpTask);
    static void vZeroNotEqual(NNP_Task nnpTask);
    static void vZeroGreaterEqual(NNP_Task nnpTask);
    static void vZeroGreater(NNP_Task nnpTask);
    
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    
    // socket
    static void vSocketAccept(NNP_Task nnpTask);   
    static void vSocketClose(NNP_Task nnpTask);    
    static void vSocketConnect(NNP_Task nnpTask);  
    static void vSocketListen(NNP_Task nnpTask);   
    static void vSocketOpen(NNP_Task nnpTask);     
    static void vSocketRecv(NNP_Task nnpTask);     
    static void vSocketSend(NNP_Task nnpTask);
    
    // utilities
    static void vBytesMagnitude(NNP_Task nnpTask);
    
    Footer* pFooter(void);
  };
  
  Task* _pTask(void* pAddr);
  Task* _pTask(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
