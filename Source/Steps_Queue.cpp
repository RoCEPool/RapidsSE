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
void Task::vQueueAppend(NNP_Task nnpTask)
{
ENTRY("vQueueAppend"_Str);
  
  Object* pObject;
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject,xValWsp) )
    THROW_INVALID_DSTACK("queue.append"_Str);
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  ASSERT(CN_QUEUE == pObject->eClass());
  
  pQueue->bAppend(&xValWsp);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* q.dump -- queue dump                                                       */
/*----------------------------------------------------------------------------*/
void Task::vQueueDump(NNP_Task nnpTask)
{
ENTRY("vQueueDump"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("q.dump"_Str);
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  pQueue->vContentDump(aSingleTab());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.init -- queue init                                                   */
/*----------------------------------------------------------------------------*/
void Task::vQueueInit(NNP_Task nnpTask)
{
ENTRY("vQueueInit"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
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
void Task::vQueuePop(NNP_Task nnpTask)
{
ENTRY("vQueuePop"_Str);
  
  Object* pObject;
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("queue.pop"_Str);
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  
  if (pQueue->bPop(&xValWsp)) {
    nnpTask->m_pDataStack->vPushValWsp(xValWsp);
    nnpTask->m_pDataStack->vPushInt((i8)1);
  }
  else
    nnpTask->m_pDataStack->vPushInt((i8)0);
  
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
