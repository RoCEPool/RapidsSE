/*============================================================================*/
/* Steps_Job.cpp - job step group                                             */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Job(void)
{
  vAddStep("job::findOriginator"_Stz,  &Step::vJobFindOriginator);
  vAddStep("job::findParticipant"_Stz, &Step::vJobFindParticipant);
  
  vAddStep("job.acksRemaining!"_Stz,   &Step::vJobAcksRemaining);
  vAddStep("job.addOriginator"_Stz,    &Step::vJobAddOriginator);
  vAddStep("job.addParticipant"_Stz,   &Step::vJobAddParticipant);
  
  vAddStep("job.class"_Stz,            &Step::vJobClass);
  vAddStep("job.columnCount@"_Stz,     &Step::vJobColumnCountFetch);
  vAddStep("job.columnIsPartKey"_Stz,  &Step::vJobColumnIsPartKey);
  vAddStep("job.columnName@"_Stz,      &Step::vJobColumnNameFetch);
  vAddStep("job.columnType@"_Stz,      &Step::vJobColumnTypeFetch);
  
  vAddStep("job.decAcksRemaining"_Stz, &Step::vJobDecAcksRemaining);
  vAddStep("job.delete"_Stz,           &Step::vJobDelete);
  vAddStep("job.dicPath"_Stz,          &Step::vJobDicPath);
  
  vAddStep("job.entryBuffer@"_Stz,     &Step::vJobEntryBufferFetch);
  vAddStep("job.entryBuffer!"_Stz,     &Step::vJobEntryBufferStore);
  vAddStep("job.error!"_Stz,           &Step::vJobErrorStore);
  vAddStep("job.error@"_Stz,           &Step::vJobErrorFetch);
  vAddStep("job.execute@"_Stz,         &Step::vJobExecuteFetch);
  vAddStep("job.executingTask@"_Stz,   &Step::vJobExecutingTaskFetch);
  
  vAddStep("job.hasError"_Stz,         &Step::vJobHasError);
  vAddStep("job.hasEntryBuffer"_Stz,   &Step::vJobHasEntryBuffer);
  
  vAddStep("job.id@"_Stz,              &Step::vJobIdFetch);
  vAddStep("job.indexName@"_Stz,       &Step::vJobIndexNameFetch);
  vAddStep("job.IsExecuting"_Stz,      &Step::vJobIsExecuting);
  
  vAddStep("job.jobClass"_Stz,         &Step::vJobJobClass);
  
  vAddStep("job.metadata!"_Stz,        &Step::vJobMetadataStore);
  
  vAddStep("job.newBegun"_Stz,         &Step::vJobNewBegun);
  vAddStep("job.newCancel"_Stz,        &Step::vJobNewCancel);
  vAddStep("job.newCancelled"_Stz,     &Step::vJobNewCancelled);
  vAddStep("job.newEnd"_Stz,           &Step::vJobNewEnd);
  vAddStep("job.newEnded"_Stz,         &Step::vJobNewEnded);
  vAddStep("job.newExecute"_Stz,       &Step::vJobNewExecute);
  vAddStep("job.newExecuted"_Stz,      &Step::vJobNewExecuted);

  vAddStep("job.objectName@"_Stz,      &Step::vJobObjectNameFetch);
  vAddStep("job.originatingTask@"_Stz, &Step::vJobOriginatingTaskFetch);
  vAddStep("job.originatingTask!"_Stz, &Step::vJobOriginatingTaskStore);
  vAddStep("job::originator@"_Stz,     &Step::vJobOriginatorFetch);

  vAddStep("job.participant@"_Stz,     &Step::vJobParticipantFetch);
  vAddStep("job.participant!"_Stz,     &Step::vJobParticipantStore);
  vAddStep("job.partitions@"_Stz,      &Step::vJobPartitionsFetch);
  vAddStep("job.phase@"_Stz,           &Step::vJobPhaseFetch);
  vAddStep("job.phase!"_Stz,           &Step::vJobPhaseStore);
  vAddStep("job.postRecv"_Stz,         &Step::vJobPostRecv);
  vAddStep("job.preSend"_Stz,          &Step::vJobPreSend);

  vAddStep("job.recv"_Stz,             &Step::vJobRecv);
  vAddStep("job.regionSize@"_Stz,      &Step::vJobRegionSizeFetch);
  vAddStep("job.removeOriginator"_Stz, &Step::vJobRemoveOriginator);
  vAddStep("job.removeParticipant"_Stz,&Step::vJobRemoveParticipant);
  vAddStep("job.response@"_Stz,        &Step::vJobResponseFetch);
  vAddStep("job.response!"_Stz,        &Step::vJobResponseStore);
  vAddStep("job.rowsInvalid@"_Stz,     &Step::vJobRowsInvalidFetch);
  vAddStep("job.rowsInvalid!"_Stz,     &Step::vJobRowsInvalidStore);
  vAddStep("job.rowsProcessed@"_Stz,   &Step::vJobRowsProcessedFetch);
  vAddStep("job.rowsProcessed!"_Stz,   &Step::vJobRowsProcessedStore);
  vAddStep("job.rowsSkipped@"_Stz,     &Step::vJobRowsSkippedFetch);
  vAddStep("job.rowsSkipped!"_Stz,     &Step::vJobRowsSkippedStore);

  vAddStep("job.send"_Stz,             &Step::vJobSend);
  vAddStep("job.show"_Stz,             &Step::vJobShow);
  vAddStep("job.signalConnector"_Stz,  &Step::vJobSignalConnector);
  vAddStep("job.spawn"_Stz,            &Step::vJobSpawn);
  
  vAddStep("job::this"_Stz,            &Step::vJobThis);
  vAddStep("job.trace!"_Stz,           &Step::vJobTraceStore);
  vAddStep("job.trace@"_Stz,           &Step::vJobTraceFetch);

  vAddStep("job::waitParticipant"_Stz, &Step::vJobWaitParticipant);
}


/*----------------------------------------------------------------------------*/
/* job.acksRemaining! -- set acks remaining for this job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobAcksRemaining(MP_Task mpTask)
{
  ENTRY("vJobAcksRemaining"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.acksRemaining!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 nAcksRemaining;
  if ( ! mpTask->m_pDataStack->bPop(nAcksRemaining) )
    THROW_INVALID_DSTACK("job.acksRemaining!"_Stz);
  
  pJob->m_oAcksRemaining.vSetRemaining(nAcksRemaining);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addOriginator -- add job to originator job set                         */
/*----------------------------------------------------------------------------*/
void Step::vJobAddOriginator(MP_Task mpTask)
{
  ENTRY("vJobAddOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.addOriginator"_Stz);
  Job* pJob = _pJob(pJobPtr);
    
  // add the job to the originating jobs
  gpAttachMap->m_pOriginatingJobs->vGrabLock();
  gpAttachMap->m_pOriginatingJobs->vAddJob(pJob);
  gpAttachMap->m_pOriginatingJobs->vDropLock();

  if (pJob->m_bMark) {
    ActivityMap::vMarkJob(pJob,"Added to Originator List: %s %p"_Stz,
                       aCN[pJob->eClass()],
                       pJob);
    gpAttachMap->m_pOriginatingJobs->vMarkContents(pJob->m_iJobId);
  }
  
  ActionSequence::vAdd(mpTask->m_pJob->m_i_ActionRoot,
                       mpTask->m_pJob->m_iJobId,
                       mpTask->m_pJob->eClass(),
                       AN_ADDED_ORIGINATOR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.addParticipant -- add job to participant job set                       */
/*----------------------------------------------------------------------------*/
void Step::vJobAddParticipant(MP_Task mpTask)
{
  ENTRY("vJobAddParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.addParticipant"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  gpAttachMap->m_pParticipatingJobs->vAddJob(pJob);
  gpAttachMap->m_pParticipatingJobs->vDropLock();
  
  if (pJob->m_bMark) {
    ActivityMap::vMarkJob(pJob,"Added to ParticipantList: %s %p "_Stz,
                       aCN[pJob->eClass()],
                       pJob);
    gpAttachMap->m_pParticipatingJobs->vMarkContents(pJob->m_iJobId);
  }
  
  ActionSequence::vAdd(mpTask->m_pJob->m_i_ActionRoot,
                       mpTask->m_pJob->m_iJobId,
                       mpTask->m_pJob->eClass(),
                       AN_ADDED_PARTICIPANT);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.columnCount@ -- fetch the no of columns of the create table job        */
/*----------------------------------------------------------------------------*/
void Step::vJobColumnCountFetch(MP_Task mpTask)
{
  ENTRY("vJobColumnCountFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.columnCount@"_Stz);
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob_CreateTable->m_iColumns);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.columnIsPartKey -- return true if column is a partition by key        */
/*----------------------------------------------------------------------------*/
void Step::vJobColumnIsPartKey(MP_Task mpTask)
{
  ENTRY("vJobColumnIsPartKey"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.columnIsPartKey"_Stz);
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJobPtr);
  
  i8 iColumnNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iColumnNo) )
    THROW_INVALID_DSTACK("job.columnIsPartKey@"_Stz);
  
  mpTask->m_pDataStack->vPushI8(pJob_CreateTable->m_xColumns[iColumnNo].m_bIsPartitionKey);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.columnName@ -- fetch the name of the column of the create table job    */
/*----------------------------------------------------------------------------*/
void Step::vJobColumnNameFetch(MP_Task mpTask)
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
void Step::vJobColumnTypeFetch(MP_Task mpTask)
{
  ENTRY("vJobColumnTypeFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.columnType@"_Stz);
  Job_CreateTable* pJob_CreateTable = _pJob_CreateTable(pJobPtr);
  
  i8 iColumnNo;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iColumnNo) )
    THROW_INVALID_DSTACK("job.columnType@"_Stz);
  
  mpTask->m_pDataStack->vPushI8(pJob_CreateTable->m_xColumns[iColumnNo].m_eColumnType);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.class -- get the class of the job                                      */
/*----------------------------------------------------------------------------*/
void Step::vJobClass(MP_Task mpTask)
{
  ENTRY("vJobClass"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.class"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  eCN eClass = pJob->eClass();
  
  mpTask->m_pDataStack->vPushI8(eClass);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.delete -- delete the job                                               */
/*----------------------------------------------------------------------------*/
void Step::vJobDelete(MP_Task mpTask)
{
  ENTRY("vJobDelete"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.delete"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  //vTrace("Deleting job %p\n"_Stz, pJob);
  
  // find the job in the catalog
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = JobClass::pFind(eClass);
  
  // call the job delete function
  pJobClass->vDeleteJob(nullptr, pJob);  // come back to this, remove session as parameter to delete
  
  EXIT();
}
  

/*----------------------------------------------------------------------------*/
/* job.decAcksRemaining -- decrement acks remaining for this job              */
/*----------------------------------------------------------------------------*/
void Step::vJobDecAcksRemaining(MP_Task mpTask)
{
  ENTRY("vJobDecAcksRemaining"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.decAcksRemaining"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  bool bAmDecider = pJob->m_oAcksRemaining.bDecAndDecide();
  
  mpTask->m_pDataStack->vPushI8(bAmDecider);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.dicPath -- get job dic path                                            */
/*----------------------------------------------------------------------------*/
void Step::vJobDicPath(MP_Task mpTask)
{
  ENTRY("vJobDicPath"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.dicPath"_Stz);
  Job* pJob = _pJob(pJobPtr);

  mpTask->m_pDataStack->vPushPointer(&pJob->m_oDicPath);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error@ -- fetch the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorFetch(MP_Task mpTask)
{
  ENTRY("vJobErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.error@"_Stz);
  Job* pJob = _pJob(pJobPtr);

  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.error@"_Stz);
  
  mpTask->m_pDataStack->vPushI8(pJob->m_xNode[iNode].m_eErrorCode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.error! -- store the job error for the node                             */
/*----------------------------------------------------------------------------*/
void Step::vJobErrorStore(MP_Task mpTask)
{
  ENTRY("vJobErrorStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.error!"_Stz);
  Job* pJob = _pJob(pJobPtr);

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
void Step::vJobExecuteFetch(MP_Task mpTask)
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
/* job.exectingTask@ -- fetch the job executing task                          */
/*----------------------------------------------------------------------------*/
void Step::vJobExecutingTaskFetch(MP_Task mpTask)
{
  ENTRY("vJobExecutingTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.exectingTask@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushPointer(pJob->m_pExecutingTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findOriginator -- find job from originator job set                    */
/*----------------------------------------------------------------------------*/
void Step::vJobFindOriginator(MP_Task mpTask)
{
  ENTRY("vJobFindOriginator"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job::findOriginator"_Stz);
  
  Job* pJobFound = gpAttachMap->m_pOriginatingJobs->pFindJob(iJobId);
  RASSERT(pJobFound);
  
  mpTask->m_pDataStack->vPushPointer(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::findParticipant -- find job from participant job set                  */
/*----------------------------------------------------------------------------*/
void Step::vJobFindParticipant(MP_Task mpTask)
{
  ENTRY("vJobFindParticipant"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job::findPartcipant"_Stz);
  
  Object* pJobObject;
  if ( ! mpTask->m_pDataStack->bPop(pJobObject) )
    THROW_INVALID_DSTACK("job::findPartcipant"_Stz);
  
  Job* pCallingJob = _pJob(pJobObject);
  
  
  Job* pJobFound = gpAttachMap->m_pParticipatingJobs->pFindJob(iJobId);
  RASSERT(pJobFound);
  
  mpTask->m_pDataStack->vPushPointer(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.hasError -- return true if job has error                               */
/*----------------------------------------------------------------------------*/
void Step::vJobHasError(MP_Task mpTask)
{
  ENTRY("vJobErrorFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.hasError"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob->m_bHasError);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.hasEntryBuffer -- return true if job has a entry buffer                */
/*----------------------------------------------------------------------------*/
void Step::vJobHasEntryBuffer(MP_Task mpTask)
{
  ENTRY("vJobHasEntryBuffer"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.hasEntryBuffer"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  
  i8 bHas = pJob->m_pCurrentBuffer ? true : false;
  if (pJob->m_pJobClass->m_eClass == CN_JOB_WRITEBUFFER)
    int iDebug = 1;
  
  mpTask->m_pDataStack->vPushI8(bHas);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.id@ -- fetch the job id of the job                                     */
/*----------------------------------------------------------------------------*/
void Step::vJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.id@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob->m_iJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.indexName@ -- fetch the index name of the job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobIndexNameFetch(MP_Task mpTask)
{
  ENTRY("vJobIndexNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.indexName@"_Stz);
  Job* pJob = _pJob(pJobPtr);

  mpTask->m_pDataStack->vPushS32(pJob->m_sIndexName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.isExecuting -- is the job executing (SEED_EXE)                         */
/*----------------------------------------------------------------------------*/
void Step::vJobIsExecuting(MP_Task mpTask)
{
  ENTRY("vJobIsExecuting"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.isExecuting"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iIsExecuting = pJob->m_eSeed == SS_SEED_EXE ? 1 : 0;
  mpTask->m_pDataStack->vPushI8(iIsExecuting);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.jobClass -- get the JobClass of the job                                */
/*----------------------------------------------------------------------------*/
void Step::vJobJobClass(MP_Task mpTask)
{
  ENTRY("vJobJobClass"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.jobClass"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iJobClass_p = (i8)pJob->m_pJobClass;
  
  mpTask->m_pDataStack->vPushI8(iJobClass_p);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.metadata! -- store meta data string in the job                         */
/*----------------------------------------------------------------------------*/
void Step::vJobMetadataStore(MP_Task mpTask)
{
  ENTRY("vJobMetadataStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.metadata"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  s4K sMetadata;
  if ( ! mpTask->m_pDataStack->bPop(sMetadata) )
    THROW_INVALID_DSTACK("job.metadata!"_Stz);
  
  pJob->m_sMetadata = sMetadata;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newBegun -- new a job__begun for this job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobNewBegun(MP_Task mpTask)
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
  
  mpTask->m_pDataStack->vPushPointer(pJobBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newCancel -- new a job__cancel for this job                            */
/*----------------------------------------------------------------------------*/
void Step::vJobNewCancel(MP_Task mpTask)
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
  
  mpTask->m_pDataStack->vPushPointer(pJobCancel);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newCancelled -- new a job__cancelled for this job                      */
/*----------------------------------------------------------------------------*/
void Step::vJobNewCancelled(MP_Task mpTask)
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
  
  mpTask->m_pDataStack->vPushPointer(pJobCancelled);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newEnd - new a job__end for this job                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobNewEnd(MP_Task mpTask)
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
  
  mpTask->m_pDataStack->vPushPointer(pJobEnd);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newEnded -- new a job__ended for this job                              */
/*----------------------------------------------------------------------------*/
void Step::vJobNewEnded(MP_Task mpTask)
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
  
  mpTask->m_pDataStack->vPushPointer(pJobEnded);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newExecute -- new a job__execute for this job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobNewExecute(MP_Task mpTask)
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
  
  mpTask->m_pDataStack->vPushPointer(pJobExecute);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.newExecuted -- new a job__executed for this job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobNewExecuted(MP_Task mpTask)
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
  
  mpTask->m_pDataStack->vPushPointer(pJobExecuted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.objectName@ -- fetch the object name of the job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobObjectNameFetch(MP_Task mpTask)
{
  ENTRY("vJobObjectNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.objectNamed@"_Stz);
  Job* pJob = _pJob(pJobPtr);

  mpTask->m_pDataStack->vPushS32(pJob->m_sObjectName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingTask@ -- fetch the originating task from the job            */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingTaskFetch(MP_Task mpTask)
{
  ENTRY("vJobOriginatingTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.originatingTask@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushPointer(pJob->m_pOriginatingTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.originatingTask! -- store the originating task in the job              */
/*----------------------------------------------------------------------------*/
void Step::vJobOriginatingTaskStore(MP_Task mpTask)
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
void Step::vJobOriginatorFetch(MP_Task mpTask)
{
  ENTRY("vJobOriginatorFetch"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job::originator@@"_Stz);
  
  mpTask->m_pDataStack->vPushI8(Job::iOriginatorNode(iJobId));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.participant@ -- fetch the participant node from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantFetch(MP_Task mpTask)
{
  ENTRY("vJobRemoveParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.partcipant@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob->m_iParticipantNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.partcipant! -- store the participant node in the job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobParticipantStore(MP_Task mpTask)
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
void Step::vJobPartitionsFetch(MP_Task mpTask)
{
  ENTRY("vJobParticipantResponseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.partitions@"_Stz);
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob_Sanity->m_iPartitions);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.phase@ -- fetch the job phase                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseFetch(MP_Task mpTask)
{
  ENTRY("vJobPhaseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.phase@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob->m_eJobPhase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.phase! -- store the job phase                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobPhaseStore(MP_Task mpTask)
{
  ENTRY("vJobPhaseStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.phase!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iJobPhase;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobPhase) )
    THROW_INVALID_DSTACK("job.phase!"_Stz);
  
  pJob->m_eJobPhase = (eJP)iJobPhase;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.postRecv -- run the post-receive function of the job                   */
/*----------------------------------------------------------------------------*/
void Step::vJobPostRecv(MP_Task mpTask)
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

  ASSERT(pJobClass->m_fpRecv == vJob_recv);
  
  if (pJobClass->m_fpRecv)
    (*pJobClass->m_fpRecv)(pJob);
  
  if (pJob->m_bMark) {
    ActivityMap::vMarkJob(pJob,
                          "PostRecv: %s %p"_Stz,
                          aCN[pJob->eClass()],
                          pJob);
    pJob->vMarkJob();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.preSend -- run the pre-send function of the job                        */
/*----------------------------------------------------------------------------*/
void Step::vJobPreSend(MP_Task mpTask)
{
  ENTRY("vJobPreSend"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.preSend"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  JobClass* pJobClass = pJob->m_pJobClass;
  
  if (pJobClass->m_fpSend)
    (*pJobClass->m_fpSend)(pJob);
  
  if (pJob->m_bMark)
    ActivityMap::vMarkJob(pJob,
                          "PreSend: %s %p"_Stz,
                          aCN[pJob->eClass()],
                          pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.recv -- post-receive function                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobRecv(MP_Task mpTask)
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
  pJobClass->vRecvJob(pJob);  // come back to this, remove session as parameter to delete
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removeOriginator -- remove job from originator job set                 */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveOriginator(MP_Task mpTask)
{
  ENTRY("vJobRemoveOriginator"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.id@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  
  gpAttachMap->m_pOriginatingJobs->vGrabLock();
  bool bRemove = gpAttachMap->m_pOriginatingJobs->bRemoveJob(pJob);
  gpAttachMap->m_pOriginatingJobs->vDropLock();
  
  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from originator\n"_Stz, aCN[pJob->eClass()],pJob->m_iJobId);
    vTrace("------------------------------------\n"_Stz);
    
    gpAttachMap->m_pOriginatingJobs->vGrabLock();
    gpAttachMap->m_pOriginatingJobs->vContentDump();
    bRemove = gpAttachMap->m_pOriginatingJobs->bRemoveJob(pJob);
    gpAttachMap->m_pOriginatingJobs->vDropLock();
  }
  
  if (pJob->m_bMark) {
    ActivityMap::vMarkJob(pJob,
                       "Removed from Originator List: %s %p"_Stz,
                       aCN[pJob->eClass()],
                       pJob);
    gpAttachMap->m_pOriginatingJobs->vMarkContents(pJob->m_iJobId);
  }
  
  ActionSequence::vAdd(mpTask->m_pJob->m_i_ActionRoot,
                       mpTask->m_pJob->m_iJobId,
                       mpTask->m_pJob->eClass(),
                       AN_REMOVED_ORIGINATOR);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.removeParticipant -- remove job from participant job set               */
/*----------------------------------------------------------------------------*/
void Step::vJobRemoveParticipant(MP_Task mpTask)
{
  ENTRY("vJobRemoveParticipant"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.id@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  bool bRemove = gpAttachMap->m_pParticipatingJobs->bRemoveJob(pJob);
  gpAttachMap->m_pParticipatingJobs->vDropLock();
 
  if (! bRemove) {
    vTrace("------------------------------------\n"_Stz);
    vTrace("%s Failed to Remove JobId %lld from participant\n"_Stz, aCN[pJob->eClass()],pJob->m_iJobId);
    vTrace("------------------------------------\n"_Stz);
    
    gpAttachMap->m_pParticipatingJobs->vGrabLock();
    gpAttachMap->m_pParticipatingJobs->vContentDump();
    bRemove = gpAttachMap->m_pParticipatingJobs->bRemoveJob(pJob);
    gpAttachMap->m_pParticipatingJobs->vDropLock();
  }
  
  if (pJob->m_bMark) {
    ActivityMap::vMarkJob(pJob,"Removed from Participant List: "_Stz,
                       aCN[pJob->eClass()],
                       pJob);
    gpAttachMap->m_pParticipatingJobs->vMarkContents(pJob->m_iJobId);
  }
  
  ActionSequence::vAdd(mpTask->m_pJob->m_i_ActionRoot,
                       mpTask->m_pJob->m_iJobId,
                       mpTask->m_pJob->eClass(),
                       AN_REMOVED_PARTICIPANT);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.regionSizeResponse@ -- fetch region size from job_sanity               */
/*----------------------------------------------------------------------------*/
void Step::vJobRegionSizeFetch(MP_Task mpTask)
{
  ENTRY("vJobRegionSizeFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.regionSize@"_Stz);
  Job_Sanity* pJob_Sanity = _pJob_Sanity(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob_Sanity->m_iRegionSize);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.response@ -- fetch the job response from a node                        */
/*----------------------------------------------------------------------------*/
void Step::vJobResponseFetch(MP_Task mpTask)
{
  ENTRY("vJobResponseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.response@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.response@"_Stz);
  
  mpTask->m_pDataStack->vPushS4K(pJob->m_xNode[iNode].m_sResponse);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.response! -- store the job response from a node                        */
/*----------------------------------------------------------------------------*/
void Step::vJobResponseStore(MP_Task mpTask)
{
  ENTRY("vJobResponseStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.response!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
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
/* job.rowsInvalid@ -- fetch the rows invalid by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsInvalidFetch(MP_Task mpTask)
{
  ENTRY("vJobRowsInvalidFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.rowsInvalid@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.rowsInvalid@"_Stz);
  
  i8 iRowsInvalid = *pJob->m_xNode[iNode].m_ixRowsInvalid;
  mpTask->m_pDataStack->vPushI8(iRowsInvalid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsInvalid! -- store the rows invalid by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsInvalidStore(MP_Task mpTask)
{
  ENTRY("vJobRowsInvalidStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.rowsInvalid!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.rowsInvalid!"_Stz);
  
  i8 iRowsInvalid;
  if ( ! mpTask->m_pDataStack->bPop(iRowsInvalid) )
    THROW_INVALID_DSTACK("job.rowsInvalid!"_Stz);
  
  pJob->m_xNode[iNode].m_ixRowsInvalid = iRowsInvalid;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed@ -- fetch the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsProcessedFetch(MP_Task mpTask)
{
  ENTRY("vJobRowsProcessedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.rowsProcessed@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.rowsProcessed@"_Stz);
  
  i8 iRowsProcessed = *pJob->m_xNode[iNode].m_ixRowsProcessed;
  mpTask->m_pDataStack->vPushI8(iRowsProcessed);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsProcessed! -- store the rows processed by the job from a node      */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsProcessedStore(MP_Task mpTask)
{
  ENTRY("vJobRowsProcessedStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.rowsProcessed!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.rowsProcessed!"_Stz);
  
  i8 iRowsProcessed;
  if ( ! mpTask->m_pDataStack->bPop(iRowsProcessed) )
    THROW_INVALID_DSTACK("job.rowsProcessed!"_Stz);
  
  pJob->m_xNode[iNode].m_ixRowsProcessed = iRowsProcessed;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsSkipped@ -- fetch the rows skipped by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsSkippedFetch(MP_Task mpTask)
{
  ENTRY("vJobRowsSkippedFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.rowsSkipped@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.rowsSkipped@"_Stz);
 
  i8 iRowsSkipped = *pJob->m_xNode[iNode].m_ixRowsSkipped;

  mpTask->m_pDataStack->vPushI8(iRowsSkipped);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.rowsSkipped! -- store the rows skipped by the job from a node          */
/*----------------------------------------------------------------------------*/
void Step::vJobRowsSkippedStore(MP_Task mpTask)
{
  ENTRY("vJobRowsSkippedStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.rowsSkipped!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  i8 iNode;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iNode) )
    THROW_INVALID_DSTACK("job.rowsSkipped!"_Stz);
  
  i8 iRowsSkipped;
  if ( ! mpTask->m_pDataStack->bPop(iRowsSkipped) )
    THROW_INVALID_DSTACK("job.rowsSkipped!"_Stz);
  
  pJob->m_xNode[iNode].m_ixRowsSkipped = iRowsSkipped;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.send -- pre-send of job                                                */
/*----------------------------------------------------------------------------*/
void Step::vJobSend(MP_Task mpTask)
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
  pJobClass->vSendJob(pJob);  // come back to this, remove session as parameter to delete
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* job.show -- show the job                                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobShow(MP_Task mpTask)
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
void Step::vJobSignalConnector(MP_Task mpTask)
{
  ENTRY("vJobSignalConnector"_Stz);
  
  i8 rCode;
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.signalConnector"_Stz);
  Job* pJob = _pJob(pJobPtr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pJob->m_bNoWait) {
    EXIT();
    return;
  }
  
  if (pJob->m_bMark)
    ActivityMap::vMarkJob(pJob,"Connector signalled by %s"_Stz,
                       aCN[pJob->eClass()]);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock & unlock this job's mutex to ensure the connector has entered wait
  //if (! gxArguments.m_nDummyThreads) {
  pJob->vMutexLock();
//    rCode = pthread_mutex_lock(&pJob->m_xThreadMutex);
//    if (0 != rCode)
//      vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
  //}
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  pJob->m_eJobState = JS_SIGNALLED;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  RASSERT( _bIsValidJob(pJob->eClass()) );
  i8 i_State = pJob->m_i_State;
  
  SessionState* pState = gpAttachMap->m_pApiState->pGetState(i_State);
  ApiState::vMarkSignalled(pState->m_pSession);
  RASSERT( _bIsValidJob(pJob->eClass()) );

  // trash the cond variable
  //MemLib::vSet(&pJob->m_xThreadCond, 0xEB, sizeof(pthread_cond_t),CN_MUTEX);

  pJob->vMutexSignal();
//  rCode = pthread_cond_signal(&pJob->m_xThreadCond);
//  if (0 != rCode)
//    vWarn("pthread_cond_signal() error; rCode=%lld\n"_Stz,rCode);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //if (! gxArguments.m_nDummyThreads) {
  
  pJob->vMutexUnlock();
//    rCode = pthread_mutex_unlock(&pJob->m_xThreadMutex);
//    if (0 != rCode)
//      vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);
  //}
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.spawn -- spawn the job on the stack                                    */
/*----------------------------------------------------------------------------*/
void Step::vJobSpawn(MP_Task mpTask)
{
  ENTRY("vJobSpawn"_Stz);
  
  i8 i_Seed;
  if ( ! mpTask->m_pDataStack->bPop(i_Seed) )
    THROW_INVALID_DSTACK("job.spawn"_Stz);

  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.spawn"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  IGNORE_RETURN(Api::bSpawnJob(nullptr,pJob,(eSS)i_Seed));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job::this -- get this job from this task                                   */
/*----------------------------------------------------------------------------*/
void Step::vJobThis(MP_Task mpTask)
{
  ENTRY("vJobThis"_Stz);
  
  Job* pJob = mpTask->m_pJob;
  
  mpTask->m_pDataStack->vPushPointer(pJob);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.trace@ -- fetch the job trace                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobTraceFetch(MP_Task mpTask)
{
  ENTRY("vJobTraceFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.trace@"_Stz);
  Job_Space* pJob = _pJob_Space(pJobPtr);
  
  eJT eJobTrace = pJob->eJobTrace();
  
  mpTask->m_pDataStack->vPushI8(eJobTrace);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.trace! -- store the job trace                                          */
/*----------------------------------------------------------------------------*/
void Step::vJobTraceStore(MP_Task mpTask)
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
/* job::waitParticipant -- wait for participant till found in participant jobs*/
/*----------------------------------------------------------------------------*/
void Step::vJobWaitParticipant(MP_Task mpTask)
{
  ENTRY("vJobWaitParticipant"_Stz);
  
  i8 iJobId;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iJobId) )
    THROW_INVALID_DSTACK("job::vJobWaitParticipant"_Stz);
  
  Object* pJobObject;
  if ( ! mpTask->m_pDataStack->bPop(pJobObject) )
    THROW_INVALID_DSTACK("job::vJobWaitParticipant"_Stz);
  
  //Job* pCallingJob = _pJob(pJobObject);
  
  Job__Find* pFindJob = new Job__Find;
  pFindJob->m_iJobId = iJobId;

  Job* pJobFound = nullptr;

  while ( ! pJobFound ) {
    gpAttachMap->m_pParticipatingJobs->vGrabLock();
    pJobFound = gpAttachMap->m_pParticipatingJobs->pFindJob(pFindJob);
    gpAttachMap->m_pParticipatingJobs->vDropLock();
  }

  delete pFindJob;
  
//  Job* pJobFound = gpAttachMap->m_pParticipatingJobs->pFindJob(iJobId);
//  RASSERT(pJobFound);
  
  mpTask->m_pDataStack->vPushPointer(pJobFound);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.entryBuffer@ -- fetch entry buffer from job                            */
/*----------------------------------------------------------------------------*/
void Step::vJobEntryBufferFetch(MP_Task mpTask)
{
  ENTRY("vJobEntryBufferFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
  THROW_INVALID_DSTACK("job.entryBuffer@"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  mpTask->m_pDataStack->vPushPointer(pJob->m_pCurrentBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job.entryBuffer! -- store entry buffer in the job                          */
/*----------------------------------------------------------------------------*/
void Step::vJobEntryBufferStore(MP_Task mpTask)
{
  ENTRY("vJobEntryBufferStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job.entryBuffer!"_Stz);
  Job* pJob = _pJob(pJobPtr);
  
  Object* pEntryBufferPtr;
  if ( ! mpTask->m_pDataStack->bPop(pEntryBufferPtr) )
    THROW_INVALID_DSTACK("job.entryBuffer!"_Stz);
  EntryBuffer* pEntryBuffer = _pEntryBuffer(pEntryBufferPtr);
  
  pJob->m_pCurrentBuffer = pEntryBuffer;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
