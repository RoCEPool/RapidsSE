/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once

#include "eEP.h"
#include "eQK.h"
#include "eTR.h"
#include "eTS.h"

namespace STORE
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
  friend class STORE::Step;

  private:
    Lock         m_okLock;       // lock for this task

    SMA<Job>     m_pJob;         // the job for this task
    SMA<Thread>  m_pThread;      // current thread
    SMA<LogicUnit> m_pLogicUnit;      // set of registers for the task steps

    eTS          m_eState;       // state (running, sleeping, ...)
    eTR          m_eTransition;  // schedulre transition request
    eQK          m_eQueue;       // which queue the task is currently in
    eEP          m_eEntryPoint;  // task entry point
    
    Decider      m_oAcksRmg;     // number of acks still waiting for
    Sleep        m_oWakeup;      // wakeup time from sleeping

  
    i8           m_iTraceMask;   // tracing mask
  
    i8           m_nOperandYield;  //
  
    Profile*     m_pProfile;     // runtime profile
  
  
  
    // -- from Forth --
    //LogicUnit*   m_pLogicUnit;      // associated register set

  public:
    Word*     m_pSeedWord;      // seed
    Word*     m_pExceptionWord; // seed

    bool      m_bSeedInitially; // seed_initially sentinel has been executed
    bool      m_bDoneFinally;   // done_finally sentinel has been executed

    DicStack  m_oDicStack;      // dictionary stack
    DicStack  m_oSaveDicStack;  // save dictionary stack

    ForthCell*     m_aInxPtr;      // instruction/cell pointer

    TypedStack*    m_pAuxStack;    // auxiliary stack
    TypedStack*    m_pDataStack;   // data stack
    TypedStack*    m_pReturnStack; // return stack

    LocalChain* m_pLocalsFrame; // locals| | register frame chain

    Compiler*   m_pCompiler;    // word compiler (Job boot, create table etc)
    Input*      m_pInput;       // input stream


    // always last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Task(Job* pJob, i8 iFocalId);
    ~Task(void) noexcept(false);
    
    static Task* pThisTLSTask(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    i8   iAcksOriginal(void);
    i8   iAcksRmg_assert_only_(void);
    void vAcksRmg(i8 nAcksRmg);
    bool bAcksRmgDec(void);
    void vAdjustFIP(i8 nBytes);
    Object* pArgObject(void);
    void vAssertStructure(void);
    void vBeginOp(void);
    void vCaught(Exception xException);
    void vCheckBreakPt(void);
    bool bCorrectLayout(void);
    Compiler* pCompiler(void);
    void vContentDump(void);
    void vDeleteInput(void);
    static void vDebug(void);
    DicStack* pDicStack(void);
    void vDropTaskLock(void);
    void vDump(void);
    void vEntryPoint(eEP eEntryPoint);
    eEP  eEntryPoint(void);
    void vGrabTaskLock(void);
    void vInitExeContext(void);
    bool bIsLocked(void);
    bool bIsTaskLocked(void);
    Job* pJob(void);
    void vJob(Job* pJob);
    Stz* szJobName(void);
    void vLocalsPop(void);
    void vLocalsPush(void);
    void vMutexLock(void);
    void vMutexSignal(void);
    void vMutexUnlock(void);
    void vMutexWait(void);
    void vNewInput(void);
    i8&  i_NextRnd(void);
    Profile* pProfile(void);
    void vProfile(Profile* pProfile);
    eQK  eQueue(void);
    void vQueue(eQK eQueue);
    LogicUnit* pLogicUnit(void);
    void vReset(void);
    void vReseed(void);
    DicStack* pSaveDicStack(void);
    bool bSeeded(void);
    void vSeedTask(void);
    Node* pSendNode(void);
    void vSendNode(Node* pSendNode);
    void vSleepUsec(i8 iMicroSeconds);
    void vValSprint(s48K& sFormat, s48K& sString);
    void vStackContents(TypedStack* pStack, s48K& sStackContents);
    eTS  eState(void);
    void vState(eTS eState);
    i8   iTaskId(void);
    void vTaskLoop(void);
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

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vAddMunged(void);
    void vAddMunged(Task* pTask);
    void vAddAxioms(void);
    Word* pCompileAxiomaticColon(s48K& sDefinition);
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
