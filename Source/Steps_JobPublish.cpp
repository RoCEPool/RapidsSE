/*============================================================================*/
/* Steps_JobPublish.cpp - job_publish step group                              */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobPublish(void)
{
  vAddStep("Job_publish.spawnSub"_Stz, &Step::vJob_PublishSpawnSub);
  vAddStep("job_publish.table!"_Stz,   &Step::vJob_PublishTableStore);
}

/*----------------------------------------------------------------------------*/
/* Job_publish.spawnSub -- spawn sub publish job                              */
/*----------------------------------------------------------------------------*/
void Step::vJob_PublishSpawnSub(void)
{
  ENTRY("vJob_PublishSpawnSub"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_Publish* pJob_Publish = _pJob_Publish(pJobPtr);
  
  i8 iSlot;
  if ( ! m_pDataStack->bPopAsInt(iSlot) )
    THROW_DSTACK();
  
  Session* pSession = pJob_Publish->pSession();
  i8 iPublishJobId = pJob_Publish->m_iFocalId;
  ReqTuple* aReqTuple = &pJob_Publish->m_xReqTuple;

  JobClass* pJobClass_SubPublish = JobClass::pFind(CN_JOB_SUBPUBLISH);

  Job_SubPublish* pJob_SubPublish =
    static_cast<Job_SubPublish*>
      (pJobClass_SubPublish->pPreJob(aReqTuple,nullptr));

  pJob_SubPublish->vSeed(SS_SEED_RUN);

  pJob_SubPublish->m_iParentId     = pJob_Publish->m_iFocalId;
  pJob_SubPublish->m_iPublishJobId = iPublishJobId;
  pJob_SubPublish->m_oEnameSet.vAdd("<tableNm>"_Stz,
                                  pJob_Publish->m_oEnameSet.szValue("<tableNm>"_Stz));
  pJob_SubPublish->vSlot(iSlot);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  IGNORE_RETURN(Api::bSpawnJob(pJob_SubPublish));
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_purge.table! -- store table into job_purge                             */
/*----------------------------------------------------------------------------*/
void Step::vJob_PublishTableStore(void)
{
  ENTRY("vJob_PublishTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_Publish;
  if ( ! m_pDataStack->bPopObject(pPtrJob_Publish,pPtrTable) )
    THROW_DSTACK();
  
  Job_Publish* pJob_Publish = _pJob_Publish(pPtrJob_Publish);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_Publish->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
