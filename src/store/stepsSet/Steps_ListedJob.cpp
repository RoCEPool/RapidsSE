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
  vAddStep("ListedJob[]"_Stz,                 &Step::vListedJobPointer);

  vAddStep("ListedJob[].assertNoRef"_Stz,     &Step::vListedJobAssertNoRef);
  vAddStep("ListedJob[].findOriginator"_Stz,  &Step::vListedJobFindOriginator);
  vAddStep("ListedJob[].findParticipant"_Stz, &Step::vListedJobFindParticipant);
  vAddStep("ListedJob[].hasTask"_Stz,         &Step::vListedJobHasTask);
  vAddStep("ListedJob[].references"_Stz,      &Step::vListedJobReferences);
  vAddStep("ListedJob[].returnReference"_Stz, &Step::vListedJobReturn);
  vAddStep("ListedJob[].task@"_Stz,           &Step::vListedJobTaskFetch);
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].assertNoRef -- check there is no job reference                 */
/*----------------------------------------------------------------------------*/
void Step::vListedJobAssertNoRef(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpJobRef.pReferencedObject())
    THROW("ListedJob[].assertNoRef : register still has reference to a job"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].findOriginator -- find job from originator job set             */
/*----------------------------------------------------------------------------*/
void Step::vListedJobFindOriginator(void)
{
  ENTRY();

  JobId tJobId;
  tJobId = m_pDataStack->iPopAsInt(__func__);

  Job* pJobFound = AM::pJob_FindOriginating(tJobId);
  RASSERT(pJobFound);


  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pLogicUnit->m_xpJobRef);


  m_pDataStack->vPushPtr(pJobFound);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].findParticipant -- find job from participant job set           */
/*----------------------------------------------------------------------------*/
void Step::vListedJobFindParticipant(void)
{
  ENTRY();

  JobId tJobId;
  tJobId = m_pDataStack->iPopAsInt(__func__);

  Job* pJobFound = AM::pJob_FindParticipating(tJobId);
  RASSERT(pJobFound);


  //! borrow the job ref [ this needs to be done under lock ]
  pJobFound->vBorrowRefBy(&m_pLogicUnit->m_xpJobRef);


  m_pDataStack->vPushPtr(pJobFound);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].hasTask -- does the jobRef have a task ?                       */
/*----------------------------------------------------------------------------*/
void Step::vListedJobHasTask(void)
{
  ENTRY();
  
  Job* pJob = m_pLogicUnit->m_xpJobRef.pReferencedObject();
  
  m_pDataStack->vPushInt(pJob->pTask() ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[] -- return pointer to the referenced job                        */
/*----------------------------------------------------------------------------*/
void Step::vListedJobPointer(void)
{
  ENTRY();
  
  Job* pJob = m_pLogicUnit->m_xpJobRef.pReferencedObject();

  m_pDataStack->vPushPtr(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].references -- return the job references                        */
/*----------------------------------------------------------------------------*/
void Step::vListedJobReferences(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpJobRef.nReferences());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].returnReference -- return the job reference                    */
/*----------------------------------------------------------------------------*/
void Step::vListedJobReturn(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpJobRef.bHasReference())
    m_pLogicUnit->m_xpJobRef.vReturnRef();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ListedJob[].task@ -- get the task of the referenced job                    */
/*----------------------------------------------------------------------------*/
void Step::vListedJobTaskFetch(void)
{
  ENTRY();
  
  Job* pJob = m_pLogicUnit->m_xpJobRef.pReferencedObject();

  m_pDataStack->vPushPtr(pJob->pTask());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
