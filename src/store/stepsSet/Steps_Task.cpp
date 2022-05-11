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
  vAddStep("task.isWaiting?"_Stz,      &Step::vTaskIsWaiting);

  
  // # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

  vAddStep("[task]"_Stz,                &Step::vTaskGet);
  vAddStep("[task].acksRemaining!"_Stz, &Step::vTaskAcksStore);
  vAddStep("[task].breakPoint"_Stz,     &Step::vTaskBreakPoint);
  vAddStep("[task].debugMark@"_Stz,     &Step::vTaskDebugMarkFetch);
  vAddStep("[task].debugMark!"_Stz,     &Step::vTaskDebugMarkStore);
  vAddStep("[task].dropLock"_Stz,       &Step::vTaskDropLock);
  vAddStep("[task].dump"_Stz,           &Step::vTaskDump);
  vAddStep("[task].exception"_Stz,      &Step::vTaskException);
  vAddStep("[task].grabLock"_Stz,       &Step::vTaskGrabLock);
  vAddStep("[task].hibernate"_Stz,      &Step::vTaskHibernate);
  vAddStep("[task].islocked?"_Stz,      &Step::vTaskIsLocked);
  vAddStep("[task].name@"_Stz,          &Step::vTaskNameFetch);
  vAddStep("[task].operandYield@"_Stz,  &Step::vTaskOperandYieldFetch);
  vAddStep("[task].operandYield!"_Stz,  &Step::vTaskOperandYieldStore);
  vAddStep("[task].preHibernate"_Stz,   &Step::vTaskPreHibernate);
  vAddStep("[task].releaseLockSets"_Stz,&Step::vTaskReleaseLockSets);
  vAddStep("[task].retire"_Stz,         &Step::vTaskRetire);
  vAddStep("[task].seeFrameStack"_Stz,  &Step::vTaskSeeFrameStack);
  vAddStep("[task].seeLockSets"_Stz,    &Step::vTaskSeeLockSets);
  vAddStep("[task].seeLogicUnit"_Stz,   &Step::vTaskSeeLogicUnit);
  vAddStep("[task].sleepMsec"_Stz,      &Step::vTaskSleepMsec);
  vAddStep("[task].sleepSsec"_Stz,      &Step::vTaskSleepSsec);
  vAddStep("[task].sleepUsec"_Stz,      &Step::vTaskSleepUsec);
  vAddStep("[task].trace@"_Stz,         &Step::vTaskTraceFetch);
  vAddStep("[task].trace!"_Stz,         &Step::vTaskTraceStore);
  vAddStep("[task].thread@"_Stz,        &Step::vTaskThreadFetch);
  vAddStep("[task].transition!"_Stz,    &Step::vTaskTransitionStore);
  vAddStep("[task].wait"_Stz,           &Step::vTaskWait);
  vAddStep("[task].yield"_Stz,          &Step::vTaskYield);
  vAddStep("[task].yield_opLoop"_Stz,   &Step::vTaskYield_opLoop);
}

/*----------------------------------------------------------------------------*/
/* task.isWaiting? -- is given task waiting?                                  */
/*----------------------------------------------------------------------------*/
void Step::vTaskIsWaiting(void)
{
  ENTRY();

  Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  
  // should never s=check for DEDICATED tasks
  RASSERT(pThatTask->eEntryPoint() != EP_DEDICATED);
  
  m_pDataStack->vPushInt(pThatTask->eState() == TS_WAITING ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].name@ -- fetch name of given task                                   */
/*----------------------------------------------------------------------------*/
void Step::vTaskNameFetch(void)
{
  ENTRY();
    
  s256 sTaskNm((Stz*)szTaskNm());
  
  m_pDataStack->vPushStr(sTaskNm);
  
  EXIT();
}



// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
// # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


/*----------------------------------------------------------------------------*/
/* [task].acksRemaining! -- store value into given tasks m_oAcksRmg_task      */
/*----------------------------------------------------------------------------*/
void Step::vTaskAcksStore(void)
{
  ENTRY();
    
  i8 nAcksRequired = m_pDataStack->iPopAsInt(__func__);
  
  ASSERT(0 == m_oAcksRmg.iPrintOnlyValue());

  RASSERT(bIsLocked());
  
  vAcksRmg(nAcksRequired);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].breakPoint -- break point so we can examine task & thread state     */
/*----------------------------------------------------------------------------*/
void Step::vTaskBreakPoint(void)
{
  ENTRY();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].debugMark@ -- debug mark fetch                                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskDebugMarkFetch(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_iDebugMark);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].debugMark! -- debug mark store                                      */
/*----------------------------------------------------------------------------*/
void Step::vTaskDebugMarkStore(void)
{
  ENTRY();
    
  m_iDebugMark = m_pDataStack->iPopInt(__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].dropLock -- drop this task's lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vTaskDropLock(void)
{
  ENTRY();
    
  vDropTaskLock((Stz*)__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].dump -- dump of task on data stack                                  */
/*----------------------------------------------------------------------------*/
void Step::vTaskDump(void)
{
  ENTRY();
  
  vDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].get -- get task                                                     */
/*----------------------------------------------------------------------------*/
void Step::vTaskGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].exception -- get exception of this task as a string                 */
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
/* [task].grabLock -- grab this task's lock                                   */
/*----------------------------------------------------------------------------*/
void Step::vTaskGrabLock(void)
{
  ENTRY();
    
  vGrabTaskLock((Stz*)__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].hibernate -- this task to hibernate for next use                    */
/*----------------------------------------------------------------------------*/
void Step::vTaskHibernate(void)
{
  ENTRY();
  
  vScheduleHibernate();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* task[].isLocked? -- is task locked?                                        */
/*----------------------------------------------------------------------------*/
void Step::vTaskIsLocked(void)
{
  ENTRY();

  bool bLocked = m_okLock.bIsLocked();
  
  m_pDataStack->vPushInt(bLocked);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].operandYield@ -- fetch operand yield of this task                   */
/*----------------------------------------------------------------------------*/
void Step::vTaskOperandYieldFetch(void)
{
  ENTRY();
    
  m_pDataStack->vPushInt(m_nOperandYield);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].operandYield! -- store operand yield of this task                   */
/*----------------------------------------------------------------------------*/
void Step::vTaskOperandYieldStore(void)
{
  ENTRY();
    
  m_nOperandYield = m_pDataStack->iPopInt(__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].preHibernate -- get task ready to hibernate                         */
/*----------------------------------------------------------------------------*/
void Step::vTaskPreHibernate(void)
{
  ENTRY();
    
//  m_pAuxStack->vReset();
//  m_pDataStack->vReset();
//  m_pReturnStack->vReset();
  
  while (m_pLocalsFrame) {
    
    LocalChain* pLocalsFrame = m_pLocalsFrame;
    m_pLocalsFrame = m_pLocalsFrame->pNext();
    
    delete pLocalsFrame;
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].releaseLockSets -- release all locks in read and write lock sets    */
/*----------------------------------------------------------------------------*/
void Step::vTaskReleaseLockSets(void)
{
  ENTRY();
  
  vReleaseLockSets();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].retire -- this task to retire (exit, reap)                          */
/*----------------------------------------------------------------------------*/
void Step::vTaskRetire(void)
{
  ENTRY();
  
  vScheduleRetire();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].seeFrameStack -- see event handler frames (aka return stack)        */
/*----------------------------------------------------------------------------*/
void Step::vTaskSeeFrameStack(void)
{
  ENTRY();
  
  vSeeFrameStack();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].seeLockSets -- see read and write lock sets                         */
/*----------------------------------------------------------------------------*/
void Step::vTaskSeeLockSets(void)
{
  ENTRY();
  
  vSeeLockSets();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].seeLogicUnit -- see logic Unit registers                            */
/*----------------------------------------------------------------------------*/
void Step::vTaskSeeLogicUnit(void)
{
  ENTRY();

  vSeeLogicUnitRegisters();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].sleepMsec -- this task sleeps for given milliseconds                */
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
/* [task].sleepSsec -- this task sleeps for given seconds                     */
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
/* [task].sleepUsec -- this task sleeps for given microseconds                */
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
/* [task].thread@ -- push this tasks thread                                   */
/*----------------------------------------------------------------------------*/
void Step::vTaskThreadFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(m_pThread);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].trace@ -- fetch trace mask                                          */
/*----------------------------------------------------------------------------*/
void Step::vTaskTraceFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_iTraceMask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].trace! -- store trace mask                                          */
/*----------------------------------------------------------------------------*/
void Step::vTaskTraceStore(void)
{
  ENTRY();
  
  m_iTraceMask = m_pDataStack->iPopInt(__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].transition! -- store transition into this task                      */
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
/* [task].wait -- this task waits for signal                                  */
/*----------------------------------------------------------------------------*/
void Step::vTaskWait(void)
{
  ENTRY();
  
  vScheduleWait();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].yield  -- this task yields                                          */
/*----------------------------------------------------------------------------*/
void Step::vTaskYield(void)
{
  ENTRY();
    
  vGrabTaskLock((Stz*)__func__);
  vScheduleYield();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [task].yield  -- this task yields                                          */
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
