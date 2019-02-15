/*============================================================================*/
/* Steps_JobGeneric.cpp - generic job step group                              */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobGeneric(void)
{
  vAddStep("jobBegun.error@"_Stz,     &Step::vJob__BegunErrorFetch);
  vAddStep("jobBegun.error!"_Stz,     &Step::vJob__BegunErrorStore);
  
  vAddStep("jobCancelled.error@"_Stz, &Step::vJob__CancelledErrorFetch);
  vAddStep("jobCancelled.error!"_Stz, &Step::vJob__CancelledErrorStore);

  vAddStep("jobExecuted.error@"_Stz,    &Step::vJob__ExecutedErrorFetch);
  vAddStep("jobExecuted.error!"_Stz,    &Step::vJob__ExecutedErrorStore);
  vAddStep("jobExecuted.response@"_Stz, &Step::vJob__ExecutedResponseFetch);
  vAddStep("jobExecuted.response!"_Stz, &Step::vJob__ExecutedResponseStore);
  vAddStep("jobExecuted.rowsInvalid@"_Stz,  &Step::vJob__ExecutedRowsInvalidFetch);
  vAddStep("jobExecuted.rowsInvalid!"_Stz,  &Step::vJob__ExecutedRowsInvalidStore);
  vAddStep("jobExecuted.rowsProcessed@"_Stz,&Step::vJob__ExecutedRowsProcessedFetch);
  vAddStep("jobExecuted.rowsProcessed!"_Stz,&Step::vJob__ExecutedRowsProcessedStore);
  vAddStep("jobExecuted.rowsSkipped@"_Stz,  &Step::vJob__ExecutedRowsSkippedFetch);
  vAddStep("jobExecuted.rowsSkipped!"_Stz,  &Step::vJob__ExecutedRowsSkippedStore);
  
  vAddStep("job__Begun::new"_Stz,        &Step::vJob__BegunNew);
  vAddStep("job__Cancel::new"_Stz,       &Step::vJob__CancelNew);
  vAddStep("job__Cancelled::new"_Stz,    &Step::vJob__CancelledNew);
  vAddStep("job__End::new"_Stz,          &Step::vJob__EndNew);
  vAddStep("job__Ended::new"_Stz,        &Step::vJob__EndedNew);
  vAddStep("job__Execute::new"_Stz,      &Step::vJob__ExecuteNew);
  vAddStep("job__Executed::new"_Stz,     &Step::vJob__ExecutedNew);
}

/*----------------------------------------------------------------------------*/
/* jobBegun.error@ -- fetch job error of participant from job__begun          */
/*----------------------------------------------------------------------------*/
void Step::vJob__BegunErrorFetch(MP_Task mpTask)
{
  ENTRY("vJob__BegunErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobBegun.error@"_Stz);
  
  Job__Begun* pJob__Begun = _pJob__Begun(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob__Begun->m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobBegun.error! -- store job error of participant in job__begun            */
/*----------------------------------------------------------------------------*/
void Step::vJob__BegunErrorStore(MP_Task mpTask)
{
  ENTRY("vJob__BegunErrorStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobBegun.error!"_Stz);
  Job__Begun* pJob__Begun = _pJob__Begun(pJobPtr);
  
  i8 iErrorCode;
  if ( ! mpTask->m_pDataStack->bPop(iErrorCode) )
    THROW_INVALID_DSTACK("jobBegun.error!"_Stz);
  
  pJob__Begun->m_eErrorCode = (eEC)iErrorCode;
 
  ActionSequence::vAdd(pJob__Begun,AN_ERROR_CODE,iErrorCode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobCancelled.error@ -- fetch job error of participant from job__cancelled  */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelledErrorFetch(MP_Task mpTask)
{
  ENTRY("vJob__CancelledErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobCancelled.error@"_Stz);
  
  Job__Cancelled* pJob__Cancelled = _pJob__Cancelled(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob__Cancelled->m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobCancelled.error! -- store job error of participant in job__cancelled    */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelledErrorStore(MP_Task mpTask)
{
  ENTRY("vJob__CancelledErrorStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobCancelled.error!"_Stz);
  Job__Cancelled* pJob__Cancelled = _pJob__Cancelled(pJobPtr);
  
  i8 iErrorCode;
  if ( ! mpTask->m_pDataStack->bPop(iErrorCode) )
    THROW_INVALID_DSTACK("jobCancelled.error!"_Stz);
  
  pJob__Cancelled->m_eErrorCode = (eEC)iErrorCode;
  
  ActionSequence::vAdd(pJob__Cancelled,AN_ERROR_CODE,iErrorCode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.error@ -- fetch job error of participant from job__executed    */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedErrorFetch(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.error@"_Stz);
  
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob__Executed->m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.error! -- store job error of participant in job__executed      */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedErrorStore(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedErrorStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.error!"_Stz);
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  i8 iErrorCode;
  if ( ! mpTask->m_pDataStack->bPop(iErrorCode) )
    THROW_INVALID_DSTACK("jobExecuted.error!"_Stz);
  
  pJob__Executed->m_eErrorCode = (eEC)iErrorCode;
  
  ActionSequence::vAdd(pJob__Executed,AN_ERROR_CODE,iErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.response@ -- fetch participant response from job__executed     */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedResponseFetch(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedResponseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.response@"_Stz);
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  mpTask->m_pDataStack->vPushS256(pJob__Executed->m_sResponse);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.response! -- store participant response in job__executed       */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedResponseStore(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedResponseStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.response!"_Stz);
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  s256 sResponse;
  if ( ! mpTask->m_pDataStack->bPop(sResponse) )
    THROW_INVALID_DSTACK("jobExecuted.response!"_Stz);
  
  pJob__Executed->m_sResponse = sResponse;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.rowsInvalid@ -- fetch rowsInvalid in job__executed             */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedRowsInvalidFetch(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedRowsInvalidFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.rowsProcessed@"_Stz);
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob__Executed->m_iRowsInvalid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.rowsInvalid! -- store rowsInvalid in job__executed             */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedRowsInvalidStore(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedRowsInvalidStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.rowsInvalid!"_Stz);
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  i8 iRowsInvalid;
  if ( ! mpTask->m_pDataStack->bPop(iRowsInvalid) )
    THROW_INVALID_DSTACK("jobExecuted.rowsInvalid!"_Stz);
  
  pJob__Executed->m_iRowsInvalid = iRowsInvalid;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.rowsProcessed@ -- store rowsProcessed in job__executed         */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedRowsProcessedFetch(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedRowsProcessedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.rowsProcessed@"_Stz);
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob__Executed->m_iRowsProcessed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.rowsProcessed! -- store rowsProcessed in job__executed         */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedRowsProcessedStore(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedRowsProcessedStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.rowsProcessed!"_Stz);
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  i8 iRowsProcessed;
  if ( ! mpTask->m_pDataStack->bPop(iRowsProcessed) )
    THROW_INVALID_DSTACK("jobExecuted.rowsProcessed!"_Stz);
  
  pJob__Executed->m_iRowsProcessed = iRowsProcessed;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.rowsSkipped@ -- fetch rowsSkipped in job__executed             */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedRowsSkippedFetch(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedRowsSkippedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.rowsSkipped@"_Stz);
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob__Executed->m_iRowsSkipped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.rowsSkipped! -- store rowsSkipped in job__executed             */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedRowsSkippedStore(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedRowsSkippedStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.rowsSkipped!"_Stz);
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  i8 iRowsSkipped;
  if ( ! mpTask->m_pDataStack->bPop(iRowsSkipped) )
    THROW_INVALID_DSTACK("jobExecuted.rowsSkipped!"_Stz);
  
  pJob__Executed->m_iRowsSkipped = iRowsSkipped;
  
  EXIT();
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* job_begun::new -- new a job__begun for this job                            */
/*----------------------------------------------------------------------------*/
void Step::vJob__BegunNew(MP_Task mpTask)
{
  ENTRY("vJob__BegunNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pDistJobPtr) )
    THROW_INVALID_DSTACK("vJob__Begun::new"_Stz);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobBegun = new Job__Begun(pDistJob);
  pJobBegun->m_iJobId = pDistJob->m_iJobId;
  pJobBegun->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Begun = JobClass::pFind(CN_JOB__BEGUN);
  pJobBegun->m_pJobClass = pJobClass__Begun;
  
  ActionSequence::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobBegun);
  
  mpTask->m_pDataStack->vPushPointer(pJobBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__cancel::new -- new a job__cancel for this job                         */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelNew(MP_Task mpTask)
{
  ENTRY("vJob__CancelNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pDistJobPtr) )
    THROW_INVALID_DSTACK("job__cancel::new"_Stz);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobCancel = new Job__Cancel(pDistJob);
  pJobCancel->m_iJobId = pDistJob->m_iJobId;
  pJobCancel->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Cancel = JobClass::pFind(CN_JOB__CANCEL);
  pJobCancel->m_pJobClass = pJobClass__Cancel;
  
  ActionSequence::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobCancel);
  
  mpTask->m_pDataStack->vPushPointer(pJobCancel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__cancelled::new -- new a job__cancelled for this job                   */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelledNew(MP_Task mpTask)
{
  ENTRY("vJob__CancelledNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pDistJobPtr) )
    THROW_INVALID_DSTACK("job__cancelled::new"_Stz);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobCancelled = new Job__Cancelled(pDistJob);
  pJobCancelled->m_iJobId = pDistJob->m_iJobId;
  pJobCancelled->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Cancelled = JobClass::pFind(CN_JOB__CANCELLED);
  pJobCancelled->m_pJobClass = pJobClass__Cancelled;
  
  ActionSequence::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobCancelled);
  
  mpTask->m_pDataStack->vPushPointer(pJobCancelled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__end::new - new a job__end for this job                                */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndNew(MP_Task mpTask)
{
  ENTRY("vJob__EndNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pDistJobPtr) )
    THROW_INVALID_DSTACK("job__end::new"_Stz);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobEnd = new Job__End(pDistJob);
  pJobEnd->m_iJobId = pDistJob->m_iJobId;
  pJobEnd->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__End = JobClass::pFind(CN_JOB__END);
  pJobEnd->m_pJobClass = pJobClass__End;
  
  ActionSequence::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobEnd);
  
  mpTask->m_pDataStack->vPushPointer(pJobEnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__ended::new -- new a job__ended for this job                           */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndedNew(MP_Task mpTask)
{
  ENTRY("vJob__EndedNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pDistJobPtr) )
    THROW_INVALID_DSTACK("job__ended::new"_Stz);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobEnded = new Job__Ended(pDistJob);
  pJobEnded->m_iJobId = pDistJob->m_iJobId;
  pJobEnded->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Ended = JobClass::pFind(CN_JOB__ENDED);
  pJobEnded->m_pJobClass = pJobClass__Ended;
  
  ActionSequence::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobEnded);
  
  mpTask->m_pDataStack->vPushPointer(pJobEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__execute::new -- new a job__execute for this job                       */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecuteNew(MP_Task mpTask)
{
  ENTRY("vJob__ExecuteNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pDistJobPtr) )
    THROW_INVALID_DSTACK("job__execute::new"_Stz);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobExecute = new Job__Execute(pDistJob);
  pJobExecute->m_iJobId = pDistJob->m_iJobId;
  pJobExecute->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Execute = JobClass::pFind(CN_JOB__EXECUTE);
  pJobExecute->m_pJobClass = pJobClass__Execute;
  
  ActionSequence::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobExecute);
  
  mpTask->m_pDataStack->vPushPointer(pJobExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job__executed::new -- new a job__executed for this job                     */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedNew(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedNew"_Stz);
  
  Object* pDistJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pDistJobPtr) )
    THROW_INVALID_DSTACK("job__executed::new"_Stz);
  Job* pDistJob = _pJob(pDistJobPtr);

  Job* pJobExecuted = new Job__Executed(pDistJob);
  pJobExecuted->m_iJobId = pDistJob->m_iJobId;
  pJobExecuted->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Executed = JobClass::pFind(CN_JOB__EXECUTED);
  pJobExecuted->m_pJobClass = pJobClass__Executed;
  
  ActionSequence::vAdd(mpTask->pJob(),AN_NEW_JOB,pJobExecuted);
  
  mpTask->m_pDataStack->vPushPointer(pJobExecuted);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
