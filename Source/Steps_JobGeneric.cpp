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
  
  vAddStep("job_Begun::new"_Stz,        &Step::vJob__BegunNew);
  vAddStep("job_Cancel::new"_Stz,       &Step::vJob__CancelNew);
  vAddStep("job_Cancelled::new"_Stz,    &Step::vJob__CancelledNew);
  vAddStep("job_End::new"_Stz,          &Step::vJob__EndNew);
  vAddStep("job_Ended::new"_Stz,        &Step::vJob__EndedNew);
  vAddStep("job_Execute::new"_Stz,      &Step::vJob__ExecuteNew);
  vAddStep("job_Executed::new"_Stz,     &Step::vJob__ExecutedNew);
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
/* job.newBegun -- new a job__begun for this job                              */
/*----------------------------------------------------------------------------*/
void Step::vJob__BegunNew(MP_Task mpTask)
{
  ENTRY("vJob__BegunNew"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
    THROW_INVALID_DSTACK("vJob__BegunNew"_Stz);
  
  Job* pJobBegun = new Job__Begun;
  pJobBegun->m_iJobId = iJobId;
  pJobBegun->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Begun = JobClass::pFind(CN_JOB__BEGUN);
  pJobBegun->m_pJobClass = pJobClass__Begun;
  
  mpTask->m_pDataStack->vPushPointer(pJobBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newCancel -- new a job__cancel for this job                            */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelNew(MP_Task mpTask)
{
  ENTRY("vJob__CancelNew"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
    THROW_INVALID_DSTACK("vJob__CancelNew"_Stz);
  
  Job* pJobCancel = new Job__Cancel;
  pJobCancel->m_iJobId = iJobId;
  pJobCancel->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Cancel = JobClass::pFind(CN_JOB__CANCEL);
  pJobCancel->m_pJobClass = pJobClass__Cancel;
  
  mpTask->m_pDataStack->vPushPointer(pJobCancel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newCancelled -- new a job__cancelled for this job                      */
/*----------------------------------------------------------------------------*/
void Step::vJob__CancelledNew(MP_Task mpTask)
{
  ENTRY("vJob__CancelledNew"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
    THROW_INVALID_DSTACK("vJob__CancelledNew"_Stz);
  
  Job* pJobCancelled = new Job__Cancelled;
  pJobCancelled->m_iJobId = iJobId;
  pJobCancelled->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Cancelled = JobClass::pFind(CN_JOB__CANCELLED);
  pJobCancelled->m_pJobClass = pJobClass__Cancelled;
  
  mpTask->m_pDataStack->vPushPointer(pJobCancelled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newEnd - new a job__end for this job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndNew(MP_Task mpTask)
{
  ENTRY("vJob__EndNew"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
    THROW_INVALID_DSTACK("vJob__EndNew"_Stz);
  
  Job* pJobEnd = new Job__End;
  pJobEnd->m_iJobId = iJobId;
  pJobEnd->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__End = JobClass::pFind(CN_JOB__END);
  pJobEnd->m_pJobClass = pJobClass__End;
  
  mpTask->m_pDataStack->vPushPointer(pJobEnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newEnded -- new a job__ended for this job                              */
/*----------------------------------------------------------------------------*/
void Step::vJob__EndedNew(MP_Task mpTask)
{
  ENTRY("vJob__EndedNew"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
    THROW_INVALID_DSTACK("vJob__EndedNew"_Stz);
  
  Job* pJobEnded = new Job__Ended;
  pJobEnded->m_iJobId = iJobId;
  pJobEnded->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Ended = JobClass::pFind(CN_JOB__ENDED);
  pJobEnded->m_pJobClass = pJobClass__Ended;
  
  mpTask->m_pDataStack->vPushPointer(pJobEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newExecute -- new a job__execute for this job                          */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecuteNew(MP_Task mpTask)
{
  ENTRY("vJob__ExecuteNew"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
    THROW_INVALID_DSTACK("vJob__ExecuteNew"_Stz);
  
  Job* pJobExecute = new Job__Execute;
  pJobExecute->m_iJobId = iJobId;
  pJobExecute->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Execute = JobClass::pFind(CN_JOB__EXECUTE);
  pJobExecute->m_pJobClass = pJobClass__Execute;
  
  mpTask->m_pDataStack->vPushPointer(pJobExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newExecuted -- new a job__executed for this job                        */
/*----------------------------------------------------------------------------*/
void Step::vJob__ExecutedNew(MP_Task mpTask)
{
  ENTRY("vJob__ExecutedNew"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
    THROW_INVALID_DSTACK("vJob__ExecutedNew"_Stz);
  
  Job* pJobExecuted = new Job__Executed;
  pJobExecuted->m_iJobId = iJobId;
  
  pJobExecuted->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  JobClass* pJobClass__Executed = JobClass::pFind(CN_JOB__EXECUTED);
  pJobExecuted->m_pJobClass = pJobClass__Executed;
  
  mpTask->m_pDataStack->vPushPointer(pJobExecuted);
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
