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
  
  i8 iInsertJobId = pJob_WriteBuffer->m_iEndInsertJobId;
  mpTask->m_pDataStack->vPushInt(iInsertJobId);
  
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
  
  // convert partition to node slot
  i8 iPartition = pJob_WriteBuffer->m_iPartitionId;
  i8 iSlot = TablePtn::iPartitionToSlot(iPartition);
  
  // get table, partition and working buffer
  Table* pTable = pJob_WriteBuffer->m_pTable;
  RASSERT_TYPE(pTable,CN_TABLE);
  
  TablePtn* pPartition = pTable->pPartition(iSlot);
  RASSERT_TYPE(pPartition,CN_TABLEPTN);
  
  WorkingBuffer* pWorkingBuffer = pJob_WriteBuffer->m_pCurrentBuffer;
  RASSERT_TYPE(pWorkingBuffer,CN_WORKINGBUFFER);
  
  i8 iTxId = 0;
  
  // insert the working buffer
  pTable->vInsertEntries(iTxId, iSlot, pWorkingBuffer);
  
  // delete the workingBuffer
  //delete pWorkingBuffer;
  
  // we inserted one additional buffer
  pPartition->vIncBuffersWritten();
  
  // does this job have information about the number of buffers loadable
  // from a subloader for this partition?
  i8 nWriteBehindBuffers = pJob_WriteBuffer->m_nWriteBehindBuffers;
  if (C_UNKNOWN != nWriteBehindBuffers) {
    pPartition->vWriteBehindBuffers(nWriteBehindBuffers);
    pPartition->vEndInsertJobId(pJob_WriteBuffer->m_iEndInsertJobId);
    if (pJob_WriteBuffer->m_bMark) {
      ActivityMap::vMark(pJob_WriteBuffer->m_iJobId,"Setting m_nWriteBehindBuffers in partition %lld slot %lld to %lld"_Stz,
                         iPartition, iSlot, nWriteBehindBuffers);
      //gpAttachMap->m_pOriginatingJobs->vMarkContents(pJob_InsertBuffer->m_iJobId);
      
      ActivityMap::vMark(pJob_WriteBuffer->m_iJobId,"Setting EndInsertJobId in partition %lld to %lld"_Stz,
                         iPartition, pJob_WriteBuffer->m_iEndInsertJobId);
      //gpAttachMap->m_pOriginatingJobs->vMarkContents(pJob_InsertBuffer->m_iJobId);
      
      //vTrace("Setting m_nWriteBehindBuffers in partition %lld slot %lld to %lld\n"_Stz,
      //                   iPartition, iSlot, nBuffersToBeInserted);
      //vTrace("Setting EndInsertJobId in partition %lld to %lld\n"_Stz,
      //                   iPartition, pJob_InsertBuffer->m_iEndInsertJobId);
    }
  }
  
  // have all buffers been loaded?
  bool bAllBuffersInserted = false;
  if ( C_UNKNOWN != pPartition->iEndInsertJobId() &&
      (pPartition->nWriteBehindBuffers() == pPartition->nBuffersWritten())) {
    //vTrace("Buffers inserted for partition %lld %lld\n"_Stz, iPartition, pPartition->nWriteBehindBuffers(), pPartition->iEndInsertJobId());
    pJob_WriteBuffer->m_iEndInsertJobId = pPartition->iEndInsertJobId();
    bAllBuffersInserted = true;
    
    if (pJob_WriteBuffer->m_bMark) {
      ActivityMap::vMark(pJob_WriteBuffer->m_iJobId,"All buffers inserted for partition %lld"_Stz, iPartition);
      //gpAttachMap->m_pOriginatingJobs->vMarkContents(pJob_InsertBuffer->m_iJobId);
    }
    
    // reset these in tblPtn for next insert into table
    pPartition->vWriteBehindBuffers(0);
    pPartition->vBuffersWritten(0);
    pPartition->vEndInsertJobId(-1);
  }
  
  mpTask->m_pDataStack->vPushInt(bAllBuffersInserted);
  
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
