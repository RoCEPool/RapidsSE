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
    ReqTuple     m_xReqTuple;    // request tuple
  
    Lock         m_okLock;       // lock for this task
    i8           m_iTraceMask;   // tracing mask

    SMA<Job>     m_pJob;         // the job for this task
    SMA<Thread>  m_pThread;      // current thread
    SMA<Mutex>   m_pMutex;       // mutex

    eTS          m_eState;       // state (running, sleeping, ...)
    eTR          m_eTransition;  // state transition request
    eQK          m_eQueue;       // which queue the task is currently in
    eLH          m_eDdlLocked;   // this task has the ddl lock?
  
    Decider      m_oAcksRmg;     // number of acks still waiting for
    Sleep        m_oWakeup;      // wakeup time from sleeping

    SMA<Forth>   m_pForth;       // forth execution context
    SMA<Op>      m_pOp;          // the op for this task
    SMA<RegSet>  m_pRegSet;      // set of registers for the task steps

    // always last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Task(Job* pJob);
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
    void vAssertStructure(void);
    void vBeginOp(void);
    void vCaught(Exception xException);
    bool bCorrectLayout(void);
    static void vDebug(void);
    Compiler* pCompiler(void);
    void vContentDump(void);
    void vDropTaskLock(void);
    void vFocalOutline(s4K& sOutline);
    void vGrabTaskLock(void);
    void vInitExeContext(void);
    bool bIsLocked(void);
    bool bIsTaskLocked(void);
    Job* pJob(void);
    void vJob(Job* pJob);
    Stz* szJobName(void);
    void vLocalsPop(void);
    void vLocalsPush(void);
    void vMonitorPortrait(s48K& sPortrait);
    void vMutexLock(void);
    void vMutexSignal(void);
    void vMutexUnlock(void);
    void vMutexWait(void);
    i8&  i_NextRnd(void);
    Op*  pOp(void);
    eQK  eQueue(void);
    void vQueue(eQK eQueue);
    void vRefindJobClass(void);
    RegSet* pRegSet(void);
    ReqTuple* aReqTuple(void);
    void vRenderPortrait(s256& sTaskPortrait);
    void vReseed(void);
    void vRetry(void);
    bool bSeeded(void);
    void vSeedTask(void);
    Grids::Node* pSendNode(void);
    void vSendNode(Grids::Node* pSendNode);
    void vSleepUsec(i8 iMicroSeconds);
    void vSprint(s48K& sFormat, s48K& sString);
    void vStackContents(Stack* pStack, s48K& sStackContents);
    eTS  eState(void);
    void vState(eTS eState);
    i8   iTaskId(void);
    void vTaskLoop(void);
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
    void vAddMunged(Forth* pForth);
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

bool bTaskCompareWakeupTime(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
