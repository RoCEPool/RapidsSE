/*============================================================================*/
/* Steps_JobIngest.cpp - job_ingest step group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobIngest(void)
{
  vAddStep("Job_Ingest.begin"_Stz,      &Step::vJob_IngestBegin);
  vAddStep("job_ingest.chunk"_Stz,      &Step::vJob_IngestChunk);
  vAddStep("Job_Ingest.end"_Stz,        &Step::vJob_IngestEnd);
  vAddStep("job_ingest.table!"_Stz,     &Step::vJob_IngestTableStore);
}

/*----------------------------------------------------------------------------*/
/* Job_Ingest.begin -- begin ingesting                                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_IngestBegin(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_IngestBegin"_Stz);
  
  Object* pPtrJob_Ingest;
  if ( ! pForth->m_pDataStack->bPop(pPtrJob_Ingest) )
    THROW_DSTACK();
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pPtrJob_Ingest);
  
  bool bBegun = pJob_Ingest->bBegin();
  
  pForth->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_ingest.chunk -- ingest a chunk of data into a table                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_IngestChunk(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_IngestChunk"_Stz);
  
  Object* pPtrJobIngest;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobIngest) )
    THROW_DSTACK();
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pPtrJobIngest);
  
  bool bCallAgain = pJob_Ingest->bChunk();
  
  pForth->m_pDataStack->vPushBool(bCallAgain);
  
//  if (! bCallAgain) {
//    // sleep for 10 seconds
//    mpTask->vSleepUs(C_1M * 100);
//    mpTask->vTransition(TR_SLEEP);
//  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_Ingest.end -- end ingesting                                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_IngestEnd(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_IngestEnd"_Stz);
  
  Object* pPtrJob_Ingest;
  if ( ! pForth->m_pDataStack->bPop(pPtrJob_Ingest) )
    THROW_DSTACK();
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pPtrJob_Ingest);
  
  pJob_Ingest->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_ingest.table! -- store table into job_ingest                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_IngestTableStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_IngestTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_Ingest;
  if ( ! pForth->m_pDataStack->bPop(pPtrJob_Ingest,pPtrTable) )
    THROW_DSTACK();
  
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pPtrJob_Ingest);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_Ingest->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
