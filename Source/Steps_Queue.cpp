/*============================================================================*/
/* Steps_Queue.cpp - queue step group                                         */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Queue(void)
{
  vAddStep("queue::new"_Stz,      &Step::vQueueNew);
  
  vAddStep("queue.append"_Stz,    &Step::vQueueAppend);
  vAddStep("queue.delete"_Stz,    &Step::vQueueDelete);
  vAddStep("queue.dump"_Stz,      &Step::vQueueDump);
  vAddStep("queue.init"_Stz,      &Step::vQueueInit);
  vAddStep("queue.pop"_Stz,       &Step::vQueuePop);
}

/*----------------------------------------------------------------------------*/
/* queue.append -- append onto queue                                          */
/*----------------------------------------------------------------------------*/
void Step::vQueueAppend(MP_Task mpTask)
{
ENTRY("vQueueAppend"_Stz);
  
  Object* pObject;
  GenVar xVariable;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject,xVariable) )
    THROW_INVALID_DSTACK("queue.append"_Stz);
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  ASSERT(CN_QUEUE == pObject->eClass());
  
  pQueue->bAppend(xVariable.pNestedValue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.delete -- delete the queue                                           */
/*----------------------------------------------------------------------------*/
void Step::vQueueDelete(MP_Task mpTask)
{
  ENTRY("vQueueDelete"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
  THROW_INVALID_DSTACK("queue.delete"_Stz);
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  pQueue->vFinalize();
  delete pQueue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* q.dump -- queue dump                                                       */
/*----------------------------------------------------------------------------*/
void Step::vQueueDump(MP_Task mpTask)
{
ENTRY("vQueueDump"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("q.dump"_Stz);
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  pQueue->vContentDump(aSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.init -- queue init                                                   */
/*----------------------------------------------------------------------------*/
void Step::vQueueInit(MP_Task mpTask)
{
ENTRY("vQueueInit"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("queue.init"_Stz);
    
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  new(pQueue) Queue;
  
  Word* pQueueWord = Word::pToWord(pQueue);
    
  //pQueue->vLockName(pQueueWord->sName().aStz());

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue::new -- create a named queue                                         */
/*----------------------------------------------------------------------------*/
void Step::vQueueNew(MP_Task mpTask)
{
  ENTRY("vQueueNew"_Stz);
  
  s256 sQueueName;
  
  if ( ! mpTask->m_pDataStack->bPop(sQueueName) )
    THROW_INVALID_DSTACK("queue::new"_Stz);
  
  Queue* pQueue = new Queue();
  
  //pQueue->vLockName(sQueueName);
  
  mpTask->m_pDataStack->vPushObject(pQueue);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.pop -- queue pop                                                     */
/*----------------------------------------------------------------------------*/
void Step::vQueuePop(MP_Task mpTask)
{
  ENTRY("vQueuePop"_Stz);
  
  Object* pObject;
  GenVar xVariable;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("queue.pop"_Stz);
  
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  
  if (pQueue->bPop(&xVariable)) {
    mpTask->m_pDataStack->vPushGenVar(xVariable);
    mpTask->m_pDataStack->vPushInt((i8)1);
  }
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
