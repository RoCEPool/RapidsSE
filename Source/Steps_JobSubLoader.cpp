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
void Step::vJob_SubLoaderSpawn(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SubLoaderSpawn"_Stz);
  
  Object* pPtrJob_LoadTable;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJob_LoadTable) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pPtrJob_LoadTable);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = pJob_LoadTable->m_pSession;
  ReqTuple* aReqTuple = &pJob_LoadTable->m_xReqTuple;

  //! this needs to be optimised
  JobClass* pJobClass_SubLoader = JobClass::pFind(CN_JOB_SUBLOADER);
  
  Job_SubLoader* pJob_SubLoader =
    static_cast<Job_SubLoader*>
      (pJobClass_SubLoader->pPreJob(pSession,aReqTuple,pJob_LoadTable->m_iUniqueId));
  
  pJob_SubLoader->vSeed(SS_SEED_RUN);
  
  pJob_SubLoader->m_iParentId  = pJob_LoadTable->m_iUniqueId;

  pJob_SubLoader->vObjectName(pJob_LoadTable->szObjectName());
  pJob_SubLoader->vJob_LoadTable(pJob_LoadTable);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pSession,pJob_SubLoader));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* job_subLoader.begin -- begin loading                                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderBegin(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SubLoaderBegin"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJob_SubLoader) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);

  bool bBegun = pJob_SubLoader->bBegin();
  
  mpTask->m_pForth->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.chunk -- read & process rows till buffer full / eof          */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderChunk(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SubLoaderChunk"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJob_SubLoader) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // stall if ready queue LP is building up (to discuss with Ray)
  if (AM::pQueues()->m_pReadyQueueLP->nTasks() >
      AM::nMaxReadyLP()) {
    mpTask->vSleepUsec(1000);
    mpTask->vTransition(TR_SLEEP);
    mpTask->m_pForth->m_pDataStack->vPushBool(true);
  }
  else {
    bool bCallAgain = pJob_SubLoader->bChunk();
    mpTask->m_pForth->m_pDataStack->vPushBool(bCallAgain);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.end -- end loading                                           */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderEnd(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SubLoaderEnd"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJob_SubLoader) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  pJob_SubLoader->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subLoader.terminate -- test early termination                          */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubLoaderTerminate(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_SubLoaderTerminate"_Stz);
  
  Object* pPtrJob_SubLoader;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJob_SubLoader) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_SubLoader* pJob_SubLoader = _pJob_SubLoader(pPtrJob_SubLoader);
  
  i8 iTerminate = pJob_SubLoader->bTerminate();
  
  mpTask->m_pForth->m_pDataStack->vPushI8(iTerminate);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
