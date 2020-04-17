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

  JobClass* aJobClass_Universal = JobClass::pFind(CN_JOB_UNIVERSAL);

  Job* pJobBegun = new Job_Universal(aJobClass_Universal,&pDistJob->m_xReqTuple); // new Job__Begun(pDistJob);
  pJobBegun->m_iFocalId = pDistJob->m_iFocalId;
  pJobBegun->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobBegun->vNormalSides(false);
  pJobBegun->vJobKind(JK_GENERIC);

  //JobClass* pJobClass__Begun = JobClass::pFind(CN_JOB__BEGUN);
  //pJobBegun->m_aJobClass = pJobClass__Begun;

  pJobBegun->vJobClass(aJobClass_Universal,(char*)__func__);

  pJobBegun->vJobNm("_begun"_Stz);
  pJobBegun->pEnameSet()->vInsert(C_TAG_JOBCLASS,CN_JOB_UNIVERSAL);
  pJobBegun->pEnameSet()->vInsert(C_TAG_JOBNM,"_begun"_Stz);
  pJobBegun->pEnameSet()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobBegun->pEnameSet()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobBegun->pEnameSet()->vInsert(C_TAG_NODENO,AM::pGrid()->m_iThisNode);
  pJobBegun->pEnameSet()->vInsert(C_TAG_SEEDRUN,"_begun_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobBegun->pEnameSet()->vInsert(C_TAG_FLOW,1);

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

  JobClass* pJobClass_Universal = JobClass::pFind(CN_JOB_UNIVERSAL);
  
  Job* pJobCancel = new Job_Universal(pJobClass_Universal,&pDistJob->m_xReqTuple); // new Job__Cancel(pDistJob);
  pJobCancel->m_iFocalId = pDistJob->m_iFocalId;
  pJobCancel->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobCancel->vJobKind(JK_GENERIC);

  JobClass* aJobClass__Cancel = pJobClass_Universal;
  pJobCancel->vJobClass(aJobClass__Cancel,(char*)__func__);

  pJobCancel->vJobNm("_cancel"_Stz);
  pJobCancel->pEnameSet()->vInsert(C_TAG_JOBCLASS,CN_JOB_UNIVERSAL);
  pJobCancel->pEnameSet()->vInsert(C_TAG_JOBNM,"_cancel"_Stz);
  pJobCancel->pEnameSet()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobCancel->pEnameSet()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobCancel->pEnameSet()->vInsert(C_TAG_NODENO,AM::pGrid()->m_iThisNode);
  pJobCancel->pEnameSet()->vInsert(C_TAG_SEEDRUN,"_cancel_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobCancel->pEnameSet()->vInsert(C_TAG_FLOW,1);
  
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

  JobClass* pJobClass_Universal = JobClass::pFind(CN_JOB_UNIVERSAL);

  Job* pJobCancelled = new Job_Universal(pJobClass_Universal,&pDistJob->m_xReqTuple); // new Job__Cancelled(pDistJob);
  pJobCancelled->m_iFocalId = pDistJob->m_iFocalId;
  pJobCancelled->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobCancelled->vNormalSides(false);
  pJobCancelled->vJobKind(JK_GENERIC);

  JobClass* aJobClass__Cancelled = pJobClass_Universal;
  pJobCancelled->vJobClass(aJobClass__Cancelled,(char*)__func__);
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobCancelled);
  
  pJobCancelled->vJobNm("_cancelled"_Stz);
  pJobCancelled->pEnameSet()->vInsert(C_TAG_JOBCLASS,CN_JOB_UNIVERSAL);
  pJobCancelled->pEnameSet()->vInsert(C_TAG_JOBNM,"_cancelled"_Stz);
  pJobCancelled->pEnameSet()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobCancelled->pEnameSet()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobCancelled->pEnameSet()->vInsert(C_TAG_NODENO,AM::pGrid()->m_iThisNode);
  pJobCancelled->pEnameSet()->vInsert(C_TAG_SEEDRUN,"_cancelled_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobCancelled->pEnameSet()->vInsert(C_TAG_FLOW,1);
  
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

  JobClass* pJobClass_Universal = JobClass::pFind(CN_JOB_UNIVERSAL);
  
  Job* pJobEnd = new Job_Universal(pJobClass_Universal,&pDistJob->m_xReqTuple); // new Job__End(pDistJob);
  pJobEnd->m_iFocalId = pDistJob->m_iFocalId;
  pJobEnd->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobEnd->vJobKind(JK_GENERIC);

  JobClass* aJobClass__End = pJobClass_Universal;
  pJobEnd->vJobClass(aJobClass__End,(char*)__func__);
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobEnd);
  
  pJobEnd->vJobNm("_end"_Stz);
  pJobEnd->pEnameSet()->vInsert(C_TAG_JOBCLASS,CN_JOB_UNIVERSAL);
  pJobEnd->pEnameSet()->vInsert(C_TAG_JOBNM,"_end"_Stz);
  pJobEnd->pEnameSet()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobEnd->pEnameSet()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobEnd->pEnameSet()->vInsert(C_TAG_NODENO,AM::pGrid()->m_iThisNode);
  pJobEnd->pEnameSet()->vInsert(C_TAG_SEEDRUN,"_end_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobEnd->pEnameSet()->vInsert(C_TAG_FLOW,1);
  
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

  JobClass* pJobClass_Universal = JobClass::pFind(CN_JOB_UNIVERSAL);
  
  Job* pJobEnded = new Job_Universal(pJobClass_Universal,&pDistJob->m_xReqTuple); // new Job__Ended(pDistJob);
  pJobEnded->m_iFocalId = pDistJob->m_iFocalId;
  pJobEnded->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobEnded->vNormalSides(false);
  pJobEnded->vJobKind(JK_GENERIC);

  JobClass* aJobClass__Ended = pJobClass_Universal;
  pJobEnded->vJobClass(aJobClass__Ended,(char*)__func__);
  
  pJobEnded->vJobNm("_ended"_Stz);
  pJobEnded->pEnameSet()->vInsert(C_TAG_JOBCLASS,CN_JOB_UNIVERSAL);
  pJobEnded->pEnameSet()->vInsert(C_TAG_JOBNM,"_ended"_Stz);
  pJobEnded->pEnameSet()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobEnded->pEnameSet()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobEnded->pEnameSet()->vInsert(C_TAG_NODENO,AM::pGrid()->m_iThisNode);
  pJobEnded->pEnameSet()->vInsert(C_TAG_SEEDRUN,"_ended_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobEnded->pEnameSet()->vInsert(C_TAG_FLOW,1);
  
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

  JobClass* pJobClass_Universal = JobClass::pFind(CN_JOB_UNIVERSAL);
  
  Job* pJobExecute = new Job_Universal(pJobClass_Universal,&pDistJob->m_xReqTuple); // new Job__Execute(pDistJob);
  pJobExecute->m_iFocalId = pDistJob->m_iFocalId;
  pJobExecute->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobExecute->vJobKind(JK_GENERIC);

  JobClass* aJobClass__Execute = pJobClass_Universal;
  pJobExecute->vJobClass(aJobClass__Execute,(char*)__func__);
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobExecute);
  
  pJobExecute->vJobNm("_execute"_Stz);
  pJobExecute->pEnameSet()->vInsert(C_TAG_JOBCLASS,CN_JOB_UNIVERSAL);
  pJobExecute->pEnameSet()->vInsert(C_TAG_JOBNM,"_execute"_Stz);
  pJobExecute->pEnameSet()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobExecute->pEnameSet()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobExecute->pEnameSet()->vInsert(C_TAG_NODENO,AM::pGrid()->m_iThisNode);
  pJobExecute->pEnameSet()->vInsert(C_TAG_SEEDRUN,"_execute_run"_Stz);

  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobExecute->pEnameSet()->vInsert(C_TAG_FLOW,1);
  
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

  JobClass* pJobClass_Universal = JobClass::pFind(CN_JOB_UNIVERSAL);
  
  Job* pJobExecuted = new Job_Universal(pJobClass_Universal,&pDistJob->m_xReqTuple); // new Job__Executed(pDistJob);
  pJobExecuted->m_iFocalId = pDistJob->m_iFocalId;
  pJobExecuted->vParticipantNode(AM::pGrid()->m_iThisNode);
  pJobExecuted->vNormalSides(false);
  pJobExecuted->vJobKind(JK_GENERIC);

  JobClass* aJobClass__Executed = pJobClass_Universal;
  pJobExecuted->vJobClass(aJobClass__Executed,(char*)__func__);
  
  pJobExecuted->vJobNm("_executed"_Stz);
  pJobExecuted->pEnameSet()->vInsert(C_TAG_JOBCLASS,CN_JOB_UNIVERSAL);
  pJobExecuted->pEnameSet()->vInsert(C_TAG_JOBNM,"_executed"_Stz);
  pJobExecuted->pEnameSet()->vInsert(C_TAG_PATH,"/"_Stz);
  pJobExecuted->pEnameSet()->vInsert(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobExecuted->pEnameSet()->vInsert(C_TAG_NODENO,AM::pGrid()->m_iThisNode);
  pJobExecuted->pEnameSet()->vInsert(C_TAG_SEEDRUN,"_executed_run"_Stz);

  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobExecuted);
  
  if (pDistJob->bHasTag(C_TAG_FLOW))
    pJobExecuted->pEnameSet()->vInsert(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPtr(pJobExecuted);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
