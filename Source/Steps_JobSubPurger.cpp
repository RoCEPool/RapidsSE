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
  vAddStep("job_subPurger.begin"_Stz,      &Step::vJob_SubPurgerBegin);
  vAddStep("job_subPurger.chunk"_Stz,      &Step::vJob_SubPurgerChunk);
  vAddStep("job_subPurger.end"_Stz,        &Step::vJob_SubPurgerEnd);
  vAddStep("job_subPurger.purgeJobId@"_Stz,&Step::vJob_SubPurgerPurgeJobIdFetch);
  vAddStep("job_subPurger.slot@"_Stz,      &Step::vJob_SubPurgerSlotFetch);
}

/*----------------------------------------------------------------------------*/
/* job_subPurger.begin -- begin subpurger                                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgerBegin(MP_Task mpTask)
{
  ENTRY("vJob_SubPurgerBegin"_Stz);
  
  Object* pPtrJobSubPurger;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobSubPurger) )
    THROW_INVALID_DSTACK("job_subPurger.begin"_Stz);
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pPtrJobSubPurger);
  
  bool bBegun = pJob_SubPurger->bBegin();
  
  mpTask->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPurger.chunk -- chunk subpurger                                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgerChunk(MP_Task mpTask)
{
  ENTRY("vJob_SubPurgerChunk"_Stz);
  
  Object* pPtrJobSubPurger;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobSubPurger) )
    THROW_INVALID_DSTACK("job_subPurger.chunk"_Stz);
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pPtrJobSubPurger);

  Object* pPtrTable;
  if ( ! mpTask->m_pDataStack->bPop(pPtrTable) )
  THROW_INVALID_DSTACK("job_subPurger.chunk"_Stz);
  Table* pTable = _pTable(pPtrTable);

  pJob_SubPurger->m_pTable = pTable;
  
  bool bCallAgain = pJob_SubPurger->bChunk();
  
  mpTask->m_pDataStack->vPushBool(true);
  
  if (! bCallAgain) {
    i8 iOlderThanSeconds = pJob_SubPurger->m_pJob_Purge->m_iOlderThanSeconds;
    if (iOlderThanSeconds < 10)
      mpTask->vSleepUs(C_1M);
    else
      mpTask->vSleepUs(C_1M * 10);
    mpTask->vState(TK_SLEEPING);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPurger.end -- end subpurger                                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgerEnd(MP_Task mpTask)
{
  ENTRY("vJob_SubPurgerEnd"_Stz);
  
  Object* pPtrJobSubPurger;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobSubPurger) )
    THROW_INVALID_DSTACK("job_subPurger.end"_Stz);
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pPtrJobSubPurger);
  
  pJob_SubPurger->vEnd();
  
  EXIT();
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
  
  mpTask->m_pDataStack->vPushI8(pJob->m_iPurgeJobId);
  
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
  
  mpTask->m_pDataStack->vPushI8(pJob->m_iSlot);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
