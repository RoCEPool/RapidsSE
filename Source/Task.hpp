/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once

namespace Tasks
{  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // tracing masks
  static const i8 C_TRACE_NONE     = 0x0;
  static const i8 C_TRACE_STEPS    = 0x1;
  static const i8 C_TRACE_ROUTINES = 0x2;
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  class Task : public Object,
               public ListNode_t<Task>
  {
  friend class Steps::Step;
  friend class ForthWord;

  private:
    i8      m_iTaskId;      // task id
  
    Lock    m_okLock;       // lock for this task
    eTK     m_eKind;        // task kind
    union {
      Job*  m_pJob;         // the job for this task   [DDL]
      Op*   m_pOp;          // OR the op for this task [DML]
    };
  
    Thread*   m_pThread;    // current thread
    Session*  m_pSession;   // session for this task
    DicStack  m_oDicStack;    // dictionary stack

    eTS     m_eState;       // state (running, sleeping, ...)
    eTR     m_eTransition;  // state transition request
    eQK     m_eQueue;       // which queue the task is in
    i8      m_iQueueOrder;  // queue ordering number
    Decider m_oAcksRmg;     // number of acks still waiting for
    i8      m_iDdlLocked;   // has the ddl lock?
    Sleep   m_oWakeup;      // wakeup time from sleeping

    i8      m_iTracing;     // tracing mask
    i8      m_i_ActionRoot; // action root
    i8      m_i_NextRnd;    // which next repeatable random nummber?

    Compiler* m_pCompiler;  // forth word compiler (Job boot, create table etc)
    Node*     m_pSendNode;  // node to send to
  
    ForthClump* m_pForthClump;
  
    Cell*     m_aFipCell;     // points to cell inside a word
    LocalSet* m_pLocalsFrame; // local register frame chain
    Stack*    m_pDataStack;   // data stack
    Stack*    m_pReturnStack; // return stack
    Stack*    m_pAuxStack;    // auxiliary stack
  
    // exception
    s256  m_sFile;
    s256  m_sException;

    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Task(Job* pJob, Session* pSession);
    Task(Op* pOp, Session* pSession);
    void vInit(void);
    ~Task(void) noexcept(false);
    
    static Task* pThisTLSTask(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vMonitorProfile_stats(s48K& sPortrait);
  
    i8   iAcksRmg_assert_only_(void);
    void vAcksRmg(i8 iAcksRmg);
    bool bAcksRmgDec(void);
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
    eQK  eQueue(void);
    void vQueue(eQK eQueue);
    void vQueueOrder(i8 iTaskOrder);
    i8   iQueueOrder(void);
    void vRefindJobClass(void);
    void vRenderPortrait(s256& sTaskPortrait);
    void vRetry(void);
    bool bSeeded(void);
    void vSeedJob(void);
    Grids::Node* pSendNode(void);
    Session* pSession(void);
    void vSession(Session* pSession);
    void vSleepUsec(i8 iMicroSeconds);
    void vSprint(s48K& sFormat, s48K& sString);
    void vStackContents(Stack* pStack, s48K& sStackContents);
    eTS  eState(void);
    void vState(eTS eState);
    i8   iTaskId(void);
    void vTaskLoop_job(void);
    void vTaskLoop_op(void);
    Threads::Thread* pThread(void);
    void vThread(Threads::Thread* pThread);
    void vTracePoint(Stz* aMark);
    void vTraceStep(Stz* aStepNm);
    eTR  eTransition(void);
    void vTransition(eTR eTransition);
    void vTransitionClear(void);
    void vWakeupClear(void);
    u8   uWakeupInUsec(void);
    bool bWakeupNow(void);
    Sleep& oWakeupSleep(void);
    u8   uWakeupTime(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vAddBaseWords(void);
    void vAddMunged(MP_Task mpTask);
    void vAddRoutines(void);
    ForthWord* pCompileColon(s48K& sDefinition);
    void  vCompileConstant(s48K& sDefinition);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private:
    Footer* aFooter(void);
  };

  Task* _pTask(void* aAddr);
  Task* _pTask(i8 iAddr);
}

bool bTaskCompareQueueOrder(void* pObjectInsert, void* pObjectScan, bool& bMatch);
bool bTaskCompareWakeupTime(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
