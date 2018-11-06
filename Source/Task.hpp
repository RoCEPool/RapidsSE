/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once

extern pthread_key_t  giTLS_TaskKey;

namespace Interface
{
  class Session;
}

namespace GridModel
{
  class Node;
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
  /* 1 */ TS_EXECUTING, // task is currenty being executed by Thread::vLoop
  /* 2 */ TS_EXITED,    // task has completed
  /* 3 */ TS_READY,     // task is ready to be executed
  /* 4 */ TS_SEND,      // task is wants to send
  /* 5 */ TS_SLEEPING,  // task is sleeping
  /* 6 */ TS_WAITING,   // task is waiting on ack from cooperating task
  /* 7 */ TS_YIELDED,   // task is yielded, not running and is in a TaskSet
  /* 8 */ TS_ENUMS
  };
  
  static Stz* aTS[]
  {
    /* 0 */ "NONE"_Stz,
    /* 1 */ "EXECUTING"_Stz,
    /* 2 */ "EXITED"_Stz,
    /* 3 */ "READY"_Stz,
    /* 4 */ "SEND"_Stz,
    /* 5 */ "SLEEPING"_Stz,
    /* 6 */ "WAITING"_Stz,
    /* 7 */ "YIELDED"_Stz,
    /* 8 */ "ENUMS"_Stz,
  };
  
  typedef NP<Task,CN_TASK> NP_Task;
  typedef MP<Task,CN_TASK> MP_Task;
  
  class Task : public Object,
               public ListNode_t<Task>
  {
  private:
  
    // tracing masks
    static const i8 C_TRACE_NONE     = 0x0;
    static const i8 C_TRACE_STEPS    = 0x1;
    static const i8 C_TRACE_ROUTINES = 0x2;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // keep

    Job*    m_pJob;         // the job for this task

    Threads::Thread* m_pThread; // current thread
  
    eTS     m_eState;       // state (running, sleeping, ...)
    eQR     m_eQueue;       // which queue the task is in
    i8      m_iQueueOrder;  // queue ordering number
    Lock    m_xLock;        // lock for this task
    Decider m_xAcksRemaining;        // number of acks still waiting for
    Since   m_xWakeupTime;  // wakeup time

    i8      m_iPartition;   // which partition if a partition task
    i8      m_iDdlLocked;   // ? has the ddl lock?
    i8      m_iTracing;     // tracing mask

    GridModel::Node* m_pSendNode; // node to send to

    Stack*   m_pDataStack;   // data stack
    Stack*   m_pReturnStack; // return stack
    Stack*   m_pAuxStack;    // auxiliary stack
    DicStack m_xPathStack;   // dictionary stack
    
    RegFrame* m_pLocalsFrame; // local register frame chain
    RegFrame* m_pOpFrame;     // op frame chain

    union {                 // instruction pointer
      Cell*   m_pCell;      // points to cell inside a word
      Value*  m_pValue;     // points to an embedded value within a word
      Object* m_pObject;    // points to an embedded object within a word
    } IP;
    TaskStep* m_pEntryCell; // points to does arg (entry cell of word)
  
    // exception
    s256  m_sFile;
    s256  m_sException;

    Compiler* m_pCompiler;  // word compiler
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // can these be removed ?
  
    Word* m_pWord;
 
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // remove
  
    Interface::Session* m_pSession; // the session for this task
    Plan*   m_pPlan;                // the plan associated with task

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Task(Job* pJob);
    ~Task(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    bool bAckAndDecide(void);
    void vAdjustNext(i8 iBytes);
    Object* pArgObject(void);
    static void vDebug(void);
    void vContentDump(void);
    void vDropLock(void);
    void vException(StrVal& sFile, StrVal& eException);
    void vGrabLock(void);
    bool bIsLocked(void);
    bool bIsNonPartitioned(void);
    bool bIsPartitioned(void);
    Job* pJob(void);
    void vJob(Job* pJob);
    Stz* aJobName(void);
    void vLocalsPop(void);
    void vLocalsPush(void);
    void vNewStacks(void);
    i8   iPartition(void);
    void vPerformancePoint(Stz* aWordName, Stz* aPointNote);
    void vPerformancePrint(void);
    bool bPopInt(i8& iInt);
    bool bPopObject(Object*& pObject);
    bool bPopStr(s48K& sStr);
    void vPrimitive(Stz *aPrimitive);
    void vPushInt(i8 iInt);
    void vPushObject(Object* pObject);
    void vPushStr(Stz* aStr);
    void vPushStr(s48K& sStr);
    eQR  eQueue(void);
    void vQueue(eQR eQueue);
    void vRefindJobClass(void);
    void vRetry(void);
    bool bSeeded(void);
    void vSeedJob(void);
    GridModel::Node* pSendNode(void);
    Interface::Session* pSession(void);
    void vSession(Interface::Session* pSession);
    void vSleep(i8 iMicroSeconds);
    i8   iSlot(void);
    void vSprint(s48K& sFormat, s48K& sString);
    void vStackContents(Stack* pStack, StrVal& sStackContents);
    eTS  eState(void);
    void vState(eTS eState);
    void vTaskLoop(void);
    void vQueueOrder(i8 iTaskOrder);
    i8   iQueueOrder(void);
    Threads::Thread* pThread(void);
    void vThread(Threads::Thread* pThread);
    void vTracePoint(Stz* aMark);
    bool bWakeupNow(void);
    i8   iWakeupTime(void);
    void vWakeupTask(void);
    eQR  eWhichQueue(void);

    /*------------------------------------------------------------------------*/

    void vAddBaseWords(void);
    void vAddConstant(Stz* aName, i8 iVal);
    void vAddConstant(Stz* aName, Stz* aValue);
    void vAddConstants(void);
    void vAddMunged(MP_Task mpTask);
    static void vAddStep(Stz* aName, TaskStep pStep, bool bImmediate=false);
    void vAddSteps(void);
    void vAddRoutines(void);
    Word* pCompileColon(StrVal& sDefinition);
    void  vCompileConstant(StrVal& sDefinition);

    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/


    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------*/

    // access path
    static void vAccessPathTableFetch(MP_Task mpTask);
    static void vAccessPathTableStore(MP_Task mpTask);
 
    // accum
    static void vAccumDelete(MP_Task mpTask);
    static void vAccumNew(MP_Task mpTask);
  
    static void vAccumAccumBuffer(MP_Task mpTask);
    static void vAccumCountFetch(MP_Task mpTask);
    static void vAccumCountStore(MP_Task mpTask);
    static void vAccumGroupFetch(MP_Task mpTask);
    static void vAccumGroupStore(MP_Task mpTask);
    static void vAccumMaxFetch(MP_Task mpTask);
    static void vAccumMaxStore(MP_Task mpTask);
    static void vAccumMinFetch(MP_Task mpTask);
    static void vAccumMinStore(MP_Task mpTask);
    static void vAccumSumFetch(MP_Task mpTask);
    static void vAccumSumStore(MP_Task mpTask);
  
    // activity map
    static void vActivityMapDump(MP_Task mpTask);
    static void vActivityMapMark(MP_Task mpTask);
    static void vActivityMapMarkEntry(MP_Task mpTask);
    static void vActivityMapMarkExit(MP_Task mpTask);

    // address
    static void vAddressEqual(MP_Task mpTask);
    static void vAddressFetch(MP_Task mpTask);
    static void vAddressStore(MP_Task mpTask);
    
    // apiState
    static void vApiStateSetPosition(MP_Task mpTask);
  
    // args
    static void vArgsPrint(MP_Task mpTask);

    // assert
    static void vAssert(MP_Task mpTask);

    static void vAssertClass(MP_Task mpTask);
    static void vAssertClass_0(MP_Task mpTask);
    static void vAssertClass_1(MP_Task mpTask);
    static void vAssertClass_2(MP_Task mpTask);
    static void vAssertClass_3(MP_Task mpTask);
    static void vAssertClass_4(MP_Task mpTask);

    static void vAssertStack_0(MP_Task mpTask);
    static void vAssertStack_0more(MP_Task mpTask);
    static void vAssertStack_1(MP_Task mpTask);
    static void vAssertStack_1more(MP_Task mpTask);
    static void vAssertStack_2(MP_Task mpTask);
    static void vAssertStack_2more(MP_Task mpTask);
    static void vAssertStack_3(MP_Task mpTask);
    static void vAssertStack_3more(MP_Task mpTask);
    static void vAssertStack_4(MP_Task mpTask);
    static void vAssertStack_4more(MP_Task mpTask);
    
    // astack
    static void vAstackClass(MP_Task mpTask);
    static void vAstackDepth(MP_Task mpTask);
    static void vAstackDrop(MP_Task mpTask);
    static void vAstackDup(MP_Task mpTask);
    static void vAstackFetch(MP_Task mpTask);
    static void vAstackNip(MP_Task mpTask);
    static void vAstackPick(MP_Task mpTask);
    static void vAstackReverse(MP_Task mpTask);
    static void vAstackStk(MP_Task mpTask);
    static void vAstackSwap(MP_Task mpTask);
    static void vAstackToA(MP_Task mpTask);
    static void vAstackToP(MP_Task mpTask);
    static void vAstackToValSet(MP_Task mpTask);
    static void vAstackToValues(MP_Task mpTask);
    static void vAstackTransfer(MP_Task mpTask);
    static void vAstack0(MP_Task mpTask);
    static void vAstack1(MP_Task mpTask);
    static void vAstack2(MP_Task mpTask);
    static void vAstack3(MP_Task mpTask);
    static void vAstack4(MP_Task mpTask);
    static void vAstack5(MP_Task mpTask);
    static void vAstack6(MP_Task mpTask);
    static void vAstack7(MP_Task mpTask);
    static void vAstack8(MP_Task mpTask);
    static void vAstack9(MP_Task mpTask);
    
    // attachmap
    static void vAttachMapFetch(MP_Task mpTask);
    static void vAttachMapStore(MP_Task mpTask);
    static void vAttachMapTenantsFetch(MP_Task mpTask);
    
    // bp
    static void vBpAnalyze(MP_Task mpTask);
    static void vBpDump(MP_Task mpTask);
    static void vBpEpochDump(MP_Task mpTask);
    static void vBpEpochTake(MP_Task mpTask);
    static void vBpReadBufferPool(MP_Task mpTask);
    static void vBpWriteBufferPool(MP_Task mpTask);
    static void vFreeBuffers(MP_Task mpTask);
    static void vFreePercent(MP_Task mpTask);
    static void vFreeSpace(MP_Task mpTask);
    static void vInUseSpace(MP_Task mpTask);
    static void vSnapshot(MP_Task mpTask);
    static void vTotalSpace(MP_Task mpTask);
    
    // cast
    static void vCastInt(MP_Task mpTask);
    static void vCastPtr(MP_Task mpTask);
    
    // class
    static void vClassName(MP_Task mpTask);

    // column
    static void vColumnIsNullableFetch(MP_Task mpTask);
    static void vColumnIsPartitionKeyFetch(MP_Task mpTask);
    static void vColumnLengthFetch(MP_Task mpTask);
    static void vColumnNameFetch(MP_Task mpTask);
    static void vColumnScaleFetch(MP_Task mpTask);
    static void vColumnTypeFetch(MP_Task mpTask);
    
    // command processor
    static void vCommandProcessorBlankLine(MP_Task mpTask);
    static void vCommandProcessorDeleteJob(MP_Task mpTask);
    static void vCommandProcessorExecuteFile(MP_Task mpTask);
    static void vCommandProcessorFindJobClass(MP_Task mpTask);
    static void vCommandProcessorGetCommand(MP_Task mpTask);
    static void vCommandProcessorJobClass(MP_Task mpTask);
    static void vCommandProcessorPostJob(MP_Task mpTask);
    static void vCommandProcessorPreJob(MP_Task mpTask);
    static void vCommandProcessorSpawnJob(MP_Task mpTask);

   // comment
    static void vComment(MP_Task mpTask);
    
    // common
    static void vCommonFetch(MP_Task mpTask);
    static void vCommonStore(MP_Task mpTask);
    static void vCommonSlash(MP_Task mpTask);
    static void vCommonToCommon(MP_Task mpTask);
    
    static void vOpFrameDrop(MP_Task mpTask);
    static void vOpFramePush(MP_Task mpTask);
    
    
    // cpu
    static void vCpuRdtscp(MP_Task mpTask);
    
    // cursor
    static void vCursorFetch(MP_Task mpTask);
    static void vCursorStore(MP_Task mpTask);
    
    
    // diclock
    static void vDicLockDropWrite(MP_Task mpTask);
    static void vDicLockGrabWrite(MP_Task mpTask);
    
    // ddllock
    static void vDdlLockDropRead(MP_Task mpTask);
    static void vDdlLockDropWrite(MP_Task mpTask);
    static void vDdlLockGrabRead(MP_Task mpTask);
    static void vDdlLockGrabWrite(MP_Task mpTask);
    static void vDdlLockTestDrop(MP_Task mpTask);
    
    // debug
    static void vDebugTrap(MP_Task mpTask);
    
    // dic
    static void vDicFrozenGet(MP_Task mpTask);
    static void vDicFrozenSet(MP_Task mpTask);
    static void vDicInit(MP_Task mpTask);
    static void vDicKindGet(MP_Task mpTask);
    static void vDicKindSet(MP_Task mpTask);
    static void vDicLastWord(MP_Task mpTask);
    static void vDicMain(MP_Task mpTask);
    static void vDicName(MP_Task mpTask);
    static void vDicRef(MP_Task mpTask);
    static void vDicWordCount(MP_Task mpTask);
    
    // dicPath
    static void vDicPathDepth(MP_Task mpTask);
    static void vDicPathDump(MP_Task mpTask);
    static void vDicPathPick(MP_Task mpTask);
    
    // dicStack
    static void vDicStackDepth(MP_Task mpTask);
    static void vDicStackDrop(MP_Task mpTask);
    static void vDicStackDump(MP_Task mpTask);
    static void vDicStackPick(MP_Task mpTask);
    static void vDicStackPop(MP_Task mpTask);
    static void vDicStackPush(MP_Task mpTask);

    // dir
    static void vDirChangeCurrent(MP_Task mpTask);
    static void vDirGetCurrent(MP_Task mpTask);
    static void vDirExists(MP_Task mpTask);
    static void vDirMake(MP_Task mpTask);
    static void vDirRemove(MP_Task mpTask);
    
    static void vDirClose(MP_Task mpTask);
    static void vDirDelete(MP_Task mpTask);
    static void vDirEntryName(MP_Task mpTask);
    static void vDirEntryType(MP_Task mpTask);
    static void vDirInit(MP_Task mpTask);
    static void vDirIsClosed(MP_Task mpTask);
    static void vDirIsOpen(MP_Task mpTask);
    static void vDirNew(MP_Task mpTask);
    static void vDirOpen(MP_Task mpTask);
    static void vDirPathGet(MP_Task mpTask);
    static void vDirPathSet(MP_Task mpTask);
    static void vDirRead(MP_Task mpTask);
    static void vDirRewind(MP_Task mpTask);
    static void vDirSeek(MP_Task mpTask);
    static void vDirTell(MP_Task mpTask);
    
    // docol
    static void vDocolCompiling(MP_Task mpTask);
    static void vDocolInterpreting(MP_Task mpTask);
    static void vDocolIsCompiling(MP_Task mpTask);
    static void vDocolIsInterpreting(MP_Task mpTask);
    
    // dot
    static void vDot(MP_Task mpTask);
    static void vDotPut(MP_Task mpTask);
    static void vDotNewline(MP_Task mpTask);
    static void vDotVerbose(MP_Task mpTask);
    
    // dstack
    static void vDstackClass(MP_Task mpTask);                 //! DUP OF class@
    static void vDstackDepth(MP_Task mpTask);
    static void vDstackDrop(MP_Task mpTask);
    static void vDstackDropTwo(MP_Task mpTask);
    static void vDstackDump(MP_Task mpTask);
    static void vDstackDup(MP_Task mpTask);
    static void vDstackDupNonZero(MP_Task mpTask);
    static void vDstackNip(MP_Task mpTask);
    static void vDstackOver(MP_Task mpTask);
    static void vDstackPick(MP_Task mpTask);
    static void vDstackReverse(MP_Task mpTask);
    static void vDstackRoll(MP_Task mpTask);
    static void vDstackRollInv(MP_Task mpTask);
    static void vDstackRot(MP_Task mpTask);
    static void vDstackRotInv(MP_Task mpTask);
    static void vDstackStk(MP_Task mpTask);
    static void vDstackSwap(MP_Task mpTask);
    static void vDstackToValSet(MP_Task mpTask);
    static void vDstackToValues(MP_Task mpTask);
    static void vDstackTransfer(MP_Task mpTask);
    static void vDstackTuck(MP_Task mpTask);
    
    // encode
    static void vEncodeAgain(MP_Task mpTask);
    static void vEncodeAhead(MP_Task mpTask);
    static void vEncodeCallPrimitive(MP_Task mpTask);
    static void vEncodeCallRoutine(MP_Task mpTask);
    static void vEncodeContinue(MP_Task mpTask);
    static void vEncodeHere(MP_Task mpTask);
    static void vEncodeIf(MP_Task mpTask);
    static void vEncodeIfAgain(MP_Task mpTask);
    static void vEncodeLoop(MP_Task mpTask);
    static void vEncodeLoopPlus(MP_Task mpTask);
    static void vEncodeNext(MP_Task mpTask);
    static void vEncodePushValue(MP_Task mpTask);
    static void vEncodeThen(MP_Task mpTask);
    static void vEncodeUntil(MP_Task mpTask);
    
    // enter
    static void vEnterObject(MP_Task mpTask);
    static void vEnterRoutine(MP_Task mpTask);
    
    // entry
    static void vEntryMatches(MP_Task mpTask);
    static void vEntrySame(MP_Task mpTask);
    static void vEntryToDataRow(MP_Task mpTask);

    // entry buffer
    static void vEntryBufferCreate(MP_Task mpTask);
    static void vEntryBufferDelete(MP_Task mpTask);
    static void vEntryBufferDump(MP_Task mpTask);
    static void vEntryBufferFirstEntry(MP_Task mpTask);
    static void vEntryBufferIndexFetch(MP_Task mpTask);
    static void vEntryBufferIndexStore(MP_Task mpTask);
    static void vEntryBufferIsLastFetch(MP_Task mpTask);
    static void vEntryBufferIsLastStore(MP_Task mpTask);
    static void vEntryBufferIterate(MP_Task mpTask);
    static void vEntryBufferNextBuffer(MP_Task mpTask);
    static void vEntryBufferNextEntry(MP_Task mpTask);
    static void vEntryBufferPartitionFetch(MP_Task mpTask);
    static void vEntryBufferPartitionStore(MP_Task mpTask);
    static void vEntryBufferToEntryBuffer(MP_Task mpTask);
    
    // env
    static void vEnvGet(MP_Task mpTask);
    
    // errors
    static void vErrorMsg(MP_Task mpTask);
    
    // exit
    static void vExit(MP_Task mpTask);
    
    // file
    static void vFileClose(MP_Task mpTask);     
    static void vFileDelete(MP_Task mpTask);
    static void vFileInit(MP_Task mpTask);
    static void vFileNew(MP_Task mpTask);
    static void vFileOpen(MP_Task mpTask);
    static void vFilePathGet(MP_Task mpTask);
    static void vFilePathSet(MP_Task mpTask);   
    static void vFileRead(MP_Task mpTask);
    static void vFileReadRecord(MP_Task mpTask);
    static void vFileWrite(MP_Task mpTask);
    
    // flow
    static void vFlowBegin_(MP_Task mpTask);
    static void vFlowExecute(MP_Task mpTask);
    static void vFlowHazard(MP_Task mpTask);
    static void vFlowJump(MP_Task mpTask);
    static void vFlowJumpBegin_(MP_Task mpTask);
    static void vFlowJumpFalse(MP_Task mpTask);
    static void vFlowJumpTrue(MP_Task mpTask);
    static void vFlowNop(MP_Task mpTask);
    static void vFlowRetryPoint(MP_Task mpTask);
    static void vFlowTest(MP_Task mpTask);
    static void vFlowThrow(MP_Task mpTask);
    
    // grid    
    static void vGridFindNode(MP_Task mpTask);
    static void vGridNode(MP_Task mpTask);
    static void vGridNodes(MP_Task mpTask);
    static void vGridPartitions(MP_Task mpTask);
    static void vGridShow(MP_Task mpTask);
    static void vGridSlots(MP_Task mpTask);
    static void vGridStartedStore(MP_Task mpTask);
    static void vGridStartedFetch(MP_Task mpTask);
    static void vGridStartNodeStore(MP_Task mpTask);
    static void vGridStartNodeFetch(MP_Task mpTask);
    static void vGridThisNodeFetch(MP_Task mpTask);
    static void vGridThisNodeNameFetch(MP_Task mpTask);
    static void vGridToNode(MP_Task mpTask);
    static void vGridToPartition(MP_Task mpTask);
    static void vGridToSlot(MP_Task mpTask);
  
    // group by
    static void vGroupByApplyBuffer(MP_Task mpTask);
    static void vGroupByApplyEntry(MP_Task mpTask);
    static void vGroupByApplyPartial(MP_Task mpTask);
    static void vGroupByDelete(MP_Task mpTask);
    static void vGroupByEnd(MP_Task mpTask);
    static void vGroupByNew(MP_Task mpTask);
    static void vGroupByStart(MP_Task mpTask);
  
    // handler
    static void vHandlerServiceSleepQueue(MP_Task mpTask);
  
    // index
    static void vIndexAnalyze(MP_Task mpTask);
    static void vIndexColumnFetch(MP_Task mpTask);
    static void vIndexColumnCountFetch(MP_Task mpTask);
    static void vIndexColumnName(MP_Task mpTask);
    static void vIndexColumnType(MP_Task mpTask);
    static void vIndexCompact(MP_Task mpTask);
    static void vIndexCreateColumnVecHidden(MP_Task mpTask);
    static void vIndexCreateColumnVec(MP_Task mpTask);
    static void vIndexDump(MP_Task mpTask);
    static void vIndexExactKey(MP_Task mpTask);
    static void vIndexInit(MP_Task mpTask);
    static void vIndexIsHiddenFetch(MP_Task mpTask);
    static void vIndexIsHiddenStore(MP_Task mpTask);
    static void vIndexIsPrimaryKeyFetch(MP_Task mpTask);
    static void vIndexIsPrimaryKeyStore(MP_Task mpTask);
    static void vIndexIsUniqueFetch(MP_Task mpTask);
    static void vIndexIsUniqueStore(MP_Task mpTask);
    static void vIndexIsValidUniqueIndex(MP_Task mpTask);
    static void vIndexLowerBound(MP_Task mpTask);
    static void vIndexNameFetch(MP_Task mpTask);
    static void vIndexPopulate(MP_Task mpTask);
    static void vIndexPosition(MP_Task mpTask);
    static void vIndexPositionFirst(MP_Task mpTask);
    static void vIndexRange(MP_Task mpTask);
    static void vIndexScan(MP_Task mpTask);
    static void vIndexSetSingleColumn(MP_Task mpTask);
    static void vIndexShowColumnVec(MP_Task mpTask);
    static void vIndexSpace(MP_Task mpTask);
    static void vIndexSubDic(MP_Task mpTask);
    static void vIndexTableFetch(MP_Task mpTask);
    static void vIndexTableStore(MP_Task mpTask);
    static void vIndexTruncate(MP_Task mpTask);
    static void vIndexUpper(MP_Task mpTask);
    static void vIndexUpperBound(MP_Task mpTask);
    static void vIndexVerify(MP_Task mpTask);
    static void vIndexVisualize(MP_Task mpTask);

    // input
    static void vInputClose(MP_Task mpTask);
    static void vInputDepth(MP_Task mpTask);
    static void vInputNext(MP_Task mpTask);
    static void vInputOpen(MP_Task mpTask);
    static void vInputPop(MP_Task mpTask);
    static void vInputPush(MP_Task mpTask);
    static void vInputStdin(MP_Task mpTask);
    static void vInputText(MP_Task mpTask);
    static void vInputToken(MP_Task mpTask);
    static void vInputType(MP_Task mpTask);
    static void vInputUnknown(MP_Task mpTask);

    // int
    static void vIntAdd(MP_Task mpTask);
    static void vIntDivide(MP_Task mpTask);
    static void vIntEqual(MP_Task mpTask);
    static void vIntGreater(MP_Task mpTask);
    static void vIntGreaterEqual(MP_Task mpTask);
    static void vIntHash(MP_Task mpTask);
    static void vIntHo16(MP_Task mpTask);
    static void vIntLess(MP_Task mpTask);
    static void vIntLessEqual(MP_Task mpTask);
    static void vIntModulo(MP_Task mpTask);
    static void vIntMultiply(MP_Task mpTask);
    static void vIntNeg(MP_Task mpTask);
    static void vIntNotEqual(MP_Task mpTask);
    static void vIntSubtract(MP_Task mpTask);
    
    // jobClass
    static void vJobClassBuffer(MP_Task mpTask);
    static void vJobClassFind(MP_Task mpTask);
    static void vJobClassName(MP_Task mpTask);
    static void vJobClassNew(MP_Task mpTask);
    
    static void vJobClassSeedRun(MP_Task mpTask);
    static void vJobClassSeedReq(MP_Task mpTask);
    static void vJobClassSeedRcv(MP_Task mpTask);
    static void vJobClassSeedExe(MP_Task mpTask);
    static void vJobClassSeedEnd(MP_Task mpTask);
    static void vJobClassSeedCan(MP_Task mpTask);
    
    // job
    static void vJobAcksRemaining(MP_Task mpTask);
    static void vJobAddOriginator(MP_Task mpTask);
    static void vJobAddParticipant(MP_Task mpTask);
    static void vJobClass(MP_Task mpTask);
    static void vJobColumnCountFetch(MP_Task mpTask);
    static void vJobColumnNameFetch(MP_Task mpTask);
    static void vJobColumnIsPartKey(MP_Task mpTask);
    static void vJobColumnTypeFetch(MP_Task mpTask);
    static void vJobDecAcksRemaining(MP_Task mpTask);
    static void vJobDelete(MP_Task mpTask);
    static void vJobDicPath(MP_Task mpTask);
    static void vJobFindOriginator(MP_Task mpTask);
    static void vJobFindParticipant(MP_Task mpTask);
    static void vJobHasError(MP_Task mpTask);
    static void vJobHasWorkingBuffer(MP_Task mpTask);
    static void vJobIdFetch(MP_Task mpTask);
    static void vJobIndexNameFetch(MP_Task mpTask);
    static void vJobErrorStore(MP_Task mpTask);
    static void vJobErrorFetch(MP_Task mpTask);
    static void vJobExecuteFetch(MP_Task mpTask);
    static void vJobJobClass(MP_Task mpTask);
    static void vJobMetadataStore(MP_Task mpTask);
    static void vJobNewBegun(MP_Task mpTask);
    static void vJobNewCancel(MP_Task mpTask);
    static void vJobNewCancelled(MP_Task mpTask);
    static void vJobNewEnd(MP_Task mpTask);
    static void vJobNewEnded(MP_Task mpTask);
    static void vJobNewExecute(MP_Task mpTask);
    static void vJobNewExecuted(MP_Task mpTask);
    static void vJobObjectNameFetch(MP_Task mpTask);
    static void vJobOriginatingTaskFetch(MP_Task mpTask);
    static void vJobOriginatingTaskStore(MP_Task mpTask);
    static void vJobOriginatorFetch(MP_Task mpTask);
    static void vJobParticipantFetch(MP_Task mpTask);
    static void vJobParticipantStore(MP_Task mpTask);
    static void vJobPostRecv(MP_Task mpTask);
    static void vJobPreSend(MP_Task mpTask);
    static void vJobRecv(MP_Task mpTask);
    static void vJobPartitionsFetch(MP_Task mpTask);
    static void vJobRegionSizeFetch(MP_Task mpTask);
    static void vJobRemoveOriginator(MP_Task mpTask);
    static void vJobRemoveParticipant(MP_Task mpTask);
    static void vJobResponseStore(MP_Task mpTask);
    static void vJobResponseFetch(MP_Task mpTask);
    static void vJobRowsInvalidFetch(MP_Task mpTask);
    static void vJobRowsInvalidStore(MP_Task mpTask);
    static void vJobRowsProcessedFetch(MP_Task mpTask);
    static void vJobRowsProcessedStore(MP_Task mpTask);
    static void vJobRowsSkippedFetch(MP_Task mpTask);
    static void vJobRowsSkippedStore(MP_Task mpTask);
    static void vJobSend(MP_Task mpTask);
    static void vJobShow(MP_Task mpTask);
    static void vJobSignalConnector(MP_Task mpTask);
    static void vJobSpawn(MP_Task mpTask);
    static void vJobThis(MP_Task mpTask);
    static void vJobTraceStore(MP_Task mpTask);
    static void vJobTraceFetch(MP_Task mpTask);
    static void vJobWaitParticipant(MP_Task mpTask);
    static void vJobWorkingBufferFetch(MP_Task mpTask);
    static void vJobWorkingBufferStore(MP_Task mpTask);
    
    // job analyzeIndex
    static void vJob_AnalyzeIndexIsDetailed(MP_Task mpTask);
    static void vJob_AnalyzeTableIsDetailed(MP_Task mpTask);
    
    // job CreateIndex
    static void vJob_CreateIndexIsUniqueFetch(MP_Task mpTask);
    
    // job generic
    static void vJob__BegunErrorFetch(MP_Task mpTask);
    static void vJob__BegunErrorStore(MP_Task mpTask);
    static void vJob__CancelledErrorFetch(MP_Task mpTask);
    static void vJob__CancelledErrorStore(MP_Task mpTask);
    static void vJob__ExecutedErrorFetch(MP_Task mpTask);
    static void vJob__ExecutedErrorStore(MP_Task mpTask);
    static void vJob__ExecutedResponseFetch(MP_Task mpTask);
    static void vJob__ExecutedResponseStore(MP_Task mpTask);
    static void vJob__ExecutedRowsInvalidFetch(MP_Task mpTask);
    static void vJob__ExecutedRowsInvalidStore(MP_Task mpTask);
    static void vJob__ExecutedRowsProcessedFetch(MP_Task mpTask);
    static void vJob__ExecutedRowsProcessedStore(MP_Task mpTask);
    static void vJob__ExecutedRowsSkippedFetch(MP_Task mpTask);
    static void vJob__ExecutedRowsSkippedStore(MP_Task mpTask);

    // job FetchRows
    static void vJob_FetchRowsLastBuffersStore(MP_Task mpTask);
    static void vJob_FetchRowsPartitionFetch(MP_Task mpTask);
    static void vJob_FetchRowsPlannerStore(MP_Task mpTask);
    static void vJob_FetchRowsSpawnSub(MP_Task mpTask);
    static void vJob_FetchRowsTableStore(MP_Task mpTask);
    static void vJob_FetchRowsWhereFetch(MP_Task mpTask);
  
    static void vJob_SubFetchBufferQueueFetch(MP_Task mpTask);
    static void vJob_SubFetchPhraseFetch(MP_Task mpTask);
    static void vJob_SubFetchPlannerFetch(MP_Task mpTask);
    static void vJob_SubFetchSlotFetch(MP_Task mpTask);
    
    // job insertRow
    static void vJob_EndInsertInsertRowJobFetch(MP_Task mpTask);
    static void vJob_EndInsertTaskFetch(MP_Task mpTask);
    static void vJob_EndInsertTaskStore(MP_Task mpTask);
  
    static void vJob_InsertBufferEndInsertJobIdFetch(MP_Task mpTask);
    static void vJob_InsertBufferInsertRows(MP_Task mpTask);
    static void vJob_InsertBufferPartitionFetch(MP_Task mpTask);
    static void vJob_InsertBufferTableStore(MP_Task mpTask);
  
    static void vJob_InsertRowDanglingBuffer(MP_Task mpTask);
    static void vJob_InsertRowSpawnInsertBuffer(MP_Task mpTask);
    static void vJob_InsertRowTableStore(MP_Task mpTask);
    
    // job LoadBuffer
    static void vJob_LoadBufferInsertRows(MP_Task mpTask);
    static void vJob_LoadBufferJobLoadTableIdFetch(MP_Task mpTask);
    static void vJob_LoadBufferJobLoadTableStore(MP_Task mpTask);
    static void vJob_LoadBufferPartitionFetch(MP_Task mpTask);
    static void vJob_LoadBufferSpawn(MP_Task mpTask);
    static void vJob_LoadBufferTableStore(MP_Task mpTask);
  
    // job LoadTable
    static void vJob_LoadTableAddInvalidRows(MP_Task mpTask);
    static void vJob_LoadTableAddValidRows(MP_Task mpTask);
    static void vJob_LoadTableChunkFile(MP_Task mpTask);
    static void vJob_LoadTableDumpMap(MP_Task mpTask);
    static void vJob_LoadTableFilePathFetch(MP_Task mpTask);
    static void vJob_LoadTableTableStore(MP_Task mpTask);
    static void vJob_LoadTableTaskFetch(MP_Task mpTask);
    static void vJob_LoadTableTaskStore(MP_Task mpTask);
  
    // job NodeInfo
    static void vJob_NodeInfoNodeNameFetch(MP_Task mpTask);
  
    // job receiver
    static void vJob_ReceiverConnNoFetch(MP_Task mpTask);
    static void vJob_ReceiverConnNoStore(MP_Task mpTask);
    static void vJob_ReceiverNodeFetch(MP_Task mpTask);
    static void vJob_ReceiverNodeStore(MP_Task mpTask);
    
    // Job subLoader
    static void vJob_SubLoaderBegin(MP_Task mpTask);
    static void vJob_SubLoaderDanglingBuffer(MP_Task mpTask);
    static void vJob_SubLoaderEnd(MP_Task mpTask);
    static void vJob_SubLoaderJobLoadTableFetch(MP_Task mpTask);
    static void vJob_SubLoaderSpawn(MP_Task mpTask);
  
    static void vJob_SubLoaderPartitionFetch(MP_Task mpTask);
    static void vJob_SubLoaderPartitionStore(MP_Task mpTask);
    static void vJob_SubLoaderProcessRecords(MP_Task mpTask);
    static void vJob_SubLoaderRecordsInvalid(MP_Task mpTask);
    static void vJob_SubLoaderWorkingBuffer(MP_Task mpTask);

    // Job subTruncate
    static void vJob_SubTruncateSlotFetch(MP_Task mpTask);
    static void vJob_SubTruncateTruncateJobIdFetch(MP_Task mpTask);
  
    // Job truncate
    static void vJob_TruncateTableSpawnSub(MP_Task mpTask);
    static void vJob_TruncateTableTaskFetch(MP_Task mpTask);
    static void vJob_TruncateTableTaskStore(MP_Task mpTask);

    // load buffer
    static void vLoadBufferSpawn(MP_Task mpTask);
  
    // load table
    static void vLoadTableChunkFile(MP_Task mpTask);
    static void vLoadTableDelete(MP_Task mpTask);
    static void vLoadTableDumpMap(MP_Task mpTask);
    static void vLoadTableNew(MP_Task mpTask);
    static void vLoadTableTableStore(MP_Task mpTask);
  
    // locals
    static void vLocalFetch(MP_Task mpTask);
    static void vLocalStore(MP_Task mpTask);
    static void vLocalsBar(MP_Task mpTask);
    static void vLocalsPop(MP_Task mpTask);
    static void vLocalsPush(MP_Task mpTask);
    static void vLocalsTo(MP_Task mpTask);
   
    // lock
    static void vLockDropRead(MP_Task mpTask);
    static void vLockDropWrite(MP_Task mpTask);
    static void vLockGrabRead(MP_Task mpTask);
    static void vLockGrabWrite(MP_Task mpTask);
    static void vLockInit(MP_Task mpTask);
    
    // locks
    static void vLocksList(MP_Task mpTask);
    static void vLocksReset(MP_Task mpTask);
  
    // logic
    static void vLogicAnd(MP_Task mpTask);
    static void vLogicNot(MP_Task mpTask);
    static void vLogicOr(MP_Task mpTask);
    
    // magnitude
    static void vBytesMagnitude(MP_Task mpTask);
    static void vNumberMagnitude(MP_Task mpTask);
    static void vToDigitName(MP_Task mpTask);
    
    // memory
    static void vMemoryByteDump(MP_Task mpTask);
    static void vMemoryList(MP_Task mpTask);
    static void vMemoryReset(MP_Task mpTask);
    static void vMemoryWordDump(MP_Task mpTask);
    
    // node (of grid)
    static void vNodeInit(MP_Task mpTask);
    static void vNodeIP(MP_Task mpTask);
    static void vNodeName(MP_Task mpTask);
    static void vNodeNew(MP_Task mpTask);
    static void vNodeNo(MP_Task mpTask);
    static void vNodePartitions(MP_Task mpTask);
    static void vNodePort(MP_Task mpTask);
    static void vNodeRecvSocketStore(MP_Task mpTask);
    static void vNodeRecvSocketFetch(MP_Task mpTask);
    static void vNodeSendSocketStore(MP_Task mpTask);
    static void vNodeSendSocketFetch(MP_Task mpTask);
    static void vNodeSize(MP_Task mpTask);
    static void vNodeStatus(MP_Task mpTask);
        
    // object
    static void vObjectClassFetch(MP_Task mpTask);
    static void vObjectClassStore(MP_Task mpTask);
    static void vObjectDump(MP_Task mpTask);
    static void vObjectFetch(MP_Task mpTask);
    static void vObjectMagicMark(MP_Task mpTask);
    static void vObjectStore(MP_Task mpTask);
    static void vObjectWordBodySet(MP_Task mpTask);
    
    // opFrame
    static void vOpFrameDic(MP_Task mpTask);
    static void vOpFrameName(MP_Task mpTask);
    
  
    // persistence
    static void vPersistenceCheckpoint(MP_Task mpTask);
    static void vPersistenceReload(MP_Task mpTask);
    
    // plan
    static void vPlanCompile(MP_Task mpTask);
    static void vPlanDelete(MP_Task mpTask);
    static void vPlanDic(MP_Task mpTask);
    static void vPlanExecutingTaskFetch(MP_Task mpTask);
    static void vPlanExecutingTaskStore(MP_Task mpTask);
    static void vPlanFinished(MP_Task mpTask);
    static void vPlanIdFetch(MP_Task mpTask);
    static void vPlanLambdaAppend(MP_Task mpTask);
    static void vPlanLambdaFetch(MP_Task mpTask);
    static void vPlanLambdaStore(MP_Task mpTask);
    static void vPlanNew(MP_Task mpTask);
    static void vPlanNameFetch(MP_Task mpTask);
    static void vPlanNameStore(MP_Task mpTask);
    static void vPlanOpFrameAdd(MP_Task mpTask);
    static void vPlanOpFrameFind(MP_Task mpTask);
    static void vPlanOpFrameTop(MP_Task mpTask);
    static void vPlanShow(MP_Task mpTask);
    
    // plan list
    static void vPlanListAddPlan(MP_Task mpTask);
    static void vPlanListFindPlan(MP_Task mpTask);
    static void vPlanListRemovePlan(MP_Task mpTask);
    static void vPlanListShow(MP_Task mpTask);
    
    // planner
    static void vPlannerChoosePaths(MP_Task mpTask);
    static void vPlannerChosenPath(MP_Task mpTask);
    static void vPlannerDelete(MP_Task mpTask);
    static void vPlannerFetchTechniqueFetch(MP_Task mpTask);
    static void vPlannerGenPaths(MP_Task mpTask);
    static void vPlannerHighKeyFetch(MP_Task mpTask);
    static void vPlannerIndexNmFetch(MP_Task mpTask);
    static void vPlannerKeyFetch(MP_Task mpTask);
    static void vPlannerLowKeyFetch(MP_Task mpTask);
    static void vPlannerMatchPaths(MP_Task mpTask);
    static void vPlannerNew(MP_Task mpTask);
    static void vPlannerParseWhere(MP_Task mpTask);
    static void vPlannerPhrasesFetch(MP_Task mpTask);
    static void vPlannerScorePaths(MP_Task mpTask);
    static void vPlannerShowPaths(MP_Task mpTask);
    static void vPlannerTableFetch(MP_Task mpTask);
    static void vPlannerTableStore(MP_Task mpTask);
  
    // process
    static void vProcessGetPid(MP_Task mpTask);
    static void vProcessQuit(MP_Task mpTask);
    static void vProcessTestPid(MP_Task mpTask);
    
    // prompt
    static void vPromptContinue(MP_Task mpTask);
    static void vPromptExecuteFunction(MP_Task mpTask);
    static void vPromptGetCommand(MP_Task mpTask);
    static void vPromptNew(MP_Task mpTask);
    static void vPromptSession(MP_Task mpTask);

    // push
    static void vPushI(MP_Task mpTask);
    static void vPushJ(MP_Task mpTask);
    static void vPushNullPtr(MP_Task mpTask);
    static void vPushValue(MP_Task mpTask);
    
    // queue
    static void vQueueAppend(MP_Task mpTask);
    static void vQueueDelete(MP_Task mpTask);
    static void vQueueDump(MP_Task mpTask);
    static void vQueueInit(MP_Task mpTask);
    static void vQueueNew(MP_Task mpTask);
    static void vQueuePop(MP_Task mpTask);
    
    // queues
    static void vQueuesExecutingLength(MP_Task mpTask);
    static void vQueuesReadyLength(MP_Task mpTask);
    static void vQueuesReadySlotLength(MP_Task mpTask);
    static void vQueuesSleepingLength(MP_Task mpTask);
    static void vQueuesWaitingLength(MP_Task mpTask);

    // rbCursor
    static void vRbCursorBegin(MP_Task mpTask);
    static void vRbCursorDelete(MP_Task mpTask);
    static void vRbCursorEnd(MP_Task mpTask);
    static void vRbCursorFirst(MP_Task mpTask);
    static void vRbCursorGet(MP_Task mpTask);
    static void vRbCursorInit(MP_Task mpTask);
    static void vRbCursorNew(MP_Task mpTask);
    static void vRbCursorNext(MP_Task mpTask);
    static void vRbCursorRowBuffer(MP_Task mpTask);
    static void vRbCursorTableFetch(MP_Task mpTask);
    static void vRbCursorTableStore(MP_Task mpTask);
        
    // row
    static void vRowString(MP_Task mpTask);
    static void vRowEscapedString(MP_Task mpTask);
    
    // rowBuffer
    static void vRowBufferDump(MP_Task mpTask);
    static void vRowBufferNew(MP_Task mpTask);
    
    // rowCursor
    static void vRowCursorBegin(MP_Task mpTask);
    static void vRowCursorDelete(MP_Task mpTask);
    static void vRowCursorEnd(MP_Task mpTask);
    static void vRowCursorEntryBufferFetch(MP_Task mpTask);
    static void vRowCursorEntryBufferStore(MP_Task mpTask);
    static void vRowCursorFirst(MP_Task mpTask);
    static void vRowCursorInit(MP_Task mpTask);
    static void vRowCursorNew(MP_Task mpTask);
    static void vRowCursorNext(MP_Task mpTask);
    static void vRowCursorRow(MP_Task mpTask);
    static void vRowCursorTableFetch(MP_Task mpTask);
    static void vRowCursorTableStore(MP_Task mpTask);
    
    // rstack
    static void vRstackCsPick(MP_Task mpTask);
    static void vRstackCsRoll(MP_Task mpTask);
    static void vRstackDepth(MP_Task mpTask);
    static void vRstackDrop(MP_Task mpTask);
    static void vRstackFetch(MP_Task mpTask);
    static void vRstackFetchTwo(MP_Task mpTask);
    static void vRstackPick(MP_Task mpTask);
    static void vRstackReverse(MP_Task mpTask);
    static void vRstackRoll(MP_Task mpTask);
    static void vRstackRollInv(MP_Task mpTask);
    static void vRstackStk(MP_Task mpTask);
    static void vRstackSwap(MP_Task mpTask);
    static void vRstackToP(MP_Task mpTask);
    static void vRstackToR(MP_Task mpTask);
    static void vRstackToRTwo(MP_Task mpTask);
    static void vRstack0(MP_Task mpTask);
    static void vRstack1(MP_Task mpTask);
    static void vRstack2(MP_Task mpTask);
    static void vRstack3(MP_Task mpTask);
    static void vRstack4(MP_Task mpTask);
    static void vRstack5(MP_Task mpTask);
    static void vRstack6(MP_Task mpTask);
    static void vRstack7(MP_Task mpTask);
    static void vRstack8(MP_Task mpTask);
    static void vRstack9(MP_Task mpTask);
    
  
    // session
    static void vSessionCommandProcessor(MP_Task mpTask);
    static void vSessionDicStack(MP_Task mpTask);
    static void vSessionDump(MP_Task mpTask);
    static void vSessionIsExiting(MP_Task mpTask);
    static void vSessionPid(MP_Task mpTask);
    static void vSessionSlotFetch(MP_Task mpTask);
    static void vSessionTxFetch(MP_Task mpTask);
    static void vSessionTxStore(MP_Task mpTask);
  
    // selCursor
    static void vSelCursorBegin(MP_Task mpTask);
    static void vSelCursorDelete(MP_Task mpTask);
    static void vSelCursorEnd(MP_Task mpTask);
    static void vSelCursorFirst(MP_Task mpTask);
    static void vSelCursorGet(MP_Task mpTask);
    static void vSelCursorHasRow(MP_Task mpTask);
    static void vSelCursorInit(MP_Task mpTask);
    static void vSelCursorNew(MP_Task mpTask);
    static void vSelCursorNext(MP_Task mpTask);
    static void vSelCursorRowBuffer(MP_Task mpTask);
    static void vSelCursorTableFetch(MP_Task mpTask);
    static void vSelCursorTableStore(MP_Task mpTask);
    
    // semaphore
    static void vSemaphoreInit(MP_Task mpTask);
  
    // shell
    static void vShellExecute(MP_Task mpTask);
    
    // slots
    static void vSlotsColumnCount(MP_Task mpTask);
    static void vSlotsColumnName(MP_Task mpTask);
    static void vSlotsCreateColumnVec(MP_Task mpTask);
    static void vSlotsSubDic(MP_Task mpTask);
    
    // socket
    static void vSocketAccept(MP_Task mpTask);
    static void vSocketClose(MP_Task mpTask);
    static void vSocketConnect(MP_Task mpTask);
    static void vSocketDropWrite(MP_Task mpTask);
    static void vSocketGrabWrite(MP_Task mpTask);
    static void vSocketListen(MP_Task mpTask);
    static void vSocketOpen(MP_Task mpTask);
    static void vSocketRecvObject(MP_Task mpTask);
    static void vSocketRecvObject_LowMem(MP_Task mpTask);
    static void vSocketRecvValSet(MP_Task mpTask);
    static void vSocketSendValSet(MP_Task mpTask);
    static void vSocketSendObject(MP_Task mpTask);

    // steps
    static void vStepsList(MP_Task mpTask);
    static void vStepsReset(MP_Task mpTask);
    
    // string
    static void vStrAlgebra(MP_Task mpTask);
    static void vStrCat(MP_Task mpTask);
    static void vStrClip(MP_Task mpTask);
    static void vStrEqual(MP_Task mpTask);
    static void vStrEscape(MP_Task mpTask);
    static void vStrFirstCsv(MP_Task mpTask);
    static void vStrFormat(MP_Task mpTask);
    static void vStrLeft(MP_Task mpTask);
    static void vStrLength(MP_Task mpTask);
    static void vStrPad(MP_Task mpTask);
    static void vStrPrepend(MP_Task mpTask);
    static void vStrRandom(MP_Task mpTask);
    static void vStrReplace(MP_Task mpTask);
    static void vStrRight(MP_Task mpTask);
    static void vStrSub(MP_Task mpTask);
    static void vStrTailInt(MP_Task mpTask);

    static void vS256Init(MP_Task mpTask);
    
    // sub loader
    static void vSubLoaderDelete(MP_Task mpTask);
  
    // table
    static void vTableAddIndex(MP_Task mpTask);
    static void vTableAnalyze(MP_Task mpTask);
    static void vTableColumnCountFetch(MP_Task mpTask);
    static void vTableCreateColumnVec(MP_Task mpTask);
    static void vTableDrop(MP_Task mpTask);
    static void vTableDump(MP_Task mpTask);
    static void vTableFirstBuffer(MP_Task mpTask);
    static void vTableGetColumnsAsString(MP_Task mpTask);
    static void vTableColumnFetch(MP_Task mpTask);
    static void vTableGetPtn(MP_Task mpTask);
    static void vTableHiddenIndexFetch(MP_Task mpTask);
    static void vTableHiddenIndexStore(MP_Task mpTask);
    static void vTableIndexCount(MP_Task mpTask);
    static void vTableInit(MP_Task mpTask);
    static void vTableInsert(MP_Task mpTask);
    static void vTableInsertEntries(MP_Task mpTask);
    static void vTableLazyBuildIndexes(MP_Task mpTask);
    static void vTableIsLoadingStore(MP_Task mpTask);
    static void vTableList(MP_Task mpTask);
    static void vTableName(MP_Task mpTask);
    static void vTableRemoveIndex(MP_Task mpTask);
    static void vTableShowColumnVec(MP_Task mpTask);
    static void vTableSlots(MP_Task mpTask);
    static void vTableSubDic(MP_Task mpTask);
    static void vTableSubLoaders(MP_Task mpTask);
    static void vTableTruncate(MP_Task mpTask);
    static void vTableIsValidIndexColumns(MP_Task mpTask);
    
    // table partition
    static void vTablePtnDump(MP_Task mpTask);
    static void vTablePtnAttemptGrabRead(MP_Task mpTask);
    static void vTablePtnAttemptGrabWrite(MP_Task mpTask);
    static void vTablePtnDropRead(MP_Task mpTask);
    static void vTablePtnDropWrite(MP_Task mpTask);
    static void vTablePtnIsLocked(MP_Task mpTask);
  
    // task
    static void vTaskAck(MP_Task mpTask);
    static void vTaskAcksStore(MP_Task mpTask);
    static void vTaskBreakPoint(MP_Task mpTask);
    static void vTaskDestructorTraceFetch(MP_Task mpTask);
    static void vTaskDestructorTraceStore(MP_Task mpTask);
    static void vTaskDicStack(MP_Task mpTask);
    static void vTaskDropLock(MP_Task mpTask);
    static void vTaskDump(MP_Task mpTask);
    static void vTaskGrabLock(MP_Task mpTask);
    static void vTaskJobFetch(MP_Task mpTask);
    static void vTaskJobStore(MP_Task mpTask);
    static void vTaskNameFetch(MP_Task mpTask);
    static void vTaskThis(MP_Task mpTask);
    static void vTaskPartitionFetch(MP_Task mpTask);
    static void vTaskPartitionStore(MP_Task mpTask);
    static void vTaskPlanFetch(MP_Task mpTask);
    static void vTaskPlanStore(MP_Task mpTask);
    static void vTaskSeed(MP_Task mpTask);
    static void vTaskSessionFetch(MP_Task mpTask);
    static void vTaskSessionStore(MP_Task mpTask);
    static void vTaskSend(MP_Task mpTask);
    static void vTaskSendNodeFetch(MP_Task mpTask);
    static void vTaskSendNodeStore(MP_Task mpTask);
    static void vTaskSleep(MP_Task mpTask);
    static void vTaskStateFetch(MP_Task mpTask);
    static void vTaskStateStore(MP_Task mpTask);
    static void vTaskThread(MP_Task mpTask);
    static void vTaskWait(MP_Task mpTask);
    static void vTaskYield(MP_Task mpTask);
        
    // thread
    static void vThreadBind(MP_Task mpTask);
    static void vThreadCondSignal(MP_Task mpTask);
    static void vThreadCondWait(MP_Task mpTask);
    static void vThreadCpu(MP_Task mpTask);
    static void vThreadDedicatedFetch(MP_Task mpTask);
    static void vThreadDedicatedStore(MP_Task mpTask);
    static void vThreadDump(MP_Task mpTask);
    static void vThreadId(MP_Task mpTask);
    static void vThreadMutexLock(MP_Task mpTask);
    static void vThreadMutexUnlock(MP_Task mpTask);
    static void vThreadSleep_s(MP_Task mpTask);
    static void vThreadSleep_ms(MP_Task mpTask);
    static void vThreadSleep_us(MP_Task mpTask);
    static void vThreadSleep_ns(MP_Task mpTask);
  
    // timer
    static void vTimerDelete(MP_Task mpTask);
    static void vTimerInit(MP_Task mpTask);
    static void vTimerMicrosecs(MP_Task mpTask);
    static void vTimerMillisecs(MP_Task mpTask);
    static void vTimerNanosecs(MP_Task mpTask);
    static void vTimerNew(MP_Task mpTask);
    static void vTimerReset(MP_Task mpTask);
    static void vTimerSecs(MP_Task mpTask);
    static void vTimerStart(MP_Task mpTask);
    static void vTimerTicks(MP_Task mpTask);
    static void vTimerUpdate(MP_Task mpTask);
    
    // to
    static void vToBinVal(MP_Task mpTask);
    static void vToBody(MP_Task mpTask);
    static void vToInt(MP_Task mpTask);
    static void vToObject(MP_Task mpTask);
    static void vToPtr(MP_Task mpTask);
    static void vToStr(MP_Task mpTask);
    static void vToTask(MP_Task mpTask);
    
    // token
    static void vTokenColon(MP_Task mpTask);
    
   // trace
    static void vTraceOff(MP_Task mpTask);
    static void vTraceOn(MP_Task mpTask);
    static void vTracePoint(MP_Task mpTask);
    static void vTraceSet(MP_Task mpTask);

    static void vPerformancePoint(MP_Task mpTask);  //! wrong place
    static void vPerformancePrint(MP_Task mpTask);  //! wrong place

    // tx
    static void vTxBegin(MP_Task mpTask);
    static void vTxCommit(MP_Task mpTask);
    static void vTxCounterDec(MP_Task mpTask);
    static void vTxCounterFetch(MP_Task mpTask);
    static void vTxCounterStore(MP_Task mpTask);
    static void vTxDistMemberFetch(MP_Task mpTask);
    static void vTxDistMemberStore(MP_Task mpTask);
    static void vTxDump(MP_Task mpTask);
    static void vTxDumpAll(MP_Task mpTask);
    static void vTxFree(MP_Task mpTask);
    static void vTxId(MP_Task mpTask);
    static void vTxPrepare(MP_Task mpTask);
    static void vTxRollback(MP_Task mpTask);
    static void vTxTestDist(MP_Task mpTask);
  
    // txCollection
    static void vTxCollectionDump(MP_Task mpTask);
    static void vTxCollectionInit(MP_Task mpTask);
    
    // txCursor
    static void vTxCursorBegin(MP_Task mpTask);
    static void vTxCursorDelete(MP_Task mpTask);
    static void vTxCursorEnd(MP_Task mpTask);
    static void vTxCursorFirst(MP_Task mpTask);
    static void vTxCursorGet(MP_Task mpTask);
    static void vTxCursorInit(MP_Task mpTask);
    static void vTxCursorNew(MP_Task mpTask);
    static void vTxCursorNext(MP_Task mpTask);
    
    // txMap
    static void vTxMapInit(MP_Task mpTask);

    // txRef
    static void vTxRefBegin(MP_Task mpTask);
    static void vTxRefCheckDistId(MP_Task mpTask);
    static void vTxRefCommit(MP_Task mpTask);
    static void vTxRefDump(MP_Task mpTask);
    static void vTxRefEnd(MP_Task mpTask);
    static void vTxRefId(MP_Task mpTask);
    static void vTxRefInit(MP_Task mpTask);
    static void vTxRefNew(MP_Task mpTask);
    static void vTxRefRollback(MP_Task mpTask);
  
    // utility
    static void vUtilityPrefetch(MP_Task mpTask);
    static void vUtilitySchemaTest(MP_Task mpTask);

    // vector
    static void vVectorDifference(MP_Task mpTask);
    
    // word
    static void vAllot(MP_Task mpTask);
    static void vCell(MP_Task mpTask);
    static void vColon(MP_Task mpTask);
    static void vComma(MP_Task mpTask);
    static void vCompileComma(MP_Task mpTask);
    static void vCreate(MP_Task mpTask);
    static void vCreateFrom(MP_Task mpTask);
    static void vCreateFromInMain(MP_Task mpTask);
    static void vCreateFromPersistent(MP_Task mpTask);
    static void vCreatePersistent(MP_Task mpTask);
    static void vCreateTenants(MP_Task mpTask);
    static void vDicWhich(MP_Task mpTask);
    static void vDoes(MP_Task mpTask);
    static void vDoes_RT(MP_Task mpTask);
    static void vFind(MP_Task mpTask);
    static void vFindInCurrent(MP_Task mpTask);
    static void vForget(MP_Task mpTask);
    static void vHashHashColon(MP_Task mpTask);
    static void vImmediate(MP_Task mpTask);
    static void vLocate(MP_Task mpTask);
    static void vOpDic(MP_Task mpTask);
    static void vPlanDicTask(MP_Task mpTask);
    static void vPostpone(MP_Task mpTask);
    static void vSee(MP_Task mpTask);
    static void vSemicolon(MP_Task mpTask);
    static void vSubDic(MP_Task mpTask);
    static void vTick(MP_Task mpTask);
    static void vWord(MP_Task mpTask);
    static void vWordDump(MP_Task mpTask);
    static void vWordForget(MP_Task mpTask);
    static void vWordImmediateGet(MP_Task mpTask);
    static void vWordImmediateSet(MP_Task mpTask);
    static void vWordIsImmediate(MP_Task mpTask);
    static void vWordIsPrimitive(MP_Task mpTask);
    static void vWordKindGet(MP_Task mpTask);
    static void vWordKindSet(MP_Task mpTask);
    static void vWordName(MP_Task mpTask);
    static void vWordPrev(MP_Task mpTask);
    static void vWordThis(MP_Task mpTask);
    static void vWordTypeGet(MP_Task mpTask);
    static void vWordTypeSet(MP_Task mpTask);

    // working buffer
    static void vWorkingBufferIsLastFetch(MP_Task mpTask);
    static void vWorkingBufferIsLastStore(MP_Task mpTask);
    static void vWorkingBufferRows(MP_Task mpTask);
    static void vWorkingBufferShow(MP_Task mpTask);
    static void vWorkingBufferToWorkingBuffer(MP_Task mpTask);
    
    // zero
    static void vZeroLess(MP_Task mpTask);
    static void vZeroLessEqual(MP_Task mpTask);
    static void vZeroEqual(MP_Task mpTask);
    static void vZeroNotEqual(MP_Task mpTask);
    static void vZeroGreaterEqual(MP_Task mpTask);
    static void vZeroGreater(MP_Task mpTask);
    
  private:
    static bool   bAppendRow(WorkingBuffer* pWorkingBuffer, Entry* pEntry, Job_FetchRows* pJob_FetchRows);
    static bool   bAppendRow(FetchBuffer* pFetchBuffer, Entry* pEntry, Job_FetchRows* pJob_FetchRows);
    //static bool   bInsertRow(EntryBuffer* pIndexBuffer, Entry* pEntry, Interface::Session* pSession);
    
    /*------------------------------------------------------------------------*/
    
    Footer* aFooter(void);

    /*------------------------------------------------------------------------*/
    
    #define GET_THROW(_1,_2,NAME,...) NAME
        
    #define THROW(...) GET_THROW(__VA_ARGS__, THROW2, THROW1)(__VA_ARGS__)
        
    #define THROW1(p1) \
    { \
      s256 sFile((Stz*)__FILE__); \
      s256 sException; \
      sException.vSprint(p1); \
      Task* pTask = reinterpret_cast<Task*>(PosixThread::pTlsGet(giTLS_TaskKey)); \
      pTask->vException(sFile,sException); \
      Debugging::vThrowTrap(); \
      throw (i8)__LINE__; \
    }
        
    #define THROW2(p1, p2) \
    { \
      s256 sFile((Stz*)__FILE__); \
      s256 sException; \
      sException.vSprint(p1,p2); \
      Task* pTask = reinterpret_cast<Task*>(PosixThread::pTlsGet(giTLS_TaskKey)); \
      pTask->vException(sFile,sException); \
      Debugging::vThrowTrap(); \
      throw (i8)__LINE__; \
    }
        
    // throw invalid auxiliary stack
    #define THROW_INVALID_ASTACK(primitive) \
    { \
      THROW("Invalid aux stack for: %s"_Stz,primitive); \
    }
        
    // throw invalid data stack
    #define THROW_INVALID_DSTACK(primitive) \
    { \
      THROW("Invalid data stack in for: %s"_Stz,primitive); \
    }
        
    // throw invalid return stack
    #define THROW_INVALID_RSTACK(primitive) \
    { \
      THROW("Invalid return stack for: %s"_Stz,primitive); \
    }
        
    #ifdef __ENTRY_EXIT
    
 //ASSERT(! mpTask->m_pJob || mpTask->m_pJob->m_pExecutingTask == *mpTask) 
    
      #define ENTRY(aStepNm) \
        static StepAccum* pStepAccum = nullptr; \
        if (! pStepAccum) { \
          gpAttachMap->m_pStepArray->vGrabWrite(); \
          if (! pStepAccum) \
            pStepAccum = gpAttachMap->m_pStepArray->pAllocAccum(aStepNm); \
          gpAttachMap->m_pStepArray->vDropWrite(); \
        } \
        i8 _iTscEntry = Synch::iTicks(); \
        mpTask->vPrimitive(aStepNm);
  
      #define EXIT() \
        ASSERT(pStepAccum); \
        i8 _iTscExit = Synch::iTicks(); \
        i8 _iTscDiff = _iTscExit - _iTscEntry; \
        pStepAccum->m_nInvoked++; \
        pStepAccum->m_nCycles += _iTscDiff;
    
    #else
  
      #define ENTRY(p1)
  
      #define EXIT()
        
    #endif
  };


  Task* _pTask(void* aAddr);
  Task* _pTask(i8 iAddr);
}

bool bTaskCompareQueueOrder(void* pObjectInsert, void* pObjectScan, bool& bMatch);
bool bTaskCompareWakeupTime(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
