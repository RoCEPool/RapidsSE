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
  vAddStep("Job_fetchRows.table!"_Stz,       &Step::vJob_FetchRowsTableStore);
  vAddStep("Job_fetchRows.where@"_Stz,       &Step::vJob_FetchRowsWhereFetch);
  
  vAddStep("Job_inflateRows.columnName@"_Stz,  &Step::vJob_InflateRowsColumnNameFetch);
  vAddStep("Job_inflateRows.column!"_Stz,      &Step::vJob_InflateRowsColumnStore);
  vAddStep("Job_inflateRows.spawnSub"_Stz,     &Step::vJob_InflateRowsSpawnSub);
  vAddStep("Job_inflateRows.spawnSubInflate"_Stz, &Step::vJob_InflateRowsSpawnSubInflate);
  vAddStep("job_inflateRows.subInfTasks@"_Stz, &Step::vJob_InflateRowsSubInfTasksFetch);
  vAddStep("Job_inflateRows.table!"_Stz,       &Step::vJob_InflateRowsTableStore);
  vAddStep("Job_inflateRows.where@"_Stz,       &Step::vJob_InflateRowsWhereFetch);
  
  vAddStep("Job_subFetch.bufferQueue@"_Stz,  &Step::vJob_SubFetchBufferQueueFetch);
  vAddStep("Job_subFetch.phrase@"_Stz,       &Step::vJob_SubFetchPhraseFetch);
  vAddStep("Job_subFetch.Planner@"_Stz,      &Step::vJob_SubFetchPlannerFetch);
  vAddStep("Job_subFetch.slot@"_Stz,         &Step::vJob_SubFetchSlotFetch);
  
  vAddStep("Job_subInflate.jobInflateRows@"_Stz, &Step::vJob_SubInflateJobInflateRowsFetch);
  vAddStep("Job_subInflate.inflateRows"_Stz, &Step::vJob_SubInflateInflateRows);
  
  vAddStep("Job_subScan.bufferQueue@"_Stz,  &Step::vJob_SubScanBufferQueueFetch);
  vAddStep("Job_subScan.slot@"_Stz,         &Step::vJob_SubScanSlotFetch);
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
  
  pJob_SubFetch->m_iParentId    = pJob_FetchRows->m_iJobId;
  pJob_SubFetch->m_pParentJob   = pJob_FetchRows;
  pJob_SubFetch->m_sObjectName  = pJob_FetchRows->m_sObjectName;
  pJob_SubFetch->m_pPlanner     = pPlanner;
  pJob_SubFetch->m_iPhrase      = iPhrase;
  pJob_SubFetch->m_i_Slot       = iSlot;
  pJob_SubFetch->m_pBufferQueue = pJob_FetchRows->m_pBufferQueue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pSession,pJob_SubFetch,SS_SEED_RUN));

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
/* Job_inflateRows.column@ -- fetch column name from the job                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_InflateRowsColumnNameFetch(MP_Task mpTask)
{
  ENTRY("vJob_InflateRowsColumnNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_inflateRows.column@"_Stz);
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJobPtr);
  
  mpTask->m_pDataStack->vPushS256(pJob_InflateRows->m_sColumnName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_inflateRows.column! -- store column in the job                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_InflateRowsColumnStore(MP_Task mpTask)
{
  ENTRY("vJob_InflateRowsColumnStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_inflateRows.column!"_Stz);
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJobPtr);
  
  Object* pColumnPtr;
  if ( ! mpTask->m_pDataStack->bPop(pColumnPtr) )
    THROW_INVALID_DSTACK("Job_inflateRows.column"_Stz);
  Column* pColumn = _pColumn(pColumnPtr);
  
  pJob_InflateRows->m_pColumn = pColumn;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_inflateRows.spawnSub -- spawn sub fetch job                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_InflateRowsSpawnSub(MP_Task mpTask)
{
  ENTRY("vJob_InflateRowsSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_inflateRows.spawnSub"_Stz);
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJobPtr);
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iSlot) )
    THROW_INVALID_DSTACK("Job_inflateRows.spawnSub"_Stz);
  
  Session* pSession = pJob_InflateRows->m_pSession;
  
  JobClass* pJobClass_SubScan = JobClass::pFind(CN_JOB_SUBSCAN);
  
  Job_SubScan* pJob_SubScan =
  static_cast<Job_SubScan*>(pJobClass_SubScan->pPreJob(pSession));
  
  pJob_SubScan->vSeed(SS_SEED_RUN);
  
  pJob_SubScan->m_iParentId    = pJob_InflateRows->m_iJobId;
  pJob_SubScan->m_pParentJob   = pJob_InflateRows;
  pJob_SubScan->m_sObjectName  = pJob_InflateRows->m_sObjectName;
  pJob_SubScan->m_i_Slot       = iSlot;
  pJob_SubScan->m_pBufferQueue = pJob_InflateRows->m_pBufferQueue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pSession,pJob_SubScan,SS_SEED_RUN));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_inflateRows.spawnSubInflate -- spawn sub inflate job                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_InflateRowsSpawnSubInflate(MP_Task mpTask)
{
  ENTRY("vJob_InflateRowsSpawnSubInflate"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_inflateRows.spawnSubInflate"_Stz);
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJobPtr);
  
  JobClass* pJobClass_SubInflate = JobClass::pFind(CN_JOB_SUBINFLATE);
  
  Job_SubInflate* pJob_SubInflate =
  static_cast<Job_SubInflate*>(pJobClass_SubInflate->pPreJob(nullptr));
  
  pJob_SubInflate->vSeed(SS_SEED_RUN);
  
  pJob_SubInflate->m_pJobInflateRows = pJob_InflateRows;
  
  // before spawning the sub inflate jobs set the m_nBufferCount
  pJob_InflateRows->m_inBufferCount = pJob_InflateRows->m_pTable->nRowBuffers();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(nullptr,pJob_SubInflate,SS_SEED_RUN));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_inflateRows.subInfTasks@ -- fetch no of sub inflate tasks to spawn */
/*----------------------------------------------------------------------------*/
void Step::vJob_InflateRowsSubInfTasksFetch(MP_Task mpTask)
{
  ENTRY("vJob_InflateRowsSubInfTasksFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_inflateRows.subInfTasks@"_Stz);
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJobPtr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  mpTask->m_pDataStack->vPushI8(pJob_InflateRows->m_nSubInflateTasks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_inflateRows.table! -- store table in the job                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_InflateRowsTableStore(MP_Task mpTask)
{
  ENTRY("vJob_InflateRowsTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_inflateRows.table!"_Stz);
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("Job_inflateRows.table"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_InflateRows->m_pTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_inflateRows.where@ -- fetch the where string from the job              */
/*----------------------------------------------------------------------------*/
void Step::vJob_InflateRowsWhereFetch(MP_Task mpTask)
{
  ENTRY("vJob_InflateRowsWhereFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_inflateRows.where@!"_Stz);
  Job_InflateRows* pJob_InflateRows = _pJob_InflateRows(pJobPtr);
  
  mpTask->m_pDataStack->vPushS256(pJob_InflateRows->m_sWhere);
  
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
/* job_subInflate.JobInflateRows@ -- fetch the inflate rows job               */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubInflateJobInflateRowsFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubInflateJobInflateRowsFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subInflate.JobFetchRows@"_Stz);
  Job_SubInflate* pJob = _pJob_SubInflate(pJobPtr);
  
  mpTask->m_pDataStack->vPushPointer(pJob->m_pJobInflateRows);
  
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
  Job_InflateRows* pJob_InflateRows = pJob_SubInflate->m_pJobInflateRows;
  
  // get the buffer queue
  Queue* pBufferQueue = pJob_InflateRows->m_pBufferQueue;
  
  CrwBuffer* CrwBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check for errors
  if ( pJob_InflateRows->m_bHasError ) {
    
    // just return
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // retrieve
  
  // create a row for retrieval
  pJob_SubInflate->m_pRow = new Row(pJob_InflateRows->m_pTable->pColumnVec());
  
  while ( *pJob_InflateRows->m_inBuffersScanned != *pJob_InflateRows->m_inBufferCount ) {
    
    CrwBuffer = _pCrwBuffer(pJob_InflateRows->m_pBufferQueue->pPop());
    if (! CrwBuffer) {
      //Pthread::vSleepMilli(1,CN_JOB_FETCHROWS);
      Pthread::vSleepMicro(250,CN_JOB_FETCHROWS);
      continue;
    }
    
    if (pJob_InflateRows->m_pColumn)
      vRetrieveColumns(pJob_InflateRows, CrwBuffer, pJob_SubInflate->m_pRow);
    else
      vRetrieveRows(pJob_InflateRows, CrwBuffer, pJob_SubInflate->m_pRow);
  
    pJob_InflateRows->m_inRowsProcessed += CrwBuffer->nRowCount();
    ++pJob_InflateRows->m_inBuffersScanned;
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subScan.bufferQueue@ -- fetch the job buffer queue                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubScanBufferQueueFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubScanBufferQueueFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subScan.bufferQueue@"_Stz);
  Job_SubScan* pJob = _pJob_SubScan(pJobPtr);
  
  mpTask->m_pDataStack->vPushPointer(pJob->m_pBufferQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subScan.slot@ -- fetch the job slot                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubScanSlotFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubScanSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subScan.slot@"_Stz);
  Job_SubScan* pJob = _pJob_SubScan(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob->m_i_Slot);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
