/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once

namespace Tasks
{  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  enum eTK // Task Kind
  {
  /* 0 */ TK_NONE=0,
  /* 1 */ TK_JOB,      // task is a job
  /* 2 */ TK_OPERATOR, // task is an operator
  /* 3 */ TK_ENUMS
  };
  
  static Stz* aTK[]
  {
  /* 0 */ "NONE"_Stz,
  /* 1 */ "JOB"_Stz,
  /* 2 */ "OPERATOR"_Stz,
  /* 3 */ "ENUMS"_Stz,
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  enum eTS // Task State
  {
  /* 0 */ TS_NONE=0,
  /* 1 */ TS_EXECUTING, // task is currenty being executed by Thread::vLoop
  /* 2 */ TS_EXITED,    // task has completed
  /* 3 */ TS_READY,     // task is ready to be executed
  /* 4 */ TS_SLEEPING,  // task is sleeping
  /* 5 */ TS_WAITING,   // task is waiting on ack from cooperating task
  /* 6 */ TS_YIELDED,   // task is yielded, not running and is in a TaskSet
  /* 7 */ TS_ENUMS
  };
  
  static Stz* aTS[]
  {
  /* 0 */ "NONE"_Stz,
  /* 1 */ "EXECUTING"_Stz,
  /* 2 */ "EXITED"_Stz,
  /* 3 */ "READY"_Stz,
  /* 4 */ "SEND"_Stz,
  /* 5 */ "WAITING"_Stz,
  /* 6 */ "YIELDED"_Stz,
  /* 7 */ "ENUMS"_Stz,
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  enum eTR // Task State Transition
  {
  /* 0 */ TR_NONE=0,
  /* 1 */ TR_EXIT,    // task to exit
  /* 2 */ TR_SLEEP,   // task to sleep
  /* 3 */ TR_WAIT,    // task to wait on ack from cooperating task
  /* 4 */ TR_YIELD,   // task to yield to other task
  /* 5 */ TR_ENUMS
  };
  
  static Stz* aTR[]
  {
  /* 0 */ "NONE"_Stz,
  /* 1 */ "EXIT"_Stz,
  /* 2 */ "SLEEP"_Stz,
  /* 3 */ "WAIT"_Stz,
  /* 4 */ "YIELD"_Stz,
  /* 5 */ "ENUMS"_Stz,
  };
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  class Task : public Object,
               public ListNode_t<Task>
  {
  friend class Steps::Step;
  friend class ForthWord;

  private:
    Lock    m_okLock;       // lock for this task
    eTK     m_eKind;        // task kind
    union {
      Job*  m_pJob;         // the job for this task   [DDL]
      Op*   m_pOp;          // OR the op for this task [DML]
    };
  
    Interface::Session* m_pSession; // session for this task
    Threads::Thread*    m_pThread;  // current thread

    eTS     m_eState;       // state (running, sleeping, ...)
    eTR     m_eTransition;  // state transition request
    eQK     m_eQueue;       // which queue the task is in
    i8      m_iQueueOrder;  // queue ordering number
    Decider m_oAcksRmg;     // number of acks still waiting for
    i8      m_iDdlLocked;   // has the ddl lock?
    Sleep   m_oSleepTime;   // sleep time helper

    i8      m_iTracing;     // tracing mask
    i8      m_i_ActionRoot; // action root
    i8      m_i_NextRnd;    // which next repeatable random nummber?

    Cell*     m_aFipCell;     // points to cell inside a word
    LocalSet* m_pLocalsFrame; // local register frame chain
    Stack*    m_pDataStack;   // data stack
    Stack*    m_pReturnStack; // return stack
    Stack*    m_pAuxStack;    // auxiliary stack
    DicStack  m_oDicStack;    // dictionary stack
    
    s256  m_sPortrait;      // short text description of task
  
    // exception
    s256  m_sFile;
    s256  m_sException;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // remove if we can
  
    Grids::Node* m_pSendNode; // node to send to
  
    Compiler* m_pCompiler;    // forth word compiler (Job boot)

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Task(Job* pJob, Interface::Session* pSession);
    Task(Op* pOp, Interface::Session* pSession);
    void vInit(void);
    ~Task(void) noexcept(false);
    
    static Task* pThisTLSTask(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vMonitorProfile_stats(s48K& sPortrait);
  
    bool bAckAndDecide(void);
    i8   iAcksRmg_assert_only_(void);
    void vAdjustFIP(i8 nBytes);
    Object* pArgObject(void);
    static void vDebug(void);
    void vContentDump(void);
    void vDropLock(void);
    void vException(s256& sFile, s256& eException);
    void vGrabLock(void);
    bool bIsLocked(void);
    Job* pJob(void);
    void vJob(Job* pJob);
    Stz* aJobName(void);
    eTK  eKind(void);
    void vLocalsPop(void);
    void vLocalsPush(void);
    void vMonitorPortrait(s48K& sPortrait);
    void vNewStacks(void);
    i8&  i_NextRnd(void);
    Op*  pOp(void);
    s256& sPortrait(void);
    eQK  eQueue(void);
    void vQueue(eQK eQueue);
    void vQueueOrder(i8 iTaskOrder);
    i8   iQueueOrder(void);
    void vRefindJobClass(void);
    void vRenderPortrait(void);
    void vRetry(void);
    bool bSeeded(void);
    void vSeedJob(void);
    Grids::Node* pSendNode(void);
    Interface::Session* pSession(void);
    void vSession(Interface::Session* pSession);
    void vSleepUsec(i8 iMicroSeconds);
    void vSprint(s48K& sFormat, s48K& sString);
    void vStackContents(Stack* pStack, s48K& sStackContents);
    eTS  eState(void);
    void vState(eTS eState);
    void vTaskLoop_job(void);
    void vTaskLoop_op(void);
    Threads::Thread* pThread(void);
    void vThread(Threads::Thread* pThread);
    void vTracePoint(Stz* aMark);
    void vTraceStep(Stz* aStepNm);
    eTR  eTransition(void);
    void vTransition(eTR eTransition);
    u8   uWakeupInUsec(void);
    bool bWakeupNow(void);
    u8   uWakeupTime(void);
    Sleep& oSleepTime(void);
    void vWakeupTask(void);

    /*------------------------------------------------------------------------*/

    void vAddBaseWords(void);
    void vAddMunged(MP_Task mpTask);
    void vAddRoutines(void);
    ForthWord* pCompileColon(s48K& sDefinition);
    void  vCompileConstant(s48K& sDefinition);

    /*------------------------------------------------------------------------*/
    
  private:
    Footer* aFooter(void);

    /*------------------------------------------------------------------------*/
    
  };

  Task* _pTask(void* aAddr);
  Task* _pTask(i8 iAddr);
}

bool bTaskCompareQueueOrder(void* pObjectInsert, void* pObjectScan, bool& bMatch);
bool bTaskCompareWakeupTime(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
