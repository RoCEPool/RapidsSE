/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once


#include "eQK.h"
#include "eTR.h"
#include "eTS.h"
#include "TraceLock.hpp"

namespace ENGINE
{  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tracing masks
  
  static const i8 C_TRACE_NONE     = 0x0;
  static const i8 C_TRACE_STEPS    = 0x1;
  static const i8 C_TRACE_ROUTINES = 0x2;
  
  typedef LatticeNL_t<CN_LOCKSET,Lock,C_MAX_LOCKS> LockSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  class Task : public Object,
               public ListItem_t<Task>
  {
  friend class STORE::Step;

  friend class STORE::Op;  // temp
  friend class STORE::Job; // temp

  private:
  
    i8 m_iDebugMark;

    Gid_t<CN_TASK>  m_xTaskId;  // task id in grid
    s32 m_sTaskNm;  // name of this task
    eTS m_eState;   // task state (running, sleeping, ...)
    b8  m_bDedicatedThread;
  
    SMA<Job,CN_JOB> m_pJob; // which job this task is executing
    SMA<Op,CN_OP>   m_pOp;  // ... or which op this task is executing

    SMA<Task, CN_TASK>  m_pInvokingTask;  // task that invoked this task

    SMA<Thread, CN_THREAD>  m_pThread;  // current thread executing this task

    Lock         m_okLock;        // lock for this task
   
    eTR          m_eTransition;   // schedulre transition request
    eQK          m_eQueue;        // which queue the task is currently in
    
    Decider      m_oAcksRmg;      // number of acks still waiting for
    Wakeup       m_oWakeup;       // wakeup time from sleeping

    i8           m_iTraceMask;    // tracing mask
  
    i8           m_nOperandYield; //
  
    Profile*     m_pProfile;      // runtime profile

    LockSet      m_oReadLocks;    // read locks currently held by this task/op
    LockSet      m_oWriteLocks;   // write locks currently held by this task/op
  
    LogicUnit*   m_pLogicUnit;    // logic unit

    Exception    m_xException;

    Word*        m_pRecoveryWord; // sen word called to recover from failure

    DicStack     m_oDicStack;     // dictionary stack
    DicStack     m_oSaveDicStack; // save dictionary stack

    CodeCell*    m_aInxPtr;       // instruction/cell pointer

    TypedStack*  m_pAuxStack;     // auxiliary stack
    TypedStack*  m_pDataStack;    // data stack
    TypedStack*  m_pReturnStack;  // return stack

    LocalChain*  m_pLocalsFrame;  // locals| | register frame chain

    SenCompiler* m_pSenCompiler;  // sen word compiler
    Input*       m_pInput;        // input stream

    // always last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Task(Stz* szTaskNm, Job* pJob);
    Task(Stz* szTaskNm, Op* pOp);
    void vTask(void);
   ~Task(void) noexcept(false);
    
    static Task* pThisTLSTask(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    static void vDebug(void);

    void vTaskLoop(void);
    void vExceptionHandler(void);
    void vExceptionTrace(void);

    void vDumpOnException(void);

    // - - - - - - - - - - - - - -  - - - - - -
  
    i8   iAcksOriginal(void);
    i8   iAcksRmg_assert_only_(void);
    void vAcksRmg(i8 nAcksRmg);
    bool bAcksRmgDec(i8& nAcksWereRemaining);
    void vAdjustFIP(i8 nBytes);
    Object* pArgObject(void);
  
    void vCheckBreakPt(void);
    bool bCorrectLayout(void);
    void vContentDump(void);
    void vCycleTaskLock(Stz* szFunc);
    Word* pCurWord(void);
  
    bool bDedicatedThread(void);
    void vDedicatedThread(bool bDedicatedThread);
    void vDeleteInput(void);
    DicStack* pDicStack(void);
    void vDotPrint(Stz* szFormat, ...);
    void vDropAllLocks(void);
    void vDropTaskLock(Stz* szFunc);
    void vDump(void);
  
    Word* pEventHandler(void);
  
    void vGrabTaskLock(Stz* szFunc);
  
    Task* pInvokingTask(void);
    void vInvokingTask(Task* pInvokingTask);
    bool bIsLocked(void);
    bool bIsTaskLocked(void);
  
    Job* pJob(void);
    void vJob(Job* pJob);
  
    void vLocalsPop(void);
    void vLocalsPush(void);
    LogicUnit* pLogicUnit(void);
  
    void vNewInput(void);
  
    Job* pOuterJobOfTask(void);
    Op*  pOp(void);
    void vOp(Op* pOp);
  
    Profile* pProfile(void);
    void vProfile(Profile* pProfile);
  
    eQK  eQueue(void);
    void vQueue(eQK eQueue);
  
    LockSet* pReadLocks(void);
    void vReleaseLockSets(void);
    void vResetSenContext(void);
    void vRetire(void);

    DicStack* pSaveDicStack(void);
    void vScheduleBide(void);
    void vScheduleHibernate(void);
    void vScheduleRetire(void);
    void vScheduleSleep(void);
    void vScheduleYield(void);
    void vScheduleWait(void);
    void vSeeFrameStack(void);
    void vSeeLockSets(void);
    void vSeeLogicUnitRegisters(void);
    SenCompiler* pSenCompiler(void);
    void vSetSenRecoverContext(void);
    void vSetInxPtr(Stz* szEntry);
    void vSetInxPtr(Word* pEntryWordCheck);
    void vSleepUsec(i8 iMicroSeconds);
    eTS  eState(void);
    void vState(eTS eState);
  
    i8   iTaskId(void);
    Stz* szTaskNm(void);
    void vTaskNm(Stz* szTaskNm);
    Thread* pvThread_DONT_DEREF(void);
    void vThread(Thread* pThread);
    void vTracePoint(Stz* aMark);
    void vTraceStep(Stz* aStepNm);
    eTR  eTransition(void);
    void vTranstionClear(void);
  
    i8   iWakeup(void);
    void vWakeupClear(void);
    u8   uWakeupInUsec(void);
    bool bWakeupNow(void);
    Wakeup& oWakeup(void);
    u8   uWakeupTime(void);
    LockSet* pWriteLocks(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vAddMunged(void);
    void vAddMunged(Task* pTask);
    void vAddAxioms(void);
    Word* pCompileAxiomColon(s48K& sDefinition);
    void  vCompileMungedColon(Stz* szDefinition);
    void  vCompileMungedColon(i8 i_Def);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Input* pInput(void);

  private:
    void vVerifyTaskComposition(void);

  private:
    Footer* aFooter(void);
  };

  Task* _pTask(void* aAddr);
  Task* _pTask(i8 iAddr);
}

bool bTaskCompareWakeup(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
