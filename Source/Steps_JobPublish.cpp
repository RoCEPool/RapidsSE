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
  vAddStep("job_publish.publish"_Stz,  &Step::vJob_PublishPublish);
  vAddStep("job_publish.table!"_Stz,   &Step::vJob_PublishTableStore);
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
