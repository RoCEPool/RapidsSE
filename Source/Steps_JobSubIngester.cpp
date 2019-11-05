/*============================================================================*/
/* Steps_JobSubIngester.cpp - job_subIngester step group                      */
/*============================================================================*/

#include "Steps.h"

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
/* job_subIngesterr::spawn -- spawn a new subIngester                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIngesterSpawn(void)
{
  ENTRY("vJob_SubIngesterSpawn"_Stz);
  
  Object* pPtrJob_Ingest;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Ingest) )
    THROW_DSTACK();
  Job_Ingest* pJob_Ingest = _pJob_Ingest(pPtrJob_Ingest);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = pJob_Ingest->pSession();
  ReqTuple* aReqTuple = &pJob_Ingest->m_xReqTuple;
  
  //! this needs to be optimised
  JobClass* pJobClass_SubIngester = JobClass::pFind(CN_JOB_SUBINGESTER);
  
  Job_SubIngester* pJob_SubIngester =
    static_cast<Job_SubIngester*>
      (pJobClass_SubIngester->pPreJob(aReqTuple,pSession));
  
  pJob_SubIngester->m_iParentId  = pJob_Ingest->m_iFocalId;
  
  pJob_SubIngester->vSeed(SS_SEED_RUN);
  
  pJob_SubIngester->vJob_Ingest(pJob_Ingest);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pJob_SubIngester));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job_subIngester.begin -- begin ingesting                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIngesterBegin(void)
{
  ENTRY("vSubIngesterBegin"_Stz);
  
  Object* pPtrJob_SubIngester;
  if ( ! m_pDataStack->bPopObject(pPtrJob_SubIngester) )
    THROW_DSTACK();
  Job_SubIngester* pJob_SubIngester = _pJob_SubIngester(pPtrJob_SubIngester);

  bool bBegun = pJob_SubIngester->bBegin();
  
  m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subIngester.chunk -- read & process rows till buffer full / eof        */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIngesterChunk(void)
{
  ENTRY("vJob_SubIngesterChunk"_Stz);
  
  Object* pPtrJob_SubIngester;
  if ( ! m_pDataStack->bPopObject(pPtrJob_SubIngester) )
    THROW_DSTACK();
  Job_SubIngester* pJob_SubIngester = _pJob_SubIngester(pPtrJob_SubIngester);
  
  bool bCallAgain = pJob_SubIngester->bChunk();
  
  if (bCallAgain) {
    m_pDataStack->vPushBool(true);
  }
  else {
    if ( 0 == pJob_SubIngester->m_pJob_Ingest->nIngestChunks() &&
        -1 != pJob_SubIngester->m_pJob_Ingest->m_iLastChunk) {
      m_pDataStack->vPushBool(false);
    }
    else {
      // sleep for 0.010 second
      m_pTask->vSleepUsec(C_1K * 10);
      m_pTask->vTransition(TR_SLEEP);
      m_pDataStack->vPushBool(true);
    }
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subIngester.end -- end ingesting                                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIngesterEnd(void)
{
  ENTRY("vSubIngesterEnd"_Stz);
  
  Object* pPtrJob_SubIngester;
  if ( ! m_pDataStack->bPopObject(pPtrJob_SubIngester) )
    THROW_DSTACK();
  Job_SubIngester* pJob_SubIngester = _pJob_SubIngester(pPtrJob_SubIngester);
  
  pJob_SubIngester->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.jobIngest -- get job_ingest                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubIngesterJobIngest(void)
{
  ENTRY("vJob_SubIngesterJobIngest"_Stz);
  
  Object* pPtrJob_SubIngester;
  if ( ! m_pDataStack->bPopObject(pPtrJob_SubIngester) )
  THROW_DSTACK();
  Job_SubIngester* pJob_SubIngester = _pJob_SubIngester(pPtrJob_SubIngester);
  
  m_pDataStack->vPushPointer(pJob_SubIngester->m_pJob_Ingest);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
