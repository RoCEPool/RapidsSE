/*============================================================================*/
/* Steps_JobSubPurger.cpp - job_subPurger step group                          */
/*============================================================================*/

#include "Steps.h"

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
void Step::vJob_SubPurgerBegin(void)
{
  ENTRY("vJob_SubPurgerBegin"_Stz);
  
  Object* pPtrJobSubPurger;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubPurger) )
    THROW_DSTACK();
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pPtrJobSubPurger);
  
  bool bBegun = pJob_SubPurger->bBegin();
  
  m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPurger.chunk -- chunk subpurger                                     */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgerChunk(void)
{
  ENTRY("vJob_SubPurgerChunk"_Stz);
  
  Object* pPtrJobSubPurger;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubPurger) )
    THROW_DSTACK();
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pPtrJobSubPurger);

  Object* pPtrTable;
  if ( ! m_pDataStack->bPopObject(pPtrTable) )
  THROW_DSTACK();
  Table* pTable = _pTable(pPtrTable);

  pJob_SubPurger->m_pTable = pTable;
  
  bool bCallAgain = pJob_SubPurger->bChunk();
  
  m_pDataStack->vPushBool(true);
  
  if (! bCallAgain) {
    i8 iOlderThanSeconds = pJob_SubPurger->m_pJob_Purge->m_iOlderThanSeconds;
    if (iOlderThanSeconds < 10)
      m_pTask->vSleepUsec(C_1M);
    else
      m_pTask->vSleepUsec(C_1M * 10);
    m_pTask->vTransition(TR_SLEEP);
  }
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPurger.end -- end subpurger                                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgerEnd(void)
{
  ENTRY("vJob_SubPurgerEnd"_Stz);
  
  Object* pPtrJobSubPurger;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubPurger) )
    THROW_DSTACK();
  Job_SubPurger* pJob_SubPurger = _pJob_SubPurger(pPtrJobSubPurger);
  
  pJob_SubPurger->vEnd();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPurger.purgeJobId@ -- fetch the purge job id                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgerPurgeJobIdFetch(void)
{
  ENTRY("vJob_SubPurgerPurgeJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubPurger* pJob = _pJob_SubPurger(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_iPurgeJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPurger.slot@ -- fetch the job slot                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgerSlotFetch(void)
{
  ENTRY("vJob_SubPurgerSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubPurger* pJob = _pJob_SubPurger(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->i_Slot());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
