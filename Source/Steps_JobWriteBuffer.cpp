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
  vAddStep("job_writeBuffer.partition@"_Stz,    &Step::vJob_WriteBufferPartitionFetch);
  vAddStep("job_writeBuffer::spawn"_Stz,        &Step::vJob_WriteBufferSpawn);
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
  
  bool bAllBuffersLoaded = pJob_WriteBuffer->bInsertRows();
  
  mpTask->m_pDataStack->vPushInt(bAllBuffersLoaded);
  
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
/* job_WriteBuffer::spawn -- spawn a write buffer                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteBufferSpawn(MP_Task mpTask)
{
  ENTRY("vJob_WriteBufferSpawn"_Stz);
  
  Object* pPtrInsertRow;
  if ( ! mpTask->m_pDataStack->bPop(pPtrInsertRow) )
    THROW_INVALID_DSTACK("job_InsertRow.spawnInsertBuffer!"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pPtrInsertRow);
  
  i8 iJob_EndJobid;
  if ( ! mpTask->m_pDataStack->bPop(iJob_EndJobid) )
    THROW_INVALID_DSTACK("job_InsertRow.spawnInsertBuffer"_Stz);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("job_InsertRow.spawnInsertBuffer"_Stz);
  
  i8 iIsFinal;
  if ( ! mpTask->m_pDataStack->bPop(iIsFinal) )
    THROW_INVALID_DSTACK("job_InsertRow.spawnInsertBuffer"_Stz);
  
  WorkingBuffer* pWorkingBuffer = pJob_InsertRow->m_pWorkingBuffer[iPartition];
  pJob_InsertRow->m_pWorkingBuffer[iPartition] = nullptr;
  Session* pSession = pJob_InsertRow->m_pSession;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a insert buffer job
  
  //! this needs to be optimised
  JobClass* pJobClass_WriteBuffer = JobClass::pFind(CN_JOB_WRITEBUFFER);
  
  Job_WriteBuffer* pJob_WriteBuffer =
  static_cast<Job_WriteBuffer*>(pJobClass_WriteBuffer->pPreJob(pSession));
  
  pJob_WriteBuffer->vSeed(SS_SEED_RUN);
  
  pJob_WriteBuffer->m_eDataSource  = DS_INSERT;
  pJob_WriteBuffer->m_iParentJobId = iJob_EndJobid;
  pJob_WriteBuffer->m_sObjectName  = pJob_InsertRow->m_sObjectName;
  pJob_WriteBuffer->m_iPartitionId = iPartition;
  pJob_WriteBuffer->m_pCurrentBuffer = pWorkingBuffer;
  
  if (iIsFinal) {
    i8 nWriteBehindBuffers = pJob_InsertRow->m_nWriteBehindBuffers[iPartition];
    RASSERT(C_UNKNOWN != nWriteBehindBuffers);
    pJob_WriteBuffer->m_nWriteBehindBuffers = nWriteBehindBuffers;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn the load buffer job
  
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
