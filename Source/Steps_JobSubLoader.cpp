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
  vAddStep("Job_subLoader::spawn"_Stz,  &Step::vJob_SubLoaderSpawn);
  
  vAddStep("job_subLoader.begin"_Stz,   &Step::vJob_SubLoaderBegin);
  vAddStep("Job_subLoader.chunk"_Stz,   &Step::vJob_SubLoaderChunk);
  vAddStep("job_subLoader.end"_Stz,     &Step::vJob_SubLoaderEnd);
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

  bool bBegun = pJob_SubLoader->bBegin();
  
  mpTask->m_pDataStack->vPushI8((i8)bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.chunk -- read & process rows till buffer full / eof          */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderChunk(MP_Task mpTask)
{
  ENTRY("vJob_SubLoaderChunk"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubLoader) )
    THROW_INVALID_DSTACK("job_subLoader.chunk"_Stz);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  bool bCallAgain = pJob_SubLoader->bChunk();
  
  mpTask->m_pDataStack->vPushI8((i8)bCallAgain);

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
