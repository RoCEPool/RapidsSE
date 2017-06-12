/*============================================================================*/
/* Steps_TaskQueue.cpp - task queue step group                               */
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

// TaskQueue: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskQueue(void)
{
  vAddPrimitive("TaskQueue.append"_Str,  &vTaskQueueAppend,    false);
  vAddPrimitive("TaskQueue.dump"_Str,    &vTaskQueueDump,      false);
  vAddPrimitive("TaskQueue.init"_Str,    &vTaskQueueInit,      false);
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.append -- TaskQueue append                                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueAppend(NNP_Task nnpTask)
{
ENTRY("vTaskQueueAppend"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("TaskQueue.append"_Str);
  
  ASSERT(CN_TASKQUEUE == pObject->eClass());
  
  TaskQueue* pTaskQueue = static_cast<TaskQueue*>(pObject);
  
  Object* pTaskObject;
  if ( ! nnpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("TaskQueue.append"_Str);
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskQueue->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.dump -- TaskQueue dump                                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueDump(NNP_Task nnpTask)
{
ENTRY("vTaskQueueDump"_Str);
  
  Object* pObject;
  
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("TaskQueue.dump"_Str);
  
  ASSERT(CN_TASKQUEUE == pObject->eClass());
  
  TaskQueue* pTaskQueue = static_cast<TaskQueue*>(pObject);
  
  pTaskQueue->vContentDump();
  
  EXIT();
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
