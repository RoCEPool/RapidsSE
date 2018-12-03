/*============================================================================*/
/* Steps_JobInsertRow.cpp - job step group                                    */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobWriteBuffer(void)
{
  vAddStep("job_writeBuffer.parentJobId@"_Stz,  &Step::vJob_WriteBufferParentJobIdFetch);
  vAddStep("job_writeBuffer.InsertRows"_Stz,    &Step::vJob_WriteBufferInsertRows);
  vAddStep("job_writeBuffer.isIngesting"_Stz,   &Step::vJob_WriteBufferIsIngesting);
  vAddStep("job_writeBuffer.isInserting"_Stz,   &Step::vJob_WriteBufferIsInserting);
  vAddStep("job_writeBuffer.isLoading"_Stz,     &Step::vJob_WriteBufferIsLoading);
  vAddStep("job_writeBuffer.partition@"_Stz,    &Step::vJob_WriteBufferPartitionFetch);
  vAddStep("job_writeBuffer::spawnInsert"_Stz,  &Step::vJob_WriteBufferSpawnInsert);
  vAddStep("job_writeBuffer::spawnLoad"_Stz,    &Step::vJob_WriteBufferSpawnLoad);
  vAddStep("job_writeBuffer.table!"_Stz,        &Step::vJob_WriteBufferTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.parentJobId@ -- fetch parent jobid from job                */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferParentJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_WriteBufferParentJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteBuffer.parentJobId@"_Stz);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  i8 iParentJobId = pJob_WriteBuffer->m_iParentJobId;
  RASSERT(C_UNKNOWN != iParentJobId);
  mpTask->m_pDataStack->vPushInt(iParentJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.insertRows -- insert rows from working buffer into table  */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferInsertRows(MP_Task mpTask)
{
  ENTRY("vJob_WriteBufferInsertRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteBuffer.insertRows!"_Stz);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  bool bAllBuffersWritten = pJob_WriteBuffer->bInsertRows();
  
  mpTask->m_pDataStack->vPushInt(bAllBuffersWritten);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.isIngesting -- is writebuffer being ingesting              */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferIsIngesting(MP_Task mpTask)
{
  ENTRY("vJob_WriteBufferIsIngesting"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteBuffer.isIngesting"_Stz);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  eDS eDataSource = pJob_WriteBuffer->m_eDataSource;
  
  mpTask->m_pDataStack->vPushInt(eDataSource == DS_INGEST ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.isInserting -- is writebuffer being inserted               */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferIsInserting(MP_Task mpTask)
{
  ENTRY("vJob_WriteBufferIsInserting"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteBuffer.isInserting"_Stz);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  eDS eDataSource = pJob_WriteBuffer->m_eDataSource;
  
  mpTask->m_pDataStack->vPushInt(eDataSource == DS_INSERT ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.isLoading -- is writebuffer being loaded                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferIsLoading(MP_Task mpTask)
{
  ENTRY("vJob_WriteBufferIsLoading"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteBuffer.isLoading"_Stz);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  eDS eDataSource = pJob_WriteBuffer->m_eDataSource;
  
  mpTask->m_pDataStack->vPushInt(eDataSource == DS_LOAD ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.partition@ -- fetch partition no. from job_writeBuffer     */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferPartitionFetch(MP_Task mpTask)
{
  ENTRY("vJob_WriteBufferPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteBuffer.partition@"_Stz);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  i8 i_Partition = pJob_WriteBuffer->m_iPartitionId;
  mpTask->m_pDataStack->vPushInt(i_Partition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer::spawnInsert -- spawn a write buffer for insert            */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferSpawnInsert(MP_Task mpTask)
{
  ENTRY("vJob_WriteBufferSpawnInsert"_Stz);
  
  Object* pPtrInsertRow;
  if ( ! mpTask->m_pDataStack->bPop(pPtrInsertRow) )
    THROW_INVALID_DSTACK("job_WriteBuffer::spawnInsert"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pPtrInsertRow);
  
  i8 iFinal;
  if ( ! mpTask->m_pDataStack->bPop(iFinal) )
    THROW_INVALID_DSTACK("job_WriteBuffer::spawnInsert"_Stz);
  
  bool bFinal = iFinal ? true : false;
  
  // get the session
  Session* pSession = pJob_InsertRow->m_pSession;
  
  // get the insert table
  Job_InsertTable* pJob_InsertTable = pJob_InsertRow->m_pJob_InsertTable;
  
  // get the working buffer
  i8 iPartition = pJob_InsertRow->m_iPartition;
  WorkingBuffer* pWorkingBuffer = pJob_InsertRow->m_pWorkingBuffer[iPartition];
  pJob_InsertRow->m_pWorkingBuffer[iPartition] = nullptr;
  
  i8 iParentJobId = pJob_InsertTable->iJobId();
  i8 nWriteBehindBuffers = pJob_InsertRow->m_nWriteBehindBuffers[iPartition];
  Stz* pTableName = pJob_InsertRow->m_sObjectName;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a write buffer job for insert
  Job_WriteBuffer* pJob_WriteBuffer = Job_WriteBuffer::pNewJobWriteBuffer(
                                        pSession, pTableName, DS_INSERT,
                                        pWorkingBuffer, iParentJobId, iPartition,
                                        bFinal, nWriteBehindBuffers);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn the write buffer job
  
  RapidsSE_bSpawnJob(pJob_WriteBuffer,SS_SEED_RUN,pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer::spawnLoad -- spawn a write buffer for load                */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferSpawnLoad(MP_Task mpTask)
{
  ENTRY("vJob_WriteBufferSpawnLoad"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_WriteBuffer::spawnLoad"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  i8 iFinal;
  if ( ! mpTask->m_pDataStack->bPop(iFinal) )
    THROW_INVALID_DSTACK("job_WriteBuffer::spawnLoad"_Stz);
  
  bool bFinal = iFinal ? true : false;
  
  // get the session
  Session* pSession = pJob_SubLoader->m_pSession;
  
  // get the load table
  Job_LoadTable* pJob_LoadTable = pJob_SubLoader->pJob_LoadTable();
  
  i8 iPartition = pJob_SubLoader->m_iPartition;
  WorkingBuffer* pWorkingBuffer = pJob_SubLoader->m_pWorkingBuffer[iPartition];
  pJob_SubLoader->m_pWorkingBuffer[iPartition] = nullptr;
  
  i8 iParentJobId = pJob_LoadTable->iJobId();
  i8 nWriteBehindBuffers = pJob_SubLoader->m_nWriteBehindBuffers[iPartition];
  Stz* sTableName = pJob_SubLoader->m_sObjectName;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a write buffer job for load
  Job_WriteBuffer* pJob_WriteBuffer = Job_WriteBuffer::pNewJobWriteBuffer(
                                          pSession, sTableName, DS_LOAD,
                                          pWorkingBuffer, iParentJobId, iPartition,
                                          bFinal, nWriteBehindBuffers);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn the write buffer job
  
  RapidsSE_bSpawnJob(pJob_WriteBuffer,SS_SEED_RUN,pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteBuffer.table! -- store table into job_writeBuffer                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferTableStore(MP_Task mpTask)
{
  ENTRY("vJob_WriteBufferTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_WriteBuffer.table!"_Stz);
  Job_WriteBuffer* pJob_WriteBuffer = _pJob_WriteBuffer(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("job_WriteBuffer.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_WriteBuffer->m_pTable = pTable;
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
