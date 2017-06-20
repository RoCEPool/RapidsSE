/*============================================================================*/
/* Steps_Queue.cpp - queue step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
extern Heap*           gpMediumHeap;
extern BPA*            gpBPA;
extern Dic*            gpMainDic;
extern Word*           gpWord;

extern Grid*           gpGrid;
extern StepArray*      gpStepArray;
extern ThreadList*     gpThreadList;
extern TaskList*       gpTaskList;
extern TxCollection*   gpTxCollection;

extern NP_Task         gnpRootTask;
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// queue: //////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Queue(void)
{
  vAddPrimitive("queue.append"_Str,    &vQueueAppend,      false);
  vAddPrimitive("queue.dump"_Str,      &vQueueDump,        false);
  vAddPrimitive("queue.init"_Str,      &vQueueInit,        false);
  vAddPrimitive("queue.pop"_Str,       &vQueuePop,         false);
}

/*----------------------------------------------------------------------------*/
/* queue.append -- append onto queue                                          */
/*----------------------------------------------------------------------------*/
void Task::vQueueAppend(MP_Task mpTask)
{
ENTRY("vQueueAppend"_Str);
  
  Object* pObject;
  ValWrk xValWrk;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject,xValWrk) )
    THROW_INVALID_DSTACK("queue.append"_Str);
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  ASSERT(CN_QUEUE == pObject->eClass());
  
  pQueue->bAppend(xValWrk.pNestedValue());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* q.dump -- queue dump                                                       */
/*----------------------------------------------------------------------------*/
void Task::vQueueDump(MP_Task mpTask)
{
ENTRY("vQueueDump"_Str);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("q.dump"_Str);
  
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
ENTRY("vQueueInit"_Str);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("queue.init"_Str);
    
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  new(pQueue) Queue;
  
  Word* pQueueWord = Word::pToWord(pQueue);
    
  pQueue->vLockName(pQueueWord->sName().aStr());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.pop -- queue pop                                                     */
/*----------------------------------------------------------------------------*/
void Task::vQueuePop(MP_Task mpTask)
{
ENTRY("vQueuePop"_Str);
  
  Object* pObject;
  ValWrk xValWrk;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("queue.pop"_Str);
  
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
