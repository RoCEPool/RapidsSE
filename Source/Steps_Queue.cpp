/*============================================================================*/
/* Steps_Queue.cpp - queue step group                                         */
/*============================================================================*/

#include "StorageEngine.h"
//#include "Tasks.h"
#include "SePrompt.hpp"

extern Arguments       gxArguments;

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
  vAddPrimitive("queue.append",    &vQueueAppend,      false);
  vAddPrimitive("queue.dump",      &vQueueDump,        false);
  vAddPrimitive("queue.init",      &vQueueInit,        false);
  vAddPrimitive("queue.pop",       &vQueuePop,         false);
}

/*----------------------------------------------------------------------------*/
/* queue.append -- append onto queue                                          */
/*----------------------------------------------------------------------------*/
void Task::vQueueAppend(NNP_Task nnpTask)
{
ENTRY("vQueueAppend");
  
  Object* pObject;
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject,xValWsp) )
    THROW_INVALID_DSTACK("queue.append");
  
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
ENTRY("vQueueDump");
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("q.dump");
  
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
ENTRY("vQueueInit");
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("queue.init");
    
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  
  new(pQueue) Queue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* queue.pop -- queue pop                                                     */
/*----------------------------------------------------------------------------*/
void Task::vQueuePop(NNP_Task nnpTask)
{
ENTRY("vQueuePop");
  
  Object* pObject;
  ValWsp xValWsp;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("queue.pop");
  
  ASSERT(CN_QUEUE == pObject->eClass());
  
  Queue* pQueue = static_cast<Queue*>(pObject);
  ASSERT(pQueue);
  
  if (pQueue->bPop(&xValWsp)) {
    nnpTask->m_pDataStack->vPush(&xValWsp);
    nnpTask->m_pDataStack->vPush((i8)1);
  }
  else
    nnpTask->m_pDataStack->vPush((i8)0);
  
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
