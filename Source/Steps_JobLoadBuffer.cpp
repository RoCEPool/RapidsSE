/*============================================================================*/
/* Steps_JobLoadBuffer.cpp - job_loadBuffer step group                        */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobLoadBuffer(void)
{
  vAddStep("job_loadBuffer.insertRows"_Stz,      &Task::vJob_LoadBufferInsertRows);
  vAddStep("job_loadBuffer.jobLoadTableId@"_Stz, &Task::vJob_LoadBufferJobLoadTableIdFetch);
  vAddStep("job_loadBuffer.jobLoadTable!"_Stz,   &Task::vJob_LoadBufferJobLoadTableStore);
  vAddStep("job_loadBuffer.partition@"_Stz,      &Task::vJob_LoadBufferPartitionFetch);
  vAddStep("job_loadBuffer.table!"_Stz,          &Task::vJob_LoadBufferTableStore);
  
  vAddStep("job_loadBuffer::spawn"_Stz,          &Task::vJob_LoadBufferSpawn);
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.insertRows -- insert rows from working buffer into table    */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadBufferInsertRows(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferInsertRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.insertRows!"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  // convert partition to node slot
  i8 iPartition = pJob_LoadBuffer->iPartition();
  i8 iSlot = TablePtn::iPartitionToSlot(iPartition);

  // get table, partition and working buffer
  Table* pTable = pJob_LoadBuffer->pTable();
  TablePtn* pPartition = pTable->pPartition(iSlot);
  WorkingBuffer* pWorkingBuffer = pJob_LoadBuffer->m_pWorkingBuffer;
  
  vTrace("Job_LoadBuffer %lld %lld %s\n"_Stz,
         pWorkingBuffer->nWorkingRows(),
         pJob_LoadBuffer->m_nRows,
         pWorkingBuffer->bIsLast() ? "IsLast" : "");
  
  i8 iTxId = 0;
  
  // insert the working buffer
  pTable->vInsertEntries(iTxId, iSlot, pWorkingBuffer);
  
  // we loaded one additional buffer from a subloader
  pPartition->vIncTotalBuffersLoaded();
  
  // does this job have information about the number of buffers loadable
  // from a subloader for this partition?
  i8 nBuffersFromSubLoader = pJob_LoadBuffer->nBuffersFromSubLoader();
  if (C_UNKNOWN != nBuffersFromSubLoader)
    pPartition->vAddTotalBuffersLoadable(nBuffersFromSubLoader);
  
  // have all buffers been loaded?
  bool bAllBuffersLoaded = pPartition->bAllBuffersLoaded();
  //if (bAllBuffersLoaded)
  //  vTrace("Buffers loaded\n"_Stz);

  mpTask->m_pDataStack->vPushInt(bAllBuffersLoaded);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.jobLoadTableId@ -- fetch load table job id                  */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadBufferJobLoadTableIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferLoadJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.loadJobId@"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  i8 iJobId = pJob_LoadBuffer->iJobLoadTableId();
  mpTask->m_pDataStack->vPushInt(iJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.jobLoadTable! -- store job_loadTable                        */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadBufferJobLoadTableStore(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferJobLoadTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.jobLoadTable!"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.jobLoadTable!"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);

  pJob_LoadBuffer->vJobLoadTable(pJob_LoadTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.partition@ -- fetch partition no. from job_loadBuffer       */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadBufferPartitionFetch(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferPartitionFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.partition@"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  i8 i_Partition = pJob_LoadBuffer->iPartition();
  mpTask->m_pDataStack->vPushInt(i_Partition);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_loadBuffer::spawn -- spawn Job_LoadBuffer                              */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadBufferSpawn(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferSpawn"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_loadBuffer::spawn"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  i8 iFinal;
  if ( ! mpTask->m_pDataStack->bPop(iFinal) )
    THROW_INVALID_DSTACK("job_loadBuffer::spawn"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get info
  
  Job_LoadTable* pJob_LoadTable = pJob_SubLoader->pJob_LoadTable();
  
  i8 iJobLoadTableId = pJob_LoadTable->iJobId();
  i8 iPartition = pJob_SubLoader->m_iPartition;
  WorkingBuffer* pWorkingBuffer = pJob_SubLoader->m_pWorkingBuffer[iPartition];
  pJob_SubLoader->m_pWorkingBuffer[iPartition] = nullptr;
  
  Session* pSession = pJob_SubLoader->m_pSession;
  
  //ASSERT( (  pWorkingBuffer->bIsLast() && 0 == pWorkingBuffer->nWorkingRows()) ||
  //        ( !pWorkingBuffer->bIsLast() && pWorkingBuffer->nWorkingRows()));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a load buffer job
  
  //! this needs to be optimised
  JobClass* pJobClass_LoadBuffer = JobClass::pFind(CN_JOB_LOADBUFFER);
  
  Job_LoadBuffer* pJob_LoadBuffer =
    static_cast<Job_LoadBuffer*>(pJobClass_LoadBuffer->pPreJob(pSession));
  
  pJob_LoadBuffer->vSeed(SS_SEED_RUN);
  
  pJob_LoadBuffer->vJobLoadTableId(iJobLoadTableId);
  pJob_LoadBuffer->m_sObjectName = pJob_SubLoader->m_sObjectName;
  pJob_LoadBuffer->vPartition(iPartition);
  pJob_LoadBuffer->m_pWorkingBuffer = pWorkingBuffer;
  pJob_LoadBuffer->m_nRows = pWorkingBuffer->nWorkingRows();
  
  if (iFinal) {
    i8 nBuffersLoadable = pJob_SubLoader->m_nBuffersLoadable[iPartition];
    
    ASSERT(C_UNKNOWN != nBuffersLoadable);
    pJob_LoadBuffer->vBuffersFromSubLoader(nBuffersLoadable);
    
    pJob_LoadBuffer->m_bEndOfBuffers = true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn the load buffer job
  
  RapidsSE_bSpawnJob(pJob_LoadBuffer,SS_SEED_RUN,pSession);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.table! -- store table into job_loadBuffer                   */
/*----------------------------------------------------------------------------*/
void Task::vJob_LoadBufferTableStore(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferTableStore"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table!"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  Object* pTablePtr;
  if ( ! mpTask->m_pDataStack->bPop(pTablePtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.table!"_Stz);
  Table* pTable = _pTable(pTablePtr);
  
  pJob_LoadBuffer->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
