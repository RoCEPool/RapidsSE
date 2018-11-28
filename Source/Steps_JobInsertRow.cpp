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
  JobClass* pJobClass_WriteBuffer = JobClass::pFind(CN_JOB_WRITEBUFFER);
  
  Job_WriteBuffer* pJob_WriteBuffer =
  static_cast<Job_WriteBuffer*>(pJobClass_WriteBuffer->pPreJob(pSession));
  
  pJob_WriteBuffer->vSeed(SS_SEED_RUN);
  
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
