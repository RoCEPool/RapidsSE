/*============================================================================*/
/* Steps_Queue.cpp - queue step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// queue: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Queue(void)
{
  vAddStep("queue.append"_Stz,    &Task::vQueueAppend);
  vAddStep("queue.delete"_Stz,    &Task::vQueueDelete);
  vAddStep("queue.dump"_Stz,      &Task::vQueueDump);
  vAddStep("queue.init"_Stz,      &Task::vQueueInit);
  vAddStep("queue::new"_Stz,      &Task::vQueueNew);
  vAddStep("queue.pop"_Stz,       &Task::vQueuePop);
}

/*----------------------------------------------------------------------------*/
/* queue.append -- append onto queue                                          */
/*----------------------------------------------------------------------------*/
void Task::vQueueAppend(MP_Task mpTask)
{
ENTRY("vQueueAppend"_Stz);
  
  Object* pObject;
  ValWrk xValWrk;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject,xValWrk) )
    THROW_INVALID_DSTACK("queue.append"_Stz);
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  ASSERT(CN_QUEUE == pObject->eClass());
  
  pQueue->bAppend(xValWrk.pNestedValue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.delete -- delete the queue                                           */
/*----------------------------------------------------------------------------*/
void Task::vQueueDelete(MP_Task mpTask)
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
void Task::vQueueDump(MP_Task mpTask)
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
void Task::vQueueInit(MP_Task mpTask)
{
ENTRY("vQueueInit"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("queue.init"_Stz);
    
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  new(pQueue) Queue;
  
  Word* pQueueWord = Word::pToWord(pQueue);
    
  //pQueue->vLockName(pQueueWord->sName().aStr());

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue::new -- create a named queue                                         */
/*----------------------------------------------------------------------------*/
void Task::vQueueNew(MP_Task mpTask)
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
void Task::vQueuePop(MP_Task mpTask)
{
  ENTRY("vQueuePop"_Stz);
  
  Object* pObject;
  ValWrk xValWrk;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("queue.pop"_Stz);
  
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  
  if (pQueue->bPop(&xValWrk)) {
    mpTask->m_pDataStack->vPushValWrk(xValWrk);
    mpTask->m_pDataStack->vPushInt((i8)1);
  }
  else
    mpTask->m_pDataStack->vPushInt((i8)0);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
