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
void Task::vAddPrimitives_Job(void)
{
  vAddPrimitive("job.acksRemaining!"_Stz,   &vJobAcksRemaining,    false);
  vAddPrimitive("job.addOriginator"_Stz,    &vJobAddOriginator,    false);
  vAddPrimitive("job.addParticipant"_Stz,   &vJobAddParticipant,   false);
  vAddPrimitive("jobBegun.error@"_Stz,      &vJobBegunErrorFetch,  false);
  vAddPrimitive("jobBegun.error!"_Stz,      &vJobBegunErrorStore,  false);
  vAddPrimitive("jobCancelled.error@"_Stz,  &vJobCancelledErrorFetch, false);
  vAddPrimitive("jobCancelled.error!"_Stz,  &vJobCancelledErrorStore, false);
  vAddPrimitive("job.checkResources@"_Stz,  &vJobCheckResourcesFetch,false);
  vAddPrimitive("job.class"_Stz,            &vJobClass,            false);
  vAddPrimitive("job.columnCount@"_Stz,     &vJobColumnCountFetch, false);
  vAddPrimitive("job.columnIsPartKey"_Stz,  &vJobColumnIsPartKey,  false);
  vAddPrimitive("job.columnName@"_Stz,      &vJobColumnNameFetch,  false);
  vAddPrimitive("job.columnType@"_Stz,      &vJobColumnTypeFetch,  false);
  vAddPrimitive("job.decAcksRemaining"_Stz, &vJobDecAcksRemaining, false);
  vAddPrimitive("job.delete"_Stz,           &vJobDelete,           false);
  vAddPrimitive("job.dicPath"_Stz,          &vJobDicPath,          false);
  vAddPrimitive("job.error!"_Stz,           &vJobErrorStore,       false);
  vAddPrimitive("job.error@"_Stz,           &vJobErrorFetch,       false);
  vAddPrimitive("job.execute@"_Stz,         &vJobExecuteFetch,     false);
  vAddPrimitive("jobExecuted.error@"_Stz,   &vJobExecutedErrorFetch,   false);
  vAddPrimitive("jobExecuted.error!"_Stz,   &vJobExecutedErrorStore,   false);
  vAddPrimitive("jobExecuted.response@"_Stz,&vJobExecutedResponseFetch,false);
  vAddPrimitive("jobExecuted.response!"_Stz,&vJobExecutedResponseStore,false);
  vAddPrimitive("job.findOriginator"_Stz,   &vJobFindOriginator,   false);
  vAddPrimitive("job.findParticipant"_Stz,  &vJobFindParticipant,  false);
  vAddPrimitive("job.id@"_Stz,              &vJobIdFetch,          false);
  vAddPrimitive("job.newBegun"_Stz,         &vJobNewBegun,         false);
  vAddPrimitive("job.newCancel"_Stz,        &vJobNewCancel,        false);
  vAddPrimitive("job.newCancelled"_Stz,     &vJobNewCancelled,     false);
  vAddPrimitive("job.newEnd"_Stz,           &vJobNewEnd,           false);
  vAddPrimitive("job.newEnded"_Stz,         &vJobNewEnded,         false);
  vAddPrimitive("job.newExecute"_Stz,       &vJobNewExecute,       false);
  vAddPrimitive("job.newExecuted"_Stz,      &vJobNewExecuted,      false);
  vAddPrimitive("job.objectName@"_Stz,      &vJobObjectNameFetch,  false);
  vAddPrimitive("job::originator@"_Stz,     &vJobOriginatorFetch,  false);
  vAddPrimitive("job.participant@"_Stz,     &vJobParticipantFetch, false);
  vAddPrimitive("job.participant!"_Stz,     &vJobParticipantStore, false);
  vAddPrimitive("job.partitions@"_Stz,      &vJobPartitionsFetch,false);
  vAddPrimitive("job.regionSize@"_Stz,      &vJobRegionSizeFetch,false);
  vAddPrimitive("job.releaseResources@"_Stz,  &vJobReleaseResourcesFetch,false);
  vAddPrimitive("job.removeOriginator"_Stz, &vJobRemoveOriginator, false);
  vAddPrimitive("job.removeParticipant"_Stz,&vJobRemoveParticipant,false);
  vAddPrimitive("job.response@"_Stz,        &vJobResponseFetch,    false);
  vAddPrimitive("job.response!"_Stz,        &vJobResponseStore,    false);
}

/*----------------------------------------------------------------------------*/
/* job.acksRemaining! -- set acks remaining for this job                      */
/*----------------------------------------------------------------------------*/
void Task::vJobAcksRemaining(MP_Task mpTask)
{
  ENTRY("vJobAcksRemaining"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.acksRemaining!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iAcksRemaing;
  if ( ! mpTask->m_pDataStack->bPop(iAcksRemaing) )
    THROW_INVALID_DSTACK("job.acksRemaining!"_Stz);
  
  pJob->m_ziAcksRemaining = iAcksRemaing;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addOriginator -- add job to originator job set                         */
/*----------------------------------------------------------------------------*/
void Task::vJobAddOriginator(MP_Task mpTask)
{
  ENTRY("vJobAddOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.addOriginator"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  gpAttachMap->m_pOriginatingJobs->vGrabLock();
  gpAttachMap->m_pOriginatingJobs->vAddJob(pJob);
  i8 iJobId = gpAttachMap->m_pOriginatingJobs->iNextJobId();
  pJob->m_iJobId = iJobId;
  gpAttachMap->m_pOriginatingJobs->vDropLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addParticipant -- add job to participant job set                       */
/*----------------------------------------------------------------------------*/
void Task::vJobAddParticipant(MP_Task mpTask)
{
  ENTRY("vJobAddParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.addParticipant"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  gpAttachMap->m_pParticipatingJobs->vAddJob(pJob);
  gpAttachMap->m_pParticipatingJobs->vDropLock();
  
  EXIT();
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
/* job.checkResources@ -- fetch the word to be executed for checking resources*/
/*----------------------------------------------------------------------------*/
void Task::vJobCheckResourcesFetch(MP_Task mpTask)
{
  ENTRY("vJobCheckResourcesFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.checkResources@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  s32 sExecute;
  sExecute = pJob->m_sCheckResources;
  
  mpTask->m_pDataStack->vPushS32(sExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.class -- get the class of the job                                      */
/*----------------------------------------------------------------------------*/
void Task::vJobClass(MP_Task mpTask)
{
  ENTRY("vJobClass"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.class"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->eClass());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.columnCount@ -- fetch the no of columns of the create table job        */
/*----------------------------------------------------------------------------*/
void Task::vJobColumnCountFetch(MP_Task mpTask)
{
  ENTRY("vJobColumnCountFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.columnCount@"_Stz);
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob_CreateTable->m_iColumns);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.columnIsPartKey -- return true if column is a partition by key        */
/*----------------------------------------------------------------------------*/
void Task::vJobColumnIsPartKey(MP_Task mpTask)
{
  ENTRY("vJobColumnIsPartKey"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.columnIsPartKey"_Stz);
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJobPtr);
  
  i8 iColumnNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iColumnNo) )
    THROW_INVALID_DSTACK("job.columnIsPartKey@"_Stz);
  
  mpTask->m_pDataStack->vPushInt(pJob_CreateTable->m_xColumns[iColumnNo].m_bIsPartitionKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.columnName@ -- fetch the name of the column of the create table job    */
/*----------------------------------------------------------------------------*/
void Task::vJobColumnNameFetch(MP_Task mpTask)
{
  ENTRY("vJobColumnNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.columnName@"_Stz);
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJobPtr);
  
  i8 iColumnNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iColumnNo) )
    THROW_INVALID_DSTACK("job.columnName@"_Stz);
  
  mpTask->m_pDataStack->vPushS32(pJob_CreateTable->m_xColumns[iColumnNo].m_sColumnName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.columnType@ -- fetch column type from create table job                 */
/*----------------------------------------------------------------------------*/
void Task::vJobColumnTypeFetch(MP_Task mpTask)
{
  ENTRY("vJobColumnTypeFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.columnType@"_Stz);
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJobPtr);
  
  i8 iColumnNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iColumnNo) )
    THROW_INVALID_DSTACK("job.columnType@"_Stz);
  
  mpTask->m_pDataStack->vPushInt(pJob_CreateTable->m_xColumns[iColumnNo].m_eColumnType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.decAcksRemaining -- decrement acks remaining for this job              */
/*----------------------------------------------------------------------------*/
void Task::vJobDecAcksRemaining(MP_Task mpTask)
{
  ENTRY("vJobDecAcksRemaining"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.decAcksRemaining"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  --pJob->m_ziAcksRemaining;
  
  mpTask->m_pDataStack->vPushInt(*pJob->m_ziAcksRemaining);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.delete -- delete the job                                               */
/*----------------------------------------------------------------------------*/
void Task::vJobDelete(MP_Task mpTask)
{
  ENTRY("vJobDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
  THROW_INVALID_DSTACK("job.class"_Stz);
  
  Job* pJob = _pJob(pJobPtr);
  
  switch (pJob->eClass()) {
    
    case CN_JOB__BEGUN:
    {
      Job__Begun* pJobBegun = _pJob__Begun(pJobPtr);
      delete pJobBegun;
      break;
    }
    
    case CN_JOB__CANCEL:
    {
      Job__Cancel* pJobCancel = _pJob__Cancel(pJobPtr);
      delete pJobCancel;
      break;
    }
      
    case CN_JOB__CANCELLED:
    {
      Job__Cancelled* pJobCancelled = _pJob__Cancelled(pJobPtr);
      delete pJobCancelled;
      break;
    }
      
    case CN_JOB__END:
    {
      Job__End* pJobEnd = _pJob__End(pJobPtr);
      delete pJobEnd;
      break;
    }
    
    case CN_JOB__ENDED:
    {
      Job__Ended* pJobEnded = _pJob__Ended(pJobPtr);
      delete pJobEnded;
      break;
    }
    
    case CN_JOB__EXECUTE:
    {
      Job__Execute* pJobExecute = _pJob__Execute(pJobPtr);
      delete pJobExecute;
      break;
    }
    
    case CN_JOB__EXECUTED:
    {
      Job__Executed* pJobExecuted = _pJob__Executed(pJobPtr);
      delete pJobExecuted;
      break;
    }
    
    case CN_JOB_G35:
    {
      Job_G35* pJobG35 = _pJob_G35(pJobPtr);
      delete pJobG35;
      break;
    }
    
    case CN_JOB_CREATECATALOG:
    {
      Job_CreateCatalog* pJobCreateCatalog = _pJob_CreateCatalog(pJobPtr);
      delete pJobCreateCatalog;
      break;
    }
      
    case CN_JOB_CREATEINDEX:
    {
      Job_CreateIndex* pJobCreateIndex = _pJob_CreateIndex(pJobPtr);
      delete pJobCreateIndex;
      break;
    }
      
    case CN_JOB_CREATESCHEMA:
    {
      Job_CreateSchema* pJobCreateSchema = _pJob_CreateSchema(pJobPtr);
      delete pJobCreateSchema;
      break;
    }
   
    case CN_JOB_CREATETABLE:
    {
      Job_CreateTable* pJobCreateTable = _pJob_CreateTable(pJobPtr);
      delete pJobCreateTable;
      break;
    }
     
    case CN_JOB_CREATETENANT:
    {
      Job_CreateTenant* pJobCreateTenant = _pJob_CreateTenant(pJobPtr);
      delete pJobCreateTenant;
      break;
    }
      
    case CN_JOB_DROPCATALOG:
    {
      Job_DropCatalog* pJobDropCatalog = _pJob_DropCatalog(pJobPtr);
      delete pJobDropCatalog;
      break;
    }
      
    case CN_JOB_DROPINDEX:
    {
      Job_DropIndex* pJobDropIndex = _pJob_DropIndex(pJobPtr);
      delete pJobDropIndex;
      break;
    }
      
    case CN_JOB_DROPSCHEMA:
    {
      Job_DropSchema* pJobDropSchema = _pJob_DropSchema(pJobPtr);
      delete pJobDropSchema;
      break;
    }
      
    case CN_JOB_DROPTABLE:
    {
      Job_DropTable* pJobDropTable = _pJob_DropTable(pJobPtr);
      delete pJobDropTable;
      break;
    }
     
    case CN_JOB_DROPTENANT:
    {
      Job_DropTenant* pJobDropTenant = _pJob_DropTenant(pJobPtr);
      delete pJobDropTenant;
      break;
    }
      
    case CN_JOB_SANITY:
    {
      Job_Sanity* pJobSanity = _pJob_Sanity(pJobPtr);
      delete pJobSanity;
      break;
    }
      
    case CN_JOB_SPACE:
    {
      Job_Space* pJobSpace = _pJob_Space(pJobPtr);
      delete pJobSpace;
      break;
    }
      
    case CN_JOB_STATUS:
    {
      Job_Status* pJobStatus = _pJob_Status(pJobPtr);
      delete pJobStatus;
      break;
    }
      
    default:
      ASSERT(0);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.dicPath -- get job dic path                                            */
/*----------------------------------------------------------------------------*/
void Task::vJobDicPath(MP_Task mpTask)
{
  ENTRY("vJobDicPath"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.dicPath"_Stz);
  Job_CreateSchema* pJob = _pJob_CreateSchema(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(&pJob->m_xDicPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error@ -- fetch the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Task::vJobErrorFetch(MP_Task mpTask)
{
  ENTRY("vJobErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.error@"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.error@"_Stz);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_xNode[iNode].m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error! -- store the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Task::vJobErrorStore(MP_Task mpTask)
{
  ENTRY("vJobErrorStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.error!"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.error!"_Stz);
  
  i8 iJobError;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobError) )
    THROW_INVALID_DSTACK("job.error!"_Stz);
  
  pJob->m_xNode[iNode].m_eErrorCode = (eEC)iJobError;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.execute@ -- fetch the job word to be executed                          */
/*----------------------------------------------------------------------------*/
void Task::vJobExecuteFetch(MP_Task mpTask)
{
  ENTRY("vJobExecuteFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.execute@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  s32 sExecute;
  sExecute = pJob->sJobExecute();
  
  mpTask->m_pDataStack->vPushS32(sExecute);
  
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
/* job.findOriginator -- find job from originator job set                     */
/*----------------------------------------------------------------------------*/
void Task::vJobFindOriginator(MP_Task mpTask)
{
  ENTRY("vJobFindOriginatort"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job.findOriginator"_Stz);
  
  Job__Begun* pFindJob = new Job__Begun;
  pFindJob->m_iJobId = iJobId;
  
  gpAttachMap->m_pOriginatingJobs->vGrabLock();
  Job* pJob = gpAttachMap->m_pOriginatingJobs->pFindJob(pFindJob);
  gpAttachMap->m_pOriginatingJobs->vDropLock();
  
  delete pFindJob;
  ASSERT(pJob);
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.findParticipant -- find job from participant job set                   */
/*----------------------------------------------------------------------------*/
void Task::vJobFindParticipant(MP_Task mpTask)
{
  ENTRY("vJobFindParticipant"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job.findPartcipant"_Stz);
  
  Job__Begun* pFindJob = new Job__Begun;
  pFindJob->m_iJobId = iJobId;
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  Job* pJob = gpAttachMap->m_pParticipatingJobs->pFindJob(pFindJob);
  gpAttachMap->m_pParticipatingJobs->vDropLock();
  
  delete pFindJob;
  
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.id@ -- fetch the job id of the job                                     */
/*----------------------------------------------------------------------------*/
void Task::vJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.id@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newBegun -- new a job__begun for this job                              */
/*----------------------------------------------------------------------------*/
void Task::vJobNewBegun(MP_Task mpTask)
{
  ENTRY("vJobNewBegun"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
  THROW_INVALID_DSTACK("job.newBegun"_Stz);
  
  Job* pJobBegun = new Job__Begun;
  pJobBegun->m_iJobId = iJobId;
  pJobBegun->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newCancel -- new a job__cancel for this job                            */
/*----------------------------------------------------------------------------*/
void Task::vJobNewCancel(MP_Task mpTask)
{
  ENTRY("vJobNewCancel"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
    THROW_INVALID_DSTACK("job.newCancel"_Stz);
  
  Job* pJobCancel = new Job__Cancel;
  pJobCancel->m_iJobId = iJobId;
  pJobCancel->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobCancel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newCancelled -- new a job__cancelled for this job                      */
/*----------------------------------------------------------------------------*/
void Task::vJobNewCancelled(MP_Task mpTask)
{
  ENTRY("vJobNewCancelled"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
    THROW_INVALID_DSTACK("job.newCancelled"_Stz);
  
  Job* pJobCancelled = new Job__Cancelled;
  pJobCancelled->m_iJobId = iJobId;
  pJobCancelled->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobCancelled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newEnd - new a job__end for this job                                   */
/*----------------------------------------------------------------------------*/
void Task::vJobNewEnd(MP_Task mpTask)
{
  ENTRY("vJobNewEnd"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
  THROW_INVALID_DSTACK("job.newEnd"_Stz);
  
  Job* pJobEnd = new Job__End;
  pJobEnd->m_iJobId = iJobId;
  pJobEnd->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobEnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newEnded -- new a job__ended for this job                              */
/*----------------------------------------------------------------------------*/
void Task::vJobNewEnded(MP_Task mpTask)
{
  ENTRY("vJobNewEnded"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
  THROW_INVALID_DSTACK("job.newEnded"_Stz);
  
  Job* pJobEnded = new Job__Ended;
  pJobEnded->m_iJobId = iJobId;
  pJobEnded->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newExecute -- new a job__execute for this job                          */
/*----------------------------------------------------------------------------*/
void Task::vJobNewExecute(MP_Task mpTask)
{
  ENTRY("vJobNewExecute"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
  THROW_INVALID_DSTACK("job.newExecute"_Stz);
  
  Job* pJobExecute = new Job__Execute;
  pJobExecute->m_iJobId = iJobId;
  pJobExecute->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newExecuted -- new a job__executed for this job                        */
/*----------------------------------------------------------------------------*/
void Task::vJobNewExecuted(MP_Task mpTask)
{
  ENTRY("vJobNewExecuted"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPop(iJobId) )
  THROW_INVALID_DSTACK("job.newExecuted"_Stz);
  
  Job* pJobExecuted = new Job__Executed;
  pJobExecuted->m_iJobId = iJobId;
  
  pJobExecuted->m_iParticipantNode = gpAttachMap->m_pGrid->m_iThisNode;
  
  mpTask->m_pDataStack->vPushObject(pJobExecuted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.objectName@ -- fetch the object name of the job                        */
/*----------------------------------------------------------------------------*/
void Task::vJobObjectNameFetch(MP_Task mpTask)
{
  ENTRY("vJobObjectNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.objectNamed@"_Stz);
  Job_CreateSchema* pJob = _pJob_CreateSchema(pJobPtr);
  
  mpTask->m_pDataStack->vPushS32(pJob->m_sObjectName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::originator@ -- fetch the originator node from the job                  */
/*----------------------------------------------------------------------------*/
void Task::vJobOriginatorFetch(MP_Task mpTask)
{
  ENTRY("vJobOriginatorFetch"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job::originator@@"_Stz);
  
  mpTask->m_pDataStack->vPushInt(Job::iOriginatorNode(iJobId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participant@ -- fetch the participant node from the job                */
/*----------------------------------------------------------------------------*/
void Task::vJobParticipantFetch(MP_Task mpTask)
{
  ENTRY("vJobRemoveParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.partcipant@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iParticipantNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.partcipant! -- store the participant node in the job                   */
/*----------------------------------------------------------------------------*/
void Task::vJobParticipantStore(MP_Task mpTask)
{
  ENTRY("vJobParticipantStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.partcipant!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iParticipantNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iParticipantNode) )
    THROW_INVALID_DSTACK("job.partcipant!"_Stz);
  
  pJob->m_iParticipantNode = iParticipantNode;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.releaseResources@ -- fetch the word to execute for releasing resources */
/*----------------------------------------------------------------------------*/
void Task::vJobReleaseResourcesFetch(MP_Task mpTask)
{
  ENTRY("vJobReleaseResourcesFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.releaseResources@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  s32 sExecute;
  sExecute = pJob->m_sReleaseResources;
  
  mpTask->m_pDataStack->vPushS32(sExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removeOriginator -- remove job from originator job set                 */
/*----------------------------------------------------------------------------*/
void Task::vJobRemoveOriginator(MP_Task mpTask)
{
  ENTRY("vJobRemoveOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.id@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  gpAttachMap->m_pOriginatingJobs->vGrabLock();
  gpAttachMap->m_pOriginatingJobs->vRemoveJob(pJob);
  gpAttachMap->m_pOriginatingJobs->vDropLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removePartcipant -- remove job from participant job set                */
/*----------------------------------------------------------------------------*/
void Task::vJobRemoveParticipant(MP_Task mpTask)
{
  ENTRY("vJobRemoveParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.id@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  gpAttachMap->m_pParticipatingJobs->vRemoveJob(pJob);
  gpAttachMap->m_pParticipatingJobs->vDropLock();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.partitions@ -- fetch partitions from job_sanity                        */
/*----------------------------------------------------------------------------*/
void Task::vJobPartitionsFetch(MP_Task mpTask)
{
  ENTRY("vJobParticipantResponseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.partitions@"_Stz);
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob_Sanity->m_iPartitions);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.regionSizeResponse@ -- fetch region size from job_sanity               */
/*----------------------------------------------------------------------------*/
void Task::vJobRegionSizeFetch(MP_Task mpTask)
{
  ENTRY("vJobRegionSizeFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.regionSize@"_Stz);
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob_Sanity->m_iRegionSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.response@ -- fetch the job response from a node                        */
/*----------------------------------------------------------------------------*/
void Task::vJobResponseFetch(MP_Task mpTask)
{
  ENTRY("vJobResponseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.response@"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.response@"_Stz);
  
  mpTask->m_pDataStack->vPushS256(pJob->m_xNode[iNode].m_sResponse);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.response! -- store the job response from a node                        */
/*----------------------------------------------------------------------------*/
void Task::vJobResponseStore(MP_Task mpTask)
{
  ENTRY("vJobResponseStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.response!"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.response!"_Stz);
  
  s256 sResponse;
  if ( ! mpTask->m_pDataStack->bPop(sResponse) )
    THROW_INVALID_DSTACK("job.response!"_Stz);
  
  pJob->m_xNode[iNode].m_sResponse = sResponse;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
