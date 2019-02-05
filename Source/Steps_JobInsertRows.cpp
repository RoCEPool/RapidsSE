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
  vAddStep("job_copyBuffer.copy"_Stz,              &Step::vJob_CopyBufferCopy);

  vAddStep("job_insertRow.partition@"_Stz,         &Step::vJob_InsertRowPartitionFetch);
  vAddStep("job_insertRow.newJobWriteRow"_Stz,     &Step::vJob_InsertRowNewJobWriteRow);
  vAddStep("job_insertRow.spawnJobWriteRow"_Stz,   &Step::vJob_InsertRowSpawnJobWriteRow);
  
  vAddStep("Job_insertTable.subInserterJob@"_Stz,  &Step::vJob_InsertTableSubInserterJobFetch);
  
  vAddStep("job_subInserter.begin"_Stz,            &Step::vJob_SubInserterBegin);
  vAddStep("job_subInserter.end"_Stz,              &Step::vJob_SubInserterEnd);
}

/*----------------------------------------------------------------------------*/
/* job_copyBuffer.copy -- copy the record buffer into entryBuffer             */
/*----------------------------------------------------------------------------*/
void Step::vJob_CopyBufferCopy(MP_Task mpTask)
{
  ENTRY("vJob_CopyBufferCopy"_Stz);
  
  Object* pPtrCopyBuffer;
  if ( ! mpTask->m_pDataStack->bPop(pPtrCopyBuffer) )
    THROW_INVALID_DSTACK("job_copyBuffer.copy"_Stz);
  Job_CopyBuffer* pJob_CopyBuffer = _pJob_CopyBuffer(pPtrCopyBuffer);
  
  pJob_CopyBuffer->vCopy();
  
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
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("job_InsertRow.newJobWriteRow"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  Session* pSession = pJob_InsertRow->m_pSession;
  Stz* sTableName = pJob_InsertRow->m_sObjectName;
  i8 iParentJobId = pJob_InsertRow->iJobId();
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // new a writeRow job
  
  Job_WriteRow* pJob_WriteRow = Job_WriteRow::pNewJobWriteRow(pSession,
                                                              sTableName,
                                                              pTable,
                                                              iParentJobId);
  
  mpTask->m_pDataStack->vPushPointer(pJob_WriteRow);
  
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
  
  mpTask->m_pDataStack->vPushI8(pJob_InsertRow->m_iPartitionId);
  
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
  IGNORE_RETURN(Api::bSpawnJob(pJob_InsertRow->m_pSession,pJob_WriteRow,SS_SEED_RUN));
  
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
  
  mpTask->m_pDataStack->vPushPointer(pJob_InsertTable->m_pJobSubInserter);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_SubInserter.begin -- begin subInserter                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubInserterBegin(MP_Task mpTask)
{
  ENTRY("vJob_SubInserterBegin"_Stz);
  
  Object* pPtrSubInserter;
  if ( ! mpTask->m_pDataStack->bPop(pPtrSubInserter) )
    THROW_INVALID_DSTACK("job_subInserter.begin"_Stz);
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pPtrSubInserter);
  
  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
    THROW_INVALID_DSTACK("job_subInserter.begin"_Stz);
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
    THROW_INVALID_DSTACK("job_subInserter.end"_Stz);
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pPtrSubInserter);
  
  pJob_SubInserter->vEnd();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
