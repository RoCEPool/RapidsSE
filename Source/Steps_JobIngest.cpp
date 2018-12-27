/*============================================================================*/
/* Steps_JobIngest.cpp - job_ingest step group                                */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobIngest(void)
{
  vAddStep("Job_Ingest.begin"_Stz,      &Step::vJob_IngestBegin);
  vAddStep("Job_Ingest.end"_Stz,        &Step::vJob_IngestEnd);
  vAddStep("job_ingest.ingest"_Stz,     &Step::vJob_IngestIngest);
  vAddStep("job_ingest.table!"_Stz,     &Step::vJob_IngestTableStore);
}

/*----------------------------------------------------------------------------*/
/* Job_Ingest.begin -- begin loading                                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_IngestBegin(MP_Task mpTask)
{
  ENTRY("vJob_IngestBegin"_Stz);
  
  Object* pPtrJob_Ingest;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Ingest) )
    THROW_INVALID_DSTACK("Job_Ingest.begin"_Stz);
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pPtrJob_Ingest);
  
  pJob_Ingest->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Ingest.end -- end loading                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_IngestEnd(MP_Task mpTask)
{
  ENTRY("vJob_IngestEnd"_Stz);
  
  Object* pPtrJob_Ingest;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Ingest) )
    THROW_INVALID_DSTACK("Job_Ingest.end"_Stz);
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pPtrJob_Ingest);
  
  pJob_Ingest->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_ingest.ingest -- ingest data into a table                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_IngestIngest(MP_Task mpTask)
{
  ENTRY("vJob_IngestIngest"_Stz);
  
  Object* pPtrJobIngest;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobIngest) )
    THROW_INVALID_DSTACK("job_ingest.ingest"_Stz);
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pPtrJobIngest);
  
  pJob_Ingest->vIngest();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_ingest.table! -- store table into job_ingest                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_IngestTableStore(MP_Task mpTask)
{
  ENTRY("vJob_IngestTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_Ingest;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Ingest,pPtrTable) )
    THROW_INVALID_DSTACK("job_ingest.tableStore"_Stz);
  
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pPtrJob_Ingest);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_Ingest->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
