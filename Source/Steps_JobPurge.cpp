/*============================================================================*/
/* Steps_JobPurge.cpp - job_purge step group                                  */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobPurge(void)
{
  vAddStep("job_purge.begin"_Stz,     &Step::vJob_PurgeBegin);
  vAddStep("job_purge.chunk"_Stz,     &Step::vJob_PurgeChunk);
  vAddStep("job_purge.end"_Stz,       &Step::vJob_PurgeEnd);
  vAddStep("job_purge.purge"_Stz,     &Step::vJob_PurgePurge);
  vAddStep("Job_purge.spawnSub"_Stz,  &Step::vJob_PurgeSpawnSub);
  vAddStep("job_purge.table!"_Stz,    &Step::vJob_PurgeTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_purge.begin -- begin purge                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgeBegin(MP_Task mpTask)
{
  ENTRY("vJob_PurgeBegin"_Stz);
  
  Object* pPtrJobPurge;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobPurge) )
    THROW_INVALID_DSTACK("job_purge.begin"_Stz);
  Job_Purge* pJob_Purge = _pJob_Purge(pPtrJobPurge);
  
  bool bBegun = pJob_Purge->bBegin();
  
  mpTask->m_pDataStack->vPushInt((i8)bBegun);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_purge.chunk -- chunk purge                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgeChunk(MP_Task mpTask)
{
  ENTRY("vJob_PurgeChunk"_Stz);
  
  Object* pPtrJobPurge;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobPurge) )
    THROW_INVALID_DSTACK("job_purge.chunk"_Stz);
  Job_Purge* pJob_Purge = _pJob_Purge(pPtrJobPurge);
  
  bool bCallAgain = pJob_Purge->bChunk();
  
  mpTask->m_pDataStack->vPushInt((i8)bCallAgain);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_purge.end -- end purge                                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgeEnd(MP_Task mpTask)
{
  ENTRY("vJob_PurgeEnd"_Stz);
  
  Object* pPtrJobPurge;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobPurge) )
    THROW_INVALID_DSTACK("job_purge.end"_Stz);
  Job_Purge* pJob_Purge = _pJob_Purge(pPtrJobPurge);
  
  pJob_Purge->vEnd();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_purge.purge -- purge table                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgePurge(MP_Task mpTask)
{
  ENTRY("vJob_PurgePurge"_Stz);
  
  Object* pPtrJobPurge;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobPurge) )
    THROW_INVALID_DSTACK("job_purge.purge"_Stz);
  Job_Purge* pJob_Purge = _pJob_Purge(pPtrJobPurge);
  
  pJob_Purge->vPurge();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_purge.spawnSub -- spawn sub purge job                                  */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgeSpawnSub(MP_Task mpTask)
{
  ENTRY("vJob_PurgeSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_purge.spawnSub"_Stz);
  Job_Purge* pJob_Purge = _pJob_Purge(pJobPtr);
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iSlot) )
    THROW_INVALID_DSTACK("Job_purge.spawnSub"_Stz);
  
  Session* pSession = pJob_Purge->m_pSession;
  i8 iPurgeJobId = pJob_Purge->m_iJobId;
  
  JobClass* pJobClass_SubPurger = JobClass::pFind(CN_JOB_SUBPURGER);
  
  Job_SubPurger* pJob_SubPurger =
  static_cast<Job_SubPurger*>(pJobClass_SubPurger->pPreJob(nullptr));
  
  pJob_SubPurger->vSeed(SS_SEED_RUN);
  
  pJob_SubPurger->m_iPurgeJobId = iPurgeJobId;
  pJob_SubPurger->m_sObjectName = pJob_Purge->m_sObjectName;
  pJob_SubPurger->m_iSlot = iSlot;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RapidsSE_bSpawnJob(pJob_SubPurger,SS_SEED_RUN,nullptr);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_purge.table! -- store table into job_purge                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PurgeTableStore(MP_Task mpTask)
{
  ENTRY("vJob_PurgeTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_Purge;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Purge,pPtrTable) )
    THROW_INVALID_DSTACK("job_purge.tableStore"_Stz);
  
  Job_Purge* pJob_Purge = _pJob_Purge(pPtrJob_Purge);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_Purge->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
