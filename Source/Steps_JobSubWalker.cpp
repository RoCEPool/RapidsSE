/*============================================================================*/
/* Steps_JobSubWalker.cpp - jobSubWalker step group                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSubWalker(void)
{
  vAddStep("Job_subWalker::spawn"_Stz,     &Step::vJob_SubWalkerSpawn);

  vAddStep("job_subWalker.begin"_Stz,      &Step::vJob_SubWalkerBegin);
  vAddStep("job_subWalker.chunk"_Stz,      &Step::vJob_SubWalkerChunk);
  vAddStep("job_subWalker.end"_Stz,        &Step::vJob_SubWalkerEnd);
  
  vAddStep("job_subWalker.walkJob@"_Stz,   &Step::vJob_SubWalkerWalkJobFetch);
}

/*----------------------------------------------------------------------------*/
/* job_subWalker::spawn -- spawn a new subexporter                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubWalkerSpawn(Forth* pForth)
{
  ENTRY("vJob_SubWalkerSpawn"_Stz);
  
  Object* pPtrJob_Walk;
  if ( ! pForth->m_pDataStack->bPop(pPtrJob_Walk) )
    THROW_DSTACK();
  Job_Walk* pJob_Walk = _pJob_Walk(pPtrJob_Walk);
  
  i8 i_Slot;
  if ( ! pForth->m_pDataStack->bPop(i_Slot) )
    THROW_DSTACK();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = pJob_Walk->m_pSession;
  ReqTuple* aReqTuple = &pJob_Walk->m_xReqTuple;

  //! this needs to be optimised
  JobClass* pJobClass_SubWalker = JobClass::pFind(CN_JOB_SUBWALKER);
  
  Job_SubWalker* pJob_SubWalker =
    static_cast<Job_SubWalker*>
      (pJobClass_SubWalker->pPreJob(pSession,aReqTuple,pJob_Walk->m_iUniqueId));
  
  pJob_SubWalker->m_iParentId    = pJob_Walk->m_iUniqueId;
  pJob_SubWalker->m_pJobWalk     = pJob_Walk;
  pJob_SubWalker->m_i_Slot       = i_Slot;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pJob_SubWalker->vSeed(SS_SEED_RUN);

  IGNORE_RETURN(Api::bSpawnJob(pSession,pJob_SubWalker));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subWalker.exportJob@ -- get export job                               */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubWalkerWalkJobFetch(Forth* pForth)
{
  ENTRY("vJob_SubWalkerSpawn"_Stz);
  
  Object* pPtrJob_SubWalker;
  if ( ! pForth->m_pDataStack->bPop(pPtrJob_SubWalker) )
    THROW_DSTACK();
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pPtrJob_SubWalker);
  
  pForth->m_pDataStack->vPushPointer(pJob_SubWalker->m_pJobWalk);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subWalker.begin -- begin sub-exporter                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubWalkerBegin(Forth* pForth)
{
  ENTRY("vJob_SubWalkerBegin"_Stz);
  
  Object* pPtrJobSubWalker;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobSubWalker) )
    THROW_DSTACK();
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pPtrJobSubWalker);
  
  bool bBegun = pJob_SubWalker->bBegin();
  
  pForth->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subWalker.chunk -- chunk sub-exporter                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubWalkerChunk(Forth* pForth)
{
  ENTRY("vJob_SubWalkerChunk"_Stz);
  
  Object* pPtrJobSubWalker;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobSubWalker) )
    THROW_DSTACK();
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pPtrJobSubWalker);
  
  bool bCallAgain = pJob_SubWalker->bChunk();
  
  pForth->m_pDataStack->vPushBool(bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subWalker.end -- end sub-exporter                                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubWalkerEnd(Forth* pForth)
{
  ENTRY("vJob_SubWalkerEnd"_Stz);
  
  Object* pPtrJobSubWalker;
  if ( ! pForth->m_pDataStack->bPop(pPtrJobSubWalker) )
    THROW_DSTACK();
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pPtrJobSubWalker);
  
  pJob_SubWalker->vEnd();
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
