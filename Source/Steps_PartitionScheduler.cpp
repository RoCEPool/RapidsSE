/*============================================================================*/
/* Steps_ServerScheduler.cpp - server scheduler step group                    */
/*============================================================================*/

#include "StorageEngine.h"
#include "Schedulers.h"
#include "SePrompt.hpp"

extern AttachMap*      gpAttachMap;

// ParritionScheduler: /////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_PartitionScheduler(void)
{
  vAddPrimitive("PartitionScheduler.enqueueTask"_Stz,
                &vPartitionSchedulerEnqueueTask, false);
  
  vAddPrimitive("PartitionScheduler.queueLength@"_Stz,
                &vPartitionSchedulerQueueLengthFetch, false);
  
  vAddPrimitive("PartitionScheduler.semaphore!"_Stz,
                &vPartitionSchedulerSemaphoreStore, false);
  
  vAddPrimitive("PartitionScheduler.taskFifo!"_Stz,
                &vPartitionSchedulerTaskFifoStore, false);

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
    THROW_INVALID_DSTACK("PartitionScheduler.enqueueTask"_Stz);

  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.enqueueTask"_Stz);
  Task* pTask = _pTask(pObject);
  
  pTask->m_iPartition = iPtnNo;
  
  pPartitionScheduler->vEnqueueTask_Mutex(iPtnNo,pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PartitionScheduler.queueLength@ -- task queue fetch                        */
/*----------------------------------------------------------------------------*/
void Task::vPartitionSchedulerQueueLengthFetch(MP_Task mpTask)
{
  ENTRY("vPartitionSchedulerQueueLengthFetch"_Stz);
  
  i8 iPtnNo;
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.queueLength@"_Stz);
  PartitionScheduler* pPartitionScheduler = _pPartitionScheduler(pObject);
  
  if ( ! mpTask->m_pDataStack->bPop(iPtnNo) )
    THROW_INVALID_DSTACK("PartitionScheduler.queueLength"_Stz);
  
  mpTask->m_pDataStack->vPushInt(pPartitionScheduler->iTaskFifoLength(iPtnNo));
  
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
/* PartitionScheduler.taskFifo! -- task fifo store                            */
/*----------------------------------------------------------------------------*/
void Task::vPartitionSchedulerTaskFifoStore(MP_Task mpTask)
{
  ENTRY("vPartitionSchedulerTaskFifoStore"_Stz);
  
  i8 iPtnNo;
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.taskFifo!"_Stz);
  PartitionScheduler* pPartitionScheduler = _pPartitionScheduler(pObject);
  
  if ( ! mpTask->m_pDataStack->bPop(iPtnNo) )
    THROW_INVALID_DSTACK("PartitionScheduler.taskFifo!"_Stz);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("PartitionScheduler.taskFifo!"_Stz);
  TaskFifo* pTaskFifo = _pTaskFifo(pObject);
  
  pPartitionScheduler->vTaskFifo(iPtnNo,pTaskFifo);

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
