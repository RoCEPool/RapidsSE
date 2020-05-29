/*============================================================================*/
/* Steps_JobFetchRows.cpp - jobFetchRows step group                           */
/*============================================================================*/

#include "Steps.h"

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
  
  vAddStep("Job_subFetch.bufferQueue@"_Stz,  &Step::vJob_SubFetchBufferQueueFetch);
  vAddStep("Job_subFetch.phrase@"_Stz,       &Step::vJob_SubFetchPhraseFetch);
  vAddStep("Job_subFetch.Planner@"_Stz,      &Step::vJob_SubFetchPlannerFetch);
  vAddStep("Job_subFetch.slot@"_Stz,         &Step::vJob_SubFetchSlotFetch);
  
  vAddStep("Job_subScan.bufferQueue@"_Stz,  &Step::vJob_SubScanBufferQueueFetch);
  vAddStep("Job_subScan.slot@"_Stz,         &Step::vJob_SubScanSlotFetch);
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.column! -- store column in the job                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsColumnStore(void)
{
  ENTRY("vJob_FetchRowsColumnStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  Object* pColumnPtr;
  if ( ! m_pDataStack->bPopObject(pColumnPtr) )
    THROW_DSTACK();
  Column* pColumn = _pColumn(pColumnPtr);
  
  pJob_FetchRows->m_pColumn = pColumn;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.column@ -- fetch column name from the job                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsColumnNameFetch(void)
{
  ENTRY("vJob_FetchRowsColumnNameFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  m_pDataStack->vPushS256(pJob_FetchRows->m_sColumnName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.lastBuffers! -- set no of expected last buffers              */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsLastBuffersStore(void)
{
  ENTRY("vJob_FetchRowsLastBuffersStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  i8 nLastBuffers;
  if ( ! m_pDataStack->bPopAsInt(nLastBuffers) )
    THROW_DSTACK();
  
  pJob_FetchRows->m_nLastBuffers = nLastBuffers;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.partition@ -- fetch partition no from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsPartitionFetch(void)
{
  ENTRY("vJob_FetchRowsPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);

  m_pDataStack->vPushI8(pJob_FetchRows->m_iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.planner! -- store planner in the job                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsPlannerStore(void)
{
  ENTRY("vJob_FetchRowsPlannerStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  Object* pPlannerPtr;
  if ( ! m_pDataStack->bPopObject(pPlannerPtr) )
    THROW_DSTACK();
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
void Step::vJob_FetchRowsSpawnSub(void)
{
  ENTRY("vJob_FetchRowsSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);

  Object* pPlannerPtr;
  if ( ! m_pDataStack->bPopObject(pPlannerPtr) )
    THROW_DSTACK();
  Planner* pPlanner = _pPlanner(pPlannerPtr);
  ASSERT(CN_PLANNER == pPlanner->eClass());
  
  i8 iPhrase;
  if ( ! m_pDataStack->bPopAsInt(iPhrase) )
    THROW_DSTACK();
  
  i8 iSlot;
  if ( ! m_pDataStack->bPopAsInt(iSlot) )
    THROW_DSTACK();
  
  Session* pSession = pJob_FetchRows->pSession();
  ReqTuple* aReqTuple = &pJob_FetchRows->m_xReqTuple;

  JobClass* pJobClass_SubFetch = JobClass::pFind(CN_JOB_SUBFETCH);
  
  Job_SubFetch* pJob_SubFetch =
    static_cast<Job_SubFetch*>
      (pJobClass_SubFetch->pPreJob(aReqTuple,pSession));
  
  pJob_SubFetch->vSeed(SS_SEED_RUN);
  
  pJob_SubFetch->m_iParentId    = pJob_FetchRows->m_iFocalId;
  pJob_SubFetch->m_pParentJob   = pJob_FetchRows;
  pJob_SubFetch->m_oEnameSet.vAdd("<tableNm>"_Stz,
                                pJob_FetchRows->m_oEnameSet.szValue("<tableNm>"_Stz));
  pJob_SubFetch->m_pPlanner     = pPlanner;
  pJob_SubFetch->m_iPhrase      = iPhrase;
  pJob_SubFetch->vSlot(iSlot);
  pJob_SubFetch->m_pBufferQueue = pJob_FetchRows->m_pBufferQueue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pJob_SubFetch));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.table! -- store table in the job                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsTableStore(void)
{
  ENTRY("vJob_FetchRowsTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  Object* pTablePtr;
  if ( ! m_pDataStack->bPopObject(pTablePtr) )
    THROW_DSTACK();
  Table* pTable = _pTable(pTablePtr);
  
  pJob_FetchRows->m_pTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.where@ -- fetch the where string from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsWhereFetch(void)
{
  ENTRY("vJob_FetchRowsWhereFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
  m_pDataStack->vPushS256(pJob_FetchRows->m_sWhere);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subFetch.bufferQueue@ -- fetch the job buffer queue                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchBufferQueueFetch(void)
{
  ENTRY("vJob_SubFetchBufferQueueFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  m_pDataStack->vPushPointer(pJob->m_pBufferQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subFetch.phrase@ -- fetch the job phrase                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchPhraseFetch(void)
{
  ENTRY("vJob_SubFetchPhraseFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_iPhrase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subFetch.planner@ -- fetch the job planner                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchPlannerFetch(void)
{
  ENTRY("vJob_SubFetchPlannerFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  m_pDataStack->vPushPointer(pJob->m_pPlanner);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subFetch.slot@ -- fetch the job slot                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchSlotFetch(void)
{
  ENTRY("vJob_SubFetchSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubFetch* pJob = _pJob_SubFetch(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->i_Slot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subScan.bufferQueue@ -- fetch the job buffer queue                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubScanBufferQueueFetch(void)
{
  ENTRY("vJob_SubScanBufferQueueFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubScan* pJob = _pJob_SubScan(pJobPtr);
  
  m_pDataStack->vPushPointer(pJob->m_pBufferQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subScan.slot@ -- fetch the job slot                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubScanSlotFetch(void)
{
  ENTRY("vJob_SubScanSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubScan* pJob = _pJob_SubScan(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->i_Slot());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
