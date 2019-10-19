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
/* job_begun::new -- new a job__begun for this job                            */
/*----------------------------------------------------------------------------*/
void Step::vJob__BegunNew(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob__BegunNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDistJobPtr) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobBegun = new Job__Begun(pDistJob);
  pJobBegun->m_iUniqueId = pDistJob->m_iUniqueId;
  pJobBegun->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  
  JobClass* pJobClass__Begun = JobClass::pFind(CN_JOB__BEGUN);
  pJobBegun->m_aJobClass = pJobClass__Begun;
  
  //ActLog::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobBegun);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pJobBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__cancel::new -- new a job__cancel for this job                         */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelNew(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob__CancelNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDistJobPtr) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobCancel = new Job__Cancel(pDistJob);
  pJobCancel->m_iUniqueId = pDistJob->m_iUniqueId;
  pJobCancel->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  
  JobClass* pJobClass__Cancel = JobClass::pFind(CN_JOB__CANCEL);
  pJobCancel->m_aJobClass = pJobClass__Cancel;
  
  //ActLog::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobCancel);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pJobCancel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__cancelled::new -- new a job__cancelled for this job                   */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelledNew(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob__CancelledNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDistJobPtr) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobCancelled = new Job__Cancelled(pDistJob);
  pJobCancelled->m_iUniqueId = pDistJob->m_iUniqueId;
  pJobCancelled->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  
  JobClass* pJobClass__Cancelled = JobClass::pFind(CN_JOB__CANCELLED);
  pJobCancelled->m_aJobClass = pJobClass__Cancelled;
  
  //ActLog::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobCancelled);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pJobCancelled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__end::new - new a job__end for this job                                */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndNew(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob__EndNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDistJobPtr) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobEnd = new Job__End(pDistJob);
  pJobEnd->m_iUniqueId = pDistJob->m_iUniqueId;
  pJobEnd->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  
  JobClass* pJobClass__End = JobClass::pFind(CN_JOB__END);
  pJobEnd->m_aJobClass = pJobClass__End;
  
  //ActLog::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobEnd);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pJobEnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__ended::new -- new a job__ended for this job                           */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndedNew(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob__EndedNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDistJobPtr) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobEnded = new Job__Ended(pDistJob);
  pJobEnded->m_iUniqueId = pDistJob->m_iUniqueId;
  pJobEnded->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  
  JobClass* pJobClass__Ended = JobClass::pFind(CN_JOB__ENDED);
  pJobEnded->m_aJobClass = pJobClass__Ended;
  
  //ActLog::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobEnded);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pJobEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__execute::new -- new a job__execute for this job                       */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecuteNew(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob__ExecuteNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDistJobPtr) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobExecute = new Job__Execute(pDistJob);
  pJobExecute->m_iUniqueId = pDistJob->m_iUniqueId;
  pJobExecute->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  
  JobClass* pJobClass__Execute = JobClass::pFind(CN_JOB__EXECUTE);
  pJobExecute->m_aJobClass = pJobClass__Execute;
  
  //ActLog::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobExecute);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pJobExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__executed::new -- new a job__executed for this job                     */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedNew(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob__ExecutedNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pDistJobPtr) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobExecuted = new Job__Executed(pDistJob);
  pJobExecuted->m_iUniqueId = pDistJob->m_iUniqueId;
  pJobExecuted->m_iParticipantNode = AM::pGrid()->m_iThisNode;
  
  JobClass* pJobClass__Executed = JobClass::pFind(CN_JOB__EXECUTED);
  pJobExecuted->m_aJobClass = pJobClass__Executed;
  
  //ActLog::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobExecuted);
  
  mpTask->m_pForth->m_pDataStack->vPushPointer(pJobExecuted);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
