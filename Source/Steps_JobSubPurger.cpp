/*============================================================================*/
/* Steps_JobSubPurger.cpp - job_subPurger step group                          */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSubPurger(void)
{
  vAddStep("job_subPurger.purgeJobId@"_Stz,&Step::vJob_SubPurgePurgeJobIdFetch);
  vAddStep("job_subPurger.slot@"_Stz,      &Step::vJob_SubPurgeSlotFetch);
}

/*----------------------------------------------------------------------------*/
/* job_subPurger.purgeJobId@ -- fetch the purge job id                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgePurgeJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubPurgePurgeJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subPurger.purgeJobId@"_Stz);
  Job_SubPurger* pJob = _pJob_SubPurger(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iPurgeJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPurger.slot@ -- fetch the job slot                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgeSlotFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubPurgeSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subPurger.slot@"_Stz);
  Job_SubPurger* pJob = _pJob_SubPurger(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iSlot);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
