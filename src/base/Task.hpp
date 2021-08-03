/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once

#include "eEP.h"
#include "eQK.h"
#include "eTR.h"
#include "eTS.h"
#include "TraceLock.hpp"

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
               public ListItem_t<Task>
  {
  friend class STORE::Step;

  private:
    Lock         m_okLock;       // lock for this task
  
    Gid<CN_TASK> m_xTaskId;      // tasks object id

    SMA<Thread>  m_pThread;      // current thread executing this task
    SMA<Job>     m_pJob;         // job this task is executing
    SMA<Op>      m_pOp;          // job this task is executing
    LogicUnit*   m_pLogicUnit_task; // set of registers for the task steps

    bool         m_bIsStart;     // is this the start task?
    bool         m_bIsImmediate; // is this an immediate task?

    eTS          m_eState;       // state (running, sleeping, ...)
    eTR          m_eTransition;  // schedulre transition request
    eQK          m_eQueue;       // which queue the task is currently in
    eEP          m_eEntryPoint;  // task entry point
    
    Decider      m_oAcksRmg;     // number of acks still waiting for
    Sleep        m_oWakeup;      // wakeup time from sleeping

    i8           m_iTraceMask;   // tracing mask
  
    i8           m_nOperandYield;  //
  
    Profile*     m_pProfile;     // runtime profile

    LockSet      m_oReadLocks;   // read locks currently held by this task/op
    LockSet      m_oWriteLocks;  // write locks currently held by this task/op

  public:
    Word* m_pSeedWord;      // seed
    Word* m_pRecoveryWord;  // sen word called to recover from failure

    bool  m_bSeedInitially; // seed_initially sentinel has been executed
    bool  m_bDoneFinally;   // done_finally sentinel has been executed

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
  
  
    void vTaskLoop(void);
    void vExceptionHandler(Exception xException);
    void vExceptionTrace(void);
    void vContinueAfterFailure(void);

    void vDumpOnFailure(void);

  
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
    static void vDebug(void);
    DicStack* pDicStack(void);
    void vDropAllLocks(void);
    void vDropTaskLock(void);
    void vDump(void);
    void vEntryPoint(eEP eEntryPoint);
    eEP  eEntryPoint(void);
    Word* pEventHandler(void);
    void vGrabTaskLock(void);
    void vInitExeContext(void);
    bool bIsImmediate(void);
    void vIsImmediate(bool bIsImmediate);
    bool bIsLocked(void);
    bool bIsStart(void);
    void vIsStart(bool bIsStart);
    bool bIsTaskLocked(void);
    Job* pJob(void);
    void vJob(Job* pJob);
    Stz* szJobName(void);
    void vLocalsPop(void);
    void vLocalsPush(void);
    LogicUnit* pLogicUnit_task(void);
    void vLogicUnit_task(LogicUnit* pLogicUnit_task);
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
    void vRecoverSenContext(void);
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
    Thread* pvThread_DONT_DEREF(void);
    void vThread(Thread* pThread);
    void vTracePoint(Stz* aMark);
    void vTraceStep(Stz* aStepNm);
    void vScheduleBide(void);
    void vScheduleDelete(void);
    void vScheduleSleep(void);
    void vScheduleYield(void);
    void vScheduleWait(void);
    eTR  eTranstionRequest(void);
    void vTranstionRequestClear(void);
    void vWakeupClear(void);
    u8   uWakeupInUsec(void);
    bool bWakeupNow(void);
    Sleep& oWakeupSleep(void);
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
    Footer* aFooter(void);
  };

  Task* _pTask(void* aAddr);
  Task* _pTask(i8 iAddr);
}

bool bTaskCompareWakeupTime(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
