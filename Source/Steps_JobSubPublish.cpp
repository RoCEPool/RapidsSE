/*============================================================================*/
/* Steps_JobSubPublish.cpp - job_subPublish step group                        */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobSubPublish(void)
{
  vAddStep("job_subPublish.begin"_Stz,      &Step::vJob_SubPublishBegin);
  vAddStep("job_subPublish.chunk"_Stz,      &Step::vJob_SubPublishChunk);
  vAddStep("job_subPublish.end"_Stz,        &Step::vJob_SubPublishEnd);
  
  vAddStep("job_subPublish.publishJobId@"_Stz,&Step::vJob_SubPublishPublishJobIdFetch);
  vAddStep("job_subPublish.slot@"_Stz,        &Step::vJob_SubPublishSlotFetch);
  vAddStep("job_subPublish.table!"_Stz,       &Step::vJob_SubPublishTableStore);
}

/*----------------------------------------------------------------------------*/
/* job_subPublish.begin -- begin subpublish                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishBegin(MP_Task mpTask)
{
  ENTRY("vJob_SubPublishBegin"_Stz);
  
  Object* pPtrJobSubPublish;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobSubPublish) )
    THROW_INVALID_DSTACK("job_subPublish.begin"_Stz);
  Job_SubPublish* pJob_SubPupublish = _pJob_SubPublish(pPtrJobSubPublish);
  
  bool bBegun = pJob_SubPupublish->bBegin();
  
  mpTask->m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPublish.chunk -- chunk subpublish                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishChunk(MP_Task mpTask)
{
  ENTRY("vJob_SubPublishChunk"_Stz);
  
  Object* pPtrJobSubPublish;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobSubPublish) )
    THROW_INVALID_DSTACK("job_subPublish.chunk"_Stz);
  Job_SubPublish* pJob_SubPupublish = _pJob_SubPublish(pPtrJobSubPublish);
  
  bool bBufferPublished = pJob_SubPupublish->bChunk();
  
  mpTask->m_pDataStack->vPushBool(true);
  
  if (! bBufferPublished) {
    mpTask->vSleepUs(C_100K);
    mpTask->vState(TK_SLEEPING);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPublish.end -- end subpublish                                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishEnd(MP_Task mpTask)
{
  ENTRY("vJob_SubPublishEnd"_Stz);
  
  Object* pPtrJobSubPublish;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJobSubPublish) )
    THROW_INVALID_DSTACK("job_subPublish.end"_Stz);
  Job_SubPublish* pJob_SubPupublish = _pJob_SubPublish(pPtrJobSubPublish);

  pJob_SubPupublish->vEnd();
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subPublish.publishJobId@ -- fetch the publish job id                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishPublishJobIdFetch(MP_Task mpTask)
{
  ENTRY("vJob_SubPublishPublishJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("job_subPublish.publishJobId@"_Stz);
  Job_SubPublish* pJob = _pJob_SubPublish(pJobPtr);
  
  mpTask->m_pDataStack->vPushI8(pJob->m_iPublishJobId);
  
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
  
  mpTask->m_pDataStack->vPushI8(pJob->m_i_Slot);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPublish.table! -- store table into job subPublish                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishTableStore(MP_Task mpTask)
{
  ENTRY("vJob_SubPublishTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_SubPublish;
  if ( ! mpTask->m_pDataStack->bPop(pPtrJob_SubPublish,pPtrTable) )
    THROW_INVALID_DSTACK("job_subPublish.table!"_Stz);
  
  Job_SubPublish* pJob_SubPublish = _pJob_SubPublish(pPtrJob_SubPublish);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_SubPublish->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
