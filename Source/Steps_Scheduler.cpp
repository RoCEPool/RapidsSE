/*============================================================================*/
/* Steps_Scheduler.cpp - task queue step group                               */
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
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern Scheduler*      gpScheduler;

extern i8 giSchEnqueueTask;
extern i8 giSchEnqueueTaskWait;
extern i8 giSchGetTask;
extern i8 giSchGetTaskWait;

// Scheduler: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Scheduler(void)
{
  vAddPrimitive("Scheduler::enqueueServerTask"_Str, &vSchedulerEnqueueTask,   false);
  vAddPrimitive("Scheduler::queuedTasks"_Str,       &vSchedulerQueuedTasks,   false);
  vAddPrimitive("Scheduler::queuedThreads"_Str,     &vSchedulerQueuedThreads, false);
  
  vAddPrimitive("Scheduler::List"_Str,              &vSchedulerList,          false);
  vAddPrimitive("Scheduler::Reset"_Str,             &vSchedulerReset,         false);
}

/*----------------------------------------------------------------------------*/
/* Scheduler.queueTask -- queue task onto given scheduler                     */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerEnqueueTask(MP_Task mpTask)
{
  ENTRY("vSchedulerEnqueueTask"_Str);
  
  Object* pObject;
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("Scheduler.enqueueTask"_Str);
  Task* pTask = _pTask(pObject);
  
  gpScheduler->vEnqueueTask_Mutex(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler.queuedTasks -- get number of queued tasks in given scheduler     */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedTasks(MP_Task mpTask)
{
  ENTRY("vSchedulerQueuedTasks"_Str);
  
  i8 nTasks = gpScheduler->iQueuedTasks();
  
  mpTask->m_pDataStack->vPushInt(nTasks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler.queuedThreads -- get count of threads waiting in scheduler       */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedThreads(MP_Task mpTask)
{
  ENTRY("vSchedulerQueuedThreads"_Str);
  
  i8 nThreads = gpScheduler->iQueuedThreads();
  
  mpTask->m_pDataStack->vPushInt(nThreads);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* scheduler.reset -- reset scheduler statistics                              */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerReset(MP_Task mpTask)
{
  ENTRY("vSchedulerReset"_Str);
  
  giSchEnqueueTask = 0;
  giSchEnqueueTaskWait = 0;
  giSchGetTask = 0;
  giSchGetTaskWait = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* schedulery.list-- trace scheduler statistics                               */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerList(MP_Task mpTask)
{
  ENTRY("vSchedulerList"_Str);
  
  vTrace("SchEnqueueTask %lld\n"_Str,giSchEnqueueTask);
  vTrace("SchEnqueueTaskWait %lld\n"_Str,giSchEnqueueTaskWait);
  vTrace("SchGetTask %lld\n"_Str,giSchGetTask);
  vTrace("SchGetTaskWait %lld\n"_Str,giSchGetTaskWait);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
