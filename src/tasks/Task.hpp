/*============================================================================*/
/* Task.hpp - task                                                            */
/*============================================================================*/
#pragma once

#include "eEP.h"
#include "eQK.h"
#include "eTR.h"
#include "eTS.h"

namespace Threads
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

  private:
    Lock         m_okLock;       // lock for this task

    SMA<Job>     m_pJob;         // the job for this task
    SMA<Thread>  m_pThread;      // current thread

    eTS          m_eState;       // state (running, sleeping, ...)
    eTR          m_eTransition;  // schedulre transition request
    eQK          m_eQueue;       // which queue the task is currently in
    eEP          m_eEntryPoint;  // task entry point
    
    Decider      m_oAcksRmg;     // number of acks still waiting for
    Sleep        m_oWakeup;      // wakeup time from sleeping

    SMA<Forth>   m_pForth;       // forth execution context
    SMA<LogicUnit>  m_pLogicUnit;      // set of registers for the task steps

    i8           m_iTraceMask;   // tracing mask
    eCN          m_eWhatJob;     // what job this task is/was for (debugging)
  
    i8           m_nOperandYield;  //
  
    Profile*     m_pProfile;     // runtime profile

    // always last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Task(Job* pJob);
    ~Task(void) noexcept(false);
    
    static Task* pThisTLSTask(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vMonitorProfile_stats(s48K& sPortrait);
  
    i8   iAcksOriginal(void);
    i8   iAcksRmg_assert_only_(void);
    void vAcksRmg(i8 nAcksRmg);
    bool bAcksRmgDec(void);
    void vAdjustFIP(i8 nBytes);
    Object* pArgObject(void);
    void vAssertStructure(void);
    void vBeginOp(void);
    void vCaught(Exception xException);
    bool bCorrectLayout(void);
    Compiler* pCompiler(void);
    void vContentDump(void);
    static void vDebug(void);
    void vDropTaskLock(void);
    void vDump(void);
    void vEntryPoint(eEP eEntryPoint);
    eEP  eEntryPoint(void);
    void vFocalOutline(s4K& sOutline);
    Forth* pForth(void);
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
    Profile* pProfile(void);
    void vProfile(Profile* pProfile);
    eQK  eQueue(void);
    void vQueue(eQK eQueue);
    LogicUnit* pLogicUnit(void);
    void vRenderPortrait(s256& sTaskPortrait);
    void vReseed(void);
    bool bSeeded(void);
    void vSeedTask(void);
    Node* pSendNode(void);
    void vSendNode(Node* pSendNode);
    void vSleepUsec(i8 iMicroSeconds);
    void vValSprint(s48K& sFormat, s48K& sString);
    void vStackContents(Stack* pStack, s48K& sStackContents);
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
    void vUnlinkJob(void);
    void vWakeupClear(void);
    u8   uWakeupInUsec(void);
    bool bWakeupNow(void);
    Sleep& oWakeupSleep(void);
    u8   uWakeupTime(void);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    void vAddMunged(void);
    void vAddMunged(Forth* pForth);
    void vAddAxioms(void);
    Word* pCompileAxiomaticColon(s48K& sDefinition);
    void  vCompileMungedColon(Stz* szDefinition);
    void  vCompileMungedColon(i8 i_Def);

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
