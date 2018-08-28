/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once

extern pthread_key_t  giTLS_TaskKey;

namespace Interface
{
  class SEitf;
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
  enum eAC // actor models
  {
    /*  0 */ AC_NONE=0,
    /*  1 */ AC_DRONE,
    /*  2 */ AC_PARTITION,
    /*  3 */ AC_SERVER,
    /*  4 */ AC_WORKER,

    /*  5 */ AC_R35,
  
    /*  6 */ AC_ENUMS
  };
  
  static Stz* aAC[]
  {
    /*  0 */ "NONE"_Stz,
    /*  1 */ "DRONE"_Stz,
    /*  2 */ "PARTITION"_Stz,
    /*  3 */ "SERVER"_Stz,
    /*  4 */ "WORKER"_Stz,
    /*  5 */ "R35"_Stz,
    /*  6 */ "ENUMS"_Stz
  };
  
  enum eTK // task kind
  {
  /*  0 */ TK_NONE=0,
  /*  1 */ TK_ANSWER_DRONE,
  /*  2 */ TK_LISTER_DRONE,
  /*  3 */ TK_CONNECTOR_DRONE,
  /*  4 */ TK_MONITOR_DRONE,
  /*  5 */ TK_RECLAIM_DRONE,
  /*  6 */ TK_RECEIVER_DRONE,
  /*  7 */ TK_REQUEST_SERVER,
  /*  8 */ TK_LOADSUB_SERVER,
  /*  9 */ TK_COORDINATOR_WORKER,
  /* 10 */ TK_COORDINATOR_PROXY_WORKER,
  /* 11 */ TK_PARTICIPANT_WORKER,
  /* 12 */ TK_PARTICIPANT_PROXY_WORKER,
  /* 13 */ TK_SENDER_WORKER,
  /* 14 */ TK_LOADSUB_PART,
  /* 15 */ TK_SELECT_PART,
  /* 16 */ TK_TRUNCATE_PART,
  
  /* 17 */ TK_R35_COMMAND,
  /* 18 */ TK_R35_CHILD,
  
  /* 19 */ TK_ENUMS
  };
  
  static Stz* aTK[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "ANSWER_DRONE"_Stz,
  /*  2 */ "LISTER_DRONE"_Stz,
  /*  3 */ "CONNECTOR_DRONE"_Stz,
  /*  4 */ "MONITOR_DRONE"_Stz,
  /*  5 */ "RECLAIM_DRONE"_Stz,
  /*  6 */ "RECEIVER_DRONE"_Stz,
  /*  7 */ "REQUEST_SERVER"_Stz,
  /*  8 */ "LOADSUB_SERVER"_Stz,
  /*  9 */ "COORDINATOR_WORKER"_Stz,
  /* 10 */ "COORDINATOR_PROXY_WORKER"_Stz,
  /* 11 */ "PARTICIPANT_WORKER"_Stz,
  /* 12 */ "PARTICIPANT_PROXY_WORKER"_Stz,
  /* 13 */ "SENDER_WORKER"_Stz,
  /* 14 */ "LOADSUB_PART"_Stz,
  /* 15 */ "SELECT_PART"_Stz,
  /* 16 */ "TRUNCATE_PART"_Stz,

  /* 17 */ "R35_COMMAND"_Stz,
  /* 18 */ "R35_CHILD"_Stz,
  
  /* 19 */ "ENUMS"_Stz
  };
  enum eTS // Task State
  {
  /* 0 */ TS_NONE=0,
  /* 1 */ TS_BLOCKED,       // task is blocked by another transaction
  /* 2 */ TS_EXECUTING,     // task is currenty being executed by Thread::vLoop
  /* 3 */ TS_EXITED,        // task has completed
  /* 4 */ TS_READY,         // task is ready to be executed
  /* 5 */ TS_SEND,          // task is wants to send
  /* 6 */ TS_SLEEPING,      // task is sleeping
  
  // task is waiting for signal 
  /* 7 */ TS_WAITING_ACK,   // task is waiting on ack from cooperating task
  /* 8 */ TS_WAITING_SIGNAL,// task is waiting on signal indicating queue arrival
  
  /* 9 */ TS_YIELDED,       // task is yielded, not running and is in a TaskSet
  /*10 */ TS_ENUMS
  };
  
  static Stz* aTS[]
  {
    /* 0 */ "NONE"_Stz,
    /* 1 */ "BLOCKED"_Stz,
    /* 2 */ "EXECUTING"_Stz,
    /* 3 */ "EXITED"_Stz,
    /* 4 */ "READY"_Stz,
    /* 5 */ "SEND"_Stz,
    /* 6 */ "SLEEPING"_Stz,
    /* 7 */ "WAITING_ACK"_Stz,
    /* 8 */ "WAITING_SIGNAL"_Stz,
    /* 9 */ "YIELDED"_Stz,
    /*10 */ "ENUMS"_Stz,
  };
  
  typedef NP<Task,CN_TASK> NP_Task;
  typedef MP<Task,CN_TASK> MP_Task;
  
  class Task : public Object,
               public ListNode_t<Task>
  {
  private:
    
    static const i8 C_TRACE_STEPS    = 0x1;
    static const i8 C_TRACE_ROUTINES = 0x2;
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // keep

    i8      m_iTaskNo;      // task
    bool    m_bDestructorTrace;
    Job*    m_pJob;         // the job for this task
    Task*   m_pParentTask;  // parent of this task

    i8      m_iTaskOrderNo; // task order no. (ready queue)
    i8      m_iPartition;   // which partition if a partition task
    eTS     m_eState;       // state (running, sleeping, ...)
    eQR     m_eQueue;       // which queue the task is in
    Atomic  m_zAcks;        // number of acks still waiting for
    Since   m_xWakeupTime;  // wakeup time
    i8      m_iDdlLocked;   // ? has the ddl lock?
  
    Object* m_pSeedObject;   // seed object
 
    Socket* m_pSocket;       // socket associated with task

    Threads::Thread* m_pThread; // current thread

    Stack*  m_pDataStack;   // data stack
    Stack*  m_pReturnStack; // return stack
    Stack*  m_pAuxStack;    // auxiliary stack
    DicStack m_xPathStack;   // dictionary stack
    
    RegFrame* m_pRegFrame;    // register frame chain
    RegFrame* m_pLocalsFrame; // local register frame chain
    RegFrame* m_pOpFrame;     // op frame chain

    union {
      Cell*   m_pCell;      // points to cell inside a word
      Value*  m_pValue;     // points to an embedded value within a word
      Object* m_pObject;    // points to an embedded object within a word
      //Header* m_pHeader;    // points to an embbeded header within a word
    } IP;
    TaskStep* m_pEntryCell; // points to does arg (entry cell of word)
  
    Cell* m_pResetIP;
    Cell* m_pBeginIP;
    Word* m_pWord;
    bool  m_bCompiling;
    i8    m_iTracing;
    i8    m_nVariables;      // number of variables 
    Word* m_pCurrentWord;
    i8    m_nSubDic;
    
    // exception
    s256  m_sFile;
    s256  m_sException;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // goes somewhere else
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // questionable
    Lock    m_xLock;       // lock for this task

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // remove
    eTK     m_eTaskKind;   // task kind
  
    s32     m_sName;       // task name
  
    i8      m_iTxId;       // transaction id
    i8      m_iWakeAt;     // time at which to wake this tx
  
    ValSet*    m_pRequest;   // request as a value set
    s256       m_sLambda;    // lambda input
    Tokeniser* m_pTokeniser; // input stream tokenizer
  
    Interface::SEitf* m_pSEitf; // the seitf for this task
  
    Queue*  m_pJobQueue;        // job queue that this task serves
                                //   may or may not be shared
  
    GridModel::Node* m_pSendNode; // node to send to
    Plan*   m_pPlan;            // the plan associated with task

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // always last
  
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void* operator new(size_t, Task* pTask);
    void  operator delete(void*);
    
    Task(void);
    Task(Value* pRequest);
    Task(MP_Task mpTask);
    Task(Stz* aLambda);
    void vInit(Stz* aLambda);
    ~Task(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vAdjustNext(i8 iBytes);
    void vAddConstant(Stz* aName, i8 iVal);
    void vAddConstant(Stz* aName, Stz* aValue);
    void vAddConstants(void);
    void vAddMunged(MP_Task mpTask);
    void vAddPrimitive(Stz* aName, TaskStep pStep, bool bImmediate);
    void vAddRoutines(void);
    Object* pArgObject(void);
    Word* pCompileColon(StrVal& sDefinition);
    void  vCompileConstant(StrVal& sDefinition);
    static void vDebug(void);
    void vContentDump(void);
    bool bDecAck(i8& iRemaining);
    void vDestructorTrace(bool bDestructorTrace);
    bool bDestructorTrace(void);
    void vDropLock(void);
    void vException(StrVal& sFile, StrVal& eException);
    void vGrabLock(void);
    void vInitialiseMain(void);
    bool bIsLocked(void);
    bool bIsName(StrVal& sName);
    bool bIsNonPartitioned(void);
    bool bIsPartitioned(void);
    Job* pJob(void);
    void vJob(Job* pJob);
    void vLocalsPop(void);
    void vLocalsPush(void);
    StrVal&   sName(void);
    void vName(StrVal& sName);
    Task*  pParentTask(void);
    void vParentTask(Task* pParentTask);
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
    Value* pRequest(void);
    void vReset(void);
    void vRetry(void);
    void vSeedDicCompileSen(void);
    void vSeedDocol(void);
    Object* pSeedObject(void);
    void vSeedObject(Object* pSeedObject);
    void vSeedWord(Stz *aWord);
    GridModel::Node* pSendNode(void);
    Interface::SEitf* pSEitf(void);
    void vSEitf(Interface::SEitf* pSEitf);
    void vSleep(i8 iMicroSeconds);
    i8   iSlot(void);
    void vSprint(s48K& sFormat, s48K& sString);
    void vStackContents(Stack* pStack, StrVal& sStackContents);
    eTS  eState(void);
    void vState(eTS eState);
    eTK  eTaskKind(void);
    void vTaskKind(eTK eTaskKind);
    void vTaskLoop(void);
    i8   iTaskNo(void);
    void vTaskOrderNo(i8 iTaskOrder);
    i8   iTaskOrderNo(void);
    Threads::Thread* pThread(void);
    void vThread(Threads::Thread* pThread);
    void vTracePoint(Stz* aMark);
    i8   iTxId(void);
    void vTxId(i8 iTxId);
    bool bWakeupNow(void);
    i8   iWakeupTime(void);
    void vWakeupTask(void);
    eQR  eWhichQueue(void);
  
    void vAddPrimitives(void);
    void vAddPrimitives_AccessPath(void);
    void vAddPrimitives_Accum(void);
    void vAddPrimitives_Address(void);
    void vAddPrimitives_Args(void);
    void vAddPrimitives_Assert(void);
    void vAddPrimitives_Astack(void);
    void vAddPrimitives_AttachMap(void);
  
    void vAddPrimitives_Bp(void);
  
    void vAddPrimitives_Cast(void);
    void vAddPrimitives_Class(void);
    void vAddPrimitives_Column(void);
    void vAddPrimitives_CommandProcessor(void);
    void vAddPrimitives_Comment(void);
    void vAddPrimitives_Common(void);
    void vAddPrimitives_Cpu(void);
    void vAddPrimitives_Cursor(void);
  
    void vAddPrimitives_DdlLock(void);
    void vAddPrimitives_Debug(void);
    void vAddPrimitives_Dic(void);
    void vAddPrimitives_DicLock(void);
    void vAddPrimitives_DicPath(void);
    void vAddPrimitives_DicStack(void);
    void vAddPrimitives_Dir(void);
    void vAddPrimitives_Docol(void);
    void vAddPrimitives_Dot(void);
    void vAddPrimitives_Dstack(void);
  
    void vAddPrimitives_Encode(void);
    void vAddPrimitives_Entry(void);
    void vAddPrimitives_EntryBuffer(void);
    void vAddPrimitives_Enter(void);
    void vAddPrimitives_Env(void);
    void vAddPrimitives_Error(void);
    void vAddPrimitives_Exit(void);
  
    void vAddPrimitives_FetchTest(void);
    void vAddPrimitives_File(void);
    void vAddPrimitives_Flow(void);
  
    void vAddPrimitives_Grid(void);
    void vAddPrimitives_GroupBy(void);
  
    void vAddPrimitives_Index(void);
    void vAddPrimitives_Input(void);
    void vAddPrimitives_Int(void);
  
    void vAddPrimitives_Job(void);
    void vAddPrimitives_JobClass(void);
  
    void vAddPrimitives_Loader(void);
    void vAddPrimitives_LoaderSub(void);
    void vAddPrimitives_Locals(void);
    void vAddPrimitives_Lock(void);
    void vAddPrimitives_Locks(void);
    void vAddPrimitives_Logic(void);
  
    void vAddPrimitives_Magnitude(void);
    void vAddPrimitives_Memory(void);
  
    void vAddPrimitives_Node(void);
  
    void vAddPrimitives_Object(void);
    void vAddPrimitives_OpFrame(void);
  
    void vAddPrimitives_PartitionScheduler(void);
    void vAddPrimitives_Persistence(void);
    void vAddPrimitives_Plan(void);
    void vAddPrimitives_PlanList(void);
    void vAddPrimitives_Planner(void);
    void vAddPrimitives_Process(void);
    void vAddPrimitives_Prompt(void);
    void vAddPrimitives_Push(void);
  
    void vAddPrimitives_Queue(void);
    void vAddPrimitives_Queues(void);
  
    void vAddPrimitives_RbCursor(void);
    void vAddPrimitives_Reg(void);
    void vAddPrimitives_Row(void);
    void vAddPrimitives_RowBuffer(void);
    void vAddPrimitives_RowCursor(void);
    void vAddPrimitives_Rstack(void);
  
    void vAddPrimitives_Scheduler(void);
    void vAddPrimitives_SEitf(void);
    void vAddPrimitives_SelCursor(void);
    void vAddPrimitives_Semaphore(void);
    void vAddPrimitives_ServerScheduler(void);
    void vAddPrimitives_Shell(void);
    void vAddPrimitives_Slots(void);
    void vAddPrimitives_Socket(void);
    void vAddPrimitives_Steps(void);
    void vAddPrimitives_Str(void);
  
    void vAddPrimitives_Table(void);
    void vAddPrimitives_TablePartition(void);
    void vAddPrimitives_Task(void);
    void vAddPrimitives_TaskCursor(void);
    void vAddPrimitives_Thread(void);
    void vAddPrimitives_Timer(void);
    void vAddPrimitives_To(void);
    void vAddPrimitives_Trace(void);
    void vAddPrimitives_Tx(void);
    void vAddPrimitives_TxSet(void);
  
    void vAddPrimitives_Vector(void);
  
    void vAddPrimitives_Utility(void);
  
    void vAddPrimitives_Word(void);
    void vAddPrimitives_WorkingBuffer(void);
  
    void vAddPrimitives_Zero(void);

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
  
    // address
    static void vAddressEqual(MP_Task mpTask);
    static void vAddressFetch(MP_Task mpTask);
    static void vAddressStore(MP_Task mpTask);
    
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
    static void vAttachMapMaxQueueFetch(MP_Task mpTask);
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
  
    static void vEntryBufferUnitTest_c(MP_Task mpTask);
  
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
    static void vGridAddNodes(MP_Task mpTask);
    static void vGridInit(MP_Task mpTask);
    static void vGridNode(MP_Task mpTask);
    static void vGridNodes(MP_Task mpTask);
    static void vGridPartitions(MP_Task mpTask);
    static void vGridSlots(MP_Task mpTask);
    static void vGridStartedStore(MP_Task mpTask);
    static void vGridStartedFetch(MP_Task mpTask);
    static void vGridStartNodeStore(MP_Task mpTask);
    static void vGridStartNodeFetch(MP_Task mpTask);
    static void vGridThisNodeStore(MP_Task mpTask);
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
  
    // index
    static void vIndexAnalyze(MP_Task mpTask);
    static void vIndexBuffer(MP_Task mpTask);
    static void vIndexColumnFetch(MP_Task mpTask);
    static void vIndexColumnCountFetch(MP_Task mpTask);
    static void vIndexColumnName(MP_Task mpTask);
    static void vIndexColumnType(MP_Task mpTask);
    static void vIndexCompact(MP_Task mpTask);
    static void vIndexCreateColumnVec(MP_Task mpTask);
    static void vIndexCreateColumnVecR35(MP_Task mpTask);
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
    static void vInputAccept(MP_Task mpTask);
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
    
    // job
    static void vJobAcksRemaining(MP_Task mpTask);
    static void vJobAddOriginator(MP_Task mpTask);
    static void vJobAddParticipant(MP_Task mpTask);
    static void vJobAncilliaryFetch(MP_Task mpTask);
    static void vJobAnalyzeIndexIsDetailed(MP_Task mpTask);
    static void vJobAnalyzeTableIsDetailed(MP_Task mpTask);
    static void vJobBegunErrorFetch(MP_Task mpTask);
    static void vJobBegunErrorStore(MP_Task mpTask);
    static void vJobCancelledErrorFetch(MP_Task mpTask);
    static void vJobCancelledErrorStore(MP_Task mpTask);
    static void vJobCheckResourcesFetch(MP_Task mpTask);
    static void vJobClass(MP_Task mpTask);
    static void vJobColumnCountFetch(MP_Task mpTask);
    static void vJobColumnNameFetch(MP_Task mpTask);
    static void vJobColumnIsPartKey(MP_Task mpTask);
    static void vJobColumnTypeFetch(MP_Task mpTask);
    static void vJobCompactIndexTableNameFetch(MP_Task mpTask);
    static void vJobDecAcksRemaining(MP_Task mpTask);
    static void vJobDelete(MP_Task mpTask);
    static void vJobDicPath(MP_Task mpTask);
    static void vJobErrorStore(MP_Task mpTask);
    static void vJobErrorFetch(MP_Task mpTask);
    static void vJobExecuteFetch(MP_Task mpTask);
    static void vJobExecutedErrorFetch(MP_Task mpTask);
    static void vJobExecutedErrorStore(MP_Task mpTask);
    static void vJobExecutedResponseFetch(MP_Task mpTask);
    static void vJobExecutedResponseStore(MP_Task mpTask);
    static void vJobExecutedRowsProcessedFetch(MP_Task mpTask);
    static void vJobExecutedRowsProcessedStore(MP_Task mpTask);
    static void vJobFetchRowsLastBuffersStore(MP_Task mpTask);
    static void vJobFetchRowsPlannerStore(MP_Task mpTask);
    static void vJobFetchRowsSpawnSub(MP_Task mpTask);
    static void vJobFetchRowsTableStore(MP_Task mpTask);
    static void vJobFetchRowsWhereFetch(MP_Task mpTask);
    static void vJobFindOriginator(MP_Task mpTask);
    static void vJobFindParticipant(MP_Task mpTask);
    static void vJobHasError(MP_Task mpTask);
    static void vJobIdFetch(MP_Task mpTask);
    static void vJobIndexNameFetch(MP_Task mpTask);
    static void vJobIsPromptCommand(MP_Task mpTask);
    static void vJobJobClass(MP_Task mpTask);
    static void vJobLoadBufferInsertRows(MP_Task mpTask);
    static void vJobLoadBufferLoadJobIdFetch(MP_Task mpTask);
    static void vJobLoadBufferPartitionFetch(MP_Task mpTask);
    static void vJobLoadBufferTableStore(MP_Task mpTask);
    static void vJobLoadTableTaskFetch(MP_Task mpTask);
    static void vJobLoadTableTaskStore(MP_Task mpTask);
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
    static void vJobPromptTaskFetch(MP_Task mpTask);
    static void vJobRecv(MP_Task mpTask);
    static void vJobReleaseResourcesFetch(MP_Task mpTask);
    static void vJobPartitionsFetch(MP_Task mpTask);
    static void vJobReceiverConnNoFetch(MP_Task mpTask);
    static void vJobReceiverConnNoStore(MP_Task mpTask);
    static void vJobReceiverSocketFetch(MP_Task mpTask);
    static void vJobReceiverSocketStore(MP_Task mpTask);
    static void vJobRegionSizeFetch(MP_Task mpTask);
    static void vJobRemoveOriginator(MP_Task mpTask);
    static void vJobRemoveParticipant(MP_Task mpTask);
    static void vJobResponseStore(MP_Task mpTask);
    static void vJobResponseFetch(MP_Task mpTask);
    static void vJobRowsProcessedFetch(MP_Task mpTask);
    static void vJobRowsProcessedStore(MP_Task mpTask);
    static void vJobSend(MP_Task mpTask);
    static void vJobShow(MP_Task mpTask);
    static void vJobSignalConnector(MP_Task mpTask);
    static void vJobSpawn(MP_Task mpTask);
    static void vJobSubFetchBufferQueueFetch(MP_Task mpTask);
    static void vJobSubFetchPhraseFetch(MP_Task mpTask);
    static void vJobSubFetchPlannerFetch(MP_Task mpTask);
    static void vJobSubFetchSlotFetch(MP_Task mpTask);
    static void vJobSubTruncateSlotFetch(MP_Task mpTask);
    static void vJobSubTruncateTruncateJobIdFetch(MP_Task mpTask);
    static void vJobThis(MP_Task mpTask);
    static void vJobTraceStore(MP_Task mpTask);
    static void vJobTraceFetch(MP_Task mpTask);
    static void vJobTruncateTableSpawnSub(MP_Task mpTask);
    static void vJobTruncateTableTaskFetch(MP_Task mpTask);
    static void vJobTruncateTableTaskStore(MP_Task mpTask);
    static void vJobWorkingBufferFetch(MP_Task mpTask);
    static void vJobWorkingBufferStore(MP_Task mpTask);

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

    // loader
    static void vLoaderAddRecordsInvalid(MP_Task mpTask);
    static void vLoaderAddRecordsValid(MP_Task mpTask);
    static void vLoaderChunkFile(MP_Task mpTask);
    static void vLoaderDelete(MP_Task mpTask);
    static void vLoaderDumpMap(MP_Task mpTask);
    static void vLoaderFindChunk(MP_Task mpTask);
    static void vLoaderInit(MP_Task mpTask);
    static void vLoaderNew(MP_Task mpTask);
    static void vLoaderNewSub(MP_Task mpTask);
    static void vLoaderPathStore(MP_Task mpTask);
    static void vLoaderPathDistributedStore(MP_Task mpTask);
    static void vLoaderRecordsInvalidFetch(MP_Task mpTask);
    static void vLoaderRecordsValidFetch(MP_Task mpTask);
    static void vLoaderSEitfFetch(MP_Task mpTask);
    static void vLoaderSEitfStore(MP_Task mpTask);
    static void vLoaderSpawnSubLoader(MP_Task mpTask);
    static void vLoaderTableStore(MP_Task mpTask);
    
    // loader sub
    static void vLoaderSubBegin(MP_Task mpTask);
    static void vLoaderSubDanglingBuffer(MP_Task mpTask);
    static void vLoaderSubDelete(MP_Task mpTask);
    static void vLoaderSubEnd(MP_Task mpTask);
    static void vLoaderSubFetchRecord(MP_Task mpTask);
    static void vLoaderSubFetchRecordDist(MP_Task mpTask);
    static void vLoaderSubFieldDelimiterStore(MP_Task mpTask);
    static void vLoaderSubHashPartition(MP_Task mpTask);
    static void vLoaderSubHashRecord(MP_Task mpTask);
    static void vLoaderSubInit(MP_Task mpTask);
    static void vLoaderSubInsertRow(MP_Task mpTask);
    static void vLoaderSubLoadData(MP_Task mpTask);
    static void vLoaderSubLoadNodeData(MP_Task mpTask);
    static void vLoaderSubLoaderFetch(MP_Task mpTask);
    static void vLoaderSubLoaderStore(MP_Task mpTask);
    static void vLoaderSubNew(MP_Task mpTask);
    static void vLoaderSubNewWorkingBuffer(MP_Task mpTask);
    static void vLoaderSubPartitionFetch(MP_Task mpTask);
    static void vLoaderSubPartitionStore(MP_Task mpTask);
    static void vLoaderSubProcessRecords(MP_Task mpTask);
    static void vLoaderSubRecordsInvalid(MP_Task mpTask);
    static void vLoaderSubRecordsValid(MP_Task mpTask);
    static void vLoaderSubSpawnLoadBuffer(MP_Task mpTask);
    static void vLoaderSubTable(MP_Task mpTask);
    static void vLoaderSubWorkingBufferFetch(MP_Task mpTask);
    static void vLoaderSubWorkingBufferStore(MP_Task mpTask);
    
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
    static void vNodeSize(MP_Task mpTask);
    static void vNodeSocketStore(MP_Task mpTask);
    static void vNodeSocketFetch(MP_Task mpTask);
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
    static void vPromptSEitf(MP_Task mpTask);

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
    
    // registers
    static void vRegPop(MP_Task mpTask);
    static void vRegPush(MP_Task mpTask);
    static void vRegReg0(MP_Task mpTask);
    static void vRegReg1(MP_Task mpTask);
    static void vRegReg2(MP_Task mpTask);
    static void vRegReg3(MP_Task mpTask);
    static void vRegReg4(MP_Task mpTask);
    static void vRegReg5(MP_Task mpTask);
    static void vRegReg6(MP_Task mpTask);
    static void vRegReg7(MP_Task mpTask);
    static void vRegReg8(MP_Task mpTask);
    static void vRegReg9(MP_Task mpTask);
    static void vRegRegA(MP_Task mpTask);
    static void vRegRegB(MP_Task mpTask);
    static void vRegRegC(MP_Task mpTask);
    static void vRegRegD(MP_Task mpTask);
    static void vRegRegE(MP_Task mpTask);
    static void vRegRegF(MP_Task mpTask);
    static void vRegRegNth(MP_Task mpTask);
    static void vRegToReg0(MP_Task mpTask);
    static void vRegToReg1(MP_Task mpTask);
    static void vRegToReg2(MP_Task mpTask);
    static void vRegToReg3(MP_Task mpTask);
    static void vRegToReg4(MP_Task mpTask);
    static void vRegToReg5(MP_Task mpTask);
    static void vRegToReg6(MP_Task mpTask);
    static void vRegToReg7(MP_Task mpTask);
    static void vRegToReg8(MP_Task mpTask);
    static void vRegToReg9(MP_Task mpTask);
    static void vRegToRegA(MP_Task mpTask);
    static void vRegToRegB(MP_Task mpTask);
    static void vRegToRegC(MP_Task mpTask);
    static void vRegToRegD(MP_Task mpTask);
    static void vRegToRegE(MP_Task mpTask);
    static void vRegToRegF(MP_Task mpTask);
    static void vRegToRegNth(MP_Task mpTask);
    
    // row
    static void vRowString(MP_Task mpTask);
    static void vRowEscapedString(MP_Task mpTask);
    
    // rowBuffer
    static void vRowBufferAppend(MP_Task mpTask);
    static void vRowBufferAppendRecord(MP_Task mpTask);
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
    
  
    // seitf
    static void vSEitfAccessPathFetch(MP_Task mpTask);
    static void vSEitfAccessPathStore(MP_Task mpTask);
    static void vSEitfAcksRemainingDec(MP_Task mpTask);
    static void vSEitfAcksRemainingFetch(MP_Task mpTask);
    static void vSEitfAcksRemainingStore(MP_Task mpTask);
    static void vSEitfBufferQueueFetch(MP_Task mpTask);
    static void vSEitfBufferQueueStore(MP_Task mpTask);
    static void vSEitfCommandProcessor(MP_Task mpTask);
    static void vSEitfDelete(MP_Task mpTask);
    static void vSEitfDicStack(MP_Task mpTask);
    static void vSEitfDisplayFetch(MP_Task mpTask);
    static void vSEitfDisplayStore(MP_Task mpTask);
    static void vSEitfDump(MP_Task mpTask);
    static void vSEitfEntryFetch(MP_Task mpTask);
    static void vSEitfEntryStore(MP_Task mpTask);
    static void vSEitfFetchTaskEnded(MP_Task mpTask);
    static void vSEitfFieldDelimiterFetch(MP_Task mpTask);
    static void vSEitfGroupByFetch(MP_Task mpTask);
    static void vSEitfGroupByStore(MP_Task mpTask);
    static void vSEitfIndexBufferFetch(MP_Task mpTask);
    static void vSEitfIndexBufferStore(MP_Task mpTask);
    static void vSEitfIndexFetch(MP_Task mpTask);
    static void vSEitfIndexStore(MP_Task mpTask);
    static void vSEitfIsExiting(MP_Task mpTask);
    static void vSEitfLastBuffersFetch(MP_Task mpTask);
    static void vSEitfLastBuffersStore(MP_Task mpTask);
    static void vSEitfPartitionFetch(MP_Task mpTask);
    static void vSEitfPartitionStore(MP_Task mpTask);
    static void vSEitfPid(MP_Task mpTask);
    static void vSEitfPlannerFetch(MP_Task mpTask);
    static void vSEitfPlannerStore(MP_Task mpTask);
    static void vSEitfRowsInvalidStore(MP_Task mpTask);
    static void vSEitfRowsProcessed(MP_Task mpTask);
    static void vSEitfRowsValidStore(MP_Task mpTask);
    static void vSEitfSlotFetch(MP_Task mpTask);
    static void vSEitfTableFetch(MP_Task mpTask);
    static void vSEitfTableRef(MP_Task mpTask);
    static void vSEitfTableDeref(MP_Task mpTask);
    static void vSEitfTxFetch(MP_Task mpTask);
    static void vSEitfTxStore(MP_Task mpTask);
  
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
    static void vSlotsCreateColumnVecR35(MP_Task mpTask);
    static void vSlotsSubDic(MP_Task mpTask);
    
    // socket
    static void vSocketAccept(MP_Task mpTask);
    static void vSocketClose(MP_Task mpTask);
    static void vSocketConnect(MP_Task mpTask);
    static void vSocketListen(MP_Task mpTask);
    static void vSocketOpen(MP_Task mpTask);
    static void vSocketRecvObject(MP_Task mpTask);
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
    
    // table
    static void vTableAddIndex(MP_Task mpTask);
    static void vTableAnalyze(MP_Task mpTask);
    static void vTableAppendBuffer(MP_Task mpTask);
    static void vTableColumnCountFetch(MP_Task mpTask);
    static void vTableCreateColumnVec(MP_Task mpTask);
    static void vTableCreateColumnVecR35(MP_Task mpTask);
    static void vTableDrop(MP_Task mpTask);
    static void vTableDump(MP_Task mpTask);
    static void vTableFirstBuffer(MP_Task mpTask);
    static void vTableGetColumnsAsString(MP_Task mpTask);
    static void vTableColumnFetch(MP_Task mpTask);
    static void vTableGetPtn(MP_Task mpTask);
    static void vTableGetRef(MP_Task mpTask);
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
    static void vTableRef(MP_Task mpTask);
    static void vTableRemoveIndex(MP_Task mpTask);
    static void vTableRetRef(MP_Task mpTask);
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
    static void vTaskAcksFetch(MP_Task mpTask);
    static void vTaskAcksStore(MP_Task mpTask);
    static void vTaskBreakPoint(MP_Task mpTask);
    static void vTaskDelete(MP_Task mpTask);
    static void vTaskDestructorTraceFetch(MP_Task mpTask);
    static void vTaskDestructorTraceStore(MP_Task mpTask);
    static void vTaskDicStack(MP_Task mpTask);
    static void vTaskDropLock(MP_Task mpTask);
    static void vTaskDump(MP_Task mpTask);
    static void vTaskGrabLock(MP_Task mpTask);
    static void vTaskInit(MP_Task mpTask);
    static void vTaskJobFetch(MP_Task mpTask);
    static void vTaskJobStore(MP_Task mpTask);
    static void vTaskJobQueueFetch(MP_Task mpTask);
    static void vTaskJobQueueStore(MP_Task mpTask);
    static void vTaskKindFetch(MP_Task mpTask);
    static void vTaskKindStore(MP_Task mpTask);
    static void vTaskNameFetch(MP_Task mpTask);
    static void vTaskNameStore(MP_Task mpTask);
    static void vTaskThis(MP_Task mpTask);
    static void vTaskParentFetch(MP_Task mpTask);
    static void vTaskParentStore(MP_Task mpTask);
    static void vTaskPartitionFetch(MP_Task mpTask);
    static void vTaskPartitionStore(MP_Task mpTask);
    static void vTaskPlanFetch(MP_Task mpTask);
    static void vTaskPlanStore(MP_Task mpTask);
    static void vTaskRequest(MP_Task mpTask);
    static void vTaskSeed(MP_Task mpTask);
    static void vTaskSeitfFetch(MP_Task mpTask);
    static void vTaskSeitfStore(MP_Task mpTask);
    static void vTaskSend(MP_Task mpTask);
    static void vTaskSendNodeFetch(MP_Task mpTask);
    static void vTaskSendNodeStore(MP_Task mpTask);
    static void vTaskSleep(MP_Task mpTask);
    static void vTaskSocketFetch(MP_Task mpTask);
    static void vTaskSocketStore(MP_Task mpTask);
    static void vTaskStateFetch(MP_Task mpTask);
    static void vTaskStateStore(MP_Task mpTask);
    static void vTaskThread(MP_Task mpTask);
    static void vTaskWait(MP_Task mpTask);
    static void vTaskYield(MP_Task mpTask);
        
    
    // taskQueue
    static void vTaskFifoAppend(MP_Task mpTask);
    static void vTaskFifoDump(MP_Task mpTask);
    static void vTaskFifoInit(MP_Task mpTask);
    static void vTaskFifoNew(MP_Task mpTask);
    
    // thread
    static void vThreadBind(MP_Task mpTask);
    static void vThreadCondSignal(MP_Task mpTask);
    static void vThreadCondWait(MP_Task mpTask);
    static void vThreadCpu(MP_Task mpTask);
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
    static void vToSEitf(MP_Task mpTask);
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
  
    // vector
    static void vVectorDifference(MP_Task mpTask);
    
    // word
    static void vAllParticipants(MP_Task mpTask);
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
    static void vIndirect(MP_Task mpTask);
    static void vImmediate(MP_Task mpTask);
    static void vLocalCalls(MP_Task mpTask);
    static void vLocate(MP_Task mpTask);
    static void vOneParticipant(MP_Task mpTask);
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
    static void vWorkingBufferToWorkingBuffer(MP_Task mpTask);
    
    // zero
    static void vZeroLess(MP_Task mpTask);
    static void vZeroLessEqual(MP_Task mpTask);
    static void vZeroEqual(MP_Task mpTask);
    static void vZeroNotEqual(MP_Task mpTask);
    static void vZeroGreaterEqual(MP_Task mpTask);
    static void vZeroGreater(MP_Task mpTask);
    
  private:
    static bool   bAppendRow(WorkingBuffer* pWorkingBuffer, Entry* pEntry, Interface::SEitf* pSEitf);
    static bool   bAppendRow(FetchBuffer* pFetchBuffer, Entry* pEntry, Interface::SEitf* pSEitf);
    //static bool   bInsertRow(EntryBuffer* pIndexBuffer, Entry* pEntry, Interface::SEitf* pSEitf);
    
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

bool bTaskCompareTaskOrderNo(void* pObjectInsert, void* pObjectScan, bool& bMatch);
bool bTaskCompareWakeupTime(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
