/*============================================================================*/
/* Steps_JobTruncateTable.cpp - job_truncateTable step group                  */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobTruncateTable(void)
{
  vAddStep("job_subTruncate.truncateJobId@"_Stz,&Step::vJob_SubTruncateTruncateJobIdFetch);
  vAddStep("job_subTruncate.slot@"_Stz,         &Step::vJob_SubTruncateSlotFetch);
  
  vAddStep("Job_truncateTable.spawnSub"_Stz, &Step::vJob_TruncateTableSpawnSub);
}

/*----------------------------------------------------------------------------*/
/* job_subTruncate.slot@ -- fetch the job slot                                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubTruncateSlotFetch(void)
{
  ENTRY("vJob_SubTruncateSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubTruncate* pJob = _pJob_SubTruncate(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->i_Slot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subTruncate.truncateJobId@ -- fetch the truncate job id                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubTruncateTruncateJobIdFetch(void)
{
  ENTRY("vJob_SubTruncateTruncateJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubTruncate* pJob = _pJob_SubTruncate(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_iTruncateJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_truncateTable.spawnSub -- spawn sub truncate job                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_TruncateTableSpawnSub(void)
{
  ENTRY("vJob_TruncateTableSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_TruncateTable* pJob_TruncateTable = _pJob_TruncateTable(pJobPtr);
  
  i8 iSlot;
  if ( ! m_pDataStack->bPopAsInt(iSlot) )
    THROW_DSTACK();
  
  i8 iTruncateJobId = pJob_TruncateTable->m_iUniqueId;
  ReqTuple* aReqTuple = &pJob_TruncateTable->m_xReqTuple;
  
  JobClass* pJobClass_SubTruncate = JobClass::pFind(CN_JOB_SUBTRUNCATE);
  
  Job_SubTruncate* pJob_SubTruncate =
    static_cast<Job_SubTruncate*>
      (pJobClass_SubTruncate->pPreJob(aReqTuple,nullptr));
  
  pJob_SubTruncate->vSeed(SS_SEED_RUN);
  
  pJob_SubTruncate->m_iParentId  = pJob_TruncateTable->m_iUniqueId;
  pJob_SubTruncate->m_iTruncateJobId = iTruncateJobId;
  pJob_SubTruncate->m_oEnmSet.vAdd("<tableNm>"_Stz,
                                   pJob_TruncateTable->m_oEnmSet.szValue("<tableNm>"_Stz),
                                   TN_STRING);
  pJob_SubTruncate->vSlot(iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pJob_SubTruncate));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
