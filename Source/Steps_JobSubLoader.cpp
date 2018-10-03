/*============================================================================*/
/* Steps_JobSubLoader.cpp - job_subLoader step group                          */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap* gpAttachMap;

// job: ////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSubLoader(void)
{
  vAddStep("job_subLoader.begin"_Stz,         &Task::vJob_SubLoaderBegin,             false);
  vAddStep("job_subLoader.end"_Stz,           &Task::vJob_SubLoaderEnd,               false);
  
  vAddStep("Job_subLoader.jobLoadTable@"_Stz, &Task::vJob_SubLoaderJobLoadTableFetch, false);
  vAddStep("Job_subLoader::spawn"_Stz,        &Task::vJob_SubLoaderSpawn, false);
  
  vAddStep("Job_subLoader.partition@"_Stz,    &Task::vJob_SubLoaderPartitionFetch,    false);
  vAddStep("Job_subLoader.partition!"_Stz,    &Task::vJob_SubLoaderPartitionStore,    false);
  vAddStep("Job_subLoader.recordsInvalid"_Stz,&Task::vJob_SubLoaderRecordsInvalid,    false);
  vAddStep("Job_subLoader.recordsValid"_Stz,  &Task::vJob_SubLoaderRecordsValid,      false);
}

/*----------------------------------------------------------------------------*/
/* subLoader.begin -- begin loading                                           */
/*----------------------------------------------------------------------------*/
void Task::vJob_SubLoaderBegin(MP_Task mpTask)
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
/* subLoader.end -- end loading                                               */
/*----------------------------------------------------------------------------*/
void Task::vJob_SubLoaderEnd(MP_Task mpTask)
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
void Task::vJob_SubLoaderJobLoadTableFetch(MP_Task mpTask)
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
/* subLoader::spawn -- spawn a new subloader                                  */
/*----------------------------------------------------------------------------*/
void Task::vJob_SubLoaderSpawn(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderSpawn"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_LoadTable) )
    THROW_INVALID_DSTACK("subLoader::spawn"_Stz);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  SEitf* pSEitf = pJob_LoadTable->m_pSEitf;
  
  //! this needs to be optimised
  JobClass* pJobClass_SubLoader = JobClass::pFind(CN_JOB_SUBLOADER);
  
  Job_SubLoader* pJob_SubLoader =
  static_cast<Job_SubLoader*>(pJobClass_SubLoader->pPreJob(pSEitf));
  
  pJob_SubLoader->vSeed(SS_SEED_RUN);
  
  pJob_SubLoader->m_sObjectName = pJob_LoadTable->m_sObjectName;
  pJob_SubLoader->vJob_LoadTable(pJob_LoadTable);
  
  SubLoader* pSubLoader = new SubLoader(pJob_SubLoader);
  pJob_SubLoader->vSubLoader(pSubLoader);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RapidsSE_bSpawnJob(pJob_SubLoader,SS_SEED_RUN,pSEitf);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subLoader.partition@ -- get partition no derived from hash of record   */
/*----------------------------------------------------------------------------*/
void Task::vJob_SubLoaderPartitionFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderPartitionFetch"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_subLoader.partition@"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  i8 iPartition = pJob_SubLoader->m_iPartition;
  
  mpTask->m_pDataStack->vPushInt(iPartition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.partition! -- store partition number                         */
/*----------------------------------------------------------------------------*/
void Task::vJob_SubLoaderPartitionStore(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderPartitionStore"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_subLoader.partition!"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  i8 iPartition;
  if ( ! mpTask->m_pDataStack->bPop(iPartition) )
    THROW_INVALID_DSTACK("job_subLoader.partition!"_Stz);
  
  pJob_SubLoader->m_iPartition = iPartition;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.recordsInvalid -- return no of invalid records fetched by sub    */
/*----------------------------------------------------------------------------*/
void Task::vJob_SubLoaderRecordsInvalid(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderRecordsInvalid"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_subLoader.recordsInvalid"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  mpTask->m_pDataStack->vPushInt(pJob_SubLoader->m_iInvalidRecords);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* subLoader.recordsValid -- return no of valid records fetched by sub        */
/*----------------------------------------------------------------------------*/
void Task::vJob_SubLoaderRecordsValid(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderRecordsValid"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_subLoader.recordsValid"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  mpTask->m_pDataStack->vPushInt(pJob_SubLoader->m_iValidRecords);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
