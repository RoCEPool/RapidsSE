/*============================================================================*/
/* Steps_ServerScheduler.cpp - server scheduler step group                    */
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
  
  vAddPrimitive("PartitionScheduler.taskQueue!"_Stz,
                &vPartitionSchedulerTaskQueueStore, false);
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
  
  pPartitionScheduler->vEnqueueTask_Mutex(iPtnNo,pTask);
  
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
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
