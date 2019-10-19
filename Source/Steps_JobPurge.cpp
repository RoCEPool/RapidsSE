/*============================================================================*/
/* Steps_JobPurge.cpp - job_purge step group                                  */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobPurge(void)
{
  vAddStep("Job_purge.spawnSub"_Stz,  &Step::vJob_PurgeSpawnSub);
  vAddStep("job_purge.table!"_Stz,    &Step::vJob_PurgeTableStore);
}

/*----------------------------------------------------------------------------*/
/* Job_purge.spawnSub -- spawn sub purge job                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgeSpawnSub(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_PurgeSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pJobPtr) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  Job_Purge* pJob_Purge = _pJob_Purge(pJobPtr);
  
  i8 iSlot;
  if ( ! mpTask->m_pForth->m_pDataStack->bPopAsInt(iSlot) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  i8 iPurgeJobId = pJob_Purge->m_iUniqueId;
  ReqTuple* aReqTuple = &pJob_Purge->m_xReqTuple;
  
  JobClass* pJobClass_SubPurger = JobClass::pFind(CN_JOB_SUBPURGER);
  
  Job_SubPurger* pJob_SubPurger =
    static_cast<Job_SubPurger*>
      (pJobClass_SubPurger->pPreJob(nullptr,aReqTuple,pJob_Purge->m_iUniqueId));
  
  pJob_SubPurger->vSeed(SS_SEED_RUN);
  
  pJob_SubPurger->m_iParentId   = pJob_Purge->m_iUniqueId;
  pJob_SubPurger->m_iPurgeJobId = iPurgeJobId;
  pJob_SubPurger->vObjectName(pJob_Purge->szObjectName());
  pJob_SubPurger->m_i_Slot      = iSlot;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  IGNORE_RETURN(Api::bSpawnJob(nullptr,pJob_SubPurger));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_purge.table! -- store table into job_purge                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgeTableStore(MP_Task mpTask, Forth* pForth)
{
  ENTRY("vJob_PurgeTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_Purge;
  if ( ! mpTask->m_pForth->m_pDataStack->bPop(pPtrJob_Purge,pPtrTable) )
    vThrow(__func__,__FILE__,__LINE__,FK_DSTACK,__STEP__);
  
  Job_Purge* pJob_Purge = _pJob_Purge(pPtrJob_Purge);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_Purge->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
