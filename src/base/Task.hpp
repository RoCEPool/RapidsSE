/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once

#include "eEP.h"
#include "eQK.h"
#include "eTR.h"
#include "eTS.h"
#include "TraceLock.hpp"

#include "LifeGuard_t.hpp"

namespace BASE
{  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tracing masks
  
  static const i8 C_TRACE_NONE     = 0x0;
  static const i8 C_TRACE_STEPS    = 0x1;
  static const i8 C_TRACE_ROUTINES = 0x2;
  
  typedef LatticeNL_t<CN_LOCKSET,Lock,C_MAX_LOCKS> LockSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  class Task : public Object,
               public ListItem_t<Task>,
               public LifeGuard_t<Task>
  {
  friend class STORE::Step;
  
  private:
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // a)

    // -- id, name and kind
    Gid<CN_TASK> m_xTaskId;  // task id in grid
    s32          m_sTaskNm;  // name of this task
    // n/a                   // kind

    // -- belongs to
    SMA<Job> m_pJob; // which job this task is executing
    SMA<Op>  m_pOp;  // whic this task is executing
  
    // -- refers to
    SMA<Thread>  m_pThread;  // current thread executing this task

    // -- state
    eTS m_eState;  // task state (running, sleeping, ...)

    // -- tags, seeding and tasking
    Word* m_pSeedWord;  // seed

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // b)

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // c)

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // d)

  
    // ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?

    Lock         m_okLock;       // lock for this task
   


    bool         m_bIsStart;     // is this the start task?
    bool         m_bIsImmediate; // is this an immediate task?

    eTR          m_eTransition;  // schedulre transition request
    eQK          m_eQueue;       // which queue the task is currently in
    eEP          m_eEntryPoint;  // task entry point
    
    Decider      m_oAcksRmg_task;     // number of acks still waiting for
    Wakeup       m_oWakeup;      // wakeup time from sleeping

    i8           m_iTraceMask;   // tracing mask
  
    i8           m_nOperandYield;  //
  
    Profile*     m_pProfile;     // runtime profile

    LockSet      m_oReadLocks;   // read locks currently held by this task/op
    LockSet      m_oWriteLocks;  // write locks currently held by this task/op
  
    LogicUnit*   m_p_TaskLogicUnit; // logic unit from job or op

  public:
    Word* m_pRecoveryWord;  // sen word called to recover from failure

    DicStack m_oDicStack;         // dictionary stack
    DicStack m_oSaveDicStack;    // save dictionary stack

    CodeCell* m_aInxPtr;         // instruction/cell pointer

    TypedStack*  m_pAuxStack;    // auxiliary stack
    TypedStack*  m_pDataStack;   // data stack
    TypedStack*  m_pReturnStack; // return stack

    LocalChain*  m_pLocalsFrame; // locals| | register frame chain

    SenCompiler* m_pSenCompiler; // sen word compiler
    Input*       m_pInput;       // input stream
  
    Exception    m_xException;

    // always last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Task(void);
   ~Task(void) noexcept(false);
    
    static Task* pThisTLSTask(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    static void vDebug(void);

    void vTaskLoop(void);
    void vExceptionHandler(void);
    void vExceptionTrace(void);

    void vDumpOnException(void);

  
    i8   iAcksOriginal(void);
    i8   iAcksRmg_assert_only_(void);
    void vAcksRmg(i8 nAcksRmg);
    bool bAcksRmgDec(i8& nAcksWereRemaining);
    void vAdjustFIP(i8 nBytes);
    Object* pArgObject(void);
    void vCheckBreakPt(void);
    bool bCorrectLayout(void);
    SenCompiler* pSenCompiler(void);
    void vContentDump(void);
    Word* pCurWord(void);
    void vDeleteInput(void);
    DicStack* pDicStack(void);
    void vDotPrint(Stz* szFormat, ...);
    void vDropAllLocks(void);
    void vDropTaskLock(Stz* szFunc);
    void vDump(void);
    void vEntryPoint(eEP eEntryPoint);
    eEP  eEntryPoint(void);
    Word* pEventHandler(void);
    void vGrabTaskLock(Stz* szFunc);
    bool bIsImmediate(void);
    void vIsImmediate(bool bIsImmediate);
    bool bIsLocked(void);
    bool bIsStart(void);
    void vIsStart(bool bIsStart);
    bool bIsTaskLocked(void);
    Job* pJob(void);
    void vJob(Job* pJob);
    void vLocalsPop(void);
    void vLocalsPush(void);
    void vMutexLock(void);
    void vMutexSignal(void);
    void vMutexUnlock(void);
    void vMutexWait(void);
    void vNewInput(void);
    Op*  pOp(void);
    void vOp(Op* pOp);
    Profile* pProfile(void);
    void vProfile(Profile* pProfile);
    eQK  eQueue(void);
    void vQueue(eQK eQueue);
    LockSet* pReadLocks(void);
    void vSetSenRecoverContext(void);
    void vReleaseLockSets(void);
    void vResetSenContext(void);
    void vReseed(void);
    DicStack* pSaveDicStack(void);
    void vSeeFrameStack(void);
    void vSeeLockSets(void);
    void vSeeLogicUnitRegisters(void);
    bool bSeeded(void);
    void vSeedTask(void);
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
    void vScheduleBide(void);
    void vScheduleDelete(void);
    void vScheduleSleep(void);
    void vScheduleYield(void);
    void vScheduleWait(void);
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
