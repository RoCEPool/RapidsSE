/*============================================================================*/
/* Steps_job.cpp - job step group                                             */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;
extern JobCatalog gxJobCatalog;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Job(void)
{
  vAddPrimitive("job.acksRemaining!"_Stz,   &vJobAcksRemaining,    false);
  vAddPrimitive("job.addOriginator"_Stz,    &vJobAddOriginator,    false);
  vAddPrimitive("job.addParticipant"_Stz,   &vJobAddParticipant,   false);
  vAddPrimitive("job.ancilliary@"_Stz,      &vJobAncilliaryFetch,  false);
  vAddPrimitive("jobAnalyzeIndex.isDetailed"_Stz, &vJobAnalyzeIndexIsDetailed, false);
  vAddPrimitive("jobAnalyzeTable.isDetailed"_Stz, &vJobAnalyzeTableIsDetailed, false);
  vAddPrimitive("jobBegun.delete"_Stz,      &vJobBegunDelete,      false);
  vAddPrimitive("jobBegun.error@"_Stz,      &vJobBegunErrorFetch,  false);
  vAddPrimitive("jobBegun.error!"_Stz,      &vJobBegunErrorStore,  false);
  vAddPrimitive("jobCancel.delete"_Stz,     &vJobCancelDelete,  false);
  vAddPrimitive("jobCancelled.delete"_Stz,  &vJobCancelledDelete,  false);
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
  vAddPrimitive("jobEnd.delete"_Stz,        &vJobEndDelete,  false);
  vAddPrimitive("jobEnded.delete"_Stz,      &vJobEndedDelete,  false);
  vAddPrimitive("job.error!"_Stz,           &vJobErrorStore,       false);
  vAddPrimitive("job.error@"_Stz,           &vJobErrorFetch,       false);
  vAddPrimitive("job.execute@"_Stz,         &vJobExecuteFetch,     false);
  vAddPrimitive("jobExecute.delete"_Stz,    &vJobExecuteDelete,    false);
  vAddPrimitive("jobExecuted.delete"_Stz,   &vJobExecutedDelete,   false);
  vAddPrimitive("jobExecuted.error@"_Stz,   &vJobExecutedErrorFetch,   false);
  vAddPrimitive("jobExecuted.error!"_Stz,   &vJobExecutedErrorStore,   false);
  vAddPrimitive("jobExecuted.response@"_Stz,&vJobExecutedResponseFetch,false);
  vAddPrimitive("jobExecuted.response!"_Stz,&vJobExecutedResponseStore,false);
  vAddPrimitive("jobExecuted.rowsProcessed@"_Stz,&vJobExecutedRowsProcessedFetch,false);
  vAddPrimitive("jobExecuted.rowsProcessed!"_Stz,&vJobExecutedRowsProcessedStore,false);
  vAddPrimitive("job.findOriginator"_Stz,   &vJobFindOriginator,   false);
  vAddPrimitive("job.findParticipant"_Stz,  &vJobFindParticipant,  false);
  vAddPrimitive("job.hasError"_Stz,         &vJobHasError,         false);
  vAddPrimitive("job.id@"_Stz,              &vJobIdFetch,          false);
  vAddPrimitive("job.indexName@"_Stz,       &vJobIndexNameFetch,   false);
  vAddPrimitive("job.isPromptCommand"_Stz,  &vJobIsPromptCommand,  false);
  vAddPrimitive("job_loadBuffer.insertRows"_Stz,   &vJobLoadBufferInsertRows,       false);
  vAddPrimitive("job_loadBuffer.loadJobId@"_Stz,   &vJobLoadBufferLoadJobIdFetch,   false);
  vAddPrimitive("job_loadBuffer.partition@"_Stz,   &vJobLoadBufferPartitionFetch,   false);
  vAddPrimitive("job_loadBuffer.table!"_Stz,   &vJobLoadBufferTableStore,   false);
  vAddPrimitive("job_loadTable.task@"_Stz,  &vJobLoadTableTaskFetch,   false);
  vAddPrimitive("job_loadTable.task!"_Stz,  &vJobLoadTableTaskStore,   false);
  vAddPrimitive("job.metadata!"_Stz,        &vJobMetadataStore,    false);
  vAddPrimitive("job.newBegun"_Stz,         &vJobNewBegun,         false);
  vAddPrimitive("job.newCancel"_Stz,        &vJobNewCancel,        false);
  vAddPrimitive("job.newCancelled"_Stz,     &vJobNewCancelled,     false);
  vAddPrimitive("job.newEnd"_Stz,           &vJobNewEnd,           false);
  vAddPrimitive("job.newEnded"_Stz,         &vJobNewEnded,         false);
  vAddPrimitive("job.newExecute"_Stz,       &vJobNewExecute,       false);
  vAddPrimitive("job.newExecuted"_Stz,      &vJobNewExecuted,      false);
  vAddPrimitive("job.objectName@"_Stz,      &vJobObjectNameFetch,  false);
  vAddPrimitive("job.originatingTask@"_Stz, &vJobOriginatingTaskFetch, false);
  vAddPrimitive("job.originatingTask!"_Stz, &vJobOriginatingTaskStore, false);
  vAddPrimitive("job::originator@"_Stz,     &vJobOriginatorFetch,  false);
  vAddPrimitive("job.participant@"_Stz,     &vJobParticipantFetch, false);
  vAddPrimitive("job.participant!"_Stz,     &vJobParticipantStore, false);
  vAddPrimitive("job.partitions@"_Stz,      &vJobPartitionsFetch,  false);
  vAddPrimitive("job.promptTask@"_Stz,      &vJobPromptTaskFetch,  false);
  
  vAddPrimitive("job_receiver.connNo@"_Stz, &vJobReceiverConnNoFetch,   false);
  vAddPrimitive("job_receiver.connNo!"_Stz, &vJobReceiverConnNoStore,   false);
  vAddPrimitive("job_receiver.socket@"_Stz, &vJobReceiverSocketFetch,   false);
  vAddPrimitive("job_receiver.socket!"_Stz, &vJobReceiverSocketStore,   false);
  
  vAddPrimitive("job.recv"_Stz,             &vJobRecv,             false);
  vAddPrimitive("job.regionSize@"_Stz,      &vJobRegionSizeFetch,  false);
  vAddPrimitive("job.releaseResources@"_Stz,&vJobReleaseResourcesFetch,false);
  vAddPrimitive("job.removeOriginator"_Stz, &vJobRemoveOriginator, false);
  vAddPrimitive("job.removeParticipant"_Stz,&vJobRemoveParticipant,false);
  vAddPrimitive("job.response@"_Stz,        &vJobResponseFetch,    false);
  vAddPrimitive("job.response!"_Stz,        &vJobResponseStore,    false);
  vAddPrimitive("job.rowsProcessed@"_Stz,   &vJobRowsProcessedFetch, false);
  vAddPrimitive("job.rowsProcessed!"_Stz,   &vJobRowsProcessedStore, false);
  vAddPrimitive("job.send"_Stz,             &vJobRecv,             false);
  vAddPrimitive("job.show"_Stz,             &vJobShow,             false);
  vAddPrimitive("job.signalConnector"_Stz,  &vJobSignalConnector,  false);
  vAddPrimitive("job.spawn"_Stz,            &vJobSpawn,            false);
  vAddPrimitive("job_subTruncate.truncateJobId@"_Stz,&vJobSubTruncateTruncateJobIdFetch, false);
  vAddPrimitive("job_subTruncate.slot@"_Stz,&vJobSubTruncateSlotFetch, false);
  vAddPrimitive("job::this"_Stz,            &vJobThis,             false);
  vAddPrimitive("job.trace!"_Stz,           &vJobTraceStore,       false);
  vAddPrimitive("job.trace@"_Stz,           &vJobTraceFetch,       false);
  vAddPrimitive("Job_truncateTable.spawnSub"_Stz, &vJobTruncateTableSpawnSub, false);
  vAddPrimitive("Job_truncateTable.task@"_Stz, &vJobTruncateTableTaskFetch, false);
  vAddPrimitive("Job_truncateTable.task!"_Stz, &vJobTruncateTableTaskStore, false);
  vAddPrimitive("job.workingBuffer@"_Stz,   &vJobWorkingBufferFetch, false);
  vAddPrimitive("job.workingBuffer!"_Stz,   &vJobWorkingBufferStore, false);
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
/* job.Ancilliary@ -- fetch ancilliary object from job                        */
/*----------------------------------------------------------------------------*/
void Task::vJobAncilliaryFetch(MP_Task mpTask)
{
  ENTRY("vJobAncilliaryFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobBegun.error@"_Stz);
  
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob->m_pAncillary);
  
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
  
  s256 sExecute;
  sExecute = ""_Stz;
  
  if (pJob->m_bCheckResources) {
    ASSERT(pJob->m_pJobClass->m_aSeedEnd);
    ASSERT(pJob->m_pJobClass->m_aSeedCan);
    sExecute = pJob->m_pJobClass->m_aName + 4;// EEK! pJob->sJobSeed();
    sExecute.vConcat("_chk"_Stz);
  }
  
  mpTask->m_pDataStack->vPushS256(sExecute);
  
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
/* job.class -- get the class of the job                                      */
/*----------------------------------------------------------------------------*/
void Task::vJobClass(MP_Task mpTask)
{
  ENTRY("vJobClass"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.class"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  eCN eClass = pJob->eClass();
  
  mpTask->m_pDataStack->vPushInt(eClass);
  
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
    THROW_INVALID_DSTACK("job.delete"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  // find the job in the catalog
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = gxJobCatalog.pFindJob(eClass);
  
  // call the job delete function
  pJobClass->vDeleteJob(nullptr, pJob);  // come back to this, remove seitf as parameter to delete
  
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
/* jobAnalyzeIndex.isDetailed -- is it a detailed analysis of the index       */
/*----------------------------------------------------------------------------*/
void Task::vJobAnalyzeIndexIsDetailed(MP_Task mpTask)
{
  ENTRY("vJobAnalyzeIndexIsDetailed"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobAnalyzeIndex.isDetailed"_Stz);
  
  Job_AnalyzeIndex* pJoAnalyzeIndex = _pJob_AnalyzeIndex(pJobPtr);
  ASSERT(pJoAnalyzeIndex->eClass() == CN_JOB_ANALYZEINDEX);
  
  mpTask->m_pDataStack->vPushInt(pJoAnalyzeIndex->m_bIsDetailed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobAnalyzeTable.isDetailed -- is it a detailed analysis of the index       */
/*----------------------------------------------------------------------------*/
void Task::vJobAnalyzeTableIsDetailed(MP_Task mpTask)
{
  ENTRY("vJobAnalyzeTableIsDetailed"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobAnalyzeTable.isDetailed"_Stz);
  
  Job_AnalyzeTable* pJoAnalyzeTable = _pJob_AnalyzeTable(pJobPtr);
  ASSERT(pJoAnalyzeTable->eClass() == CN_JOB_ANALYZETABLE);
  
  mpTask->m_pDataStack->vPushInt(pJoAnalyzeTable->m_bIsDetailed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobBegun.delete -- delete the begun job                                    */
/*----------------------------------------------------------------------------*/
void Task::vJobBegunDelete(MP_Task mpTask)
{
  ENTRY("vJobDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
  THROW_INVALID_DSTACK("jobBegun.delete"_Stz);
  
  Job__Begun* pJobBegun = _pJob__Begun(pJobPtr);
  ASSERT(pJobBegun->eClass() == CN_JOB__BEGUN);
  delete pJobBegun;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobCancel.delete -- delete the cancel job                                  */
/*----------------------------------------------------------------------------*/
void Task::vJobCancelDelete(MP_Task mpTask)
{
  ENTRY("vJobDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobCancel.delete"_Stz);
  
  Job__Cancel* pJobCancel = _pJob__Cancel(pJobPtr);
  ASSERT(pJobCancel->eClass() == CN_JOB__CANCEL);
  delete pJobCancel;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobCancelled.delete -- delete the cancelled job                            */
/*----------------------------------------------------------------------------*/
void Task::vJobCancelledDelete(MP_Task mpTask)
{
  ENTRY("vJobCancelledDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobCancelled.delete"_Stz);
  Job__Cancelled* pJobCancelled = _pJob__Cancelled(pJobPtr);
  ASSERT(pJobCancelled->eClass() == CN_JOB__CANCELLED);
  delete pJobCancelled;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobEnd.delete -- delete the end job                                        */
/*----------------------------------------------------------------------------*/
void Task::vJobEndDelete(MP_Task mpTask)
{
  ENTRY("vJobEndDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobEnd.delete"_Stz);
  
  Job__End* pJobEnd = _pJob__End(pJobPtr);
  ASSERT(pJobEnd->eClass() == CN_JOB__END);
  delete pJobEnd;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobEnded.delete -- delete the ended job                                    */
/*----------------------------------------------------------------------------*/
void Task::vJobEndedDelete(MP_Task mpTask)
{
  ENTRY("vJobEndedDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobEnded.delete"_Stz);
  
  Job__Ended* pJobEnded = _pJob__Ended(pJobPtr);
  ASSERT(pJobEnded->eClass() == CN_JOB__ENDED);
  delete pJobEnded;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecute.delete -- delete the execute job                                */
/*----------------------------------------------------------------------------*/
void Task::vJobExecuteDelete(MP_Task mpTask)
{
  ENTRY("vJobExecuteDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecute.delete"_Stz);
  
  Job__Execute* pJobExecute = _pJob__Execute(pJobPtr);
  ASSERT(pJobExecute->eClass() == CN_JOB__EXECUTE);
  delete pJobExecute;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* jobExecuted.delete -- delete the executed job                              */
/*----------------------------------------------------------------------------*/
void Task::vJobExecutedDelete(MP_Task mpTask)
{
  ENTRY("vJobExecutedDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("jobExecuted.delete"_Stz);
  
  Job__Executed* pJobExecuted = _pJob__Executed(pJobPtr);
  ASSERT(pJobExecuted->eClass() == CN_JOB__EXECUTED);
  delete pJobExecuted;
  
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
  
  pJob->m_bHasError = true;
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

  switch (pJob->eClass()) {
      
    case CN_JOB__BEGUN:
    case CN_JOB__CANCEL:
    case CN_JOB__CANCELLED:
    case CN_JOB__END:
    case CN_JOB__ENDED:
    case CN_JOB__EXECUTED:
    case CN_JOB__EXECUTE:
      ASSERT(0);
      break;
      
    default:
      sExecute = pJob->m_pJobClass->m_aSeedExe;
      //sExecute.vConcat("_node"_Stz);
      break;
  }
  
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
  
  Job__Cancelled* pFindJob = new Job__Cancelled;
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
  
  Job__Cancelled* pFindJob = new Job__Cancelled;
  pFindJob->m_iJobId = iJobId;
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  Job* pJob = gpAttachMap->m_pParticipatingJobs->pFindJob(pFindJob);
  gpAttachMap->m_pParticipatingJobs->vDropLock();
  
  delete pFindJob;
  
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.hasError -- return true if job has error                               */
/*----------------------------------------------------------------------------*/
void Task::vJobHasError(MP_Task mpTask)
{
  ENTRY("vJobErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.hasError"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_bHasError);
  
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
/* job.indexName@ -- fetch the index name of the job                          */
/*----------------------------------------------------------------------------*/
void Task::vJobIndexNameFetch(MP_Task mpTask)
{
  ENTRY("vJobIndexNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.indexName@"_Stz);
  Job_CreateSchema* pJob = _pJob_CreateSchema(pJobPtr);
  
  mpTask->m_pDataStack->vPushS32(pJob->m_sIndexName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.isPromptCpmmand -- is this job a prompt command job ?                 */
/*----------------------------------------------------------------------------*/
void Task::vJobIsPromptCommand(MP_Task mpTask)
{
  ENTRY("vJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.isPromptCpmmand"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_pPromptTask ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.insertRows -- insert rows from working buffer into table    */
/*----------------------------------------------------------------------------*/
void Task::vJobLoadBufferInsertRows(MP_Task mpTask)
{
  ENTRY("vJobLoadBufferInsertRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.insertRows!"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  // convert partition to node slot
  i8 iPartition = pJob_LoadBuffer->iPartition();
  i8 iSlot = TablePtn::iPartitionToSlot(iPartition);

  Table* pTable = pJob_LoadBuffer->pTable();

  WorkingBuffer* pWorkingBuffer = pJob_LoadBuffer->m_pWorkingBuffer;
  
  i8 iTxId = 0;
  
  pTable->vInsertEntries(iTxId, iSlot, pWorkingBuffer);
  
  TablePtn* pPartition = pTable->pPartition(iSlot);
  pPartition->vIncBuffersLoaded();
  
  i8 nBuffersLoadable = pJob_LoadBuffer->nBuffersLoadable();
  if (C_UNKNOWN != nBuffersLoadable)
    pPartition->vBuffersLoadable(nBuffersLoadable);
  
  bool bBuffersLoaded = pPartition->bBuffersLoaded();
  if (bBuffersLoaded)
    vTrace("Buffers loaded\n"_Stz);

  mpTask->m_pDataStack->vPushInt(bBuffersLoaded);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.loadJobId@ -- fetch load job id from job_loadBuffer         */
/*----------------------------------------------------------------------------*/
void Task::vJobLoadBufferLoadJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJobLoadBufferLoadJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.loadJobId@"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  i8 iLoadJobId = pJob_LoadBuffer->iLoadJobId();
  mpTask->m_pDataStack->vPushInt(iLoadJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.partition@ -- fetch partition no. from job_loadBuffer       */
/*----------------------------------------------------------------------------*/
void Task::vJobLoadBufferPartitionFetch(MP_Task mpTask)
{
  ENTRY("vJobLoadBufferPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.partition@"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  i8 i_Partition = pJob_LoadBuffer->iPartition();
  mpTask->m_pDataStack->vPushInt(i_Partition);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.table! -- store table into job_loadBuffer                   */
/*----------------------------------------------------------------------------*/
void Task::vJobLoadBufferTableStore(MP_Task mpTask)
{
  ENTRY("vJobLoadBufferTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table!"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_LoadBuffer->vTable(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadTable.task@ -- fetch task from job_loadTable                       */
/*----------------------------------------------------------------------------*/
void Task::vJobLoadTableTaskFetch(MP_Task mpTask)
{
  ENTRY("vJobLoadTableTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadTable.task@"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);
  
  Task* pTask = pJob_LoadTable->pTask();
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadTable.task! -- store task into job_loadTable                       */
/*----------------------------------------------------------------------------*/
void Task::vJobLoadTableTaskStore(MP_Task mpTask)
{
  ENTRY("vJobLoadTableTaskStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table@"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);

  Object* pTaskPtr;
  if ( ! mpTask->m_pDataStack->bPop(pTaskPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table@"_Stz);
  Task* pTask = _pTask(pTaskPtr);
  
  pJob_LoadTable->vTask(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.metadata! -- store meta data string in the job                         */
/*----------------------------------------------------------------------------*/
void Task::vJobMetadataStore(MP_Task mpTask)
{
  ENTRY("vJobMetadataStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.metadata"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  s4K sMetadata;
  if ( ! mpTask->m_pDataStack->bPop(sMetadata) )
    THROW_INVALID_DSTACK("seitf.metadata@"_Stz);
  
  pJob->m_sMetadata = sMetadata;
  
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
  
  JobClass* pJobClass__Begun = gxJobCatalog.pFindJob(CN_JOB__BEGUN);
  pJobBegun->m_pJobClass = pJobClass__Begun;
  
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
  
  JobClass* pJobClass__Cancel = gxJobCatalog.pFindJob(CN_JOB__CANCEL);
  pJobCancel->m_pJobClass = pJobClass__Cancel;
  
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
  
  JobClass* pJobClass__Cancelled = gxJobCatalog.pFindJob(CN_JOB__CANCELLED);
  pJobCancelled->m_pJobClass = pJobClass__Cancelled;
  
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
  
  JobClass* pJobClass__End = gxJobCatalog.pFindJob(CN_JOB__END);
  pJobEnd->m_pJobClass = pJobClass__End;
  
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
  
  JobClass* pJobClass__Ended = gxJobCatalog.pFindJob(CN_JOB__ENDED);
  pJobEnded->m_pJobClass = pJobClass__Ended;
  
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
  
  JobClass* pJobClass__Execute = gxJobCatalog.pFindJob(CN_JOB__EXECUTE);
  pJobExecute->m_pJobClass = pJobClass__Execute;
  
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
  
  JobClass* pJobClass__Executed = gxJobCatalog.pFindJob(CN_JOB__EXECUTED);
  pJobExecuted->m_pJobClass = pJobClass__Executed;
  
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
/* job.originatingTask@ -- fetch the originating task from the job            */
/*----------------------------------------------------------------------------*/
void Task::vJobOriginatingTaskFetch(MP_Task mpTask)
{
  ENTRY("vJobOriginatingTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.originatingTask@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob->m_pOriginatingTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingTask! -- store the originating task in the job              */
/*----------------------------------------------------------------------------*/
void Task::vJobOriginatingTaskStore(MP_Task mpTask)
{
  ENTRY("vJobOriginatingTaskStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.originatingTask!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  Object* pOriginatingTaskPtr;
  if ( ! mpTask->m_pDataStack->bPop(pOriginatingTaskPtr) )
    THROW_INVALID_DSTACK("job.originatingTask!"_Stz);
  Task* pOriginatingTask = _pTask(pOriginatingTaskPtr);
  ASSERT(CN_TASK == pOriginatingTask->eClass());
  
  pJob->m_pOriginatingTask = pOriginatingTask;
  
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
/* job.promptTask@ -- fetch the prompt task from the job                      */
/*----------------------------------------------------------------------------*/
void Task::vJobPromptTaskFetch(MP_Task mpTask)
{
  ENTRY("vJobPromptTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.promptTask@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob->m_pPromptTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.connNo@ -- fetch connNo from job_receiver                     */
/*----------------------------------------------------------------------------*/
void Task::vJobReceiverConnNoFetch(MP_Task mpTask)
{
  ENTRY("vJobReceiverConnNoFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_Receiver.connNo@"_Stz);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  i8 iConnNo = pJob_Receiver->iConnNo();
  mpTask->m_pDataStack->vPushInt(iConnNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.connNo! -- store connNo into job_receiver                     */
/*----------------------------------------------------------------------------*/
void Task::vJobReceiverConnNoStore(MP_Task mpTask)
{
  ENTRY("vJobReceiverConnNoStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_receiver.connNo!"_Stz);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);

  i8 iConnNo;
  if ( ! mpTask->m_pDataStack->bPop(iConnNo) )
    THROW_INVALID_DSTACK("job_receiver.connNo!"_Stz);
  
  pJob_Receiver->vConnNo(iConnNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.socket@ -- fetch socket from job_receiver                     */
/*----------------------------------------------------------------------------*/
void Task::vJobReceiverSocketFetch(MP_Task mpTask)
{
  ENTRY("vJobReceiverSocketFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_Receiver.socket@"_Stz);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  Socket* pSocket = pJob_Receiver->pSocket();
  mpTask->m_pDataStack->vPushObject(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_receiver.socket! -- store socket into job_receiver                     */
/*----------------------------------------------------------------------------*/
void Task::vJobReceiverSocketStore(MP_Task mpTask)
{
  ENTRY("vJobReceiverFetchStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_receiver.socket!"_Stz);
  Job_Receiver* pJob_Receiver = _pJob_Receiver(pJobPtr);
  
  Object* pSocketPtr;
  if ( ! mpTask->m_pDataStack->bPop(pSocketPtr) )
    THROW_INVALID_DSTACK("job_receiver.socket!"_Stz);
  Socket* pSocket = _pSocket(pSocketPtr);
  
  pJob_Receiver->vSocket(pSocket);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.recv -- post-receive function                                          */
/*----------------------------------------------------------------------------*/
void Task::vJobRecv(MP_Task mpTask)
{
  ENTRY("vJobRecv"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.recv"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  // find the job in the catalog
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = gxJobCatalog.pFindJob(eClass);
  
  // call the job post-receive function
  pJobClass->vRecvJob(pJob);  // come back to this, remove seitf as parameter to delete
  
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
  
  s256 sExecute;
  sExecute = ""_Stz;
  
  if (pJob->m_bCheckResources) {
    ASSERT(pJob->m_pJobClass->m_aSeedEnd);
    ASSERT(pJob->m_pJobClass->m_aSeedCan);
    sExecute = pJob->m_pJobClass->m_aName+4;//EEK! pJob->sJobSeed();
    sExecute.vConcat("_rel"_Stz);
  }
  
  mpTask->m_pDataStack->vPushS256(sExecute);
  
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

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed@ -- fetch the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Task::vJobRowsProcessedFetch(MP_Task mpTask)
{
  ENTRY("vJobRowsProcessedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.rowsProcessed@"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.rowsProcessed@"_Stz);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_xNode[iNode].m_iRowsProcessed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed! -- store the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Task::vJobRowsProcessedStore(MP_Task mpTask)
{
  ENTRY("vJobRowsProcessedStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.rowsProcessed!"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.rowsProcessed!"_Stz);
  
  i8 iRowsProcessed;
  if ( ! mpTask->m_pDataStack->bPop(iRowsProcessed) )
    THROW_INVALID_DSTACK("job.rowsProcessed!"_Stz);
  
  pJob->m_xNode[iNode].m_iRowsProcessed = iRowsProcessed;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.send -- pre-send of job                                                */
/*----------------------------------------------------------------------------*/
void Task::vJobSend(MP_Task mpTask)
{
  ENTRY("vJobSend"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.send"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  // find the job in the catalog
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = gxJobCatalog.pFindJob(eClass);
  
  // call the job pre-send function
  pJobClass->vSendJob(pJob);  // come back to this, remove seitf as parameter to delete
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* job.show -- show the job                                                   */
/*----------------------------------------------------------------------------*/
void Task::vJobShow(MP_Task mpTask)
{
  ENTRY("vJobShow"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.show"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  // find the job in the catalog
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = gxJobCatalog.pFindJob(eClass);
  
  // call the job delete function
  pJobClass->vShowJob(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.signalConnector -- signal the connector thread                         */
/*----------------------------------------------------------------------------*/
void Task::vJobSignalConnector(MP_Task mpTask)
{
  ENTRY("vJobSignalConnector"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.signalConnector"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  // there should not be a prompt task
  ASSERT( ! pJob->m_pPromptTask);
  
  i8 rCode = pthread_cond_signal(&pJob->m_xThreadCond);
  
  if (0 != rCode) {
    vWarn("pthread_cond_signal() error; rCode=%lld\n"_Stz,rCode);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.spawn -- spawn the job on the stack                                    */
/*----------------------------------------------------------------------------*/
void Task::vJobSpawn(MP_Task mpTask)
{
  ENTRY("vJobSpawn"_Stz);
  
  i8 i_Seed;
  if ( ! mpTask->m_pDataStack->bPop(i_Seed) )
    THROW_INVALID_DSTACK("job.spawn"_Stz);

  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.spawn"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  // there should not be a prompt task
  //ASSERT( ! pJob->m_pPromptTask);
 
  // form the word to execute for the request
  s32 sSeed;
  
  switch (i_Seed) {
    case C_SEED_1: sSeed = pJob->m_pJobClass->m_aSeedReq; break;
    case C_SEED_2: sSeed = pJob->m_pJobClass->m_aSeedRcv; break;
    case C_SEED_3: sSeed = pJob->m_pJobClass->m_aSeedExe; break;
    default: NEVER_REACH;
  }
  
  Task* pNewTask = new Task;

  pNewTask->vSeedWord(sSeed);
  pNewTask->vState(TS_READY);
  pNewTask->vR35model(true);
  pNewTask->vActorModel(AC_R35);
//  pNewTask->vSEitf(pSEitf);
  
  pNewTask->vJob(pJob);
  
  Handlers::vSpawnedTaskHandler(pNewTask);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subTruncate.slot@ -- fetch the job slot                                */
/*----------------------------------------------------------------------------*/
void Task::vJobSubTruncateSlotFetch(MP_Task mpTask)
{
  ENTRY("vJobSubTruncateSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subTruncate.slot@"_Stz);
  Job_SubTruncate* pJob = _pJob_SubTruncate(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subTruncate.truncateJobId@ -- fetch the truncate job id                */
/*----------------------------------------------------------------------------*/
void Task::vJobSubTruncateTruncateJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJobSubTruncateTruncateJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subTruncate.truncateJobId@"_Stz);
  Job_SubTruncate* pJob = _pJob_SubTruncate(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iTruncateJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.trace@ -- fetch the job trace                                          */
/*----------------------------------------------------------------------------*/
void Task::vJobTraceFetch(MP_Task mpTask)
{
  ENTRY("vJobTraceFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.trace@"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  eJT eJobTrace = pJob->eJobTrace();
  
  mpTask->m_pDataStack->vPushInt(eJobTrace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::this -- get this job from this task                                   */
/*----------------------------------------------------------------------------*/
void Task::vJobThis(MP_Task mpTask)
{
  ENTRY("vJobThis"_Stz);
  
  Job* pJob = mpTask->m_pJob;
  
  mpTask->m_pDataStack->vPushObject(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.trace! -- store the job trace                                          */
/*----------------------------------------------------------------------------*/
void Task::vJobTraceStore(MP_Task mpTask)
{
  ENTRY("vJobTraceStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.trace!"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  i8 iJobTrace;
  if ( ! mpTask->m_pDataStack->bPop(iJobTrace) )
    THROW_INVALID_DSTACK("job.trace!"_Stz);
  
  pJob->vJobTrace((eJT)iJobTrace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_truncateTable.spawnSub -- spawn sub truncate job                       */
/*----------------------------------------------------------------------------*/
void Task::vJobTruncateTableSpawnSub(MP_Task mpTask)
{
  ENTRY("vJobTruncateTableSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_truncateTable.spawnSub"_Stz);
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJobPtr);
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iSlot) )
    THROW_INVALID_DSTACK("Job_truncateTable.spawnSub"_Stz);
  
  SEitf* pSEitf = pJob_TruncateTable->m_pSEitf;
  i8 iTruncateJobId = pJob_TruncateTable->m_iJobId;
  
  JobClass* pJobClass_SubTruncate = gxJobCatalog.pFindJob(CN_JOB_SUBTRUNCATE);
  
  Job_SubTruncate* pJob_SubTruncate =
  static_cast<Job_SubTruncate*>(pJobClass_SubTruncate->pPreJob(pSEitf));
  
  pJob_SubTruncate->m_iTruncateJobId = iTruncateJobId;
  pJob_SubTruncate->m_sObjectName = pJob_TruncateTable->m_sObjectName;
  pJob_SubTruncate->m_iSlot = iSlot;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Task* pSubTruncateTask = new Task;
  pSubTruncateTask->vParentTask(*mpTask);
  
  pSEitf->bAppendTask(pSubTruncateTask,pJob_SubTruncate);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_truncateTable.task@ -- fetch task from job_truncateTable               */
/*----------------------------------------------------------------------------*/
void Task::vJobTruncateTableTaskFetch(MP_Task mpTask)
{
  ENTRY("vJobTruncateTableTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_truncateTable.task@"_Stz);
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJobPtr);
  
  Task* pTask = pJob_TruncateTable->m_pTask;
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_truncateTable.task! -- store task in job_truncateTable                 */
/*----------------------------------------------------------------------------*/
void Task::vJobTruncateTableTaskStore(MP_Task mpTask)
{
  ENTRY("vJobTruncateTableTaskStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_truncateTable.task!"_Stz);
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJobPtr);
  
  Object* pTaskPtr;
  if ( ! mpTask->m_pDataStack->bPop(pTaskPtr) )
    THROW_INVALID_DSTACK("Job_truncateTable.task!"_Stz);
  Task* pTask = _pTask(pTaskPtr);
  
  pJob_TruncateTable->m_pTask = pTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.workingBuffer! -- store working buffer in the job                      */
/*----------------------------------------------------------------------------*/
void Task::vJobWorkingBufferStore(MP_Task mpTask)
{
  ENTRY("vJobWorkingBufferStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.WorkingBuffer!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  Object* pWorkingBufferPtr;
  if ( ! mpTask->m_pDataStack->bPop(pWorkingBufferPtr) )
    THROW_INVALID_DSTACK("job.WorkingBuffer!"_Stz);
  WorkingBuffer* pWorkingBuffer = _pWorkingBuffer(pWorkingBufferPtr);

  pJob->m_pWorkingBuffer = pWorkingBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.workingBuffer@ -- fetch working buffer from job                        */
/*----------------------------------------------------------------------------*/
void Task::vJobWorkingBufferFetch(MP_Task mpTask)
{
  ENTRY("vJobWorkingBufferStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
  THROW_INVALID_DSTACK("job.WorkingBuffer@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob->m_pWorkingBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
