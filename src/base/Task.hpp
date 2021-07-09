/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once

#include "eEP.h"
#include "eQK.h"
#include "eTR.h"
#include "eTS.h"

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
  
    ObjId<CN_TASK> m_xTaskId;    // tasks object id

    SMA<Thread>  m_pThread;      // current thread executing this task
    SMA<Job>     m_pJob;         // job this task is executing
  
    bool         m_bIsStart;     // is this the start task?
    bool         m_bIsImmediate; // is this an immediate task?

    eTS          m_eState;       // state (running, sleeping, ...)
    eTR          m_eTransition;  // schedulre transition request
    eQK          m_eQueue;       // which queue the task is currently in
    eEP          m_eEntryPoint;  // task entry point
    
    Decider      m_oAcksRmg;     // number of acks still waiting for
    Sleep        m_oWakeup;      // wakeup time from sleeping

    bool         m_bLogMemory;   // log memory usage?
    bool         m_bLogResource; // log resources usage?

    i8           m_iTraceMask;   // tracing mask
  
    i8           m_nOperandYield;  //
  
    Profile*     m_pProfile;     // runtime profile
  
#ifdef WITH_LOCK_SETS
    LockSet      m_oReadLocks;   // read locks currently held by this task/op
    LockSet      m_oWriteLocks;  // write locks currently held by this task/op
#endif
  
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


    // always last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Task(Job* pJob);
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
    bool bAcksRmgDec(void);
    void vAdjustFIP(i8 nBytes);
    Object* pArgObject(void);
    void vCheckBreakPt(void);
    bool bCorrectLayout(void);
    SenCompiler* pSenCompiler(void);
    void vContentDump(void);
    void vDeleteInput(void);
    static void vDebug(void);
    DicStack* pDicStack(void);
    void vDropAllLocks(void);
    void vDropTaskLock(void);
    void vDump(void);
    void vEntryPoint(eEP eEntryPoint);
    eEP  eEntryPoint(void);
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
    bool bLogMemory(void);
    void vLogMemory(bool bLogMemory);
    void vMutexLock(void);
    void vMutexSignal(void);
    void vMutexUnlock(void);
    void vMutexWait(void);
    void vNewInput(void);
    Profile* pProfile(void);
    void vProfile(Profile* pProfile);
    eQK  eQueue(void);
    void vQueue(eQK eQueue);
    LockSet* pReadLocks(void);
    void vResetSenContext(void);
    void vReseed(void);
    bool bLogResource(void);
    void vLogResource(bool bLogResource);
    DicStack* pSaveDicStack(void);
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
