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
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.column! -- store column in the job                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsColumnStore(void)
{
  ENTRY();
  
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(m_pDataStack->pPopPtr(__func__));
  
  Column* pColumn = _pColumn(m_pDataStack->pPopPtr(__func__));
  
  pJob_FetchRows->m_pColumn = pColumn;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.column@ -- fetch column name from the job                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsColumnNameFetch(void)
{
  ENTRY();
  
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushStr(pJob_FetchRows->m_sColumnName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.lastBuffers! -- set no of expected last buffers              */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsLastBuffersStore(void)
{
  ENTRY();
  
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(m_pDataStack->pPopPtr(__func__));
  
  i8 nLastBuffers = m_pDataStack->iPopAsInt(__func__);
  
  pJob_FetchRows->m_nLastBuffers = nLastBuffers;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.partition@ -- fetch partition no from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsPartitionFetch(void)
{
  ENTRY();
  
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(m_pDataStack->pPopPtr(__func__));

  m_pDataStack->vPushInt(pJob_FetchRows->m_iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.planner! -- store planner in the job                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsPlannerStore(void)
{
  ENTRY();
  
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(m_pDataStack->pPopPtr(__func__));
  
  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  
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
  ENTRY();
 /*
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(m_pDataStack->pPopPtr(__func__));

  Planner* pPlanner = _pPlanner(m_pDataStack->pPopPtr(__func__));
  ASSERT(CN_PLANNER == pPlanner->eClass());
  
  i8 iPhrase = m_pDataStack->iPopAsInt(__func__);
  
  i8 iSlot = m_pDataStack->iPopAsInt(__func__);
  
  Session* pSession = pJob_FetchRows->pSession();
  ReqTuple* aReqTuple = &pJob_FetchRows->m_xReqTuple;

  JobClass* pJobClass_SubFetch = JobClass::pFind(CN_JOB_SUBFETCH);
  
  Job_SubFetch* pJob_SubFetch =
    static_cast<Job_SubFetch*>
      (pJobClass_SubFetch->pPreJob(aReqTuple,pSession));
  
  pJob_SubFetch->vSeed(SS_SEED_RUN);
  
  pJob_SubFetch->vParentId(pJob_FetchRows->m_iFocalId);
  pJob_SubFetch->m_pParentJob   = pJob_FetchRows;
  pJob_SubFetch->pJobTags()->vInsert(C_TAG_TABLENM,
                                      pJob_FetchRows->pJobTags()->szValue(C_TAG_TABLENM));
  pJob_SubFetch->m_pPlanner     = pPlanner;
  pJob_SubFetch->m_iPhrase      = iPhrase;
  pJob_SubFetch->vSlot(iSlot);
  pJob_SubFetch->m_pBufferQueue = pJob_FetchRows->m_pBufferQueue;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Task* pTask = pJob_SubFetch->pTask();
  if (pTask) {
    RASSERT(QK_NONE == pTask->eQueue());
    RASSERT(TR_NONE == pTask->eTranstionRequest());
  }
  Api::vSpawnJob_service(pJob_SubFetch);
*/
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.table! -- store table in the job                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsTableStore(void)
{
  ENTRY();
  
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(m_pDataStack->pPopPtr(__func__));
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  pJob_FetchRows->m_pTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_fetchRows.where@ -- fetch the where string from the job                */
/*----------------------------------------------------------------------------*/
void Step::vJob_FetchRowsWhereFetch(void)
{
  ENTRY();
  
  Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushStr(pJob_FetchRows->m_sWhere);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subFetch.bufferQueue@ -- fetch the job buffer queue                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchBufferQueueFetch(void)
{
  ENTRY();
  
  Job_SubFetch* pJob = _pJob_SubFetch(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pJob->m_pBufferQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subFetch.phrase@ -- fetch the job phrase                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchPhraseFetch(void)
{
  ENTRY();
  
  Job_SubFetch* pJob = _pJob_SubFetch(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->m_iPhrase);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subFetch.planner@ -- fetch the job planner                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchPlannerFetch(void)
{
  ENTRY();
  
  Job_SubFetch* pJob = _pJob_SubFetch(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushPtr(pJob->m_pPlanner);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subFetch.slot@ -- fetch the job slot                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubFetchSlotFetch(void)
{
  ENTRY();
  
  Job_SubFetch* pJob = _pJob_SubFetch(m_pDataStack->pPopPtr(__func__));
  
  m_pDataStack->vPushInt(pJob->i_Slot());
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
