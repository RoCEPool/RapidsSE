/*============================================================================*/
/* Steps_TaskQueue.cpp - task queue step group                                */
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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;

// TaskQueue: //////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_TaskQueue(void)
{
  vAddPrimitive("TaskQueue.append"_Stz,  &vTaskQueueAppend,    false);
  vAddPrimitive("TaskQueue.dump"_Stz,    &vTaskQueueDump,      false);
  vAddPrimitive("TaskQueue.init"_Stz,    &vTaskQueueInit,      false);
  vAddPrimitive("TaskQueue::new"_Stz,    &vTaskQueueNew,       false);
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.append -- TaskQueue append                                       */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueAppend(MP_Task mpTask)
{
ENTRY("vTaskQueueAppend"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("TaskQueue.append"_Stz);
  
  ASSERT(CN_TASKQUEUE == pObject->eClass());
  
  TaskQueue* pTaskQueue = static_cast<TaskQueue*>(pObject);
  
  Object* pTaskObject;
  if ( ! mpTask->m_pDataStack->bPop(pTaskObject) )
    THROW_INVALID_DSTACK("TaskQueue.append"_Stz);
  Task* pThatTask = _pTask(pTaskObject);
  
  pTaskQueue->vAppend(pThatTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.dump -- TaskQueue dump                                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueDump(MP_Task mpTask)
{
ENTRY("vTaskQueueDump"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("TaskQueue.dump"_Stz);
  
  ASSERT(CN_TASKQUEUE == pObject->eClass());
  
  TaskQueue* pTaskQueue = static_cast<TaskQueue*>(pObject);
  
  pTaskQueue->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskQueue.init -- TaskQueue init                                           */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueInit(MP_Task mpTask)
{
ENTRY("vTaskQueueInit"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("TaskQueue.init"_Stz);
  
  ASSERT(CN_TASKQUEUE == pObject->eClass());
  
  TaskQueue* pTaskQueue = static_cast<TaskQueue*>(pObject);
  
  new(pTaskQueue) TaskQueue;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TaskQueue::new -- create new TaskQueue                                     */
/*----------------------------------------------------------------------------*/
void Task::vTaskQueueNew(MP_Task mpTask)
{
  ENTRY("vTaskQueueNew"_Stz);

  TaskQueue* mpTaskQueue = new TaskQueue;
  
  mpTask->m_pDataStack->vPushObject(mpTaskQueue);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
