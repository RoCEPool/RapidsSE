/*============================================================================*/
/* Steps_job.cpp - job step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_JobGeneric(void)
{
  vAddPrimitive("jobBegun.error@"_Stz,      &vJobBegunErrorFetch,  false);
  vAddPrimitive("jobBegun.error!"_Stz,      &vJobBegunErrorStore,  false);
  
  vAddPrimitive("jobCancelled.error@"_Stz,  &vJobCancelledErrorFetch, false);
  vAddPrimitive("jobCancelled.error!"_Stz,  &vJobCancelledErrorStore, false);

  vAddPrimitive("jobExecuted.error@"_Stz,   &vJobExecutedErrorFetch,   false);
  vAddPrimitive("jobExecuted.error!"_Stz,   &vJobExecutedErrorStore,   false);
  vAddPrimitive("jobExecuted.response@"_Stz,&vJobExecutedResponseFetch,false);
  vAddPrimitive("jobExecuted.response!"_Stz,&vJobExecutedResponseStore,false);
  vAddPrimitive("jobExecuted.rowsProcessed@"_Stz,&vJobExecutedRowsProcessedFetch,false);
  vAddPrimitive("jobExecuted.rowsProcessed!"_Stz,&vJobExecutedRowsProcessedStore,false);
  
}

/*----------------------------------------------------------------------------*/
/* jobBegun.error@ -- fetch job error of participant from job__begun          */
/*----------------------------------------------------------------------------*/
void Task::vJobBegunErrorFetch(MP_Task mpTask)
{
  ENTRY("vJobBegunErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobBegun.error@"_Stz);
  
  Job__Begun* pJob__Begun = _pJob__Begun(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob__Begun->m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobBegun.error! -- store job error of participant in job__begun            */
/*----------------------------------------------------------------------------*/
void Task::vJobBegunErrorStore(MP_Task mpTask)
{
  ENTRY("vJobBegunErrorStore"_Stz);
  
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
void Task::vJobCancelledErrorFetch(MP_Task mpTask)
{
  ENTRY("vJobCancelledErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobCancelled.error@"_Stz);
  
  Job__Cancelled* pJob__Cancelled = _pJob__Cancelled(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob__Cancelled->m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobCancelled.error! -- store job error of participant in job__cancelled    */
/*----------------------------------------------------------------------------*/
void Task::vJobCancelledErrorStore(MP_Task mpTask)
{
  ENTRY("vJobCancelledErrorStore"_Stz);
  
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
void Task::vJobExecutedErrorFetch(MP_Task mpTask)
{
  ENTRY("vJobExecutedErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.error@"_Stz);
  
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob__Executed->m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.error! -- store job error of participant in job__executed      */
/*----------------------------------------------------------------------------*/
void Task::vJobExecutedErrorStore(MP_Task mpTask)
{
  ENTRY("vJobParticipantErrorFetch"_Stz);
  
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
void Task::vJobExecutedResponseFetch(MP_Task mpTask)
{
  ENTRY("vJobExecutedResponseFetch"_Stz);
  
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
void Task::vJobExecutedResponseStore(MP_Task mpTask)
{
  ENTRY("vJobExecutedResponseStore"_Stz);
  
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
/* jobExecuted.rowsProcessed! -- store rowsProcessed in job__executed         */
/*----------------------------------------------------------------------------*/
void Task::vJobExecutedRowsProcessedStore(MP_Task mpTask)
{
  ENTRY("vJobExecutedRowsProcessedStore"_Stz);
  
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
/* jobExecuted.rowsProcessed@ -- store rowsProcessed in job__executed         */
/*----------------------------------------------------------------------------*/
void Task::vJobExecutedRowsProcessedFetch(MP_Task mpTask)
{
  ENTRY("vJobExecutedRowsProcessedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.rowsProcessed@"_Stz);
  Job__Executed* pJob__Executed = _pJob__Executed(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob__Executed->m_iRowsProcessed);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
