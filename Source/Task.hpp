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
  
  class Task : public Focal,
               public ListItem_t<Task>
  {
  friend class Steps::Step;
  friend class Word;
  friend Scheduler;

  private:
    ReqTuple m_xReqTuple;   // request tuple
  
    Lock    m_okLock;       // lock for this task
    eTK     m_eKind;        // task kind
    union {
      Job*  m_pJob;         // the job for this task [TK_JOB] or...
      Op*   m_pOp;          // the op for this task  [TK_OP]
    };
  
    Thread*   m_pThread;    // current thread
    Mutex*    m_pMutex;     // mutex
  
    Session*  m_pSession;   // session for this task
    DicStack  m_oDicStack;  // dictionary stack

    eTS     m_eState;       // state (running, sleeping, ...)
    eTR     m_eTransition;  // state transition request
    eQK     m_eQueue;       // which queue the task is in
    i8      m_iQueueOrder;  // queue ordering number
    Decider m_oAcksRmg;     // number of acks still waiting for
    eLH     m_eDdlLocked;   // this task has the ddl lock?
    Sleep   m_oWakeup;      // wakeup time from sleeping

    i8      m_iTracing;     // tracing mask
    i8      m_i_NextRnd;    // which next repeatable random nummber?

    Node*     m_pSendNode;  // node to send to
    Compiler* m_pCompiler;  // forth word compiler (Job boot, create table etc)
  
    Forth*    m_pForth;     // forth implementation
  
    // exception
    s256  m_sException;
    s256  m_sFileNm;
    i8    m_iLineNo;

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
    void vAcksRmg(i8 nAcksRmg);
    bool bAcksRmgDec(void);
    void vAdjustFIP(i8 nBytes);
    Object* pArgObject(void);
    static void vDebug(void);
    Compiler* pCompiler(void);
    void vContentDump(void);
    void vDropTaskLock(void);
    void vException(Stz* szException, Stz* szFile, i8 iLineNo);
    void vFocalOutline(s4K& sOutline);
    void vGrabTaskLock(void);
    bool bIsLocked(void);
    bool bIsTaskLocked(void);
    Job* pJob(void);
    void vJob(Job* pJob);
    Stz* szJobName(void);
    eTK  eKind(void);
    void vLocalsPop(void);
    void vLocalsPush(void);
    void vMonitorPortrait(s48K& sPortrait);
    void vMutexLock(void);
    void vMutexSignal(void);
    void vMutexUnlock(void);
    void vMutexWait(void);
    void vNewStacks(void);
    i8&  i_NextRnd(void);
    Op*  pOp(void);
    eQK  eQueue(void);
    void vQueue(eQK eQueue);
    void vQueueOrder(i8 iTaskOrder);
    i8   iQueueOrder(void);
    void vRefindJobClass(void);
    ReqTuple* aReqTuple(void);
    void vRenderPortrait(s256& sTaskPortrait);
    void vReseed(void);
    void vRetry(void);
    bool bSeeded(void);
    void vSeedJob(void);
    Grids::Node* pSendNode(void);
    void vSendNode(Grids::Node* pSendNode);
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
    void vUnlinkJob(void);
    void vWakeupClear(void);
    u8   uWakeupInUsec(void);
    bool bWakeupNow(void);
    Sleep& oWakeupSleep(void);
    u8   uWakeupTime(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vAddBaseWords(void);
    void vAddMunged(MP_Task mpTask);
    void vAddRoutines(void);
    Word* pCompileColon(s48K& sDefinition);
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
