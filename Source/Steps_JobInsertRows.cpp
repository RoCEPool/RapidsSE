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
  vAddStep("Job_InsertTable.insertRowsJob@"_Stz,    &Step::vJob_InsertTableInsertRowsJobFetch);
  vAddStep("Job_InsertTable.spawnLastBuffs"_Stz,    &Step::vJob_InsertTableSpawnLastBuffers);
  vAddStep("job_insertRows.table!"_Stz,             &Step::vJob_InsertRowsTableStore);
  
  vAddStep("job_insertRow.writeRow"_Stz,            &Step::vJob_InsertRowWriteRow);
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
/* Job_InsertTable.spawnLastBuffs --spawn last write buffers ~each partition*/
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertTableSpawnLastBuffers(MP_Task mpTask)
{
  ENTRY("vJob_InsertTableSpawnLastInsert"_Stz);
  
  Object* pPtrInsertRow;
  if ( ! mpTask->m_pDataStack->bPop(pPtrInsertRow) )
    THROW_INVALID_DSTACK("Job_InsertTable.spawnLastBuffs"_Stz);
  Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pPtrInsertRow);
  
  // get the insert row job
  Job_InsertRows* pJob_InsertRows = pJob_InsertTable->m_pJobInsertRows;
  
  // get the session
  Session* pSession = pJob_InsertRows->m_pSession;
  
  
  i8 iParentJobId = pJob_InsertTable->iJobId();
  Stz* pTableName = pJob_InsertRows->m_sObjectName;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn a write buffer job for insert for each partition
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nPartitions; iPartition++) {
    
    // get the working buffer
    if ( ! pJob_InsertRows->m_pWorkingBuffer[iPartition] )
      pJob_InsertRows->vNewWorkingBuffer(pJob_InsertRows->m_pTable, iPartition);
    
    WorkingBuffer* pWorkingBuffer = pJob_InsertRows->m_pWorkingBuffer[iPartition];
    pJob_InsertRows->m_pWorkingBuffer[iPartition] = nullptr;
    
    i8 nWriteBehindBuffers = pJob_InsertRows->m_nWriteBehindBuffers[iPartition];
    
    Job_WriteBuffer::vSpawnJobWriteBuffer(
                                          pSession, pTableName, DS_INSERT,
                                          pWorkingBuffer, iParentJobId, iPartition,
                                          true, nWriteBehindBuffers);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRows.table! -- store table into job_insertRows                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowsTableStore(MP_Task mpTask)
{
  ENTRY("vJobInsertRowsTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
  THROW_INVALID_DSTACK("job_InsertRows.table!"_Stz);
  Job_InsertRows* pJob_InsertRows = _pJob_InsertRows(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
  THROW_INVALID_DSTACK("job_InsertRows.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_InsertRows->m_pTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.writeRow -- write row into the table                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowWriteRow(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowWriteRow"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertRow.writeRow!"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("job_InsertRow.writeRow!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // convert partition to node slot
  i8 iSlot = TablePtn::iPartitionToSlot(pJob_InsertRow->m_iPartitionId);
  
  TablePtn* pPartition = pTable->pPartition(iSlot);
  RASSERT_TYPE(pPartition,CN_TABLEPTN);
  
  i8 iTxId = 0;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert the row
  pTable->vInsertRow(iTxId, iSlot, pJob_InsertRow->m_aRecord, pJob_InsertRow->m_nRecordBytes);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
