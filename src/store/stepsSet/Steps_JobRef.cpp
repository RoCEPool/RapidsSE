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
  
  
}

/*----------------------------------------------------------------------------*/
/* JobRef[].findOriginator -- find job from originator job set                    */
/*----------------------------------------------------------------------------*/
void Step::vJobFindOriginator(void)
{
  ENTRY();
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
  
  Job* pJobFound = AM::pJob_FindOriginating(iJobId);
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
  
  i8 iJobId = m_pDataStack->iPopAsInt(__func__);
    
  Job* pJobFound = AM::pJob_FindParticipating(iJobId);
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
  
  m_pDataStack->vPushInt(pJob->pTask() ? true : false);
  
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
  
  if (! pJob() || ! pJob()->pJobLogicUnit()) {
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
/* JobRef[].returnReference -- return the job reference                                */
/*----------------------------------------------------------------------------*/
void Step::vJobRefReturn(void)
{
  ENTRY();
  
  if (m_p_TaskLogicUnit->m_xpJobRef.bHasReference())
    m_p_TaskLogicUnit->m_xpJobRef.vReturnRef();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
