/*============================================================================*/
/* Steps_JobSubLoader.cpp - job_subLoader step group                          */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSubLoader(void)
{
  vAddStep("Job_subLoader::spawn"_Stz,    &Step::vJob_SubLoaderSpawn);
  
  vAddStep("job_subLoader.begin"_Stz,     &Step::vJob_SubLoaderBegin);
  vAddStep("Job_subLoader.chunk"_Stz,     &Step::vJob_SubLoaderChunk);
  vAddStep("job_subLoader.end"_Stz,       &Step::vJob_SubLoaderEnd);
  vAddStep("job_subLoader.terminate"_Stz, &Step::vJob_SubLoaderTerminate);
}

/*----------------------------------------------------------------------------*/
/* job_subLoader::spawn -- spawn a new subloader                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderSpawn(void)
{
  ENTRY("vJob_SubLoaderSpawn"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! m_pDataStack->bPopObject(pPtrJob_LoadTable) )
    THROW_DSTACK();
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ReqTuple* aReqTuple = &pJob_LoadTable->m_xReqTuple;

  //! this needs to be optimised
  JobClass* pJobClass_SubLoader = JobClass::pFind(CN_JOB_SUBLOADER);
  
  Job_SubLoader* pJob_SubLoader = new Job_SubLoader(pJobClass_SubLoader,aReqTuple);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pJob_SubLoader->vSeed(SS_SEED_RUN);
  
  pJob_SubLoader->m_iParentId = pJob_LoadTable->m_iFocalId;

  //pJob_SubLoader->m_oEnameSet.vAdd("<tableNm>"_Stz,
  //                               pJob_LoadTable->m_oEnameSet.szValue("<tableNm>"_Stz));
  
  pJob_SubLoader->vJob_LoadTable(pJob_LoadTable);
  
  
  pJob_SubLoader->m_oEnameSet.vCopy(&pJob_LoadTable->m_oEnameSet);
  
  pJob_SubLoader->m_eJobType = pJob_LoadTable->m_eJobType;
  pJob_SubLoader->m_eJobKind = pJob_LoadTable->m_eJobKind;


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pJob_SubLoader));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job_subLoader.begin -- begin loading                                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderBegin(void)
{
  ENTRY("vJob_SubLoaderBegin"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! m_pDataStack->bPopObject(pPtrJob_SubLoader) )
    THROW_DSTACK();
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);

  bool bBegun = pJob_SubLoader->bBegin();
  
  m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.chunk -- read & process rows till buffer full / eof          */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderChunk(void)
{
  ENTRY("vJob_SubLoaderChunk"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! m_pDataStack->bPopObject(pPtrJob_SubLoader) )
    THROW_DSTACK();
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stall if ready queue LP is building up (to discuss with Ray)
  if (AM::pQueues()->m_pReadyQueueLP->nTasks() >
      AM::nMaxReadyLP()) {
    m_pTask->vSleepUsec(1000);
    m_pTask->vTransition(SA_SLEEP);
    m_pDataStack->vPushBool(true);
  }
  else {
    bool bCallAgain = pJob_SubLoader->bChunk();
    m_pDataStack->vPushBool(bCallAgain);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.end -- end loading                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderEnd(void)
{
  ENTRY("vJob_SubLoaderEnd"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! m_pDataStack->bPopObject(pPtrJob_SubLoader) )
    THROW_DSTACK();
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  pJob_SubLoader->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.terminate -- test early termination                          */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderTerminate(void)
{
  ENTRY("vJob_SubLoaderTerminate"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! m_pDataStack->bPopObject(pPtrJob_SubLoader) )
    THROW_DSTACK();
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  i8 iTerminate = pJob_SubLoader->bTerminate();
  
  m_pDataStack->vPushI8(iTerminate);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
