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
extern StepBoard*      gpStepBoard;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern Scheduler*      gpScheduler;

// Scheduler: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Scheduler(void)
{
  vAddPrimitive("Scheduler::enqueueTask"_Str,   &vSchedulerEnqueueTask,   false);
  vAddPrimitive("Scheduler::queuedTasks"_Str,   &vSchedulerQueuedTasks,   false);
  vAddPrimitive("Scheduler::queuedThreads"_Str, &vSchedulerQueuedThreads, false);
}

/*----------------------------------------------------------------------------*/
/* Scheduler.queueTask -- queue task onto given scheduler                     */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerEnqueueTask(NNP_Task nnpTask)
{
  ENTRY("vSchedulerEnqueueTask"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("Scheduler.enqueueTask"_Str);
  Task* pTask = _pTask(pObject);
  
  gpScheduler->vEnqueueTask(pTask);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler.queuedTasks -- get number of queued tasks in given scheduler     */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedTasks(NNP_Task nnpTask)
{
  ENTRY("vSchedulerQueuedTasks"_Str);
  
  i8 nTasks = gpScheduler->iQueuedTasks();
  
  nnpTask->m_pDataStack->vPushInt(nTasks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler.queuedThreads -- get count of threads waiting in scheduler       */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedThreads(NNP_Task nnpTask)
{
  ENTRY("vSchedulerQueuedThreads"_Str);
  
  i8 nThreads = gpScheduler->iQueuedThreads();
  
  nnpTask->m_pDataStack->vPushInt(nThreads);
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
