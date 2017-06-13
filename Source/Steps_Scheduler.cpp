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

// Scheduler: ////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Task::vAddPrimitives_Scheduler(void)
{
  vAddPrimitive("Scheduler.init"_Str,          &vSchedulerInit,          false);

  vAddPrimitive("Scheduler.dropLock"_Str,      &vSchedulerDropLock,      false);
  vAddPrimitive("Scheduler.grabLock"_Str,      &vSchedulerGrabLock,      false);
  
  vAddPrimitive("Scheduler.enqueueTask"_Str,   &vSchedulerEnqueueTask,   false);
  vAddPrimitive("Scheduler.queuedTasks"_Str,   &vSchedulerQueuedTasks,   false);
  
  vAddPrimitive("Scheduler.enqueueThread"_Str, &vSchedulerEnqueueThread, false);
  vAddPrimitive("Scheduler.queuedThreads"_Str, &vSchedulerQueuedThreads, false);
}

/*----------------------------------------------------------------------------*/
/* Scheduler.init -- Scheduler init                                           */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerInit(NNP_Task nnpTask)
{
ENTRY("vSchedulerInit"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("Scheduler.init"_Str);
  
  Scheduler* pScheduler = _pScheduler(pObject);
  
  new(pScheduler) Scheduler;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler.dropLock -- drop lock of given scheduler                         */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerDropLock(NNP_Task nnpTask)
{
  ENTRY("vSchedulerDropLock"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("Scheduler.append"_Str);
  
  Scheduler* pScheduler = _pScheduler(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler.grabLock -- grab lock of given scheduler                         */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerGrabLock(NNP_Task nnpTask)
{
  ENTRY("vSchedulerGrabLock"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("Scheduler.append"_Str);
  
  Scheduler* pScheduler = _pScheduler(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler.queueTask -- queue task onto given scheduler                     */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerEnqueueTask(NNP_Task nnpTask)
{
  ENTRY("vSchedulerEnqueueTask"_Str);
  
  Object* pObject;
  if ( ! nnpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("Scheduler.enqueueThread"_Str);
  
  Scheduler* pScheduler = _pScheduler(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler.queuedTasks -- get number of queued tasks in given scheduler     */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedTasks(NNP_Task nnpTask)
{
  ENTRY("vSchedulerQueuedTasks"_Str);
  
  Object* pSchedulerPtr;
  if ( ! nnpTask->m_pDataStack->bPop(pSchedulerPtr) )
    THROW_INVALID_DSTACK("Scheduler.queuedTasks"_Str);
  
  Scheduler* pScheduler = _pScheduler(pSchedulerPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler.queueThread -- queue thread via scheduler                        */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerEnqueueThread(NNP_Task nnpTask)
{
  ENTRY("vSchedulerEnqueueThread"_Str);
  
  Object* pSchedulerPtr;
  if ( ! nnpTask->m_pDataStack->bPop(pSchedulerPtr) )
    THROW_INVALID_DSTACK("Scheduler.enqueueThread"_Str);
  
  Scheduler* pScheduler = _pScheduler(pSchedulerPtr);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler.queuedThreads -- get count of threads waiting in scheduler       */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedThreads(NNP_Task nnpTask)
{
  ENTRY("vSchedulerQueuedThreads"_Str);
  
  Object* pSchedulerPtr;
  if ( ! nnpTask->m_pDataStack->bPop(pSchedulerPtr) )
    THROW_INVALID_DSTACK("Scheduler.queuedThreads"_Str);
  
  Scheduler* pScheduler = _pScheduler(pSchedulerPtr);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
