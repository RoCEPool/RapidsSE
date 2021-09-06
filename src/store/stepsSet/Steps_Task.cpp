/*============================================================================*/
/* Steps_Task.cpp - task step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Node.hpp"
#include "Node_I.hpp"

#include "Job.hpp"
#include "JobList.hpp"
#include "LogicUnit.hpp"

#include "MY.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Task(void)
{
  vAddStep("task::acks!"_Stz,          &Step::vTaskAcksStore);
  vAddStep("task::breakPoint"_Stz,     &Step::vTaskBreakPoint);
  vAddStep("task::scheduledelete"_Stz, &Step::vTaskDelete);
  vAddStep("task::dump"_Stz,           &Step::vTaskDump);
  vAddStep("task::exception"_Stz,      &Step::vTaskException);
  vAddStep("task::operandYield@"_Stz,  &Step::vTaskOperandYieldFetch);
  vAddStep("task::operandYield!"_Stz,  &Step::vTaskOperandYieldStore);
  vAddStep("task::releaseLockSets"_Stz,&Step::vTaskReleaseLockSets);
  vAddStep("task::seeFrameStack"_Stz,  &Step::vTaskSeeFrameStack);
  vAddStep("task::seeLockSets"_Stz,    &Step::vTaskSeeLockSets);
  vAddStep("task::seeLogicUnitRegisters"_Stz,  &Step::vTaskSeeLogicUnitRegisters);
  vAddStep("task::sleepMsec"_Stz,      &Step::vTaskSleepMsec);
  vAddStep("task::sleepSsec"_Stz,      &Step::vTaskSleepSsec);
  vAddStep("task::sleepUsec"_Stz,      &Step::vTaskSleepUsec);
  vAddStep("task::transition!"_Stz,    &Step::vTaskTransitionStore);
  vAddStep("task::wait"_Stz,           &Step::vTaskWait);
  vAddStep("task::yield"_Stz,          &Step::vTaskYield);
  vAddStep("task::yield_opLoop"_Stz,   &Step::vTaskYield_opLoop);

  vAddStep("task.ack"_Stz,             &Step::vTaskAck);
  vAddStep("task.dropLock"_Stz,        &Step::vTaskDropLock);
  vAddStep("task.grabLock"_Stz,        &Step::vTaskGrabLock);
  vAddStep("task.isAckSet"_Stz,        &Step::vTaskIsAckSet);
  vAddStep("task.isWaiting"_Stz,       &Step::vTaskIsWaiting);
  vAddStep("task.job@"_Stz,            &Step::vTaskJobFetch);
  vAddStep("task.job!"_Stz,            &Step::vTaskJobStore);
  vAddStep("task.name@"_Stz,           &Step::vTaskNameFetch);
  vAddStep("task.profile@"_Stz,        &Step::vTaskProfileFetch);
  vAddStep("task.profile!"_Stz,        &Step::vTaskProfileStore);
  vAddStep("task.state@"_Stz,          &Step::vTaskStateFetch);
  vAddStep("task.thread"_Stz,          &Step::vTaskThread);
}

/*----------------------------------------------------------------------------*/
/* task.ack -- acknowledge that task of this tasks completion                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskAck(void)
{
  ENTRY();
  
  Task* pTaskWaiting = _pTask(m_pDataStack->pPopPtr(__func__));
  
  i8 nAcksWereRemaining;
  bool bWasQueued = Scheduler::bTaskCross_AckWaiting(pTaskWaiting,this,
                                                     nAcksWereRemaining);
  UNUSED(bWasQueued);
  
#ifdef WITH_STATISTICS_COUNTERS
  if (bWasQueued)
    gpAttachMap->m_oStatistics.Task.m_nAcks++;
#endif
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::acks! -- store value into given tasks m_oAcksRmg_task                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskAcksStore(void)
{
  ENTRY();
    
  i8 nAcksRequired = m_pDataStack->iPopAsInt(__func__);
  
  ASSERT(0 == m_oAcksRmg_task.iPrintOnlyValue());

  RASSERT(bIsLocked());
  
  vAcksRmg(nAcksRequired);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.breakPoint -- break point so we can examine task & thread state       */
/*----------------------------------------------------------------------------*/
void Step::vTaskBreakPoint(void)
{
  ENTRY();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dropLock -- drop lock of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Step::vTaskDropLock(void)
{
  ENTRY();
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  pThatTask->vDropTaskLock((Stz*)__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.dump -- dump of task on data stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskDump(void)
{
  ENTRY();
  
  vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::scheduledelete -- remove task from scheduler and delete              */
/*----------------------------------------------------------------------------*/
void Step::vTaskDelete(void)
{
  ENTRY();
    
  // unlink task and job
  if (m_pJob) {
    m_pJob->vTask(nullptr);
    m_pJob = nullptr;
  }
  
  RASSERT(! m_pJob);
  
//  Stz* szThreadNm = MY::szThreadNm();

  //vGrabTaskLock((Stz*)__func__);

  //vScheduleDelete();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::exception -- get current exception as a string                       */
/*----------------------------------------------------------------------------*/
void Step::vTaskException(void)
{
  ENTRY();
    
  s4K sException;
  
  sException.vSprint("Exception %lld %s; "
                     "thrown in %s (%s:%lld) "
                     ""_Stz
                     
                     ,m_xException.eFailure()
                     ,m_xException.azText()

                     ,m_xException.azFuncNm()
                     ,m_xException.azFileNm()
                     ,m_xException.iLineNo()

                     );
  
  m_pDataStack->vPushStr(sException);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.grabLock -- grab lock of task on data stack                           */
/*----------------------------------------------------------------------------*/
void Step::vTaskGrabLock(void)
{
  ENTRY();
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  pThatTask->vGrabTaskLock((Stz*)__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.isAckSet -- is task ack set?                                          */
/*----------------------------------------------------------------------------*/
void Step::vTaskIsAckSet(void)
{
  ENTRY();
  
  Task* pTaskBeingAcked = _pTask(m_pDataStack->pPopPtr(__func__));
  
  i8 iIsAckSet = pTaskBeingAcked->m_oAcksRmg_task.bIsSet();
    
  m_pDataStack->vPushInt(iIsAckSet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.isWaiting -- is task waiting?                                         */
/*----------------------------------------------------------------------------*/
void Step::vTaskIsWaiting(void)
{
  ENTRY();

  Task* pTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  // should never s=check for DEDICATED tasks
  RASSERT(pTask->eEntryPoint() != EP_DEDICATED);
  
  m_pDataStack->vPushInt(pTask->eState() == TS_WAITING ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.job@ -- push job of given task                                        */
/*----------------------------------------------------------------------------*/
void Step::vTaskJobFetch(void)
{
  ENTRY();
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  Job* pJob = pThatTask->m_pJob;
  
  m_pDataStack->vPushPtr(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.job! -- store job into given task                                     */
/*----------------------------------------------------------------------------*/
void Step::vTaskJobStore(void)
{
  ENTRY();
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  pThatTask->m_pJob = pJob;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.name@ -- fetch task name                                              */
/*----------------------------------------------------------------------------*/
void Step::vTaskNameFetch(void)
{
  ENTRY();
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  s256 sName((Stz*)pThatTask->szTaskNm());
  
  m_pDataStack->vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::operandYield@ -- operand yield fetch                                  */
/*----------------------------------------------------------------------------*/
void Step::vTaskOperandYieldFetch(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_nOperandYield);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::operandYield! -- operand yield store                                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskOperandYieldStore(void)
{
  ENTRY();
    
  m_nOperandYield = m_pDataStack->iPopInt(__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.profile@ -- fetch task profile                                        */
/*----------------------------------------------------------------------------*/
void Step::vTaskProfileFetch(void)
{
  ENTRY();
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pThatTask->pProfile());
  pThatTask->vProfile(nullptr);
   
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.profile! -- store task profile                                        */
/*----------------------------------------------------------------------------*/
void Step::vTaskProfileStore(void)
{
  ENTRY();
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  Profile* pProfile = _pProfile(m_pDataStack->pPopPtr(__func__));
  
  pThatTask->vProfile(pProfile);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::releaseLockSets -- release all locks in read and write lock sets     */
/*----------------------------------------------------------------------------*/
void Step::vTaskReleaseLockSets(void)
{
  ENTRY();
  
  vReleaseLockSets();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::seeFrameStack -- see event handler frames (aka return stack)         */
/*----------------------------------------------------------------------------*/
void Step::vTaskSeeFrameStack(void)
{
  ENTRY();
  
  vSeeFrameStack();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::seeLockSets -- see read and write lock sets                          */
/*----------------------------------------------------------------------------*/
void Step::vTaskSeeLockSets(void)
{
  ENTRY();
  
  vSeeLockSets();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::seeLogicUnitRegisters -- see logic Unit registers                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskSeeLogicUnitRegisters(void)
{
  ENTRY();

  vSeeLogicUnitRegisters();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::sleepMsec -- this task sleeps for given milliseconds                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskSleepMsec(void)
{
  ENTRY();
  
  i8 nMilliSeconds = m_pDataStack->iPopInt(__func__);
  
  i8 nMicroSeconds = nMilliSeconds * C_1K;
  
  vSleepUsec(nMicroSeconds);
  vGrabTaskLock((Stz*)__func__);
  vScheduleSleep();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::sleepSsec -- this task sleeps for given seconds                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskSleepSsec(void)
{
  ENTRY();
  
  i8 nSeconds = m_pDataStack->iPopInt(__func__);
  i8 nMicroSeconds = nSeconds * C_1M;
  
  vSleepUsec(nMicroSeconds);
  vGrabTaskLock((Stz*)__func__);
  vScheduleSleep();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::sleepUsec -- this task sleeps for given microseconds                 */
/*----------------------------------------------------------------------------*/
void Step::vTaskSleepUsec(void)
{
  ENTRY();
  
  i8 nMicroSeconds = m_pDataStack->iPopInt(__func__);
  
  vSleepUsec(nMicroSeconds);
  vGrabTaskLock((Stz*)__func__);
  vScheduleSleep();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.state@ -- fetch state of task                                         */
/*----------------------------------------------------------------------------*/
void Step::vTaskStateFetch(void)
{
  ENTRY();
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  eTS eState = pThatTask->m_eState;
  
  m_pDataStack->vPushInt((i8)eState);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task.thread -- push given tasks thread                                     */
/*----------------------------------------------------------------------------*/
void Step::vTaskThread(void)
{
  ENTRY();
  
  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));

  Thread* pThread = pThatTask->m_pThread;
  m_pDataStack->vPushPtr(pThread);
  ASSERT(pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::transition! -- transition store                                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskTransitionStore(void)
{
  ENTRY();
  
  eTR eTransition = (eTR)m_pDataStack->iPopInt(__func__);

  RASSERT(bIsLocked());
  m_eTransition = eTransition;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::wait -- this task waits for signal                                   */
/*----------------------------------------------------------------------------*/
void Step::vTaskWait(void)
{
  ENTRY();
  
  vScheduleWait();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::yield -- this task yields                                            */
/*----------------------------------------------------------------------------*/
void Step::vTaskYield(void)
{
  ENTRY();
    
  vGrabTaskLock((Stz*)__func__);
  vScheduleYield();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task::yield -- this task yields                                            */
/*----------------------------------------------------------------------------*/
void Step::vTaskYield_opLoop(void)
{
  ENTRY();
    
  vGrabTaskLock((Stz*)__func__);
  RASSERT(bIsLocked());
  vScheduleYield();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
