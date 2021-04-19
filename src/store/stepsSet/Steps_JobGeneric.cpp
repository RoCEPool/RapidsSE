/*============================================================================*/
/* Steps_JobGeneric.cpp - generic job step group                              */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Grid.hpp"
#include "Job.hpp"
#include "TagConstants.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobGeneric(void)
{
  vAddStep("job__Begun::new"_Stz,        &Step::vJob__BegunNew);
  vAddStep("job__Cancel::new"_Stz,       &Step::vJob__CancelNew);
  vAddStep("job__Cancelled::new"_Stz,    &Step::vJob__CancelledNew);
  vAddStep("job__End::new"_Stz,          &Step::vJob__EndNew);
  vAddStep("job__Ended::new"_Stz,        &Step::vJob__EndedNew);
  vAddStep("job__Execute::new"_Stz,      &Step::vJob__ExecuteNew);
  vAddStep("job__Executed::new"_Stz,     &Step::vJob__ExecutedNew);
}

/*----------------------------------------------------------------------------*/
/* job_begun::new -- new a job__begun for this distributed 2pc job            */
/*----------------------------------------------------------------------------*/
void Step::vJob__BegunNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));

  Job* pJobBegun = new Job(&pDistJob->m_xReqTuple);
  pJobBegun->m_xJobId = pDistJob->iJobId();
  pJobBegun->vParticipantNode(AM::pGrid()->iThisNode());
  pJobBegun->vNormalSides(false);
  pJobBegun->vJobKind(JK_GENERIC);

  pJobBegun->vJobNm("_begun"_Stz);
  pJobBegun->pJobTags()->vInsert(C_TAG_JOBNM,"_begun"_Stz);
  pJobBegun->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobBegun->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobBegun->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNode());
  pJobBegun->pJobTags()->vInsert(C_TAG_SEEDRUN,"_begun_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobBegun->pJobTags()->vInsert(C_TAG_FLOW,1);

  m_pDataStack->vPushPtr(pJobBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__cancel::new -- new a job__cancel for this distributed 2pc job         */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobCancel = new Job(&pDistJob->m_xReqTuple);
  pJobCancel->m_xJobId = pDistJob->iJobId();
  pJobCancel->vParticipantNode(AM::pGrid()->iThisNode());
  pJobCancel->vJobKind(JK_GENERIC);

  pJobCancel->vJobNm("_cancel"_Stz);
  pJobCancel->pJobTags()->vInsert(C_TAG_JOBNM,"_cancel"_Stz);
  pJobCancel->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobCancel->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobCancel->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNode());
  pJobCancel->pJobTags()->vInsert(C_TAG_SEEDRUN,"_cancel_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobCancel->pJobTags()->vInsert(C_TAG_FLOW,1);
    
  m_pDataStack->vPushPtr(pJobCancel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__cancelled::new -- new a job__cancelled for this distributed 2pc job   */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelledNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));

  Job* pJobCancelled = new Job(&pDistJob->m_xReqTuple);
  pJobCancelled->m_xJobId = pDistJob->iJobId();
  pJobCancelled->vParticipantNode(AM::pGrid()->iThisNode());
  pJobCancelled->vNormalSides(false);
  pJobCancelled->vJobKind(JK_GENERIC);
  
  pJobCancelled->vJobNm("_cancelled"_Stz);
  pJobCancelled->pJobTags()->vInsert(C_TAG_JOBNM,"_cancelled"_Stz);
  pJobCancelled->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobCancelled->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobCancelled->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNode());
  pJobCancelled->pJobTags()->vInsert(C_TAG_SEEDRUN,"_cancelled_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobCancelled->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobCancelled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__end::new - new a job__end for this distributed 2pc job                */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobEnd = new Job(&pDistJob->m_xReqTuple); // new Job__End(pDistJob);
  pJobEnd->m_xJobId = pDistJob->iJobId();
  pJobEnd->vParticipantNode(AM::pGrid()->iThisNode());
  pJobEnd->vJobKind(JK_GENERIC);
    
  pJobEnd->vJobNm("_end"_Stz);
  pJobEnd->pJobTags()->vInsert(C_TAG_JOBNM,"_end"_Stz);
  pJobEnd->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobEnd->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobEnd->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNode());
  pJobEnd->pJobTags()->vInsert(C_TAG_SEEDRUN,"_end_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobEnd->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobEnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__ended::new -- new a job__ended for this distributed 2pc job           */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndedNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobEnded = new Job(&pDistJob->m_xReqTuple);
  pJobEnded->m_xJobId = pDistJob->iJobId();
  pJobEnded->vParticipantNode(AM::pGrid()->iThisNode());
  pJobEnded->vNormalSides(false);
  pJobEnded->vJobKind(JK_GENERIC);
  
  pJobEnded->vJobNm("_ended"_Stz);
  pJobEnded->pJobTags()->vInsert(C_TAG_JOBNM,"_ended"_Stz);
  pJobEnded->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobEnded->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobEnded->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNode());
  pJobEnded->pJobTags()->vInsert(C_TAG_SEEDRUN,"_ended_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobEnded->pJobTags()->vInsert(C_TAG_FLOW,1);
    
  m_pDataStack->vPushPtr(pJobEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__execute::new -- new a job__execute for this distributed 2pc job       */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecuteNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobExecute = new Job(&pDistJob->m_xReqTuple);
  pJobExecute->m_xJobId = pDistJob->iJobId();
  pJobExecute->vParticipantNode(AM::pGrid()->iThisNode());
  pJobExecute->vJobKind(JK_GENERIC);
    
  pJobExecute->vJobNm("_execute"_Stz);
  pJobExecute->pJobTags()->vInsert(C_TAG_JOBNM,"_execute"_Stz);
  pJobExecute->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobExecute->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobExecute->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNode());
  pJobExecute->pJobTags()->vInsert(C_TAG_SEEDRUN,"_execute_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobExecute->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__executed::new -- new a job__executed for this distributed 2pc job     */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedNew(void)
{
  ENTRY();
  
  Job* pDistJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  Job* pJobExecuted = new Job(&pDistJob->m_xReqTuple);
  pJobExecuted->m_xJobId = pDistJob->iJobId();
  pJobExecuted->vParticipantNode(AM::pGrid()->iThisNode());
  pJobExecuted->vNormalSides(false);
  pJobExecuted->vJobKind(JK_GENERIC);
  
  pJobExecuted->vJobNm("_executed"_Stz);
  pJobExecuted->pJobTags()->vInsert(C_TAG_JOBNM,"_executed"_Stz);
  pJobExecuted->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobExecuted->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->iJobId());
  pJobExecuted->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->iThisNode());
  pJobExecuted->pJobTags()->vInsert(C_TAG_SEEDRUN,"_executed_run"_Stz);
  
  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobExecuted->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobExecuted);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
