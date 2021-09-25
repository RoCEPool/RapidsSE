/*============================================================================*/
/* Steps_JobRef.cpp -- job reference                                          */
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
void Step::vAddSteps_JobRef(void)
{
  vAddStep("JobRef[]"_Stz,                 &Step::vJobRefPointer);

  vAddStep("JobRef[].findOriginator"_Stz,  &Step::vJobFindOriginator);
  vAddStep("JobRef[].findParticipant"_Stz, &Step::vJobFindParticipant);

  vAddStep("JobRef[].hasTask"_Stz,         &Step::vJobRefHasTask);

  vAddStep("JobRef[].none"_Stz,            &Step::vJobRefNone);

  vAddStep("JobRef[].returnReference"_Stz, &Step::vJobRefReturn);
  vAddStep("JobRef[].references"_Stz,      &Step::vJobRefReferences);
  
  vAddStep("JobRef[].task@"_Stz,           &Step::vJobRefTaskFetch);
}

/*----------------------------------------------------------------------------*/
/* JobRef[].findOriginator -- find job from originator job set                */
/*----------------------------------------------------------------------------*/
void Step::vJobFindOriginator(void)
{
  ENTRY();
  
  JobId tJobId;
  tJobId = m_pDataStack->iPopAsInt(__func__);
  
  Job* pJobFound = AM::pJob__FindOriginating(tJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_p_TaskLogicUnit->m_xpJobRef);
  
  
  m_pDataStack->vPushPtr(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].findParticipant -- find job from participant job set                  */
/*----------------------------------------------------------------------------*/
void Step::vJobFindParticipant(void)
{
  ENTRY();
  
  JobId tJobId;
  tJobId = m_pDataStack->iPopAsInt(__func__);

  Job* pJobFound = AM::pJob__FindParticipating(tJobId);
  RASSERT(pJobFound);
  
  
  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_p_TaskLogicUnit->m_xpJobRef);
  
  
  m_pDataStack->vPushPtr(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].hasTask -- does the jobRef have a task ?                          */
/*----------------------------------------------------------------------------*/
void Step::vJobRefHasTask(void)
{
  ENTRY();
  
  Job* pJob = m_p_TaskLogicUnit->m_xpJobRef.pReferencedObject();
  
  m_pDataStack->vPushInt(pJob->pJobTask() ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[] -- return pointer to the referenced job                           */
/*----------------------------------------------------------------------------*/
void Step::vJobRefPointer(void)
{
  ENTRY();
  
  Job* pJob = m_p_TaskLogicUnit->m_xpJobRef.pReferencedObject();

  m_pDataStack->vPushPtr(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].none -- check there is no job reference                           */
/*----------------------------------------------------------------------------*/
void Step::vJobRefNone(void)
{
  ENTRY();
  
  if (! pJob_task() || ! pJob_task()->pJobLogicUnit()) {
    EXIT();
    return;
  }

  if (m_p_TaskLogicUnit->m_xpJobRef.pReferencedObject())
    THROW("JobRef[].none : register still has reference to a job"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].references -- return the job references                           */
/*----------------------------------------------------------------------------*/
void Step::vJobRefReferences(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpJobRef.nReferences());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].returnReference -- return the job reference                       */
/*----------------------------------------------------------------------------*/
void Step::vJobRefReturn(void)
{
  ENTRY();
  
  if (m_p_TaskLogicUnit->m_xpJobRef.bHasReference())
    m_p_TaskLogicUnit->m_xpJobRef.vReturnRef();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* JobRef[].task@ -- get the task of the referenced job                       */
/*----------------------------------------------------------------------------*/
void Step::vJobRefTaskFetch(void)
{
  ENTRY();
  
  Job* pJob = m_p_TaskLogicUnit->m_xpJobRef.pReferencedObject();

  m_pDataStack->vPushPtr(pJob->pJobTask());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
