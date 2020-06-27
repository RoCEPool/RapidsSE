/*============================================================================*/
/* Steps_JobGeneric.cpp - generic job step group                              */
/*============================================================================*/

#include "Steps.h"

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
  pJobBegun->m_iFocalId = pDistJob->m_iFocalId;
  pJobBegun->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobBegun->vNormalSides(false);
  pJobBegun->vJobKind(JK_GENERIC);

  pJobBegun->vJobNm("_begun"_Stz);
  pJobBegun->pJobTags()->vInsert(C_TAG_JOBNM,"_begun"_Stz);
  pJobBegun->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobBegun->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobBegun->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->m_iThisNode);
  pJobBegun->pJobTags()->vInsert(C_TAG_SEEDRUN,"_begun_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobBegun->pJobTags()->vInsert(C_TAG_FLOW,1);

  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobBegun);
  
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
  pJobCancel->m_iFocalId = pDistJob->m_iFocalId;
  pJobCancel->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobCancel->vJobKind(JK_GENERIC);

  pJobCancel->vJobNm("_cancel"_Stz);
  pJobCancel->pJobTags()->vInsert(C_TAG_JOBNM,"_cancel"_Stz);
  pJobCancel->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobCancel->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobCancel->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->m_iThisNode);
  pJobCancel->pJobTags()->vInsert(C_TAG_SEEDRUN,"_cancel_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobCancel->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobCancel);
  
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
  pJobCancelled->m_iFocalId = pDistJob->m_iFocalId;
  pJobCancelled->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobCancelled->vNormalSides(false);
  pJobCancelled->vJobKind(JK_GENERIC);
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobCancelled);
  
  pJobCancelled->vJobNm("_cancelled"_Stz);
  pJobCancelled->pJobTags()->vInsert(C_TAG_JOBNM,"_cancelled"_Stz);
  pJobCancelled->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobCancelled->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobCancelled->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->m_iThisNode);
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
  pJobEnd->m_iFocalId = pDistJob->m_iFocalId;
  pJobEnd->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobEnd->vJobKind(JK_GENERIC);
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobEnd);
  
  pJobEnd->vJobNm("_end"_Stz);
  pJobEnd->pJobTags()->vInsert(C_TAG_JOBNM,"_end"_Stz);
  pJobEnd->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobEnd->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobEnd->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->m_iThisNode);
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
  pJobEnded->m_iFocalId = pDistJob->m_iFocalId;
  pJobEnded->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobEnded->vNormalSides(false);
  pJobEnded->vJobKind(JK_GENERIC);
  
  pJobEnded->vJobNm("_ended"_Stz);
  pJobEnded->pJobTags()->vInsert(C_TAG_JOBNM,"_ended"_Stz);
  pJobEnded->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobEnded->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobEnded->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->m_iThisNode);
  pJobEnded->pJobTags()->vInsert(C_TAG_SEEDRUN,"_ended_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobEnded->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobEnded);
  
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
  pJobExecute->m_iFocalId = pDistJob->m_iFocalId;
  pJobExecute->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobExecute->vJobKind(JK_GENERIC);
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobExecute);
  
  pJobExecute->vJobNm("_execute"_Stz);
  pJobExecute->pJobTags()->vInsert(C_TAG_JOBNM,"_execute"_Stz);
  pJobExecute->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobExecute->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobExecute->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->m_iThisNode);
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
  pJobExecuted->m_iFocalId = pDistJob->m_iFocalId;
  pJobExecuted->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobExecuted->vNormalSides(false);
  pJobExecuted->vJobKind(JK_GENERIC);
  
  pJobExecuted->vJobNm("_executed"_Stz);
  pJobExecuted->pJobTags()->vInsert(C_TAG_JOBNM,"_executed"_Stz);
  pJobExecuted->pJobTags()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobExecuted->pJobTags()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobExecuted->pJobTags()->vInsert(C_TAG_ORIGIN,AM::pGrid()->m_iThisNode);
  pJobExecuted->pJobTags()->vInsert(C_TAG_SEEDRUN,"_executed_run"_Stz);

  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobExecuted);
  
  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobExecuted->pJobTags()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobExecuted);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
