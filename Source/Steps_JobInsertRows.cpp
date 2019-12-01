/*============================================================================*/
/* Steps_JobInsertRow.cpp - job step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobInsertRows(void)
{
  vAddStep("job_copyBuffer.copy"_Stz,              &Step::vJob_CopyBufferCopy);

  vAddStep("job_insertRow.partition@"_Stz,         &Step::vJob_InsertRowPartitionFetch);
  vAddStep("job_insertRow.newJobWriteRow"_Stz,     &Step::vJob_InsertRowNewJobWriteRow);
  vAddStep("job_insertRow.record!"_Stz,            &Step::vJob_InsertRowRecordStore);
  vAddStep("job_insertRow.spawnJobWriteRow"_Stz,   &Step::vJob_InsertRowSpawnJobWriteRow);
  
  vAddStep("Job_insertTable.subInserterJob@"_Stz,  &Step::vJob_InsertTableSubInserterJobFetch);
  
  vAddStep("job_subInserter.begin"_Stz,            &Step::vJob_SubInserterBegin);
  vAddStep("job_subInserter.end"_Stz,              &Step::vJob_SubInserterEnd);
}

/*----------------------------------------------------------------------------*/
/* job_copyBuffer.copy -- copy the record buffer into entryBuffer             */
/*----------------------------------------------------------------------------*/
void Step::vJob_CopyBufferCopy(void)
{
  ENTRY(); // ENTRY("vJob_CopyBufferCopy"_Stz);
  
  Object* pPtrCopyBuffer;
  m_pDataStack->vPopPtr(pPtrCopyBuffer,__func__);
  Job_CopyBuffer* pJob_CopyBuffer = _pJob_CopyBuffer(pPtrCopyBuffer);
  
  pJob_CopyBuffer->vCopy();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.newJobWriteRow -- new the writeRow job                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowNewJobWriteRow(void)
{
  ENTRY(); // ENTRY("vJob_InsertRowNewJobWriteRow"_Stz);
  
  Object* pJobPtr;
  m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
  
  Object* pTablePtr;
  m_pDataStack->vPopPtr(pTablePtr,__func__);
  Table* pTable = _pTable(pTablePtr);
  
  Session* pSession = pJob_InsertRow->pSession();
  Stz* sTableName = pJob_InsertRow->pEnameSet()->szValue("<tableNm>"_Stz);
  i8 iParentJobId = pJob_InsertRow->iJobId();
  Record* pRecord = pJob_InsertRow->m_pRecord;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // new a writeRow job
  
  Job_WriteRow* pJob_WriteRow = Job_WriteRow::pNewJobWriteRow(pSession,
                                                              sTableName,
                                                              pTable,
                                                              pRecord,
                                                              iParentJobId);
  
  m_pDataStack->vPushPtr(pJob_WriteRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_InsertRow.partition@ -- fetch partition id from job                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowPartitionFetch(void)
{
  ENTRY(); // ENTRY("vJob_InsertRowPartitionFetch"_Stz);
  
  Object* pJobPtr;
  m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
  
  m_pDataStack->vPushInt(pJob_InsertRow->m_iPartitionId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_InsertRow.record! -- store the record in the job                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowRecordStore(void)
{
  ENTRY(); // ENTRY("vJob_InsertRowRecordNull"_Stz);
  
  Object* pJobPtr;
  m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
  
  Object* pRecordObject;
  m_pDataStack->vPopPtr(pRecordObject,__func__);
  Record* pRecord = _pRecord(pRecordObject);
  
  pJob_InsertRow->m_pRecord = pRecord;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.spawnWriteRow -- spawn a writeRow job                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowSpawnJobWriteRow(void)
{
  ENTRY(); // ENTRY("vJob_WriteRowSpawnJobWriteRow"_Stz);
  
  Object* pJobInsertRowPtr;
  m_pDataStack->vPopPtr(pJobInsertRowPtr,__func__);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobInsertRowPtr);
  
  Object* pJobPtr;
  m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(pJobPtr);
  
  // spawn a writeRow job
  IGNORE_RETURN(Api::bSpawnJob(pJob_WriteRow));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_InsertTable.SubInserterJob@ -- fetch subInserter jobid from job        */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertTableSubInserterJobFetch(void)
{
  ENTRY(); // ENTRY("vJob_InsertTableSubInserterJobFetch"_Stz);
  
  Object* pJobPtr;
  m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pJobPtr);
  
  m_pDataStack->vPushPtr(pJob_InsertTable->m_pJobSubInserter);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_SubInserter.begin -- begin subInserter                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubInserterBegin(void)
{
  ENTRY(); // ENTRY("vJob_SubInserterBegin"_Stz);
  
  Object* pPtrSubInserter;
  m_pDataStack->vPopPtr(pPtrSubInserter,__func__);
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pPtrSubInserter);
  
  Object* pPtrTable;
  m_pDataStack->vPopPtr(pPtrTable,__func__);
  Table* pTable = _pTable(pPtrTable);
  
  pJob_SubInserter->vBegin(pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_SubInsert.end -- spawn last write buffers ~each partition              */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubInserterEnd(void)
{
  ENTRY(); // ENTRY("vJob_SubInserterEnd"_Stz);
  
  Object* pPtrSubInserter;
  m_pDataStack->vPopPtr(pPtrSubInserter,__func__);
  Job_SubInserter* pJob_SubInserter = _pJob_SubInserter(pPtrSubInserter);
  
  pJob_SubInserter->vEnd();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
