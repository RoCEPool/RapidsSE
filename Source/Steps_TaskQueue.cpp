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

// TaskQueue: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskQueue(void)
{
  vAddPrimitive("TaskQueue.init"_Str,           &vTaskQueueInit,           false);

  vAddPrimitive("TaskQueue.dropLock"_Str,       &vTaskQueueDropLock,       false);
  vAddPrimitive("TaskQueue.grabLock"_Str,       &vTaskQueueGrabLock,       false);
  
  vAddPrimitive("TaskQueue.queueTask"_Str,      &vTaskQueueQueueTask,      false);
  vAddPrimitive("TaskQueue.queuedTasks"_Str,    &vTaskQueueQueuedTasks,    false);
  
  vAddPrimitive("TaskQueue.threadsWaiting"_Str, &vTaskQueueThreadsWaiting, false);
  vAddPrimitive("TaskQueue.queueThread"_Str,    &vTaskQueueQueueThread,    false);
  vAddPrimitive("TaskQueue.signalThread"_Str,   &vTaskQueueSignalThread,   false);
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.init -- TaskQueue init                                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueInit(NNP_Task nnpTask)
{
ENTRY("vTaskQueueInit"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("TaskQueue.init"_Str);
  
  ASSERT(CN_TASKQUEUE == pObject->eClass());
  
  TaskQueue* pTaskQueue = static_cast<TaskQueue*>(pObject);
  
  new(pTaskQueue) TaskQueue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.dropLock -- drop lock of given task queue                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueDropLock(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueDropLock"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("TaskQueue.append"_Str);
  
  TaskQueue* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.grabLock -- grab lock of given task queue                        */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueGrabLock(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueGrabLock"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("TaskQueue.append"_Str);
  
  TaskQueue* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.queueTask -- queue task onto given task queue                    */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueQueueTask(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueQueueTask"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("TaskQueue.append"_Str);
  
  TaskQueue* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.queuedTasks -- get number of queued tasks in given task queue    */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueQueuedTasks(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueQueuedTasks"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("TaskQueue.append"_Str);
  
  TaskQueue* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.threadsWaiting -- get count of threads waiting in given task que */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueThreadsWaiting(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueThreadsWaiting"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("TaskQueue.append"_Str);
  
  TaskQueue* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.queueThread -- queue thread onto give task queue                 */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueQueueThread(NNP_Task nnpTask)
{
  ENTRY("vTaskQueueQueueThread"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("TaskQueue.append"_Str);
  
  TaskQueue* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.signalThread -- pop task and thread and signal thread            */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueSignalThread(NNP_Task nnpTask)
{
  ENTRY("vTaskQueuePopThread"_Str);
  
  Object* pTaskQueuePtr;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskQueuePtr) )
    THROW_INVALID_DSTACK("TaskQueue.append"_Str);
  
  TaskQueue* pTaskQueue = _pTaskQueue(pTaskQueuePtr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
