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
void Task::vAddSteps_Job(void)
{
  vAddStep("job.acksRemaining!"_Stz,   &vJobAcksRemaining,    false);
  vAddStep("job.addOriginator"_Stz,    &vJobAddOriginator,    false);
  vAddStep("job.addParticipant"_Stz,   &vJobAddParticipant,   false);
  vAddStep("job.ancilliary@"_Stz,      &vJobAncilliaryFetch,  false);
  vAddStep("job.class"_Stz,            &vJobClass,            false);
  vAddStep("job.columnCount@"_Stz,     &vJobColumnCountFetch, false);
  vAddStep("job.columnIsPartKey"_Stz,  &vJobColumnIsPartKey,  false);
  vAddStep("job.columnName@"_Stz,      &vJobColumnNameFetch,  false);
  vAddStep("job.columnType@"_Stz,      &vJobColumnTypeFetch,  false);
  vAddStep("job.decAcksRemaining"_Stz, &vJobDecAcksRemaining, false);
  vAddStep("job.delete"_Stz,           &vJobDelete,           false);
  vAddStep("job.dicPath"_Stz,          &vJobDicPath,          false);
  
  vAddStep("job.error!"_Stz,           &vJobErrorStore,       false);
  vAddStep("job.error@"_Stz,           &vJobErrorFetch,       false);
  vAddStep("job.execute@"_Stz,         &vJobExecuteFetch,     false);
  
  vAddStep("job::findOriginator"_Stz,   &vJobFindOriginator,   false);
  vAddStep("job::findParticipant"_Stz,  &vJobFindParticipant,  false);
  vAddStep("job.hasError"_Stz,         &vJobHasError,         false);
  vAddStep("job.hasWorkingBuffer"_Stz, &vJobHasWorkingBuffer, false);
  vAddStep("job.id@"_Stz,              &vJobIdFetch,          false);
  vAddStep("job.indexName@"_Stz,       &vJobIndexNameFetch,   false);
  
  vAddStep("job.isPromptCommand"_Stz,  &vJobIsPromptCommand,  false);
  vAddStep("job.jobClass"_Stz,         &vJobJobClass,         false);
  vAddStep("job.metadata!"_Stz,        &vJobMetadataStore,    false);
  vAddStep("job.newBegun"_Stz,         &vJobNewBegun,         false);
  vAddStep("job.newCancel"_Stz,        &vJobNewCancel,        false);
  vAddStep("job.newCancelled"_Stz,     &vJobNewCancelled,     false);
  vAddStep("job.newEnd"_Stz,           &vJobNewEnd,           false);
  vAddStep("job.newEnded"_Stz,         &vJobNewEnded,         false);
  vAddStep("job.newExecute"_Stz,       &vJobNewExecute,       false);
  vAddStep("job.newExecuted"_Stz,      &vJobNewExecuted,      false);
  vAddStep("job.objectName@"_Stz,      &vJobObjectNameFetch,  false);
  vAddStep("job.originatingTask@"_Stz, &vJobOriginatingTaskFetch, false);
  vAddStep("job.originatingTask!"_Stz, &vJobOriginatingTaskStore, false);
  vAddStep("job::originator@"_Stz,     &vJobOriginatorFetch,  false);
  vAddStep("job.participant@"_Stz,     &vJobParticipantFetch, false);
  vAddStep("job.participant!"_Stz,     &vJobParticipantStore, false);
  vAddStep("job.partitions@"_Stz,      &vJobPartitionsFetch,  false);
  vAddStep("job.postRecv"_Stz,         &vJobPostRecv,         false);
  vAddStep("job.preSend"_Stz,          &vJobPreSend,          false);
  vAddStep("job.promptTask@"_Stz,      &vJobPromptTaskFetch,  false);
  
  vAddStep("job.recv"_Stz,             &vJobRecv,             false);
  vAddStep("job.regionSize@"_Stz,      &vJobRegionSizeFetch,  false);
  vAddStep("job.removeOriginator"_Stz, &vJobRemoveOriginator, false);
  vAddStep("job.removeParticipant"_Stz,&vJobRemoveParticipant,false);
  vAddStep("job.response@"_Stz,        &vJobResponseFetch,    false);
  vAddStep("job.response!"_Stz,        &vJobResponseStore,    false);
  vAddStep("job.rowsProcessed@"_Stz,   &vJobRowsProcessedFetch, false);
  vAddStep("job.rowsProcessed!"_Stz,   &vJobRowsProcessedStore, false);
  vAddStep("job.send"_Stz,             &vJobRecv,             false);
  vAddStep("job.show"_Stz,             &vJobShow,             false);
  vAddStep("job.signalConnector"_Stz,  &vJobSignalConnector,  false);
  vAddStep("job.spawn"_Stz,            &vJobSpawn,            false);
  vAddStep("job::this"_Stz,            &vJobThis,             false);
  vAddStep("job.trace!"_Stz,           &vJobTraceStore,       false);
  vAddStep("job.trace@"_Stz,           &vJobTraceFetch,       false);
  vAddStep("job.workingBuffer@"_Stz,   &vJobWorkingBufferFetch, false);
  vAddStep("job.workingBuffer!"_Stz,   &vJobWorkingBufferStore, false);
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
      StrLib::vCopy(sExecute.aChar(),pJob->m_pJobClass->m_aSeedExe->aChar());
      //sExecute.vConcat("_exe"_Stz);
      break;
  }
  
  mpTask->m_pDataStack->vPushS32(sExecute);
  
  EXIT();
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
