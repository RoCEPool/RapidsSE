/*============================================================================*/
/* Steps_JobInsertRow.cpp - job step group                                    */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobInsertRows(void)
{
  vAddStep("Job_SubInserter.begin"_Stz,              &Step::vJob_SubInserterBegin);
  vAddStep("Job_SubInserter.end"_Stz,                &Step::vJob_SubInserterEnd);
  
  vAddStep("Job_insertTable.subInserterJob@"_Stz,    &Step::vJob_InsertTableSubInserterJobFetch);

  vAddStep("job_insertRow.partition@"_Stz,         &Step::vJob_InsertRowPartitionFetch);
  vAddStep("job_insertRow.newJobWriteRow"_Stz,     &Step::vJob_InsertRowNewJobWriteRow);
  vAddStep("job_insertRow.spawnJobWriteRow"_Stz,   &Step::vJob_InsertRowSpawnJobWriteRow);
}

/*----------------------------------------------------------------------------*/
/* Job_SubInserter.begin -- begin subInserter                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubInserterBegin(MP_Task mpTask)
{
  ENTRY("vJob_SubInserterBegin"_Stz);
  
  Object* pPtrSubInserter;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubInserter) )
    THROW_INVALID_DSTACK("Job_SubInserter.begin"_Stz);
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pPtrSubInserter);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
     THROW_INVALID_DSTACK("Job_SubInserter.begin"_Stz);
  Table* pTable = _pTable(pPtrTable);
  
  pJob_SubInserter->vBegin(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_SubInsert.end -- spawn last write buffers ~each partition              */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubInserterEnd(MP_Task mpTask)
{
  ENTRY("vJob_SubInserterEnd"_Stz);
  
  Object* pPtrSubInserter;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubInserter) )
    THROW_INVALID_DSTACK("Job_SubInsert.end"_Stz);
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pPtrSubInserter);
  
  pJob_SubInserter->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_InsertTable.SubInserterJob@ -- fetch subInserter jobid from job        */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertTableSubInserterJobFetch(MP_Task mpTask)
{
  ENTRY("vJob_InsertTableSubInserterJobFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_InsertTable.SubInsertJob@"_Stz);
  Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob_InsertTable->m_pJobSubInserter);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.newJobWriteRow -- new the writeRow job                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowNewJobWriteRow(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowNewJobWriteRow"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertRow.newJobWriteRow"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
  
  Session* pSession = pJob_InsertRow->m_pSession;
  Stz* sTableName = pJob_InsertRow->m_sObjectName;
  i8 iParentJobId = pJob_InsertRow->iJobId();
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // new a writeRow job
  
  Job_WriteRow* pJob_WriteRow = Job_WriteRow::pNewJobWriteRow(pSession,
                                                              sTableName,
                                                              iParentJobId);
  
  mpTask->m_pDataStack->vPushObject(pJob_WriteRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_InsertRow.partition@ -- fetch partition id from job                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowPartitionFetch(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_InsertRow.partition@"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob_InsertRow->m_iPartitionId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.spawnWriteRow -- spawn a writeRow job                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowSpawnJobWriteRow(MP_Task mpTask)
{
  ENTRY("vJob_WriteRowSpawnJobWriteRow"_Stz);
  
  Object* pJobInsertRowPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobInsertRowPtr) )
    THROW_INVALID_DSTACK("job_WriteRow.spawnWriteRow"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobInsertRowPtr);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteRow.spawnWriteRow"_Stz);
  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobPtr);
  
  // spawn a writeRow job
  RapidsSE_bSpawnJob(pJob_WriteRow,SS_SEED_RUN,pJob_InsertRow->m_pSession);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
