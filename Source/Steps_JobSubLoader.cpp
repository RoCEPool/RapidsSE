/*============================================================================*/
/* Steps_JobSubLoader.cpp - job_subLoader step group                          */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSubLoader(void)
{
  vAddStep("Job_subLoader::spawn"_Stz,        &Step::vJob_SubLoaderSpawn);
  
  vAddStep("job_subLoader.begin"_Stz,         &Step::vJob_SubLoaderBegin);
  vAddStep("job_subLoader.end"_Stz,           &Step::vJob_SubLoaderEnd);
  vAddStep("Job_subLoader.jobLoadTable@"_Stz, &Step::vJob_SubLoaderJobLoadTableFetch);
  vAddStep("Job_subLoader.processRecords"_Stz,&Step::vJob_SubLoaderProcessRecords);
  vAddStep("Job_subLoader.spawnLastBuffers"_Stz,&Step::vJob_SubLoaderSpawnLastBuffers);
  vAddStep("Job_subLoader.recordsInvalid"_Stz,&Step::vJob_SubLoaderRecordsInvalid);

  vAddStep("Job_subLoader.workingBuffer"_Stz, &Step::vJob_SubLoaderWorkingBuffer);
}

/*----------------------------------------------------------------------------*/
/* job_subLoader::spawn -- spawn a new subloader                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderSpawn(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderSpawn"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("job_subLoader::spawn"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = pJob_LoadTable->m_pSession;
  
  //! this needs to be optimised
  JobClass* pJobClass_SubLoader = JobClass::pFind(CN_JOB_SUBLOADER);
  
  Job_SubLoader* pJob_SubLoader =
  static_cast<Job_SubLoader*>(pJobClass_SubLoader->pPreJob(pSession));
  
  pJob_SubLoader->vSeed(SS_SEED_RUN);
  
  pJob_SubLoader->m_sObjectName = pJob_LoadTable->m_sObjectName;
  pJob_SubLoader->vJob_LoadTable(pJob_LoadTable);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RapidsSE_bSpawnJob(pJob_SubLoader,SS_SEED_RUN,pSession);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job_subLoader.begin -- begin loading                                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderBegin(MP_Task mpTask)
{
  ENTRY("vSubLoaderBegin"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_subLoader.begin"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);

  pJob_SubLoader->vBegin();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.end -- end loading                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderEnd(MP_Task mpTask)
{
  ENTRY("vSubLoaderEnd"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("subLoader.end"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  pJob_SubLoader->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.jobLoadTable@ -- fetch the job load table                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderJobLoadTableFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderJobLoadTableFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subLoader.jobLoadTable@"_Stz);
  Job_SubLoader* pJob = _pJob_SubLoader(pJobPtr);
  
  mpTask->m_pDataStack->vPushObject(pJob->m_pJob_LoadTable);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.processRecords -- read & process rows till buffer full / eof */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderProcessRecords(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderProcessRecords"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_subLoader.processRecords"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
//  bool bTargetBufferFull = pJob_SubLoader->bProcessRecords();
//
//  mpTask->m_pDataStack->vPushInt(bTargetBufferFull);
  
  pJob_SubLoader->bProcessRecords();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.spawnLastBuffers --spawn last write buffers ~ each partition */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderSpawnLastBuffers(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderSpawnLastBuffers"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_subLoader.spawnLastBuffers::spawnLastLoad"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  // get the session
  Session* pSession = pJob_SubLoader->m_pSession;
  
  // get the load table
  Job_LoadTable* pJob_LoadTable = pJob_SubLoader->pJob_LoadTable();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn a write buffer job for insert for each partition
  i8 iParentJobId = pJob_LoadTable->iJobId();
  Stz* sTableName = pJob_SubLoader->m_sObjectName;
  
  for (i8 iPartition = 0; iPartition < gpAttachMap->m_pGrid->m_nPartitions; iPartition++) {
    
    if ( ! pJob_SubLoader->m_pWorkingBuffer[iPartition] )
      pJob_SubLoader->vNewWorkingBuffer(pJob_LoadTable->m_pTable, iPartition);
    
    // get the working buffer
    WorkingBuffer* pWorkingBuffer = pJob_SubLoader->m_pWorkingBuffer[iPartition];
    pJob_SubLoader->m_pWorkingBuffer[iPartition] = nullptr;
    
    i8 nWriteBehindBuffers = pJob_SubLoader->m_nWriteBehindBuffers[iPartition];
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // create a write buffer job for load
    Job_WriteBuffer::vSpawnJobWriteBuffer(
          pSession, sTableName, DS_LOAD,
          pWorkingBuffer, iParentJobId, iPartition,
          true, nWriteBehindBuffers);
  }
  
  EXIT();
}
/*----------------------------------------------------------------------------*/
/* subLoader.recordsInvalid -- return no of invalid records fetched by sub    */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderRecordsInvalid(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderRecordsInvalid"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_subLoader.recordsInvalid"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  i8 i_Node = gpAttachMap->m_pGrid->m_iThisNode;
  mpTask->m_pDataStack->vPushInt(*(pJob_SubLoader->m_xNode[i_Node].m_ziRowsInvalid));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.workingBuffer -- get a working buffer                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderWorkingBuffer(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderWorkingBuffer"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_subLoader.workingBuffer"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("job_subLoader.workingBuffer"_Stz);

  WorkingBuffer* pWorkingBuffer = pJob_SubLoader->m_pWorkingBuffer[iPartition];
  
  mpTask->m_pDataStack->vPushObject(pWorkingBuffer);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
