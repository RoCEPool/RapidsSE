/*============================================================================*/
/* Steps_JobInsertRow.cpp - job step group                                    */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobInsertRow(void)
{
  vAddStep("job_endInsert.insertRowJob@"_Stz, &Task::vJob_EndInsertInsertRowJobIdFetch, false);
  vAddStep("job_endInsert.task@"_Stz,         &Task::vJob_EndInsertTaskFetch,           false);
  vAddStep("job_endInsert.task!"_Stz,         &Task::vJob_EndInsertTaskStore,           false);

  vAddStep("job_insertBuffer.endInsertJob@"_Stz, &Task::vJob_InsertBufferEndInsertJobIdFetch, false);
  vAddStep("job_insertBuffer.InsertRows"_Stz,    &Task::vJob_InsertBufferInsertRows,          false);
  vAddStep("job_insertBuffer.partition@"_Stz,    &Task::vJob_InsertBufferPartitionFetch,      false);
  vAddStep("job_insertBuffer.table!"_Stz,        &Task::vJob_InsertBufferTableStore,          false);
  
  vAddStep("job_insertRow.table!"_Stz,           &Task::vJob_InsertRowTableStore,        false);
  vAddStep("job_InsertRow.danglingBuffer"_Stz,   &Task::vJob_InsertRowDanglingBuffer,    false);
  vAddStep("job_InsertRow.SpawnInsertBuffer"_Stz,&Task::vJob_InsertRowSpawnInsertBuffer, false);
  
}

/*----------------------------------------------------------------------------*/
/* job_EndInsert.InsertRowJob@ -- fetch insertRow jobid from job              */
/*----------------------------------------------------------------------------*/
void Task::vJob_EndInsertInsertRowJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_EndInsertInsertRowJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_EndInsert.InsertRowJob@"_Stz);
  Job_EndInsert* pJob_EndInsert = _pJob_EndInsert(pJobPtr);
  
  i8 iInsertJobId = pJob_EndInsert->m_iInsertRowJobId;
  mpTask->m_pDataStack->vPushInt(iInsertJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_EndInsert.task@ -- fetch task from job_endInsert                       */
/*----------------------------------------------------------------------------*/
void Task::vJob_EndInsertTaskFetch(MP_Task mpTask)
{
  ENTRY("vJob_EndInsertTaskFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_EndInsert.task@"_Stz);
  Job_EndInsert* pJob_EndInsert = _pJob_EndInsert(pJobPtr);
  
  Task* pTask = pJob_EndInsert->m_pTask;
  mpTask->m_pDataStack->vPushObject(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_EndInsert.task! -- store task from job_endInsert                       */
/*----------------------------------------------------------------------------*/
void Task::vJob_EndInsertTaskStore(MP_Task mpTask)
{
  ENTRY("vJob_EndInsertTaskStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_EndInsert.task!"_Stz);
  Job_EndInsert* pJob_EndInsert = _pJob_EndInsert(pJobPtr);
  
  Object* pTaskPtr;
  if ( ! mpTask->m_pDataStack->bPop(pTaskPtr) )
    THROW_INVALID_DSTACK("job_EndInsert.task!"_Stz);
  Task* pTask = _pTask(pTaskPtr);
  
  pJob_EndInsert->m_pTask = pTask;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertBuffer.endInsertJob@ -- fetch endInsert jobid from job           */
/*----------------------------------------------------------------------------*/
void Task::vJob_InsertBufferEndInsertJobIdFetch(MP_Task mpTask)
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
void Task::vJob_InsertBufferInsertRows(MP_Task mpTask)
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
  TablePtn* pPartition = pTable->pPartition(iSlot);
  WorkingBuffer* pWorkingBuffer = pJob_InsertBuffer->m_pWorkingBuffer;
  
  i8 iTxId = 0;
  
  // insert the working buffer
  pTable->vInsertEntries(iTxId, iSlot, pWorkingBuffer);
  
  // we inserted one additional buffer
  pPartition->vIncBuffersInserted();
  
  // does this job have information about the number of buffers loadable
  // from a subloader for this partition?
  i8 nBuffersToBeInserted = pJob_InsertBuffer->m_nBuffersToBeInserted;
  if (C_UNKNOWN != nBuffersToBeInserted)
    pPartition->vBuffersToBeInserted(nBuffersToBeInserted);
  
  // have all buffers been loaded?
  bool bAllBuffersInserted = false;
  if ( pPartition->nBuffersToBeInserted() &&
      (pPartition->nBuffersToBeInserted() == pPartition->nBuffersInserted())) {
    vTrace("Buffers inserted for partition %lld\n"_Stz, iPartition);
    bAllBuffersInserted = true;
    
    // reset these in tblPtn for next insert into table
    pPartition->vBuffersToBeInserted(0);
    pPartition->vBuffersInserted(0);
  }
  
  mpTask->m_pDataStack->vPushInt(bAllBuffersInserted);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertBuffer.partition@ -- fetch partition no. from job_insertBuffer   */
/*----------------------------------------------------------------------------*/
void Task::vJob_InsertBufferPartitionFetch(MP_Task mpTask)
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
void Task::vJob_InsertBufferTableStore(MP_Task mpTask)
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
void Task::vJob_InsertRowDanglingBuffer(MP_Task mpTask)
{
  ENTRY("vJob_InsertRowDanglingBuffer"_Stz);
  
  Object* pPtrInsertRow;
  if ( ! mpTask->m_pDataStack->bPop(pPtrInsertRow) )
    THROW_INVALID_DSTACK("job_InsertRow.danglingBuffer"_Stz);
  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pPtrInsertRow);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iPartition) )
    THROW_INVALID_DSTACK("job_InsertRow.danglingBuffer"_Stz);
  
  if ( ! pJob_InsertRow->m_pWorkingBuffer[iPartition] )
    pJob_InsertRow->vNewWorkingBuffer(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_InsertRow.spawnInsertBuffer -- spawn Job_InsertBuffer                  */
/*----------------------------------------------------------------------------*/
void Task::vJob_InsertRowSpawnInsertBuffer(MP_Task mpTask)
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
  SEitf* pSEitf = pJob_InsertRow->m_pSEitf;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a load buffer job
  
  //! this needs to be optimised
  JobClass* pJobClass_InsertBuffer = JobClass::pFind(CN_JOB_INSERTBUFFER);
  
  Job_InsertBuffer* pJob_InsertBuffer =
  static_cast<Job_InsertBuffer*>(pJobClass_InsertBuffer->pPreJob(pSEitf));
  
  pJob_InsertBuffer->vSeed(SS_SEED_RUN);
  
  pJob_InsertBuffer->m_iEndInsertJobId = iJob_EndJobid;
  pJob_InsertBuffer->m_sObjectName = pJob_InsertRow->m_sObjectName;
  pJob_InsertBuffer->m_iPartitionId = iPartition;
  pJob_InsertBuffer->m_pWorkingBuffer = pWorkingBuffer;
  
  if (iIsFinal) {
    i8 nBuffersToBeInserted = pJob_InsertRow->m_nBuffersToBeInserted[iPartition];
    ASSERT(C_UNKNOWN != nBuffersToBeInserted);
    pJob_InsertBuffer->m_nBuffersToBeInserted = nBuffersToBeInserted;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn the load buffer job
  
  RapidsSE_bSpawnJob(pJob_InsertBuffer,SS_SEED_RUN,pSEitf);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_InsertRow.table! -- store table into job_insertRow                     */
/*----------------------------------------------------------------------------*/
void Task::vJob_InsertRowTableStore(MP_Task mpTask)
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
