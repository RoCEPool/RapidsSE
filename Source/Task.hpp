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
  /* 1 */ TS_BLOCKED,       // task is blocked by another transaction
  /* 2 */ TS_EXITED,        // task has completed
  /* 3 */ TS_RUNNING,       // task is currenty being executed by Thread::vLoop
  /* 4 */ TS_SLEEPING,      // task is sleeping (not yet used)
  /* 5 */ TS_WAITING_ACK,   // task is waiting on ack from cooperating task
  /* 6 */ TS_WAITING_SIGNAL,// task is waiting on signal indicating queue arrival
  /* 7 */ TS_YIELDED,       // task is yielded, not running and is in a TaskSet
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
  private:
    
    static const i8 C_TRACE_STEPS    = 0x1;
    static const i8 C_TRACE_ROUTINES = 0x2;

    Lock    m_xLock;
    i8      m_iTaskNo;     // task
    s32     m_sName;       // task name
    s32     m_sComment;    // comment
    eTS     m_eState;      // state (running, sleeping, ...)
    i8      m_iTxId;       // transaction id
    i8      m_iWakeAt;     // time at which to wake this tx
    Task*   m_pParentTask; // parent of this task
    Atomic  m_zAcks;       // number of acks still waiting for
    Threads::Thread* m_pThread; // current thread

    TaskSet* m_pTaskSet;   // which set this task belongs to
    NP_Task m_npNextInSet; // next task in set
    NP_Task m_npPrevInSet; // prev task in set
    
    Stack*  m_pDataStack;   // data stack
    Stack*  m_pReturnStack; // return stack
    Stack*  m_pAuxStack;    // auxiliary stack
    DicPath m_xPathStack;   // path stack
    
    RegFrame* m_pRegFrame;    // register frame chain
    RegFrame* m_pLocalsFrame; // register frame chain

    s256       m_sLambda;    // lambda input
    Tokeniser* m_pTokeniser; // input stream tokenizer
    
    union {
      Cell*   m_pCell;      // points to cell inside a word
      Value*  m_pValue;     // points to an embedded value within a word
      Object* m_pObject;    // points to an embedded object within a word
      Header* m_pHeader;    // points to an embbeded header within a word
    } IP;
    TaskStep* m_pEntryCell; // points to does arg (entry cell of word)
    
    
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

    Interface::SEitf* m_pSEitf; // the seitf for this task
    Queue*  m_pJobQueue;        // job queue that this task serves
                                //   may or may not be shared
    Socket* m_pSocket;          // socket associated with task
    
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
    void vAddMunged(NNP_Task nnpTask);
    void vAddPrimitive(const char* aName, TaskStep pStep, bool bImmediate);
    void vAddRoutines(void);
    Object* pArgObject(void);
    Word* pCompileColon(s& sDefinition);
    Word* pCompileConstant(s& sDefinition);
    static void vDebug(void);
    void vContentDump(void);
    void vException(s& sFile, s& eException);
    void vInitialiseMain(void);
    bool  bIsName(s& sName);
    void  vLocalsPop(void);
    void  vLocalsPush(void);
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
    void vSprint(s48K& sFormat, s48K& sString);
    void vStackContents(Stack* pStack, s& sStackContents);
    eTS  eState(void);
    void vState(eTS eState);
    Threads::Thread* pThread(void);
    void vThread(Threads::Thread* pThread);
    void vTracePoint(const char* aMark);
    i8   iTxId(void);
    void vTxId(i8 iTxId);
    
    
    void vAddPrimitives(void);
    void vAddPrimitives_Address(void);
    void vAddPrimitives_Args(void);
    void vAddPrimitives_Assert(void);
    void vAddPrimitives_Astack(void);
    void vAddPrimitives_Bp(void);
    void vAddPrimitives_Cast(void);
    void vAddPrimitives_Class(void);
    void vAddPrimitives_Comment(void);
    void vAddPrimitives_Cpu(void);
    void vAddPrimitives_Cursor(void);
    void vAddPrimitives_DdlLock(void);
    void vAddPrimitives_Debug(void);
    void vAddPrimitives_Dic(void);
    void vAddPrimitives_DicLock(void);
    void vAddPrimitives_DicStack(void);
    void vAddPrimitives_Dir(void);
    void vAddPrimitives_Docol(void);
    void vAddPrimitives_Dot(void);
    void vAddPrimitives_Dstack(void);
    void vAddPrimitives_Encode(void);
    void vAddPrimitives_Enter(void);
    void vAddPrimitives_Env(void);
    void vAddPrimitives_Error(void);
    void vAddPrimitives_Exit(void);
    void vAddPrimitives_File(void);
    void vAddPrimitives_Flow(void);
    void vAddPrimitives_Grid(void);
    void vAddPrimitives_Input(void);
    void vAddPrimitives_Int(void);
    void vAddPrimitives_Loader(void);
    void vAddPrimitives_LoaderSub(void);
    void vAddPrimitives_Local(void);
    void vAddPrimitives_Lock(void);
    void vAddPrimitives_Logic(void);
    void vAddPrimitives_Magnitude(void);
    void vAddPrimitives_Memory(void);
    void vAddPrimitives_Node(void);
    void vAddPrimitives_NodeCursor(void);
    void vAddPrimitives_Object(void);
    void vAddPrimitives_Plan(void);
    void vAddPrimitives_PlanList(void);
    void vAddPrimitives_Process(void);
    void vAddPrimitives_Push(void);
    void vAddPrimitives_Queue(void);
    void vAddPrimitives_RbCursor(void);
    void vAddPrimitives_Reg(void);
    void vAddPrimitives_RowBuffer(void);
    void vAddPrimitives_RowCursor(void);
    void vAddPrimitives_Rstack(void);
    void vAddPrimitives_SEitf(void);
    void vAddPrimitives_SEitfCursor(void);
    void vAddPrimitives_SEitfList(void);
    void vAddPrimitives_Shell(void);
    void vAddPrimitives_Socket(void);
    void vAddPrimitives_Steps(void);
    void vAddPrimitives_Str(void);
    void vAddPrimitives_Table(void);
    void vAddPrimitives_Task(void);
    void vAddPrimitives_TaskCursor(void);
    void vAddPrimitives_TaskList(void);
    void vAddPrimitives_TaskSet(void);
    void vAddPrimitives_Thread(void);
    void vAddPrimitives_ThreadCursor(void);
    void vAddPrimitives_ThreadList(void);
    void vAddPrimitives_Timer(void);
    void vAddPrimitives_To(void);
    void vAddPrimitives_Trace(void);
    void vAddPrimitives_Tx(void);
    void vAddPrimitives_TxCollection(void);
    void vAddPrimitives_TxCursor(void);
    void vAddPrimitives_TxMap(void);
    void vAddPrimitives_TxRef(void);
    void vAddPrimitives_Vector(void);
    void vAddPrimitives_Word(void);
    void vAddPrimitives_Zero(void);

    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/

    // address
    static void vAddressEqual(NNP_Task nnpTask);
    static void vAddressFetch(NNP_Task nnpTask);
    static void vAddressStore(NNP_Task nnpTask);
    
    // args
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
    static void vAstackClass(NNP_Task nnpTask);
    static void vAstackDepth(NNP_Task nnpTask);
    static void vAstackDrop(NNP_Task nnpTask);
    static void vAstackDup(NNP_Task nnpTask);
    static void vAstackFetch(NNP_Task nnpTask);
    static void vAstackNip(NNP_Task nnpTask);
    static void vAstackPick(NNP_Task nnpTask);
    static void vAstackReverse(NNP_Task nnpTask);
    static void vAstackStk(NNP_Task nnpTask);
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
    
    // bp
    static void vBpAnalyze(NNP_Task nnpTask);
    static void vBpDump(NNP_Task nnpTask);
    static void vFreeBuffers(NNP_Task nnpTask);
    static void vFreePercent(NNP_Task nnpTask);
    static void vFreeSpace(NNP_Task nnpTask);
    static void vInUseSpace(NNP_Task nnpTask);
    static void vSnapshot(NNP_Task nnpTask);
    static void vTotalSpace(NNP_Task nnpTask);
    
    // cast
    static void vCastInt(NNP_Task nnpTask);
    static void vCastPtr(NNP_Task nnpTask);
    
    // class
    static void vClassName(NNP_Task nnpTask);

    // comment
    static void vComment(NNP_Task nnpTask);
    
    // cpu
    static void vCpuRdtscp(NNP_Task nnpTask);
    
    // cursor
    static void vCursorFetch(NNP_Task nnpTask);
    static void vCursorStore(NNP_Task nnpTask);
    
    
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
    static void vDicFrozenGet(NNP_Task nnpTask);
    static void vDicFrozenSet(NNP_Task nnpTask);
    static void vDicInit(NNP_Task nnpTask);
    static void vDicIsCompiled(NNP_Task nnpTask);
    static void vDicKindGet(NNP_Task nnpTask);
    static void vDicKindSet(NNP_Task nnpTask);
    static void vDicLastWord(NNP_Task nnpTask);
    static void vDicMain(NNP_Task nnpTask);
    static void vDicName(NNP_Task nnpTask);
    static void vDicRef(NNP_Task nnpTask);
    static void vDicWordCount(NNP_Task nnpTask);
    
    // dicPath
    static void vDicPathDepth(NNP_Task nnpTask);
    static void vDicPathDrop(NNP_Task nnpTask);
    static void vDicPathDump(NNP_Task nnpTask);
    static void vDicPathPick(NNP_Task nnpTask);
    static void vDicPathPush(NNP_Task nnpTask);

    // dir
    static void vDirChangeCurrent(NNP_Task nnpTask);
    static void vDirGetCurrent(NNP_Task nnpTask);
    static void vDirExists(NNP_Task nnpTask);
    static void vDirMake(NNP_Task nnpTask);
    static void vDirRemove(NNP_Task nnpTask);
    
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
    
    // dstack
    static void vDstackClass(NNP_Task nnpTask);                 //! DUP OF class@
    static void vDstackDepth(NNP_Task nnpTask);
    static void vDstackDrop(NNP_Task nnpTask);
    static void vDstackDropTwo(NNP_Task nnpTask);
    static void vDstackDump(NNP_Task nnpTask);
    static void vDstackDup(NNP_Task nnpTask);
    static void vDstackDupNonZero(NNP_Task nnpTask);
    static void vDstackNip(NNP_Task nnpTask);
    static void vDstackOver(NNP_Task nnpTask);
    static void vDstackPick(NNP_Task nnpTask);
    static void vDstackReverse(NNP_Task nnpTask);
    static void vDstackRoll(NNP_Task nnpTask);
    static void vDstackRollInv(NNP_Task nnpTask);
    static void vDstackRot(NNP_Task nnpTask);
    static void vDstackRotInv(NNP_Task nnpTask);
    static void vDstackStk(NNP_Task nnpTask);
    static void vDstackSwap(NNP_Task nnpTask);
    static void vDstackToValSet(NNP_Task nnpTask);
    static void vDstackToValues(NNP_Task nnpTask);
    static void vDstackTuck(NNP_Task nnpTask);
    
    // encode
    static void vEncodeAgain(NNP_Task nnpTask);
    static void vEncodeAhead(NNP_Task nnpTask);
    static void vEncodeCallPrimitive(NNP_Task nnpTask);
    static void vEncodeCallRoutine(NNP_Task nnpTask);
    static void vEncodeContinue(NNP_Task nnpTask);
    static void vEncodeHere(NNP_Task nnpTask);
    static void vEncodeIf(NNP_Task nnpTask);
    static void vEncodeLoop(NNP_Task nnpTask);
    static void vEncodeLoopPlus(NNP_Task nnpTask);
    static void vEncodeNext(NNP_Task nnpTask);
    static void vEncodePushValue(NNP_Task nnpTask);
    static void vEncodeThen(NNP_Task nnpTask);
    static void vEncodeUntil(NNP_Task nnpTask);
    
    // enter
    static void vEnterObject(NNP_Task nnpTask);
    static void vEnterRoutine(NNP_Task nnpTask);
    
    // env
    static void vEnvGet(NNP_Task nnpTask);
    
    // errors
    static void vErrorMsg(NNP_Task nnpTask);
    
    // exit
    static void vExit(NNP_Task nnpTask);
    
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
    static void vFlowBegin_(NNP_Task nnpTask);
    static void vFlowExecute(NNP_Task nnpTask);
    static void vFlowHazard(NNP_Task nnpTask);
    static void vFlowJump(NNP_Task nnpTask);
    static void vFlowJumpBegin_(NNP_Task nnpTask);
    static void vFlowJumpFalse(NNP_Task nnpTask);
    static void vFlowJumpTrue(NNP_Task nnpTask);
    static void vFlowNop(NNP_Task nnpTask);
    static void vFlowRetryPoint(NNP_Task nnpTask);
    static void vFlowTest(NNP_Task nnpTask);
    static void vFlowThrow(NNP_Task nnpTask);
    
    // grid    
    static void vGridAppendNodes(NNP_Task nnpTask);
    static void vGridInit(NNP_Task nnpTask);
    static void vGridNode(NNP_Task nnpTask);
    static void vGridThisNodeStore(NNP_Task nnpTask);
    static void vGridThisNodeFetch(NNP_Task nnpTask);
    static void vGridStartedStore(NNP_Task nnpTask);
    static void vGridStartedFetch(NNP_Task nnpTask);
    static void vGridStartNodeStore(NNP_Task nnpTask);
    static void vGridStartNodeFetch(NNP_Task nnpTask);
    
    // input
    static void vInputAccept(NNP_Task nnpTask);
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
    static void vIntHash(NNP_Task nnpTask);
    static void vIntLess(NNP_Task nnpTask);
    static void vIntLessEqual(NNP_Task nnpTask);
    static void vIntModulo(NNP_Task nnpTask);
    static void vIntMultiply(NNP_Task nnpTask);
    static void vIntNeg(NNP_Task nnpTask);
    static void vIntNotEqual(NNP_Task nnpTask);
    static void vIntSubtract(NNP_Task nnpTask);
    
    // loader
    static void vLoaderDelete(NNP_Task nnpTask);
    static void vLoaderFinalize(NNP_Task nnpTask);
    static void vLoaderFindChunk(NNP_Task nnpTask);
    static void vLoaderInit(NNP_Task nnpTask);
    static void vLoaderNew(NNP_Task nnpTask);
    static void vLoaderNewSub(NNP_Task nnpTask);
    static void vLoaderPathStore(NNP_Task nnpTask);
    static void vLoaderTableStore(NNP_Task nnpTask);
    static void vLoaderTxRefStore(NNP_Task nnpTask);
    
    // loader sub
    static void vLoaderSubAppendBuffer(NNP_Task nnpTask);
    static void vLoaderSubBegin(NNP_Task nnpTask);
    static void vLoaderSubDelete(NNP_Task nnpTask);
    static void vLoaderSubEnd(NNP_Task nnpTask);
    static void vLoaderSubFetchRecord(NNP_Task nnpTask);
    static void vLoaderSubHashPartition(NNP_Task nnpTask);
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
    
    // magnitude
    static void vBytesMagnitude(NNP_Task nnpTask);
    static void vNumberMagnitude(NNP_Task nnpTask);
    static void vToDigitName(NNP_Task nnpTask);
    
    // memory
    static void vMemoryByteDump(NNP_Task nnpTask);
    static void vMemoryWordDump(NNP_Task nnpTask);
    
    // node (of grid)
    static void vNodeInit(NNP_Task nnpTask);
    static void vNodeIP(NNP_Task nnpTask);
    static void vNodeName(NNP_Task nnpTask);
    static void vNodeNew(NNP_Task nnpTask);
    static void vNodeNo(NNP_Task nnpTask);
    static void vNodePort(NNP_Task nnpTask);
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
    
    // object
    static void vObjectClassFetch(NNP_Task nnpTask);
    static void vObjectClassStore(NNP_Task nnpTask);
    static void vObjectDump(NNP_Task nnpTask);
    static void vObjectFetch(NNP_Task nnpTask);
    static void vObjectMagicMark(NNP_Task nnpTask);
    static void vObjectStore(NNP_Task nnpTask);
    static void vObjectWordBodySet(NNP_Task nnpTask);
    
    // plan
    static void vPlanDelete(NNP_Task nnpTask);
    static void vPlanNew(NNP_Task nnpTask);
    static void vPlanNameFetch(NNP_Task nnpTask);
    static void vPlanNameStore(NNP_Task nnpTask);
    static void vPlanShow(NNP_Task nnpTask);
    
    // plan list
    static void vPlanListAddPlan(NNP_Task nnpTask);
    static void vPlanListFindPlan(NNP_Task nnpTask);
    static void vPlanListRemovePlan(NNP_Task nnpTask);
    static void vPlanListShow(NNP_Task nnpTask);
    
    // process
    static void vProcessGetPid(NNP_Task nnpTask);
    static void vProcessQuit(NNP_Task nnpTask);
    static void vProcessTestPid(NNP_Task nnpTask);
    
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

    // rbCursor
    static void vRbCursorBegin(NNP_Task nnpTask);
    static void vRbCursorDelete(NNP_Task nnpTask);
    static void vRbCursorEnd(NNP_Task nnpTask);
    static void vRbCursorFirst(NNP_Task nnpTask);
    static void vRbCursorGet(NNP_Task nnpTask);
    static void vRbCursorInit(NNP_Task nnpTask);
    static void vRbCursorNew(NNP_Task nnpTask);
    static void vRbCursorNext(NNP_Task nnpTask);
    static void vRbCursorRowBuffer(NNP_Task nnpTask);
    static void vRbCursorTableFetch(NNP_Task nnpTask);
    static void vRbCursorTableStore(NNP_Task nnpTask);
    
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
    static void vRstackCsPick(NNP_Task nnpTask);
    static void vRstackCsRoll(NNP_Task nnpTask);
    static void vRstackDepth(NNP_Task nnpTask);
    static void vRstackDrop(NNP_Task nnpTask);
    static void vRstackFetch(NNP_Task nnpTask);
    static void vRstackFetchTwo(NNP_Task nnpTask);
    static void vRstackPick(NNP_Task nnpTask);
    static void vRstackReverse(NNP_Task nnpTask);
    static void vRstackRoll(NNP_Task nnpTask);
    static void vRstackRollInv(NNP_Task nnpTask);
    static void vRstackStk(NNP_Task nnpTask);
    static void vRstackSwap(NNP_Task nnpTask);
    static void vRstackToP(NNP_Task nnpTask);
    static void vRstackToR(NNP_Task nnpTask);
    static void vRstackToRTwo(NNP_Task nnpTask);
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
    static void vSEitfDicStack(NNP_Task nnpTask);
    static void vSEitfDump(NNP_Task nnpTask);
    static void vSEitfPid(NNP_Task nnpTask);
    static void vSEitfReqFetch(NNP_Task nnpTask);
    static void vSEitfReqStore(NNP_Task nnpTask);
    static void vSEitfRspFetch(NNP_Task nnpTask);
    static void vSEitfRspStore(NNP_Task nnpTask);
    static void vSEitfResponseQueue(NNP_Task nnpTask);
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
    
    // socket
    static void vSocketAccept(NNP_Task nnpTask);
    static void vSocketClose(NNP_Task nnpTask);
    static void vSocketConnect(NNP_Task nnpTask);
    static void vSocketListen(NNP_Task nnpTask);
    static void vSocketOpen(NNP_Task nnpTask);
    static void vSocketRecv(NNP_Task nnpTask);
    static void vSocketSend(NNP_Task nnpTask);
    
    // steps
    static void vStepsList(NNP_Task nnpTask);
    static void vStepsReset(NNP_Task nnpTask);
    
    // string

    static void vStrCat(NNP_Task nnpTask);
    static void vStrClip(NNP_Task nnpTask);
    static void vStrEqual(NNP_Task nnpTask);
    static void vStrEscape(NNP_Task nnpTask);
    static void vStrFirstCsv(NNP_Task nnpTask);
    static void vStrFormat(NNP_Task nnpTask);
    static void vStrLeft(NNP_Task nnpTask);
    static void vStrLength(NNP_Task nnpTask);
    static void vStrPad(NNP_Task nnpTask);
    static void vStrPrepend(NNP_Task nnpTask);
    static void vStrRandom(NNP_Task nnpTask);
    static void vStrReplace(NNP_Task nnpTask);
    static void vStrRight(NNP_Task nnpTask);
    static void vStrSub(NNP_Task nnpTask);

    static void vS256Init(NNP_Task nnpTask);
    
    // table
    static void vTableAnalyze(NNP_Task nnpTask);
    static void vTableAppend(NNP_Task nnpTask);
    static void vTableAppendBuffer(NNP_Task nnpTask);
    static void vTableDrop(NNP_Task nnpTask);
    static void vTableDump(NNP_Task nnpTask);
    static void vTableFirstBuffer(NNP_Task nnpTask);
    static void vTableInit(NNP_Task nnpTask);
    static void vTableInsert(NNP_Task nnpTask);
    static void vTableList(NNP_Task nnpTask);
    static void vTableName(NNP_Task nnpTask);
    static void vTableRef(NNP_Task nnpTask);
    static void vTableSpace(NNP_Task nnpTask);
    static void vTableSubDic(NNP_Task nnpTask);
    static void vTableTruncate(NNP_Task nnpTask);
    
    // task
    static void vTaskAck(NNP_Task nnpTask);
    static void vTaskAcksFetch(NNP_Task nnpTask);
    static void vTaskAcksStore(NNP_Task nnpTask);
    static void vTaskCommentFetch(NNP_Task nnpTask);
    static void vTaskCommentStore(NNP_Task nnpTask);
    static void vTaskDicStack(NNP_Task nnpTask);
    static void vTaskDump(NNP_Task nnpTask);
    static void vTaskInit(NNP_Task nnpTask);
    static void vTaskJobQueueFetch(NNP_Task nnpTask);
    static void vTaskJobQueueStore(NNP_Task nnpTask);
    static void vTaskNameFetch(NNP_Task nnpTask);
    static void vTaskNameStore(NNP_Task nnpTask);
    static void vTaskNew(NNP_Task nnpTask);
    static void vTaskThis(NNP_Task nnpTask);
    static void vTaskParentFetch(NNP_Task nnpTask);
    static void vTaskParentStore(NNP_Task nnpTask);
    static void vTaskSeed(NNP_Task nnpTask);
    static void vTaskSeitfFetch(NNP_Task nnpTask);
    static void vTaskSeitfStore(NNP_Task nnpTask);
    static void vTaskSocketFetch(NNP_Task nnpTask);
    static void vTaskSocketStore(NNP_Task nnpTask);
    static void vTaskStateFetch(NNP_Task nnpTask);
    static void vTaskStateStore(NNP_Task nnpTask);
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
    static void vToBinVal(NNP_Task nnpTask);
    static void vToBody(NNP_Task nnpTask);
    static void vToCmd(NNP_Task nnpTask);
    static void vToInt(NNP_Task nnpTask);
    static void vToObject(NNP_Task nnpTask);
    static void vToSEitf(NNP_Task nnpTask);
    static void vToStr(NNP_Task nnpTask);
    static void vToTask(NNP_Task nnpTask);
    
    // token
    static void vTokenColon(NNP_Task nnpTask);
    
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
    
    // vector
    static void vVectorDifference(NNP_Task nnpTask);
    
    // word
    static void vAllParticipants(NNP_Task nnpTask);
    static void vAllot(NNP_Task nnpTask);
    static void vCell(NNP_Task nnpTask);
    static void vColon(NNP_Task nnpTask);
    static void vComma(NNP_Task nnpTask);
    static void vCompileComma(NNP_Task nnpTask);
    static void vCreate(NNP_Task nnpTask);
    static void vCreateFrom(NNP_Task nnpTask);
    static void vDoes(NNP_Task nnpTask);
    static void vDoes_RT(NNP_Task nnpTask);
    static void vFind(NNP_Task nnpTask);
    static void vFindInCurrent(NNP_Task nnpTask);
    static void vForget(NNP_Task nnpTask);
    static void vImmediate(NNP_Task nnpTask);
    static void vLocalCalls(NNP_Task nnpTask);
    static void vLocate(NNP_Task nnpTask);
    static void vOneParticipant(NNP_Task nnpTask);
    static void vPostpone(NNP_Task nnpTask);
    static void vSee(NNP_Task nnpTask);
    static void vSemicolon(NNP_Task nnpTask);
    static void vTick(NNP_Task nnpTask);
    static void vWord(NNP_Task nnpTask);
    static void vWordDump(NNP_Task nnpTask);
    static void vWordForget(NNP_Task nnpTask);
    static void vWordImmediateGet(NNP_Task nnpTask);
    static void vWordImmediateSet(NNP_Task nnpTask);
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
    
    Footer* pFooter(void);

    /*------------------------------------------------------------------------*/
    
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
        
    // throw invalid auxiliary stack
    #define THROW_INVALID_ASTACK(primitive) \
    { \
      THROW("Invalid aux stack for: %s",primitive); \
    }
        
    // throw invalid data stack
    #define THROW_INVALID_DSTACK(primitive) \
    { \
      THROW("Invalid data stack for: %s",primitive); \
    }
        
    // throw invalid return stack
    #define THROW_INVALID_RSTACK(primitive) \
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
  };
  
  Task* _pTask(void* pAddr);
  Task* _pTask(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
