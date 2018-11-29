/*============================================================================*/
/* Steps_JobLoadBuffer.cpp - job_loadBuffer step group                        */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobLoadBuffer(void)
{
  vAddStep("job_loadBuffer::spawn"_Stz,          &Step::vJob_LoadBufferSpawn);
  
  vAddStep("job_loadBuffer.insertRows"_Stz,      &Step::vJob_LoadBufferInsertRows);
  vAddStep("job_loadBuffer.parentJobId@"_Stz,    &Step::vJob_LoadBufferParentJobIdFetch);
  vAddStep("job_loadBuffer.partition@"_Stz,      &Step::vJob_LoadBufferPartitionFetch);
  vAddStep("job_loadBuffer.rows@"_Stz,           &Step::vJob_LoadBufferRowsFetch);
  vAddStep("job_loadBuffer.table!"_Stz,          &Step::vJob_LoadBufferTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.insertRows -- insert rows from working buffer into table    */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadBufferInsertRows(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferInsertRows"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.insertRows!"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  bool bAllBuffersLoaded = pJob_LoadBuffer->bInsertRows();

  mpTask->m_pDataStack->vPushInt(bAllBuffersLoaded);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.parentJobId@ -- fetch load table job id                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadBufferParentJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferParentJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.parentJobId@"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  i8 iJobId = pJob_LoadBuffer->m_iParentJobId;
  mpTask->m_pDataStack->vPushInt(iJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.partition@ -- fetch partition no. from job_loadBuffer       */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadBufferPartitionFetch(MP_Task mpTask)
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
/* job_LoadBuffer.rows@ -- fetch rows from job_loadBuffer                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadBufferRowsFetch(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferRowsFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_LoadBuffer.rows@"_Stz);
  Job_LoadBuffer* pJob_LoadBuffer = _pJob_LoadBuffer(pJobPtr);
  
  i8 nRows = pJob_LoadBuffer->m_nRows;
  mpTask->m_pDataStack->vPushInt(nRows);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_loadBuffer::spawn -- spawn Job_LoadBuffer                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadBufferSpawn(MP_Task mpTask)
{
  ENTRY("vJob_LoadBufferSpawn"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_loadBuffer::spawn"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  i8 iFinal;
  if ( ! mpTask->m_pDataStack->bPop(iFinal) )
    THROW_INVALID_DSTACK("job_loadBuffer::spawn"_Stz);
  
  bool bFinal = iFinal ? true : false;
 
  Session* pSession = pJob_SubLoader->m_pSession;

  Job_LoadBuffer* pJob_LoadBuffer =
    Job_LoadBuffer::pNewJobLoadBuffer(pJob_SubLoader,bFinal);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn the load buffer job
  
  RapidsSE_bSpawnJob(pJob_LoadBuffer,SS_SEED_RUN,pSession);
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* job_LoadBuffer.table! -- store table into job_loadBuffer                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_LoadBufferTableStore(MP_Task mpTask)
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
