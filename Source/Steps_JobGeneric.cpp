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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
