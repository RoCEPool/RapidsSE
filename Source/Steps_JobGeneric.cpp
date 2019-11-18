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
  ENTRY("vJob__BegunNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! m_pDataStack->bPopObject(pDistJobPtr) )
    THROW_DSTACK();
  Job* pDistJob = _pJob(pDistJobPtr);

  JobClass* pJobClass_Universal = JobClass::pFind(CN_JOB_UNIVERSAL);

  Job* pJobBegun = new Job_Universal(pJobClass_Universal,&pDistJob->m_xReqTuple); // new Job__Begun(pDistJob);
  pJobBegun->m_iFocalId = pDistJob->m_iFocalId;
  pJobBegun->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  pJobBegun->m_bNormalSides = false;

  //JobClass* pJobClass__Begun = JobClass::pFind(CN_JOB__BEGUN);
  //pJobBegun->m_aJobClass = pJobClass__Begun;

  pJobBegun->m_aJobClass = pJobClass_Universal;

  pJobBegun->m_sJobNm = "_begun"_Stz;
  pJobBegun->m_oEnameSet.vAdd("<jobClass>"_Stz,CN_JOB_UNIVERSAL);
  pJobBegun->m_oEnameSet.vAdd(C_TAG_JOBNM,"_begun"_Stz);
  pJobBegun->m_oEnameSet.vAdd(C_TAG_PATH,"/"_Stz);
  pJobBegun->m_oEnameSet.vAdd(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobBegun->m_oEnameSet.vAdd(C_TAG_NODENO,AM::pGrid()->m_iThisNode);

  if (pDistJob->bHasEnameTag(C_TAG_FLOW))
    pJobBegun->m_oEnameSet.vAdd(C_TAG_FLOW,1);

  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobBegun);
  
  m_pDataStack->vPushPointer(pJobBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__cancel::new -- new a job__cancel for this distributed 2pc job         */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelNew(void)
{
  ENTRY("vJob__CancelNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! m_pDataStack->bPopObject(pDistJobPtr) )
    THROW_DSTACK();
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobCancel = new Job__Cancel(pDistJob);
  pJobCancel->m_iFocalId = pDistJob->m_iFocalId;
  pJobCancel->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  
  JobClass* pJobClass__Cancel = JobClass::pFind(CN_JOB__CANCEL);
  pJobCancel->m_aJobClass = pJobClass__Cancel;

  pJobCancel->m_sJobNm = "_cancel"_Stz;
  pJobCancel->m_oEnameSet.vAdd(C_TAG_JOBNM,"_cancel"_Stz);
  pJobCancel->m_oEnameSet.vAdd(C_TAG_PATH,"/"_Stz);
  pJobCancel->m_oEnameSet.vAdd(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobCancel->m_oEnameSet.vAdd(C_TAG_NODENO,AM::pGrid()->m_iThisNode);

  if (pDistJob->bHasEnameTag(C_TAG_FLOW))
    pJobCancel->m_oEnameSet.vAdd(C_TAG_FLOW,1);
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobCancel);
  
  m_pDataStack->vPushPointer(pJobCancel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__cancelled::new -- new a job__cancelled for this distributed 2pc job   */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelledNew(void)
{
  ENTRY("vJob__CancelledNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! m_pDataStack->bPopObject(pDistJobPtr) )
    THROW_DSTACK();
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobCancelled = new Job__Cancelled(pDistJob);
  pJobCancelled->m_iFocalId = pDistJob->m_iFocalId;
  pJobCancelled->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  pJobCancelled->m_bNormalSides = false;

  JobClass* pJobClass__Cancelled = JobClass::pFind(CN_JOB__CANCELLED);
  pJobCancelled->m_aJobClass = pJobClass__Cancelled;
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobCancelled);
  
  pJobCancelled->m_sJobNm = "_cancelled"_Stz;
  pJobCancelled->m_oEnameSet.vAdd(C_TAG_JOBNM,"_cancelled"_Stz);
  pJobCancelled->m_oEnameSet.vAdd(C_TAG_PATH,"/"_Stz);
  pJobCancelled->m_oEnameSet.vAdd(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobCancelled->m_oEnameSet.vAdd(C_TAG_NODENO,AM::pGrid()->m_iThisNode);

  if (pDistJob->bHasEnameTag(C_TAG_FLOW))
    pJobCancelled->m_oEnameSet.vAdd(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPointer(pJobCancelled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__end::new - new a job__end for this distributed 2pc job                */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndNew(void)
{
  ENTRY("vJob__EndNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! m_pDataStack->bPopObject(pDistJobPtr) )
    THROW_DSTACK();
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobEnd = new Job__End(pDistJob);
  pJobEnd->m_iFocalId = pDistJob->m_iFocalId;
  pJobEnd->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  
  JobClass* pJobClass__End = JobClass::pFind(CN_JOB__END);
  pJobEnd->m_aJobClass = pJobClass__End;
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobEnd);
  
  pJobEnd->m_sJobNm = "_end"_Stz;
  pJobEnd->m_oEnameSet.vAdd(C_TAG_JOBNM,"_end"_Stz);
  pJobEnd->m_oEnameSet.vAdd(C_TAG_PATH,"/"_Stz);
  pJobEnd->m_oEnameSet.vAdd(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobEnd->m_oEnameSet.vAdd(C_TAG_NODENO,AM::pGrid()->m_iThisNode);

  if (pDistJob->bHasEnameTag(C_TAG_FLOW))
    pJobEnd->m_oEnameSet.vAdd(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPointer(pJobEnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__ended::new -- new a job__ended for this distributed 2pc job           */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndedNew(void)
{
  ENTRY("vJob__EndedNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! m_pDataStack->bPopObject(pDistJobPtr) )
    THROW_DSTACK();
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobEnded = new Job__Ended(pDistJob);
  pJobEnded->m_iFocalId = pDistJob->m_iFocalId;
  pJobEnded->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  pJobEnded->m_bNormalSides = false;

  JobClass* pJobClass__Ended = JobClass::pFind(CN_JOB__ENDED);
  pJobEnded->m_aJobClass = pJobClass__Ended;
  
  pJobEnded->m_sJobNm = "_ended"_Stz;
  pJobEnded->m_oEnameSet.vAdd(C_TAG_JOBNM,"_ended"_Stz);
  pJobEnded->m_oEnameSet.vAdd(C_TAG_PATH,"/"_Stz);
  pJobEnded->m_oEnameSet.vAdd(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobEnded->m_oEnameSet.vAdd(C_TAG_NODENO,AM::pGrid()->m_iThisNode);

  if (pDistJob->bHasEnameTag(C_TAG_FLOW))
    pJobEnded->m_oEnameSet.vAdd(C_TAG_FLOW,1);
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobEnded);
  
  m_pDataStack->vPushPointer(pJobEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__execute::new -- new a job__execute for this distributed 2pc job       */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecuteNew(void)
{
  ENTRY("vJob__ExecuteNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! m_pDataStack->bPopObject(pDistJobPtr) )
    THROW_DSTACK();
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobExecute = new Job__Execute(pDistJob);
  pJobExecute->m_iFocalId = pDistJob->m_iFocalId;
  pJobExecute->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  
  JobClass* pJobClass__Execute = JobClass::pFind(CN_JOB__EXECUTE);
  pJobExecute->m_aJobClass = pJobClass__Execute;
  
  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobExecute);
  
  pJobExecute->m_sJobNm = "_execute"_Stz;
  pJobExecute->m_oEnameSet.vAdd(C_TAG_JOBNM,"_execute"_Stz);
  pJobExecute->m_oEnameSet.vAdd(C_TAG_PATH,"/"_Stz);
  pJobExecute->m_oEnameSet.vAdd(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobExecute->m_oEnameSet.vAdd(C_TAG_NODENO,AM::pGrid()->m_iThisNode);

  if (pDistJob->bHasEnameTag(C_TAG_FLOW))
    pJobExecute->m_oEnameSet.vAdd(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPointer(pJobExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__executed::new -- new a job__executed for this distributed 2pc job     */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedNew(void)
{
  ENTRY("vJob__ExecutedNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! m_pDataStack->bPopObject(pDistJobPtr) )
    THROW_DSTACK();
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobExecuted = new Job__Executed(pDistJob);
  pJobExecuted->m_iFocalId = pDistJob->m_iFocalId;
  pJobExecuted->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  pJobExecuted->m_bNormalSides = false;

  JobClass* pJobClass__Executed = JobClass::pFind(CN_JOB__EXECUTED);
  pJobExecuted->m_aJobClass = pJobClass__Executed;
  
  pJobExecuted->m_sJobNm = "_executed"_Stz;
  pJobExecuted->m_oEnameSet.vAdd(C_TAG_JOBNM,"_executed"_Stz);
  pJobExecuted->m_oEnameSet.vAdd(C_TAG_PATH,"/"_Stz);
  pJobExecuted->m_oEnameSet.vAdd(C_TAG_DISTID,pDistJob->m_iFocalId);
  pJobExecuted->m_oEnameSet.vAdd(C_TAG_NODENO,AM::pGrid()->m_iThisNode);

  //ActLog::vAdd(m_pTask->pJob(),AN_NEW_JOB,pJobExecuted);
  
  if (pDistJob->bHasEnameTag(C_TAG_FLOW))
    pJobExecuted->m_oEnameSet.vAdd(C_TAG_FLOW,1);
  
  m_pDataStack->vPushPointer(pJobExecuted);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
