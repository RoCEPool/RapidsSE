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
  ENTRY(); // ENTRY("vJob_SubTruncateSlotFetch"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_SubTruncate* pJob = _pJob_SubTruncate(pJobPtr);
  
  m_pDataStack->vPushInt(pJob->i_Slot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subTruncate.truncateJobId@ -- fetch the truncate job id                */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubTruncateTruncateJobIdFetch(void)
{
  ENTRY(); // ENTRY("vJob_SubTruncateTruncateJobIdFetch"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_SubTruncate* pJob = _pJob_SubTruncate(pJobPtr);
  
  m_pDataStack->vPushInt(pJob->m_iTruncateJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_truncateTable.spawnSub -- spawn sub truncate job                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_TruncateTableSpawnSub(void)
{
  ENTRY(); // ENTRY("vJob_TruncateTableSpawnSub"_Stz);
  
  Object* pJobPtr; m_pDataStack->vPopPtr(pJobPtr,__func__);
  Job_Universal* pJob_Universal = _pJob_Universal(pJobPtr);
  
  i8 iSlot = m_pDataStack->iPopAsInt(__func__);
  
  i8 iTruncateJobId = pJob_Universal->m_iFocalId;
  ReqTuple* aReqTuple = &pJob_Universal->m_xReqTuple;
  
  JobClass* pJobClass_SubTruncate = JobClass::pFind(CN_JOB_SUBTRUNCATE);
  
  Job_SubTruncate* pJob_SubTruncate =
    static_cast<Job_SubTruncate*>
      (pJobClass_SubTruncate->pPreJob(aReqTuple,nullptr));
  
  //! ALKA: We need to separate out preJob and preNew
  //! preJob to be called only from prompt
  //! copy enameSet from parent job here (to be moved to pNew)
  //pJob_Universal->m_oEnameSet.vPrint();
  pJob_SubTruncate->pEnameSet()->vCopy(pJob_Universal->pEnameSet());
  //pJob_SubTruncate->m_oEnameSet.vPrint();
  //!
  
  pJob_SubTruncate->vSeed(SS_SEED_RUN);
  
  pJob_SubTruncate->vParentId(pJob_Universal->m_iFocalId);
  pJob_SubTruncate->m_iTruncateJobId = iTruncateJobId;
  pJob_SubTruncate->pEnameSet()->vAdd("<tableNm>"_Stz,
                        pJob_Universal->pEnameSet()->szValue("<tableNm>"_Stz));
  pJob_SubTruncate->vSlot(iSlot);
 
  pJob_SubTruncate->m_eJobType = pJob_Universal->m_eJobType;
  pJob_SubTruncate->vJobKind(pJob_Universal->eJobKind());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  IGNORE_RETURN(Api::bSpawnJob(pJob_SubTruncate));
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
