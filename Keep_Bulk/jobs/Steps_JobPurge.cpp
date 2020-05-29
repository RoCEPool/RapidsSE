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
void Step::vJob_PurgeSpawnSub(void)
{
  ENTRY("vJob_PurgeSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_Purge* pJob_Purge = _pJob_Purge(pJobPtr);
  
  i8 iSlot;
  if ( ! m_pDataStack->bPopAsInt(iSlot) )
    THROW_DSTACK();
  
  i8 iPurgeJobId = pJob_Purge->m_iFocalId;
  ReqTuple* aReqTuple = &pJob_Purge->m_xReqTuple;
  
  JobClass* pJobClass_SubPurger = JobClass::pFind(CN_JOB_SUBPURGER);
  
  Job_SubPurger* pJob_SubPurger =
    static_cast<Job_SubPurger*>
      (pJobClass_SubPurger->pPreJob(aReqTuple,nullptr));
  
  pJob_SubPurger->vSeed(SS_SEED_RUN);
  
  pJob_SubPurger->m_iParentId   = pJob_Purge->m_iFocalId;
  pJob_SubPurger->m_iPurgeJobId = iPurgeJobId;
  pJob_SubPurger->m_oEnameSet.vAdd("<tableNm>"_Stz,
                                 pJob_Purge->m_oEnameSet.szValue("<tableNm>"_Stz));
  pJob_SubPurger->vSlot(iSlot);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  IGNORE_RETURN(Api::bSpawnJob(pJob_SubPurger));

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_purge.table! -- store table into job_purge                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgeTableStore(void)
{
  ENTRY("vJob_PurgeTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_Purge;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Purge,pPtrTable) )
    THROW_DSTACK();
  
  Job_Purge* pJob_Purge = _pJob_Purge(pPtrJob_Purge);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_Purge->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
