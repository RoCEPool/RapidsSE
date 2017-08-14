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

extern NP_Task         gnpBootTask;
extern Lock            gxDdlLock;
extern Lock            gxDicLock;
extern SEitfList*      gpSEitfList;
extern ServerScheduler* gpServerScheduler;

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
  vAddPrimitive("Scheduler::list"_Stz,              &vSchedulerList,          false);
  vAddPrimitive("Scheduler::queuedTasks"_Stz,       &vSchedulerQueuedTasks,   false);
  vAddPrimitive("Scheduler::queuedThreads"_Stz,     &vSchedulerQueuedThreads, false);
  vAddPrimitive("Scheduler::reset"_Stz,             &vSchedulerReset,         false);
  vAddPrimitive("Scheduler::server"_Stz,            &vSchedulerServer,        false);

  vAddPrimitive("Scheduler.enqueueServerTask"_Stz, &vSchedulerEnqueueServerTask, false);
}

/*----------------------------------------------------------------------------*/
/* Scheduler::list -- trace scheduler statistics                              */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerList(MP_Task mpTask)
{
  ENTRY("vSchedulerList"_Stz);
  
  vTrace("SchEnqueueTask %lld\n"_Stz,giSchEnqueueTask);
  vTrace("SchEnqueueTaskWait %lld\n"_Stz,giSchEnqueueTaskWait);
  vTrace("SchGetTask %lld\n"_Stz,giSchGetTask);
  vTrace("SchGetTaskWait %lld\n"_Stz,giSchGetTaskWait);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler::queuedTasks -- get number of queued tasks in given scheduler    */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedTasks(MP_Task mpTask)
{
  ENTRY("vSchedulerQueuedTasks"_Stz);
  
  i8 nTasks = gpServerScheduler->iQueuedTasks();
  
  mpTask->m_pDataStack->vPushInt(nTasks);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler::queuedThreads -- get count of threads waiting in scheduler      */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerQueuedThreads(MP_Task mpTask)
{
  ENTRY("vSchedulerQueuedThreads"_Stz);
  
  i8 nThreads = gpServerScheduler->iQueuedThreads();
  
  mpTask->m_pDataStack->vPushInt(nThreads);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* scheduler::reset -- reset scheduler statistics                             */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerReset(MP_Task mpTask)
{
  ENTRY("vSchedulerReset"_Stz);
  
  giSchEnqueueTask = 0;
  giSchEnqueueTaskWait = 0;
  giSchGetTask = 0;
  giSchGetTaskWait = 0;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Scheduler::server -- push server scheduler                                 */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerServer(MP_Task mpTask)
{
  ENTRY("vSchedulerServer"_Stz);
  
  mpTask->m_pDataStack->vPushObject(gpServerScheduler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Scheduler.enqueueServerTask -- enqueue server task                         */
/*----------------------------------------------------------------------------*/
void Task::vSchedulerEnqueueServerTask(MP_Task mpTask)
{
  ENTRY("vSchedulerEnqueueTask"_Stz);
  
  Object* pObject;
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("Scheduler.enqueueTask"_Stz);
  ServerScheduler* pServerScheduler = _pServerScheduler(pObject);
  
  if ( ! mpTask->m_pDataStack->bPop(pObject) )
    THROW_INVALID_DSTACK("Scheduler.enqueueTask"_Stz);
  Task* pTask = _pTask(pObject);
  
  pServerScheduler->vEnqueueTask_Mutex(pTask);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
