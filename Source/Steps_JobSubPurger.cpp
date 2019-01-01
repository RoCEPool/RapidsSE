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
  vAddStep("job_subPurger.purgeJobId@"_Stz,&Step::vJob_SubPurgerPurgeJobIdFetch);
  vAddStep("job_subPurger.slot@"_Stz,      &Step::vJob_SubPurgerSlotFetch);
}

/*----------------------------------------------------------------------------*/
/* job_subPurger.purgeJobId@ -- fetch the purge job id                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgerPurgeJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubPurgerPurgeJobIdFetch"_Stz);
  
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
void Step::vJob_SubPurgerSlotFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubPurgerSlotFetch"_Stz);
  
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
