/*============================================================================*/
/* Steps_ListedJob.cpp -- list job reference                                  */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ListedJob(void)
{
  vAddStep("ListedJob[]"_Stz,                  &Step::vListedJobPointer);

  vAddStep("ListedJob[].assertNoRef"_Stz,      &Step::vListedJobAssertNoRef);
  vAddStep("ListedJob[].cycleTaskLock"_Stz,    &Step::vListedJobCycleTaskLock);
  vAddStep("ListedJob[].findOriginator?"_Stz,  &Step::vListedJobFindOriginator);
  vAddStep("ListedJob[].findParticipant?"_Stz, &Step::vListedJobFindParticipant);
  vAddStep("ListedJob[].hasTask"_Stz,          &Step::vListedJobHasTask);
  vAddStep("ListedJob[].references"_Stz,       &Step::vListedJobReferences);
  vAddStep("ListedJob[].returnReference"_Stz,  &Step::vListedJobReturn);
  vAddStep("ListedJob[].scheduleNextPhase"_Stz,&Step::vListedJobScheduleNextPhase);
  vAddStep("ListedJob[].taskAck"_Stz,          &Step::vListedJobTaskAck);
  vAddStep("ListedJob[].task@"_Stz,            &Step::vListedJobTaskFetch);
  vAddStep("ListedJob[].taskState@"_Stz,       &Step::vListedJobTaskStateFetch);
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].assertNoRef -- check there is no job reference                 */
/*----------------------------------------------------------------------------*/
void Step::vListedJobAssertNoRef(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpListedJob.pReferencedObject())
    THROW("ListedJob[].assertNoRef : register still has reference to a job"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].cycleTaskLock -- grab and drop lock of given task                         */
/*----------------------------------------------------------------------------*/
void Step::vListedJobCycleTaskLock(void)
{
  ENTRY();
  
  //Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  Job* pJob = m_pLogicUnit->m_xpListedJob.pReferencedObject();

  Task* pThatTask = pJob->pTask();

  // cycle the task lock to ensure task has fully entered next state
  pThatTask->vCycleTaskLock((Stz*)__func__);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].findOriginator? -- find job from originator job set            */
/*----------------------------------------------------------------------------*/
void Step::vListedJobFindOriginator(void)
{
  ENTRY();

  JobId tJobId;
  tJobId = m_pDataStack->iPopAsInt(__func__);

  Job* pJobFound = AM::pJob_FindOriginating(tJobId);
  RASSERT(pJobFound);


  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pLogicUnit->m_xpListedJob);


  m_pDataStack->vPushPtr(pJobFound);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].findParticipant? -- find job from participant job set          */
/*----------------------------------------------------------------------------*/
void Step::vListedJobFindParticipant(void)
{
  ENTRY();

  JobId tJobId;
  tJobId = m_pDataStack->iPopAsInt(__func__);

  Job* pJobFound = AM::pJob_FindParticipating(tJobId);
  RASSERT(pJobFound);


  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pLogicUnit->m_xpListedJob);


  m_pDataStack->vPushPtr(pJobFound);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].hasTask -- does the jobRef have a task ?                       */
/*----------------------------------------------------------------------------*/
void Step::vListedJobHasTask(void)
{
  ENTRY();
  
  Job* pJob = m_pLogicUnit->m_xpListedJob.pReferencedObject();
  
  m_pDataStack->vPushInt(pJob->pTask() ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[] -- return pointer to the referenced job                        */
/*----------------------------------------------------------------------------*/
void Step::vListedJobPointer(void)
{
  ENTRY();
  
  Job* pJob = m_pLogicUnit->m_xpListedJob.pReferencedObject();

  m_pDataStack->vPushPtr(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].references -- return the job references                        */
/*----------------------------------------------------------------------------*/
void Step::vListedJobReferences(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpListedJob.nReferences());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].returnReference -- return the job reference                    */
/*----------------------------------------------------------------------------*/
void Step::vListedJobReturn(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpListedJob.bHasReference())
    m_pLogicUnit->m_xpListedJob.vReturnRef();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.scheduleNextPhase -- schedule next phase for the job                   */
/*----------------------------------------------------------------------------*/
void Step::vListedJobScheduleNextPhase(void)
{
  ENTRY();
    
  //Job* pJobParticipant = _pJob(m_pDataStack->pPopPtr(__func__));
  Job* pJob = m_pLogicUnit->m_xpListedJob.pReferencedObject();

  Task* pTaskParticipant = pJob->pTask();
  RASSERT(pTaskParticipant);
  
  AM::vScheduleNextPhase(pJob,this);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].taskAck -- acknowledge waiting task of this tasks completion   */
/*----------------------------------------------------------------------------*/
void Step::vListedJobTaskAck(void)
{
  ENTRY();
  
  Job* pJob = m_pLogicUnit->m_xpListedJob.pReferencedObject();

  Task* pThatTask = pJob->pTask();
  
  i8 nAcksWereRemaining;
  bool bWasQueued = Scheduler::bTaskCross_AckWaiting(pThatTask,this,
                                                     nAcksWereRemaining);
  UNUSED(bWasQueued);
  
#ifdef WITH_STATISTICS_COUNTERS
  if (bWasQueued)
    gpAttachMap->m_oStatistics.Task.m_nAcks++;
#endif
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].task@ -- get the task of the referenced job                    */
/*----------------------------------------------------------------------------*/
void Step::vListedJobTaskFetch(void)
{
  ENTRY();
  
  Job* pJob = m_pLogicUnit->m_xpListedJob.pReferencedObject();

  m_pDataStack->vPushPtr(pJob->pTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].taskState@ -- fetch state of task                              */
/*----------------------------------------------------------------------------*/
void Step::vListedJobTaskStateFetch(void)
{
  ENTRY();
  
  //Task* pThatTask = _pTask(m_pDataStack->pPopPtr(__func__));
  Job* pJob = m_pLogicUnit->m_xpListedJob.pReferencedObject();

  Task* pThatTask = pJob->pTask();

  eTS eState = pThatTask->m_eState;
  
  m_pDataStack->vPushInt((i8)eState);
  
  EXIT();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
