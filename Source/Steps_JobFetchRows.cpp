/*============================================================================*/
/* Steps_JobFetchRows.cpp - jobFetchRows step group                           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobFetchRows(void)
{
  vAddStep("Job_fetchRows.columnName@"_Stz,  &Step::vJob_FetchRowsColumnNameFetch);
  vAddStep("Job_fetchRows.column!"_Stz,      &Step::vJob_FetchRowsColumnStore);
  vAddStep("Job_fetchRows.lastBuffers!"_Stz, &Step::vJob_FetchRowsLastBuffersStore);
  vAddStep("Job_fetchRows.partition@"_Stz,   &Step::vJob_FetchRowsPartitionFetch);
  vAddStep("Job_fetchRows.planner!"_Stz,     &Step::vJob_FetchRowsPlannerStore);
  vAddStep("Job_fetchRows.spawnSub"_Stz,     &Step::vJob_FetchRowsSpawnSub);
  vAddStep("Job_fetchRows.spawnSubInflate"_Stz, &Step::vJob_FetchRowsSpawnSubInflate);
  vAddStep("Job_fetchRows.table!"_Stz,       &Step::vJob_FetchRowsTableStore);
  vAddStep("Job_fetchRows.where@"_Stz,       &Step::vJob_FetchRowsWhereFetch);
  
  vAddStep("Job_subFetch.bufferQueue@"_Stz,  &Step::vJob_SubFetchBufferQueueFetch);
  vAddStep("Job_subFetch.phrase@"_Stz,       &Step::vJob_SubFetchPhraseFetch);
  vAddStep("Job_subFetch.Planner@"_Stz,      &Step::vJob_SubFetchPlannerFetch);
  vAddStep("Job_subFetch.slot@"_Stz,         &Step::vJob_SubFetchSlotFetch);
  
  vAddStep("Job_subInflate.jobFetchRows@"_Stz, &Step::vJob_SubInflateJobFetchRowsFetch);
  vAddStep("Job_subInflate.inflateRows"_Stz, &Step::vJob_SubInflateInflateRows);
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.column! -- store column in the job                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsColumnStore(MP_Task mpTask)
{
  ENTRY("vJob_FetchRowsColumnStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.column!"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  Object* pColumnPtr;
  if ( ! mpTask->m_pDataStack->bPop(pColumnPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.table"_Stz);
  Column* pColumn = _pColumn(pColumnPtr);
  
  pJob_FetchRows->m_pColumn = pColumn;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.column@ -- fetch column name from the job                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsColumnNameFetch(MP_Task mpTask)
{
  ENTRY("vJob_FetchRowsColumnNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.columnName@"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  mpTask->m_pDataStack->vPushS256(pJob_FetchRows->m_sColumnName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.lastBuffers! -- set no of expected last buffers              */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsLastBuffersStore(MP_Task mpTask)
{
  ENTRY("vJob_FetchRowsLastBuffersStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.lastBuffers!"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  i8 nLastBuffers;
  if ( ! mpTask->m_pDataStack->bPopAsInt(nLastBuffers) )
    THROW_INVALID_DSTACK("Job_fetchRows.lastBuffers"_Stz);
  
  pJob_FetchRows->m_nLastBuffers = nLastBuffers;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.partition@ -- fetch partition no from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsPartitionFetch(MP_Task mpTask)
{
  ENTRY("vJob_FetchRowsPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.partition@"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);

  mpTask->m_pDataStack->vPushI8(pJob_FetchRows->m_iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.planner! -- store planner in the job                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsPlannerStore(MP_Task mpTask)
{
  ENTRY("vJob_FetchRowsPlannerStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.planner!"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  Object* pPlannerPtr;
  if ( ! mpTask->m_pDataStack->bPop(pPlannerPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.lastBuffers"_Stz);
  Planner* pPlanner = _pPlanner(pPlannerPtr);
  
  pJob_FetchRows->m_pPlanner = pPlanner;
  
  // if we have or phrases we'll need a RowOnce object
  if (pPlanner->nPhrases() > 1)
    pJob_FetchRows->vCreateRowOnce();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.spawnSub -- spawn sub fetch job                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsSpawnSub(MP_Task mpTask)
{
  ENTRY("vJob_FetchRowsSpawnSub"_Stz);
  
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
  
  Session* pSession = pJob_FetchRows->m_pSession;
  
  JobClass* pJobClass_SubFetch = JobClass::pFind(CN_JOB_SUBFETCH);
  
  Job_SubFetch* pJob_SubFetch =
  static_cast<Job_SubFetch*>(pJobClass_SubFetch->pPreJob(pSession));
  
  pJob_SubFetch->vSeed(SS_SEED_RUN);
  
  pJob_SubFetch->m_iParentId     = pJob_FetchRows->m_iJobId;
  pJob_SubFetch->m_pJobFetchRows = pJob_FetchRows;
  pJob_SubFetch->m_sObjectName   = pJob_FetchRows->m_sObjectName;
  pJob_SubFetch->m_pPlanner     = pPlanner;
  pJob_SubFetch->m_iPhrase      = iPhrase;
  pJob_SubFetch->m_i_Slot       = iSlot;
  pJob_SubFetch->m_pBufferQueue  = pJob_FetchRows->m_pBufferQueue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pSession,pJob_SubFetch,SS_SEED_RUN));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.spawnSubInflate -- spawn sub inflate job                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsSpawnSubInflate(MP_Task mpTask)
{
  ENTRY("vJob_FetchRowsSpawnSubInflate"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.spawnSubInflate"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  JobClass* pJobClass_SubInflate = JobClass::pFind(CN_JOB_SUBINFLATE);
  
  Job_SubInflate* pJob_SubInflate =
  static_cast<Job_SubInflate*>(pJobClass_SubInflate->pPreJob(nullptr));
  
  pJob_SubInflate->vSeed(SS_SEED_RUN);
  
  pJob_SubInflate->m_pJobFetchRows = pJob_FetchRows;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(nullptr,pJob_SubInflate,SS_SEED_RUN));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.table! -- store table in the job                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsTableStore(MP_Task mpTask)
{
  ENTRY("vJob_FetchRowsTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.table!"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.table"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_FetchRows->m_pTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.where@ -- fetch the where string from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsWhereFetch(MP_Task mpTask)
{
  ENTRY("vJob_FetchRowsWhereFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_fetchRows.where@!"_Stz);
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  mpTask->m_pDataStack->vPushS256(pJob_FetchRows->m_sWhere);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subFetch.bufferQueue@ -- fetch the job buffer queue                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchBufferQueueFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubFetchBufferQueueFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subFetch.bufferQueue@"_Stz);
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  mpTask->m_pDataStack->vPushPointer(pJob->m_pBufferQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subFetch.phrase@ -- fetch the job phrase                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchPhraseFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubFetchPhraseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subFetch.phrase@"_Stz);
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob->m_iPhrase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subFetch.planner@ -- fetch the job planner                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchPlannerFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubFetchPlannerFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subFetch.planner@"_Stz);
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  mpTask->m_pDataStack->vPushPointer(pJob->m_pPlanner);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subFetch.slot@ -- fetch the job slot                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchSlotFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubFetchSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subFetch.slot@"_Stz);
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob->m_i_Slot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subInflate.JobFetchRows@ -- fetch the fetch rows job                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubInflateJobFetchRowsFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubInflateJobFetchRowsFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subInflate.JobFetchRows@"_Stz);
  Job_SubInflate* pJob = _pJob_SubInflate(pJobPtr);
  
  mpTask->m_pDataStack->vPushPointer(pJob->m_pJobFetchRows);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subInflate.inflateRows -- inflate rows                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubInflateInflateRows(MP_Task mpTask)
{
  ENTRY("vJob_SubInflateInflateRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subInflate.inflateRows"_Stz);
  Job_SubInflate* pJob_SubInflate = _pJob_SubInflate(pJobPtr);
  
  // get the fetch job
  Job_FetchRows* pJob_FetchRows = pJob_SubInflate->m_pJobFetchRows;
  
  // get the buffer queue
  Queue* pBufferQueue = pJob_FetchRows->m_pBufferQueue;
  
  i8 nRowsProcessed = 0;
  i8 nLastBuffersScanned = 0;
  i8 iRowsPrinted = 0;
  
  FetchBuffer* pFetchBuffer = nullptr;
  bool bDone = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check for errors
  if ( pJob_FetchRows->m_bHasError ) {
    
    // just return
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // retrieve
  if (pJob_FetchRows->m_bRetrieve) {
    
    if (pJob_FetchRows->m_bPrint && gpAttachMap->m_bMetadata) {
      Column* pCol = pJob_FetchRows->m_pColumn;
      vTrace("metadata: (%s %s)\n"_Stz, pCol->sColumnName().aChar(), aCT[pCol->eColumnType()]);
    }
    
    // create a row for retrieval
    pJob_FetchRows->m_pRow = new Row(pJob_FetchRows->m_pTable->pColumnVec());
    
    while ( nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers ) {
      
      pFetchBuffer = _pFetchBuffer(pJob_FetchRows->m_pBufferQueue->pPop());
      if (! pFetchBuffer) {
        //Pthread::vSleepMilli(1,CN_JOB_FETCHROWS);
        Pthread::vSleepMicro(250,CN_JOB_FETCHROWS);
        continue;
      }
      
      vRetrieveColumn(pJob_FetchRows, pFetchBuffer);
      nRowsProcessed += pFetchBuffer->nFetchedRows();
      
      if (pFetchBuffer->bIsLast()) {
        nLastBuffersScanned++;
      }
      delete pFetchBuffer;
      pFetchBuffer = nullptr;
    }
    
    if (pJob_FetchRows->m_bPrint)
      vTrace("success: %lld rows fetched\n"_Stz,nRowsProcessed);
    else
      vTrace("success: %lld rows affected\n"_Stz,nRowsProcessed);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fetch
  if (gpAttachMap->m_bMetadata) {
    vTrace("metadata: %s\n"_Stz, pJob_FetchRows->m_sMetadata.aStz());
  }
  
  // create a row for retrieval
  pJob_FetchRows->m_pRow = new Row(pJob_FetchRows->m_pTable->pColumnVec());
  
  while ( nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers ) {
    
    pFetchBuffer = _pFetchBuffer(pJob_FetchRows->m_pBufferQueue->pPop());
    if (! pFetchBuffer) {
      Pthread::vSleepMilli(1,CN_JOB_FETCHROWS);
      continue;
    }
    
    if (pJob_FetchRows->m_bDisplay) {
      if (pJob_FetchRows->m_bLimit && iRowsPrinted != pJob_FetchRows->m_iRows)
        vIterateBuffer(pJob_FetchRows, pFetchBuffer, iRowsPrinted);
      else
        vIterateBuffer(pJob_FetchRows, pFetchBuffer, iRowsPrinted);
    }
    
    nRowsProcessed += pFetchBuffer->nFetchedRows();
    
    if (pFetchBuffer->bIsLast()) {
      nLastBuffersScanned++;
    }
    delete pFetchBuffer;
    pFetchBuffer = nullptr;
  }
  
  if (pJob_FetchRows->m_iRows == C_UNKNOWN)
    vTrace("success: %lld rows fetched\n"_Stz,nRowsProcessed);
  else {
    if (pJob_FetchRows->m_bFirst)
      vTrace("success: %lld rows fetched; first %lld rows printed\n"_Stz, nRowsProcessed, pJob_FetchRows->m_iRows);
    else if (pJob_FetchRows->m_bLimit)
      vTrace("success: %lld rows fetched\n"_Stz, pJob_FetchRows->m_iRows);
    else
      RASSERT(0);
  }
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
