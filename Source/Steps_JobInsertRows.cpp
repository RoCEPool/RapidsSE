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
  vAddStep("Job_InsertRows.begin"_Stz,              &Step::vJob_InsertRowsBegin);
  vAddStep("Job_InsertRows.end"_Stz,                &Step::vJob_InsertRowsEnd);
  vAddStep("Job_InsertTable.insertRowsJob@"_Stz,    &Step::vJob_InsertTableInsertRowsJobFetch);

  vAddStep("job_insertRow.partition@"_Stz,          &Step::vJob_InsertRowPartitionFetch);
  vAddStep("job_insertRow.newJobWriteRow"_Stz,      &Step::vJob_InsertRowNewJobWriteRow);
  vAddStep("job_insertRow.spawnJobWriteRow"_Stz,    &Step::vJob_InsertRowSpawnJobWriteRow);
}

/*----------------------------------------------------------------------------*/
/* Job_InsertRows.begin -- begin insertrows                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowsBegin(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowsBegin"_Stz);
  
  Object* pPtrInsertRows;
  if ( ! mpTask->m_pDataStack->bPop(pPtrInsertRows) )
    THROW_INVALID_DSTACK("Job_InsertRows.begin"_Stz);
  Job_InsertRows* pJob_InsertRows = _pJob_InsertRows(pPtrInsertRows);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
     THROW_INVALID_DSTACK("Job_InsertRows.begin"_Stz);
  Table* pTable = _pTable(pPtrTable);
  
  pJob_InsertRows->vBegin(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_InsertRows.end -- spawn last write buffers ~each partition            */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowsEnd(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowsEnd"_Stz);
  
  Object* pPtrInsertRows;
  if ( ! mpTask->m_pDataStack->bPop(pPtrInsertRows) )
    THROW_INVALID_DSTACK("Job_InsertRows.end"_Stz);
  Job_InsertRows* pJob_InsertRows = _pJob_InsertRows(pPtrInsertRows);
  
  pJob_InsertRows->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_InsertTable.InsertRowsJob@ -- fetch insertRow jobid from job            */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertTableInsertRowsJobFetch(MP_Task mpTask)
{
  ENTRY("vJob_InsertTableInsertRowsJobFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_InsertTable.InsertRowsJob@"_Stz);
  Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob_InsertTable->m_pJobInsertRows);
  
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
