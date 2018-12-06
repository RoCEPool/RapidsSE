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
  vAddStep("job_subPurge.purgeJobId@"_Stz,&Step::vJob_SubPurgePurgeJobIdFetch);
  vAddStep("job_subPurge.slot@"_Stz,      &Step::vJob_SubPurgeSlotFetch);
  
  vAddStep("job_purge.purge"_Stz,    &Step::vJob_PurgePurge);
  vAddStep("Job_purge.spawnSub"_Stz, &Step::vJob_PurgeSpawnSub);
  vAddStep("job_purge.table!"_Stz,   &Step::vJob_PurgeTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_subPurge.purgeJobId@ -- fetch the purge job id                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgePurgeJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubPurgePurgeJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subPurge.purgeJobId@"_Stz);
  Job_SubPurge* pJob = _pJob_SubPurge(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iPurgeJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPurge.slot@ -- fetch the job slot                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPurgeSlotFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubPurgeSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subPurge.slot@"_Stz);
  Job_SubPurge* pJob = _pJob_SubPurge(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iSlot);
  
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
  
  JobClass* pJobClass_SubPurge = JobClass::pFind(CN_JOB_SUBPURGE);
  
  Job_SubPurge* pJob_SubPurge =
  static_cast<Job_SubPurge*>(pJobClass_SubPurge->pPreJob(nullptr));
  
  pJob_SubPurge->vSeed(SS_SEED_RUN);
  
  pJob_SubPurge->m_iPurgeJobId = iPurgeJobId;
  pJob_SubPurge->m_sObjectName = pJob_Purge->m_sObjectName;
  pJob_SubPurge->m_iSlot = iSlot;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  RapidsSE_bSpawnJob(pJob_SubPurge,SS_SEED_RUN,nullptr);

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
