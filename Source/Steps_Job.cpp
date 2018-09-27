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
  vAddPrimitive("job.ancilliary@"_Stz,      &vJobAncilliaryFetch,  false);
  vAddPrimitive("jobAnalyzeIndex.isDetailed"_Stz, &vJobAnalyzeIndexIsDetailed, false);
  vAddPrimitive("jobAnalyzeTable.isDetailed"_Stz, &vJobAnalyzeTableIsDetailed, false);
  vAddPrimitive("jobBegun.error@"_Stz,      &vJobBegunErrorFetch,  false);
  vAddPrimitive("jobBegun.error!"_Stz,      &vJobBegunErrorStore,  false);
  vAddPrimitive("jobCancelled.error@"_Stz,  &vJobCancelledErrorFetch, false);
  vAddPrimitive("jobCancelled.error!"_Stz,  &vJobCancelledErrorStore, false);
  vAddPrimitive("job.class"_Stz,            &vJobClass,            false);
  vAddPrimitive("job.columnCount@"_Stz,     &vJobColumnCountFetch, false);
  vAddPrimitive("job.columnIsPartKey"_Stz,  &vJobColumnIsPartKey,  false);
  vAddPrimitive("job.columnName@"_Stz,      &vJobColumnNameFetch,  false);
  vAddPrimitive("job.columnType@"_Stz,      &vJobColumnTypeFetch,  false);
  vAddPrimitive("job_createIndex.isUnique@"_Stz, &vJobCreateIndexIsUniqueFetch,  false);
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
  vAddPrimitive("jobExecuted.rowsProcessed@"_Stz,&vJobExecutedRowsProcessedFetch,false);
  vAddPrimitive("jobExecuted.rowsProcessed!"_Stz,&vJobExecutedRowsProcessedStore,false);
  
  vAddPrimitive("Job_fetchRows.lastBuffers!"_Stz, &vJobFetchRowsLastBuffersStore, false);
  vAddPrimitive("Job_fetchRows.partition@"_Stz, &vJobFetchRowsPartitionFetch,   false);
  vAddPrimitive("Job_fetchRows.planner!"_Stz, &vJobFetchRowsPlannerStore,   false);
  vAddPrimitive("Job_fetchRows.spawnSub"_Stz, &vJobFetchRowsSpawnSub, false);
  vAddPrimitive("Job_fetchRows.table!"_Stz,   &vJobFetchRowsTableStore,   false);
  vAddPrimitive("Job_fetchRows.where@"_Stz, &vJobFetchRowsWhereFetch, false);
  
  vAddPrimitive("job::findOriginator"_Stz,   &vJobFindOriginator,   false);
  vAddPrimitive("job::findParticipant"_Stz,  &vJobFindParticipant,  false);
  vAddPrimitive("job.hasError"_Stz,         &vJobHasError,         false);
  vAddPrimitive("job.hasWorkingBuffer"_Stz, &vJobHasWorkingBuffer, false);
  vAddPrimitive("job.id@"_Stz,              &vJobIdFetch,          false);
  vAddPrimitive("job.indexName@"_Stz,       &vJobIndexNameFetch,   false);
  
  vAddPrimitive("job_insertBuffer.partition@"_Stz, &vJob_InsertBufferPartitionFetch, false);
  vAddPrimitive("job_insertBuffer.endInsertJob@"_Stz, &vJob_InsertBufferEndInsertJobIdFetch, false);
  vAddPrimitive("job_insertBuffer.InsertRows"_Stz, &vJob_InsertBufferInsertRows, false);
  vAddPrimitive("job_insertBuffer.table!"_Stz, &vJob_InsertBufferTableStore, false);
  
  vAddPrimitive("job_insertRow.table!"_Stz, &vJobInsertRowTableStore, false);
  vAddPrimitive("job_InsertRow.danglingBuffer"_Stz, &vJob_InsertRowDanglingBuffer, false);
  vAddPrimitive("job_InsertRow.SpawnInsertBuffer"_Stz,&vJob_InsertRowSpawnInsertBuffer, false);
  
  vAddPrimitive("job_endInsert.insertRowJob@"_Stz, &vJob_EndInsertInsertRowJobIdFetch, false);
  vAddPrimitive("job_endInsert.task@"_Stz, &vJob_EndInsertTaskFetch, false);
  vAddPrimitive("job_endInsert.task!"_Stz, &vJob_EndInsertTaskStore, false);
  
  vAddPrimitive("job.isPromptCommand"_Stz,  &vJobIsPromptCommand,  false);
  vAddPrimitive("job.jobClass"_Stz,         &vJobJobClass,         false);
  vAddPrimitive("job_loadBuffer.insertRows"_Stz,   &vJobLoadBufferInsertRows,       false);
  vAddPrimitive("job_loadBuffer.jobLoadTableId@"_Stz, &vJobLoadBufferJobLoadTableIdFetch,   false);
  vAddPrimitive("job_loadBuffer.jobLoadTable!"_Stz,   &vJobLoadBufferJobLoadTableStore,   false);
  vAddPrimitive("job_loadBuffer.partition@"_Stz,   &vJobLoadBufferPartitionFetch,   false);
  vAddPrimitive("job_loadBuffer.table!"_Stz,   &vJobLoadBufferTableStore,   false);
  vAddPrimitive("job_loadTable.addInvalidRows"_Stz,  &vJobLoadTableAddInvalidRows,   false);
  vAddPrimitive("job_loadTable.addValidRows"_Stz,  &vJobLoadTableAddValidRows,   false);
   vAddPrimitive("job_loadTable.filePath@"_Stz,  &vJobLoadTableFilePathFetch,   false);
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
  vAddPrimitive("job.postRecv"_Stz,         &vJobPostRecv,         false);
  vAddPrimitive("job.preSend"_Stz,          &vJobPreSend,          false);
  vAddPrimitive("job.promptTask@"_Stz,      &vJobPromptTaskFetch,  false);
  
  vAddPrimitive("job_receiver.connNo@"_Stz, &vJobReceiverConnNoFetch,   false);
  vAddPrimitive("job_receiver.connNo!"_Stz, &vJobReceiverConnNoStore,   false);
  vAddPrimitive("job_receiver.socket@"_Stz, &vJobReceiverSocketFetch,   false);
  vAddPrimitive("job_receiver.socket!"_Stz, &vJobReceiverSocketStore,   false);
  
  vAddPrimitive("job.recv"_Stz,             &vJobRecv,             false);
  vAddPrimitive("job.regionSize@"_Stz,      &vJobRegionSizeFetch,  false);
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
  vAddPrimitive("Job_subFetch.bufferQueue@"_Stz, &vJobSubFetchBufferQueueFetch, false);
  vAddPrimitive("Job_subFetch.phrase@"_Stz,  &vJobSubFetchPhraseFetch, false);
  vAddPrimitive("Job_subFetch.Planner@"_Stz,  &vJobSubFetchPlannerFetch, false);
  vAddPrimitive("Job_subFetch.slot@"_Stz,  &vJobSubFetchSlotFetch, false);
  vAddPrimitive("Job_subLoader.jobLoadTable@"_Stz,  &vJobSubLoaderJobLoadTableFetch, false);
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
  i8 iJobId = gpAttachMap->m_pOriginatingJobs->iNextJobId();
  pJob->m_iJobId = iJobId;
  //vTrace("adding job %p in originatingJobs %lld\n"_Stz,pJob, pJob->iJobId());
  gpAttachMap->m_pOriginatingJobs->vAddJob(pJob);
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
/* job_createIndex.isUnique@ -- fetch column type from create table job       */
/*----------------------------------------------------------------------------*/
void Task::vJobCreateIndexIsUniqueFetch(MP_Task mpTask)
{
  ENTRY("vJobCreateIndexIsUniqueFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_createIndex.isUnique@"_Stz);
  Job_CreateIndex* pJob_CreateIndex = _pJob_CreateIndex(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob_CreateIndex->m_bIsUnique);
  
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
  JobClass* pJobClass = JobClass::pFind(eClass);
  
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
/* job_InsertRow.danglingBuffer--ensure there's a dangling buffer in partition */
/*----------------------------------------------------------------------------*/
void Task::vJob_InsertRowDanglingBuffer(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowDanglingBuffer"_Stz);
  
  Object* pPtrInsertRow;
  if ( ! mpTask->m_pDataStack->bPop(pPtrInsertRow) )
    THROW_INVALID_DSTACK("job_InsertRow.danglingBuffer"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pPtrInsertRow);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iPartition) )
    THROW_INVALID_DSTACK("job_InsertRow.danglingBuffer"_Stz);
  
  if ( ! pJob_InsertRow->m_pWorkingBuffer[iPartition] )
    pJob_InsertRow->vNewWorkingBuffer(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.spawnInsertBuffer -- spawn Job_InsertBuffer                  */
/*----------------------------------------------------------------------------*/
void Task::vJob_InsertRowSpawnInsertBuffer(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowSpawnInsertBuffer"_Stz);
  
  Object* pPtrInsertRow;
  if ( ! mpTask->m_pDataStack->bPop(pPtrInsertRow) )
    THROW_INVALID_DSTACK("job_InsertRow.spawnInsertBuffer!"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pPtrInsertRow);
  
  i8 iJob_EndJobid;
  if ( ! mpTask->m_pDataStack->bPop(iJob_EndJobid) )
    THROW_INVALID_DSTACK("job_InsertRow.spawnInsertBuffer"_Stz);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("job_InsertRow.spawnInsertBuffer"_Stz);
  
  i8 iIsFinal;
  if ( ! mpTask->m_pDataStack->bPop(iIsFinal) )
    THROW_INVALID_DSTACK("job_InsertRow.spawnInsertBuffer"_Stz);
  
  WorkingBuffer* pWorkingBuffer = pJob_InsertRow->m_pWorkingBuffer[iPartition];
  pJob_InsertRow->m_pWorkingBuffer[iPartition] = nullptr;
  SEitf* pSEitf = pJob_InsertRow->m_pSEitf;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a load buffer job
  
  //! this needs to be optimised
  JobClass* pJobClass_InsertBuffer = JobClass::pFind(CN_JOB_INSERTBUFFER);
  
  Job_InsertBuffer* pJob_InsertBuffer =
  static_cast<Job_InsertBuffer*>(pJobClass_InsertBuffer->pPreJob(pSEitf));
  
  pJob_InsertBuffer->vSeed(SS_SEED_RUN);
  
  pJob_InsertBuffer->m_iEndInsertJobId = iJob_EndJobid;
  pJob_InsertBuffer->m_sObjectName = pJob_InsertRow->m_sObjectName;
  pJob_InsertBuffer->m_iPartitionId = iPartition;
  pJob_InsertBuffer->m_pWorkingBuffer = pWorkingBuffer;
  
  if (iIsFinal) {
    i8 nBuffersToBeInserted = pJob_InsertRow->m_nBuffersToBeInserted[iPartition];
    ASSERT(C_UNKNOWN != nBuffersToBeInserted);
    pJob_InsertBuffer->m_nBuffersToBeInserted = nBuffersToBeInserted;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn the load buffer job
  
  RapidsSE_bSpawnJob(pJob_InsertBuffer,SS_SEED_RUN,pSEitf);
  
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
      StrLib::vCopy(sExecute.aChar(),pJob->m_pJobClass->m_aSeedExe->aChar());
      //sExecute.vConcat("_exe"_Stz);
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
/* Job_fetchRows.lastBuffers! -- set no of expected last buffers              */
/*----------------------------------------------------------------------------*/
void Task::vJobFetchRowsLastBuffersStore(MP_Task mpTask)
{
  ENTRY("vJobFetchRowsLastBuffersStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.lastBuffers!"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  i8 nLastBuffers;
  if ( ! mpTask->m_pDataStack->bPopAsInt(nLastBuffers) )
    THROW_INVALID_DSTACK("Job_fetchRows.lastBuffers"_Stz);
  
  pJob_FetchRows->m_nLastBuffers = nLastBuffers;
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.partition@ -- fetch partition no from the job                */
/*----------------------------------------------------------------------------*/
void Task::vJobFetchRowsPartitionFetch(MP_Task mpTask)
{
  ENTRY("vJobFetchRowsPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.partition@"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);

  mpTask->m_pDataStack->vPushInt(pJob_FetchRows->m_iPartition);
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.planner! -- store planner in the job                         */
/*----------------------------------------------------------------------------*/
void Task::vJobFetchRowsPlannerStore(MP_Task mpTask)
{
  ENTRY("vJobFetchRowsPlannerStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.planner!"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  Object* pPlannerPtr;
  if ( ! mpTask->m_pDataStack->bPop(pPlannerPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.lastBuffers"_Stz);
  Planner* pPlanner = _pPlanner(pPlannerPtr);
  
  pJob_FetchRows->m_pPlanner = pPlanner;
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.spawnSub -- spawn sub fetch job                              */
/*----------------------------------------------------------------------------*/
void Task::vJobFetchRowsSpawnSub(MP_Task mpTask)
{
  ENTRY("vJobFetchRowsSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.spawnSub"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);

  Object* pPlannerPtr;
  if ( ! mpTask->m_pDataStack->bPop(pPlannerPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.spawnSub"_Stz);
  Planner* pPlanner = _pPlanner(pPlannerPtr);
  ASSERT(CN_PLANNER == pPlanner->eClass());
  
  i8 iPhrase;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iPhrase) )
    THROW_INVALID_DSTACK("Job_fetchRows.spawnSub"_Stz);
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iSlot) )
    THROW_INVALID_DSTACK("Job_fetchRows.spawnSub"_Stz);
  
  SEitf* pSEitf = pJob_FetchRows->m_pSEitf;
  i8 iFetchRowsJobId = pJob_FetchRows->m_iJobId;
  
  JobClass* pJobClass_SubFetch = JobClass::pFind(CN_JOB_SUBFETCH);
  
  Job_SubFetch* pJob_SubFetch =
  static_cast<Job_SubFetch*>(pJobClass_SubFetch->pPreJob(pSEitf));
  
  pJob_SubFetch->vSeed(SS_SEED_RUN);
  
  pJob_SubFetch->m_iFetchJobId = iFetchRowsJobId;
  pJob_SubFetch->m_sObjectName = pJob_FetchRows->m_sObjectName;
  pJob_SubFetch->m_pPlanner = pPlanner;
  pJob_SubFetch->m_iPhrase = iPhrase;
  pJob_SubFetch->m_iSlot = iSlot;
  pJob_SubFetch->m_pBufferQueue = pJob_FetchRows->m_pBufferQueue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RapidsSE_bSpawnJob(pJob_SubFetch,SS_SEED_RUN,pSEitf);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.table! -- store table in the job                             */
/*----------------------------------------------------------------------------*/
void Task::vJobFetchRowsTableStore(MP_Task mpTask)
{
  ENTRY("vJobFetchRowsTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.table!"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.table"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_FetchRows->m_pTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.where@ -- fetch the where string from the job                */
/*----------------------------------------------------------------------------*/
void Task::vJobFetchRowsWhereFetch(MP_Task mpTask)
{
  ENTRY("vJobFetchRowsWhereFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.where@!"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  mpTask->m_pDataStack->vPushS256(pJob_FetchRows->m_sWhere);
}

/*----------------------------------------------------------------------------*/
/* job::findOriginator -- find job from originator job set                     */
/*----------------------------------------------------------------------------*/
void Task::vJobFindOriginator(MP_Task mpTask)
{
  ENTRY("vJobFindOriginatort"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job::findOriginator"_Stz);
  
  Job__Cancelled* pFindJob = new Job__Cancelled;
  pFindJob->m_iJobId = iJobId;
  
  //vTrace("---------------\n"_Stz);
  gpAttachMap->m_pOriginatingJobs->vGrabLock();
  Job* pJob = gpAttachMap->m_pOriginatingJobs->pFindJob(pFindJob);
  gpAttachMap->m_pOriginatingJobs->vDropLock();

  
  //gpAttachMap->m_pOriginatingJobs->vContentDump();
  //vTrace("---------------\n"_Stz);
  
  delete pFindJob;
  ASSERT(pJob);
  mpTask->m_pDataStack->vPushObject(pJob);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findParticipant -- find job from participant job set                   */
/*----------------------------------------------------------------------------*/
void Task::vJobFindParticipant(MP_Task mpTask)
{
  ENTRY("vJobFindParticipant"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job::findPartcipant"_Stz);
  
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
/* job.hasWorkingBuffer -- return true if job has a working buffer            */
/*----------------------------------------------------------------------------*/
void Task::vJobHasWorkingBuffer(MP_Task mpTask)
{
  ENTRY("vJobHasWorkingBuffer"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.hasWorkingBuffer"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  
  i8 bHas = pJob->m_pWorkingBuffer ? true : false;
  if (pJob->m_pJobClass->m_eClass == CN_JOB_LOADBUFFER)
    int iDebug = 1;
  
  mpTask->m_pDataStack->vPushInt(bHas);
  
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
/* job_EndInsert.InsertRowJob@ -- fetch insertRow jobid from job              */
/*----------------------------------------------------------------------------*/
void Task::vJob_EndInsertInsertRowJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_EndInsertInsertRowJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_EndInsert.InsertRowJob@"_Stz);
  Job_EndInsert* pJob_EndInsert = _pJob_EndInsert(pJobPtr);
  
  i8 iInsertJobId = pJob_EndInsert->m_iInsertRowJobId;
  mpTask->m_pDataStack->vPushInt(iInsertJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_EndInsert.task@ -- fetch task from job_endInsert                       */
/*----------------------------------------------------------------------------*/
void Task::vJob_EndInsertTaskFetch(MP_Task mpTask)
{
  ENTRY("vJob_EndInsertTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_EndInsert.task@"_Stz);
  Job_EndInsert* pJob_EndInsert = _pJob_EndInsert(pJobPtr);
  
  Task* pTask = pJob_EndInsert->m_pTask;
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_EndInsert.task! -- store task from job_endInsert                       */
/*----------------------------------------------------------------------------*/
void Task::vJob_EndInsertTaskStore(MP_Task mpTask)
{
  ENTRY("vJob_EndInsertTaskStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_EndInsert.task!"_Stz);
  Job_EndInsert* pJob_EndInsert = _pJob_EndInsert(pJobPtr);
  
  Object* pTaskPtr;
  if ( ! mpTask->m_pDataStack->bPop(pTaskPtr) )
    THROW_INVALID_DSTACK("job_EndInsert.task!"_Stz);
  Task* pTask = _pTask(pTaskPtr);
  
  pJob_EndInsert->m_pTask = pTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.table! -- store table into job_insertRow                     */
/*----------------------------------------------------------------------------*/
void Task::vJobInsertRowTableStore(MP_Task mpTask)
{
  ENTRY("vJobInsertRowTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
  THROW_INVALID_DSTACK("job_InsertRow.table!"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
  THROW_INVALID_DSTACK("job_InsertRow.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_InsertRow->m_pTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertBuffer.insertRows -- insert rows from working buffer into table  */
/*----------------------------------------------------------------------------*/
void Task::vJob_InsertBufferInsertRows(MP_Task mpTask)
{
  ENTRY("vJobInsertBufferInsertRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertBuffer.insertRows!"_Stz);
  Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJobPtr);
  
  // convert partition to node slot
  i8 iPartition = pJob_InsertBuffer->m_iPartitionId;
  i8 iSlot = TablePtn::iPartitionToSlot(iPartition);
  
  // get table, partition and working buffer
  Table* pTable = pJob_InsertBuffer->m_pTable;
  TablePtn* pPartition = pTable->pPartition(iSlot);
  WorkingBuffer* pWorkingBuffer = pJob_InsertBuffer->m_pWorkingBuffer;
  
  i8 iTxId = 0;
  
  // insert the working buffer
  pTable->vInsertEntries(iTxId, iSlot, pWorkingBuffer);
  
  // we inserted one additional buffer
  pPartition->vIncBuffersInserted();
  
  // does this job have information about the number of buffers loadable
  // from a subloader for this partition?
  i8 nBuffersToBeInserted = pJob_InsertBuffer->m_nBuffersToBeInserted;
  if (C_UNKNOWN != nBuffersToBeInserted)
    pPartition->vBuffersToBeInserted(nBuffersToBeInserted);
  
  // have all buffers been loaded?
  bool bAllBuffersInserted = false;
  if ( pPartition->nBuffersToBeInserted() &&
      (pPartition->nBuffersToBeInserted() == pPartition->nBuffersInserted())) {
    vTrace("Buffers inserted for partition %lld\n"_Stz, iPartition);
    bAllBuffersInserted = true;
    
    // reset these in tblPtn for next insert into table
    pPartition->vBuffersToBeInserted(0);
    pPartition->vBuffersInserted(0);
  }
  
  mpTask->m_pDataStack->vPushInt(bAllBuffersInserted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertBuffer.partition@ -- fetch partition no. from job_insertBuffer   */
/*----------------------------------------------------------------------------*/
void Task::vJob_InsertBufferPartitionFetch(MP_Task mpTask)
{
  ENTRY("vJob_InsertBufferPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertBuffer.partition@"_Stz);
  Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJobPtr);
  
  i8 i_Partition = pJob_InsertBuffer->m_iPartitionId;
  mpTask->m_pDataStack->vPushInt(i_Partition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertBuffer.endInsertJob@ -- fetch endInsert jobid from job           */
/*----------------------------------------------------------------------------*/
void Task::vJob_InsertBufferEndInsertJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_InsertBufferEndInsertJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertBuffer.endInsertJob@"_Stz);
  Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJobPtr);
  
  i8 iInsertJobId = pJob_InsertBuffer->m_iEndInsertJobId;
  mpTask->m_pDataStack->vPushInt(iInsertJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertBuffer.table! -- store table into job_insertBuffer               */
/*----------------------------------------------------------------------------*/
void Task::vJob_InsertBufferTableStore(MP_Task mpTask)
{
  ENTRY("vJob_InsertBufferTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertBuffer.table!"_Stz);
  Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("job_InsertBuffer.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_InsertBuffer->m_pTable = pTable;
  
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
    THROW_INVALID_DSTACK("job.isPromptCommand"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_pPromptTask ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.jobClass -- get the JobClass of the job                                */
/*----------------------------------------------------------------------------*/
void Task::vJobJobClass(MP_Task mpTask)
{
  ENTRY("vJobJobClass"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.jobClass"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iJobClass_p = (i8)pJob->m_pJobClass;
  
  mpTask->m_pDataStack->vPushInt(iJobClass_p);
  
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

  // get table, partition and working buffer
  Table* pTable = pJob_LoadBuffer->pTable();
  TablePtn* pPartition = pTable->pPartition(iSlot);
  WorkingBuffer* pWorkingBuffer = pJob_LoadBuffer->m_pWorkingBuffer;
  
  i8 iTxId = 0;
  
  // insert the working buffer
  pTable->vInsertEntries(iTxId, iSlot, pWorkingBuffer);
  
  // we loaded one additional buffer from a subloader
  pPartition->vIncTotalBuffersLoaded();
  
  // does this job have information about the number of buffers loadable
  // from a subloader for this partition?
  i8 nBuffersFromSubLoader = pJob_LoadBuffer->nBuffersFromSubLoader();
  if (C_UNKNOWN != nBuffersFromSubLoader)
    pPartition->vAddTotalBuffersLoadable(nBuffersFromSubLoader);
  
  // have all buffers been loaded?
  bool bAllBuffersLoaded = pPartition->bAllBuffersLoaded();
  //if (bAllBuffersLoaded)
  //  vTrace("Buffers loaded\n"_Stz);

  mpTask->m_pDataStack->vPushInt(bAllBuffersLoaded);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.jobLoadTableId@ -- fetch load table job id                  */
/*----------------------------------------------------------------------------*/
void Task::vJobLoadBufferJobLoadTableIdFetch(MP_Task mpTask)
{
  ENTRY("vJobLoadBufferLoadJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.loadJobId@"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  i8 iJobId = pJob_LoadBuffer->iJobLoadTableId();
  mpTask->m_pDataStack->vPushInt(iJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.jobLoadTable! -- store job_loadTable                        */
/*----------------------------------------------------------------------------*/
void Task::vJobLoadBufferJobLoadTableStore(MP_Task mpTask)
{
  ENTRY("vJobLoadBufferJobLoadTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.jobLoadTable!"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.jobLoadTable!"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);

  pJob_LoadBuffer->vJobLoadTable(pJob_LoadTable);
  
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
/* job_loadTable.addInvalidRows -- add invalid rows to load table's invalid   */
/*----------------------------------------------------------------------------*/
void Task::vJobLoadTableAddInvalidRows(MP_Task mpTask)
{
  ENTRY("vJobLoadTableAddInvalidRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_loadTable.addInvalidRows"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);
  
  i8 iInvalidRows;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iInvalidRows) )
    THROW_INVALID_DSTACK("job_loadTable.addInvalidRows"_Stz);
  
  pJob_LoadTable->m_ziInvalidRows.iAdd(iInvalidRows);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_loadTable.addValidRows -- add invalid rows to load table's valid rows  */
/*----------------------------------------------------------------------------*/
void Task::vJobLoadTableAddValidRows(MP_Task mpTask)
{
  ENTRY("vJobLoadTableAddValidRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_loadTable.addValidRows"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);
  
  i8 iValidRows;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iValidRows) )
    THROW_INVALID_DSTACK("job_loadTable.addValidRows"_Stz);
  
  pJob_LoadTable->m_ziValidRows.iAdd(iValidRows);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadTable.filePath@ -- fetch filePath from job_loadTable              */
/*----------------------------------------------------------------------------*/
void Task::vJobLoadTableFilePathFetch(MP_Task mpTask)
{
  ENTRY("vJobLoadTableFilePathFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadTable.filePath@"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);
  
  s256 sFilePath = pJob_LoadTable->m_sFileName;
  mpTask->m_pDataStack->vPushS256(sFilePath);
  
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
    THROW_INVALID_DSTACK("job.metadata!"_Stz);
  
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
  
  JobClass* pJobClass__Begun = JobClass::pFind(CN_JOB__BEGUN);
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
  
  JobClass* pJobClass__Cancel = JobClass::pFind(CN_JOB__CANCEL);
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
  
  JobClass* pJobClass__Cancelled = JobClass::pFind(CN_JOB__CANCELLED);
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
  
  JobClass* pJobClass__End = JobClass::pFind(CN_JOB__END);
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
  
  JobClass* pJobClass__Ended = JobClass::pFind(CN_JOB__ENDED);
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
  
  JobClass* pJobClass__Execute = JobClass::pFind(CN_JOB__EXECUTE);
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
  
  JobClass* pJobClass__Executed = JobClass::pFind(CN_JOB__EXECUTED);
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
/* job.postRecv -- run the post-receive function of the job                   */
/*----------------------------------------------------------------------------*/
void Task::vJobPostRecv(MP_Task mpTask)
{
  ENTRY("vJobPostRecv"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.postRecv"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  // find the job class on this node
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = JobClass::pFind(eClass);
  
  pJob->m_pJobClass = pJobClass;

  if (pJobClass->m_fRecv)
    (*pJobClass->m_fRecv)(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.preSend -- run the pre-send function of the job                        */
/*----------------------------------------------------------------------------*/
void Task::vJobPreSend(MP_Task mpTask)
{
  ENTRY("vJobPreSend"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.preSend"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  JobClass* pJobClass = pJob->m_pJobClass;
  
  if (pJobClass->m_fSend)
    (*pJobClass->m_fSend)(pJob);
  
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
  JobClass* pJobClass = JobClass::pFind(eClass);
  
  // call the job post-receive function
  pJobClass->vRecvJob(pJob);  // come back to this, remove seitf as parameter to delete
  
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
  
  //vTrace("removing job %p in originatingJobs %lld\n"_Stz,pJob, pJob->iJobId());
  
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
  
  mpTask->m_pDataStack->vPushS4K(pJob->m_xNode[iNode].m_sResponse);
  
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
  
  s4K sResponse;
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
  JobClass* pJobClass = JobClass::pFind(eClass);
  
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
  JobClass* pJobClass = JobClass::pFind(eClass);
  
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
  
  RapidsSE_bSpawnJob(pJob,(eSS)i_Seed,nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subFetch.bufferQueue@ -- fetch the job buffer queue                    */
/*----------------------------------------------------------------------------*/
void Task::vJobSubFetchBufferQueueFetch(MP_Task mpTask)
{
  ENTRY("vJobSubFetchBufferQueueFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subFetch.bufferQueue@"_Stz);
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob->m_pBufferQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subFetch.phrase@ -- fetch the job phrase                               */
/*----------------------------------------------------------------------------*/
void Task::vJobSubFetchPhraseFetch(MP_Task mpTask)
{
  ENTRY("vJobSubFetchPhraseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subFetch.phrase@"_Stz);
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iPhrase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subFetch.planner@ -- fetch the job planner                             */
/*----------------------------------------------------------------------------*/
void Task::vJobSubFetchPlannerFetch(MP_Task mpTask)
{
  ENTRY("vJobSubFetchPlannerFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subFetch.planner@"_Stz);
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob->m_pPlanner);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subFetch.slot@ -- fetch the job slot                                   */
/*----------------------------------------------------------------------------*/
void Task::vJobSubFetchSlotFetch(MP_Task mpTask)
{
  ENTRY("vJobSubFetchSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subFetch.slot@"_Stz);
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.jobLoadTable@ -- fetch the job load table                    */
/*----------------------------------------------------------------------------*/
void Task::vJobSubLoaderJobLoadTableFetch(MP_Task mpTask)
{
  ENTRY("vJobSubLoaderJobLoadTableFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subLoader.jobLoadTable@"_Stz);
  Job_SubLoader* pJob = _pJob_SubLoader(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob->m_pJob_LoadTable);
  
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
  
  JobClass* pJobClass_SubTruncate = JobClass::pFind(CN_JOB_SUBTRUNCATE);
  
  Job_SubTruncate* pJob_SubTruncate =
    static_cast<Job_SubTruncate*>(pJobClass_SubTruncate->pPreJob(nullptr));
  
  pJob_SubTruncate->vSeed(SS_SEED_RUN);
  
  pJob_SubTruncate->m_iTruncateJobId = iTruncateJobId;
  pJob_SubTruncate->m_sObjectName = pJob_TruncateTable->m_sObjectName;
  pJob_SubTruncate->m_iSlot = iSlot;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  RapidsSE_bSpawnJob(pJob_SubTruncate,SS_SEED_RUN,nullptr);

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
