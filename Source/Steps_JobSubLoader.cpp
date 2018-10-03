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
  vAddStep("Job_subLoader.jobLoadTable@"_Stz, &Task::vJob_SubLoaderJobLoadTableFetch, false);
  vAddStep("Job_subLoader::spawn"_Stz,        &Task::vJob_SubLoaderSpawn, false);
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



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
