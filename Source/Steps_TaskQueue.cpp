/*============================================================================*/
/* Steps_TaskQueue.cpp - task queue step group                               */
/*============================================================================*/

#include "StorageEngine.h"
#include "Schedulers.h"
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

// QueryScheduler: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskQueue(void)
{
  vAddPrimitive("QueryScheduler.init"_Str,           &vTaskQueueInit,           false);

  vAddPrimitive("QueryScheduler.dropLock"_Str,       &vTaskQueueDropLock,       false);
  vAddPrimitive("QueryScheduler.grabLock"_Str,       &vTaskQueueGrabLock,       false);
  
  vAddPrimitive("QueryScheduler.queueTask"_Str,      &vTaskQueueQueueTask,      false);
  vAddPrimitive("QueryScheduler.queuedTasks"_Str,    &vTaskQueueQueuedTasks,    false);
  
  vAddPrimitive("QueryScheduler.threadsWaiting"_Str, &vTaskQueueThreadsWaiting, false);
  vAddPrimitive("QueryScheduler.queueThread"_Str,    &vTaskQueueQueueThread,    false);
  vAddPrimitive("QueryScheduler.signalThread"_Str,   &vTaskQueueSignalThread,   false);
}

/*----------------------------------------------------------------------------*/
/* QueryScheduler.init -- QueryScheduler init                                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueInit(NNP_Task nnpTask)
{
ENTRY("vTaskQueueInit"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("QueryScheduler.init"_Str);
  
  ASSERT(CN_TASKQUEUE == pObject->eClass());
  
  QueryScheduler* pTaskQueue = static_cast<QueryScheduler*>(pObject);
  
  new(pTaskQueue) QueryScheduler;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* QueryScheduler.dropLock -- drop lock of given task queue                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueDropLock(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueDropLock"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("QueryScheduler.append"_Str);
  
  QueryScheduler* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* QueryScheduler.grabLock -- grab lock of given task queue                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueGrabLock(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueGrabLock"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("QueryScheduler.append"_Str);
  
  QueryScheduler* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* QueryScheduler.queueTask -- queue task onto given task queue                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueQueueTask(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueQueueTask"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("QueryScheduler.append"_Str);
  
  QueryScheduler* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* QueryScheduler.queuedTasks -- get number of queued tasks in given task queue    */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueQueuedTasks(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueQueuedTasks"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("QueryScheduler.append"_Str);
  
  QueryScheduler* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* QueryScheduler.threadsWaiting -- get count of threads waiting in given task que */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueThreadsWaiting(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueThreadsWaiting"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("QueryScheduler.append"_Str);
  
  QueryScheduler* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* QueryScheduler.queueThread -- queue thread onto give task queue                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueQueueThread(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueQueueThread"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("QueryScheduler.append"_Str);
  
  QueryScheduler* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* QueryScheduler.signalThread -- pop task and thread and signal thread            */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueSignalThread(NNP_Task nnpTask)
{
  ENTRY("vTaskQueuePopThread"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("QueryScheduler.append"_Str);
  
  QueryScheduler* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
