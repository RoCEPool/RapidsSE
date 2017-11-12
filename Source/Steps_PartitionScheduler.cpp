/*============================================================================*/
/* Steps_ServerScheduler.cpp - server scheduler step group                    */
/*============================================================================*/

#include "StorageEngine.h"
#include "Schedulers.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;
extern BufferPool*     gpBufferPool;
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
extern PartitionScheduler* gpPartitionScheduler;
extern ServerScheduler* gpServerScheduler;

// ParritionScheduler: /////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_PartitionScheduler(void)
{
  vAddPrimitive("PartitionScheduler.enqueueTask"_Stz,
                &vPartitionSchedulerEnqueueTask, false);
  
  vAddPrimitive("PartitionScheduler.semaphore!"_Stz,
                &vPartitionSchedulerSemaphoreStore, false);
  
  vAddPrimitive("PartitionScheduler.taskQueue!"_Stz,
                &vPartitionSchedulerTaskQueueStore, false);

  vAddPrimitive("PartitionScheduler.threadQueue!"_Stz,
                &vPartitionSchedulerThreadQueueStore, false);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* PartitionScheduler.enqueueTask -- enqueue partition task                   */
/*----------------------------------------------------------------------------*/
void Task::vPartitionSchedulerEnqueueTask(MP_Task mpTask)
{
  ENTRY("vPartitionSchedulerEnqueueTask"_Stz);
  
  i8 iPtnNo;
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.enqueueTask"_Stz);
  PartitionScheduler* pPartitionScheduler = _pPartitionScheduler(pObject);
  
  if ( ! mpTask->m_pDataStack->bPop(iPtnNo) )
    THROW_INVALID_DSTACK("PartitionScheduler.taskQueueStore"_Stz);

  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.enqueueTask"_Stz);
  Task* pTask = _pTask(pObject);
  
  pTask->m_iPartition = iPtnNo;
  
  pPartitionScheduler->vEnqueueTask_Mutex(iPtnNo,pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PartitionScheduler.semaphoreStore -- semaphore store                       */
/*----------------------------------------------------------------------------*/
void Task::vPartitionSchedulerSemaphoreStore(MP_Task mpTask)
{
  ENTRY("vPartitionSchedulerSemaphoreStore"_Stz);
  
  i8 iPtnNo;
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.semaphoreStore"_Stz);
  PartitionScheduler* pPartitionScheduler = _pPartitionScheduler(pObject);
  
  if ( ! mpTask->m_pDataStack->bPop(iPtnNo) )
    THROW_INVALID_DSTACK("PartitionScheduler.semaphoreStore"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.semaphoreStore"_Stz);
  Semaphore* pSemaphore = _pSemaphore(pObject);
  
  pPartitionScheduler->vSempahore(iPtnNo,pSemaphore);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PartitionScheduler.taskQueueStore -- task queue store                      */
/*----------------------------------------------------------------------------*/
void Task::vPartitionSchedulerTaskQueueStore(MP_Task mpTask)
{
  ENTRY("vPartitionSchedulerTaskQueueStore"_Stz);
  
  i8 iPtnNo;
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.taskQueueStore"_Stz);
  PartitionScheduler* pPartitionScheduler = _pPartitionScheduler(pObject);
  
  if ( ! mpTask->m_pDataStack->bPop(iPtnNo) )
    THROW_INVALID_DSTACK("PartitionScheduler.taskQueueStore"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.taskQueueStore"_Stz);
  TaskQueue* pTaskQueue = _pTaskQueue(pObject);
  
  pPartitionScheduler->vTaskQueue(iPtnNo,pTaskQueue);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PartitionScheduler.threadQueueStore -- thread queue store                  */
/*----------------------------------------------------------------------------*/
void Task::vPartitionSchedulerThreadQueueStore(MP_Task mpTask)
{
  ENTRY("vPartitionSchedulerThreadQueueStore"_Stz);
  
  i8 iPtnNo;
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.threadQueueStore"_Stz);
  PartitionScheduler* pPartitionScheduler = _pPartitionScheduler(pObject);
  
  if ( ! mpTask->m_pDataStack->bPop(iPtnNo) )
    THROW_INVALID_DSTACK("PartitionScheduler.threadQueueStore"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.threadQueueStore"_Stz);
  ThreadQueue* pThreadQueue = _pThreadQueue(pObject);
  
  pPartitionScheduler->vThreadQueue(iPtnNo,pThreadQueue);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
