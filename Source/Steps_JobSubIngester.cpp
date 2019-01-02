/*============================================================================*/
/* Steps_JobSubIngester.cpp - job_subIngester step group                      */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSubIngester(void)
{
  vAddStep("Job_subIngester::spawn"_Stz,    &Step::vJob_SubIngesterSpawn);
  
  vAddStep("job_subIngester.begin"_Stz,     &Step::vJob_SubIngesterBegin);
  vAddStep("Job_subIngester.chunk"_Stz,     &Step::vJob_SubIngesterChunk);
  vAddStep("job_subIngester.end"_Stz,       &Step::vJob_SubIngesterEnd);
  vAddStep("job_subIngester.jobIngest"_Stz, &Step::vJob_SubIngesterJobIngest);
}

/*----------------------------------------------------------------------------*/
/* job_subLoader::spawn -- spawn a new subloader                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIngesterSpawn(MP_Task mpTask)
{
  ENTRY("vJob_SubIngesterSpawn"_Stz);
  
  Object* pPtrJob_Ingest;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Ingest) )
    THROW_INVALID_DSTACK("job_subLoader::spawn"_Stz);
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pPtrJob_Ingest);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = pJob_Ingest->m_pSession;
  
  //! this needs to be optimised
  JobClass* pJobClass_SubIngester = JobClass::pFind(CN_JOB_SUBINGESTER);
  
  Job_SubIngester* pJob_SubIngester =
  static_cast<Job_SubIngester*>(pJobClass_SubIngester->pPreJob(pSession));
  
  pJob_SubIngester->vSeed(SS_SEED_RUN);
  
  pJob_SubIngester->vJob_Ingest(pJob_Ingest);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RapidsSE_bSpawnJob(pJob_SubIngester,SS_SEED_RUN,pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job_subLoader.begin -- begin loading                                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIngesterBegin(MP_Task mpTask)
{
  ENTRY("vSubIngesterBegin"_Stz);
  
  Object* pPtrJob_SubIngester;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubIngester) )
    THROW_INVALID_DSTACK("job_subLoader.begin"_Stz);
  Job_SubIngester* pJob_SubIngester = _pJob_SubIngester(pPtrJob_SubIngester);

  bool bBegun = pJob_SubIngester->bBegin();
  
  mpTask->m_pDataStack->vPushInt((i8)bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.chunk -- read & process rows till buffer full / eof          */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIngesterChunk(MP_Task mpTask)
{
  ENTRY("vJob_SubIngesterChunk"_Stz);
  
  Object* pPtrJob_SubIngester;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubIngester) )
    THROW_INVALID_DSTACK("job_subLoader.chunk"_Stz);
  Job_SubIngester* pJob_SubIngester = _pJob_SubIngester(pPtrJob_SubIngester);
  
  bool bCallAgain = pJob_SubIngester->bChunk();
  
  mpTask->m_pDataStack->vPushInt((i8)bCallAgain);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.end -- end loading                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIngesterEnd(MP_Task mpTask)
{
  ENTRY("vSubIngesterEnd"_Stz);
  
  Object* pPtrJob_SubIngester;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubIngester) )
    THROW_INVALID_DSTACK("subLoader.end"_Stz);
  Job_SubIngester* pJob_SubIngester = _pJob_SubIngester(pPtrJob_SubIngester);
  
  pJob_SubIngester->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.jobIngest -- get job_ingest                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIngesterJobIngest(MP_Task mpTask)
{
  ENTRY("vJob_SubIngesterJobIngest"_Stz);
  
  Object* pPtrJob_SubIngester;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubIngester) )
  THROW_INVALID_DSTACK("subLoader.end"_Stz);
  Job_SubIngester* pJob_SubIngester = _pJob_SubIngester(pPtrJob_SubIngester);
  
  mpTask->m_pDataStack->vPushObject(pJob_SubIngester->m_pJob_Ingest);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
