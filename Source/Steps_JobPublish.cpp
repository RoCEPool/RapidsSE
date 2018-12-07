/*============================================================================*/
/* Steps_JobPublish.cpp - job_publish step group                              */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobPublish(void)
{
  vAddStep("job_subPublish.publishJobId@"_Stz,&Step::vJob_SubPublishPublishJobIdFetch);
  vAddStep("job_subPublish.slot@"_Stz,        &Step::vJob_SubPublishSlotFetch);
  
  vAddStep("job_publish.publish"_Stz,  &Step::vJob_PublishPublish);
  vAddStep("Job_publish.spawnSub"_Stz, &Step::vJob_PublishSpawnSub);
  vAddStep("job_publish.table!"_Stz,   &Step::vJob_PublishTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_subPurge.purgeJobId@ -- fetch the purge job id                         */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishPublishJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubPublishPublishJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subPublish.publishJobId@"_Stz);
  Job_SubPublish* pJob = _pJob_SubPublish(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iPublishJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPublish.slot@ -- fetch the job slot                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishSlotFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubPublishSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subPublish.slot@"_Stz);
  Job_SubPublish* pJob = _pJob_SubPublish(pJobPtr);
  
  mpTask->m_pDataStack->vPushInt(pJob->m_iSlot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_purge.purge -- purge table                                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PublishPublish(MP_Task mpTask)
{
  ENTRY("vJob_PublishPublish"_Stz);
  
  Object* pPtrJob_Publish;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Publish) )
    THROW_INVALID_DSTACK("job_publish.publish"_Stz);
  Job_Publish* pJob_Publish = _pJob_Publish(pPtrJob_Publish);
  
  pJob_Publish->vPublish();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Job_publish.spawnSub -- spawn sub publish job                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_PublishSpawnSub(MP_Task mpTask)
{
  ENTRY("vJob_PublishSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("Job_publish.spawnSub"_Stz);
  Job_Publish* pJob_Publish = _pJob_Publish(pJobPtr);
  
  i8 iSlot;
  if ( ! mpTask->m_pDataStack->bPopAsInt(iSlot) )
    THROW_INVALID_DSTACK("Job_publish.spawnSub"_Stz);
  
  Session* pSession = pJob_Publish->m_pSession;
  i8 iPublishJobId = pJob_Publish->m_iJobId;

  JobClass* pJobClass_SubPublish = JobClass::pFind(CN_JOB_SUBPUBLISH);

  Job_SubPublish* pJob_SubPublish =
  static_cast<Job_SubPublish*>(pJobClass_SubPublish->pPreJob(nullptr));

  pJob_SubPublish->vSeed(SS_SEED_RUN);

  pJob_SubPublish->m_iPublishJobId = iPublishJobId;
  pJob_SubPublish->m_sObjectName = pJob_Publish->m_sObjectName;
  pJob_SubPublish->m_iSlot = iSlot;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RapidsSE_bSpawnJob(pJob_SubPublish,SS_SEED_RUN,nullptr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_purge.table! -- store table into job_purge                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PublishTableStore(MP_Task mpTask)
{
  ENTRY("vJob_PublishTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_Publish;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_Publish,pPtrTable) )
    THROW_INVALID_DSTACK("job_publish.table"_Stz);
  
  Job_Publish* pJob_Publish = _pJob_Publish(pPtrJob_Publish);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_Publish->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
