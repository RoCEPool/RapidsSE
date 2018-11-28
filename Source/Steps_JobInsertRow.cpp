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
  vAddStep("job_endInsert.insertRowJob@"_Stz, &Step::vJob_EndInsertInsertRowJobFetch);

  vAddStep("job_insertBuffer.endInsertJob@"_Stz, &Step::vJob_InsertBufferEndInsertJobIdFetch);
  vAddStep("job_insertBuffer.InsertRows"_Stz,    &Step::vJob_InsertBufferInsertRows);
  vAddStep("job_insertBuffer.partition@"_Stz,    &Step::vJob_InsertBufferPartitionFetch);
  vAddStep("job_insertBuffer.table!"_Stz,        &Step::vJob_InsertBufferTableStore);
  
  vAddStep("job_InsertRow.danglingBuffer"_Stz,   &Step::vJob_InsertRowDanglingBuffer);
  vAddStep("job_insertRow.partition!"_Stz,       &Step::vJob_InsertRowPartitionStore);
  vAddStep("job_insertRow.table!"_Stz,           &Step::vJob_InsertRowTableStore);
  vAddStep("job_InsertRow.SpawnInsertBuffer"_Stz,&Step::vJob_InsertRowSpawnInsertBuffer);
  
}

/*----------------------------------------------------------------------------*/
/* job_EndInsert.InsertRowJob@ -- fetch insertRow jobid from job              */
/*----------------------------------------------------------------------------*/
void Step::vJob_EndInsertInsertRowJobFetch(MP_Task mpTask)
{
  ENTRY("vJob_EndInsertInsertRowJobFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_EndInsert.InsertRowJob@"_Stz);
  Job_EndInsert* pJob_EndInsert = _pJob_EndInsert(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob_EndInsert->m_pJobInsertRow);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertBuffer.endInsertJob@ -- fetch endInsert jobid from job           */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertBufferEndInsertJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_InsertBufferEndInsertJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertBuffer.endInsertJob@"_Stz);
  Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJobPtr);
  
  i8 iInsertJobId = pJob_InsertBuffer->m_iEndInsertJobId;
  mpTask->m_pDataStack->vPushInt(iInsertJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertBuffer.insertRows -- insert rows from working buffer into table  */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertBufferInsertRows(MP_Task mpTask)
{
  ENTRY("vJobInsertBufferInsertRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertBuffer.insertRows!"_Stz);
  Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJobPtr);
  
  // convert partition to node slot
  i8 iPartition = pJob_InsertBuffer->m_iPartitionId;
  i8 iSlot = TablePtn::iPartitionToSlot(iPartition);
  
  // get table, partition and working buffer
  Table* pTable = pJob_InsertBuffer->m_pTable;
  RASSERT_TYPE(pTable,CN_TABLE);
  
  TablePtn* pPartition = pTable->pPartition(iSlot);
  RASSERT_TYPE(pPartition,CN_TABLEPTN);
  
  WorkingBuffer* pWorkingBuffer = pJob_InsertBuffer->m_pCurrentBuffer;
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
  i8 nWriteBehindBuffers = pJob_InsertBuffer->m_nWriteBehindBuffers;
  if (C_UNKNOWN != nWriteBehindBuffers) {
    pPartition->vWriteBehindBuffers(nWriteBehindBuffers);
    pPartition->vEndInsertJobId(pJob_InsertBuffer->m_iEndInsertJobId);
    if (pJob_InsertBuffer->m_bMark) {
      ActivityMap::vMark(pJob_InsertBuffer->m_iJobId,"Setting m_nWriteBehindBuffers in partition %lld slot %lld to %lld"_Stz,
                         iPartition, iSlot, nWriteBehindBuffers);
      //gpAttachMap->m_pOriginatingJobs->vMarkContents(pJob_InsertBuffer->m_iJobId);
      
      ActivityMap::vMark(pJob_InsertBuffer->m_iJobId,"Setting EndInsertJobId in partition %lld to %lld"_Stz,
                         iPartition, pJob_InsertBuffer->m_iEndInsertJobId);
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
    pJob_InsertBuffer->m_iEndInsertJobId = pPartition->iEndInsertJobId();
    bAllBuffersInserted = true;
    
    if (pJob_InsertBuffer->m_bMark) {
      ActivityMap::vMark(pJob_InsertBuffer->m_iJobId,"All buffers inserted for partition %lld"_Stz, iPartition);
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
/* job_InsertBuffer.partition@ -- fetch partition no. from job_insertBuffer   */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertBufferPartitionFetch(MP_Task mpTask)
{
  ENTRY("vJob_InsertBufferPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertBuffer.partition@"_Stz);
  Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJobPtr);
  
  i8 i_Partition = pJob_InsertBuffer->m_iPartitionId;
  mpTask->m_pDataStack->vPushInt(i_Partition);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_InsertBuffer.table! -- store table into job_insertBuffer               */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertBufferTableStore(MP_Task mpTask)
{
  ENTRY("vJob_InsertBufferTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertBuffer.table!"_Stz);
  Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("job_InsertBuffer.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_InsertBuffer->m_pTable = pTable;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.danglingBuffer--ensure there's a dangling buffer in partition */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowDanglingBuffer(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowDanglingBuffer"_Stz);
  
  Object* pPtrInsertRow;
  if ( ! mpTask->m_pDataStack->bPop(pPtrInsertRow) )
    THROW_INVALID_DSTACK("job_InsertRow.danglingBuffer"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pPtrInsertRow);
  
  if ( ! pJob_InsertRow->m_pWorkingBuffer[pJob_InsertRow->m_iPartition] )
    pJob_InsertRow->vNewWorkingBuffer(pJob_InsertRow->m_pTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.partition! -- store partition into job_insertRow             */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowPartitionStore(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowPartitionStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_InsertRow.partition!"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("job_InsertRow.partition!"_Stz);
  
  pJob_InsertRow->m_iPartition = iPartition;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.spawnInsertBuffer -- spawn Job_InsertBuffer                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_InsertRowSpawnInsertBuffer(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowSpawnInsertBuffer"_Stz);
  
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
  JobClass* pJobClass_InsertBuffer = JobClass::pFind(CN_JOB_INSERTBUFFER);
  
  Job_InsertBuffer* pJob_InsertBuffer =
  static_cast<Job_InsertBuffer*>(pJobClass_InsertBuffer->pPreJob(pSession));
  
  pJob_InsertBuffer->vSeed(SS_SEED_RUN);
  
  pJob_InsertBuffer->m_iEndInsertJobId = iJob_EndJobid;
  pJob_InsertBuffer->m_sObjectName = pJob_InsertRow->m_sObjectName;
  pJob_InsertBuffer->m_iPartitionId = iPartition;
  pJob_InsertBuffer->m_pCurrentBuffer = pWorkingBuffer;
  
  if (iIsFinal) {
    i8 nWriteBehindBuffers = pJob_InsertRow->m_nWriteBehindBuffers[iPartition];
    RASSERT(C_UNKNOWN != nWriteBehindBuffers);
    pJob_InsertBuffer->m_nWriteBehindBuffers = nWriteBehindBuffers;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn the load buffer job
  
  RapidsSE_bSpawnJob(pJob_InsertBuffer,SS_SEED_RUN,pSession);
  
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
