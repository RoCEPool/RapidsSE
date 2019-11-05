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
void Step::vJob_SubWalkerSpawn(void)
{
  ENTRY("vJob_SubWalkerSpawn"_Stz);
  
  Object* pPtrJob_Walk;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Walk) )
    THROW_DSTACK();
  Job_Walk* pJob_Walk = _pJob_Walk(pPtrJob_Walk);
  
  i8 i_Slot;
  if ( ! m_pDataStack->bPopI8(i_Slot) )
    THROW_DSTACK();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Session* pSession = pJob_Walk->pSession();
  ReqTuple* aReqTuple = &pJob_Walk->m_xReqTuple;

  //! this needs to be optimised
  JobClass* pJobClass_SubWalker = JobClass::pFind(CN_JOB_SUBWALKER);
  
  Job_SubWalker* pJob_SubWalker =
    static_cast<Job_SubWalker*>
      (pJobClass_SubWalker->pPreJob(aReqTuple,pSession));
  
  pJob_SubWalker->m_iParentId    = pJob_Walk->m_iFocalId;
  pJob_SubWalker->m_pJobWalk     = pJob_Walk;
  pJob_SubWalker->vSlot(i_Slot);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pJob_SubWalker->vSeed(SS_SEED_RUN);

  IGNORE_RETURN(Api::bSpawnJob(pJob_SubWalker));
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subWalker.exportJob@ -- get export job                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubWalkerWalkJobFetch(void)
{
  ENTRY("vJob_SubWalkerSpawn"_Stz);
  
  Object* pPtrJob_SubWalker;
  if ( ! m_pDataStack->bPopObject(pPtrJob_SubWalker) )
    THROW_DSTACK();
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pPtrJob_SubWalker);
  
  m_pDataStack->vPushPointer(pJob_SubWalker->m_pJobWalk);
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subWalker.begin -- begin sub-exporter                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubWalkerBegin(void)
{
  ENTRY("vJob_SubWalkerBegin"_Stz);
  
  Object* pPtrJobSubWalker;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubWalker) )
    THROW_DSTACK();
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pPtrJobSubWalker);
  
  bool bBegun = pJob_SubWalker->bBegin();
  
  m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subWalker.chunk -- chunk sub-exporter                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubWalkerChunk(void)
{
  ENTRY("vJob_SubWalkerChunk"_Stz);
  
  Object* pPtrJobSubWalker;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubWalker) )
    THROW_DSTACK();
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pPtrJobSubWalker);
  
  bool bCallAgain = pJob_SubWalker->bChunk();
  
  m_pDataStack->vPushBool(bCallAgain);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subWalker.end -- end sub-exporter                                    */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubWalkerEnd(void)
{
  ENTRY("vJob_SubWalkerEnd"_Stz);
  
  Object* pPtrJobSubWalker;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubWalker) )
    THROW_DSTACK();
  Job_SubWalker* pJob_SubWalker = _pJob_SubWalker(pPtrJobSubWalker);
  
  pJob_SubWalker->vEnd();
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
