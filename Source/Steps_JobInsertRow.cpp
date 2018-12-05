/*============================================================================*/
/* Steps_JobInsertRow.cpp - job step group                                    */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobInsertRow(void)
{
  vAddStep("Job_InsertTable.insertRowJob@"_Stz,    &Step::vJob_InsertTableInsertRowJobFetch);
  vAddStep("Job_InsertTable.spawnLastBuffers"_Stz, &Step::vJob_InsertTableSpawnLastBuffers);
  vAddStep("job_insertRow.table!"_Stz,             &Step::vJob_InsertRowTableStore);
}

/*----------------------------------------------------------------------------*/
/* Job_InsertTable.InsertRowJob@ -- fetch insertRow jobid from job            */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertTableInsertRowJobFetch(MP_Task mpTask)
{
  ENTRY("vJob_InsertTableInsertRowJobFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_InsertTable.InsertRowJob@"_Stz);
  Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob_InsertTable->m_pJobInsertRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_InsertTable.spawnLastBuffers --spawn last write buffers ~each partition*/
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertTableSpawnLastBuffers(MP_Task mpTask)
{
  ENTRY("vJob_InsertTableSpawnLastInsert"_Stz);
  
  Object* pPtrInsertRow;
  if ( ! mpTask->m_pDataStack->bPop(pPtrInsertRow) )
    THROW_INVALID_DSTACK("Job_InsertTable.spawnLastBuffers"_Stz);
  Job_InsertTable* pJob_InsertTable = _pJob_InsertTable(pPtrInsertRow);
  
  // get the insert row job
  Job_InsertRow* pJob_InsertRow = pJob_InsertTable->m_pJobInsertRow;
  
  // get the session
  Session* pSession = pJob_InsertRow->m_pSession;
  
  
  i8 iParentJobId = pJob_InsertTable->iJobId();
  Stz* pTableName = pJob_InsertRow->m_sObjectName;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn a write buffer job for insert for each partition
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nPartitions; iPartition++) {
    
    // get the working buffer
    if ( ! pJob_InsertRow->m_pWorkingBuffer[iPartition] )
      pJob_InsertRow->vNewWorkingBuffer(pJob_InsertRow->m_pTable, iPartition);
    
    WorkingBuffer* pWorkingBuffer = pJob_InsertRow->m_pWorkingBuffer[iPartition];
    pJob_InsertRow->m_pWorkingBuffer[iPartition] = nullptr;
    
    i8 nWriteBehindBuffers = pJob_InsertRow->m_nWriteBehindBuffers[iPartition];
    
    Job_WriteBuffer::vSpawnJobWriteBuffer(
                                          pSession, pTableName, DS_INSERT,
                                          pWorkingBuffer, iParentJobId, iPartition,
                                          true, nWriteBehindBuffers);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.table! -- store table into job_insertRow                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowTableStore(MP_Task mpTask)
{
  ENTRY("vJobInsertRowTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
  THROW_INVALID_DSTACK("job_InsertRow.table!"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
  THROW_INVALID_DSTACK("job_InsertRow.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_InsertRow->m_pTable = pTable;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
