/*============================================================================*/
/* Steps_JobSubPublish.cpp - job_subPublish step group                        */
/*============================================================================*/

#include "Steps.h"

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
void Step::vJob_SubPublishBegin(void)
{
  ENTRY("vJob_SubPublishBegin"_Stz);
  
  Object* pPtrJobSubPublish;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubPublish) )
    THROW_DSTACK();
  Job_SubPublish* pJob_SubPupublish = _pJob_SubPublish(pPtrJobSubPublish);
  
  bool bBegun = pJob_SubPupublish->bBegin();
  
  m_pDataStack->vPushBool(bBegun);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPublish.chunk -- chunk subpublish                                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishChunk(void)
{
  ENTRY("vJob_SubPublishChunk"_Stz);
  
  Object* pPtrJobSubPublish;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubPublish) )
    THROW_DSTACK();
  Job_SubPublish* pJob_SubPupublish = _pJob_SubPublish(pPtrJobSubPublish);
  
  bool bBufferPublished = pJob_SubPupublish->bChunk();
  
  m_pDataStack->vPushBool(true);
  
  if (! bBufferPublished) {
    m_pTask->vSleepUsec(C_100K);
    m_pTask->vTransition(TR_SLEEP);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPublish.end -- end subpublish                                       */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishEnd(void)
{
  ENTRY("vJob_SubPublishEnd"_Stz);
  
  Object* pPtrJobSubPublish;
  if ( ! m_pDataStack->bPopObject(pPtrJobSubPublish) )
    THROW_DSTACK();
  Job_SubPublish* pJob_SubPupublish = _pJob_SubPublish(pPtrJobSubPublish);

  pJob_SubPupublish->vEnd();
  
  EXIT();
}


/*----------------------------------------------------------------------------*/
/* job_subPublish.publishJobId@ -- fetch the publish job id                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishPublishJobIdFetch(void)
{
  ENTRY("vJob_SubPublishPublishJobIdFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubPublish* pJob = _pJob_SubPublish(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->m_iPublishJobId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPublish.slot@ -- fetch the job slot                                 */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishSlotFetch(void)
{
  ENTRY("vJob_SubPublishSlotFetch"_Stz);
  
  Object* pJobPtr;
  if ( ! m_pDataStack->bPopObject(pJobPtr) )
    THROW_DSTACK();
  Job_SubPublish* pJob = _pJob_SubPublish(pJobPtr);
  
  m_pDataStack->vPushI8(pJob->i_Slot());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_subPublish.table! -- store table into job subPublish                   */
/*----------------------------------------------------------------------------*/
void Step::vJob_SubPublishTableStore(void)
{
  ENTRY("vJob_SubPublishTableStore"_Stz);
  
  Object* pPtrTable;
  Object* pPtrJob_SubPublish;
  if ( ! m_pDataStack->bPopObject(pPtrJob_SubPublish,pPtrTable) )
    THROW_DSTACK();
  
  Job_SubPublish* pJob_SubPublish = _pJob_SubPublish(pPtrJob_SubPublish);
  
  Table* pTable = _pTable(pPtrTable);
  
  pJob_SubPublish->vTable(pTable);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
